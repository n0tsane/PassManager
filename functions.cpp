#include<iostream>
#include<cstdlib>

#include"functions.h"

using namespace std;


string rand_gen(int plen)
{
	string ucase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string lcase = "abcdefghijklmnopqrstuvwxyz";
	string digits = "0123456789";
	string sym = "@#$^&*+";

	string pass = "";

	for (int i = 0; i < plen; i++)
	{
		switch ((rand() % 4))
		{
		case 0:
			pass += lcase.at(randNumGen() % 26);
			break;

		case 1:
			pass += digits.at(randNumGen() % 10);
			break;

		case 2:
			pass += ucase.at(randNumGen() % 26);
			break;

		case 3:
			pass += sym.at(randNumGen() % 7);
			break;
		}
	}

	if (check_pass_security(pass) < 0)
	{
		pass.erase();
		pass = rand_gen(plen);
	}

	return pass;
}


int randNumGen(void)
{
	int rand_a = rand();
	int rand_b = rand();
	int rand_c = rand();

	int num = (rand_a * rand_b) - rand_c;
	num = num % 100;

	return num;
}


int check_pass_security(string pass)
{
	int check = 0;
	if (pass.length() < 8)
	{
		check = -1;
	}

	int ucase_count = 0, lcase_count = 0, digit_count = 0, sym_count = 0;
	for (int j = 0; j < pass.length(); j++)
	{
		//check for "characters"
		int r = toascii(pass.at(j));
		if (r >= 65 && r <= 90)
		{
			ucase_count++;
		}
		else if (r >= 97 && r <= 122)
		{
			lcase_count++;
		}
		else if (r >= 48 && r <= 57)
		{
			digit_count++;
		}
		else if (r == 35 || r == 36 || r == 38 || r == 64 || r == 94)
		{
			sym_count++;
		}
	}

	if (ucase_count == 0 || lcase_count == 0 || digit_count == 0 || sym_count == 0)
	{
		check--;
	}

	return check;
}


int rotation_key(int wl, int pl)
{
	int rkey = 0;
	int count = 0;
	while (rkey == 0)
	{
		rkey = ((wl + count) * (pl + count)) % 26;
		count++;
	}

	return rkey;
}


string rotate(string pass, int key)    // For rotation, use original key. For reversing, use 26-key.
{
	string sPass;
	for (int i = 0; i < pass.length(); i++)
	{
		int r = toascii(pass.at(i));
		if (r >= 65 && r <= 90)
		{
			r = r - 64;
			r = (r + key) % 26;
			if (r == 0)     // ensures that r can be the 26th letter
			{
				r += 26;
			}
			r += 64;
			sPass.push_back(char(r));
		}
		else if (r >= 97 && r <= 122)
		{
			r = r - 96;
			r = (r + key) % 26;
			if (r == 0)      // ensures that r can be the 26th letter
			{
				r += 26;
			}
			r += 96;
			//sPass.at(i) = char(r);   //exception
			sPass.append(1, (char)r);
		}
		else
		{
			sPass.append(1, pass.at(i));
		}
	}

	return sPass;
}


string sEncode(string pass)
{
	string ePass;
	unsigned int count = 0;

	for (int i = 0; i < pass.length(); i++)
	{
		ePass.append(1, pass.at(i));
		ePass.append(1, char(140 + count));
		count = (count + 1) % 16;
	}

	return ePass;
}


string sDecode(string pass)
{
	string dPass;

	for (int i = 0; i < pass.length(); i += 2)
	{
		dPass.append(1, pass.at(i));
	}

	return dPass;
}