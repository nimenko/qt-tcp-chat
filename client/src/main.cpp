#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QObject>
#include <QTextStream>
#include <QDebug>

#include "include/client.h"
#include "include/non_blocking_cli.h"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("chat-client-side");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Simple chat client.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("hostname", QCoreApplication::translate("main", "Hostname to connect."));
    parser.addPositionalArgument("port", QCoreApplication::translate("main", "Port to connect."));
    parser.addPositionalArgument("name", QCoreApplication::translate("main", "Your username."));
    parser.process(app);

    const QStringList args = parser.positionalArguments();

    QHostAddress hostname = QHostAddress::LocalHost;
    quint16 port = 3000;
    QString name = "Linus Torvalds";

    if (args.size() >= 3)
    {
        hostname.setAddress(args.at(0));
        port = args.at(1).toUInt();
        name = args.at(2);
    }
    else
    {
        qWarning() << "No connection info provided.";
    }

    NonBlockingCLI cli;

    Client client;

    client.connect_to_server(hostname, port, name);

    qDebug() << "Connecting to hostname: " << hostname;
    qDebug() << "Connecting to port: " << port;

    QObject::connect(&cli, &NonBlockingCLI::line_is_read, &client, &Client::on_message_entered);

    QTextStream(stdout) << "Print your message:"  << "\n";

    return app.exec();
}
