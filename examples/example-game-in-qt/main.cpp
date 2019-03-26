#include "mainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("imageformats");
    paths.append("platforms");
    paths.append("sqldrivers");
    QCoreApplication::setLibraryPaths(paths);

    QApplication a(argc, argv);
    MainWindow* w = new MainWindow();
    w->show();
    int result = a.exec();
    Game* game = w->game();
    delete w;
    // Kill game, when everything is done
    MainWindow::freeGame(game);
    return result;
}
