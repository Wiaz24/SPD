
#include <iostream>
#include<fstream>
#include <string>
using namespace std;
struct zadanie
{
	public:
		int ID;
		int r;
		int p;
		int q;
};
void zamiana(zadanie& a, zadanie& b)
{
	swap(a.ID, b.ID);
	swap(a.p, b.p);
	swap(a.q, b.q);
	swap(a.r, b.r);
}
int schrage(int n, zadanie* T, int* X)
{
	int ND[100], D[100];							//ND - tablica zadań niedostępnych  D - tablica zadań dostępnych
	int nd = n, d = 0, w = 0;		// nd - liczba zadań niedostępnych  d-tablica zadań dostępnych  w-liczba zadań wykonanych 
	int  t = 0, cmax = 0;			//t - obecna jednostka czasu  cmax-długość najdłuższego uszeregowania
	for (int i = 0; i < n; i++)
	{
		ND[i] = i;
	}
	for (int i = 0; i < n - 1; i++)			//sortowanie tablicy zadań niedostęnych malejąco po r
	{
		for (int j = 0; j < n - 1; j++)
		{
			if (T[ND[j]].r < T[ND[j + 1]].r)
			{
				swap(ND[j], ND[j + 1]);
			}
		}
	}
	while (w != n)					//jeśli są zadania niewykonane
	{
		if (nd != 0)				//jeśli są jeszcze zadania niedostępne
		{	
			if (T[ND[nd - 1]].r <= t)		//jeśli jest jakieś zadanie które przyszło
			{
				D[d] = ND[nd - 1];
				d++;
				nd--;
				for (int k = d - 1; k > 0; k--)			//sortowanie tablicy zadań dostępnych rosnąco po q
				{
					if (T[D[k]].q < T[D[k - 1]].q)
					{
						swap(D[k], D[k - 1]);
					}
				}
				continue;
			}
		}
		if (d != 0)			//jeśli są zadania dostępne
		{
			X[w] = D[d - 1];
			t += T[X[w]].p;
			cmax = max(cmax, t + T[X[w]].q);
			d--;
			w++;
			continue;
		}
		if (d == 0 && T[ND[nd - 1]].r > t)			//jeśli jest jakiaś przerwa w pracy to ją przeskocz
		{
			t = T[ND[nd - 1]].r;
		}
	}
	return cmax;
}
int schrage_podziel(int n, zadanie* T)
{
	int ND[100], D[100],pom[100];
	int nd = n, d = 0, w = 0;
	int  t = 0, cmax = 0;
	int poz = 100;
	int ile_zr = 0;
	for (int i = 0; i < n; i++)
	{
		pom[i] = T[i].p;
	}
	for (int i = 0; i < n; i++)
	{
		ND[i] = i;
	}
	for (int i = 0; i < n - 1; i++)			
	{
		for (int j = 0; j < n - 1; j++)
		{
			if (T[ND[j]].r < T[ND[j + 1]].r)
			{
				swap(ND[j], ND[j + 1]);
			}
		}
	}
	while (nd != 0 || d != 0)
	{
		if (nd != 0)
		{
			if (T[ND[nd-1]].r <= t)
			{
				D[d] = ND[nd - 1];
				d++;
				nd--;
				for (int k = d - 1; k > 0; k--)			
				{
					if (T[D[k]].q < T[D[k - 1]].q)
					{
						swap(D[k], D[k - 1]);
					}
				}
				if (poz != 100)
				{
					if (T[D[d - 1]].q > T[poz].q)
					{
						D[d] = poz;
						swap(D[d], D[d - 1]);
						d++;						
						poz = 100;
					}
				}
				continue;
			}
		}
		if (d != 0)
		{
			if (poz == 100)
			{
				poz = D[d - 1];
				d--;
			}
			if (nd != 0)
			{
				ile_zr = min(pom[poz], T[ND[nd - 1]].r - t);
			}
			else
			{
				ile_zr = pom[poz];
			}
			t += ile_zr;
			pom[poz] -= ile_zr;
			if (pom[poz]== 0)
			{
				cmax = max(cmax, t + T[poz].q);
				poz = 100;
			}
			continue;
		}
		if (d == 0 && nd != 0)
		{
			if (T[ND[nd - 1]].r > t)
			{
				t = T[ND[nd - 1]].r;
			}
		}
	}
	return cmax;
}
int main()
{
	int X[100];
	zadanie T[100];
	string s = "data.00", s1, s2;
	ifstream f("data.txt");
	int n;
	for (int i = 0; i < 9; i++)			//pętla przechodząca przez każdy zbiór danych
	{
		s1 = s + to_string(i) + ":";
		while (s2 != s1)				//szukanie odpowiedniego zbioru danych
		{
			f >> s2;
		}
		f >> n;							//wczytywanie ilości zadań w danym zbiorze danych
		for (int j = 0; j < n; j++)		//wczytywanie parametrów kolejnych zadań
		{
			T[j].ID = j;
			f >> T[j].r >> T[j].p >> T[j].q;
		}
		cout << "Zbior danych: " << s1 << endl;
		cout << "Podzial: " << schrage_podziel(n, T) << endl;
		cout << "Schrage: " << schrage(n, T, X) << endl;
		cout << "Roznica: " << schrage(n, T, X) - schrage_podziel(n, T) << endl;
		for (int i = 0; i < n; i++)
		{
			cout << X[i]+1 << " ";
		}
		cout <<endl<< "===========================================" << endl;
	}
}