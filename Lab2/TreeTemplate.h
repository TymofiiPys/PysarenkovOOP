#ifndef TREETEMPLATE_H
#define TREETEMPLATE_H

#include "Tree.h"
#include <iostream>

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

class StringTreeAnalyzer : public TreeAnalyzer<std::string> {
public:
	StringTreeAnalyzer(Node<std::string>* root) : TreeAnalyzer(root) {}
	std::string findMin() override {
		return TreeAnalyzer::findMin() + this->root_->getMin()->getKey();
	}
	std::string findMax() override {
		return TreeAnalyzer::findMax() + this->root_->getMax()->getKey();
	}
	std::string findLongestWord() override {
		std::string theword = this->root_->getLongestWord();
		return "Longest word: " + theword + ", of length: " + std::to_string(theword.length());
	}
};

#endif // TREETEMPLATE_H
