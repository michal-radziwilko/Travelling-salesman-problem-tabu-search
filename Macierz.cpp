#include "Macierz.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <Windows.h>
#include <random>


using namespace std;

Macierz::Macierz(int wierzcholki_)
{
	wierzcholki = wierzcholki_;
	a = new int *[wierzcholki];


	for (int i = 0; i < wierzcholki; i++)
	{
		a[i] = new int[wierzcholki];
	}

	for (int i = 0; i < wierzcholki; i++)
	{
		for (int j = 0; j < wierzcholki; j++)
			a[i][j] = -1;	
	}

	route = new int[wierzcholki + 1];

	GenerateNewRoute();
}


Macierz::~Macierz()
{
	for (int i = 0; i < wierzcholki; i++)
		delete[] a[i];
	
	delete[] a;
}

void Macierz::losowaMacierz()
{
	srand(time(NULL));


	for (int i = 0; i < wierzcholki; i++)
	{
		for (int j = 0; j < wierzcholki; j++)
		{
			if(i==j) a[i][j] = -1;

			else a[i][j] = ((rand() % 100) + 1);
		}
	}
}


void Macierz::wpiszMacierz()
{

	for (int i = 0; i < wierzcholki; i++)
	{
		for (int j = 0; j < wierzcholki; j++)
		{
			cin >> a[i][j];
		}
	}
}


void Macierz::wyswietlMacierz()
{
	cout << endl << endl << "Graf w postaci macierzy: " << endl << endl;

	for (int i = 0; i < wierzcholki; i++)
	{
		for (int j = 0; j < wierzcholki; j++)
		{
			cout.precision(4);
			cout << setw(4) << a[i][j];
		}

		cout << endl << endl;
	}

	cout << endl << endl;
}


void Macierz::Greedy()
{
	boolean *visited = new boolean[wierzcholki];
	for (int i = 0; i < wierzcholki; i++)
		visited[i] = false;
	route[wierzcholki] = 0;
	int min = 0, next = 0, current = 0;
	for (int i = 0; i < wierzcholki; i++)
	{
		visited[current] = true;
		min = 0;

		for (int j = 0; j < wierzcholki; j++)
		{
			if (!visited[j] && (a[current][j] < min || min == 0))
			{
				min = a[current][j];
				next = j;
			}
		}
		route[i] = current;
		current = next;
	}

	cout << endl << endl << "Trasa (Greedy): " << route[0];
	for (int i = 1; i < wierzcholki + 1; i++)
		cout << "-> " << route[i];

	cout << endl << "Dlugosc(Greedy): " << RouteLength(route);

	cout << endl << endl;
}

int* Macierz::Swap(int *series ,int i, int j)
{
	int temp;
	temp = series[i];
	series[i] = series[j];
	series[j] = temp;

	return series;
}

int* Macierz::Insert(int *series, int i, int j)
{
	if (i > j)
	{
		for (int a = i; a > j; a--)
			Swap(series, a, a - 1);
	}
	else if (i < j)
	{
		for (int a = i; a < j - 1; a++)
			Swap(series, a, a + 1);
	}
	return series;
}

int* Macierz::Invert(int *series, int i, int j)
{
	int substring_length, b, c;
	if (i < j)
	{
		substring_length = j - (i - 1);
		b = i;
		c = j;
		
		for (int a = 0; a < floor(substring_length / 2); a++)
		{
			Swap(series, b, c);
			b++;
			c--;
		}
	}
	if (i > j)
	{
		substring_length = i - (j - 1);
		b = j;
		c = i;

		for (int a = 0; a < floor(substring_length / 2); a++)
		{
			Swap(series, b, c);
			b++;
			c--;
		}
	}
	return series;
}

int Macierz::RouteLength(int *route)
{
	int length = 0;

	for (int i = 0; i < wierzcholki; i++)
		length += a[route[i]][route[i + 1]];

	return length;
}

void Macierz::GenerateNewRoute()
{
	random_device rd; // non-deterministic generator
	mt19937 gen(rd()); // random engine seeded with rd()
	uniform_int_distribution<> dist(1, wierzcholki - 1); // distribute results between

	int random;
	bool *generated;
	generated = new bool[wierzcholki];
	generated[0] = true;
	route[0] = 0;
	route[wierzcholki] = 0;
	for (int i = 1; i < wierzcholki; i++)
		generated[i] = false;

	for (int i = 1; i < wierzcholki; i++)
	{
		random = dist(gen);

		while (generated[random])
			random = dist(gen);

			route[i] = random;
			generated[random] = true;
	}
}

