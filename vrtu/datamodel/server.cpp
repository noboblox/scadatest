#include "server.hpp"
#include <stdexcept>
#include <string>
#include <cstring>
#include "model.hpp"

namespace VRTU
{
    Server::Server(Model& arParent)
        : mrParent(arParent),
          mpSlave(CS104_Slave_create(1024, 1024), CS104_Slave_destroy),
          mLocalAddress(),
          mPort(0)
    {
    }

    bool Server::Start(const std::string& arInterface, int aPort)
    {
        mLocalAddress = arInterface;
        mPort = aPort;

        CS104_Slave_setRawMessageHandler(mpSlave.get(), &Server::OnTelegramEventInternal, this);
        CS104_Slave_setASDUHandler(mpSlave.get(), &Server::IgnoreAsdu, this);
        CS104_Slave_setConnectionEventHandler(mpSlave.get(), &Server::OnConnectionEventInternal, this);
        CS104_Slave_setLocalAddress(mpSlave.get(), mLocalAddress.c_str());
        CS104_Slave_setLocalPort(mpSlave.get(), mPort);

        // as this service is for a test tool, we need multiple independent ACTIVE connections.
        // the default for productive slaves is ONE active connection.
        CS104_Slave_setServerMode(mpSlave.get(), CS104_MODE_CONNECTION_IS_REDUNDANCY_GROUP);

        CS104_Slave_start(mpSlave.get());
        return IsRunning();
    }

    void Server::Stop() noexcept
    {
        mpSlave.reset(CS104_Slave_create(1024, 1024));
    }

    bool Server::IsRunning() const noexcept
    {
        return CS104_Slave_isRunning(mpSlave.get());
    }

    std::pair<std::string, int> Server::peerAddress(IMasterConnection connection)
    {
        char peer_endpoint[64] = "";
        IMasterConnection_getPeerAddress(connection, peer_endpoint, sizeof(peer_endpoint));

        const char* peer_ip = std::strtok(peer_endpoint, ":");
        const char* peer_port_str = std::strtok(nullptr, ":");

        if (peer_ip && peer_port_str)
        {
            const int port = std::stoi(peer_port_str);
            return std::make_pair(peer_ip, port);
        }

        return std::make_pair("", 0);
    }

    /* static */
    void Server::OnConnectionEventInternal(void* parameter, IMasterConnection connection, CS104_PeerConnectionEvent event) noexcept
    {
        Server& self = *static_cast<Server*> (parameter);

        switch (event)
        {
        case CS104_CON_EVENT_CONNECTION_OPENED:
            self.mrParent.AddConnection(self, connection);
            return;
        case CS104_CON_EVENT_CONNECTION_CLOSED:
            self.mrParent.RemoveConnection(connection);
            return;
        case CS104_CON_EVENT_ACTIVATED:
            self.mrParent.ActivateConnection(connection);
            return;
        case CS104_CON_EVENT_DEACTIVATED:
            self.mrParent.DeactivateConnection(connection);
            return;
        default:
            return;
        }
    }

    /* static */
    void Server::OnTelegramEventInternal(void* parameter, IMasterConnection connection, uint8_t* msg, int msgSize, bool send) noexcept
    {
        Server& self = *static_cast<Server*> (parameter);
        self.mrParent.ApduEvent(connection, msg, msg + msgSize, send);
    }
}
