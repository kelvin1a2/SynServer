#include <iostream>
#include <thread>
#include "Thread.hpp"
#include "SynServer.hpp"

//using namespace std;

enum class states
{
	WRITE = 1,
	READ,
	BW,
	ERROR = -1
};

int **LSchouderP;
int **RSchouderP;
int **LSchouderR;
int **RSchouderR;
int **Lelleboog;
int **Relleboog;
int **ToReady;

void initProcs()
{
	// arr ProcA //////////////////
	LSchouderP = new int *[2];
	for (int i = 0; i < 2; i++)
	{
		LSchouderP[i] = new int[2];
	}
	LSchouderP[0][0] = 1;
	LSchouderP[0][1] = -1;
	LSchouderP[1][0] = -1;
	LSchouderP[1][1] = 0;

	RSchouderP = new int *[2];
	for (int i = 0; i < 2; i++)
	{
		RSchouderP[i] = new int[2];
	}
	RSchouderP[0][0] = 1;
	RSchouderP[0][1] = -1;
	RSchouderP[1][0] = -1;
	RSchouderP[1][1] = 0;

	LSchouderR = new int *[2];
	for (int i = 0; i < 2; i++)
	{
		LSchouderR[i] = new int[2];
	}
	LSchouderR[0][0] = 1;
	LSchouderR[0][1] = -1;
	LSchouderR[1][0] = -1;
	LSchouderR[1][1] = 0;

	RSchouderR = new int *[2];
	for (int i = 0; i < 2; i++)
	{
		RSchouderR[i] = new int[2];
	}
	RSchouderR[0][0] = 1;
	RSchouderR[0][1] = -1;
	RSchouderR[1][0] = -1;
	RSchouderR[1][1] = 0;

	Lelleboog = new int *[2];
	for (int i = 0; i < 2; i++)
	{
		Lelleboog[i] = new int[2];
	}
	Lelleboog[0][0] = 1;
	Lelleboog[0][1] = -1;
	Lelleboog[1][0] = -1;
	Lelleboog[1][1] = 0;

	Relleboog = new int *[2];
	for (int i = 0; i < 2; i++)
	{
		Relleboog[i] = new int[2];
	}
	Relleboog[0][0] = 1;
	Relleboog[0][1] = -1;
	Relleboog[1][0] = -1;
	Relleboog[1][1] = 0;

	ToReady = new int *[6];
	for (int i = 0; i < 6; i++)
	{
		ToReady[i] = new int[7];
	}

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			ToReady[i][j] = -1;
		}
	}
	
	ToReady[0][0] = 1;
	ToReady[1][1] = 2;
	ToReady[2][2] = 3;
	ToReady[2][3] = 4;
	ToReady[3][4] = 0;
	ToReady[4][5] = 5;
	ToReady[5][6] = 2;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			std::cout << " | "<< ToReady[i][j];
		}
		std::cout << '\n';
	}
	//ToReady[5][3] = 2;

}


int main()
{

	SynServer SynServer;

	initProcs();
	std::vector<Thread> procs;
	std::vector<std::string> alphabet;
	alphabet.push_back("schouderPomhoog");
	alphabet.push_back("schouderPomlaag");

	//maybe alphabet.size() can be buggy, need research
	Thread proc("LSchouderP", LSchouderP);
	// proc.addAlphabet(alphabet);

	procs.push_back(proc);

	Thread proc2("RSchouderP", RSchouderP);
	proc2.addAlphabet(alphabet);
	procs.push_back(proc2);

	alphabet.clear();
	alphabet.push_back("schouderRomhoog");
	alphabet.push_back("schouderRomlaag");
	Thread proc3("LSchouderR", LSchouderR);
	proc3.addAlphabet(alphabet);
	procs.push_back(proc3);

	Thread proc4("RSchouderR", RSchouderR);
	proc4.addAlphabet(alphabet);
	procs.push_back(proc4);

	alphabet.clear();
	alphabet.push_back("elleboogOmhoog");
	alphabet.push_back("elleboogOmlaag");
	Thread proc5("Lelleboog", Lelleboog);
	proc5.addAlphabet(alphabet);
	procs.push_back(proc5);

	Thread proc6("Relleboog", Relleboog);
	proc6.addAlphabet(alphabet);
	procs.push_back(proc6);
	alphabet.clear();
	alphabet.push_back("schouderPomhoog");
	alphabet.push_back("elleboogOmhoog");
	alphabet.push_back("elleboogOmlaag");
	alphabet.push_back("klappen");
	alphabet.push_back("schouderPomlaag");
	alphabet.push_back("schouderRomhoog");
	alphabet.push_back("schouderRomlaag");
	
	Thread proc7("ToReady", ToReady);
	
	proc7.addAlphabet(alphabet);
	procs.push_back(proc7);
	for (auto& proc : procs)
	{
		SynServer.addThread(&proc);
	}
	SynServer.init();
	SynServer.getNextActions();

	// start loop here
	while (true)
	{
		// enter number
		std::string input = "";
		std::cout << "Please enter an string value: ";
		getline(std::cin, input);
		//std::cin >> i;
		// make transistion
		//std::cout << input;
		SynServer.makeTransition(input);
		SynServer.getNextActions();
	}

	return 1;
}
