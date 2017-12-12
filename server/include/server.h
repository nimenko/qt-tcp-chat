#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QMap>
#include <QSet>
#include <QRegExp>
#include <QString>
#include <QTextStream>
#include <QDebug>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject* parent = nullptr);

    void start_server(const quint16& port);

private:
    quint16 port_;

    QSet<QTcpSocket*> clients_;
    QMap<QTcpSocket*, QString> users_;

    void send_users_list();

protected:
    void incomingConnection(qintptr socket_d);

private slots:
    void on_ready_read();
    void on_disconnected();
};

#endif // SERVER_H
