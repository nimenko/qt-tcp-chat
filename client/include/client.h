#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QTcpSocket>
#include <QHostAddress>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject* parent = nullptr);

    void connect_to_server(const QHostAddress& hostname, const quint16& port, const QString& name);
    void send_message(const QString& message);

private:
    QTcpSocket* socket_;

    QString name_;

    QHostAddress hostname_;
    quint16 port_;

    QStringList users_list_;
    QStringList messages_list_;

public slots:
    void on_message_entered(QString message);

private slots:
    void on_socket_connected();
    void on_socket_ready_read();
    void on_socket_error(QAbstractSocket::SocketError error);
};

#endif // CLIENT_H
