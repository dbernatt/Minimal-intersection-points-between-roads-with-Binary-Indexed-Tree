#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <fstream>
#include <iosfwd>
#include <ctime>
#include <string.h>
#include <vector>
#include "BITree.h"

using namespace std;
using namespace sf;

const double citySize = 5;
const double dist = 20;

struct edges
{
	int x;
	int y;
};

struct draw_city
{
	double x;
	double y;
};

void read(const char *filenames, edges *&edge, int &n, int &m, int &numberOfEdges)
{

	cout << "Read from" << filenames << " ...\n";

	fstream file(filenames);

	file >> n >> m >> numberOfEdges;
	edge = new edges[numberOfEdges];

	for (int i = 0; i < numberOfEdges; i++)
	{
		file >> edge[i].x >> edge[i].y;
	}
	file.close();
	cout << "Read from" << filenames << " finshed.\n";
}

void print_input(edges *edge, int n, int m, int numberOfEdges)
{

	cout << "Number of cities on West: " << n << endl;
	cout << "Number of cities on East: " << m << endl;
	cout << "Number of highways: " << numberOfEdges << endl;
	for (int i = 0; i < numberOfEdges; ++i)
	{
		cout << edge[i].x << " -> " << edge[i].y << endl;
	}
	cout << "-------------------------" << endl
		 << endl;
}

void print_file(char *filename)
{

	int n;
	int m;
	int numberOfEdges;
	fstream outputFile(filename);
	outputFile >> n >> m >> numberOfEdges;
	cout << n << " " << m << " " << numberOfEdges << endl;
	int x, y;
	while (outputFile >> x && outputFile >> y)
	{
		cout << x << " " << y << endl;
	}
	cout << endl;
	outputFile.close();
}

void init(edges *edge, int *&v, int numberOfEdges)
{
	for (int i = 0; i < numberOfEdges; ++i)
	{
		v[i] = edge[i].y;
	}
}

void sort_right_if_left_equal(edges *&edge, int numberOfEdges)
{

	for (int i = 0; i < numberOfEdges - 1; ++i)
	{
		for (int j = i + 1; j < numberOfEdges; ++j)
		{
			if (edge[i].x == edge[j].x)
			{
				if (edge[i].y > edge[j].y)
				{
					swap(edge[i], edge[j]);
				}
			}
		}
	}
}

void quicksort(edges *&edge, int l, int numberOfEdges)
{
	if (l >= numberOfEdges)
		return;

	int pivot = edge[numberOfEdges].x;
	int cnt = l;

	for (int i = l; i <= numberOfEdges; i++)
	{
		if (edge[i].x < pivot)
		{
			swap(edge[cnt], edge[i]);
			cnt++;
		}
	}
	quicksort(edge, l, cnt - 2);
	quicksort(edge, cnt, numberOfEdges);
}

void sort(edges *&edge, int numberOfEdges)
{
	// quicksort(edge, 0, numberOfEdges-1);

	cout << "Sort edges...\n";
	for (int i = 0; i < numberOfEdges - 1; ++i)
	{
		for (int j = i + 1; j < numberOfEdges; ++j)
		{
			if (edge[i].x > edge[j].y)
			{
				swap(edge[i], edge[j]);
			}
		}
	}
	sort_right_if_left_equal(edge, numberOfEdges);
	cout << "Sorted successfully.\n";
}

long int crossings(edges *edge, int numberOfEdges)
{
	long int c = 0;
	int right;

	for (int i = 0; i < numberOfEdges - 1; ++i)
	{
		right = edge[i].y;
		for (int j = i + 1; j < numberOfEdges; ++j)
		{
			if (edge[j].y < right)
				c++;
		}
	}
	return c;
}

long int crossingsBIT(edges *&edge, int *v, BITree<int> &b, int numberOfEdges)
{

	long int numberOfInversions = 0;
	for (int i = numberOfEdges - 1; i >= 0; i--)
	{
		numberOfInversions += b.getSumBIT(v[i] - 1);
		b.updateBIT(v[i], 1);
	}
	return numberOfInversions;
}

void make_coord(draw_city *&westernCities, int n, draw_city *&easternCities, int m)
{
	double k = citySize + 10.0;

	for (int i = 0; i < n; i++)
	{
		westernCities[i].x = 100;
		westernCities[i].y = k;
		k += dist;
	}

	k = citySize + 10;

	for (int i = 0; i < m; ++i)
	{
		easternCities[i].x = 900;
		easternCities[i].y = k;
		k += dist;
	}
}

