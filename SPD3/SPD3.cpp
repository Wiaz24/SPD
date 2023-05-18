#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <string> 
#include <chrono>
using namespace std;

#define BOOST

int numTasks = 0, numMachines = 0;

typedef struct Task
{
	int* timeOnMach;
	int* cmax_to;		//left number
	int* cmax_from;		//right number
	int ID;
} Task;

vector<Task> allTasks;

string readData(int dataIndex)
{
	ifstream f("data.txt");
	string buffer, dataName;

	if (allTasks.size() > 0)
	{
		for (int i = 0; i < allTasks.size(); i++)
		{
			delete[] allTasks[i].cmax_from;
			delete[] allTasks[i].cmax_to;
			delete[] allTasks[i].timeOnMach;
		}
		allTasks.clear();
	}

	if		(dataIndex < 10) dataName = "data.00" + to_string(dataIndex) + ":";
	else if (dataIndex < 100) dataName = "data.0" + to_string(dataIndex) + ":";
	else	dataName = "data." + to_string(dataIndex) + ":";

	while (buffer != dataName) f >> buffer;
	
	f >> numTasks >> numMachines;
	for (int i = 0; i < numTasks; i++)
	{
		Task tmpTask;
		tmpTask.timeOnMach = new int[numTasks];
		tmpTask.cmax_to = new int[numTasks];
		tmpTask.cmax_from = new int[numTasks];
		int tmpTime;
		for (int j = 0; j < numMachines; j++)
		{
			f >> tmpTime;
			tmpTask.timeOnMach[j] = tmpTime;
			tmpTask.cmax_from[j] = 0;
			tmpTask.cmax_to[j] = 0;
		}
		tmpTask.ID = i;
		allTasks.push_back(tmpTask);
	}
	return dataName;
}

int fillCmax(vector<Task>& _tasks, bool reverse, int bestIndex)
{
	int locNumTasks = _tasks.size();
	vector<int> timeOnPrevTask(numMachines,0);		//czasy na maszynach poprzedniego zadania
	int timeOFPrevMach; //czas na poprzedniej maszynie tego samego zadania
	int ans = 0;
	if (reverse == false)	//obliczanie cmax_from (liczba z prawej)
	{
		for (int i = 0; i < locNumTasks; i++) //po każdym zadaniu: j1, j3 ...
		{
			timeOFPrevMach = 0;		//jeśli to pierwsza maszyna to 0
			for (int j = 0; j < numMachines; j++) //po każdej maszynie
			{
				ans = max(timeOFPrevMach, timeOnPrevTask[j]) + _tasks[i].timeOnMach[j];
				timeOnPrevTask[j] = ans;
				_tasks[i].cmax_from[j] = ans;
				timeOFPrevMach = ans;
			}
		}
	}
	else	//obliczanie cmax_to (liczba z lewej)
	{
		for (int i = locNumTasks-1; i >= 0; i--) //po każdym zadaniu: j1, j3 ...
		{
			timeOFPrevMach = 0;		//jeśli to pierwsza maszyna to 0
			for (int j = numMachines-1; j >= 0; j--) //po każdej maszynie
			{
				ans = max(timeOFPrevMach, timeOnPrevTask[j]) + _tasks[i].timeOnMach[j];
				timeOnPrevTask[j] = ans;
				_tasks[i].cmax_to[j] = ans;
				timeOFPrevMach = ans;
			}
		}
	}
	return ans;
}

int partialCmax(vector<Task>& _tasks, int index)
{
	int locNumTasks = _tasks.size();
	vector<int> timeOnPrevTask(numMachines,0);		//czasy na maszynach poprzedniego zadania
	if (index != 0)
		for (int i = 0; i < numMachines; i++) 
			timeOnPrevTask[i] = _tasks[index-1].cmax_from[i];

	int timeOFPrevMach = 0; //czas na poprzedniej maszynie tego samego zadania
	int ans = 0;
	int maxans = 0;
	for (int i = 0; i < numMachines; i++)
	{
		ans = max(timeOFPrevMach, timeOnPrevTask[i]) + _tasks[index].timeOnMach[i];
		//cout << "new cmax of " << i << " machine = " << ans << endl;
		timeOnPrevTask[i] = ans;
		_tasks[index].cmax_from[i] = ans;
		timeOFPrevMach = ans;
		if (index != locNumTasks-1) ans += _tasks[index+1].cmax_to[i];
		if (ans > maxans) maxans = ans;
	}
	return maxans;
}

bool compareWeight(Task& task1, Task& task2)
{
	int sum1 = 0;
	int sum2 = 0;
	for (int i = 0; i < numMachines; i++)
	{
		sum1 += task1.timeOnMach[i];
		sum2 += task2.timeOnMach[i];
	}
	return sum1 > sum2;
}

void printTasks(vector<Task>& _tasks)
{
	for (Task task : _tasks)
	{
		cout << "ID=" << task.ID << endl;
		for (int mach = 0; mach < numMachines; mach ++)
		{
			cout << "L=" << task.cmax_to[mach] << ", ";
			cout << "T = " << task.timeOnMach[mach] << ", R = " << task.cmax_from[mach] << endl;
		}
		cout << endl;
	}
}

int main()
{
	ofstream plotData("plotdata.csv");
	for (int index = 0; index < 121; index++)
	{
		
		string dataName = readData(index);
		sort(allTasks.begin(), allTasks.end(), compareWeight);	//sortowanie rosnąco po wagach
		vector<Task> output;
		int minTime, bestIndex;
		auto start = chrono::high_resolution_clock::now();
		for (int i = 0; i < numTasks; i++)
		{
			minTime = INT_MAX;
			bestIndex = -1;
			for (int j = 0; j < i + 1; j++)
			{
				output.insert(output.begin() + j, allTasks[i]);	//dorzucamy element na zadaną pozycję
				#ifdef BOOST
					int thisCmax = partialCmax(output, j);					//obliczamy Cmax tej konfiguracji
				#else
					int thisCmax = fillCmax(output, false, 0);
				#endif
				if (thisCmax < minTime)
				{
					minTime = thisCmax;		//jeśli czas jest mniejszy to nadpisz
					bestIndex = j;
				}
				output.erase(output.begin() + j);			//cofnij dodanie
			}
			output.insert(output.begin() + bestIndex, allTasks[i]);	//dodaj na najlepszy indeks
			#ifdef BOOST
				fillCmax(output, false, bestIndex);
				fillCmax(output, true, bestIndex);					//obliczamy Cmax tej konfiguracji
			#endif
			
		}
		auto end = chrono::high_resolution_clock::now();
		auto execTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count()/1000000000.0;
		cout << dataName << "   Cmax = " << minTime << "   time[s] = " << execTime << endl << endl;
		plotData << execTime << "\n";
		// 
		// for (Task task : output) cout << task.ID + 1 << " ";
	}
	plotData.close();
	return 0;
}
