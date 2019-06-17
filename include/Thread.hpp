#ifndef THREAD_H // include guard
#define THREAD_H
#include <vector>
#include <map>

class Thread
{
public:
	Thread(std::string name, int **FSM);
	~Thread();
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
