#ifndef TREE_H
#define TREE_H

#include <vector>


enum Color{
    BLACK, RED
};

template<typename T>
class TreeIterator;

/**
 * \interface Node Tree.h
 *
 * \brief Interface which declares operations and fields common for all of the trees and their nodes implemented in the program (except for B-tree)
 */
template<typename T>
class Node{
protected:
    //! Key of the node, can be of any type (int/double/char/std::string)
    T key_;
    //! Parent of the node
    Node* parent_;
public:
    Node() : key_(NULL), parent_(nullptr){}
    Node(T key) : key_(key), parent_(nullptr){}
    Node(T key, Node* parent) : key_(key), parent_(parent){}
    /* Destructor*/
    virtual ~Node(){}
    /*
    * Addition of a key to the tree
    * 
    * \param key Key which will be added to the tree
    */
    virtual void Add(T key){}
    /*
    * Addition of a node to the tree (used for traversal)
    */
    virtual Node* AddDummy() { return this; }
    //! 
    //! Search for a key in the tree
    //! 
    //! \param Key which will be found in the tree
    //! 
    virtual Node* Search(T key) { return nullptr; }
    //!
    //! Removal of a node containing the key
    //! 
    //! \param key Node, containing this key, will be removed
    //! 
    virtual Node<T>* Remove(T key) { return this; }
    /*
    * Removal of a node to the tree (used for traversal)
    */
    virtual Node* RemoveDummy() { return this; }
    /** Set left child of the node
    *
    * \param n Node which is set as a left child of this node
    */
    virtual void setLeft(Node* n){}
    /** Set right child of the node
    *
    * \param n Node which is set as a right child of this node
    */
    virtual void setRight(Node* n){}
    /** Get left child of the node
    *
    * \return Node which is the left child of this node
    */
    virtual Node* getLeft() { return nullptr; }
    /** Get right child of the node
    *
    * \return Node which is the right child of this node
    */
    virtual Node* getRight() { return nullptr; }
    //! Set parent node
    /*
    * \param p Node which is set as parent of this node
    */
    void setParent(Node* p){
        this->parent_ = p;
    }
    //! 
    //! Get parent of the node
    //! 
    //! \return Parent of the node
    //! 
    Node* getParent() {
        return this->parent_;
    }
    //!
    //!  Get key of the node
    //! 
    //! \return Key stored in the node
    //! 
    T getKey() {
        return this->key_;
    }
    //!
    //!  Get successor of the node 
    //! 
    //! \return Node-successor (node with next greater key)
    //! 
    virtual Node* getSuccessor() { return nullptr; }
    //!
    //! Get node with the least key
    //! 
    //! \return Node with the least key in a tree rooted by this node
    //! 
    virtual Node* getMin() { return nullptr; }
    //!
    //! Get node with the greatest key
    //! 
    //! \return Node with the greatest key in a tree rooted by this node
    //! 
    virtual Node* getMax() { return nullptr; }
    //! 
    //! Check whether the node is leaf
    //! 
    //! \return true if the node is leaf, false if the node is composite
    //! 
    virtual bool isLeaf(){
        return false;
    }
    TreeIterator<T> *CreateIterator(){
        return new TreeIterator<T>(this);
    }
};

/**
* \brief Class for composite nodes and tree roots with no children
*
* Class which represents nodes with children
*/
template<typename T>
class NodeCompos : virtual public Node<T>{
protected:
    //! Left child of the node
    Node<T>* left_;
    //! Right child of the node
    Node<T>* right_;
public:
    NodeCompos() : left_(nullptr), right_(nullptr){}
    NodeCompos(Node<T>* left, Node<T>* right) : left_(left), right_(right){}
    void setLeft(Node<T>* n) override {
        this->left_ = n;
    }
    void setRight(Node<T>* n) override {
        this->right_ = n;
    }
    Node<T>* getLeft() override {
        return this->left_;
    }
    Node<T>* getRight() override {
        return this->right_;
    }
};

//! Class for leaf nodes
/*
* Class which represents nodes without children
*/
template<typename T>
class NodeLeaf : virtual public Node<T>{
public:
    NodeLeaf(){}
    bool isLeaf() override{
        return true;
    }
};

//! Class for Binary Search Tree nodes
template<typename T>
class BinaryNode : virtual public Node<T>{
public:
    BinaryNode(){
        
    }
};

template<typename T>
class BinaryNodeLeaf;

