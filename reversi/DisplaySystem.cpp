#include "DisplaySystem.h"
#include"GameParameter.h"
#include"Manager.h"
DisplaySystem::DisplaySystem()
{
	hConsole= GetStdHandle(STD_OUTPUT_HANDLE);
	init();
	
}


DisplaySystem::~DisplaySystem()
{
}

void DisplaySystem::gotoXY(int x, int y)
{
	cursor.X = x;
	cursor.Y = y;
	SetConsoleCursorPosition(hConsole, cursor);
}

DisplaySystem* DisplaySystem::singleton = NULL;

void DisplaySystem::init()
{
	drawBoard();
	
}

void DisplaySystem::drawBoard()
{
	
	SetConsoleTextAttribute(hConsole, 7);
	system("CLS");
	showMenu();
	view[0]  = "שÝשששששששששששששששששששששששששששששששß";                                                                                                                           
	view[16] = "שדשששששששששששששששששששששששששששששששו";
	view[17] = "¡@¢ֿ¡@¢׀¡@¢ׁ¡@¢ׂ¡@¢׃¡@¢װ¡@¢ױ¡@¢ײ";
	for (int i = 0; i < 18; i++)
	{
		if (i != 0 && i < 16)
		{
			if (i % 2 != 0)
			{
				view[i] = "שר";
				for (int j = 0; j < GameParameter::boarder; j++)
					view[i] += getIcon((i - 1) / 2, j) + "שר";
				view[i] += " "+std::to_string((i-1)/2 + 1) ;
			}
			else
				view[i] = "שרשששששששששששששששששששששששששששששששר";
		}
		std::cout << view[i] <<std::endl;
	}
	showIfo(18, 0, "¡³¡G" + std::to_string(Manager::singleton->calcChess(GameParameter::black)) + "¡@¡´¡G" + std::to_string(Manager::singleton->calcChess(GameParameter::white)));
	showIfo(18, 3, (Manager::singleton->round == GameParameter::black ? "Black's turn" : "White's turn"));
}

void DisplaySystem::drawAvailalePos()
{
	if (Manager::singleton->availableX.size() == 0)
		return;    
	SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);
	for (int i = 0; i < Manager::singleton->availableX.size(); i++)
	{
		gotoXY(4*Manager::singleton->availableX[i]+2, 2*Manager::singleton->availableY[i]+1);
		std::cout << "¡@";
	}
	SetConsoleTextAttribute(hConsole, 7);
	drawSelectedPos();
}

void DisplaySystem::eraseAvailablePos()
{
	SetConsoleTextAttribute(hConsole, 7);
	for (int i = 0; i < Manager::singleton->availableX.size(); i++)
	{
		gotoXY(4 * Manager::singleton->availableX[i] + 2, 2 * Manager::singleton->availableY[i] + 1);
		std::cout << "¡@";
	}
}

void DisplaySystem::drawSelectedPos()
{
	SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE);
	gotoXY(4 * Manager::singleton->availableX[Manager::singleton->index] + 2, 2 * Manager::singleton->availableY[Manager::singleton->index] + 1);
	std::cout << "¡@";
	SetConsoleTextAttribute(hConsole, 7);
}

void DisplaySystem::eraseSelectedPos(int index)
{
	SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);
	gotoXY(4 * Manager::singleton->availableX[index] + 2, 2 * Manager::singleton->availableY[index] + 1);
	std::cout << "¡@";
	SetConsoleTextAttribute(hConsole, 7);
}

void DisplaySystem::showIfo(int x,int y,std::string content)
{
	gotoXY(x * 2, y);
	std::string sTop="",sDown="";
	sTop += "שÝ"; sDown += "שד";
	for (int i = 0; i < ((content.length() % 2==0)? (content.length() / 2) : (content.length() / 2+1)); i++)
	{
		sTop += "שש";
		sDown += "שש";
	}
	sTop += "שß"; sDown += "שו";
	std::cout << sTop;
	gotoXY(x * 2, y + 1);
	std::cout << "שר" + content + (   (content.length() % 2 == 0) ? "שר": " שר");
	gotoXY(x * 2, y + 2);
	std::cout << sDown;
	gotoXY(0, 0);
}

void DisplaySystem::showWinner()
{
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
	int b = Manager::singleton->calcChess(GameParameter::black), w = Manager::singleton->calcChess(GameParameter::white);
	if (b > w)
		showIfo(18, 0, "¡@Black win!!¡@");
	else if (b < w)
		showIfo(18, 0, "¡@White win!!¡@");
	else
		showIfo(18, 0, "¡@Deuce!!¡@");
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");
}

void DisplaySystem::showMenu()
{
	showIfo(18, 6, "Press TAB to recover.");
	showIfo(18, 9, "Press Backspace to regret");
	showIfo(18, 12, "Press ENTER to restart.");
	showIfo(18, 15, "Press ESC to Quit.");
}

std::string DisplaySystem::getIcon(int i,int j)
{
	switch (Manager::singleton->board[i][j])
	{
	case 0:
		return "¡³";
		break;
	case 1:
		return "¡´";
		break;
	case -1:
		return "¡@";
		break;
	}
}
