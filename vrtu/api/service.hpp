#ifndef VRTU_SERVICE_HPP
#define VRTU_SERVICE_HPP

#include <functional>
#include <memory>
#include <mutex>
#include <vector>
#include "vrtu/datamodel/model.hpp"

namespace VRTU
{
    class ApiMessage;
    class Request;
    class Event;

    class Service
    {
    public:
        explicit Service(ApiMessageHandler aEventHandler);
        ~Service();

        /// add a request to the service (thread-safe)
        void post(std::unique_ptr<Request>&& apMessage);

        /**
         *  run all requests previously posted.
         *  The queue itself is thread-safe,
         *  The event processing itself is not thread-safe.
         */
        void tick();

    private:
        /// receive a request from the queue (thread-safe)
        std::unique_ptr<Request> next();
        void ExecuteRequest(const Request& arRequest);

        std::mutex m;
        std::vector<std::unique_ptr<Request>> mPending;
        ApiMessageHandler mServiceClient;
        Model mModel;
    };

} // namespace VRTU

#endif // VRTU_SERVICE_HPP
