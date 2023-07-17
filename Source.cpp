#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <WinBase.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include<cwchar>
#include <fstream>

using namespace std;

//dimensiunile chenarului
#define DIM_X 40
#define DIM_Y 75

//caracterele pentru chenar
#define CHR_STANGA_SUS 201
#define CHR_STANGA_JOS 200
#define CHR_DREAPTA_SUS 187
#define CHR_DREAPTA_JOS 188
#define CHR_BARA_VERTICALA 186
#define CHR_BARA_ORIZONTALA 205
#define CHR_RACHETA 24
#define CHR_CORP_MINGE 'o'
#define CHR_CARAMIDA_DESTRUCTIBILA 22
#define CHR_CARAMIDA_INDESTRUCTIBILA 21
#define CHR_BARA 19
#define CHR_CARAMIDA_X2DESTRUCTIBILA 23
#define CHR_CARAMIDA_SPECIALA 20

//variabile globale
char scena[DIM_X][DIM_Y];
char caramizi[DIM_X / 2][DIM_Y];
int lungime_bara=5;
int bara_x[DIM_Y];
int viteza_x = 0;
int prag_viteza = 101;
int minge_x;
int minge_y;
int push=0;
int viteza_minge_x=0;
int viteza_minge_y=0;
int racheta_x = DIM_X - 3;
bool firsttime = true;
int caramizi_speciale[DIM_X][DIM_Y];
int intrare_minge=1;
int score = 0;
int viteza_mingii = 1;
int viteza_barii;
int numar_lovituri = 0;
int rachetute = 0;
int k;
int viteza_joc=0;
bool bara_e_dubla = false;
bool racheta_shot = false;
bool select_start = true;
bool select_help = false;
bool super_minge = false;


//Functie folosita pentru a ascunde cursorul.
void hidecursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void spargere_caramida(int x, int y);

void logica_rachetute()
{
	racheta_shot = true; //Variabila bool care imi tine minte daca este o racheta care zboara in scena
	k = (bara_x[0] + bara_x[lungime_bara - 1]) / 2; //pozitioneaza racheta la mijlocul barii
	racheta_x = DIM_X - 3; //pozitioneaza racheta initial pe o linie deasupra barii
	rachetute--; //sterge numarul total de rachete ramase
}

