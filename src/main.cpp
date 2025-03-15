#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qRegisterMetaType<BoardInfo>("BoardInfo");
    QCoreApplication::setOrganizationName("Signal Lab");
    QCoreApplication::setApplicationName("xiaohuixiangqi");
    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    return app.exec();
}

