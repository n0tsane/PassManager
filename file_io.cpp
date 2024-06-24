#include<iostream>
#include<cstdlib>
#include<ctime>
#include<fstream>
#include<ctype.h>

#include"file_io.h"
#include"functions.h"

using namespace std;


void set_master(void)
{
	ifstream checkOpen;
	checkOpen.open("masterp.pwm");
	if (checkOpen)
	{
		cout << "\n\tMaster password already exists. To change it, use the change master password option.";
		checkOpen.close();
		return;
	}
	string mpass, mpass2;
	cout << "\n\tSet master password: ";
	cin >> mpass;
	if (check_pass_security(mpass) < 0)
	{
		cout << "\tPassword does not match the requirements. \n\tRe-enter password.";
		set_master();
		return;
	}
	else
	{
		cout << "\tRe-enter master password: ";
		cin >> mpass2;
		if (mpass.compare(mpass2) == 0)
		{
			fHeaderManip("masterp");

			int len = mpass.length();
			mpass.append(to_string(time(NULL)));
			if (len < 10)
			{
				mpass.append(to_string(0));
			}
			mpass.append(to_string(len));
			int key = rotation_key(time(NULL), mpass.length());
			if (key < 10)
			{
				mpass.append(to_string(0));
			}
			mpass.append(to_string(key));
			mpass = rotate(mpass, key);
			mpass = sEncode(mpass);

			ofstream mp;
			mp.open("masterp.pwm", ios_base::app);
			mp << "\n" << mpass;
			cout << "\tMaster password has been set.";
			mp.close();
		}
		else
		{
			cout << "\tPasswords do not match! Try again.";
			set_master();
		}
	}
}


void change_master(void)
{
	ifstream mpfile;
	mpfile.open("masterp.pwm");

	if (!mpfile)
	{
		cout << "\tNo master password currently exists. Set a master password first.";
	}
	else
	{
		string mpread, mpcheck;
		cout << "\n\tEnter current master password: ";
		cin >> mpcheck;
		mpfile.seekg(4, ios::beg);
		getline(mpfile, mpread);
		getline(mpfile, mpread);
		mpread = sDecode(mpread);
		int pass_len = stoi(mpread.substr(mpread.length() - 4, 2));
		int key = stoi(mpread.substr(mpread.length() - 2, 2));
		mpread = rotate(mpread, 26 - key);
		mpread = mpread.substr(0, pass_len);
		if (mpread.compare(mpcheck) == 0)
		{
			string newmp;
			cout << "\tEnter new master password: ";
			cin >> newmp;
			if (check_pass_security(newmp) < 0)
			{
				cout << "\tPassword does not match the requirements. \n\tRe-enter password.";
				mpfile.close();
				change_master();
				return;
			}
			else
			{
				string newmp2;
				cout << "\tRe-enter new master password: ";
				cin >> newmp2;
				if (newmp.compare(newmp2) == 0)
				{
					mpfile.close();
					remove("masterp.pwm");
					fHeaderManip("masterp");
					int len = newmp.length();
					newmp.append(to_string(time(NULL)));
					if (len < 10)
					{
						newmp.append(to_string(0));
					}
					newmp.append(to_string(len));
					int key = rotation_key(time(NULL), newmp.length());
					if (key < 10)
					{
						newmp.append(to_string(0));
					}
					newmp.append(to_string(key));
					newmp = rotate(newmp, key);
					newmp = sEncode(newmp);

					ofstream mp;
					mp.open("masterp.pwm", ios_base::app);
					mp << "\n" << newmp;
					mp.close();
					cout << "\tMaster password has been changed successfully!";
				}
				else
				{
					cout << "\tPasswords do not match! Retry.";
				}
			}
		}
		else
		{
			cout << "\tWrong master password! Try again.";
			choice_f();
		}
	}
}


