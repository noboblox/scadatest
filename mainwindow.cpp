#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vrtu/api/apimessage.hpp"
#include "vrtu/api/requests.hpp"
#include "vrtu/api/response.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      service(this)
{
    ui->setupUi(this);
    ui->eventTableView->setModel(&mEventTable);
    ui->connectionTable->setModel(&mConnectionTable);

    QObject::connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::StartServer);
    QObject::connect(ui->stopButton,  &QPushButton::clicked, this, &MainWindow::StopServer);
    QObject::connect(&service,  &VrtuThread::SignalMessageReceived, this, &MainWindow::ExecuteVrtuMessage, Qt::QueuedConnection);
    QObject::connect(&mEventTable,  &TelegramTableModel::rowsInserted, this, &MainWindow::EventTableRowAdded);

    service.startThread();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::StartServer()
{
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);

    const auto ip = ui->ipAddressEdit->document()->toPlainText().toStdString();
    const int port = ui->portEdit->toPlainText().toUShort();
    service.post(std::make_unique<VRTU::RequestStartServer>(ip, port));
}

void
MainWindow::EventTableRowAdded(const QModelIndex&, int, int end)
{
    if (end <= 0)
        return;

    const int previousRow = end - 1;
    const bool wasAtBottom = ui->eventTableView->rowViewportPosition(previousRow) < ui->eventTableView->visibleRegion().boundingRect().bottom();

    if (wasAtBottom)
        ui->eventTableView->scrollToBottom();
}

void
MainWindow::ExecuteVrtuMessage(std::shared_ptr<const VRTU::ApiMessage> apMsg)
{
    if (apMsg->isResponse())
        HandleResponse(static_cast<const VRTU::Response&> (*apMsg));
    else if (apMsg->isEvent())
        HandleEvent(static_cast<const VRTU::Event&> (*apMsg));
}

void
MainWindow::HandleResponse(const VRTU::Response&)
{
    if (state.server.isNull())
        ui->startButton->setEnabled(true);
    else
        ui->stopButton->setEnabled(true);
}

void
MainWindow::HandleEvent(const VRTU::Event& arMsg)
{
    switch (arMsg.type())
    {
    case VRTU::Event::ET_SERVER_STARTED:
        HandleServerStarted(static_cast<const VRTU::EventServerStarted&> (arMsg));
        return;
    case VRTU::Event::ET_SERVER_STOPPED:
        HandleServerStopped(static_cast<const VRTU::EventServerStopped&> (arMsg));
        return;
    case VRTU::Event::ET_PEER_CONNECTED:
        HandlePeerConnected(static_cast<const VRTU::EventPeerConnected&> (arMsg));
        return;
    case VRTU::Event::ET_CONNECTION_ACTIVE:
        HandleConnectionActive(static_cast<const VRTU::EventConnectionActive&> (arMsg));
        return;
    case VRTU::Event::ET_CONNECTION_PASSIVE:
        HandleConnectionPassive(static_cast<const VRTU::EventConnectionPassive&> (arMsg));
        return;
    case VRTU::Event::ET_PEER_DISCONNECTED:
        HandlePeerDisconnected(static_cast<const VRTU::EventPeerDisconnected&> (arMsg));
        return;
    case VRTU::Event::ET_APDU_RECEIVED:
        HandleApduReceived(static_cast<const VRTU::EventApduReceived&> (arMsg));
        return;
    case VRTU::Event::ET_APDU_SENT:
        HandleApduSent(static_cast<const VRTU::EventApduSent&> (arMsg));
        return;
    default:
        return;
    }
}

void MainWindow::HandleServerStarted(const VRTU::EventServerStarted& arMsg)
{
    state.server = arMsg.id();
    state.serverIp = arMsg.ip();
    state.serverPort = arMsg.port();
}

void MainWindow::HandleServerStopped(const VRTU::EventServerStopped&)
{
    state.server = VRTU::Id();
    state.serverIp = "";
    state.serverPort = 0;
}

void MainWindow::HandlePeerConnected(const VRTU::EventPeerConnected& arMsg)
{
    mConnectionTable.Add(ConnectionModel::Row{arMsg.connectionId(), (arMsg.ip()), arMsg.port(), false});
}

void MainWindow::HandleConnectionActive(const VRTU::EventConnectionActive& arMsg)
{
    mConnectionTable.SetActivation(arMsg.connectionId(), true);
}

void MainWindow::HandleConnectionPassive(const VRTU::EventConnectionPassive& arMsg)
{
    mConnectionTable.SetActivation(arMsg.connectionId(), false);
}

void MainWindow::HandlePeerDisconnected(const VRTU::EventPeerDisconnected& arMsg)
{
    mConnectionTable.Remove(arMsg.connectionId());
}

void MainWindow::HandleApduReceived(const VRTU::EventApduReceived& arMsg)
{
    const auto peer = mConnectionTable.getPeerAddress(arMsg.connectionId());
    mEventTable.Add(TelegramTableModel::Row{peer.first, peer.second, state.serverIp, state.serverPort, arMsg.apdu()});
}

void MainWindow::HandleApduSent(const VRTU::EventApduSent& arMsg)
{
    const auto peer = mConnectionTable.getPeerAddress(arMsg.connectionId());
    mEventTable.Add(TelegramTableModel::Row{state.serverIp, state.serverPort, peer.first, peer.second, arMsg.apdu()});
}

void
MainWindow::StopServer()
{
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
    service.post(std::make_unique<VRTU::RequestStopServer>(state.server));
}
