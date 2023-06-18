#ifndef TREEITERSTRATEGY_H
#define TREEITERSTRATEGY_H
#include <memory>
#include <iostream>
#include "Tree.h"

enum TravType{
    InOrder, PreOrder, PostOrder
};

template <typename T>
class TreeIterStrategy{
public:
    virtual ~TreeIterStrategy() = default;
    virtual std::string doTraverse(Node<T>* root){
        return "";
    }
};

template <typename T>
class InOrderStrategy : public TreeIterStrategy<T>{
public:
    std::string doTraverse(Node<T>* root) override{
        std::string r = "";
        TreeIterator<T> *it = root->CreateInOrderIterator();
        for (it->First(); !it->isDone(); it->Next()) {
            r += std::string(Converter<T>(it->Current()->getKey())) + "\n";
        }
        delete it;
        return r;
    }
};

template <typename T>
class PreOrderStrategy : public TreeIterStrategy<T> {
    std::string doTraverse(Node<T>* root) override {
        std::string r = "";
        TreeIterator<T>* it = root->CreatePreOrderIterator();
        for (it->First(); !it->isDone(); it->Next()) {
            r += std::string(Converter<T>(it->Current()->getKey())) + "\n";
        }
        delete it;
        return r;
    }
};

template <typename T>
class PostOrderStrategy : public TreeIterStrategy<T> {
    std::string doTraverse(Node<T>* root) override {
        std::string r = "";
        TreeIterator<T>* it = root->CreatePostOrderIterator();
        for (it->First(); !it->isDone(); it->Next()) {
            r += std::string(Converter<T>(it->Current()->getKey())) + "\n";
        }
        delete it;
        return r;
    }
};

template <typename T>
class Traverser{
private:
    std::unique_ptr<TreeIterStrategy<T>> strategy_;
public:
    Traverser(){}
    Traverser(TravType type){
        this->set_strategy(type);
    }
    void set_strategy(TravType type){
        switch(type){
        case InOrder:
        {strategy_ = std::make_unique<InOrderStrategy<T>>();break;}
        case PreOrder:
        {strategy_ = std::make_unique<PreOrderStrategy<T>>();break;}
        case PostOrder:
        {strategy_ = std::make_unique<PostOrderStrategy<T>>();break;}
        }
    }
    std::string Iterate(Node<T>* root){
        if(strategy_){
            return strategy_->doTraverse(root);
        }
        else{
            return "No strategy set\n";
        }
    }
};

#endif // TREEITERSTRATEGY_H