//Functie care muta cursorul la pozitia x,y in consola.
void gotoXY(int x, int y)
{
	COORD coord = { x, y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//Functie care imi initializeaza scena. Aceasta va fi apelata o singura data, la prima deschidere a jocului.
void initializeaza_scena()
{
	
	int i,j;
	srand(time(NULL));
	


	for (i = 0;i < DIM_X;i++)
		for (j = 0;j < DIM_Y;j++)
			scena[i][j] = ' ';
	//initializeaza scena cu spatii libere
	for (i = 0;i < DIM_X;i++)
	{
		scena[i][0] = CHR_BARA_VERTICALA;
		scena[i][DIM_Y - 1] = CHR_BARA_VERTICALA;
	}
	//seteaza barile verticale 
	for (i=0;i<DIM_Y;i++)
	{
		scena[0][i] = CHR_BARA_ORIZONTALA;
		scena[DIM_X - 1][i] = CHR_BARA_ORIZONTALA;
	}
	//seteaza barile orizontale
	scena[0][0] = CHR_STANGA_SUS;
	scena[0][DIM_Y - 1] = CHR_DREAPTA_SUS;
	scena[DIM_X - 1][0] = CHR_STANGA_JOS;
	scena[DIM_X - 1][DIM_Y - 1] = CHR_DREAPTA_JOS;
	//seteaza colturile
	for (i = 2;i < DIM_X / 2;i++)
		for (j = 2;j < DIM_Y - 2;j++)
			if (((j-2) % 5 != 0)&&(i%3==0))
				caramizi[i][j] = CHR_CARAMIDA_DESTRUCTIBILA;
			else caramizi[i][j] = ' ';
	
	//aici setez caramizile
	for (i = 2;i < DIM_X / 2;i += 2) // setarea aleatorie a barilor indestructibile
		for (j = 0;j < DIM_Y / 25;j++)
		{
			int k = rand()%DIM_Y;
			for (int p = k;p <= k + 4;p++)
				if (caramizi[i][p] != ' ')
					caramizi[i][p] = CHR_CARAMIDA_INDESTRUCTIBILA;
				else break;
			for (int p = k; p >= k - 4;p--)
				if (caramizi[i][p] != ' ')
					caramizi[i][p] = CHR_CARAMIDA_INDESTRUCTIBILA;
				else break;
			if (caramizi[i][k] != ' ')
				caramizi[i][k] = CHR_CARAMIDA_INDESTRUCTIBILA;
		}
	for (i = 2;i < DIM_X / 2;i += 2) // setarea aleatorie a barilor portocalii
		for (j = 0;j < DIM_Y / 25;j++)
		{
			int k = rand() % DIM_Y;
			for (int p = k;p <= k + 4;p++)
				if (caramizi[i][p] != ' ')
					caramizi[i][p] = CHR_CARAMIDA_X2DESTRUCTIBILA;
				else break;
			for (int p = k; p >= k - 4;p--)
				if (caramizi[i][p] != ' ')
					caramizi[i][p] = CHR_CARAMIDA_X2DESTRUCTIBILA;
				else break;
			if (caramizi[i][k] != ' ')
				caramizi[i][k] = CHR_CARAMIDA_X2DESTRUCTIBILA;
		}
	
	for (i = 3; i <= DIM_X / 2; i += 3) //setarea aleatorie a barilor speciale
	{
		j = rand() % (DIM_Y - 2) + 2;
		if (caramizi[i][j] == ' ')
			j--;
		for (int p = j;p <= j + 4;p++)
			if (caramizi[i][p] != ' ')
				caramizi[i][p] = CHR_CARAMIDA_SPECIALA;
			else break;
		for (int p = j; p >= j - 4;p--)
			if (caramizi[i][p] != ' ')
				caramizi[i][p] = CHR_CARAMIDA_SPECIALA;
			else break;
		caramizi[i][j] = CHR_CARAMIDA_SPECIALA;
	}

}

void initializeaza_joc(int x)
{
	//initializeaza bara
	for (int i = 0;i < lungime_bara;i++)
		bara_x[i] = x + i;
}

//Functie care afiseaza scena.
void afiseaza_scena()
{
	
	COORD cursorPosition;
	cursorPosition.X = 0;
	cursorPosition.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition); 
	//cod care fluidizeaza miscarea, folosit in locul system("CLS")
	int i, j;
	
	
	
	for (i = 2;i < DIM_X / 2;i++)
		for (j = 2;j < DIM_Y-2;j++)
			if (caramizi[i][j]!=' ')
			scena[i][j] = caramizi[i][j]; //din matricea fantoma (caramizi), scrie barile
	scena[minge_x][minge_y] = CHR_CORP_MINGE; //scrie noua pozitie a mingii
	if (rachetute>0)
		scena[DIM_X-3][bara_x[lungime_bara/2]]=CHR_RACHETA; //daca sunt rachete, afiseaza constant una la baza barii
	if (racheta_shot == true)
	{
		scena[racheta_x][k] = CHR_RACHETA;
		racheta_x = racheta_x - 1; //daca a fost apasata tasta f, si sunt rachete, racheta se va duce in sus
	}
	if (scena[racheta_x - 1][k] != ' '&& scena[racheta_x - 1][k]!=CHR_CORP_MINGE) //aici verific daca racheta se va intalni cu vreo caramida
	{
		spargere_caramida(racheta_x - 1, k); //spargerea caramidei, asemenea mingii
		scena[racheta_x][k] = ' '; //sterge racheta
		racheta_shot = false; 
		racheta_x = 2;
		k= 2;
	}
	for (i = 0;i < DIM_X;i++) //afisarea se face cu switch pentru a avea mai multe culori
	{
		for (j = 0;j < DIM_Y;j++)
			switch (scena[i][j])
			{case CHR_CARAMIDA_DESTRUCTIBILA: 
				printf("\033[1;33m");
				printf("%c", 219); //CHR_CARAMIDA_(etc) tine minte o valoare aleatorie, afisarea simbolului pentru caramida se face nominal
				printf("\033[0m");
				break;
			 case CHR_CARAMIDA_INDESTRUCTIBILA:
				 printf("\033[1;30m");
				 printf("%c", 219);
				 printf("\033[0m");
				break;
			 case CHR_BARA:
				 printf("\033[1;30m");
				 printf("%c", 22);
				 printf("\033[0m");
				 break;
			 case CHR_CARAMIDA_X2DESTRUCTIBILA:
				 printf("\033[0;33m");
				 printf("%c", 219);
				 printf("\033[0m");
				 break;
			 case CHR_CORP_MINGE:
				 if (super_minge)//verifica daca mingea a primit o super putere
					 switch (intrare_minge) //la fiecare intrare in functie, mingea isi va schimba culoarea
					 {
					 case 1:
						 printf("\033[0;31m");
						 printf("%c", scena[i][j]);
						 printf("\033[0m");
						 intrare_minge++;
						 break;
					 case 2:
						 printf("\033[0;31m");
						 printf("%c", scena[i][j]);
						 printf("\033[0m");
						 intrare_minge++;
						 break;
					 case 3:
						 printf("\033[0;32m");
						 printf("%c", scena[i][j]);
						 printf("\033[0m");
						 intrare_minge++;
						 break;
					 case 4:
						 printf("\033[0;33m");
						 printf("%c", scena[i][j]);
						 printf("\033[0m");
						 intrare_minge = 1;
						 break;

					 }
				 else
					 printf("%c", scena[i][j]);
				 break;
			 case CHR_CARAMIDA_SPECIALA:
				printf("\033[0;32m");
			    printf("%c", 219);
			    printf("\033[0m");
				 break;
			 
			default:
				printf("%c", scena[i][j]);
				break;
			}
		
	    	
		
		
		printf("\n");
		
	}
	
	
	printf("Score:%d\nNumar de rachetute:%d", score,rachetute); //afiseaza scorul si numarul de rachete sub matrice
	printf("\n");
	
	
}

void caramida_speciala()
{
	srand(time(NULL)); //la spargerea caramizii speciale se face in random, intr-o variabila, care va dicta puterea primita
	int putere = rand() % 3 + 1;
	
	if (putere == 1 && bara_e_dubla == false) //variabila de tip bool are scopul de a nu dubla de doua ori bara
	{
		numar_lovituri = 0;
		bara_e_dubla = true;
		lungime_bara = lungime_bara * 2; //dubleaza bara
		int ok = 0; //o variabila locala ajutatoare
		for (int i = 0;i < lungime_bara / 2;i++)
			if (bara_x[i] == DIM_Y - lungime_bara / 2) //daca bara este prea aproape de bara din dreapta
				ok++; // variabila ajutatoare isi schimba valoarea
		if (ok)
			initializeaza_joc(bara_x[0] - lungime_bara / 2); //daca bara este prea aproape de cadran, aceasta se muta mai la stanga, apoi se dubleaza spre dreapta
		else
			initializeaza_joc(bara_x[0]); // daca bara are loc, atunci se dubleaza spre dreapta
		

	}
	

	if (putere == 2)
	{
		rachetute = 5; //doar schimba numarul de rachete la care are acces jucatorul

	}
	
	if (putere == 3)
	{
		super_minge = true; //schimba variabila globala de tip bool, variabila verificata in alta functie

	}


}

void spargere_caramida(int x, int y)
{
	if (caramizi[x][y] == CHR_CARAMIDA_DESTRUCTIBILA || caramizi[x][y] == CHR_CARAMIDA_SPECIALA) 
	{//la fiecare spargere a unei caramizi, se modifica variabila numar_lovituri, daca bara este dubla
	 //Dupa 5 lovituri, bara se injumatateste si se schimba variabila globala de tip bool
		if (bara_e_dubla)
		{
			numar_lovituri++;
			if (numar_lovituri == 5)
			{
				numar_lovituri = 0;
				bara_e_dubla = false;
				lungime_bara = lungime_bara / 2;
			}
		}
		
		if (caramizi[x][y] == CHR_CARAMIDA_SPECIALA)
			caramida_speciala();
		score += 100;
		caramizi[x][y] = ' ';
		for (int i = 1;i < 4;i++)
		{ //sterge fiecare simbol, de la punctul de coliziune, pana la primul spatiu din dreapta
			if (caramizi[x][y + i] == CHR_CARAMIDA_DESTRUCTIBILA|| caramizi[x][y+i] == CHR_CARAMIDA_SPECIALA)
				caramizi[x][y + i] = ' ';
			else break;
		}
		for (int i = 1;i < 4;i++)
		{//sterge fiecare simbol, de la punctul de coliziune, pana la primul spatiu din stanga
			if (caramizi[x][y - i] == CHR_CARAMIDA_DESTRUCTIBILA|| caramizi[x][y-i] == CHR_CARAMIDA_SPECIALA)
				caramizi[x][y - i] = ' ';
			else break;
		}
	}
	
	if (caramizi[x][y] == CHR_CARAMIDA_X2DESTRUCTIBILA)
	{
		for (int i = 1;i < 4;i++) //acelasi principiu ca la loop-urile de distrugere, doar ca inlocuieste simbolul
		{
			if (caramizi[x][y + i] == CHR_CARAMIDA_X2DESTRUCTIBILA)
				caramizi[x][y + i] = CHR_CARAMIDA_DESTRUCTIBILA;
			else break;
		}
		for (int i = 1;i < 4;i++)
		{
			if (caramizi[x][y - i] == CHR_CARAMIDA_X2DESTRUCTIBILA)
				caramizi[x][y - i] = CHR_CARAMIDA_DESTRUCTIBILA;
			else break;
		}
		caramizi[x][y] = CHR_CARAMIDA_DESTRUCTIBILA;
	}
}


void pozitia_mingii()
{
	if (firsttime) //acest if verifica daca este prima data cand se apasa "sageata sus" pentru minge
	{if (push == 0)
		{//Mingea isi porneste deplasarea, initial, doar daca tasta f este apasata. Daca nu, aceasta va sta la mijlocul barii
			minge_x = DIM_X - 3;
			minge_y = bara_x[lungime_bara / 2];
		}
		else
		{//aici este algoritmul pentru prima apasare
			if (viteza_x >= 1)
			{
				viteza_minge_x = -viteza_mingii;
				viteza_minge_y = viteza_mingii;
			} //daca bara se afla in deplasare, atunci si mingea va primi o viteza orizontala, pe langa cea verticala
			else
				if (viteza_x <= -1)
				{
					viteza_minge_x = -viteza_mingii;
					viteza_minge_y = -viteza_mingii;
				}
				else
					viteza_minge_x = -viteza_mingii; //daca tasta "sageata sus" este apasata, iar bara este statica, mingea se va duce vertical in sus.
			firsttime = false;

		}
}
else
{if (minge_x == 1)
viteza_minge_x = viteza_mingii;
if (minge_y == 1)
viteza_minge_y = viteza_mingii;
if (minge_y == DIM_Y - 2)
viteza_minge_y = -viteza_mingii;
//aici se face coliziunea cu bara de sus si cele laterale
if (minge_x < DIM_X / 2 && minge_y>2)

if (caramizi[minge_x + viteza_minge_x][minge_y + viteza_minge_y] == CHR_CARAMIDA_DESTRUCTIBILA || caramizi[minge_x + viteza_minge_x][minge_y + viteza_minge_y] == CHR_CARAMIDA_INDESTRUCTIBILA || caramizi[minge_x + viteza_minge_x][minge_y + viteza_minge_y] == CHR_CARAMIDA_X2DESTRUCTIBILA|| caramizi[minge_x + viteza_minge_x][minge_y + viteza_minge_y] == CHR_CARAMIDA_SPECIALA)
{if (super_minge)
{//face spargerea caramizii atinse si a vecinilor, daca bila are putere speciala
	spargere_caramida(minge_x + viteza_minge_x, minge_y + viteza_minge_y);
	spargere_caramida(minge_x + viteza_minge_x-3, minge_y + viteza_minge_y);
	spargere_caramida(minge_x + viteza_minge_x, minge_y + viteza_minge_y+4);
	spargere_caramida(minge_x + viteza_minge_x, minge_y + viteza_minge_y-4);
	super_minge = false;

}else
	spargere_caramida(minge_x + viteza_minge_x, minge_y + viteza_minge_y);
	viteza_minge_x = -viteza_mingii * viteza_minge_x;
}

minge_x += viteza_minge_x; //aceste doua linii de cod fac miscarea continua a mingii
minge_y += viteza_minge_y;
if (minge_y >= bara_x[0] && minge_y <= bara_x[lungime_bara - 1] && minge_x == DIM_X - 3) //verificarea coliziunii cu bara
if (viteza_x >= 1)
{
	viteza_minge_x = -viteza_mingii;
	viteza_minge_y = viteza_mingii;
}
else
if (viteza_x <= -1) //acelasi algoritm de ricosare
{
	viteza_minge_x = -viteza_mingii;
	viteza_minge_y = -viteza_mingii;
}
else
viteza_minge_x = -viteza_mingii;
}


	
		
	
}



void pozitioneaza_bara()
{//inlocuieste simbolurile din scena cu simbolurile pentru bara.
	for (int i = 0; i < lungime_bara; i++)
		scena[DIM_X-2][bara_x[i]] = CHR_BARA;
}


void curata_scena()
{
	int i, j;
	
	//Acest lucru se realizeaza prin completarea cu ' '(spatiu) in interiorul matricii
	for (i = 1;i <= DIM_X - 2;i++)
		for (j = 1;j <= DIM_Y - 2;j++)
			scena[i][j] = ' ';
	for (int j = 0;j < DIM_Y;j++)
		scena[0][j] = scena[DIM_X - 1][j] = CHR_BARA_ORIZONTALA;


	//seteaza barele verticale (stanga si dreapta)
	for (int i = 0;i < DIM_X;i++)
		scena[i][0] = scena[i][DIM_Y - 1] = CHR_BARA_VERTICALA;
	//seteaza colturile chenarului
	scena[0][0] = CHR_STANGA_SUS;
	scena[0][DIM_Y - 1] = CHR_DREAPTA_SUS;
	scena[DIM_X - 1][0] = CHR_STANGA_JOS;
	scena[DIM_X - 1][DIM_Y - 1] = CHR_DREAPTA_JOS;
}


void calculeaza_pozitia_barii(int &x)
{//aceasta functie face posibila miscarea barii. Daca este prea aproape de cadran, incetineste si opreste
	if (bara_x[0] == 2 && x < 0) 
	{
		x = 0;
		for (int i = 0;i < lungime_bara;i++)
			bara_x[i] -= 1;
	}
	else
		if (bara_x[0] == 1 && x < 0)
			x = 0;
		else
			if ((bara_x[lungime_bara - 1] == DIM_Y - 3) && x > 0)
			{
				x = 0;
				for (int i = 0;i < lungime_bara;i++)
					bara_x[i] += 1;
			}
			else
				if ((bara_x[lungime_bara - 1] == DIM_Y - 2) && x > 0)
					x = 0;
				else
					for (int i = 0;i < lungime_bara;i++)
						bara_x[i] += x;
	
	
	
		
}

bool joc_pierdut()
{
	if (minge_x == DIM_X - 1)
		return true;
	return false;
	//Daca trece de bara, jocul este pierdut
}

void pauza()
{
	char ch;
	ch = '[';
	printf("Paused");
	//Jocul intra intr-un loop, pana cand se apasa "space"
	while (ch != ' ')
		ch = _getch();
	for (int i = 3;i >= 1;i--)
	{//acest for face o numaratoare inversa pentru a pregati jucatorul
		system("CLS");
		afiseaza_scena();
		printf("%d", i);
		Sleep(1000);
	}
	system("CLS");
	afiseaza_scena();
	printf("Continue!");
	Sleep(1000);
	system("CLS");
	afiseaza_scena();
	//aceste comenzi pregatesc jocul
}



void verifica_tastele()
{//functie care verifica tastele apasate si face modificari in functie de ele
	char ch;
	if (_kbhit()) //verifica daca este apasata tastatura
	{
		ch = _getch();
		switch (ch)
		{
		case 75:
			viteza_x = -2; //daca este apasata "sageata stanga" se schimba viteza barii
			break;
		case 77:
			viteza_x = 2;//daca este apasata "sageata dreapta" se schimba viteza barii
			break;
		case 80:
			viteza_x = 0;//daca este apasata "sageata jos" se opreste bara
			break;
		case 72:
			push = 1; //daca este apasata "sageata sus" incepe jocul
			break;
		case ' ': //este apasat "space", se apeleaza functia pauza
			pauza();
			break;
		case 'm':
			if(prag_viteza>50) //se apasa "m", se schimba viteza jocului
			prag_viteza-=50;
			break;
		case 'n':
			if(prag_viteza<400)//se apasa "n", se schimba viteza jocului
			prag_viteza+=50;
			break;
		case 'f':
			if(rachetute>0&&!racheta_shot) //daca sunt rachetut si niciuna nu este in scena, se apeleaza 'logica_rachetute'
			logica_rachetute();
			break;

		}
	}
	
}


void meniu()
{
	
	
	meniu:
	
	char matrice_meniu[18][75];
	ifstream menu;
	menu.open("matrice_menu.txt");
	for (int i=1;i<18;i++)
	{
		for (int j = 1;j < 75;j++)
		{
			char a;
			menu >> a;
			matrice_meniu[i][j] = a;
		}
		//se citeste matricea meniu din fisier
		//in matrice, se afla cifre printre simboluri, care se interpreteaza in switch-ul de mai jos
	}



	for (int i = 1;i < 18;i++)
	{
		for (int j = 1;j < 75;j++)
			switch (matrice_meniu[i][j])
			{
			case '0':
				printf(" ");
				break;
			case '1':
				printf("%c",CHR_BARA_ORIZONTALA);
				break;
			case '2':
				printf("%c",CHR_DREAPTA_SUS);
				break;
			case '3':
				printf("%c",CHR_BARA_VERTICALA);
				break;
			case '4':
				printf("%c",CHR_DREAPTA_JOS);
				break;
			case '5':
				printf("%c",CHR_STANGA_JOS);
				break;
			case '6':
				printf("%c",CHR_STANGA_SUS);
				break;
			case '7':
				if (select_start == true)
				{
					printf("\033[0;31m");
					printf("%c", CHR_BARA_ORIZONTALA);
					printf("\033[0m");
				}
				else
					printf(" ");
				break;
			case '8':
				if (select_start == false)
				{
					printf("\033[0;31m");
					printf("%c", CHR_BARA_ORIZONTALA);
					printf("\033[0m");
				}
				else
					printf(" ");
				break;
			default:
				printf("%c", matrice_meniu[i][j]);
				break;
			}
		printf("\n");
	}
	menu.close();
	
	//variabila de tip bool select_start, retine ce bara este 'highlighted'
	
	{
		switch (_getch())
		{
		case '\r':
			if (select_start == true) //bara rosie sub start, incepe jocul
			{
				goto iesire_meniu;
				break;
			}
			else
				select_help = true;
			break;
		case 75: //prin sageti se schimba bara
			select_start = true;
			break;
		case 77:
			select_start = false;
			break;
		}
	}
	COORD cursorPosition;
	cursorPosition.X = 0;
	cursorPosition.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);

	if (select_help == true) //variabila select_help ajuta la evitarea unui bug
	{ //continutul din help este scris tot cu cifre in anumite puncte, interpretate de switch
		system("CLS");
		ifstream help;
		help.open("Help.TXT");
		char a='3';
		
		while (a != '4')
		{help >> a;
		if (a == '0')
			printf(" ");
		else
			if (a == '7')
				printf("\n");
			else
				if (a == '4')
					break;
				else
					printf("%c", a);
		
	    }
		select_help = false;
		_getch();
		
			system("CLS");
	}
	goto meniu;
iesire_meniu:;
	
}

