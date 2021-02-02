#pragma once
enum KeyCode {
	UpArrow = 0, DownArrow = 1, LeftArrow = 2, RightArrow = 3, Enter = 4, Esc = 5,Backspace=6,tab=7,space=8
};
class InputSystem
{
public:
	static InputSystem* singleton;
	InputSystem();
	~InputSystem();
	void init();
	void keyUpdate();
	bool getKeyState(int);
private:
	bool keyState[6];
};

