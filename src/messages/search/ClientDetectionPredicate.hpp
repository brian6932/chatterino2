#pragma once
#include "messages/Message.hpp"
#include "messages/search/MessagePredicate.hpp"
namespace chatterino {

class ClientDetectionPredicate : public MessagePredicate
{
    Message::ClientDetectionStatus type;

public:
    ClientDetectionPredicate(const QString &clientType, bool negate);

protected:
    bool appliesToImpl(const Message &message) override;
};
}  // namespace chatterino
