#define DOCTEST_CONFIG_IMPLEMENT
#include <QApplication>
#include "mainwindow.h"
#include "doctest.h"

int main(int argc, char *argv[])
{
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    context.run();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
