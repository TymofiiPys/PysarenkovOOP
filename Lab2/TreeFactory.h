#ifndef TREEFACTORY_H
#define TREEFACTORY_H

#include "Tree.h"

enum TreeType{
    BinSearchTree,
};

class TreeFactory{
private:
    static TreeFactory* treefac;
protected:
    TreeFactory() {};
    ~TreeFactory() {};
public:
    TreeFactory(TreeFactory &other) = delete;
    void operator=(const TreeFactory &) = delete;
    static TreeFactory* getTreeFac();
    
    /*template<typename T>
    Node<T>* createTree(TreeType type, T rootkey) {
        switch (type) {
        case BinSearchTree:
            return new BinaryNodeCompos<T>(rootkey);
        default:
            return nullptr;
        }
    }*/

    Node<int>* createTree(TreeType type, int rootkey){
        switch(type){
        case BinSearchTree:
            return new BinaryNodeCompos<int>(rootkey);
        default:
            return nullptr;
        }
    }
    
    Node<double>* createTree(TreeType type, double rootkey){
        switch(type){
        case BinSearchTree:
            return new BinaryNodeCompos<double>(rootkey);
        default:
            return nullptr;
        }
    }
    
    Node<std::string>* createTree(TreeType type, std::string rootkey){
        switch(type){
        case BinSearchTree:
            return new BinaryNodeCompos<std::string>(rootkey);
        default:
            return nullptr;
        }
    }
};

TreeFactory* TreeFactory::treefac{nullptr};

TreeFactory* TreeFactory::getTreeFac(){
    if(!treefac)
    {
        treefac = new TreeFactory();
    }
    return treefac;
}

#endif // TREEFACTORY_H
