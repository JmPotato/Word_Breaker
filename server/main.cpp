#include "user.h"
#include "game.h"
#include "rank.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    User userServer;
    Game gameServer;
    Rank rankServer;

    return a.exec();
}
