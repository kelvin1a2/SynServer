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
	std::vector<std::string> getSensitiviteActions();
	void makeTransition(std::string trans);

private:
	int **_FSM;
	int _currentState;
	std::vector<std::string> _vAlphabet;
	std::vector<std::string> _vSensitivityList;
	std::string _name;
};

#endif
