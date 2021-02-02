#include "InputSystem.h"
#include"GameParameter.h"
#include<conio.h>

InputSystem::InputSystem()
{
}


InputSystem::~InputSystem()
{
	
}

bool InputSystem::getKeyState(int idx)
{
	return keyState[idx];
}

InputSystem* InputSystem::singleton=NULL;

void InputSystem::init()
{
	for (int i = 0; i < GameParameter::totalKey; i++)
		keyState[i] = false;
}

void InputSystem::keyUpdate() 
{
	for (int i = 0; i < GameParameter::totalKey; i++)
		keyState[i] = false;
	switch (_getch())
	{
	case 72://up
		keyState[KeyCode::UpArrow] = true;
		break;
	case 75://left
		keyState[KeyCode::LeftArrow] = true;
		break;
	case 77://right
		keyState[KeyCode::RightArrow] = true;
		break;
	case 80://down
		keyState[KeyCode::DownArrow] = true;
		break;
	case 13://enter
		keyState[KeyCode::Enter] = true;
		break;
	case 8://backspace
		keyState[KeyCode::Backspace] = true;
		break;
	case 9://tab
		keyState[KeyCode::tab] = true;
		break;
	case 32://space
		keyState[KeyCode::space] = true;
		break;
	case 27://ESC
		keyState[KeyCode::Esc] = true;
		break;
	default:
		break;
	}
}
