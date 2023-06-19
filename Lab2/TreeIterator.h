#ifndef TREEITERATOR_H
#define TREEITERATOR_H

//template<typename T>
//class Node;

template <typename KeyType>
class TreeIterator{
protected:
    Node<KeyType>* First_;
    Node<KeyType>* Last_;
    Node<KeyType>* iter_;
    Node<KeyType>* max_;
public:
    TreeIterator() : First_(nullptr), Last_(nullptr), iter_(nullptr), max_(nullptr){}
    virtual ~TreeIterator() {}
    virtual void First(){ iter_ = First_; }
    virtual void Next(){}
    virtual bool isDone(){
        return (iter_ == Last_);
    }
    virtual Node<KeyType>* Current(){
        return iter_;
    }
};

template<typename KeyType>
class InOrderIterator : public TreeIterator<KeyType>{
public:
    InOrderIterator() {}
    InOrderIterator(Node<KeyType>* root) {
        this->First_ = root->getMin();
        this->iter_ = this->First_;
        this->max_ = root->getMax()->AddDummy();
        this->Last_ = this->max_->getRight();
    }
    ~InOrderIterator() {
        this->max_ = this->max_->RemoveDummy();
    }
    void Next() override{
        if (this->iter_->getRight()) {
            this->iter_ = this->iter_->getSuccessor();
        }
        else {
            Node<KeyType>* parent = this->iter_->getParent();
            while (/*parent->getRight() ||*/ parent->getRight() == this->iter_) {
                this->iter_ = parent;
                parent = parent->getParent();
            }
            this->iter_ = parent;
        }
    }
};

template<typename KeyType>
class PreOrderIterator : public TreeIterator<KeyType> {
public:
    PreOrderIterator(){}
    PreOrderIterator(Node<KeyType>* root){
        this->First_ = root;
        this->iter_ = this->First_;
        this->max_ = root->getMax()->AddDummy();
        this->Last_ = this->max_->getRight();
    }
    ~PreOrderIterator() {
        this->max_ = this->max_->RemoveDummy();
    }
    void Next() override {
        if (this->iter_->getLeft()) {
            this->iter_ = this->iter_->getLeft();
        }
        else if (this->iter_->getRight())
        {
            this->iter_ = this->iter_->getRight();
        }
        else {
            Node<KeyType>* parent = this->iter_->getParent();
            while (!parent->getRight() || parent->getRight() == this->iter_)
            {
                this->iter_ = parent;
                parent = parent->getParent();
            }
            this->iter_ = parent->getRight();
        }
    }
};

template<typename KeyType>
class PostOrderIterator : public TreeIterator<KeyType> {
public:
    PostOrderIterator() {}
    PostOrderIterator(Node<KeyType>* root) {
        this->First_ = root->getMin();
        this->iter_ = this->First_;
        this->max_ = root;
        this->Last_ = root->setDummyParent();
    }
    ~PostOrderIterator() {
        this->max_ = this->max_->RemoveDummyParent();
    }
    void Next() override {
        if (this->iter_->getParent()->getRight() && this->iter_->getParent()->getRight() != this->iter_) {
            this->iter_ = this->iter_->getParent()->getRight();
            while (this->iter_->getLeft() || this->iter_->getRight()) {
                if (this->iter_->getLeft()) {
                    this->iter_ = this->iter_->getLeft();
                }
                else
                    this->iter_ = this->iter_->getRight();
            }
        }
        else {
            this->iter_ = this->iter_->getParent();
        }
    }
};

#endif // TREEITERATOR_H
