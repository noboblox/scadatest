#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "telegramtablemodel.h"
#include "connectionmodel.hpp"
#include "uistate.hpp"
#include "vrtuthread.hpp"
#include "vrtu/api/events.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace VRTU
{
    class Response;
    class Event;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void ExecuteVrtuMessage(std::shared_ptr<const VRTU::ApiMessage> apMsg);
    void EventTableRowAdded(const QModelIndex&, int start, int end);
    void StartServer();
    void StopServer();

private:
    void HandleResponse(const VRTU::Response& arMsg);
    void HandleEvent(const VRTU::Event& arMsg);
    void HandleServerStarted(const VRTU::EventServerStarted& arMsg);
    void HandleServerStopped(const VRTU::EventServerStopped& arMsg);
    void HandlePeerConnected(const VRTU::EventPeerConnected& arMsg);
    void HandleConnectionActive(const VRTU::EventConnectionActive& arMsg);
    void HandleConnectionPassive(const VRTU::EventConnectionPassive& arMsg);
    void HandlePeerDisconnected(const VRTU::EventPeerDisconnected& arMsg);
    void HandleApduReceived(const VRTU::EventApduReceived& arMsg);
    void HandleApduSent(const VRTU::EventApduSent& arMsg);

private:
    Ui::MainWindow* ui;
    VrtuThread service;
    UiState state;
    TelegramTableModel mEventTable;
    ConnectionModel mConnectionTable;
};
#endif
