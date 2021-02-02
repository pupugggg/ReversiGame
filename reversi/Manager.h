#pragma once
#include<stack>
#include<vector>
class Manager
{
public:
	friend class DisplaySystem;
	static Manager *singleton;
	Manager();
	~Manager();
	void init();
	void start();
	void update();
	void restart();
	void regret();
	void recover();
	bool winCheck();
	void curcorClamping();
	int calcChess(int);
	bool findAllPositions(int);
	void findPosition(int, int, int,int,int);
	void pushRecord(int**);
	void findFilpableTerminal();
	void findFlipablePos(int, int, int, int, int);
	private:
		std::vector<int> availableX, availableY, linkX, linkY;
		int index;
		int **board;
		int round;
		std::stack<int**> record;
		std::stack<int**> temp;
		int winner;
};