void viewp(void)
{
	ifstream pf;
	pf.open("pss.pwm");

	if (!pf)
	{
		cout << "\n\tNo current password exists.";
		choice_f();
	}
	else          //password will be stored as: website + password + time + weblen(2 digits) + passlen(2 digits) + key
	{
		ifstream mp;
		mp.open("masterp.pwm");

		if (!mp)
		{
			cout << "\n\tYou have not set any master password. Set a master key first.";
			choice_f();
			return;
		}

		cout << "\n\tEnter master password: ";
		string masterpass;
		cin >> masterpass;

		string master_stored;
		mp.seekg(4, ios::beg);
		getline(mp, master_stored);
		getline(mp, master_stored);
		master_stored = sDecode(master_stored);
		int pass_len = stoi(master_stored.substr(master_stored.length() - 4, 2));
		int key = stoi(master_stored.substr(master_stored.length() - 2, 2));
		master_stored = rotate(master_stored, 26 - key);
		master_stored = master_stored.substr(0, pass_len);
		mp.close();

		if (master_stored.compare(masterpass) == 0)
		{
			pf.seekg(4, ios::beg);
			int record_count = 1;
			while (!pf.eof())
			{
				string pass;
				getline(pf, pass);
				if (record_count == 1)          //runs only for the first record and avoids the first newline
				{
					pass.erase();
					getline(pf, pass);
				}
				pass = sDecode(pass);
				string key = pass.substr(pass.length() - 2, 2);
				string wlen = pass.substr(pass.length() - 6, 2);
				string plen = pass.substr(pass.length() - 4, 2);
				int rKey = stoi(key);
				string rPass = rotate(pass.substr(0, pass.length() - 6), 26 - rKey);
				unsigned int weblen = stoi(wlen);
				unsigned int passlen = stoi(plen);
				string website = rPass.substr(0, weblen);
				string passwd = rPass.substr(weblen, passlen);     //logic error in this step

				cout << "\t" << record_count << ". The password for website " << website << " is " << passwd << endl;
				record_count++;
			}
		}
		else
		{
			cout << "\n\tWrong master password! Try again.";
		}

		pf.close();
	}

	//implement a while loop or a for loop for printing all available password records.

}


void generator(void)
{
	ifstream mpfile;
	mpfile.open("masterp.pwm");

	if (!mpfile)
	{
		cout << "\n\tYou have not set any master password. Set a master key first.";
		choice_f();
	}
	else
	{
		string mpcheck, mpread;
		mpfile.seekg(4, ios::beg);
		getline(mpfile, mpread);
		getline(mpfile, mpread);
		mpread = sDecode(mpread);
		int pass_len = stoi(mpread.substr(mpread.length() - 4, 2));
		int key = stoi(mpread.substr(mpread.length() - 2, 2));
		mpread = rotate(mpread, 26 - key);
		mpread = mpread.substr(0, pass_len);

		cout << "\n\tEnter master password: ";
		cin >> mpcheck;
		if (mpcheck.compare(mpread) == 0)
		{
			string pass, website, pTime;
			cout << "\n\tEnter the name of website (99 characters max): ";
			cin >> website;
			if (website.length() > 99)
			{
				website = website.substr(0, 99);
				cout << "\n\tWebsite length is too large. It will be truncated to the first 99 characters.";
			}

			int plen = rand() % 13;
			while (plen < 8)
			{
				plen = rand() % 13;
			}

			string randPass = rand_gen(plen);
			//cout << "\n" << randPass << endl;
			pTime = to_string(time(NULL));
			pass.append(website);
			pass.append(randPass);
			pass.append(pTime);

			if (website.length() < 10)
			{
				pass.append(to_string(0));
			}
			pass.append(to_string(website.length()));

			if (randPass.length() < 10)
			{
				pass.append(to_string(0));
			}
			pass.append(to_string(randPass.length()));

			int key = rotation_key(website.length(), randPass.length());
			pass = rotate(pass, key);
			if (key < 10)            //this process makes sure that the last two digits give the length of the key
			{
				pass.append(to_string(0));
			}
			pass.append(to_string(key));
			pass = sEncode(pass);
			//cout << "\n\t" << pass;

			ifstream pf;
			pf.open("pss.pwm");
			if (!pf)
			{
				fHeaderManip("pss");
			}
			else
			{
				pf.close();
			}

			ofstream pfile;
			pfile.open("pss.pwm", ios_base::app);
			pfile << "\n" << pass;
			cout << "\n\tPassword has been generated successfully!";
			pfile.close();
		}
		else
		{
			mpfile.close();

			cout << "\n\tWrong master password! Try again.";
			choice_f();
		}
	}
}


void delete_all(void)
{
	cout << "\n\tEnter master password: ";
	string mpass;
	cin >> mpass;

	ifstream readmp;
	readmp.open("masterp.pwm");
	if (!readmp)
	{
		cout << "\tMaster password does not exist.";
		return;
	}
	else
	{
		string mpcheck;
		readmp.seekg(4, ios::beg);
		getline(readmp, mpcheck);
		getline(readmp, mpcheck);
		mpcheck = sDecode(mpcheck);
		int pass_len = stoi(mpcheck.substr(mpcheck.length() - 4, 2));
		int key = stoi(mpcheck.substr(mpcheck.length() - 2, 2));
		mpcheck = rotate(mpcheck, 26 - key);
		mpcheck = mpcheck.substr(0, pass_len);
		if (mpass.compare(mpcheck) == 0)
		{
			remove("pss.pwm");
			cout << "\n\tAll password records have been deleted.";
		}
		else
		{
			cout << "\n\tWrong master password! Retry.";
		}
		readmp.close();
	}
}


