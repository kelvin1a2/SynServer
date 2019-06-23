#include "Thread.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

Thread::Thread(std::string name, int **FSM)
{
	_name = name;
	_FSM = FSM;
	//Thread should always start in its 0 state
	_currentState = 0;
}

Thread::~Thread()
{
}

void Thread::addAlphabet(std::vector<std::string> alphabet)
{
	for (const auto alpha : alphabet)
	{
		_vAlphabet.push_back(alpha);
	}
}

void Thread::printAlphabet()
{
	//for debugging purposes
	for (const auto alpha : _vAlphabet)
	{
		std::cout << alpha << ' ';
	}
	std::cout << '\n';
}

std::vector<std::string> Thread::getAlphabet()
{
	return _vAlphabet;
}

std::string Thread::getName()
{
	return _name;
}

std::vector<std::string> Thread::getSensitiviteActions()
{
	_vSensitivityList.clear();
	for (int i = 0; i < _vAlphabet.size(); ++i)
	{
		if (_FSM[_currentState][i] != -1)
		{
			_vSensitivityList.push_back(_vAlphabet.at(i));
		}
	}

	return _vSensitivityList;
}

void Thread::makeTransition(std::string requestedAction)
{
	//kijk in statetable
	//verhoog current state
	//Find given element in vector
	auto it = std::find(_vAlphabet.begin(), _vAlphabet.end(), requestedAction);
	int FSMIndex;

	if (it != _vAlphabet.end())
	{
		// find position in alphabet
		FSMIndex = std::distance(_vAlphabet.begin(), it);
		_currentState = _FSM[_currentState][FSMIndex];
	}
}