void Macierz::TabuSearch(bool diversification, double stop_cryterium)
{
	time_t timer1;
	time(&timer1);
	time_t  timer2;
	double second;

	int GlobalMin = RouteLength(route), MovedLength, temp = 0;
	int moved[2], counter = 0, divCounter = counter + 1000;
	TabuList = new int *[wierzcholki];
	int Cadency = (int)sqrt(wierzcholki);
	int *routeCopy, *GlobalMinRoute;
	routeCopy = new int[wierzcholki + 1];
	GlobalMinRoute = new int[wierzcholki + 1];
	boolean **TabuMove = new boolean *[wierzcholki];


	for (int i = 0; i < wierzcholki; i++)
	{
		TabuList[i] = new int[wierzcholki];
		TabuMove[i] = new boolean[wierzcholki];
	}

	for (int i = 0; i < wierzcholki; i++)
	{
		for (int j = 0; j < wierzcholki; j++)
			TabuList[i][j] = 0;
	}

	while (second <= stop_cryterium)
	{
		temp = 0;
		for (int i = 0; i < wierzcholki; i++)
		{
			for (int j = 0; j < wierzcholki; j++)
				TabuMove[i][j] = false;
		}

		for (int i = 0; i < wierzcholki + 1; i++)
			routeCopy[i] = route[i];

		for (int i = 1; i < wierzcholki; i++)
		{
			for (int j = 1; j < wierzcholki; j++)
			{
				if (i != j && (!TabuMove[i][j] && !TabuMove[j][i]))
				{
					for (int k = 0; k < wierzcholki + 1; k++)
						routeCopy[k] = route[k];

					switch (sasiedztwo)
					{
					case 1:
						MovedLength = RouteLength(Swap(routeCopy, i, j));
						TabuMove[i][j] = true;
						break;

					case 2:
						MovedLength = RouteLength(Insert(routeCopy, i, j));
						TabuMove[i][j] = true;
						break;

					case 3:
						MovedLength = RouteLength(Invert(routeCopy, i, j));
						TabuMove[i][j] = true;
						break;

					default:
						MovedLength = RouteLength(Swap(routeCopy, i, j));
						TabuMove[i][j] = true;
						break;
					}

					if (MovedLength < temp || temp == 0)
					{
						temp = MovedLength;
						moved[0] = i;
						moved[1] = j;
					}
				}
			}
		}

		if (diversification)
		{

			// Sprawdzanie listy ruchów zakazanych z uwzglêdnieniem kryterium aspiracji
			if ((TabuList[moved[0]][moved[1]] == 0 && TabuList[moved[1]][moved[0]] == 0) || temp < GlobalMin)
			{
				switch (sasiedztwo)
				{
				case 1:
					Swap(route, moved[0], moved[1]);
					break;

				case 2:
					Insert(route, moved[0], moved[1]);
					break;

				case 3:
					Invert(route, moved[0], moved[1]);
					break;

				default:
					Swap(route, moved[0], moved[1]);
					break;
				}


				TabuList[moved[0]][moved[1]] += Cadency;
				TabuList[moved[1]][moved[0]] += Cadency;

				if (temp < GlobalMin)
				{
					GlobalMin = temp;

					for (int i = 0; i < wierzcholki + 1; i++)
						GlobalMinRoute[i] = route[i];
				}
			}

			// Jesli algorytm nie zmienia rozwi¹zania od 20 iteracji to generujemy losowego s¹siada
			if (counter == divCounter)
			{
				if (GlobalMin <= temp)
				{
					GenerateNewRoute();
				}

				divCounter += 1000;

			}
		}

		else if (!diversification)
		{
			if ((TabuList[moved[0]][moved[1]] == 0 && TabuList[moved[1]][moved[0]] == 0) || temp < GlobalMin)
			{
				switch (sasiedztwo)
				{
				case 1:
					Swap(route, moved[0], moved[1]);
					break;

				case 2:
					Insert(route, moved[0], moved[1]);
					break;

				case 3:
					Invert(route, moved[0], moved[1]);
					break;

				default:
					Swap(route, moved[0], moved[1]);
					break;
				}

				for (int i = 0; i < wierzcholki; i++)
				{
					for (int j = 0; j < wierzcholki; j++)
					{
						if (TabuList[i][j] != 0)
							TabuList[i][j] -= 1;
					}
				}

				TabuList[moved[0]][moved[1]] += Cadency;

				if (temp < GlobalMin)
				{
					GlobalMin = temp;

					for (int i = 0; i < wierzcholki + 1; i++)
						GlobalMinRoute[i] = route[i];
				}
			}
		}

		for (int i = 0; i < wierzcholki; i++)
		{
			for (int j = 0; j < wierzcholki; j++)
			{
				if (TabuList[i][j] != 0)
					TabuList[i][j] = TabuList[i][j] - 1;
			}
		}

		counter++;

		time(&timer2);
		second = difftime(timer2, timer1);

		}
		if (diversification)
			cout << endl << endl << "Dywersyfikacja" << endl;
		else if(!diversification)
			cout << endl << endl << "bez dywersyfikacji" << endl;

		cout << endl << endl << "Najlepsza ze wszystkich iteracji trasa (Tabu Search)" << counter << ": " << GlobalMinRoute[0];
		for (int i = 1; i < wierzcholki + 1; i++)
			cout << "-> " << GlobalMinRoute[i];

		cout << endl << "Jej dlugosc wynosi(Tabu Search)" << counter << ": " << GlobalMin << endl << endl;

	}
