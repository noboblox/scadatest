#include "vrtuthread.hpp"

VrtuThread::VrtuThread(QObject *parent)
    : QObject{parent},
      service([this](auto&& msg){ this->MessageReceived(std::move(msg)); }),
      mRunning(false), mNeedStop(false)
{
}

VrtuThread::~VrtuThread()
{
    stopThread();
}

void VrtuThread::post(std::unique_ptr<VRTU::Request>&& apMessage)
{
    service.post(std::move(apMessage));
}

void VrtuThread::startThread()
{
    if (mRunning)
        return;

    mRunning = true;
    serviceThread = std::thread(&VrtuThread::eventLoop, this);
}

void VrtuThread::stopThread()
{
    mNeedStop = true;
    serviceThread.join();
    mRunning = false;
    mNeedStop = false;
}

void VrtuThread::eventLoop()
{
    while (!mNeedStop)
    {
        service.tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void VrtuThread::MessageReceived(std::unique_ptr<const VRTU::ApiMessage>&& apMsg)
{
    auto ptr = std::shared_ptr(std::move(apMsg));
    SignalMessageReceived(ptr);
}
