#ifndef SYNSERVER_H 
#define SYNSERVER_H
#include <vector>
#include <unordered_map>
#include "Thread.hpp"

class SynServer
{
public:
	SynServer();
	~SynServer();
	void addThread(Thread *p);
	void printThreads();
	void init();
	void getNextActions();
	void makeTransition(std::string trans);

private:
	std::vector<Thread *> _vChangedThreads;
	std::vector<Thread *> _vThreads;
	std::unordered_map<std::string, std::vector<std::string>> _mSensitivityLists;
	std::unordered_map<std::string, int> _mNextActions;
	std::unordered_map<std::string, int> _mAllActions;

	void updateSensitiveLists();
	void createStateTable();
	void removeUnsynchronisedActions(std::unordered_map<std::string, int> &_mNextActions);
	void printNextActions(std::unordered_map<std::string, int> _mNextActions);
};

#endif 
