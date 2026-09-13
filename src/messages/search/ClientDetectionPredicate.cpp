#include "messages/search/ClientDetectionPredicate.hpp"

#include "messages/search/MessagePredicate.hpp"
namespace chatterino {

ClientDetectionPredicate::ClientDetectionPredicate(const QString &clientType,
                                                   bool negate)
    : MessagePredicate(negate)
{
    QString client = clientType.toLower();
    if (client == u"web"_qs || client == u"webchat"_qs)
    {
        this->type = Message::ClientDetectionStatus::Webchat;
    }
    else if (client == u"android"_qs)
    {
        this->type = Message::ClientDetectionStatus::Android;
    }
    else if (client == u"ios"_qs || client == u"iphone"_qs)
    {
        this->type = Message::ClientDetectionStatus::IOS;
    }
    else if (client == u"abnormal"_qs)
    {
        this->type = Message::ClientDetectionStatus::Abnormal;
    }
    else if (client == u"unknown"_qs || client == u"empty"_qs ||
             client == u"no"_qs)
    {
        this->type = Message::ClientDetectionStatus::Unknown;
    }
}

bool ClientDetectionPredicate::appliesToImpl(const Message &message)
{
    return message.clientDetection == this->type;
}

}  // namespace chatterino
