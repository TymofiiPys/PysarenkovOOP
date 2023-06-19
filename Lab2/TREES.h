#ifndef TREES_H
#define TREES_H

#include "qtablewidget.h"
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QMessageBox>
#include <cmath>
#include <string>
#include <memory>
#include <time.h>
#include <vector>
#include <fstream>

//Converter
template <typename T>
class Converter {
private:
    T key_;
public:
    Converter() {};
    Converter(T key) : key_(key) {};
    operator int() {
        return int(key_);
    }
    operator std::string() {
        return std::to_string(key_);
    }
    operator double() {
        return double(key_);
    }
};

template<>
inline Converter<std::string>::operator int() {
    return std::stoi(key_);
}

template<>
inline Converter<std::string>::operator double() {
    return std::stod(key_);
}

template<>
inline Converter<std::string>::operator std::string() {
    return key_;
}


//Tree
enum Color{
    BLACK, RED
};

enum TreeType{
    BinSearchTree,
};

enum TypeNode {
    IntNode, StringNode, DoubleNode
};

template<typename T>
class TreeIterator;

template<typename T>
class InOrderIterator;

template<typename T>
class PreOrderIterator;

template<typename T>
class PostOrderIterator;

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
    //TypeNode type_;
    //! Parent of the node
    Node* parent_;
public:
    Node() : key_(T(Converter<int>(0))),  parent_(nullptr){}
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
    virtual void AddToStringedNode(std::string key){}
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
    * Removal of a node from the tree (used for traversal)
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
    Node* setDummyParent() {
        this->parent_ = new Node();
        this->parent_->setRight(this);
        return this->parent_;
    }
    Node* RemoveDummyParent() {
        Node* p = this->parent_;
        this->parent_ = nullptr;
        delete p;
        return this;
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
    virtual void setChild() {
        if (!this->parent_)
            return;
        if (this->key_ < this->parent_->getKey())
            this->parent_->setLeft(this);
        else
            this->parent_->setRight(this);
    }
    TreeIterator<T> *CreateInOrderIterator(){
        return new InOrderIterator<T>(this);
    }
    TreeIterator<T> *CreatePreOrderIterator(){
        return new PreOrderIterator<T>(this);
    }
    TreeIterator<T>* CreatePostOrderIterator() {
        return new PostOrderIterator<T>(this);
    }
    int getHeight(int depth = 1) {
        int h = depth;
        int lh = h, rh = h;
        if (this->getLeft())
            lh = this->getLeft()->getHeight(depth+1);
        if(this->getRight())
            rh = this->getRight()->getHeight(depth+1);
        if (lh > rh)
            h = lh;
        else
            h = rh;
        return h;
    }
    std::string getLongestWord() { return ""; }
    T getSumOfAllNodes() {
        T ls = 0, rs = 0;
        if (this->getLeft())
            ls = this->getLeft()->getSumOfAllNodes();
        if (this->getRight())
            rs = this->getRight()->getSumOfAllNodes();
        return this->getKey() + ls + rs;
    }
    /*Snapshot* getSnapshot(TypeNode type, std::string descr) {
        return new Snapshot(this, type, descr);
    }*/
};

template<>
inline std::string Node<std::string>::getLongestWord() {
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
    BinaryNodeCompos() : Node<T>(), NodeCompos<T>(){}
    BinaryNodeCompos(T key) : Node<T>(key), NodeCompos<T>(){}
    BinaryNodeCompos(BinaryNodeLeaf<T> *leaf) : Node<T>(leaf->getKey(), leaf->getParent()), NodeCompos<T>(){
        this->setChild();
    }   
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
    void AddToStringedNode(std::string key) override{
        return;
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
            return toLeaf;
        }
        return this;
    };
    Node<T>* AddDummy() override {
        this->setRight(new BinaryNodeLeaf<T>());
        return this;
    }
    Node<T>* RemoveDummy() override {
        Node<T>* dummy = this->getRight();
        delete this->getRight();
        this->right_ = nullptr;
        if(!this->getLeft())
            return new BinaryNodeLeaf<T>(this);
        return this;
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
    void setSNChild() {
        return;
    }
};

