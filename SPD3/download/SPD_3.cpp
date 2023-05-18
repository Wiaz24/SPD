// program do zadania 3 z SPD - Algorytm NEH
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
using namespace std;
int cmax(int N, int M, int *P, int *X) //funkcja licząca długość wykonywania zadań
{
	int T[100];
	for (int m = 0; m <= M; m++)
	{
		T[m] = 0;
	}
	for (int n = 0; n < N; n++)
	{
		for (int m = 1; m <= M; m++)
		{
			T[m] = max(T[m],T[m-1]) + P[(m-1)+X[n]*M];
		}
	}
	return T[M];
}
void wagi(int N, int M, int* P, int* X)
{
	int* W = new int[N];
	for (int c = 0; c < N; c++)	 //przypisanie wagi każdemu zadaniu
	{
		for (int d = 0; d < M; d++)
		{
			W[c] += P[c * M + d];
		}
	}
	for (int b = 0; b < N - 1; b++)		//sortowanie zadań wagami
	{
		for (int a = 0; a < N - 1; a++)
		{
			if (W[a] < W[a + 1])
			{
				swap(W[a], W[a + 1]);
				swap(X[a], X[a + 1]);
			}
		}
	}
	delete[]W;
}
int NEH(int N, int M, int* P, int* X)
{
	wagi(N,M,P,X);
	for (int n = 0; n < N; n++)		//wstawianie na odpowiednią pozycję
	{
		int bestP = -1, bestCmax = 999999999;
		for (int p = n; p >= 0; p--)
		{
			int tmp = cmax(n+1, M, P, X);
			if (bestCmax >= tmp)
			{
				bestCmax = tmp;
				bestP = p;
			}
			if (p)
			{
				swap(X[p], X[p - 1]);
			}
		}
		for (int i = 0; i < bestP; i++)
		{
			swap(X[i], X[i + 1]);
		}
	}
	return cmax(N, M, P, X);
}
int main()
{
	int N, M, P[10000],X[1000];
	int x;
	string s="data.",s1,s2;
	ifstream f("data.txt");		// otworzenie strumienia wczytywania danych z pliku
	auto start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 121; i++)
	{
		if (i < 10)
		{
			s1 = s + "00" + to_string(i) + ":";
		}
		else if (i < 100)
		{
			s1 = s + "0" + to_string(i) + ":";
		}
		else
		{
			s1 = s + to_string(i) + ":";
		}
		while (s2 != s1)				//szukanie odpowiedniego zbioru danych
		{
			f >> s2;
		}
		f >> N >> M;						//wczytanie ilości zadań oraz ilości maszyn
		for (int j = 0; j < N * M; j++)		//wczytywanie parametrów kolejnych zadań
		{
			f >> P[j];
		}
		for (int j = 0; j < N; j++)
		{
			X[j] = j;
		}
		auto start1=chrono::high_resolution_clock::now();
		x = NEH(N, M, P, X);
		auto end1 = chrono::high_resolution_clock::now();
		auto duration1 = chrono::duration_cast<chrono::nanoseconds>(end1 - start1);
		cout << s1 <<" cmax: "<<x<< " czas: "<<duration1.count() * 1e-9 << "s" <<endl;
	}
	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
	cout << "Czas działania programu: "<<duration.count() * 1e-9 << "s" << endl;
	cin.get();				// czekanie z skończeniem działania programu
	f.close();			//zamknięcie strumienia wczytywania danych z pliku
}