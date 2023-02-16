#ifndef VRTU_SERVER_HPP
#define VRTU_SERVER_HPP

#include <memory>
#include <string>
#include <functional>
#include "cs104_slave.h"

namespace VRTU
{
    class Model;

    class Server
    {
    public:
        explicit Server(Model& arParent);

        bool Start(const std::string& arIp, int aPort);
        void Stop() noexcept;
        bool IsRunning() const noexcept;

        const std::string& GetOwnAddress() const noexcept {return mLocalAddress; }

        static std::pair<std::string, int> peerAddress(IMasterConnection connection);

    private:
        // ASDUs are handled by the raw message handler -> OnTelegramEventInternal
        // However the library excepts an ASDU handler, or it will negatively acknowledge received ASDUs
        // as we want to display everything on APDU layer and dont care for specific ASDUs, we need to register a dummy routine which satisfies the library
        static bool IgnoreAsdu(void*, IMasterConnection, CS101_ASDU) noexcept { return true; }

        static void OnTelegramEventInternal(void* parameter, IMasterConnection connection, uint8_t* msg, int msgSize, bool send) noexcept;
        static void OnConnectionEventInternal(void* parameter, IMasterConnection connection, CS104_PeerConnectionEvent event) noexcept;

        Model& mrParent;
        std::unique_ptr<sCS104_Slave, void(*)(sCS104_Slave*)> mpSlave;
        std::string mLocalAddress;
        int mPort;
    };
}
#endif
