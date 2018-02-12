#include "Macierz.h"
#include <iostream>
#include <string>
#include <fstream>
#include "tinyxml2.h"

using namespace tinyxml2;

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

int main()
{
	int sasiedztwo = 1;
	char div;
	bool diversification = true;
	double stop_cryterium = 120;
	int choice;

	double edge_cost;
	int vertex_number, wierzcholki, vertex_i;

	std::string fname = "test";
	XMLDocument xmlDoc;
	XMLError eResult;
	XMLNode * pRoot;
	XMLNode * graph;
	XMLElement * vertex;
	XMLElement * edge;

	Macierz *macierz;

	while (true)
	{

		std::cout << std::endl << "          1. Wczytaj dane z pliku (.xml)";
		std::cout << std::endl << "          2. Wprowadz kryterium stopu";
		std::cout << std::endl << "          3. Wlacz/Wylacz dywersifikacje";
		std::cout << std::endl << "          4. Wybierz sasiedztwo";
		std::cout << std::endl << "          5. Uruchom TabuSearch";
		std::cout << std::endl;
		std::cout << std::endl << "          ";

		std::cin >> choice;

		switch (choice)
		{
		case 1:

			std::cout << std::endl << "          Podaj nazwe pliku do wczytania grafu: ";
			std::cin >> fname;
			fname = fname + ".xml";

			eResult = xmlDoc.LoadFile(fname.c_str());
			XMLCheckResult(eResult);


			pRoot = xmlDoc.FirstChildElement("travellingSalesmanProblemInstance");
			if (pRoot == nullptr) return XML_ERROR_FILE_READ_ERROR;

			graph = pRoot->FirstChildElement("graph");
			if (pRoot == nullptr) return XML_ERROR_FILE_READ_ERROR;

			vertex = graph->FirstChildElement("vertex");
			if (vertex == nullptr) return XML_ERROR_PARSING_ELEMENT;

			edge = vertex->FirstChildElement("edge");
			if (vertex == nullptr) return XML_ERROR_PARSING_ELEMENT;


			wierzcholki = 0;

			while (vertex != nullptr)
			{
				vertex = vertex->NextSiblingElement("vertex");

				wierzcholki++;
			}

			macierz = new Macierz(wierzcholki);

			vertex = graph->FirstChildElement("vertex");
			if (vertex == nullptr) return XML_ERROR_PARSING_ELEMENT;

			vertex_i = 0;

			while (vertex != nullptr)
			{
				while (edge != nullptr)
				{

					eResult = edge->QueryIntText(&vertex_number);
					XMLCheckResult(eResult);

					eResult = edge->QueryDoubleAttribute("cost", &edge_cost);
					XMLCheckResult(eResult);

					edge = edge->NextSiblingElement("edge");

					macierz->a[vertex_i][vertex_number] = (int)edge_cost;

				}
				vertex = vertex->NextSiblingElement("vertex");

				if (vertex != nullptr)
					edge = vertex->FirstChildElement("edge");

				vertex_i++;
			}
			break;

		case 2:
			std::cout << std::endl << "     Podaj maksymalny czas(w sekundach) szukania rozwiazania: ";
			std::cin >> stop_cryterium;
			break;
			
		case 3:
			std::cout << std::endl << "     Dywersyfikacja?(t/n): ";
			std::cin >> div;
			if (div == 'n')
				diversification = false;
			else if (div == 't')
				diversification = true;
			break;
		case 4:
			std::cout << std::endl << "     Sasiedztwo (1.Swap, 2.Insert, 3.Invert): ";
			std::cin >> sasiedztwo;
			break;
		case 5:
			macierz->sasiedztwo = sasiedztwo;
			macierz->TabuSearch(diversification, stop_cryterium);
			break;
		}
	}
}
