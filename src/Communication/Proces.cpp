#include "Process.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

Process::Process(std::string name, int **FSM)
{
	_name = name;
	_FSM = FSM;
	//process should always start in its 0 state
	_currentState = 0;
}

Process::~Process()
{
}

void Process::addAlphabet(std::vector<std::string> alphabet)
{
	for (const auto alpha : alphabet)
	{
		_alphabet.push_back(alpha);
	}
}

void Process::printAlphabet()
{
	//for debugging purposes
	for (const auto alpha : _alphabet)
	{
		std::cout << alpha << ' ';
	}
	std::cout << '\n';
}

std::vector<std::string> Process::getAlphabet()
{
	return _alphabet;
}

std::string Process::getName()
{
	return _name;
}

std::vector<std::string> Process::getSensitivityList()
{
	_sensitivityList.clear();
	for (int i = 0; i < _alphabet.size(); ++i)
	{
		if (_FSM[_currentState][i] != -1)
		{
			_sensitivityList.push_back(_alphabet.at(i));
		}
	}

	return _sensitivityList;
}

void Process::makeTransition(std::string requestedAction)
{
	//kijk in statetable
	//verhoog current state
	//Find given element in vector
	auto it = std::find(_alphabet.begin(), _alphabet.end(), requestedAction);
	int FSMIndex;

	if (it != _alphabet.end())
	{
		//find position in alphabet
		FSMIndex = std::distance(_alphabet.begin(), it);
		_currentState = _FSM[_currentState][FSMIndex];
	}
}
