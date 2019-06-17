#include "SynServer.hpp"
#include "Thread.hpp"
#include <iostream>
#include <algorithm>
#include <iterator> // std::iterator, std::input_iterator_tag

SynServer::SynServer()
{
}

SynServer::~SynServer()
{
}

void SynServer::init()
{
	// Map action + required occurences (for synchronisatie), and a vector with all actions
	// 
	createAlphabetTableHeader();
	updateSensitiveLists();
}


void SynServer::createAlphabetTableHeader()
{
	_allActions.clear();
	// _all actions vector not needed?? 

	for (auto &proc : _vProcesses)
	{
		for (auto &alpha : proc->getAlphabet())
		{
			// When action already defined, add 
			if (std::find(_allActions.begin(), _allActions.end(), alpha) != _allActions.end())
			{
				_allActionsMap[alpha]++;
			}
			// When action not defined, add entry in map and allAction list  
			else
			{
				_allActionsMap[alpha] = 1;
				_allActions.push_back(alpha);
			}
		}
	}
}


void SynServer::addProcess(Thread *p)
{
	_vProcesses.push_back(p);
}

void SynServer::printProcesses()
{
	//for debugging purposes
	for (auto proc : _vProcesses)
	{
		std::cout << proc->getName();
	}
	printf("\n");
}

void SynServer::updateSensitiveLists()
{
	// First time check all processes 
	if(_mSensitivityLists.empty())
	{
		for (auto proc : _vProcesses)
		{
			_mSensitivityLists[proc->getName()] = proc->getSensitivityList();
		}
	}
	else // only check process which has made changes in FSM (optimized)
	{
		for (auto proc : _changedProcess)
		{
			_mSensitivityLists[proc->getName()] = proc->getSensitivityList();
		}
	}
	_changedProcess.clear(); 
}

void SynServer::getNextPossibleActions()
{
	nextPossibleActions.clear();

	// Count actions
	for (auto pairOfMap : _mSensitivityLists)
	{
		// pairOfMap.second = Action 
		for (auto sensitveAction : pairOfMap.second)
		{
			//if action not found earlier, insert action into next possible actions list. 
			if (nextPossibleActions.find(sensitveAction) == nextPossibleActions.end())
			{
				nextPossibleActions.insert(std::pair<std::string, int>(sensitveAction, 1));
			}
			else //else count up number of occurrences
			{
				nextPossibleActions[sensitveAction]++;
			}
		}
	}
	
	// niet de dubbele entries, maar de gene die niet mogen... 
	// DO your synchronizing... 
	RemoveDoubleEntries(nextPossibleActions);  
	PrintNextActions(nextPossibleActions);
}


void SynServer::RemoveDoubleEntries(std::unordered_map<std::string, int> &nextPossibleActions)
{
	// Iterate over each possible Action 
	for (auto it = nextPossibleActions.begin(); it != nextPossibleActions.end();)
	{
		// If number of sensitive entries is not equal to second number of expected sensitve entries, remove it, because it is not allowed. 
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

void SynServer::PrintNextActions(std::unordered_map<std::string, int> nextPossibleActions)
{
	std::cout << "Next possible transitions:[ ";
	for (auto &x : nextPossibleActions)
	{
		std::cout << x.first << ' ';
	}
	std::cout << " ]\n";
}


void SynServer::makeTransition(std::string requestedAction)
{

	// add list of process that changed, and only asked new sensitivty list for that processes.. 
	for (auto proc : _vProcesses)
	{
		for (auto const &sensitiveAction : _mSensitivityLists[proc->getName()])
		{
			if (sensitiveAction == requestedAction)
			{
				_changedProcess.push_back(proc); 
				proc->makeTransition(sensitiveAction);
			}
		}
	}
	updateSensitiveLists();
}
