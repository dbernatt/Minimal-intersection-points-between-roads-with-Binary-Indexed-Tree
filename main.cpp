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

double meret = 5;
double tavolsag = 20;

struct elek
{
	int x;
	int y;
};

struct draw_city
{
	double x;
	double y;
};

void beolvas(const char *filenames, elek *&el, int &n, int &m, int &elsz)
{

	cout << "Beolvas " << filenames << " allomanybol...\n";

	fstream file(filenames);

	file >> n >> m >> elsz;
	el = new elek[elsz];

	for (int i = 0; i < elsz; i++)
	{
		file >> el[i].x >> el[i].y;
	}
	file.close();
	cout << "Beolvas " << filenames << " allomanybol vege.\n";
}

void kiir_bemenet(elek *el, int n, int m, int elsz)
{

	cout << "Nyugaton levo varosok szama : " << n << endl;
	cout << "Keleten levo varosok szama : " << m << endl;
	cout << "Autopalyak szama : " << elsz << endl;
	for (int i = 0; i < elsz; ++i)
	{
		cout << el[i].x << " -> " << el[i].y << endl;
	}
	cout << "-------------------------" << endl
		 << endl;
}

void kiir_allomany(char *fnev)
{

	int n;
	int m;
	int elsz;
	fstream fki(fnev);
	fki >> n >> m >> elsz;
	cout << n << " " << m << " " << elsz << endl;
	int x, y;
	while (fki >> x && fki >> y)
	{
		cout << x << " " << y << endl;
	}
	cout << endl;
	fki.close();
}

void init(elek *el, int *&v, int elsz)
{
	for (int i = 0; i < elsz; ++i)
	{
		v[i] = el[i].y;
	}
}

void rendez_jobb_ha_bal_egyenlo(elek *&el, int elsz)
{

	for (int i = 0; i < elsz - 1; ++i)
	{
		for (int j = i + 1; j < elsz; ++j)
		{
			if (el[i].x == el[j].x)
			{
				if (el[i].y > el[j].y)
				{
					swap(el[i], el[j]);
				}
			}
		}
	}
}

void quicksort(elek *&el, int l, int elsz)
{
	if (l >= elsz)
		return;

	int pivot = el[elsz].x;
	int cnt = l;

	for (int i = l; i <= elsz; i++)
	{
		if (el[i].x < pivot)
		{
			swap(el[cnt], el[i]);
			cnt++;
		}
	}
	quicksort(el, l, cnt - 2);
	quicksort(el, cnt, elsz);
}

void rendez(elek *&el, int elsz)
{
	// quicksort(el, 0, elsz-1);

	cout << "Rendez elek...\n";
	for (int i = 0; i < elsz - 1; ++i)
	{
		for (int j = i + 1; j < elsz; ++j)
		{
			if (el[i].x > el[j].y)
			{
				swap(el[i], el[j]);
			}
		}
	}
	rendez_jobb_ha_bal_egyenlo(el, elsz);
	cout << "Rendez vege.\n";
}

long int metszespontok(elek *el, int elsz)
{
	long int c = 0;
	int jobb;

	for (int i = 0; i < elsz - 1; ++i)
	{
		jobb = el[i].y;
		for (int j = i + 1; j < elsz; ++j)
		{
			if (el[j].y < jobb)
				c++;
		}
	}
	return c;
}

long int metszespontokBIT(elek *&el, int *v, BITree<int> &b, int elsz)
{

	long int inverziokSzama = 0;
	for (int i = elsz - 1; i >= 0; i--)
	{
		inverziokSzama += b.getSumBIT(v[i] - 1);
		b.updateBIT(v[i], 1);
	}
	return inverziokSzama;
}

void make_coord(draw_city *&nyugatiVarosCoord, int n, draw_city *&keletiVarosCoord, int m)
{
	double k = meret + 10.0;

	for (int i = 0; i < n; i++)
	{
		nyugatiVarosCoord[i].x = 100;
		nyugatiVarosCoord[i].y = k;
		k += tavolsag;
	}

	k = meret + 10;

	for (int i = 0; i < m; ++i)
	{
		keletiVarosCoord[i].x = 900;
		keletiVarosCoord[i].y = k;
		k += tavolsag;
	}
}

