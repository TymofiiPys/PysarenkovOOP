#ifndef TREEFACADE_H
#define TREEFACADE_H
#include <string>
#include <vector>
#include <fstream>

template<typename T>
class Node;

class StringedNode;

enum TreeType;

enum TypeNode;

enum TravType;

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
		std::time_t now = std::time(0);
		switch (tt) {
		case BinSearchTree:
			tt_ = "BST";
		default:
			break;
		}
		this->date_ = std::ctime(&now)/*.substr(0, 19)*/;
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
	TreeFacade(TypeNode ntype, TreeType ttype) : TreeFacade<T>() {
		this->ntype_ = ntype;
		this->ttype_ = ttype;
	}
	void AddKey(std::string key) {
		if (!root_) {
			TreeFactory* factory = TreeFactory::getTreeFac();
			root_ = factory->createTree(ttype_, T(Converter<std::string>(key)));
		}
		else {
			root_->Add(T(Converter<std::string>(key)));
			savecom->makeBackup();
			SyncSnaps(savecom->getSnaps());
		}
	}
	void RemoveKey(std::string key) {
		if (!root_)
			return;
		root_->Remove(T(Converter<std::string>(key)));
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
	/*void setRoot(Node<T>* root) {
		this->root_ = root;
	}*/
	void Save() {
		savecom->execute();
	}
	void Undo() {
		undocom->execute();
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
		std::ofstream output("tree.txt");
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

#endif // TREEFACADE_H
