#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QTextStream>
#include <QDebug>

#include "include/server.h"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("chat-server-side");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Simple chat server.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("port", QCoreApplication::translate("main", "Starts server on this port."));
    parser.process(app);

    const QStringList args = parser.positionalArguments();

    quint16 port = 3000;

    if (args.size() > 0)
    {
        port = args.at(0).toUInt();
    }
    else
    {
        qWarning() << "No port provided.";
    }

    Server server;

    if (server.listen(QHostAddress::Any, port))
    {
        qDebug() << "Server is listening at address " << server.serverAddress() << ".";
        qDebug() << "Server is listening on port " << port << ".";
    }
    else
    {
        qWarning() << "Can't start server on port " << port << ".";
    }

    return app.exec();
}