void graphical_representation(edges *&edge, int numberOfEdges, int n, int m)
{

	RenderWindow window(VideoMode(1024, 1024), "Minimal highway intersections between cities");

	CircleShape *westernCity = new CircleShape[n];
	CircleShape *easternCity = new CircleShape[m];

	draw_city *westernCities = new draw_city[n];
	draw_city *easternCities = new draw_city[m];

	make_coord(westernCities, n, easternCities, m);

	double k = citySize + 10.0;

	for (int j = 0; j < n; j++)
	{
		westernCity[j].setRadius(citySize);
		westernCity[j].setFillColor(Color(rand() % 255, rand() % 255, rand() % 255));
		westernCity[j].setPosition(westernCities[j].x, westernCities[j].y);
		k += dist;
	}

	k = citySize + 10.0;
	for (int j = 0; j < m; j++)
	{
		easternCity[j].setRadius(citySize);
		easternCity[j].setFillColor(Color(rand() % 255, rand() % 255, rand() % 255));
		easternCity[j].setPosition(easternCities[j].x, easternCities[j].y);
	}

	while (window.isOpen())
	{
		Event event;
		View view = window.getView();

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.key.code == Keyboard::Escape)
				window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			view.move(-2, 0);
			window.setView(view);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			view.move(2, 0);
			window.setView(view);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			view.move(0, -2);
			window.setView(view);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			view.move(0, 2);
			window.setView(view);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			view.zoom(0.997f);
			window.setView(view);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
		{
			view.zoom(1.003f);
			window.setView(view);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			window.setView(window.getDefaultView());
		}

		window.clear();

		for (int j = 0; j < n; j++)
		{
			window.draw(westernCity[j]);
		}

		for (int j = 0; j < m; j++)
		{
			window.draw(easternCity[j]);
		}

		for (int i = 0; i < numberOfEdges; i++)
		{
			Vertex line[] = {
				Vertex(Vector2f(westernCities[edge[i].x - 1].x + citySize, westernCities[edge[i].x - 1].y + citySize),
					   westernCity[edge[i].x - 1].getFillColor()),
				Vertex(Vector2f(easternCities[edge[i].y - 1].x + citySize, easternCities[edge[i].y - 1].y + citySize),
					   easternCity[edge[i].y - 1].getFillColor())};
			window.draw(line, 2, sf::Lines);
		}

		window.display();
	}

	// for (int i = 0; i < n; ++i){
	// 	delete westernCities[i];
	// 	delete nyugaztiVaros[i];
	// }

	// for (int i = 0; i < m; ++i)
	// {
	// 	delete easternCities[i];
	// 	delete easternCity[i];
	// }

	delete[] westernCity;
	delete[] easternCity;
	delete[] easternCities;
	delete[] westernCities;

	return;
}

void urit(const char *filenames)
{
	cout << filenames << " to empty...\n";
	ofstream myfile;
	myfile.open(filenames, ofstream::out | ofstream::trunc);
	cout << filenames << " is empty.\n";
	myfile.close();
}

void generate_input(const char *filenames, int n, int m, int numberOfEdges)
{
	srand(time(NULL));
	bool ok = false;
	cout << "Number of cities on West : ";
	cin >> n;
	while (n < 1)
	{
		cout << "Invalid input number!\n";
		cin >> n;
	};

	cout << "NUmber of cities on East : ";
	cin >> m;
	while (m <= 0)
	{
		cout << "Invalid input number!\n";
		cin >> m;
	}
	cout << "Number of highways : ";
	cin >> numberOfEdges;

	while (numberOfEdges < 0 && !ok)
	{
		if (numberOfEdges < 0)
		{
			cout << "Invalid input number!\n";
		}
		else
		{
			if (numberOfEdges > (n * m))
			{
				cout << "Maximum number of highways : " << (n * m) << endl;
				ok = false;
			}
			else
			{
				ok = true;
			}
		}
		cin >> numberOfEdges;
	}

	fstream fgen(filenames);
	fgen << n << " ";
	fgen << m << " ";
	fgen << numberOfEdges << "\n";
	int x;
	int y;

	vector<vector<int>> M;
	vector<int> v;
	for (int i = 0; i < n; i++)
	{
		v.push_back(i + 1);
		for (int j = 0; j < m; j++)
		{
			v.push_back(j + 1);
		}
		printf("i = %d\n", i);
		M.push_back(v);
		v.clear();
	}

	int i = 0;
	for (; i < numberOfEdges; i++)
	{
		x = rand() % M.size();
		y = (1 + (rand() % (M[x].size() - 1)));
		fgen << M[x][0] << " " << M[x][y] << "\n";
		M[x].erase(M[x].begin() + y);
		if (M[x].size() <= 1)
		{
			M.erase(M.begin() + x);
		}
		printf("%d\n", i);
	}
	system("clear");
	fgen.close();
}

