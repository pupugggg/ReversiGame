#include "Manager.h"
#include "GameParameter.h"
#include "InputSystem.h"
#include "DisplaySystem.h"
#include <iostream>
#pragma comment(lib, "winmm.lib")
Manager::Manager()
{
	init();
	InputSystem::singleton = new InputSystem();
}


Manager::~Manager()
{
}

Manager* Manager::singleton=NULL;

void Manager::init()
{
	board = new int*[GameParameter::boarder];
	for (int i = 0; i < GameParameter::boarder; i++)
	{
		board[i] = new int[GameParameter::boarder];
		for (int j = 0; j < GameParameter::boarder; j++)
			board[i][j] = GameParameter::empty;
	}
	board[3][3] = board[4][4] = GameParameter::white;
	board[3][4] = board[4][3] = GameParameter::black;
	index = 0;
	winner = GameParameter::empty;
	round = GameParameter::black;
	while (!temp.empty())
	{
		for (int i = 0; i < GameParameter::boarder; i++)
			delete[]temp.top()[i];
		delete[]temp.top();
		temp.pop();
	}
	while (!record.empty())
	{
		for (int i = 0; i < GameParameter::boarder; i++)
			delete[]record.top()[i];
		delete[]record.top();
		record.pop();
	}
	pushRecord(board);
}


