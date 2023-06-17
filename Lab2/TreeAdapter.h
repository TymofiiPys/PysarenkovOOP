#ifndef TREEADAPTER_H
#define TREEADAPTER_H

#include "Tree.h"

//template<typename T>
//class Adapter {
//protected:
//	Node<T>* root_;
//};

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
};

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
		TreeFactory* factory = TreeFactory::getTreeFac();
		Node<std::string>* sn = factory->createTree(BinSearchTree, std::to_string(root_->getKey()));
		addNodes(sn, root_->getLeft());
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
StringedNode* TNodeAdapter<std::string>::getStrNode(){
	return new StringedNode(root_, type_);
}

template <typename T>
class StringedNodeAdapter : public Node<T> {
private:
	StringedNode* root_;
public:
	StringedNodeAdapter(StringedNode* root) : root_(root) {}
	Node<T>* getNode() {
		TreeFactory* factory = TreeFactory::getTreeFac();
		Node<std::string>* sn = root_->getTree();

		Node<T>* n = factory->createTree(BinSearchTree, (T)std::stod(sn->getKey()));
		addNodes(n, sn->getLeft());
		addNodes(n, sn->getRight());
		return n;
	}
	void addNodes(Node<T>* dest, Node<std::string>* source) {
		dest->Add((T)std::stod(source->getKey()));
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
Node<std::string>* StringedNodeAdapter<std::string>::getNode() {
	return this->root_->getTree();
}

class BinaryTreeAdapter {

};

#endif // TREEADAPTER_H