void input_from_user(const char *filenames, int n, int m, int numberOfEdges)
{
	bool ok = false;
	cout << "Number of cities on West : ";
	cin >> n;
	while (n < 1)
	{
		cout << "Invalid input number!\n";
		cin >> n;
	};

	cout << "Number of cities on East : ";
	cin >> m;
	while (m <= 0)
	{
		cout << "Invalid input number!\n";
		cin >> m;
	}
	cout << "Number of highways : ";
	cin >> numberOfEdges;

	while (numberOfEdges < 0 && !ok)
	{
		if (numberOfEdges < 0)
		{
			cout << "Invalid input number!\n";
		}
		else
		{
			if (numberOfEdges > (n * m))
			{
				cout << "Maximum number of highways : " << (n * m) << endl;
				ok = false;
			}
			else
			{
				ok = true;
			}
		}
		cin >> numberOfEdges;
	}

	fstream fgen(filenames);
	fgen << n << " ";
	fgen << m << " ";
	fgen << numberOfEdges << "\n";
	int x;
	int y;

	for (int i = 0; i < numberOfEdges; i++)
	{
		cout << i << ". highway : \n";
		ok = false;
		do
		{
			cout << "start : ";
			cin >> x;
			if (x >= 1 && x <= n)
			{
				fgen << x << " ";
				ok = true;
			}
			else
			{
				cout << "Invalid strating city! Try again!\n";
			}
		} while (!ok);

		cout << i << ". highway :\n";
		ok = false;
		do
		{
			cout << "end : ";
			cin >> y;
			if (y >= 1 && y <= m)
			{
				fgen << y << "\n";
				ok = true;
			}
			else
			{
				cout << "Invalid ending city! Try again!\n";
			}
		} while (!ok);
	}
	fgen.close();
}

void read_file(const char filenames[10][256], int numberOfFiles, char *&filename)
{
	bool good = false;
	do
	{
		good = false;
		cout << " - [  ] Enter the filename : ";
		cin >> filename;
		for (int i = 0; i < numberOfFiles && !good; ++i)
		{
			if (strcmp(filenames[i], filename) == 0)
			{
				good = true;
			}
		}
		if (!good)
		{
			cout << " - [  ] This file does not exists!\n";
		}
	} while (!good);
}

void print_filenames(const char filenames[10][256], int numberOfFiles)
{
	for (int i = 0; i < numberOfFiles; ++i)
		cout << " - [ " << i + 1 << " ] " << filenames[i] << "\n";
}

