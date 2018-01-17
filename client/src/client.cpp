#include <QDebug>

#include "include/client.h"

Client::Client(QObject* parent) :
    QObject(parent),
    socket_(new QTcpSocket(this))
{
    connect(socket_, &QTcpSocket::connected, this, &Client::on_socket_connected);
    connect(socket_, &QTcpSocket::readyRead, this, &Client::on_socket_ready_read);
    connect(socket_, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Client::on_socket_error);
}

void Client::connect_to_server(const QHostAddress& hostname, const quint16& port, const QString& name)
{
    hostname_ = hostname;
    port_ = port;
    name_ = name;
    socket_->connectToHost(hostname_, port_);
}

void Client::send_message(const QString& message)
{
    if (!message.isEmpty())
    {
        socket_->write(QString(message + "\n").toUtf8());

        //qDebug() << "Message: " << message << " was sent.";
    }
}

void Client::on_socket_connected()
{
    // Send my name.
    socket_->write(QString("/me:" + name_ + "\n").toUtf8());

    //qDebug() << "Name: " << name_ << " was sent.";
}

void Client::on_socket_ready_read()
{
    while (socket_->canReadLine())
    {
        QString line = QString::fromUtf8(socket_->readLine().trimmed());

        QRegExp message_regex("^([^:]+):(.*)$");
        QRegExp users_list_regex("^/users:(.*)$");

        if (users_list_regex.indexIn(line) != -1)  // If this is a users list.
        {
            users_list_ = users_list_regex.cap(1).split(",");

            //qDebug() << "Users list update: " << users_list_;
        }
        else if (message_regex.indexIn(line) != -1)  // If this is a message.
        {
            QString user = message_regex.cap(1);
            QString message = message_regex.cap(2);

            messages_list_.append(user + " : " + message);

            QTextStream(stdout) << user << " : " << message << "\n";

            //qDebug() << "Messages list update: " << messages_list_;
        }
    }
}

void Client::on_socket_error(QAbstractSocket::SocketError error)
{
    switch (error) {
    case QAbstractSocket::RemoteHostClosedError:
        break;

    case QAbstractSocket::HostNotFoundError:
        qWarning() << "The host was not found. Please check the host name and port settings.";
        break;

    case QAbstractSocket::ConnectionRefusedError:
        qWarning() << "The connection was refused by the peer. Make sure the server is running, "
                      "and check that the host name and port settings are correct.";
        break;

    default:
        qWarning() << "The following error occurred: " << socket_->errorString();
    }
}

void Client::on_message_entered(QString message)
{
    send_message(message);
}
