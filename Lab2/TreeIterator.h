#ifndef TREEITERATOR_H
#define TREEITERATOR_H

#include "Tree.h"

template <typename KeyType>
class TreeIterator{
private:
    Node<KeyType>* First_;
    Node<KeyType>* Last_;
    Node<KeyType>* max_;
    Node<KeyType>* iter_;
public:
    TreeIterator(){}
    TreeIterator(Node<KeyType> *root) : First_(root->getMin()){
        iter_ = First_;
        max_ = root->getMax()->AddDummy();
        Last_ = max_->getRight();
    }
    ~TreeIterator() {
        max_ = max_->RemoveDummy();
    }
    void First(){
        iter_ = First_;
    }
    void Next(){
        if(iter_->getRight()){
            iter_ = iter_->getSuccessor();
        }
        else{
            Node<KeyType>* parent = iter_->getParent();
            while (parent->getRight() == iter_) {
                iter_ = parent;
                parent = parent->getParent();
            }
            iter_ = parent;
        }
    }
    bool isDone(){
        return (iter_ == Last_);
    }
    Node<KeyType>* Current(){
        return iter_;
    }
};

#endif // TREEITERATOR_H