void delete_p(void)
{
	cout << "\n\tEnter master password: ";
	string mpass;
	cin >> mpass;

	ifstream readmp;
	readmp.open("masterp.pwm");
	if (!readmp)
	{
		cout << "\tMaster password does not exist. Set a master password first.";
		return;
	}
	else
	{
		string mpcheck;
		readmp.seekg(4, ios::beg);
		getline(readmp, mpcheck);
		getline(readmp, mpcheck);
		mpcheck = sDecode(mpcheck);
		int pass_len = stoi(mpcheck.substr(mpcheck.length() - 4, 2));
		int key = stoi(mpcheck.substr(mpcheck.length() - 2, 2));
		mpcheck = rotate(mpcheck, 26 - key);
		mpcheck = mpcheck.substr(0, pass_len);

		if (mpass.compare(mpcheck) == 0)
		{
			ofstream npf;
			npf.open("npss.pwm");
			ifstream pf;
			pf.open("pss.pwm");
			int opt = delete_p_option();

			int i = 0;
			while (i < opt)
			{
				char ch;
				pf.get(ch);
				//newline check
			}




		}
		else
		{
			cout << "\n\tWrong master password! Retry.";
		}
	}



}


unsigned int delete_p_option(void)
{

}


void fHeaderManip(string fname)
{
	ofstream fh;
	if (fname == "masterp")
	{
		fh.open("masterp.pwm");
	}
	else if (fname == "pss")
	{
		fh.open("pss.pwm");
	}

	int ch = rand() % 3;
	if (ch == 0)
	{
		fh << char(255);
		fh << char(216);
		fh << char(255);
		fh << char(224);
	}
	else if (ch == 1)
	{
		fh << char(26);
		fh << char(69);
		fh << char(223);
		fh << char(163);
	}
	else
	{
		fh << char(95);
		fh << char(39);
		fh << char(168);
		fh << char(137);
	}
	fh.close();
}


void create_pass(void)
{
	ifstream mpfile;
	mpfile.open("masterp.pwm");

	if (!mpfile)
	{
		cout << "\n\tYou have not set any master password. Set a master key first.";
		choice_f();
	}
	else
	{
		string mpread, mpcheck;
		mpfile.seekg(4, ios::beg);
		getline(mpfile, mpread);
		getline(mpfile, mpread);
		mpread = sDecode(mpread);
		int pass_len = stoi(mpread.substr(mpread.length() - 4, 2));
		int key = stoi(mpread.substr(mpread.length() - 2, 2));
		mpread = rotate(mpread, 26 - key);
		mpread = mpread.substr(0, pass_len);
		cout << "\n\tEnter master password: ";
		cin >> mpcheck;
		if (mpcheck.compare(mpread) == 0)
		{
			string pass, website;
			cout << "\n\tEnter the name of website (99 characters max): ";
			cin >> website;
			if (website.length() > 99)
			{
				website = website.substr(0, 99);
				cout << "\n\tWebsite length is too large. It will be truncated to the first 99 characters.";
			}

			cout << "\n\tYour password must contain atleast one uppercase letter, one lowercase letter, one digit and one symbol.";
			cout << "\n\tPassword must also be atleast 8 characters long.";

			cout << "\n\tEnter your password: ";
			string uPass = receive_pass();
			int i = check_pass_security(uPass);
			while (i < 0)
			{
				cout << "\tYour password does not match the requirements. Re-enter password: ";
				uPass = receive_pass();
				i = check_pass_security(uPass);
			}
			cout << "\tRe-enter password: ";
			string uPass2 = receive_pass();
			while (uPass.compare(uPass2) != 0)
			{
				cout << "\tRe-enter the same password: ";
				uPass2 = receive_pass();
			}

			string pTime = to_string(time(NULL));
			pass.append(website);
			pass.append(uPass);
			pass.append(pTime);

			if (website.length() < 10)
			{
				pass.append(to_string(0));
			}
			pass.append(to_string(website.length()));

			if (uPass.length() < 10)
			{
				pass.append(to_string(0));
			}
			pass.append(to_string(uPass.length()));

			int key = rotation_key(website.length(), uPass.length());
			pass = rotate(pass, key);
			if (key < 10)
			{
				pass.append(to_string(0));
			}
			pass.append(to_string(key));
			pass = sEncode(pass);

			ifstream pf;
			pf.open("pss.pwm");
			if (!pf)
			{
				fHeaderManip("pss");
			}
			else
			{
				pf.close();
			}

			ofstream pfile;
			pfile.open("pss.pwm", ios_base::app);
			pfile << "\n";
			pfile << pass;
			cout << "\n\tPassword has been stored successfully!";
			pfile.close();
		}
		else
		{
			mpfile.close();

			cout << "Wrong master password! Try again.";
			choice_f();
		}
	}
}


string receive_pass(void)
{
	string passwd;
	cin >> passwd;

	return passwd;
}