#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>

struct Node
{
	char data;
	Node *next;

	Node()
	{
		next = NULL;
	}
};

struct Map
{
	char **M;
	int M_elements; //szerokoœæ
	int y; //wysokoœæ
	int M_size; //rozmiar jednego wymiaru tablicy
	

	void expand();
	void draw(char array_in[]);

	Map(int x, int y)
	{
		M_elements = 0;
		M_size = 1;

		for (int i = 0; i < x; i++)
		{
			expand();
		}

		this->y = y;
	}
};

void Map::draw(char array_out[])
{
	int k = 0;

	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < M_elements; j++)
		{
			M[j][i] = array_out[k];
			k++;
		}
	}
}
//-------Tablica dynamiczna (redukuje koszt z³o¿onoœci pamiêciowej):
void Map::expand()
{
	if (M_elements == 0)
	{
		M = new char*[M_size];
		M[M_elements] = new char[1000];
	}
	if (M_elements >= M_size)
	{
		M_size = M_elements * 2;

		char **buffor = new char*[M_size];

		for (int i = 0; i < M_elements; i++)
		{
			buffor[i] = M[i];
		}
		M = new char*[M_size];

		for (int i = 0; i < M_elements; i++)
		{
			M[i] = buffor[i];
		}
		M[M_elements] = new char[1000];
	}
	if (M_elements < M_size)
	{
		M[M_elements] = new char[1000];
	}

	M_elements++;
}

//----------------G¹sienica:
struct Worm
{
	Node *segment;
	int segment_quantity;
	//pozycja pocz¹tkowa G¹sienicy:
	int x; int y;
	bool tree;
	bool alive;

	void add_segment(char data);
	void delete_segment();
	void move();
	void get_dirty(char data);

	Worm(int seg_quant)
	{
		segment = NULL;
		segment_quantity = 0;
		
		for (int i = 0; i < seg_quant; i++)
		{
			add_segment('.');
		}

		x = 0;
		y = 0;

		alive = true;
		tree = false;
	}
};

void Worm::move()
{
	segment = segment->next;
}

void Worm::get_dirty(char data)
{
	segment->data = data;
}

void Worm::add_segment(char data)
{
	Node *new_segment = new Node;
	new_segment->data = data;

	if (segment != NULL && segment->next != NULL)
	{
		new_segment->next = segment->next;
		segment->next = new_segment;
		//segment = new_segment;
	}
	
	if (segment != NULL && segment->next == segment)
	{
		segment->next = new_segment;
		new_segment->next = segment;
	}

	if (segment == NULL)
	{
		segment = new_segment;
		segment->next = segment;
	}
	segment_quantity++;
}

void Worm::delete_segment()
{
	Node *buffor;

	buffor = segment;

	while (segment->next != buffor)
	{
		segment = segment->next;
	}
	segment->next = buffor->next;
	
	delete buffor;

	segment = segment->next;

	segment_quantity--;
}
//------------------------------FUNKCJA RUCHU G¥SIENICY PO MAPIE:
void action(Worm &caterpillar, Map &m)
{
	char option = m.M[caterpillar.x][caterpillar.y];

	switch (option)
	{
		case '.':
		{
			//Je¿eli g¹sienica wpadnie na kropkê:
			caterpillar.move();
			m.M[caterpillar.x][caterpillar.y] = caterpillar.segment->data;
			break;
		}
		case 'T':
		{	//Je¿eli g¹sienica wpadnie na drzewo
			caterpillar.tree = true;
			break;
		}
		case 'K':
		{
			//Je¿eli g¹sienica wpadnie na kolec
			caterpillar.delete_segment();
			if (caterpillar.segment_quantity < 1)
			{
				caterpillar.alive = false;
				break;
			}
			caterpillar.move();
			break;
		}
		case 'G':
		{	//Je¿eli g¹sienica wpadnie na grzyba
			m.M[caterpillar.x][caterpillar.y] = '.';
			caterpillar.add_segment('.');
			caterpillar.move();
			break;
		}
		default:
		{
			//Je¿eli g¹sienica wpadnie w ka³u¿ê
			caterpillar.move();
			caterpillar.get_dirty(m.M[caterpillar.x][caterpillar.y]);
			break;
		}
	}

	return;
}

void clear(Worm &caterpillar, Map &m)
{
	while (caterpillar.segment == NULL)
	{
		caterpillar.delete_segment();
	}

	delete[] m.M;
}

//-----------------------FUNKCJE WEJŒCIA:

int exp(int base, int index)
{
	int x = 1;

	for (int i = 0; i < index; i++)
	{
		x *= base;
	}

	return x;
}

int input_int(char *&pointer)
{
	int x = 0;
	int k = 0;

	while (*pointer != ' ' && *pointer != '\n' && *pointer != '\0' && pointer != NULL)
	{
		k++;
		pointer++;
	}
	for (int i = 0; i < k; i++)
	{
		pointer--;
		x = x + ((*pointer - 48) * exp(10, i));
	}
	pointer = pointer + k;

	return x;
}

