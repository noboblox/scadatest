#ifndef VRTU_MODEL_HPP
#define VRTU_MODEL_HPP

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "id.hpp"
#include "vrtu/api/apimessage.hpp"
#include "iec60870_slave.h"

namespace VRTU
{
    class ApiMessage;
    class Server;

    class Model
    {
    public:

        explicit Model(ApiMessageHandler aClient);
        ~Model();

        int StartServer(const std::string& arIp, int aPort);
        int StopServer(const Id& arId);

        void SendToClient(std::unique_ptr<const ApiMessage>&& apMsg) const { mClient(std::move(apMsg)); }

        void AddConnection(const Server& server, IMasterConnection connection);
        void ActivateConnection(IMasterConnection connection);
        void DeactivateConnection(IMasterConnection connection);
        void RemoveConnection(IMasterConnection connection);
        void ApduEvent(IMasterConnection connection, const uint8_t* begin, const uint8_t* end, bool send);

        const Id& serverId(const Server& server) const noexcept;
        const Id& connectionId(IMasterConnection) const noexcept;

    private:
        ApiMessageHandler mClient;
        std::vector<std::pair<Id, std::unique_ptr<VRTU::Server>>> mServers;
        std::vector<std::pair<Id, IMasterConnection>> mConnections;
    };
}

#endif // MODEL_HPP
