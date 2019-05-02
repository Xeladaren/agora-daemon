
#include <QCoreApplication>

#include "minecraft/TestCrash.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TestCrash testCrash ;

    testCrash.start();

    return a.exec();
}
