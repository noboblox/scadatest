#ifndef VRTU_EVENT_HPP
#define VRTU_EVENT_HPP

#include <vector>
#include "apimessage.hpp"

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
            ET_PEER_DISCONNECTED,
            ET_APDU_SENT,
            ET_APDU_RECEIVED,
        };

        EventType type() const noexcept { return mType; }

    protected:
        explicit Event(EventType aType) noexcept
            : ApiMessage(ApiMessage::CAT_EVENT),
              mType(aType)
        {
        }

    private:
        EventType mType;
    };

    class EventServerStarted : public Event
    {
    public:
        EventServerStarted(int aId, const std::string& arIp, int aPort)
            : Event(ET_SERVER_STARTED),
              mIp(arIp), mId(aId), mPort(aPort)
        {
        }

        const std::string& ip() const noexcept { return mIp; }
        int id() const noexcept { return mId; }
        int port() const noexcept { return mPort; }

    private:
        std::string mIp;
        int mId;
        int mPort;
    };

    class EventServerStopped : public Event
    {
    public:
        explicit EventServerStopped(int aId)
            : Event(ET_SERVER_STOPPED),
              mId(aId)
        {
        }

        int id() const noexcept { return mId; }

    private:
        int mId;
    };

    class EventPeerConnected : public Event
    {
    public:
        EventPeerConnected(int aServerId, int aConnectionId, const std::string& arIp, int aPort)
            : Event(ET_PEER_CONNECTED),
              mIp(arIp),
              mServerId(aServerId),
              mConnectionId(aConnectionId),
              mPort(aPort)
        {
        }

        const std::string& ip() const noexcept { return mIp; }
        int serverId() const noexcept { return mServerId; }
        int connectionId() const noexcept { return mConnectionId; }
        int port() const noexcept { return mPort; }

    private:
        std::string mIp;
        int mServerId;
        int mConnectionId;
        int mPort;
    };

    class EventPeerDisconnected : public Event
    {
    public:
        EventPeerDisconnected(int aConnectionId)
            : Event(ET_PEER_DISCONNECTED),
              mConnectionId(aConnectionId)
        {
        }

        int connectionId() const noexcept { return mConnectionId; }

    private:
        int mConnectionId;
    };

    class EventApduReceived : public Event
    {
    public:
        EventApduReceived(int aConnectionId, const uint8_t* begin, const uint8_t* end)
            : Event(ET_APDU_RECEIVED),
              mConnectionId(aConnectionId),
              mApdu(begin, end)
        {
        }

        int connectionId() const noexcept { return mConnectionId; }
        const std::vector<uint8_t>& apdu() const noexcept;

    private:
        int mConnectionId;
        std::vector<uint8_t> mApdu;
    };

    class EventApduSent : public Event
    {
    public:
        EventApduSent(int aConnectionId, const uint8_t* begin, const uint8_t* end)
            : Event(ET_APDU_SENT),
              mConnectionId(aConnectionId),
              mApdu(begin, end)
        {
        }

        int connectionId() const noexcept { return mConnectionId; }
        const std::vector<uint8_t>& apdu() const noexcept;

    private:
        int mConnectionId;
        std::vector<uint8_t> mApdu;
    };
}

#endif // EVENT_HPP
