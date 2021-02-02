#pragma once
#ifndef WINDOWS_H
#define WINDOWS_H
#include<Windows.h>
#endif


#include<iostream>
#include<string>
class DisplaySystem
{
public:
	static DisplaySystem* singleton;
	HANDLE hConsole;
	DisplaySystem();
	~DisplaySystem();
	void drawBoard();
	void drawAvailalePos();
	void eraseAvailablePos();
	void drawSelectedPos();
	void eraseSelectedPos(int);
	void showIfo(int x,int y,std::string);
	void showWinner();
	void showMenu();
	std::string getIcon(int,int);
	void init();
	std::string view[18];
	COORD cursor;
	void gotoXY(int x, int y);
};

