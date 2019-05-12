
#include <QCoreApplication>
#include <QDebug>

#include "Main.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Main mainObj ;

    return a.exec();
}
