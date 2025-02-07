// WARNING: This file is automatically generated. Any changes will be lost.
#include "twitch-eventsub-ws/chrono.hpp"  // IWYU pragma: keep
#include "twitch-eventsub-ws/detail/errors.hpp"
#include "twitch-eventsub-ws/payloads/stream-offline-v1.hpp"

#include <boost/json.hpp>

namespace chatterino::eventsub::lib::payload::stream_offline::v1 {

boost::json::result_for<Event, boost::json::value>::type tag_invoke(
    boost::json::try_value_to_tag<Event> /* tag */,
    const boost::json::value &jvRoot)
{
    if (!jvRoot.is_object())
    {
        EVENTSUB_BAIL_HERE(error::Kind::ExpectedObject);
    }
    const auto &root = jvRoot.get_object();

    const auto *jvbroadcasterUserID = root.if_contains("broadcaster_user_id");
    if (jvbroadcasterUserID == nullptr)
    {
        EVENTSUB_BAIL_HERE(error::Kind::FieldMissing);
    }

    auto broadcasterUserID =
        boost::json::try_value_to<std::string>(*jvbroadcasterUserID);

    if (broadcasterUserID.has_error())
    {
        return broadcasterUserID.error();
    }

    const auto *jvbroadcasterUserLogin =
        root.if_contains("broadcaster_user_login");
    if (jvbroadcasterUserLogin == nullptr)
    {
        EVENTSUB_BAIL_HERE(error::Kind::FieldMissing);
    }

    auto broadcasterUserLogin =
        boost::json::try_value_to<std::string>(*jvbroadcasterUserLogin);

    if (broadcasterUserLogin.has_error())
    {
        return broadcasterUserLogin.error();
    }

    const auto *jvbroadcasterUserName =
        root.if_contains("broadcaster_user_name");
    if (jvbroadcasterUserName == nullptr)
    {
        EVENTSUB_BAIL_HERE(error::Kind::FieldMissing);
    }

    auto broadcasterUserName =
        boost::json::try_value_to<std::string>(*jvbroadcasterUserName);

    if (broadcasterUserName.has_error())
    {
        return broadcasterUserName.error();
    }

    return Event{
        .broadcasterUserID = std::move(broadcasterUserID.value()),
        .broadcasterUserLogin = std::move(broadcasterUserLogin.value()),
        .broadcasterUserName = std::move(broadcasterUserName.value()),
    };
}

boost::json::result_for<Payload, boost::json::value>::type tag_invoke(
    boost::json::try_value_to_tag<Payload> /* tag */,
    const boost::json::value &jvRoot)
{
    if (!jvRoot.is_object())
    {
        EVENTSUB_BAIL_HERE(error::Kind::ExpectedObject);
    }
    const auto &root = jvRoot.get_object();

    const auto *jvsubscription = root.if_contains("subscription");
    if (jvsubscription == nullptr)
    {
        EVENTSUB_BAIL_HERE(error::Kind::FieldMissing);
    }

    auto subscription =
        boost::json::try_value_to<subscription::Subscription>(*jvsubscription);

    if (subscription.has_error())
    {
        return subscription.error();
    }

    const auto *jvevent = root.if_contains("event");
    if (jvevent == nullptr)
    {
        EVENTSUB_BAIL_HERE(error::Kind::FieldMissing);
    }

    auto event = boost::json::try_value_to<Event>(*jvevent);

    if (event.has_error())
    {
        return event.error();
    }

    return Payload{
        .subscription = std::move(subscription.value()),
        .event = std::move(event.value()),
    };
}

}  // namespace chatterino::eventsub::lib::payload::stream_offline::v1