//! Class for composite Binary Search Tree nodes
template<typename T>
class BinaryNodeCompos : public BinaryNode<T>, public NodeCompos<T>{
public:
    BinaryNodeCompos(){}
    BinaryNodeCompos(T key) : Node<T>(key), NodeCompos<T>(){}
    BinaryNodeCompos(BinaryNodeLeaf<T> *leaf) : Node<T>(leaf->getKey(), leaf->getParent()), NodeCompos<T>(){}   
    void Add(T key) override{
        if(key < this->key_)
        {
            if(this->left_)
                this->left_->Add(key);
            else //when there is right but there's no left child
            {
                this->left_ = new BinaryNodeLeaf<T>(key);
                this->left_->setParent(this);
            }
        }
        else{
            if(this->right_)
                this->right_->Add(key);
            else
            {
                this->right_ = new BinaryNodeLeaf<T>(key);
                this->right_->setParent(this);
            }
        }
    }
    Node<T>* Search(T key) override{
        Node<T>* b = nullptr;
        if (key < this->key_)
        {
            if (this->left_)
                b = this->left_->Search(key);
            else
                return nullptr;
        }
        else if (key > this->key_) {
            if (this->right_)
                b = this->right_->Search(key);
            else
                return nullptr;
        }
        else
            return this;
        return b;
    }
    Node<T>* Remove(T key) override {
        if (key < this->key_)
        {
            if (this->left_)
                this->left_ = this->left_->Remove(key);
            else
                return this;
        }
        else if (key > this->key_) {
            if (this->right_)
                this->right_ = this->right_->Remove(key);
            else
                return this;
        }
        else {
            if (!this->left_)
            {
                Node<T>* right = this->right_;
                right->setParent(this->parent_);
                delete this;
                return right;
            }
            if (!this->right_) {
                Node<T>* left = this->left_;
                left->setParent(this->parent_);
                delete this;
                return left;
            }
            else {
                Node<T>* succ = this->getSuccessor();
                this->key_ = succ->getKey();
                this->right_ = this->right_->Remove(this->key_);
            }
        }
        //if there is no children anymore
        if (!this->left_ && !this->right_) {
            BinaryNodeLeaf<T>* toLeaf = new BinaryNodeLeaf<T>(this);
            toLeaf->setChild();
            return toLeaf;
        }
        return this;
    };
    Node<T>* RemoveDummy() override {
        delete this->getRight();
        BinaryNodeLeaf<T>* toLeaf = new BinaryNodeLeaf<T>(this);
        toLeaf->setChild();
        return toLeaf;
    }
    Node<T>* getSuccessor() override {
        return this->right_->getMin();
    }
    Node<T>* getMin() override {
        Node<T>* min;
        if (this->left_)
            min = this->left_->getMin();
        else
            min = this;
        return min;
    }
    Node<T>* getMax() override {
        Node<T>* max;
        if (this->right_)
            max = this->right_->getMax();
        else
            max = this;
        return max;
    }
    //!Set this node as a child of its parent
    void setChild() {
        if (!this->parent_)
            return;
        if (this->key_ < this->parent_->getKey())
            this->parent_->setLeft(this);
        else
            this->parent_->setRight(this);
    }
    //!Set this node as a child of passed parent
    //! \param parent Node, which will have this as a child
    void setChild(BinaryNodeCompos<T>* parent) {
        if (!this->parent_)
            return;
        if (this->key_ < parent->getKey())
            parent->setLeft(this);
        else
            parent->setRight(this);
    }
};

//! Class for leaf Binary Search Tree nodes
template<typename T>
class BinaryNodeLeaf : public BinaryNode<T>, public NodeLeaf<T>{
public:
    BinaryNodeLeaf(T key) : Node<T>(key), NodeLeaf<T>(){}
    BinaryNodeLeaf(BinaryNodeCompos<T>* comp) : Node<T>(comp->getKey(), comp->getParent()), NodeLeaf<T>() {}
    void Add(T key) override{
        //Conversion of leaf to composite
        BinaryNodeCompos<T>* toCompos = new BinaryNodeCompos<T>(this);
        toCompos->setChild();
        //Addition of the key to a child node of the converted node
        toCompos->Add(key);
        delete this;
    }
    Node<T>* AddDummy() override{
        //Conversion of leaf to composite
        BinaryNodeCompos<T>* toCompos = new BinaryNodeCompos<T>(this);
        toCompos->setChild();
        //Addition of the key to a child node of the converted node
        toCompos->setRight(new BinaryNodeLeaf<T>((T)NULL));
        return toCompos;
    }
    Node<T>* Search(T key) override {
        if (this->key_ == key)
            return this;
        return nullptr;
    }
    Node<T>* Remove(T key) override {
        if (this->key_ == key)
        {
            delete this;
            return nullptr;
        }
        return this;
    }
    //Node<T>* getSuccessor() override {
    //    return this->right_->getMin();
    //}
    Node<T>* getMin() override {
        return this;
    }
    Node<T>* getMax() override {
        return this;
    }
    void setChild() {
        if (!this->parent_)
            return;
        if (this->key_ < this->parent_->getKey())
            this->parent_->setLeft(this);
        else
            this->parent_->setRight(this);
    }
};

//template<class T>
//class BNode : public Node<T>{
//private:
//    std::vector<T> keys;
//    std::vector<BNode*> children;
//    int order;
//public:
//    void Add(T key) override{};
//    void Remove() override{};
//};

//template<class T>
//class RedBlackNode : public Node<T>{
//private:
//    bool color;
//public:
//    RedBlackNode(){
        
//    }
//    RedBlackNode(T key){
//        this->key_ = key;
//    }
//    void Add(T key) override{};
//    void Remove() override{};
//};

//template<class T>
//class TreapNode : public Node<T>{
//private:
//    int treapheight;
//public:
//    void Add(T key) override{};
//    void Remove() override{};
//};

//template<class T>
//class AVLNode : public Node<T>{
//private:
//    //! 
//    short int avlheight;
//public:
//    void Add(T key) override{};
//    void Remove() override{};
//};


#endif // TREE_H