template<>
inline void BinaryNodeCompos<std::string>::setSNChild() {
    if (!this->parent_)
        return;
    if (std::stod(this->key_) < std::stod(this->parent_->getKey()))
        this->parent_->setLeft(this);
    else
        this->parent_->setRight(this);
}

//! Class for leaf Binary Search Tree nodes
template<typename T>
class BinaryNodeLeaf : public BinaryNode<T>, public NodeLeaf<T>{
public:
    BinaryNodeLeaf() : Node<T>(), NodeLeaf<T>(){}
    BinaryNodeLeaf(T key) : Node<T>(key), NodeLeaf<T>(){}
    BinaryNodeLeaf(BinaryNodeCompos<T>* comp) : Node<T>(comp->getKey(), comp->getParent()), NodeLeaf<T>() {
        this->setChild();
    }
    void Add(T key) override{
        //Conversion of leaf to composite
        BinaryNodeCompos<T>* toCompos = new BinaryNodeCompos<T>(this);
        //Addition of the key to a child node of the converted node
        toCompos->Add(key);
        delete this;
    }
    void AddToStringedNode(std::string key) override {
        return;
    }
    Node<T>* AddDummy() override{
        BinaryNodeCompos<T>* toCompos = new BinaryNodeCompos<T>(this);
        toCompos->setRight(new BinaryNodeLeaf<T>());
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
};

template<>
inline void BinaryNodeCompos<std::string>::AddToStringedNode(std::string key) {
    if (std::stod(key) < std::stod(this->key_))
    {
        if (this->left_)
            this->left_->AddToStringedNode(key);
        else
        {
            this->left_ = new BinaryNodeLeaf<std::string>(key);
            this->left_->setParent(this);
        }
    }
    else {
        if (this->right_)
            this->right_->AddToStringedNode(key);
        else
        {
            this->right_ = new BinaryNodeLeaf<std::string>(key);
            this->right_->setParent(this);
        }
    }
}

template<>
inline void BinaryNodeLeaf<std::string>::AddToStringedNode(std::string key) {
    BinaryNodeCompos<std::string>* toCompos = new BinaryNodeCompos<std::string>(this->getKey());
    toCompos->setParent(this->getParent());
    //Addition of the key to a child node of the converted node
    toCompos->Add(key);
    toCompos->setSNChild();
    delete this;
}


//TreeFactory
class TreeFactory{
private:
//    static TreeFactory* treefact;
protected:
    TreeFactory() {};
    ~TreeFactory() {};
public:
    TreeFactory(TreeFactory &other) = delete;
    void operator=(const TreeFactory &) = delete;
    static TreeFactory& getTreeFac();
    
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

//TreeFactory* TreeFactory::treefact = nullptr;

inline TreeFactory& TreeFactory::getTreeFac(){
    static TreeFactory treefact;
    return treefact;
}


//TreeIterator
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
    virtual Node<KeyType>* Last(){
        return Last_;
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

enum TravType {
    InOrder, PreOrder, PostOrder
};


//TreeIterStrategy
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


//StringedNode
class StringedNode {
private:
    Node<std::string>* root_;
    std::string type_;
public:
    StringedNode() {};
    StringedNode(Node<std::string>* root, TypeNode type) : root_(root){
        switch (type) {
        case IntNode:
        {type_ = "int"; break; }
        case DoubleNode:
        {type_ = "double"; break; }
        case StringNode:
        {type_ = "string"; break; }
        default:
        {break; }
        }
    }
    virtual StringedNode* getStrNode() {
        return this;
    }
    Node<std::string>* getTree() {
        return this->root_;
    }
    TypeNode getType() {
        TypeNode t = StringNode;
        if (type_ == "int")
            t = IntNode;
        else if (type_ == "double")
            t = DoubleNode;
        return t;
    }
    std::string getTypeStr() {
        return this->type_;
    }
};


//TreesTemplate
template <typename KeyType>
class TreeAnalyzer {
protected:
    Node<KeyType>* root_;
public:
    TreeAnalyzer(){}
    virtual ~TreeAnalyzer(){}
    TreeAnalyzer(Node<KeyType>* root) : root_(root){}
    std::string Analyze() {
        if (!root_)
        {
            return "The tree is not set or is null\n";
        }
        std::string ret = "";
        int h = this->findHeight();
        ret += "Height: " + std::to_string(h) + "\n";
        std::string min = this->findMin();
        ret += min + "\n";
        std::string max = this->findMax();
        ret += max;
        std::string lword = this->findLongestWord();
        if (lword != "")
            ret += "\n" + lword;
        std::string sum = this->findSumOfAllNodes();
        if (sum != "")
            ret += "\n" + sum;
        return ret;
    }
    int findHeight() {
        return root_->getHeight();
    }
    virtual std::string findMin(){
        return "Least element: ";
    }
    virtual std::string findMax() {
        return "Greatest element: ";
    }
    virtual std::string findLongestWord() { return ""; }
    virtual std::string findSumOfAllNodes() { return ""; }
};

template <typename Number>
class NumberTreeAnalyzer : public TreeAnalyzer<Number> {
public:
    NumberTreeAnalyzer(Node<Number>* root) : TreeAnalyzer<Number>(root) {}
    std::string findMin() override {
        return TreeAnalyzer<Number>::findMin() + std::to_string(this->root_->getMin()->getKey());
    }
    std::string findMax() override {
        return TreeAnalyzer<Number>::findMax() + std::to_string(this->root_->getMax()->getKey());
    }
    std::string findSumOfAllNodes() override {
        return "Sum of all keys in the tree: " + std::to_string(this->root_->getSumOfAllNodes());
    }
};

template<>
inline std::string NumberTreeAnalyzer<std::string>::findMin(){
    return "";
}

template<>
inline std::string NumberTreeAnalyzer<std::string>::findMax(){
    return "";
}

template<>
inline std::string NumberTreeAnalyzer<std::string>::findSumOfAllNodes(){
    return "";
}


template<typename T>
class StringTreeAnalyzer : public TreeAnalyzer<T> {
public:
    StringTreeAnalyzer(Node<T>* root) : TreeAnalyzer<T>(root) {}
    std::string findMin() override {return "";};
        
    std::string findMax() override {
        return "";
    }
    std::string findLongestWord() override {
        return "";
    }
};

template<>
inline std::string StringTreeAnalyzer<std::string>::findMin(){
    return TreeAnalyzer::findMin() + this->root_->getMin()->getKey();
}

template<>
inline std::string StringTreeAnalyzer<std::string>::findMax(){
    return TreeAnalyzer::findMax() + this->root_->getMax()->getKey();
}

template<>
inline std::string StringTreeAnalyzer<std::string>::findLongestWord(){
    std::string theword = this->root_->getLongestWord();
    return "Longest word: " + theword + ", of length: " + std::to_string(theword.length());
}

template <typename T>
class TNodeAdapter : public StringedNode{
private:
    Node<T>* root_;
    TypeNode type_;
public:
    TNodeAdapter(Node<T>* root, TypeNode type) : root_(root), type_(type){}
    StringedNode* getStrNode() override {
        //if (type_ == StringNode) {
        //	return new StringedNode(root_, type_);
        //}
        TreeFactory& factory = TreeFactory::getTreeFac();
        Node<std::string>* sn = factory.createTree(BinSearchTree, std::to_string(root_->getKey()));
        if (root_->getLeft())
            addNodes(sn, root_->getLeft());
        if(root_->getRight())
            addNodes(sn, root_->getRight());
        return new StringedNode(sn, type_);
    }
    void addNodes(Node<std::string>* dest, Node<T>* source) {
        dest->AddToStringedNode(std::to_string(source->getKey()));
        if (source->getLeft())
        {
            addNodes(dest, source->getLeft());
        }
        if (source->getRight())
        {
            addNodes(dest, source->getRight());
        }
        return;
    }
};

template <>
inline StringedNode* TNodeAdapter<std::string>::getStrNode(){
    return new StringedNode(root_, type_);
}

template <typename T>
class StringedNodeAdapter : public Node<T> {
private:
    StringedNode* root_;
public:
    StringedNodeAdapter(StringedNode* root) : root_(root) {}
    Node<T>* getNode() {
        TreeFactory& factory = TreeFactory::getTreeFac();
        Node<std::string>* sn = root_->getTree();
        
        Node<T>* n = factory.createTree(BinSearchTree, T(Converter<std::string>(sn->getKey())));
        if(sn->getLeft())
            addNodes(n, sn->getLeft());
        if(sn->getRight())
            addNodes(n, sn->getRight());
        return n;
    }
    void addNodes(Node<T>* dest, Node<std::string>* source) {
        dest->Add(T(Converter<std::string>(source->getKey())));
        if (source->getLeft())
        {
            addNodes(dest, source->getLeft());
        }
        if (source->getRight())
        {
            addNodes(dest, source->getRight());
        }
        return;
    }
};

template <>
inline Node<std::string>* StringedNodeAdapter<std::string>::getNode() {
    return this->root_->getTree();
}

class BinaryTreeAdapter {
    
};

class Snapshot {
private:
    StringedNode* root_;
    std::string tt_;
    std::string descr_;
    std::string date_;
public:
    template<typename T>
    Snapshot(Node<T>* root, TypeNode type, TreeType tt, std::string descr) : descr_(descr) {
        TNodeAdapter<T>* adapter = new TNodeAdapter<T>(root, type);
        this->root_ = adapter->getStrNode();
        delete adapter;
        time_t now = time(0);
        switch (tt) {
        case BinSearchTree:
            tt_ = "BST";
        default:
            break;
        }
        this->date_ = ctime(&now)/*.substr(0, 19)*/;
    };
    template<typename T>
    Node<T>* getTree() {
        StringedNodeAdapter<T>* adapter = new StringedNodeAdapter<T>(this->root_);
        Node<T>* n = adapter->getNode();
        delete adapter;
        return n;
    }
    StringedNode* getStrNode() {
        return this->root_;
    }
    TreeType getTreeType() {
        if (tt_ == "BST")
            return BinSearchTree;
    }
    std::string getTreeTypeStr() {
        return tt_;
    }
    std::string getName() {
        return "(" + this->date_ + ") " + this->descr_;
    }
    std::string getDate() {
        return this->date_;
    }
};

template<typename T>
class Command;

template<typename T>
class SaveCommand;

template<typename T>
class UndoCommand;

template<typename T>
class TreeFacade {
private:
    Node<T>* root_;
    TypeNode ntype_;
    TreeType ttype_;
    std::string last_action_desc_;
    std::string save_to;
    QWidget* form_;
    QTableWidget* tablewidget;
    Command<T>* savecom;
    Command<T>* undocom;
    void SyncSnaps(std::vector<Snapshot*> s) {
        this->savecom->setSnaps(s);
        this->undocom->setSnaps(s);
    }
public:
    TreeFacade() : root_(nullptr){
        this->savecom = new SaveCommand<T>(this);
        this->undocom = new UndoCommand<T>(this);
    }
    TreeFacade(std::string ntype, std::string ttype, QWidget* form = nullptr, QTableWidget* tw = nullptr, std::string saveto = "tree.txt") : TreeFacade<T>() {
        if (ntype == "Integer")
            this->ntype_ = IntNode;
        else if (ntype == "Double")
            this->ntype_ = DoubleNode;
        else if(ntype == "String")
            this->ntype_ = StringNode;
        if (ttype == "Binary Search Tree")
            this->ttype_ = BinSearchTree;
        this->save_to = saveto;
        this->form_ = form;
        this->tablewidget = tw;
    }
    void AddKey(std::string key) {
        if (!root_) {
            TreeFactory& factory = TreeFactory::getTreeFac();
            root_ = factory.createTree(ttype_, T(Converter<std::string>(key)));
        }
        else {
            root_->Add(T(Converter<std::string>(key)));
            savecom->makeBackup();
            SyncSnaps(savecom->getSnaps());
        }
        if(tablewidget)
            Draw();
    }
    void RemoveKey(std::string key) {
        if (!root_)
            return;
        if(!root_->Search(T(Converter<std::string>(key)))){
            QMessageBox::critical(form_, "Key not found", "KEY NOT FOUND!");
            return;
        }
        root_->Remove(T(Converter<std::string>(key)));
        if(tablewidget)
            Draw();
    }
    void SearchKey(std::string key){
        if (!root_)
            return;
        Node<T>* search = root_->Search(T(Converter<std::string>(key)));
        if(!root_->Search(T(Converter<std::string>(key)))){
            QMessageBox::critical(form_, "Key not found", "KEY NOT FOUND!");
            return;
        }
        Draw(search);
    }
    void getMin(){
        if (!root_)
            return;
        Node<T>* search = root_->getMin();
         Draw(search);
    }
    void getMax(){
         if (!root_)
            return;
         Node<T>* search = root_->getMax();
         Draw(search);
    }
    Snapshot* getSnapshot() {
        return new Snapshot(root_, ntype_, ttype_, last_action_desc_);
    }
    void restore(Snapshot* snap) {
        this->root_ = snap->getTree<T>();
        this->ntype_ = snap->getStrNode()->getType();
        this->ttype_ = snap->getTreeType();
        SyncSnaps(undocom->getSnaps());
    }
    std::string Iterate(std::string travtype) {
        if (!root_)
            return "";
        TravType tt;
        if (travtype == "Inorder") tt = InOrder;
        else if (travtype == "Preorder") tt = PreOrder;
        else if (travtype == "Postorder") tt = PostOrder;
        Traverser<T>* trav = new Traverser<T>(tt);
        std::string out = trav->Iterate(root_);
        return out;
    }
    void changeSavePath(std::string newsp){
        this->save_to = newsp;
    }
    std::string getSavePath(){
        return save_to;
    }
    void Save() {
        savecom->execute();
    }
    void Undo() {
        undocom->execute();
        if(tablewidget)
            Draw();
    }
    std::string Analyze(){
        TreeAnalyzer<T> *analyzer;
        if(ntype_ == StringNode){
            analyzer = new StringTreeAnalyzer<T>(this->root_);
        }
        else{
            analyzer = new NumberTreeAnalyzer<T>(this->root_);
        }
        std::string out = analyzer->Analyze();
        return out;
    }
    void Draw(Node<T>* highlighted){
        tablewidget->clearContents();
        TreeIterator<T> *it = root_->CreatePreOrderIterator();
        int i = 0;
        int count = 0;
        for (it->First(); !it->isDone(); it->Next()) {
            count++;
        }
        tablewidget->setRowCount(count+1);
        for (it->First(); !it->isDone(); it->Next()) {
            tablewidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(std::string(Converter<T>(it->Current()->getKey())))));
            if(it->Current()->isLeaf() || (!it->Current()->getLeft() && it->Current()->getRight() == it->Last()))
                tablewidget->setItem(i, 1, new QTableWidgetItem("Лист"));
            else{
                if(it->Current()->getLeft())
                    tablewidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(std::string(Converter<T>(it->Current()->getLeft()->getKey())))));
                if(it->Current()->getRight() && it->Current()->getRight() != it->Last())
                    tablewidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(std::string(Converter<T>(it->Current()->getRight()->getKey())))));
            }
            if(it->Current()->getKey() == highlighted->getKey()){
                tablewidget->setRangeSelected(QTableWidgetSelectionRange(i, 0, i, 2), true);
            }
            i++;
        }
        delete it;
    }
    void Draw(){
        tablewidget->clearContents();
        TreeIterator<T> *it = root_->CreatePreOrderIterator();
        int i = 0;
        int count = 0;
        for (it->First(); !it->isDone(); it->Next()) {
            count++;
        }
        tablewidget->setRowCount(count+1);
        for (it->First(); !it->isDone(); it->Next()) {
            tablewidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(std::string(Converter<T>(it->Current()->getKey())))));
            if(it->Current()->isLeaf() || (!it->Current()->getLeft() && it->Current()->getRight() == it->Last()))
                tablewidget->setItem(i, 1, new QTableWidgetItem("Лист"));
            else{
                if(it->Current()->getLeft())
                    tablewidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(std::string(Converter<T>(it->Current()->getLeft()->getKey())))));
                if(it->Current()->getRight() && it->Current()->getRight() != it->Last())
                    tablewidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(std::string(Converter<T>(it->Current()->getRight()->getKey())))));
            }
            i++;
        }
        delete it;
