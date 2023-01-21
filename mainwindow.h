#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "server.h"
#include "telegramtablemodel.h"
#include "connectionmodel.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void StartServer();
    void StopServer();
    void PrintTelegram(const TelegramEvent& arEvent);
    void UpdateConnections(const ConnectionEvent& arEvent);

private:
    Ui::MainWindow* ui;
    IEC104::Server mServer;
    TelegramTableModel mEventTable;
    ConnectionModel mConnectionTable;
};
#endif
