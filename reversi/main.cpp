#include"Manager.h"
#include<iostream>
using namespace std;
int main()
{
	Manager::singleton = new Manager();
	Manager::singleton->start();
}