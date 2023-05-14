#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <string> 

using namespace std;

typedef struct Task
{
	vector<int> timeOnMach;
	vector<int> cmax_to;
	vector<int> cmax_from;
	int ID;
} Task;

vector<Task> allTasks;

//23.47
void fillCmax(vector<Task>& _tasks, bool reverse)
{
	int numMachines = _tasks[0].timeOnMach.size();	//ilość maszyn
	vector<int> timeOnPrevTask(numMachines,0);		//czasy na maszynach poprzedniego zadania
	int timeOFPrevMach; //czas na poprzedniej maszynie tego samego zadania
	int ans = 0;
	if (reverse == false)
	{
		for (int i = 0; i < _tasks.size(); i++) //po każdym zadaniu: j1, j3 ...
		{
			timeOFPrevMach = 0;		//jeśli to pierwsza maszyna to 0
			for (int j = 0; j < numMachines; j++) //po każdej maszynie
			{
				ans = max(timeOFPrevMach, timeOnPrevTask[j]) + _tasks[i].timeOnMach[j];
				timeOnPrevTask[j] = ans;
				_tasks[i].cmax_from.push_back(ans);
				timeOFPrevMach = ans;
			}
		}
	}
	else
	{
		for (int i = _tasks.size()-1; i >= 0; i--) //po każdym zadaniu: j1, j3 ...
		{
			timeOFPrevMach = 0;		//jeśli to pierwsza maszyna to 0
			for (int j = numMachines-1; j >= 0; j--) //po każdej maszynie
			{
				ans = max(timeOFPrevMach, timeOnPrevTask[j]) + _tasks[i].timeOnMach[j];
				timeOnPrevTask[j] = ans;
				_tasks[i].cmax_to.push_back(ans);
				timeOFPrevMach = ans;
			}
		}
	}
	
}

bool compareWeight(Task& task1, Task& task2)
{
	int sum1 = 0;
	int sum2 = 0;
	for (int val : task1.timeOnMach) sum1 += val;
	for (int val : task2.timeOnMach) sum2 += val;
	return sum1 > sum2;
}

void printTasks()
{
	for (Task task : allTasks)
	{
		cout << "ID=" << task.ID << " ";
		for (int mach = 0; mach < task.timeOnMach.size(); mach ++)
		{
			//cout << "L=" << task.cmax_to[mach] << ", 
			cout << "T = " << task.timeOnMach[mach] << ", R = " << task.cmax_from[mach] << endl;
		}
		cout << endl;
	}
}

int main()
{
	ifstream f("data.txt");
	string buffer;
	
	for (int dataIndex = 0; dataIndex < 1; dataIndex++)
	{
		allTasks.clear();
		string dataName;
		if		(dataIndex < 10) dataName = "data.00" + to_string(dataIndex) + ":";
		else if (dataIndex < 100) dataName = "data.0" + to_string(dataIndex) + ":";
		else	dataName = "data." + to_string(dataIndex) + ":";

		while (buffer != dataName) f >> buffer;
		int numTasks, numMachines;
		f >> numTasks >> numMachines;
		for (int i = 0; i < numTasks; i++)
		{
			Task tmpTask;
			int tmpTime;
			for (int j = 0; j < numMachines; j++)
			{
				f >> tmpTime;
				tmpTask.timeOnMach.push_back(tmpTime);
			}
			tmpTask.ID = i;
			allTasks.push_back(tmpTask);
		}

		sort(allTasks.begin(), allTasks.end(), compareWeight);	//sortowanie rosnąco po wagach
		vector<Task> output(allTasks.size());
		//vector<int> output(allTasks.size(), 0);
		fillCmax(allTasks, false);
		//fillCmax(allTasks, true);
		printTasks();
		//for (int i = 0; i < allTasks.size(); i++)
		//{
		//	int minTime = INT_MAX;
		//	int bestIndex = -1;
		//	for (int j = 0; j < i + 1; j++)
		//	{
		//		output.insert(output.begin() + j, allTasks[i]);	//dorzucamy element na zadaną pozycję
		//		int thisCmax = Cmax(allTasks);					//obliczamy Cmax tej konfiguracji
		//		if (thisCmax < minTime)
		//		{
		//			minTime = thisCmax;		//jeśli czas jest mniejszy to nadpisz
		//			bestIndex = j;
		//		}
		//		output.erase(output.begin() + j);			//cofnij dodanie
		//	}
		//	output.insert(output.begin() + bestIndex, allTasks[i]);	//dodaj na najlepszy indeks
		//	cout << i << " " << flush;
		//}
		/*cout << dataName << endl;
		cout << "Kolejnosc = ";
		for (Task task : output) cout << task.ID + 1 << " ";
		cout << endl;
		cout << "Cmax = " << Cmax(output);
		cout << endl << endl;*/
	}

	return 0;
}
