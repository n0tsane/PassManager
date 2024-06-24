#include<iostream>
#include<string>
#include<cstdlib>

#include"file_io.h"
#include"functions.h"

using namespace std;


void intro(void);


int main(void)
{
	intro();

	srand((unsigned int)time(NULL));

	choice_f();


	return 0;
}


void intro(void)
{
	cout << "Welcome to Password Manager!";

	cout << "\n\nPassManager is a tool for saving passwords for different websites and email ids, and retrieving them later.";

	cout << "\n\nYou have the following options: ";
	cout << "\n\t1. Generate password (Press 1)";
	cout << "\n\t2. Set master password (Press 2)";
	cout << "\n\t3. Change master password (Press 3)";
	cout << "\n\t4. Store your own password (Press 4)";
	cout << "\n\t5. View passwords (Press 5)";
	cout << "\n\t6. Delete all passwords (Press 6)";
	cout << "\n\t7. Exit (Press 7)";
}


void choice_f(void)
{
	cout << "\n\nChoose: ";
	unsigned int choice = 0;
	//cin >> choice;

	if (!(cin >> choice))
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "\n\n\t\t\tEnter the correct type of input  :(";
		choice_f();
	}
	else
	{
		switch (choice)
		{
		case 1:
			generator();
			choice_f();
			break;

		case 2:
			set_master();
			choice_f();
			break;

		case 3:
			change_master();
			choice_f();
			break;

		case 4:
			create_pass();
			choice_f();
			break;

		case 5:
			viewp();
			choice_f();
			break;

		case 6:
			delete_all();
			choice_f();
			break;

		case 7:
			cout << "Exiting...";
			cout << "\n\n" << "=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=       =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=";
			cout << "\n\n\n\n\n";
			return;

		default:
			cout << "Error! Wrong input. Try again.";
			choice_f();
		}
	}
	return;
}