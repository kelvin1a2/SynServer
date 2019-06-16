#ifndef PROCESS_H // include guard
#define PROCESS_H
#include <vector>
#include <map>
//class CommunicationServer;

class Process
{
public:
	Process(std::string name, int **FSM);
	~Process();
	void addAlphabet(std::vector<std::string> alpahbet);
	void printAlphabet();
	std::string getName();
	std::vector<std::string> getAlphabet();
	std::vector<std::string> getSensitivityList();
	void makeTransition(std::string trans);

private:
	int **_FSM;
	int _currentState;
	int _nstates;
	std::vector<std::string> _alphabet;
	std::vector<std::string> _sensitivityList;
	std::string _name;
};

#endif