//        QGraphicsScene *s = new QGraphicsScene(0, -10, graphviewtree->width(), graphviewtree->height(), graphviewtree);
//        std::vector<Node<T>*> nodelevel;
//        std::vector<Node<T>*> parentlevel;
//        std::vector<Node<T>*> nextlevel;
//        graphviewtree->setScene(s);
//        int height = root_->getHeight();
//        int depth = 2;
//        qreal width = 75;
//        qreal gap = 20;
//        qreal curgap;
//        qreal x;
//        qreal y;
//        parentlevel.push_back(root_);
//        if(!root_->getLeft())
//            nodelevel.push_back(nullptr);
//        else{
//            nodelevel.push_back(root_->getLeft());
//        }
//        if(!root_->getRight())
//            nodelevel.push_back(nullptr);
//        else{
//            nodelevel.push_back(root_->getRight());
//        }
//        s->addEllipse(graphviewtree->width()/2 - 37.5, 0, 75, 75, QPen(Qt::black), QBrush(Qt::white));
//        s->addText(QString::fromStdString(std::string(Converter<T>(root_->getKey()))))->setPos(graphviewtree->width()/2 - 12.5 ,25);
//        y = 100;
//        int i = 0;
//        while(depth <= height){
//            x = -(width+gap)*(pow(2, (depth-2))) + gap/2 ;
//            if(depth != height){
//                nextlevel = std::vector<Node<T>*>(pow(2, depth));
//                for(Node<T>* n : nodelevel){
//                    if(n != nullptr)
//                    {
//                        if(!n->getLeft())
//                            nextlevel[i] = nullptr;
//                        else{
//                            nextlevel[i] = n->getLeft();
//                        }
//                        i++;
//                        if(!n->getRight())
//                            nextlevel[i] = nullptr;
//                        else{
//                            nextlevel[i] = n->getRight();
//                        }
//                        i++;
//                        s->addEllipse(graphviewtree->width()/2 + x, y, 75, 75, QPen(Qt::black), QBrush(Qt::white));
//                        s->addText(QString::fromStdString(std::string(Converter<T>(n->getKey()))))->setPos(graphviewtree->width()/2 + x + 25 , y + 25);
//                    }
//                    else{
//                        nextlevel.push_back(nullptr);
//                        nextlevel.push_back(nullptr);
//                    }
//                    x += width + gap;
//                }
//                parentlevel = nodelevel;
//                nodelevel = nextlevel;
//            }
//            else{
//                for(Node<T>* n : nodelevel){
//                    if(n != nullptr)
//                    {
//                        s->addEllipse(graphviewtree->width()/2 + x, y, 75, 75, QPen(Qt::black), QBrush(Qt::white));
//                        s->addText(QString::fromStdString(std::string(Converter<T>(n->getKey()))))->setPos(graphviewtree->width()/2 + x + 25 , y + 25);
//                    }
//                    x += width + gap;
//                }
//            }
//            depth++;
//            y+= 100;
//        }
//        graphviewtree->show();
    }
};