bool joc_castigat()
{//verifica daca se mai afla vreo caramida, ce se poate sparge, in scena
	bool ok = false;
	for (int i = 0;i < DIM_X;i++)
		for (int j = 0;j < DIM_Y;j++)
			if (scena[i][j] == CHR_CARAMIDA_DESTRUCTIBILA || scena[i][j] == CHR_CARAMIDA_SPECIALA || scena[i][j] == CHR_CARAMIDA_X2DESTRUCTIBILA)
				ok = true;
	if (ok == false)
		return true;
	else return false;
}


void main()
{
	
	
	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0); //functie de full screen
	
	hidecursor();
	meniu();
	system("CLS");
	initializeaza_scena();
	initializeaza_joc(DIM_Y/2);

	while (!joc_pierdut())
	{
		
		
			curata_scena();

			pozitioneaza_bara();

			pozitia_mingii();

			calculeaza_pozitia_barii(viteza_x);

			afiseaza_scena();

			verifica_tastele();

			Sleep(prag_viteza); //face posibila schimbarea vitezei jocului

	}//sfarsitul buclei principale a programului


	//Afisarea mesajului de final in cazul pierderii jocului.
	curata_scena();
	afiseaza_scena();
	//dupa ce mingea depaseste bara, se verifica statutul jocului
	if (joc_castigat())
	{
		gotoXY(DIM_X-10, DIM_Y / 4+2);
		printf("Joc castigat!");
	}
	else
	{
		gotoXY(DIM_X-10, DIM_Y / 4+2);//se deplaseaza cursorul pentru afisarea mesajului
		printf("Joc pierdut!");
	}

	_getch();

}