#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QTimer>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTimer tmr;

    QObject::connect(&tmr, &QTimer::timeout, []()
    {
        static quint32 counter;
        qDebug() << QDateTime::currentDateTimeUtc() << "hello world" << counter++;
    });

    tmr.start(500);

    return a.exec();
}
