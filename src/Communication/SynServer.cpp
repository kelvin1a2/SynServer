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
	createStateTable();
	updateSensitiveLists();
}


void SynServer::createStateTable()
{
	for (auto &proc : _vThreads)
	{
		for (auto &alpha : proc->getAlphabet())
		{
			// dd
			if (_allActionsMap.find(alpha) == _allActionsMap.end()) 
			{
				_allActionsMap[alpha] = 1;
			}
			else
			{
				_allActionsMap[alpha]++;
				
			}
			
		}
	}
}


void SynServer::addThread(Thread *p)
{
	_vThreads.push_back(p);
}

void SynServer::printThreads()
{
	//for debugging purposes
	for (auto proc : _vThreads)
	{
		std::cout << proc->getName();
	}
	printf("\n");
}

void SynServer::updateSensitiveLists()
{
	// First time check all threads 
	if(_mSensitivityLists.empty())
	{
		for (auto proc : _vThreads)
		{
			_mSensitivityLists[proc->getName()] = proc->getSensitivityList();
		}
	}
	else // only check threads which has made changes in FSM (optimized)
	{
		for (auto proc : _changedThreads)
		{
			_mSensitivityLists[proc->getName()] = proc->getSensitivityList();
		}
	}
	_changedThreads.clear(); 
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

	// add list of Thread that changed, and only asked new sensitivty list for that Threades.. 
	for (auto proc : _vThreads)
	{
		for (auto const &sensitiveAction : _mSensitivityLists[proc->getName()])
		{
			if (sensitiveAction == requestedAction)
			{
				_changedThreads.push_back(proc); 
				proc->makeTransition(sensitiveAction);
			}
		}
	}
	updateSensitiveLists();
}
