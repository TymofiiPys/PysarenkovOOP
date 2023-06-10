// TestLab2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define DOCTEST_CONFIG_IMPLEMENT
#include <iostream>
#include "..\doctest.h"
#include "..\Tree.h"

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
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
