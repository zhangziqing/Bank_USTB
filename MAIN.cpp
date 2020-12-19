#include "account.h"

#include <iostream>

#include <vector>

#include <algorithm>

#include<fstream>

#include<windows.h>

#include<map>

#include "user.h"
using namespace std;
ofstream out("commands.txt", ofstream::app);
struct deleter {
	template <class T> void operator () (T* p) { delete p; }
};

inline void Display(User* Nuser) {
	Nuser->Show();
	cout << "\n��ѡ����Ҫ�������˻���������";
}
void Check(User *Nuser,Date &date){
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD w1 = 0xf9;
	WORD w2 = 0x07;
	COORD w = { 20,2 };
	CONSOLE_SCREEN_BUFFER_INFO  store_p;
	COORD date_p = { 30,4 };
	COORD store_PP;
	system("cls");
	SetConsoleCursorPosition(hOut, w);
	SetConsoleTextAttribute(hOut, w1);
	std::cout << "\t��亵�����ϵͳ\n";
	SetConsoleTextAttribute(hOut, w2);
	cout << "\n��ǰ�û�Ϊ:" <<Nuser->getName();
	SetConsoleCursorPosition(hOut, date_p);
	cout << "��ǰ���ڣ�";
	date.show();
	SetConsoleTextAttribute(hOut, w1);
	cout << "\n----------------------------------------------------\n";
	SetConsoleTextAttribute(hOut, w2);
	Nuser->Check();
	SetConsoleTextAttribute(hOut, w1);
	cout << "----------------------------------------------------\n";
	SetConsoleTextAttribute(hOut, w2);
	system("pause");
}

