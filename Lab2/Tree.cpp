#include "Tree.h"

template<>
std::string Node<std::string>::getLongestWord() {
    std::string lw = "", rw = "", w = this->getKey();
    if (this->getLeft())
        lw = this->getLeft()->getLongestWord();
    if (this->getRight())
        rw = this->getRight()->getLongestWord();
    if (lw.length() > rw.length())
        return lw.length() > w.length() ? lw : w;
    else
        return rw.length() > w.length() ? rw : w;
    return w;
}


//template<>
//void BinaryNodeCompos<std::string>::setSNChild() {
//    if (!this->parent_)
//        return;
//    if (std::stod(this->key_) < std::stod(this->parent_->getKey()))
//        this->parent_->setLeft(this);
//    else
//        this->parent_->setRight(this);
//}
