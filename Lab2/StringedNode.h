#ifndef STRINGEDNODE_H
#define STRINGEDNODE_H

template <typename T>
class Node;

enum TypeNode {
    IntNode, StringNode, DoubleNode
};

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
    /*Snapshot* getSnapshot(std::string descr) {
		return new Snapshot(this, type_, descr);
	}*/
};

#endif // STRINGEDNODE_H
