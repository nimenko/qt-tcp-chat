#include <QTextStream>
#include <QRegExp>
#include <QDebug>

#include "include/server.h"

Server::Server(QObject* parent) : QTcpServer(parent)
{
}

void Server::start_server(const quint16& port)
{
    port_ = port;

    if (listen(QHostAddress::Any, port_))
    {
        QTextStream(stdout) << "Server is started on port " << port_ << "." << "\n";
    }
    else
    {
        qWarning() << "Server can't start on port " << port_ << ".";
    }
}

void Server::send_users_list()
{
    QStringList users_list;

    foreach (QString user, users_.values()) {
        users_list << user;
    }

    foreach (QTcpSocket* client, clients_) {
        client->write(QString("/users:" + users_list.join(",") + "\n").toUtf8());
    }
}

void Server::incomingConnection(qintptr socket_d)
{
    QTcpSocket* client = new QTcpSocket(this);
    client->setSocketDescriptor(socket_d);
    clients_.insert(client);

    QTextStream(stdout) << "New client from: " << client->peerAddress().toString() << "\n";

    connect(client, &QTcpSocket::readyRead, this, &Server::on_ready_read);
    connect(client, &QTcpSocket::disconnected, this, &Server::on_disconnected);
}

void Server::on_ready_read()
{
    QTcpSocket* client = dynamic_cast<QTcpSocket*> (sender());

    while(client->canReadLine())
    {
        QString line = QString::fromUtf8(client->readLine()).trimmed();

        //qDebug() << "Incoming message: " << line;

        QRegExp username_regex("^/me:(.*)$");

        if(username_regex.indexIn(line) != -1)  // New client send us his name.
        {
            QString user = username_regex.cap(1);
            users_[client] = user;

            QTextStream(stdout) << "User " << user << " is authorized." << "\n";

            foreach (QTcpSocket* client, clients_) {
                client->write(QString("Server: " + user + " has joined.\n").toUtf8());
            }

            send_users_list();
        }
        else if(users_.contains(client))  // It is a registered user.
        {
            QString message = line;
            QString user = users_[client];

            //qDebug() << "Message from registered user:";

            QTextStream(stdout) << user << " : " << message << "\n";

            foreach (QTcpSocket* other_client, clients_) {
                other_client->write(QString(user + ":" + message + "\n").toUtf8());
            }
        }
        else  // It is an unregistered client.
        {
            qWarning() << "Got bad message from unregistered client: " << client->peerAddress().toString() << line;
        }
    }
}

void Server::on_disconnected()
{
    QTcpSocket* client = dynamic_cast<QTcpSocket*> (sender());

    QTextStream(stdout) << "Client disconnected: " << client->peerAddress().toString() << "\n";

    clients_.remove(client);

    QString user = users_[client];
    users_.remove(client);

    send_users_list();

    foreach (QTcpSocket* client, clients_) {
        client->write(QString("Server: " + user + " has left.\n").toUtf8());
    }
}