template<typename T>
class Command {
protected:
    std::vector<Snapshot*> snaps;
    TreeFacade<T>* fac;
    //StringedNode* root;
public:
    Command() {}
    Command(TreeFacade<T>* facc) : fac(facc) {};
    virtual void execute() {};
    void makeBackup() {
        if (this->snaps.size() <= 50)
            this->snaps.push_back(fac->getSnapshot());
    }
    std::vector<Snapshot*> getSnaps() {
        return snaps;
    }
    void setSnaps(std::vector<Snapshot*> s) {
        this->snaps = s;
    }
};

template<typename T>
class SaveCommand : public Command<T> {
public:
    SaveCommand() : Command<T>() {};
    SaveCommand(TreeFacade<T>* facc) : Command<T>(facc) {};
    void execute() override {
        std::ofstream output(this->fac->getSavePath());
        Snapshot* snap = this->snaps.back();
        StringedNode* sn = snap->getStrNode();
        output << sn->getTypeStr() << "\n" << snap->getTreeTypeStr() << "\n";
        Node<std::string>* n = sn->getTree();
        Traverser<std::string>* trav = new Traverser<std::string>(PreOrder);
        std::string out = trav->Iterate(n);
        output << out;
        output.close();
    }
};

template<typename T>
class UndoCommand : public Command<T> {
public:
    UndoCommand() : Command<T>() {};
    UndoCommand(TreeFacade<T>* facc) : Command<T>(facc) {};
    void execute() override {
        if (!(this->snaps.size() - 1))
            return;
        this->snaps.pop_back();
        Snapshot* snap = this->snaps.back();
        this->fac->restore(snap);
    }
};

#endif // TREES_H
