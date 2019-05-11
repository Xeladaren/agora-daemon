
#include <QCoreApplication>
#include <QDebug>

#include "minecraft/TestCrash.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TestCrash testCrash ;

    testCrash.start();

    qDebug() << "exec app" ;

    return a.exec();
}
