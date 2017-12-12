#include "include/non_blocking_cli.h"

NonBlockingCLI::NonBlockingCLI(QObject* parent) : QObject(parent)
{
    this->moveToThread(&thread_);

    connect(&thread_, SIGNAL(started()), this, SLOT(read_stdin()));

    thread_.start();
}

void NonBlockingCLI::read_stdin()
{
    QTextStream input_stream(stdin);
    QString line;

    while (input_stream.readLineInto(&line))
    {
        emit line_is_read(line);
    }
}
