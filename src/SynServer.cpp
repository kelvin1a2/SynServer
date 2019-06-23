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
			if (_mAllActions.find(alpha) == _mAllActions.end()) 
			{
				_mAllActions[alpha] = 1;
			}
			else
			{
				_mAllActions[alpha]++;
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
			_mSensitivityLists[proc->getName()] = proc->getSensitiviteActions();
		}
	}
	else // only check threads which has made changes in FSM (optimized)
	{
		for (auto proc : _vChangedThreads)
		{
			_mSensitivityLists[proc->getName()] = proc->getSensitiviteActions();
		}
		_vChangedThreads.clear(); 
	}
	
}

void SynServer::getNextActions()
{
	_mNextActions.clear();

	// Count actions
	for (auto pairOfMap : _mSensitivityLists)
	{
		// pairOfMap.second = Action 
		for (auto sensitveAction : pairOfMap.second)
		{
			//if action not found earlier, insert action into next possible actions list. 
			if (_mNextActions.find(sensitveAction) == _mNextActions.end())
			{
				_mNextActions.insert(std::pair<std::string, int>(sensitveAction, 1));
			}
			else //else count up number of occurrences
			{
				_mNextActions[sensitveAction]++;
			}
		}
	}
	
	// DO your synchronizing... 
	removeUnsynchronisedActions(_mNextActions);  
	printNextActions(_mNextActions);
}


void SynServer::removeUnsynchronisedActions(std::unordered_map<std::string, int> &_mNextActions)
{
	// Iterate over each possible Action 
	for (auto it = _mNextActions.begin(); it != _mNextActions.end();)
	{
		// If number of sensitive entries is not equal to second number of expected sensitve entries, remove it, because it is not allowed. 
		if (_mAllActions.find(it->first)->second != it->second)
		{
			it = _mNextActions.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void SynServer::printNextActions(std::unordered_map<std::string, int> _mNextActions)
{
	std::cout << "Next possible transitions:[ ";
	for (auto &x : _mNextActions)
	{
		std::cout << x.first << ' ';
	}
	std::cout << " ]\n";
}


void SynServer::makeTransition(std::string requestedAction)
{
	// add list of Thread that changed, and only asked new sensitivty list for that Threads.. 
	for (auto proc : _vThreads)
	{
		for (auto const &sensitiveAction : _mSensitivityLists[proc->getName()])
		{
			if (sensitiveAction == requestedAction)
			{
				_vChangedThreads.push_back(proc); 
				proc->makeTransition(sensitiveAction);
			}
		}
	}
	updateSensitiveLists();
}
