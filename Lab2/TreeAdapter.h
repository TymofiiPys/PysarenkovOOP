#ifndef TREEADAPTER_H
#define TREEADAPTER_H

#include "Tree.h"
#include "StringedNode.h"

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

		Node<T>* n = factory->createTree(BinSearchTree, T(Converter<std::string>(sn->getKey())));
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
Node<std::string>* StringedNodeAdapter<std::string>::getNode() {
	return this->root_->getTree();
}

//class BinaryTreeAdapter {

//};

#endif // TREEADAPTER_H
