#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int schrage(int n, int* R, int* P, int* Q, int* X)
{
	int ND[100], D[100];
	int nd = n, d = 0, w = 0, t = 0, cmax = 0;
	for (int i = 0; i < n; i++)
	{
		ND[i] = i;
	}
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - 1; j++)
		{
			if (R[ND[j]] < R[ND[j + 1]])
			{
				swap(ND[j], ND[j + 1]);
			}
		}
	}
	while (w != n)
	{
		if (nd != 0)
		{
			if (R[ND[nd - 1]] <= t)
			{
				D[d] = ND[nd - 1];
				d++;
				nd--;
				for (int k = d - 1; k > 0; k--)
				{
					if (Q[D[k]] < Q[D[k - 1]])
					{
						swap(D[k], D[k - 1]);
					}
				}
				continue;
			}
		}
		if (d != 0)
		{
			X[w] = D[d - 1];
			t += P[X[w]];
			cmax = max(cmax, t + Q[X[w]]);
			d--;
			w++;
			continue;
		}
		if (d == 0 && R[ND[nd - 1]] > t)
		{
			t = R[ND[nd - 1]];
		}
	}
	return cmax;
}
int schrage_podziel(int n, int* R, int* P, int* Q)
{
	int ND[100], D[100], pom[100];
	int nd = n, d = 0, w = 0, t = 0, cmax = 0, poz = 100, ile_zr = 0;
	for (int i = 0; i < n; i++)
	{
		ND[i] = i;
		pom[i] = P[i];
	}
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - 1; j++)
		{
			if (R[ND[j]] < R[ND[j + 1]])
			{
				swap(ND[j], ND[j + 1]);
			}
		}
	}
	while (nd != 0 || d != 0)
	{
		if (nd != 0)
		{
			if (R[ND[nd - 1]] <= t)
			{
				D[d] = ND[nd - 1];
				d++;
				nd--;
				for (int k = d - 1; k > 0; k--)
				{
					if (Q[D[k]] < Q[D[k - 1]])
					{
						swap(D[k], D[k - 1]);
					}
				}
				if (poz != 100)
				{
					if (Q[D[d - 1]] > Q[poz])
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
				ile_zr = min(pom[poz], R[ND[nd - 1]] - t);
			}
			else
			{
				ile_zr = pom[poz];
			}
			t += ile_zr;
			pom[poz] -= ile_zr;
			if (pom[poz] == 0)
			{
				cmax = max(cmax, t + Q[poz]);
				poz = 100;
			}
			continue;
		}
		if (d == 0 && nd != 0)
		{
			if (R[ND[nd - 1]] > t)
			{
				t = R[ND[nd - 1]];
			}
		}
	}
	return cmax;
}
void Blok(int n, int* R, int* P, int* Q, int* X, int& cI, int& cR, int& cQ)
{
	int posB = -1, m = 0, cmax = 0;
	int tmp[100];
	for (int i = 0; i < n; i++)
	{
		int j = X[i];
		tmp[i] = (m >= R[j]);
		m = max(m, R[j]) + P[j];
		if (cmax < m + Q[j])
		{
			cmax = m + Q[j];
			posB = i;
		}
	}
	int i = posB, j = -1;
	int bQ = Q[X[posB]];
	int bR = R[X[posB]];
	int bP = P[X[posB]];
	while (tmp[i])
	{
		if (Q[X[--i]] < bQ)
		{
			j = X[i];
			break;
		}
		bR = min(bR, R[X[i]]);
		bP += P[X[i]];
	}
	cI = j;
	cR = bR + bP;
	cQ = bQ + bP;
}
void Carlier(int n, int* R, int* P, int* Q, int* X, int& UB)
{
	if (schrage_podziel(n, R, P, Q) >= UB)
	{
		return;
	}
	int sCmax = schrage(n, R, P, Q, X);
	if (sCmax < UB)
	{
		UB = sCmax;
	}
	int j, jr, jq;
	Blok(n, R, P, Q, X, j, jr, jq);
	if (j < 0)
	{
		return;
	}
	int tmpR = R[j];
	int tmpQ = Q[j];
	R[j] = jr;
	Carlier(n, R, P, Q, X, UB);
	R[j] = tmpR;
	Q[j] = jq;
	Carlier(n, R, P, Q, X, UB);
	Q[j] = tmpQ;
}
int main()
{
	int R[100], P[100], Q[100], X[100];
	string s = "data.00", s1, s2;
	ifstream f("data.txt");
	int n;
	for (int i = 0; i < 1; i++)
	{
		s1 = s + to_string(i) + ":";
		while (s2 != s1)
		{
			f >> s2;
		}
		f >> n;
		for (int j = 0; j < n; j++)
		{
			f >> R[j] >> P[j] >> Q[j];
		}
		int UB = schrage(n, R, P, Q, X);
		Carlier(n, R, P, Q, X, UB);
		cout << s2 << endl;
		cout << "Schrage: " << schrage(n, R, P, Q, X) << endl;
		cout << "Schrage z podzialem: " << schrage_podziel(n, R, P, Q) << endl;
		cout << "Carlier: " << UB << endl;
	}
}