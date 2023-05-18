#include <iostream>
#include <fstream>
using namespace std;
struct zadanie
{
	int czas;				//czas trwania
	int waga;				//ważność zadania
	int deadline;			//do kiedy ma się wykonać
};
int main()
{
	zadanie tablica[100];
	ifstream wczytaj("data.txt");
	int n, c;
	string s;
	string s1[11] = {"data.10:", "data.11:","data.12:","data.13:","data.14:","data.15:","data.16:","data.17:","data.18:","data.19:","data.20:"};
	for (int iter = 0; iter < 11; iter++)		//pętla przejeżdzająca po wszystkich zbiorach danych
	{
		while (s != s1[iter])			//szukanie odpowiedniego zbioru danych
		{
			wczytaj >> s;
		}
		cout <<"Dane "<<s<<endl;
		wczytaj >> n;					//wczytanie ilości zadań
		for (int i = 0; i < n; i++)		//wczytanie parametrów kolejnych zadań
		{
			wczytaj >> tablica[i].czas >> tablica[i].waga >> tablica[i].deadline;
		}
		int N = 1 << n;
		int* F = new int[N];
		F[0] = 0;
		for (int set = 1; set < N; set++)
		{
			c = 0;
			for (int i = 0, b = 1; i < n; i++, b *= 2)
			{
				if (set & b)
				{
					c += tablica[i].czas;
				}
			}
			F[set] = 999999;
			for (int k = 0, b = 1; k < n; k++, b *= 2)
			{
				if (set & b)
				{
					F[set] = min(F[set], F[set - b] + tablica[k].waga * max(c - tablica[k].deadline, 0));
				}
			}
		}
		cout << "Optymalne rozwiazanie: " << F[N - 1] << endl;
		delete[] F;
	}
	wczytaj.close();
	cout <<endl<< "Koniec dzialania programu." << endl;
	cin.get();
}