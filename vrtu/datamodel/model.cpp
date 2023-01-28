#include "model.hpp"
#include "server.hpp"
#include "vrtu/api/events.hpp"

namespace VRTU
{
    Model::Model(ApiMessageHandler aClient)
        : mClient(aClient)
    {
    }

    Model::~Model()
    {
    }

    int Model::StartServer(const std::string& arIp, int aPort)
    {
        auto server = std::make_unique<Server>(*this);

        if (server->Start(arIp, aPort))
        {
            const auto id = Id::createUUID();
            mServers.emplace_back(id, std::move(server));
            SendToClient(std::make_unique<EventServerStarted>(id, arIp, aPort));
            return 0;
        }

        return -1;
    }

    const Id& Model::serverId(const Server& server) const noexcept
    {
        static const Id invalid;
        auto it = std::find_if(mServers.begin(), mServers.end(),
                               [&server](const auto& v){ return v.second.get() == &server; });

        return it != mServers.end() ? it->first : invalid;
    }

    const Id& Model::connectionId(IMasterConnection connection) const noexcept
    {
        static const Id invalid;
        auto it = std::find_if(mConnections.begin(), mConnections.end(),
                               [connection](const auto& v){ return v.second == connection; });

        return it != mConnections.end() ? it->first : invalid;
    }

    int Model::StopServer(const Id& arId)
    {
        auto it = std::find_if(mServers.begin(), mServers.end(),
                               [&arId](const auto& v){ return v.first == arId; });

        if (it != mServers.end())
        {
            SendToClient(std::make_unique<EventServerStopped>(arId));
            mServers.erase(it);
        }
        return 0;
    }

    void Model::AddConnection(const Server& server, IMasterConnection connection)
    {
        const auto id = Id::createUUID();
        const auto ipAndPort = Server::peerAddress(connection);

        mConnections.emplace_back(id, connection);
        SendToClient(std::make_unique<EventPeerConnected>(serverId(server), id, ipAndPort.first, ipAndPort.second));
    }

    void Model::ActivateConnection(IMasterConnection connection)
    {
        auto it = std::find_if(mConnections.begin(), mConnections.end(),
                               [connection](const auto& v){ return v.second == connection; });

        if (it != mConnections.end())
        {
            SendToClient(std::make_unique<EventConnectionActive>(it->first));
        }
    }

    void Model::DeactivateConnection(IMasterConnection connection)
    {
        auto it = std::find_if(mConnections.begin(), mConnections.end(),
                               [connection](const auto& v){ return v.second == connection; });

        if (it != mConnections.end())
        {
            SendToClient(std::make_unique<EventConnectionPassive>(it->first));
        }
    }

    void Model::RemoveConnection(IMasterConnection connection)
    {
        auto it = std::find_if(mConnections.begin(), mConnections.end(),
                               [connection](const auto& v){ return v.second == connection; });

        if (it != mConnections.end())
        {
            SendToClient(std::make_unique<EventPeerDisconnected>(it->first));
            mConnections.erase(it);
        }
    }

    void Model::ApduEvent(IMasterConnection connection, const uint8_t* begin, const uint8_t* end, bool send)
    {
        if (send)
            SendToClient(std::make_unique<EventApduSent>(connectionId(connection), begin, end));
        else
            SendToClient(std::make_unique<EventApduReceived>(connectionId(connection), begin, end));
    }
}
