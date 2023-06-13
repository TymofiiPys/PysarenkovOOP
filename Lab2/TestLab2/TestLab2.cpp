#define DOCTEST_CONFIG_IMPLEMENT
#include <iostream>
#include "..\doctest.h"
#include "..\Tree.h"
#include "..\TreeFactory.h"
#include "..\TreeIterator.h"
#include "..\TreeIterStrategy.h"
#include <fstream>
#include <string>

int main()
{
    doctest::Context context;
    context.run();
    return 0;
}

TEST_CASE("int tree") {
    BinaryNodeCompos<int>* root = new BinaryNodeCompos<int>(100);
    root->Add(20);
    root->Add(10);
    root->Add(500);
    root->Add(30);
    CHECK(root->Search(10));
    CHECK(!root->Search(40));
    CHECK(root->Search(500));
    CHECK(!root->Search(399));
    root->Remove(20);
    CHECK(!root->Search(20));
    delete root;
    root = new BinaryNodeCompos<int>(50);
    root->Add(30);
    root->Add(20);
    root->Add(40);
    root->Add(70);
    root->Add(60);
    root->Add(80);
    root->Remove(20);
    CHECK(!root->Search(20));
    root->Remove(50);
    CHECK(!root->Search(50));
    delete root;
}

TEST_CASE("std::string tree") {
    BinaryNodeCompos<std::string>* root = new BinaryNodeCompos<std::string>("obaba");
    root->Add("obobo");
    root->Add("ebebe");
    root->Add("ububu");
    root->Add("aua");
    CHECK(root->Search("aua"));
    CHECK(!root->Search("auaua"));
    CHECK(root->Search("obobo"));
    CHECK(!root->Search("iiiiiiiiiii"));
    delete root;
}

TEST_CASE("singleton test") {
    TreeFactory* factory = TreeFactory::getTreeFac();
    Node<int>* root = factory->createTree(BinSearchTree, 100);
    root->Add(20);
    root->Add(10);
    root->Add(500);
    root->Add(30);
    CHECK(root->Search(10));
    CHECK(!root->Search(40));
    CHECK(root->Search(500));
    CHECK(!root->Search(399));
    delete root;
}

TEST_CASE("iterator test") {
    TreeFactory* factory = TreeFactory::getTreeFac();
    Node<int>* root = factory->createTree(BinSearchTree, 50);
    root->Add(30);
    root->Add(20);
    root->Add(40);
    root->Add(70);
    root->Add(60);
    root->Add(80);
    TreeIterator<int>* it = root->CreateInOrderIterator();
    std::string trav = "";
    for (it->First(); !it->isDone(); it->Next()) {
        trav += std::to_string(it->Current()->getKey()) + " ";
    }
    delete it;
    CHECK(trav == "20 30 40 50 60 70 80 ");
    trav = "";
    it = root->CreatePreOrderIterator();
    for (it->First(); !it->isDone(); it->Next()) {
        trav += std::to_string(it->Current()->getKey()) + " ";
    }
    delete it;
    CHECK(trav == "50 30 20 40 70 60 80 ");
    trav = "";
    it = root->CreatePostOrderIterator();
    for (it->First(); !it->isDone(); it->Next()) {
        trav += std::to_string(it->Current()->getKey()) + " ";
    }
    delete it;
    CHECK(trav == "20 40 30 60 80 70 50 ");
    delete root;
}

TEST_CASE("strategy test") {
    TreeFactory* factory = TreeFactory::getTreeFac();
    Node<std::string>* root = factory->createTree(BinSearchTree, "F");
    root->Add("B");
    root->Add("A");
    root->Add("D");
    root->Add("C");
    root->Add("E");
    root->Add("G");
    root->Add("I");
    root->Add("H");
    Traverser<std::string>* trav = new Traverser<std::string>(InOrder);
    std::string out = trav->Iterate(root);
    CHECK(out == "A B C D E F G H I ");
    trav = new Traverser<std::string>(PreOrder);
    out = trav->Iterate(root);
    CHECK(out == "F B A D C E G I H ");
    trav = new Traverser<std::string>(PostOrder);
    out = trav->Iterate(root);
    CHECK(out == "A C E D B H I G F ");
    delete root;
}

//TEST_CASE("save/read test") {
//    TreeFactory* factory = TreeFactory::getTreeFac();
//    Node<int>* root = factory->createTree(BinSearchTree, 100);
//    root->Add(20);
//    root->Add(10);
//    root->Add(500);
//    root->Add(30);
//    std::ofstream o("root.dat", std::ios::binary);
//    o << root;
//    o.close();
//    std::ifstream i("root.dat", std::ios::binary | std::ios::ate);
//    auto size = i.tellg();
//    i.seekg(0);
//    i.read(root, size);
//}