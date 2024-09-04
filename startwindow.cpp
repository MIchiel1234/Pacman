#include "startwindow.h"
#include "ui_startwindow.h"  // Include the generated header
#include "mainwindow.h"
#include "networkmanager.h"

StartWindow::StartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::on_startButton_clicked()
{
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
    this->close();
}

void StartWindow::on_hostButton_clicked()
{
    NetworkManager *networkManager = new NetworkManager();
    networkManager->startHosting();
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
    this->close();
}

void StartWindow::on_joinButton_clicked()
{
    NetworkManager *networkManager = new NetworkManager();
    networkManager->joinGame();
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
    this->close();
}