void pause() {
	int cd;
	cin >> cd;
}
void Settle(map<string, User*>& users,Date& date) {
	for (auto& a : users) {
		a.second->Settle(date);
	}
}
void read(char &cmd,User*& Nuser, Date& date, map<string, User*>& users,ifstream &in) {
	decltype(users.begin()) ilt;
	string name, password;
	in >> cmd;
	switch (cmd) {
	case 'a':
		in >> name;
		ilt = users.find(name);
		if (ilt == users.end()) {

		}
		else {
			Nuser = ilt->second;
		}
		break;
	case 'b':
		in >> name>>password;
		Nuser = new User(1,name, password);
		users.insert(make_pair(name, Nuser));
		//out << cmd << ' ' << name << " " << password;
		break;
	case 'e':
		return;
		break;
	default:
		cout << "Wrong Command!!\n";
		break;
	}
	char type;
	string id, desc;
	double rate, credit, fee, amount;
	Account* account;
	int day;
	int index;
	while(!in.eof()) {
		in >> cmd;
		switch (cmd) {
		case 'a'://�����˻�
			in >> type;

			if (type == 's') {
				in >> id;
				in >> rate;
				Nuser->CreateSAccount(date, id, rate);
			}
			else if (type == 'c') {
				in >> id;
				in >> credit >> rate >> fee;
				Nuser->CreateCAccount(date, id, credit, rate, fee);
			}
			break;
		case 'd':
			in >> index;
			in >> amount>>desc;
			if (index < Nuser->border()) {
				Nuser->SavingAccount[index]->deposit(date, amount, desc);
			}
			else {
				index -= Nuser->border();
				Nuser->CreditsAccount[index]->deposit(date, amount, desc);
			}
			break;

		case 'w'://ȡ���ֽ�
			in >> index;
			if (index > Nuser->Count() - 1) {
				throw runtime_error("�Ƿ�����\n");
				break;
			}
			in >> amount >> desc;
			if (index < Nuser->border()) {
				Nuser->SavingAccount[index]->withdraw(date, amount, desc);
			}
			else {
				index -= Nuser->border();
				Nuser->CreditsAccount[index]->withdraw(date, amount, desc);
			}
			break;
		case 'c'://�ı�����
			in >> day;
			if (day < date.getDay())
			{
				runtime_error r("You cannot specify a previous day");
				throw r;
			}
			else {
				date = Date(date.getYear(), date.getMonth(), day);
			}
		case 'n'://�����¸���
			if (date.getMonth() == 12)
				date = Date(date.getYear() + 1, 1, 1);
			else
				date = Date(date.getYear(), date.getMonth() + 1, 1);
			Settle(users, date);
			break;
		case 'q'://��ѯһ��ʱ���ڵ���Ŀ
			Account::query(date, Nuser);
			break;
		case 'l':
			system("cls");
			return;
			break;
		case 'e':
			Nuser = nullptr;
			return;
		default:
			runtime_error r("Wrong Command!!\n");
			throw r;
			break;
		}
	}
}
bool Inital(char& cmd,User* &Nuser, map<string ,User *> &users,Date &date) {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD w1 = 0xf9;
	WORD w2 = 0x07;
	COORD w = { 20,2 };
	CONSOLE_SCREEN_BUFFER_INFO  store_p;
	decltype(users.begin()) ilt;
	string name, password;
	Nuser = nullptr;
	system("cls");
	SetConsoleCursorPosition(hOut, w);
	SetConsoleTextAttribute(hOut, w1);
	cout << "��ӭ���볬亵�����ϵͳ\n";
	SetConsoleTextAttribute(hOut, w2);
	cout << "\n\n\n��ѡ����Ҫ���еĲ���\n\n";
	SetConsoleTextAttribute(hOut, w1);
	cout << "---------------------------------------------------------------\n\n";
	SetConsoleTextAttribute(hOut, w2);
	cout << ">a ��¼��sign in��\t>b ע�� (sign up)\t>e �˳�(exit)\n";
	SetConsoleTextAttribute(hOut, w1);
	cout << "\n---------------------------------------------------------------\n";
	SetConsoleTextAttribute(hOut, w2);
	std::cin >> cmd;
	system("cls");
	SetConsoleCursorPosition(hOut, w);
	SetConsoleTextAttribute(hOut, w1);
	cout << "��ӭ���볬亵�����ϵͳ\n";
	switch (cmd) {
	case 'a':
		cout << "\n\n�û���¼                                            \n";
		SetConsoleTextAttribute(hOut, w2);
		cout << "----------------------------------------------------\n";
		SetConsoleTextAttribute(hOut, w1);
		cout << "\n�û���: ";
		SetConsoleTextAttribute(hOut, w2);
		cin >> name;
		ilt = users.find(name);
		SetConsoleTextAttribute(hOut, w2);
		if (ilt == users.end()) {
			cout << "�˻������ڣ�3s���˻ص���ʼ����";
			Sleep(3000);
			return 0;
		}
		else {
			SetConsoleTextAttribute(hOut, w1);
			cout << "\n���룺";
			SetConsoleTextAttribute(hOut, w2);
			int n = 0;
			while (cin >> password) {
				if (ilt->second->Log_in(password)) {
					Nuser = (ilt->second);
					cout << "\n��¼�ɹ�!3s������û�����";
					Sleep(3000);
					out << cmd << ' ' << name<<'\n';
					Check(Nuser, date);
					return 1;
				}
				else {
					cout << "\n���������������\n";
					if (n++ > 3) {
						cout << "���ǲ�������ţ��������ˣ�3s����˵���ʼ����";
						Sleep(3000);
						return 0;
					}
				}
			}
		}
		break;
	case 'b':
		cout << "\n\n�û�ע��                                            \n";
		SetConsoleTextAttribute(hOut, w2);
		cout << "----------------------------------------------------\n";
		SetConsoleTextAttribute(hOut, w1);
		cout << "\n�û���: ";
		SetConsoleTextAttribute(hOut, w2);
		cin >> name;
		SetConsoleTextAttribute(hOut, w1);
		cout << "\n���룺";
		SetConsoleTextAttribute(hOut, w2);
		cin >> password;
		cout << "\nע��ɹ�!3s������û�����";
		Sleep(3000);
		Nuser = new User(name, password);
		users.insert(make_pair(name, Nuser));
		out << cmd << ' ' << name << " " <<Nuser->getPassword()<<'\n';
		return true;
		break;
	case 'e':
		return 1;
		break;
	default:
		cout << "Wrong Command!!\n";
		
			break;
	}
}
void Opreation(char &cmd, User* &Nuser, Date &date, map<string, User*>& users) {
	char type;
	string id, desc;
	string name = Nuser->getName();
	double rate, credit, fee, amount;
	Account* account;
	int day;
	int index;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD w1 = 0xf9;
	WORD w2 = 0x07;
	COORD w = { 20,2 };
	CONSOLE_SCREEN_BUFFER_INFO  store_p;
	COORD date_p = { 30,4 };
	COORD store_PP;
	system("cls");
	SetConsoleCursorPosition(hOut, w);
	SetConsoleTextAttribute(hOut, w1);
	std::cout << "\t��亵�����ϵͳ\n";
	SetConsoleTextAttribute(hOut, w2);
	cout << "\n��ǰ�û�Ϊ:" << name;
	SetConsoleCursorPosition(hOut, date_p);
	cout << "��ǰ���ڣ�";
	date.show();
	SetConsoleTextAttribute(hOut, w1);
	cout << "\n----------------------------------------------------\n";
	SetConsoleTextAttribute(hOut, w2);
	cout << "(a) add account \n(d) deposit\n(w) withdraw\n(s) show\n(c) change day\n(n) next month\n(q) query\n(e) exit\n(l) log out\n" ;
	SetConsoleTextAttribute(hOut, w1);
	cout << "----------------------------------------------------\n";
	SetConsoleTextAttribute(hOut, w2);
	cin >> cmd;
	switch (cmd) {
	case 'a'://�����˻�
		SetConsoleTextAttribute(hOut, w1);
		cout << "����˻� \n>s �����˻� >c ���ÿ��˻�:\n";
		SetConsoleTextAttribute(hOut, w2);
		cin >> type;

		if (type == 's') {
			cout << "�������˻�ID: ";
			cin >> id;
			cout << "�������˻�����: ";
			cin >> rate;
			if (rate > 0.5) {
				runtime_error r("\n����Ϊ��˭��������ô����������Ҫ������ѽ�����ʲ�����0.5\n\n");
				throw r;
				break;
			}
			 
			out << cmd<<' '<< type << ' ' << id<< ' ' << rate;
			Nuser->CreateSAccount(date, id, rate);
			cout << "�����ɹ�,������һ�����\n";
			//accounts.push_back(account);
		}

		else if (type == 'c') {
			cout << "�������˻�ID: ";
			cin >> id;
			cout << "���������������˻���ȣ����ʣ���ѣ�\n";
			cin >> credit >> rate >> fee;
			if (rate > 0.5) {
				runtime_error r("\n����Ϊ��˭��������ô����������Ҫ������ѽ�����ʲ�����0.5\n\n");
				throw r;
				break;
			}
			if (credit > 20000) {
				runtime_error r("\n����Ϊ��˭��,�����г���û��ô���ȣ���Ȳ�����20000\n\n");
				throw r;
				break;
			}
			if (fee < 200) {
				runtime_error r("\n����Ϊ��˭��������ô����������Ҫ�Ʋ���ѽ��Ҫǡ�������Ѳ�������200\n\n");
				throw r;
				break;
			}

			Nuser->CreateCAccount(date, id, credit, rate, fee);
			out << cmd << ' ' << type << ' ' << id << ' ' << credit << ' ' << rate << ' ' << fee<<'\n';
			cout << "�����ɹ�,������һ�����\n";
			//accounts.push_back(account);
		}
		else
		{
			runtime_error r("Wrong Command!\n����Ҫ���ָ����������̸�������д\n");
			throw r;
			break;
		}
		out << '\n';
		break;
	case 'd'://�����ֽ�
		//out << ' ' << index << ' ' << amount;
		Display(Nuser);
		cin >> index;
		if (index > Nuser->Count() - 1) {
			runtime_error r("�Ƿ�����\n");
			throw r;
			break;
		}
		cout << "������: ";
		cin >> amount;
		cout << "������;��";
		cin >> desc;
		if (index < Nuser->border()) {
			Nuser->SavingAccount[index]->deposit(date, amount, desc);
		}
		else {
			index -= Nuser->border();
			Nuser->CreditsAccount[index]->deposit(date, amount, desc);
		}
		out <<cmd<<" "<<index<<' '<<amount<<' '<<desc<< '\n';
		break;

	case 'w'://ȡ���ֽ�
		Display(Nuser);
		cin >> index;
		if (index > Nuser->Count() - 1) {
			throw runtime_error("�Ƿ�����\n");
			break;
		}
		cout << "����ȡ��: ";
		cin >> amount;
		cout << "������;��";
		cin >> desc;
		if (index < Nuser->border()) {
			Nuser->SavingAccount[index]->withdraw(date, amount, desc);
		}
		else {
			index -= Nuser->border();
			Nuser->CreditsAccount[index]->withdraw(date, amount, desc);
		}
		out <<cmd<<' '<<index<<' '<<amount<<' '<<desc<< '\n';
		break;
	case 's'://��ѯ���˻���Ϣ
		Nuser->Display();
		break;
	case 'c'://�ı�����
		//out << cmd;
		cin >> day;
		//out << day << ' ';
		if (day < date.getDay())
		{
			runtime_error r("You cannot specify a previous day");
			throw r;
		}
		else {
				date = Date(date.getYear(), date.getMonth(), day);
		}
		GetConsoleScreenBufferInfo(hOut, &store_p);
		store_PP = store_p.dwCursorPosition;
		SetConsoleCursorPosition(hOut, date_p);
		cout << "��ǰ���ڣ�";
		date.show();
		SetConsoleCursorPosition(hOut, store_PP);
		cout << "�����ɹ�,������һ�����\n";
		out << cmd << ' ' << day << '\n';
		Sleep(1000);
		//out << '\n';
		break;
	case 'n'://�����¸���
		//out << cmd;
		if (date.getMonth() == 12)
			date = Date(date.getYear() + 1, 1, 1);
		else
			date = Date(date.getYear(), date.getMonth() + 1, 1);
		GetConsoleScreenBufferInfo(hOut, &store_p);
		store_PP = store_p.dwCursorPosition;
		SetConsoleCursorPosition(hOut, date_p);
		cout << "��ǰ���ڣ�";
		date.show();
		SetConsoleCursorPosition(hOut, store_PP);
		Settle(users,date);
		//for (vector<Account*>::iterator iter = accounts.begin(); iter != accounts.end(); ++iter)
		//	(*iter)->settle(date);
		out << cmd <<'\n';
		cout << "�����ɹ�,������һ�����\n";
		break;
	case 'q'://��ѯһ��ʱ���ڵ���Ŀ
		//Date date1, date2;
		//date1 = Date::read();
		//date2 = Date::read();
		Account::query(date,Nuser);
		break;
	case 'l':
		system("cls");
		Nuser = nullptr;
		out << 'l'<<'\n';
		return;
		//goto INITAL;
		break;
	case 'e':
		return;
	default:
		runtime_error r("Wrong Command!!\n");
		throw r;
		break;
	}
	cout << '\n';
	system("pause");
}
int main() {
	Date date(2008, 1, 1);
	std::map<string, User*> users;
	User* Nuser;
	Nuser = nullptr;
	ifstream in("commands.txt");
	decltype(users.begin()) ilt;
	char cmd;
	while(!in.eof())read(cmd, Nuser, date, users,in);
	out << '\n';
	SetConsoleTitleA("����ϵͳ");
	do {
		while (!Inital(cmd,Nuser,users,date));
		if (cmd == 'e')break;
		do {
			try {
				Opreation(cmd,Nuser,date,users);
			}
			catch (AccoutException & e) {
				cerr << "�쳣�˻���"<<e.Err()<<'\n'<<e.what();
				system("pause");
			}
			catch(runtime_error& e){
				cerr << e.what()<<endl;
				system("pause");
			}
		} while (cmd != 'l'&&cmd!='e');
	}while (cmd!='e');
	out << '\n' << 'e' << '\n';
	out.close();
	cout << "\n\n\n�ڴ������´�ʹ��"; 
	Sleep(3000);
}