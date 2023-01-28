#include "service.hpp"
#include "api/requests.hpp"
#include "api/response.hpp"

namespace VRTU {

    Service::Service(ApiMessageHandler aHandler)
        : mServiceClient(aHandler),
          mModel(mServiceClient)
    {
    }

    Service::~Service()
    {
    }

    void Service::post(std::unique_ptr<Request>&& apMessage)
    {
        std::lock_guard<std::mutex> l(m);
        if (apMessage)
            mPending.insert(mPending.begin(), std::move(apMessage));
    }

    std::unique_ptr<Request> Service::next()
    {
        std::lock_guard<std::mutex> l(m);

        if (mPending.empty())
            return nullptr;

        auto result = std::move(mPending.back());
        mPending.pop_back();
        return result;
    }

    void Service::tick()
    {
        std::unique_ptr<Request> req;
        while ((req = next()) != nullptr )
        {
            ExecuteRequest(*req);
        }
    }

    static int ExecuteStartServer(Model& arModel, const RequestStartServer& arRequest)
    {
        return arModel.StartServer(arRequest.ip(), arRequest.port());
    }

    static int ExecuteStopServer(Model& arModel, const RequestStopServer& arRequest)
    {
        return arModel.StopServer(arRequest.serverId());
    }

    void Service::ExecuteRequest(const Request& arRequest)
    {
        switch (arRequest.type())
        {
        case REQUEST_START_SERVER:
            mServiceClient(std::make_unique<Response>(ExecuteStartServer(mModel, static_cast<const RequestStartServer&>(arRequest))));
            return;
        case REQUEST_STOP_SERVER:
            mServiceClient(std::make_unique<Response>(ExecuteStopServer(mModel, static_cast<const RequestStopServer&>(arRequest))));
            return;
        default:
            mServiceClient(std::make_unique<Response>(-1));
            return;
        }
    }

} // namespace VRTU
