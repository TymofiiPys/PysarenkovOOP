#define DOCTEST_CONFIG_IMPLEMENT
#include "mainwindow.h"
#include "doctest.h"
#include "Tree.h"
#include <QApplication>

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

TEST_CASE("Set children test"){
//    Node<int>* a = new BinaryNode<int>(5);
//    Node<int>* b = new RedBlackNode<int>(6);
//    a->Add(6);
//    delete a;
//    delete b;
    BinaryNodeCompos<int>* root = new BinaryNodeCompos(100);
    root->Add(20);
    root->Add(10);
    root->Add(500);
    root->Add(30);
}
