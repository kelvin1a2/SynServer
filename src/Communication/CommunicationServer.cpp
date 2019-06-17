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
	updateSensitiveLists();
}


void CommunicationServer::createAlphabetTableHeader()
{
	_allActions.clear();

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

void CommunicationServer::updateSensitiveLists()
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

void CommunicationServer::getNextPossibleActions()
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


void CommunicationServer::RemoveDoubleEntries(std::unordered_map<std::string, int> &nextPossibleActions)
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
