#ifndef TREE_H
#define TREE_H
#include <vector>

enum Color{
    BLACK, RED
};

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
    //! 
    //! Search for a key in the tree
    //! 
    //! \param Key which will be found in the tree
    //! 
    virtual void Search(T key){}
    //!
    //! Removal of a node containing the key
    //! 
    //! \param key Node, containing this key, will be removed
    //! 
    virtual void Remove(){}
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
    //! Check whether the node is leaf
    //! 
    //! \return true if the node is leaf, false if the node is composite
    //! 
    virtual bool isLeaf(){
        return false;
    }
};

/**
* \brief Class for composite nodes
*
* Class which represents nodes with children
*/
template<typename T>
class NodeCompos : virtual public Node<T>{
protected:
    //! Left child of the node
    Node<T>* left;
    //! Right child of the node
    Node<T>* right;
public:
    NodeCompos() : left(nullptr), right(nullptr){}
    void setLeft(Node<T>* n) override {
        this->left = n;
    }
    void setRight(Node<T>* n) override {
        this->right = n;
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
    void Remove() override{};
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
            if(this->left)
                this->left->Add(key);
            else //when there is right but there's no left child
            {
                this->left = new BinaryNodeLeaf<T>(key);
                this->left->setParent(this);
            }
        }
        else{
            if(this->right)
                this->right->Add(key);
            else
            {
                this->right = new BinaryNodeLeaf<T>(key);
                this->right->setParent(this);
            }
        }
    }
    void setChild() {
        if (this->key_ < this->parent_->getKey())
            this->parent_->setLeft(this);
        else
            this->parent_->setRight(this);
    }
};

//! Class for leaf Binary Search Tree nodes
template<typename T>
class BinaryNodeLeaf : public BinaryNode<T>, public NodeLeaf<T>{
public:
    BinaryNodeLeaf(T key) : Node<T>(key), NodeLeaf<T>(){}
    void Add(T key) override{
        //Conversion of leaf to composite
        BinaryNodeCompos<T>* toCompos = new BinaryNodeCompos<T>(this);
        toCompos->setChild();
        //Addition of the key to a child node of the converted node
        toCompos->Add(key);
        delete this;
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
