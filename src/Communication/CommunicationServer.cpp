#include "CommunicationServer.hpp"
#include "Process.hpp"
#include <iostream>
#include <algorithm>
#include <iterator> // std::iterator, std::input_iterator_tag

CommunicationServer::CommunicationServer()
{
}

CommunicationServer::~CommunicationServer()
{
}

void CommunicationServer::init()
{
	createAlphabetTableHeader();
	fillActionMap();
}


void CommunicationServer::createAlphabetTableHeader()
{
	_allActions.clear();

	for (auto &proc : _vProcesses)
	{
		for (auto &alpha : proc->getAlphabet())
		{
			if (std::find(_allActions.begin(), _allActions.end(), alpha) != _allActions.end())
			{
				continue;
			}
			else
			{
				_allActionsMap[alpha] = 0;
				_allActions.push_back(alpha);
			}
		}
	}
}

void CommunicationServer::fillActionMap()
{
	for (auto &proc : _vProcesses)
	{
		std::vector<std::string> alphabet = proc->getAlphabet();
		for (auto &action : _allActions)
		{
			if (std::find(alphabet.begin(), alphabet.end(), action) != alphabet.end())
			{
				_allActionsMap[action]++;
			}
		}
	}
}

void CommunicationServer::addProcess(Process *p)
{
	_vProcesses.push_back(p);
}

void CommunicationServer::printProcesses()
{
	//for debugging purposes
	for (auto proc : _vProcesses)
	{
		std::cout << proc->getName();
	}
	printf("\n");
}

void CommunicationServer::getSensitiveLists()
{
	//get all the the sensitivity lists and map them.
	for (auto proc : _vProcesses)
	{
		_mSensitivityLists[proc->getName()] = proc->getSensitivityList();
	}
}

void CommunicationServer::getNextPossibleActions()
{
	nextPossibleActions.clear();

	//loop trough all processes
	for (auto proc : _vProcesses)
	{
		//
		for (auto s : _mSensitivityLists[proc->getName()])
		{
			//if not found insert into next possible actions list
			if (nextPossibleActions.find(s) == nextPossibleActions.end())
			{
				nextPossibleActions.insert(std::pair<std::string, int>(s, 1));
			}
			else //else count up number of occurrences
			{
				nextPossibleActions[s]++;
			}
		}
	}

	RemoveDoubleEntries(nextPossibleActions); 
	PrintNextActions(nextPossibleActions);

}


void CommunicationServer::RemoveDoubleEntries(std::unordered_map<std::string, int> &nextPossibleActions)
{
	for (auto it = nextPossibleActions.begin(); it != nextPossibleActions.end();)
	{
		if (_allActionsMap.find(it->first)->second != it->second)
		{

			it = nextPossibleActions.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void CommunicationServer::PrintNextActions(std::unordered_map<std::string, int> nextPossibleActions)
{
	std::cout << "Next possible transitions:[ ";
	for (auto &x : nextPossibleActions)
	{
		std::cout << x.first << ' ';
	}
	std::cout << " ]\n";
}


void CommunicationServer::makeTransition(std::string requestedAction)
{
	for (auto proc : _vProcesses)
	{
		for (auto const &sensitiveAction : _mSensitivityLists[proc->getName()])
		{
			if (sensitiveAction == requestedAction)
			{
				proc->makeTransition(sensitiveAction);
			}
		}
	}
}