int main(int argc, char **argv)
{

	int n;
	int m;
	int numberOfEdges;
	edges *edge;
	char command;
	bool ok = true;
	clock_t c;
	float runTime;
	long int numberofCrossings;
	char *filename;
	int numberOfFiles = 5;
	bool ok2 = true;

	const char filenames[10][256] = {"test1.txt", "test2.txt", "test3.txt", "test4.txt", "test5.txt"};

	n = m = numberOfEdges = 0;
	read(filenames[0], edge, n, m, numberOfEdges);
	sort(edge, numberOfEdges);
	while (ok)
	{

		cout << " - [ MENU ] -\n\n";
		cout << " - [ 1 ] Number of crossing points of highways with square algorithm!\n";
		cout << " - [ 2 ] Number of crossing points of highways with Binary Indexed Trees!\n";
		cout << " - [ 3 ] Manage input files!\n";
		cout << " - [ 4 ] Print input file!\n";
		cout << " - [ 5 ] Graphic representation!\n";
		cout << " - [ 6 ] Exit!\n\n";
		cout << " -   ";
		cin >> command;
		system("clear");

		switch (command)
		{
		case '1':
		{
			cout << "Calculating...\n";
			c = clock();
			numberofCrossings = crossings(edge, numberOfEdges);
			cout << "Done.\n";
			runTime = ((float)(clock() - c)) / CLOCKS_PER_SEC;
			printf("Run time : %f sec. \n", runTime);
			cout << "Number of crossings O(n^2) : " << numberofCrossings << endl;
			cout << "---------------------\n\n";

			cout << "\n - [ 1 ] Back!\n";
			cout << " -  ";
			cin >> command;
		}
		break;

		case '2':
		{
			int *v;
			v = new int[numberOfEdges]();
			init(edge, v, numberOfEdges);
			BITree<int> b(m);
			c = clock();
			cout << "Calculating...\n";
			numberofCrossings = crossingsBIT(edge, v, b, numberOfEdges);
			cout << "Done.\n";
			runTime = ((float)(clock() - c)) / CLOCKS_PER_SEC;
			printf("Run time : %f sec.\n", runTime);
			cout << "Run time O(numberOfEdges * log(m)): " << numberofCrossings << endl;
			cout << "---------------------\n\n";
			delete[] v;
			cout << "\n - [ 1 ] Vissza!\n";
			cout << " -  ";
			cin >> command;
		}
		break;

		case '3':
		{

			ok2 = true;
			while (ok2)
			{
				system("clear");
				cout << " - [ Manage input files ] -\n\n";
				cout << " - [ 1 ] Select file!\n";
				cout << " - [ 2 ] Input from user!\n";
				cout << " - [ 3 ] Generate input to a specific file!\n";
				cout << " - [ 4 ] Generate input to every file!\n";
				cout << " - [ 5 ] Empty every input file!\n";
				cout << " - [ 6 ] Empty a specific input file!\n";
				cout << " - [ 7 ] Print an input file content!\n";
				cout << " - [ 8 ] Back!\n\n";

				cout << " -  ";
				cin >> command;

				switch (command)
				{
				case '1':
				{
					print_filenames(filenames, numberOfFiles);
					filename = new char[256];
					read_file(filenames, numberOfFiles, filename);
					n = m = numberOfEdges = 0;
					delete[] edge;
					read(filename, edge, n, m, numberOfEdges);
					sort(edge, numberOfEdges);
					delete[] filename;
				}
				break;

				case '2':
				{
					cout << " - [ Input from user ] !\n\n";
					print_filenames(filenames, numberOfFiles);
					filename = new char[256];
					read_file(filenames, numberOfFiles, filename);
					urit(filename);
					input_from_user(filename, n, m, numberOfEdges);
					delete[] filename;
					sort(edge, numberOfEdges);
				}
				break;

				case '3':
				{
					cout << " - [ Generate input to a specific file ] !\n\n";
					print_filenames(filenames, numberOfFiles);
					filename = new char[256];
					read_file(filenames, numberOfFiles, filename);
					urit(filename);
					generate_input(filename, n, m, numberOfEdges);
					sort(edge, numberOfEdges);
					delete[] filename;
				}
				break;

				case '4':
				{
					for (int i = 0; i < numberOfFiles; ++i)
					{
						urit(filenames[i]);
						generate_input(filenames[i], n, m, numberOfEdges);
					}
				}
				break;

				case '5':
				{
					for (int i = 0; i < numberOfFiles; ++i)
						urit(filenames[i]);
				}
				break;

				case '6':
				{
					cout << " - [ Empty every input file ] \n\n";
					print_filenames(filenames, numberOfFiles);
					filename = new char[256];
					read_file(filenames, numberOfFiles, filename);
					urit(filename);
					delete[] filename;
				}
				break;

				case '7':
				{
					cout << " - [ Print an input file content ] \n\n";
					print_filenames(filenames, numberOfFiles);
					filename = new char[256];
					read_file(filenames, numberOfFiles, filename);
					print_file(filename);
					delete[] filename;
				}
				break;

				default:
				{
					ok2 = false;
					system("clear");
				}
				break;
				}
				if (ok2 != false)
				{
					cout << "\n - [ " << numberOfFiles + 1 << " ] Back!\n";
					cout << " -  ";
					cin >> command;
				}
			}
		}
		break;

		case '4':
		{
			print_input(edge, n, m, numberOfEdges);
		}
		break;

		case '5':
		{
			cout << "Graphic representation....\n";

			graphical_representation(edge, numberOfEdges, n, m);
		}
		break;

		default:
		{
			ok = false;
		}
		break;
		}
	}
	delete[] edge;
	return 0;
}