#ifndef VRTU_REQUESTS_HPP
#define VRTU_REQUESTS_HPP

#include <string>
#include "apimessage.hpp"

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

    class StartServer : public Request
    {
    public:
        StartServer(const std::string& arIp, int aPort)
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

    class StopServer : public Request
    {
    public:
        StopServer(int aId)
            : Request(REQUEST_STOP_SERVER),
              mId(aId)
        {
        }

        int serverId() const noexcept { return mId; }

    private:
        int mId;
    };
}

#endif // REQUESTS_HPP
