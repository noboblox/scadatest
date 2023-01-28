#ifndef VRTUTHREAD_HPP
#define VRTUTHREAD_HPP

#include <QObject>
#include <atomic>
#include <thread>

#include "vrtu/service.hpp"

class VrtuThread : public QObject
{
    Q_OBJECT
public:
    explicit VrtuThread(QObject* apParent);
    ~VrtuThread();

    void post(std::unique_ptr<VRTU::Request>&& apMessage);
    void startThread();
    void stopThread();

signals:
    void SignalMessageReceived(std::shared_ptr<const VRTU::ApiMessage> apMsg);

private:
    void MessageReceived(std::unique_ptr<const VRTU::ApiMessage>&& apMsg);

    void eventLoop();
    VRTU::Service service;
    std::thread serviceThread;
    std::atomic_bool mRunning;
    std::atomic_bool mNeedStop;
};

#endif // VRTUTHREAD_HPP