void abrazol(elek *&el, int elsz, int n, int m)
{

	RenderWindow window(VideoMode(1024, 1024), "Varosok es Metszespontok");

	CircleShape *nyugatiVaros = new CircleShape[n];
	CircleShape *keletiVaros = new CircleShape[m];

	draw_city *nyugatiVarosCoord = new draw_city[n];
	draw_city *keletiVarosCoord = new draw_city[m];

	make_coord(nyugatiVarosCoord, n, keletiVarosCoord, m);

	double k = meret + 10.0;

	for (int j = 0; j < n; j++)
	{
		nyugatiVaros[j].setRadius(meret);
		nyugatiVaros[j].setFillColor(Color(rand() % 255, rand() % 255, rand() % 255));
		nyugatiVaros[j].setPosition(nyugatiVarosCoord[j].x, nyugatiVarosCoord[j].y);
		k += tavolsag;
	}

	k = meret + 10.0;
	for (int j = 0; j < m; j++)
	{
		keletiVaros[j].setRadius(meret);
		keletiVaros[j].setFillColor(Color(rand() % 255, rand() % 255, rand() % 255));
		keletiVaros[j].setPosition(keletiVarosCoord[j].x, keletiVarosCoord[j].y);
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
			window.draw(nyugatiVaros[j]);
		}

		for (int j = 0; j < m; j++)
		{
			window.draw(keletiVaros[j]);
		}

		for (int i = 0; i < elsz; i++)
		{
			Vertex line[] = {
				Vertex(Vector2f(nyugatiVarosCoord[el[i].x - 1].x + meret, nyugatiVarosCoord[el[i].x - 1].y + meret),
					   nyugatiVaros[el[i].x - 1].getFillColor()),
				Vertex(Vector2f(keletiVarosCoord[el[i].y - 1].x + meret, keletiVarosCoord[el[i].y - 1].y + meret),
					   keletiVaros[el[i].y - 1].getFillColor())};
			window.draw(line, 2, sf::Lines);
		}

		window.display();
	}

	// for (int i = 0; i < n; ++i){
	// 	delete nyugatiVarosCoord[i];
	// 	delete nyugaztiVaros[i];
	// }

	// for (int i = 0; i < m; ++i)
	// {
	// 	delete keletiVarosCoord[i];
	// 	delete keletiVaros[i];
	// }

	delete[] nyugatiVaros;
	delete[] keletiVaros;
	delete[] keletiVarosCoord;
	delete[] nyugatiVarosCoord;

	return;
}

void urit(const char *filenames)
{
	cout << filenames << " allomany uritese...\n";
	ofstream myfile;
	myfile.open(filenames, ofstream::out | ofstream::trunc);
	cout << filenames << " allomany uritese vege.\n";
	myfile.close();
}

