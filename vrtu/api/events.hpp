#ifndef VRTU_EVENT_HPP
#define VRTU_EVENT_HPP

#include <chrono>
#include <vector>
#include "apimessage.hpp"
#include "vrtu/datamodel/id.hpp"

namespace VRTU
{
    class Event : public ApiMessage
    {
    public:
        enum EventType
        {
            ET_SERVER_STARTED,
            ET_SERVER_STOPPED,
            ET_PEER_CONNECTED,
            ET_CONNECTION_ACTIVE,
            ET_CONNECTION_PASSIVE,
            ET_PEER_DISCONNECTED,
            ET_APDU_SENT,
            ET_APDU_RECEIVED,
        };

        EventType type() const noexcept { return mType; }
        std::chrono::milliseconds creationTime() const noexcept { return mCreationTime; }

    protected:
        explicit Event(EventType aType) noexcept
            : ApiMessage(ApiMessage::CAT_EVENT),
              mType(aType),
              mCreationTime(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch())
        {
        }

    private:
        EventType mType;
        std::chrono::milliseconds mCreationTime;
    };

    class EventServerStarted : public Event
    {
    public:
        EventServerStarted(const Id& aId, const std::string& arIp, int aPort)
            : Event(ET_SERVER_STARTED),
              mIp(arIp), mId(aId), mPort(aPort)
        {
        }

        const std::string& ip() const noexcept { return mIp; }
        const Id& id() const noexcept { return mId; }
        int port() const noexcept { return mPort; }

    private:
        std::string mIp;
        Id mId;
        int mPort;
    };

    class EventServerStopped : public Event
    {
    public:
        explicit EventServerStopped(const Id& aId)
            : Event(ET_SERVER_STOPPED),
              mId(aId)
        {
        }

        const Id& id() const noexcept { return mId; }

    private:
        Id mId;
    };

    class EventPeerConnected : public Event
    {
    public:
        EventPeerConnected(const Id& aServerId, const Id& aConnectionId, const std::string& arIp, int aPort)
            : Event(ET_PEER_CONNECTED),
              mIp(arIp),
              mServerId(aServerId),
              mConnectionId(aConnectionId),
              mPort(aPort)
        {
        }

        const std::string& ip() const noexcept { return mIp; }
        const Id& serverId() const noexcept { return mServerId; }
        const Id& connectionId() const noexcept { return mConnectionId; }
        int port() const noexcept { return mPort; }

    private:
        std::string mIp;
        Id mServerId;
        Id mConnectionId;
        int mPort;
    };

    class EventConnectionActive : public Event
    {
    public:
        EventConnectionActive(const Id& aConnectionId)
            : Event(ET_CONNECTION_ACTIVE),
              mConnectionId(aConnectionId)
        {
        }

        const Id& connectionId() const noexcept { return mConnectionId; }

    private:
        Id mConnectionId;
    };

    class EventConnectionPassive : public Event
    {
    public:
        EventConnectionPassive(const Id& aConnectionId)
            : Event(ET_CONNECTION_PASSIVE),
              mConnectionId(aConnectionId)
        {
        }

        const Id& connectionId() const noexcept { return mConnectionId; }

    private:
        Id mConnectionId;
    };

    class EventPeerDisconnected : public Event
    {
    public:
        EventPeerDisconnected(const Id& aConnectionId)
            : Event(ET_PEER_DISCONNECTED),
              mConnectionId(aConnectionId)
        {
        }

        const Id& connectionId() const noexcept { return mConnectionId; }

    private:
        Id mConnectionId;
    };

    class EventApduReceived : public Event
    {
    public:
        EventApduReceived(const Id& aConnectionId, const uint8_t* begin, const uint8_t* end)
            : Event(ET_APDU_RECEIVED),
              mConnectionId(aConnectionId),
              mApdu(begin, end)
        {
        }

        const Id& connectionId() const noexcept { return mConnectionId; }
        const std::vector<uint8_t>& apdu() const noexcept { return mApdu; }

    private:
        Id mConnectionId;
        std::vector<uint8_t> mApdu;
    };

    class EventApduSent : public Event
    {
    public:
        EventApduSent(const Id& aConnectionId, const uint8_t* begin, const uint8_t* end)
            : Event(ET_APDU_SENT),
              mConnectionId(aConnectionId),
              mApdu(begin, end)
        {
        }

        const Id& connectionId() const noexcept { return mConnectionId; }
        const std::vector<uint8_t>& apdu() const noexcept { return mApdu; }
    private:
        Id mConnectionId;
        std::vector<uint8_t> mApdu;
    };
}

#endif // EVENT_HPP
