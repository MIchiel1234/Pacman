#include "networkmanager.h"
#include <QHostAddress>
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent), server(nullptr), socket(nullptr)
{
}

void NetworkManager::startHosting()
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &NetworkManager::newConnection);
    if (!server->listen(QHostAddress::Any, 12345)) {
        qDebug() << "Server could not start!";
    } else {
        qDebug() << "Server started!";
    }
}

void NetworkManager::joinGame()
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::readyRead);
    socket->connectToHost(QHostAddress::LocalHost, 12345);
    if (socket->waitForConnected(3000)) {
        qDebug() << "Connected to host!";
    } else {
        qDebug() << "Failed to connect to host!";
    }
}

void NetworkManager::newConnection()
{
    socket = server->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::readyRead);
    qDebug() << "New client connected!";
}

void NetworkManager::readyRead()
{
    QByteArray data = socket->readAll();
    qDebug() << "Data received:" << data;
    // Handle data here
}