char input_char(char *&pointer, bool &flag, int &b)
{
	char a;
	flag = true;

	if (*pointer == '1' || *pointer == '2' || *pointer == '3' || *pointer == '4' || *pointer == '5' || *pointer == '6' || *pointer == '7' || *pointer == '8' || *pointer == '9' || *pointer == '0')
	{
		b = input_int(pointer);
		pointer++;
		flag = false;
		a = *pointer;
		pointer++;
		return a;
	}
	else
	{
		if (*pointer == ' ')
		{
			pointer++;
		}
		flag = true;
		a = *pointer;
		pointer++;
		return a;
	}
}

//-------------MAIN:

int main()
{///Zastosuj tablice dynamiczne :)
	char array_in[100]; char array_in_2[100000]; char array_in_2a[100000]; char array_in_3[100000]; char array_out[9999]; char array_out_3[9999];
	int seg_quant; int x; int y;
	char *pointer;
	bool flag = true;
	FILE *file; FILE *file2; FILE *file3;
	
//---------------------------------TESTY:
	file = fopen("size.in", "r"); // Plik zawieraj¹cy rozmiar mapy oraz iloœæ segmentów g¹sienicy
	file2 = fopen("map.in", "r"); // Plik zawieraj¹cy zawartoœæ mapy
	file3 = fopen("steps.in", "r"); // Plik zawieraj¹cy ruchy g¹sienicy
//--------------------------------------

	fgets(array_in, 100, file);
	pointer = &array_in[0];
	//Wczytanie wymiarów mapy:
	x = input_int(pointer);
	pointer++;
	y = input_int(pointer);
	pointer++;

	Map m(x,y);

	//Wczytanie iloœci pocz¹tkowej segmentów:
	seg_quant = input_int(pointer);

	Worm caterpillar(seg_quant);

	//Rysowanie mapy:
	int k; int i = 0;

	while (i<x*y && pointer != NULL)
	{
		fgets(array_in_2, 99999, file2);
		pointer = &array_in_2[0];

		for (i; i < x*y; i++)
		{
			if (flag == true)
			{
				array_in_2a[i] = input_char(pointer, flag, k);
				pointer++;
			}
			if (flag == false)
			{
				for (int j = 1; j < k; j++)
				{
					array_in_2a[i + j] = array_in_2a[i];
				}
				flag = true;
				i = i + k - 1;
			}
			if (*pointer == '\n'||*pointer == '\0')
			{
				i++;
				break;
			}
		}
	}
	m.draw(array_in_2a);
//-------Wczytanie ruchów edka:
	
	char *option; int l = 0;

	fgets(array_in_3, 500000, file3);
	pointer = array_in_3;

	while (*pointer != '?')
	{
		for (int i = 0; *pointer != '\0' && *pointer != '\n' && pointer != NULL && *pointer != '?'; i++)
		{
			if (flag == true)
			{
				array_out_3[i] = input_char(pointer, flag, k);
				pointer++;
				l++;
			}
			if (flag == false)
			{
				for (int j = 1; j < k; j++)
				{
					array_out_3[i + j] = array_out_3[i];
					l++;
				}
				flag = true;
				i = i + k - 1;
			}
			if (*pointer == '\n' || *pointer == '\0')
			{
				i++;
				break;
			}
		}
	}
	
//-------Algorytm poruszaj¹cy G¹sienic¹:
	
		option = &array_out_3[0];

		for (int i = 0; i < l; i++)
		{
			if (caterpillar.alive == true)
			{
				switch (*option)
				{
					case 'p':
					{
						if (caterpillar.x >= m.M_elements - 1)
						{
							break;
						}
						else
						{
							caterpillar.x++;
							action(caterpillar, m);
							if (caterpillar.tree == true)
							{
								caterpillar.x--;
								caterpillar.tree = false;
							}
							break;
						}
					}
					case 'l':
					{
						if (caterpillar.x <= 0)
						{
							break;
						}
						else
						{
							caterpillar.x--;
							action(caterpillar, m);
							if (caterpillar.tree == true)
							{
								caterpillar.x++;
								caterpillar.tree = false;
							}
							break;
						}
					}
					case 'g':
					{
						if (caterpillar.y <= 0)
						{
							break;
						}
						else
						{
							caterpillar.y--;
							action(caterpillar, m);
							if (caterpillar.tree == true)
							{
								caterpillar.y++;
								caterpillar.tree = false;
							}
							break;
						}
					}
					case 'd':
					{
						if (caterpillar.y >= m.y - 1)
						{
							break;
						}
						else
						{
							caterpillar.y++;
							action(caterpillar, m);
							if (caterpillar.tree == true)
							{
								caterpillar.y--;
								caterpillar.tree = false;
							}
							break;
						}
					}
				}
				option++;
			}
			else
			{
				std::cout << "Zegnaj okrutny swiecie!" << '\n';
				std::cout << caterpillar.x << ' ' << caterpillar.y << '\n';
				break;
			}
			
		}

		if (caterpillar.alive == true)
		{
			std::cout << caterpillar.x << ' ' << caterpillar.y << '\n';
		}

//-------Wyœwietlenie mapy:

///Do pliku:

	std::fstream file4;
	file4.open("out.txt", std::ios::in | std::ios::out);

	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			file4 << m.M[j][i];
		}
			file4 << '\n';
	}

///Na konsoli:

	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			std::cout << m.M[j][i];
		}
		std::cout << '\n';
	}

	clear(caterpillar, m);
    return 0;
}

