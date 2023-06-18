#define DOCTEST_CONFIG_IMPLEMENT
#include <iostream>
#include "..\doctest.h"
#include "..\Converter.h"
#include "..\Tree.h"
#include "..\TreeFactory.h"
#include "..\TreeIterator.h"
#include "..\TreeIterStrategy.h"
#include "..\TreeTemplate.h"
#include "..\TreeBackup.h"
#include "..\TreeAdapter.h"
#include "..\TreeFacade.h"
#include <fstream>
#include <string>
#include <time.h>
#include <vector>

int main()
{
    doctest::Context context;
    context.run();
    return 0;
}

TEST_CASE("convert types test") {
    double a = 123.33;
    int ai = int(Converter<double>(a));
    std::string as = std::string(Converter<double>(a));
    double ad = double(Converter<double>(a));
    CHECK(ai == 123);
    CHECK(as == "123.330000");
    CHECK(ad == 123.33);
    int b = 256;
    int bi = int(Converter<int>(b));
    std::string bs = std::string(Converter<int>(b));
    double bd = double(Converter<int>(b));
    CHECK(bi == 256);
    CHECK(bs == "256");
    CHECK(bd == 256);
    std::string c = "334.44";
    int ci = int(Converter<std::string>(c));
    std::string cs = std::string(Converter<std::string>(c));
    double cd = double(Converter<std::string>(c));
    CHECK(ci == 334);
    CHECK(cs == "334.44");
    CHECK(cd == 334.44);
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
    CHECK(out == "A\nB\nC\nD\nE\nF\nG\nH\nI\n");
    trav = new Traverser<std::string>(PreOrder);
    out = trav->Iterate(root);
    CHECK(out == "F\nB\nA\nD\nC\nE\nG\nI\nH\n");
    trav = new Traverser<std::string>(PostOrder);
    out = trav->Iterate(root);
    CHECK(out == "A\nC\nE\nD\nB\nH\nI\nG\nF\n");
    delete root;
}

TEST_CASE("template method test") {
    TreeFactory* factory = TreeFactory::getTreeFac();
    Node<int>* root = factory->createTree(BinSearchTree, 15);
    root->Add(6);
    root->Add(3);
    root->Add(7);
    root->Add(2);
    root->Add(4);
    root->Add(13);
    root->Add(9);
    root->Add(18);
    root->Add(17);
    root->Add(20);
    TreeAnalyzer<int>* t = new NumberTreeAnalyzer<int>(root);
    std::string out = t->Analyze();
    CHECK(out == "Height: 5\nLeast element: 2\nGreatest element: 20\nSum of all keys in the tree: 114");
    delete t;
    delete root;
    Node<std::string>* root2 = factory->createTree(BinSearchTree, "binary");
    root2->Add("search");
    root2->Add("tree");
    root2->Add("also");
    root2->Add("called");
    root2->Add("an");
    root2->Add("ordered");
    root2->Add("sorted");
    TreeAnalyzer<std::string>* t2 = new StringTreeAnalyzer(root2);
    out = t2->Analyze();
    CHECK(out == "Height: 4\nLeast element: also\nGreatest element: tree\nLongest word: ordered, of length: 7");
}

TEST_CASE("adapter test") {
    TreeFactory* factory = TreeFactory::getTreeFac();
    Node<int>* root = factory->createTree(BinSearchTree, 15);
    root->Add(6);
    root->Add(3);
    root->Add(7);
    root->Add(2);
    root->Add(4);
    root->Add(13);
    root->Add(9);
    root->Add(18);
    root->Add(17);
    root->Add(20);
    TNodeAdapter<int>* toStr = new TNodeAdapter<int>(root, IntNode);
    StringedNode* sn = toStr->getStrNode();
    Node<std::string>* sr = sn->getTree();
    StringedNodeAdapter<int>* toInt = new StringedNodeAdapter<int>(sn);
    Node<int>* root2 = toInt->getNode();

    TreeIterator<int>* it = root2->CreateInOrderIterator();
    std::string trav = "";
    for (it->First(); !it->isDone(); it->Next()) {
        trav += std::to_string(it->Current()->getKey()) + " ";
    }
    delete it;
    CHECK(trav == "2 3 4 6 7 9 13 15 17 18 20 ");
}

TEST_CASE("facade/memento/command test") {
    TreeFacade<int>* facade = new TreeFacade<int>(IntNode, BinSearchTree);
    facade->AddKey("15");
    facade->AddKey("6");
    facade->AddKey("3");
    facade->AddKey("7");
    facade->AddKey("2");
    facade->AddKey("4");
    facade->AddKey("13");
    facade->AddKey("9");
    facade->AddKey("18");
    facade->AddKey("17");
    facade->AddKey("20");
    facade->Save();
    facade->Undo();
    std::string out = facade->Iterate("Preorder");
    CHECK(out == "15\n6\n3\n2\n4\n7\n13\n9\n18\n17\n");
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