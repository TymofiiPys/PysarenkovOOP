#ifndef TREEFACTORY_H
#define TREEFACTORY_H

#include "Tree.h"
#include <string>

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
    
    //Because a template class has to be returned as a created tree,
    //typename in it is set concretely,
    //otherwise, if typename T were used, TreeFactory would have to be
    //a template class as well. But what if user wanted to create a tree with
    //keys of another data type? The singleton factory, which this class is, would
    //have to be reinstantiated, which would be impossible.
    //Derived factories which produce trees of different types also wouldn't work,
    //once again - these would have to be reinstantiated in the same use case!
    //And the virtual function which would create trees and would be overriden by derived factories
    //would still return template class!
    //The only other way would be to declare and define classes such as NodeInt, NodeDouble, etc. in Tree.h
    //but that would complicate the code even more than this way.
    
    Node<int>* createTree(TreeType type, int rootkey){
        switch(type){
        case BinSearchTree:
            return new BinaryNodeCompos<int>(rootkey);
        default:
            return nullptr;
        }
    }
    
//    Node<long int>* createTree(TreeType type, long int rootkey){
//        switch(type){
//        case BinSearchTree:
//            return new BinaryNodeLeaf<long int>(rootkey);
//        default:
//            return nullptr;
//        }
//    }
    
//    Node<long long int>* createTree(TreeType type, long long int rootkey){
//        switch(type){
//        case BinSearchTree:
//            return new BinaryNodeLeaf<long long int>(rootkey);
//        default:
//            return nullptr;
//        }
//    }
    
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
