#ifndef NON_BLOCKING_CLI_H
#define NON_BLOCKING_CLI_H

#include <QObject>
#include <QThread>
#include <QString>

class NonBlockingCLI : public QObject
{
    Q_OBJECT
public:
    explicit NonBlockingCLI(QObject* parent = nullptr);

private:
    QThread thread_;

signals:
    void line_is_read(QString line);

private slots:
    void read_stdin();
};

#endif // NON_BLOCKING_CLI_H
