#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->eventTableView->setModel(&mEventTable);
    ui->connectionTable->setModel(&mConnectionTable);
    QObject::connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::StartServer);
    QObject::connect(ui->stopButton,  &QPushButton::clicked, this, &MainWindow::StopServer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::StartServer()
{
    try
    {
        QObject::connect(&mServer, &IEC104::Server::OnTelegramEvent, this, &MainWindow::PrintTelegram, Qt::QueuedConnection);
        QObject::connect(&mServer, &IEC104::Server::OnConnectionEvent, this, &MainWindow::UpdateConnections, Qt::QueuedConnection);
        mServer.Start(ui->ipAddressEdit->document()->toPlainText().toStdString(), ui->portEdit->toPlainText().toUShort());
        ui->startButton->setEnabled(false);
        ui->stopButton->setEnabled(true);
    }
    catch (...) {}
}

void
MainWindow::UpdateConnections(const ConnectionEvent& arEvent)
{
    mConnectionTable.Update(arEvent);
}


void
MainWindow::StopServer()
{
    mServer.Stop();
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
}

void
MainWindow::PrintTelegram(const TelegramEvent& arEvent)
{
    mEventTable.Add(arEvent);
}
