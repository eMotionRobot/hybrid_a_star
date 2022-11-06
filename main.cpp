
/*
 * Grid Map
 * 编译:QT5.5.0+MinGW
 * 作者:wuxy
 * Reference metrails:https://blog.csdn.net/dpsying/article/details/53791562
*/

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
