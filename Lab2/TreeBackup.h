#ifndef TREEBACKUP_H
#define TREEBACKUP_H

#include "Tree.h"

class Command {
//private:
//	Snapshot<
//protected:

};

class SaveCommand : Command {
public:
	void execute() {

	}
};

class BackupCommand : Command {

};

template<typename T>
class Snapshot {
private:
	Node<std::string>* root_;
	std::string type_;
	std::string descr_;
	std::string date_;
public:
	Snapshot(Node<T>* root, std::string descr) : root_(root), descr_(descr) {
		std::time_t now = std::time(0);
		this->date_ = std::ctime(&now).substr(0, 19);
	};
	Node<T>* getTree() {
		return root_;
	}
	std::string getName() {
		return "(" + this->date_ + ") " + this->descr_;
	}
	std::string getDate() {
		return this->date_;
	}
};

#endif // TREEBACKUP_H
