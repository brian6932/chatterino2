#include "Toasts.hpp"

#include "Application.hpp"
#include "common/DownloadManager.hpp"
#include "common/NetworkRequest.hpp"
#include "controllers/notifications/NotificationController.hpp"
#include "providers/twitch/TwitchChannel.hpp"
#include "providers/twitch/TwitchCommon.hpp"
#include "providers/twitch/TwitchServer.hpp"

#ifdef Q_OS_WIN

#include <wintoastlib.h>

#endif

#include <QDesktopServices>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

#include <cstdlib>

namespace chatterino {

bool Toasts::isEnabled()
{
    return WinToastLib::WinToast::isCompatible() &&
           getApp()->settings->notificationToast;
}

void Toasts::sendChannelNotification(const QString &channelName, Platform p)
{
    // Fetch user profile avatar
    if (p == Platform::Twitch) {
        QFileInfo check_file(
            QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
            "2/cache/profileAvatars/twitch/" + channelName + ".png");
        if (check_file.exists() && check_file.isFile()) {
#ifdef Q_OS_WIN
            this->sendWindowsNotification(channelName, p);
#endif
            // OSX

            // LINUX

        } else {
            this->fetchChannelAvatar(
                channelName, [this, channelName, p](QString avatarLink) {
                    DownloadManager *manager = new DownloadManager();
                    manager->setFile(avatarLink, channelName);
                    manager->connect(
                        manager, &DownloadManager::downloadComplete,
                        [this, channelName, p]() {
#ifdef Q_OS_WIN
                            this->sendWindowsNotification(channelName, p);
#endif
                            // OSX

                            // LINUX
                        });
                });
        }
    }
    return;
}

#ifdef Q_OS_WIN

class CustomHandler : public WinToastLib::IWinToastHandler
{
private:
    QString channelName_;
    Platform platform_;

public:
    CustomHandler(QString channelName, Platform p)
        : channelName_(channelName)
        , platform_(p)
    {
    }
    void toastActivated() const
    {
        QString link;
        if (platform_ == Platform::Twitch) {
            link = "http://www.twitch.tv/" + channelName_;
        }
        QDesktopServices::openUrl(QUrl(link));
    }

    void toastActivated(int actionIndex) const
    {
    }

    void toastFailed() const
    {
    }

    void toastDismissed(WinToastDismissalReason state) const
    {
    }
};

void Toasts::sendWindowsNotification(const QString &channelName, Platform p)
{
    WinToastLib::WinToastTemplate templ = WinToastLib::WinToastTemplate(
        WinToastLib::WinToastTemplate::ImageAndText02);
    QString str = channelName + " is live!";
    std::string utf8_text = str.toUtf8().constData();
    std::wstring widestr = std::wstring(utf8_text.begin(), utf8_text.end());

    templ.setTextField(widestr, WinToastLib::WinToastTemplate::FirstLine);
    templ.setTextField(L"Click here to open in browser",
                       WinToastLib::WinToastTemplate::SecondLine);
    QString Path;
    if (p == Platform::Twitch) {
        Path = getApp()->paths->cacheDirectory() + "/" +
               "profileAvatars/twitch/" + channelName + ".png";
    }
    std::string temp_Utf8 = Path.toUtf8().constData();
    std::wstring imagePath = std::wstring(temp_Utf8.begin(), temp_Utf8.end());
    templ.setImagePath(imagePath);
    if (getApp()->settings->notificationPlaySound) {
        templ.setAudioOption(
            WinToastLib::WinToastTemplate::AudioOption::Silent);
    }
    WinToastLib::WinToast::instance()->setAppName(L"Chatterino2");
    int mbstowcs(wchar_t * aumi_version, const char *CHATTERINO_VERSION,
                 size_t size);
    std::string(CHATTERINO_VERSION);
    std::wstring aumi_version =
        std::wstring(CHATTERINO_VERSION.begin(), CHATTERINO_VERSION.end());
    WinToastLib::WinToast::instance()->setAppUserModelId(
        WinToastLib::WinToast::configureAUMI(L"", L"Chatterino 2", L"",
                                             aumi_version));
    WinToastLib::WinToast::instance()->initialize();
    WinToastLib::WinToast::instance()->showToast(
        templ, new CustomHandler(channelName, p));
}

#endif

void Toasts::fetchChannelAvatar(const QString channelName,
                                std::function<void(QString)> successCallback)
{
    QString requestUrl("https://api.twitch.tv/kraken/users?login=" +
                       channelName);

    NetworkRequest request(requestUrl);
    request.setCaller(QThread::currentThread());
    request.makeAuthorizedV5(getDefaultClientID());
    request.setTimeout(30000);
    request.onSuccess([successCallback](auto result) mutable -> Outcome {
        auto root = result.parseJson();
        if (!root.value("users").isArray()) {
            Log("API Error while getting user id, users is not an array");
            successCallback("");
            return Failure;
        }
        auto users = root.value("users").toArray();
        if (users.size() != 1) {
            Log("API Error while getting user id, users array size is not 1");
            successCallback("");
            return Failure;
        }
        if (!users[0].isObject()) {
            Log("API Error while getting user id, first user is not an object");
            successCallback("");
            return Failure;
        }
        auto firstUser = users[0].toObject();
        auto avatar = firstUser.value("logo");
        if (!avatar.isString()) {
            Log("API Error: while getting user avatar, first user object "
                "`avatar` key "
                "is not a "
                "string");
            successCallback("");
            return Failure;
        }
        successCallback(avatar.toString());
        return Success;
    });

    request.execute();
}
}  // namespace chatterino
