#pragma once
class Macierz
{
public:
	int wierzcholki, sasiedztwo;
	Macierz(int wierzcholki_);
	~Macierz();
	void wyswietlMacierz();
	void losowaMacierz();
	int **a;
	void wpiszMacierz();
	void TabuSearch(bool diversification, double stop_cryterium);
	void Greedy();
	int *route, **TabuList;
	int *Swap(int *series ,int i, int j);
	int RouteLength(int *route);
	void GenerateNewRoute();
	int *Insert(int *series, int i, int j);
	int *Invert(int *series, int i, int j);
};

