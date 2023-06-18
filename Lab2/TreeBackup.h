//#ifndef TREEBACKUP_H
//#define TREEBACKUP_H
//
//#include "Tree.h"
//#include "TreeAdapter.h"
//#include "TreeFacade.h"
//
//class Snapshot {
//private:
//	StringedNode* root_;
//	std::string descr_;
//	std::string date_;
//public:
//	template<typename T>
//	Snapshot(Node<T>* root, TypeNode type, std::string descr) : descr_(descr) {
//		TNodeAdapter<T>* adapter = new TNodeAdapter<T>(root, type);
//		this->root_ = adapter->getStrNode();
//		delete adapter;
//		std::time_t now = std::time(0);
//		this->date_ = std::ctime(&now)/*.substr(0, 19)*/;
//	};
//	template<typename T>
//	Node<T>* getTree() {
//		StringedNodeAdapter<T>* adapter = new StringedNodeAdapter<T>(this->root_);
//		Node<T>* n = adapter->getNode();
//		delete adapter;
//		return n;
//	}
//	StringedNode* getStrNode() {
//		return this->root_;
//	}
//	std::string getName() {
//		return "(" + this->date_ + ") " + this->descr_;
//	}
//	std::string getDate() {
//		return this->date_;
//	}
//};
//
//template<typename T>
//class Command {
//protected:
//	std::vector<Snapshot*> snaps;
//	TreeFacade<T>* fac;
//	//StringedNode* root;
//public:
//	Command(){}
//	Command(TreeFacade<T>* facc) : fac(facc) {};
//	virtual void execute() {};
//	void makeBackup() {
//		if(this->snaps.size() <= 50)
//			this->snaps.push_back(fac->getSnapshot());
//	}
//};
//
//template<typename T>
//class SaveCommand : public Command<T> {
//public:
//	SaveCommand() : Command<T>() {};
//	SaveCommand(TreeFacade<T>* facc) : Command<T>(facc) {};
//	void execute() override{
//		std::ofstream output("tree.txt");
//		StringedNode* sn = (*this->snaps.end())->getStrNode();
//		output << sn->getTypeStr() << "\n";
//		Node<std::string>* n = sn->getTree();
//		Traverser<std::string>* trav = new Traverser<std::string>(PreOrder);
//		std::string out = trav->Iterate(n);
//		output << out;
//		output.close();
//	}
//};
//
//template<typename T>
//class UndoCommand : public Command<T> {
//public:
//	UndoCommand() : Command<T>() {};
//	UndoCommand(TreeFacade<T>* facc) : Command<T>(TreeFacade<T>* facc) {};
//	void execute() override {
//		if (!this->snaps.size())
//			return;
//		Snapshot* snap = this->snaps.back();
//		this->snaps.pop_back();
//		this->fac->restore(snap);
//	}
//};
//
//#endif // TREEBACKUP_H
