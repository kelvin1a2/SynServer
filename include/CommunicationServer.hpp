// my_class.h
#ifndef COMMUNICATIONSERVER_H // include guard
#define COMMUNICATIONSERVER_H
#include <vector>
#include <unordered_map>
#include "Process.hpp"

class CommunicationServer
{
public:
	CommunicationServer();
	~CommunicationServer();
	void addProcess(Process *p);
	void printProcesses();
	void nextState(int);
	void init();
	void updateSensitiveLists();
	void getNextPossibleActions();
	void makeTransition(std::string trans);

private:
	std::vector<std::string> _allActions;
	std::vector<Process *> _changedProcess;
	std::vector<Process *> _vProcesses;
	//std::vector<std::vector<int>> _stateTable;
	char **_lookUpTable;
	std::unordered_map<std::string, std::vector<std::string>> _mSensitivityLists;
	std::unordered_map<std::string, int> nextPossibleActions;
	std::unordered_map<std::string, int> _allActionsMap;

	void createAlphabetTableHeader();
	void fillActionMap();
	void RemoveDoubleEntries(std::unordered_map<std::string, int> &nextPossibleActions);
	void PrintNextActions(std::unordered_map<std::string, int> nextPossibleActions);
};

#endif /* MY_CLASS_H */