void Manager::start()
{
	// TODO: 請在此新增您的實作程式碼.
	PlaySound(TEXT("01mP.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	DisplaySystem::singleton = new DisplaySystem();
	update();
}

void Manager::curcorClamping()
{
	if (availableX.size() == 0 || availableY.size() == 0)
		return;
	if (this->index >=(int) availableX.size())
		this->index = 0;
	if (this->index < 0)
		this->index = ((int)availableX.size() - 1);
}

int Manager::calcChess(int round)
{
	int sum = 0;
	for (int i = 0; i < GameParameter::boarder; i++)
	{
		for (int j = 0; j < GameParameter::boarder; j++)
		{
			if (board[i][j] == round)
				sum++;
		}
	}
	return sum;
}

void Manager::update()
{
	// TODO: 請在此新增您的實作程式碼.
	while (!winCheck())
	{
		
		InputSystem::singleton->keyUpdate();
		//悔棋
		if (InputSystem::singleton->getKeyState(KeyCode::Backspace))
		{
			regret();
			DisplaySystem::singleton->drawBoard();
		}
		//下棋
		else if (InputSystem::singleton->getKeyState(KeyCode::space))
		{
			findFilpableTerminal();
			for (int i = 0; i < linkX.size(); i++)
			{
				int stepX = ((int)abs(linkX[i] - availableX[index])) / ((linkX[i] - availableX[index]) == 0 ? 1 : (linkX[i] - availableX[index]));
				int stepY = ((int)abs(linkY[i] - availableY[index])) / ((linkY[i] - availableY[index]) == 0 ? 1 : (linkY[i] - availableY[index]));
				int x = availableX[index] + stepX, y = availableY[index] + stepY;
				board[availableY[index]][availableX[index]] = round;
				while (stepX != 0 || stepY != 0)
				{
					if (y == linkY[i])
						stepY = 0;
					if (x == linkX[i])
						stepX = 0;
					board[y][x] = round;
					y += stepY;
					x += stepX;
				}
			}
			if ((int)availableX.size() != 0)
				round = !round;
			pushRecord(board);
			while (!temp.empty())
			{
				for (int i = 0; i < GameParameter::boarder; i++)
					delete[]temp.top()[i];
				delete[]temp.top();
				temp.pop();
			}
			DisplaySystem::singleton->drawBoard();
			index = 0;          
		}
		//悔棋復原
		else if (InputSystem::singleton->getKeyState(KeyCode::tab))
		{
			recover();
			DisplaySystem::singleton->drawBoard();
		}
		//重新開始
		else if (InputSystem::singleton->getKeyState(KeyCode::Enter))
		{
			restart();
			DisplaySystem::singleton->drawBoard();
		}
		//選擇位置
		else if (InputSystem::singleton->getKeyState(KeyCode::UpArrow))
		{
			DisplaySystem::singleton->eraseSelectedPos(index++);
		}
		else if (InputSystem::singleton->getKeyState(KeyCode::DownArrow))
		{
			DisplaySystem::singleton->eraseSelectedPos(index--);
		}
		else if (InputSystem::singleton->getKeyState(KeyCode::LeftArrow))
		{
			DisplaySystem::singleton->eraseSelectedPos(index--);
		}
		else if (InputSystem::singleton->getKeyState(KeyCode::RightArrow))
		{
			DisplaySystem::singleton->eraseSelectedPos(index++);
		}
		else if (InputSystem::singleton->getKeyState(KeyCode::Esc))
		{
			std::exit(EXIT_SUCCESS);
		}
		curcorClamping();
	}
	DisplaySystem::singleton->showWinner();
}


void Manager::restart()
{
	// TODO: 請在此新增您的實作程式碼.
	init();
}

void Manager::recover()
{
	if (temp.empty())
		return;
	record.push(temp.top());
	temp.pop();
	pushRecord(temp.top());
	temp.pop();
	for (int i = 0; i < GameParameter::boarder; i++)
		for (int j = 0; j < GameParameter::boarder; j++)
			board[i][j] = record.top()[i][j];
}

void Manager::regret()
{
	if (record.size() <= 2)
		return;
	temp.push(record.top());
	record.pop();
	temp.push(record.top());
	record.pop();
	for (int i = 0; i < GameParameter::boarder; i++)
		for (int j = 0; j < GameParameter::boarder; j++)
			board[i][j] = record.top()[i][j];
}

void Manager::pushRecord(int **content)
{
	int **tmp = new int*[GameParameter::boarder];
	for (int i = 0; i < GameParameter::boarder; i++)
	{
		tmp[i] = new int[GameParameter::boarder];
		for (int j = 0; j < GameParameter::boarder; j++)
			tmp[i][j] =content[i][j];
	}
	record.push(tmp);
}

void Manager::findFilpableTerminal()
{
	if (availableX.size() == 0)
		return;
	linkX.clear();
	linkY.clear();
	int i = availableY[index], j = availableX[index];
	if (board[i][j] == GameParameter::empty)
	{
		if (i - 1 >= 0 && board[i - 1][j] == !round && board[i - 1][j] != GameParameter::empty)
			findFlipablePos(round, i, j, -1, 0);
		if (i - 1 >= 0 && j + 1 < GameParameter::boarder && board[i - 1][j + 1] == !round && board[i - 1][j + 1] != GameParameter::empty)
			findFlipablePos(round, i, j, -1, +1);
		if (j + 1 < GameParameter::boarder && board[i][j + 1] == !round && board[i][j + 1] != GameParameter::empty)
			findFlipablePos(round, i, j, 0, +1);
		if (i + 1 < GameParameter::boarder && j + 1 < GameParameter::boarder && board[i + 1][j + 1] == !round && board[i + 1][j + 1] != GameParameter::empty)
			findFlipablePos(round, i, j, +1, +1);
		if (i + 1 < GameParameter::boarder && board[i + 1][j] == !round && board[i + 1][j] != GameParameter::empty)
			findFlipablePos(round, i, j, +1, 0);
		if (i + 1 < GameParameter::boarder && j - 1 >= 0 && board[i + 1][j - 1] == !round && board[i + 1][j - 1] != GameParameter::empty)
			findFlipablePos(round, i, j, +1, -1);
		if (j - 1 >= 0 && board[i][j - 1] == !round && board[i][j - 1] != GameParameter::empty)
			findFlipablePos(round, i, j, 0, -1);
		if (i - 1 >= 0 && j - 1 >= 0 && board[i - 1][j - 1] == !round && board[i - 1][j - 1] != GameParameter::empty)
			findFlipablePos(round, i, j, -1, -1);
	}
}

void Manager::findFlipablePos(int round, int centerY, int centerX, int biasY, int biasX)
{
	if (centerX + biasX >= 0 && centerX + biasX < GameParameter::boarder && centerY + biasY >= 0 && centerY + biasY < GameParameter::boarder)
	{
		if (board[centerY + biasY][centerX + biasX] == !round)
			findFlipablePos(round, centerY + biasY, centerX + biasX, biasY, biasX);
		else if (board[centerY + biasY][centerX + biasX] == round)
		{
			linkX.push_back(centerX + biasX);
			linkY.push_back(centerY + biasY);
		}
	}
}

void Manager::findPosition(int round, int centerY, int centerX , int biasY, int biasX)
{
	if (centerX + biasX>=0 && centerX + biasX<GameParameter::boarder && centerY + biasY>=0 && centerY + biasY<GameParameter::boarder)
	{
		if (board[centerY + biasY][centerX + biasX] == !round)
			findPosition(round, centerY + biasY, centerX + biasX, biasY, biasX);
		else if (board[centerY + biasY][centerX + biasX] == GameParameter::empty)
		{
			availableX.push_back(centerX + biasX);
			availableY.push_back(centerY + biasY);
		}
	}
}

bool Manager::findAllPositions(int round)
{
	bool flag = false;
	availableX.clear();
	availableY.clear();
	for (int i = 0; i < GameParameter::boarder; i++)
	{
		for (int j = 0; j < GameParameter::boarder; j++)
		{
			if (board[i][j] == round)
			{
				if (i - 1 >= 0 && board[i - 1][j] == !round && board[i - 1][j] != GameParameter::empty)
					findPosition(round, i, j, -1, 0);
				if (i - 1 >= 0 && j + 1 < GameParameter::boarder && board[i - 1][j + 1] == !round && board[i - 1][j + 1] != GameParameter::empty)
					findPosition(round, i, j, -1, +1);
				if (j + 1 < GameParameter::boarder && board[i][j + 1] == !round && board[i][j + 1] != GameParameter::empty)
					findPosition(round, i, j, 0, +1);
				if (i + 1 < GameParameter::boarder && j + 1 < GameParameter::boarder && board[i + 1][j + 1] == !round && board[i + 1][j + 1] != GameParameter::empty)
					findPosition(round, i, j, +1, +1);
				if (i + 1 < GameParameter::boarder && board[i + 1][j] == !round && board[i + 1][j] != GameParameter::empty)
					findPosition(round, i, j, +1, 0);
				if (i + 1 < GameParameter::boarder && j - 1 >=0 && board[i + 1][j - 1] == !round && board[i + 1][j - 1] != GameParameter::empty)
					findPosition(round, i, j, +1, -1);
				if (j - 1 >= 0 && board[i][j - 1] == !round && board[i][j - 1] != GameParameter::empty)
					findPosition(round, i, j, 0, -1);
				if (i - 1 >= 0 && j - 1 >= 0 && board[i - 1][j - 1] == !round && board[i - 1][j - 1] != GameParameter::empty)
					findPosition(round, i, j, -1, -1);
			}
		}
	}
	if (availableX.size() != 0)
		flag = false;
	else
		flag = true;
	return flag;
}


bool Manager::winCheck()
{
	// TODO: 請在此新增您的實作程式碼.
	bool flag = false;
	if (round!=GameParameter::black)
	{
		flag = findAllPositions(GameParameter::white);
		flag=flag&&findAllPositions(GameParameter::black);
	}
	else
	{
		flag = findAllPositions(GameParameter::black);
		flag = flag && findAllPositions(GameParameter::white);
	}
	DisplaySystem::singleton->drawAvailalePos();
	return flag;
}