void general_bemenet(const char *filenames, int n, int m, int elsz)
{
	srand(time(NULL));
	bool ok = false;
	cout << "Varosok szama Nyugaton : ";
	cin >> n;
	while (n < 1)
	{
		cout << "Nullatol nagyobb pozitiv egesz szamot kerek!\n";
		cin >> n;
	};

	cout << "Varosok szama Keleten : ";
	cin >> m;
	while (m <= 0)
	{
		cout << "Nullatol nagyobb pozitiv egesz szamot kerek!\n";
		cin >> m;
	}
	cout << "Autopalyak szama : ";
	cin >> elsz;

	while (elsz < 0 && !ok)
	{
		if (elsz < 0)
		{
			cout << "Nullatol nagyobb pozitiv egesz szamot kerek!\n";
		}
		else
		{
			if (elsz > (n * m))
			{
				cout << "Maximum autopalyak szama : " << (n * m) << endl;
				ok = false;
			}
			else
			{
				ok = true;
			}
		}
		cin >> elsz;
	}

	fstream fgen(filenames);
	fgen << n << " ";
	fgen << m << " ";
	fgen << elsz << "\n";
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
	for (; i < elsz; i++)
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

void be_felhasznalo(const char *filenames, int n, int m, int elsz)
{
	bool ok = false;
	cout << "Varosok szama Nyugaton : ";
	cin >> n;
	while (n < 1)
	{
		cout << "Nullatol nagyobb pozitiv egesz szamot kerek!\n";
		cin >> n;
	};

	cout << "Varosok szama Keleten : ";
	cin >> m;
	while (m <= 0)
	{
		cout << "Nullatol nagyobb pozitiv egesz szamot kerek!\n";
		cin >> m;
	}
	cout << "Autopalyak szama : ";
	cin >> elsz;

	while (elsz < 0 && !ok)
	{
		if (elsz < 0)
		{
			cout << "Nullatol nagyobb pozitiv egesz szamot kerek!\n";
		}
		else
		{
			if (elsz > (n * m))
			{
				cout << "Maximum autopalyak szama : " << (n * m) << endl;
				ok = false;
			}
			else
			{
				ok = true;
			}
		}
		cin >> elsz;
	}

	fstream fgen(filenames);
	fgen << n << " ";
	fgen << m << " ";
	fgen << elsz << "\n";
	int x;
	int y;

	for (int i = 0; i < elsz; i++)
	{
		cout << i << ". autopalya : \n";
		ok = false;
		do
		{
			cout << "indul : ";
			cin >> x;
			if (x >= 1 && x <= n)
			{
				fgen << x << " ";
				ok = true;
			}
			else
			{
				cout << "Nem ervenyes kiindulopont! Ujra!\n";
			}
		} while (!ok);

		cout << i << ". autopalya :\n";
		ok = false;
		do
		{
			cout << "vegpont : ";
			cin >> y;
			if (y >= 1 && y <= m)
			{
				fgen << y << "\n";
				ok = true;
			}
			else
			{
				cout << "Nem ervenyes vegpont! Ujra!\n";
			}
		} while (!ok);
	}
	fgen.close();
}

void beker_fnev(const char filenames[10][256], int betest, char *&fnev)
{
	bool jo;
	do
	{
		jo = false;
		cout << " - [  ] Add meg az allomany nevet : ";
		cin >> fnev;
		for (int i = 0; i < betest && !jo; ++i)
		{
			if (strcmp(filenames[i], fnev) == 0)
			{
				jo = true;
			}
		}
		if (!jo)
		{
			cout << " - [  ] Nem letezo allomany!\n";
		}
	} while (!jo);
}

void kiir_filenames(const char filenames[10][256], int betest)
{
	for (int i = 0; i < betest; ++i)
		cout << " - [ " << i + 1 << " ] " << filenames[i] << "\n";
}

int main(int argc, char **argv)
{

	int n;
	int m;
	int elsz;
	elek *el;
	char feladat;
	bool ok = true;
	clock_t c;
	float ido;
	long int hanydb;
	char *fnev;
	int betest = 5;
	bool ok2 = true;

	const char filenames[10][256] = {"test1.txt", "test2.txt", "test3.txt", "test4.txt", "test5.txt"};

	n = m = elsz = 0;
	beolvas(filenames[0], el, n, m, elsz);
	rendez(el, elsz);
	while (ok)
	{

		cout << " - [ MENU ] -\n\n";
		cout << " - [ 1 ] Autopalyak metszespontjainak szama negyzetes algoritmussal!\n";
		cout << " - [ 2 ] Autopalyak metszespontjainak szama Binarisan Indexelt Faval!\n";
		cout << " - [ 3 ] Bemeneti Allomanyok Kezelese!\n";
		cout << " - [ 4 ] Kiir bemeneti allomany!\n";
		cout << " - [ 5 ] Grafikus abrazolas!\n";
		cout << " - [ 6 ] Kilepes!\n\n";
		cout << " -   ";
		cin >> feladat;
		system("clear");

		switch (feladat)
		{
		case '1':
		{
			cout << "Szamol...\n";
			c = clock();
			hanydb = metszespontok(el, elsz);
			cout << "Szamol vege.\n";
			ido = ((float)(clock() - c)) / CLOCKS_PER_SEC;
			printf("Futasi ido : %f sec. \n", ido);
			cout << "Metszespontok szama O(n^2) : " << hanydb << endl;
			cout << "---------------------\n\n";

			cout << "\n - [ 1 ] Vissza!\n";
			cout << " -  ";
			cin >> feladat;
		}
		break;

		case '2':
		{
			int *v;
			v = new int[elsz]();
			init(el, v, elsz);
			BITree<int> b(m);
			c = clock();
			cout << "Szamol...\n";
			hanydb = metszespontokBIT(el, v, b, elsz);
			cout << "Szamol vege.\n";
			ido = ((float)(clock() - c)) / CLOCKS_PER_SEC;
			printf("Futasi ido : %f sec.\n", ido);
			cout << "Metszespontok szama O(elsz * log(m)): " << hanydb << endl;
			cout << "---------------------\n\n";
			delete[] v;
			cout << "\n - [ 1 ] Vissza!\n";
			cout << " -  ";
			cin >> feladat;
		}
		break;

		case '3':
		{

			ok2 = true;
			while (ok2)
			{
				system("clear");
				cout << " - [ Bemeneti Allomanyok Kezelese ] -\n\n";
				cout << " - [ 1 ] Allomany kivalasztasa!\n";
				cout << " - [ 2 ] Bemenet felhasznalotol!\n";
				cout << " - [ 3 ] General egy meglevo allomanyba!\n";
				cout << " - [ 4 ] General minden meglevo allomanyba!\n";
				cout << " - [ 5 ] Urit minden allomanyt!\n";
				cout << " - [ 6 ] Urit egy allomanyt!\n";
				cout << " - [ 7 ] Kiir egy allomanyt!\n";
				cout << " - [ 8 ] Vissza Menuhoz!\n\n";

				cout << " -  ";
				cin >> feladat;

				switch (feladat)
				{
				case '1':
				{
					kiir_filenames(filenames, betest);
					fnev = new char[256];
					beker_fnev(filenames, betest, fnev);
					n = m = elsz = 0;
					delete[] el;
					beolvas(fnev, el, n, m, elsz);
					rendez(el, elsz);
					delete[] fnev;
				}
				break;

				case '2':
				{
					cout << " - [ Bemenet Felhasznalotol ] !\n\n";
					kiir_filenames(filenames, betest);
					fnev = new char[256];
					beker_fnev(filenames, betest, fnev);
					urit(fnev);
					be_felhasznalo(fnev, n, m, elsz);
					delete[] fnev;
					rendez(el, elsz);
				}
				break;

				case '3':
				{
					cout << " - [ General Meglevo Allomanyba ] !\n\n";
					kiir_filenames(filenames, betest);
					fnev = new char[256];
					beker_fnev(filenames, betest, fnev);
					urit(fnev);
					general_bemenet(fnev, n, m, elsz);
					rendez(el, elsz);
					delete[] fnev;
				}
				break;

				case '4':
				{
					for (int i = 0; i < betest; ++i)
					{
						urit(filenames[i]);
						general_bemenet(filenames[i], n, m, elsz);
					}
				}
				break;

				case '5':
				{
					for (int i = 0; i < betest; ++i)
						urit(filenames[i]);
				}
				break;

				case '6':
				{
					cout << " - [ Urit Egy Allomany ] \n\n";
					kiir_filenames(filenames, betest);
					fnev = new char[256];
					beker_fnev(filenames, betest, fnev);
					urit(fnev);
					delete[] fnev;
				}
				break;

				case '7':
				{
					cout << " - [ Kiir egy allomanyt ] \n\n";
					kiir_filenames(filenames, betest);
					fnev = new char[256];
					beker_fnev(filenames, betest, fnev);
					kiir_allomany(fnev);
					delete[] fnev;
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
					cout << "\n - [ " << betest + 1 << " ] Vissza!\n";
					cout << " -  ";
					cin >> feladat;
				}
			}
		}
		break;

		case '4':
		{
			kiir_bemenet(el, n, m, elsz);
		}
		break;

		case '5':
		{
			cout << "Grafikus abrazolas..(SFML).\n";

			abrazol(el, elsz, n, m);
		}
		break;

		default:
		{
			ok = false;
		}
		break;
		}
	}
	delete[] el;
	return 0;
}