#include "server.h"
#include <stdexcept>
#include <string>


namespace IEC104
{
    Server::Server()
        : mpSlave(CS104_Slave_create(1024, 1024), CS104_Slave_destroy),
          mLocalAddress(),
          mPort(0)
    {
    }

    void Server::Start(const std::string& arInterface, int aPort)
    {
        mLocalAddress = arInterface;
        mPort = aPort;

        CS104_Slave_setRawMessageHandler(mpSlave.get(), &Server::OnTelegramEventInternal, this);
        CS104_Slave_setConnectionEventHandler(mpSlave.get(), &Server::OnConnectionEventInternal, this);
        CS104_Slave_setLocalAddress(mpSlave.get(), mLocalAddress.c_str());
        CS104_Slave_setLocalPort(mpSlave.get(), mPort);
        CS104_Slave_start(mpSlave.get());

        if (!IsRunning())
            throw std::runtime_error("failed to start server with given arguments");
    }

    void Server::Stop() noexcept
    {
        mpSlave.reset(CS104_Slave_create(1024, 1024));
    }

    bool Server::IsRunning() const noexcept
    {
        return CS104_Slave_isRunning(mpSlave.get());
    }

    static
    bool GetPeerAddress(IMasterConnection connection, QHostAddress& ip, int& port)
    {
        char peer_endpoint[64] = "";
        IMasterConnection_getPeerAddress(connection, peer_endpoint, sizeof(peer_endpoint));

        const char* peer_ip = std::strtok(peer_endpoint, ":");
        const char* peer_port_str = std::strtok(nullptr, ":");

        if (peer_ip && peer_port_str)
        {
            ip.setAddress(peer_ip);
            port = std::stoi(peer_port_str);
            return true;
        }

        return false;
    }

    /* static */
    void Server::OnConnectionEventInternal(void* parameter, IMasterConnection connection, CS104_PeerConnectionEvent event) noexcept
    {
        Server& self = *static_cast<Server*> (parameter);

        QHostAddress peer;
        int peer_port = 0;

        if (!GetPeerAddress(connection, peer, peer_port))
            return;

        ConnectionEvent::Type type;

        switch (event)
        {
        case CS104_CON_EVENT_ACTIVATED:
            type = ConnectionEvent::STARTDT;
            break;
        case CS104_CON_EVENT_CONNECTION_CLOSED:
            type = ConnectionEvent::DISCONNECTED;
            break;
        case CS104_CON_EVENT_CONNECTION_OPENED:
            type = ConnectionEvent::CONNECTED;
            break;
        case CS104_CON_EVENT_DEACTIVATED:
            type = ConnectionEvent::STOPDT;
            break;
        default:
            return;
        }

        ConnectionEvent msg(type, peer, peer_port);
        emit self.OnConnectionEvent(msg);
    }

    /* static */
    void Server::OnTelegramEventInternal(void* parameter, IMasterConnection connection, uint8_t* msg, int msgSize, bool send) noexcept
    {
        Server& self = *static_cast<Server*> (parameter);

        QHostAddress local, peer;
        int peer_port = 0;

        GetPeerAddress(connection, peer, peer_port);
        local.setAddress(QString::fromStdString(self.GetOwnAddress()));

        if (send)
        {
            emit self.OnTelegramEvent(TelegramEvent(local, self.mPort, peer, peer_port, QByteArray((const char*) msg, msgSize)));
        }
        else
        {
            emit self.OnTelegramEvent(TelegramEvent(peer, peer_port, local, self.mPort, QByteArray((const char*) msg, msgSize)));
        }
    }
}
