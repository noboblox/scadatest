#ifndef VRTU_REQUESTS_HPP
#define VRTU_REQUESTS_HPP

#include <string>
#include "apimessage.hpp"
#include "vrtu/datamodel/id.hpp"

namespace VRTU
{
    enum RequestType
    {
        REQUEST_invalid = -1,

        REQUEST_START_SERVER = 0,
        REQUEST_STOP_SERVER,
    };

    class Request : public ApiMessage
    {
    public:
        RequestType type() const noexcept { return mType; }

    protected:
        explicit Request(RequestType aType)
            : ApiMessage(ApiMessage::CAT_REQUEST),
              mType(aType) {}

    private:
        RequestType mType;
    };

    class RequestStartServer : public Request
    {
    public:
        RequestStartServer(const std::string& arIp, int aPort)
            : Request(REQUEST_START_SERVER),
              mIp(arIp), mPort(aPort)
        {
        }

        const std::string& ip() const noexcept { return mIp; }
        int port() const noexcept { return mPort; }

    private:
        std::string mIp;
        int mPort;
    };

    class RequestStopServer : public Request
    {
    public:
        RequestStopServer(const Id& aId)
            : Request(REQUEST_STOP_SERVER),
              mId(aId)
        {
        }

        const Id& serverId() const noexcept { return mId; }

    private:
        Id mId;
    };
}

#endif // REQUESTS_HPP
