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
	cout << "\n请选择需要操作的账户的索引：";
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
	std::cout << "\t超浜德银行系统\n";
	SetConsoleTextAttribute(hOut, w2);
	cout << "\n当前用户为:" <<Nuser->getName();
	SetConsoleCursorPosition(hOut, date_p);
	cout << "当前日期：";
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
		case 'a'://增加账户
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

		case 'w'://取出现金
			in >> index;
			if (index > Nuser->Count() - 1) {
				throw runtime_error("非法索引\n");
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
		case 'c'://改变日期
			in >> day;
			if (day < date.getDay())
			{
				runtime_error r("You cannot specify a previous day");
				throw r;
			}
			else {
				date = Date(date.getYear(), date.getMonth(), day);
			}
		case 'n'://进入下个月
			if (date.getMonth() == 12)
				date = Date(date.getYear() + 1, 1, 1);
			else
				date = Date(date.getYear(), date.getMonth() + 1, 1);
			Settle(users, date);
			break;
		case 'q'://查询一段时间内的账目
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
	cout << "欢迎进入超浜德银行系统\n";
	SetConsoleTextAttribute(hOut, w2);
	cout << "\n\n\n请选择您要进行的操作\n\n";
	SetConsoleTextAttribute(hOut, w1);
	cout << "---------------------------------------------------------------\n\n";
	SetConsoleTextAttribute(hOut, w2);
	cout << ">a 登录（sign in）\t>b 注册 (sign up)\t>e 退出(exit)\n";
	SetConsoleTextAttribute(hOut, w1);
	cout << "\n---------------------------------------------------------------\n";
	SetConsoleTextAttribute(hOut, w2);
	std::cin >> cmd;
	system("cls");
	SetConsoleCursorPosition(hOut, w);
	SetConsoleTextAttribute(hOut, w1);
	cout << "欢迎进入超浜德银行系统\n";
	switch (cmd) {
	case 'a':
		cout << "\n\n用户登录                                            \n";
		SetConsoleTextAttribute(hOut, w2);
		cout << "----------------------------------------------------\n";
		SetConsoleTextAttribute(hOut, w1);
		cout << "\n用户名: ";
		SetConsoleTextAttribute(hOut, w2);
		cin >> name;
		ilt = users.find(name);
		SetConsoleTextAttribute(hOut, w2);
		if (ilt == users.end()) {
			cout << "账户不存在，3s后退回到初始界面";
			Sleep(3000);
			return 0;
		}
		else {
			SetConsoleTextAttribute(hOut, w1);
			cout << "\n密码：";
			SetConsoleTextAttribute(hOut, w2);
			int n = 0;
			while (cin >> password) {
				if (ilt->second->Log_in(password)) {
					Nuser = (ilt->second);
					cout << "\n登录成功!3s后进入用户界面";
					Sleep(3000);
					out << cmd << ' ' << name<<'\n';
					Check(Nuser, date);
					return 1;
				}
				else {
					cout << "\n密码错误！重新输入\n";
					if (n++ > 3) {
						cout << "你是不是想盗号？不许输了，3s后回退到初始界面";
						Sleep(3000);
						return 0;
					}
				}
			}
		}
		break;
	case 'b':
		cout << "\n\n用户注册                                            \n";
		SetConsoleTextAttribute(hOut, w2);
		cout << "----------------------------------------------------\n";
		SetConsoleTextAttribute(hOut, w1);
		cout << "\n用户名: ";
		SetConsoleTextAttribute(hOut, w2);
		cin >> name;
		SetConsoleTextAttribute(hOut, w1);
		cout << "\n密码：";
		SetConsoleTextAttribute(hOut, w2);
		cin >> password;
		cout << "\n注册成功!3s后进入用户界面";
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
	std::cout << "\t超浜德银行系统\n";
	SetConsoleTextAttribute(hOut, w2);
	cout << "\n当前用户为:" << name;
	SetConsoleCursorPosition(hOut, date_p);
	cout << "当前日期：";
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
	case 'a'://增加账户
		SetConsoleTextAttribute(hOut, w1);
		cout << "添加账户 \n>s 储蓄账户 >c 信用卡账户:\n";
		SetConsoleTextAttribute(hOut, w2);
		cin >> type;

		if (type == 's') {
			cout << "请输入账户ID: ";
			cin >> id;
			cout << "请输入账户利率: ";
			cin >> rate;
			if (rate > 0.5) {
				runtime_error r("\n你以为你谁啊，你这么搞我们银行要暴富的呀，利率不许超过0.5\n\n");
				throw r;
				break;
			}
			 
			out << cmd<<' '<< type << ' ' << id<< ' ' << rate;
			Nuser->CreateSAccount(date, id, rate);
			cout << "操作成功,进行下一项操作\n";
			//accounts.push_back(account);
		}

		else if (type == 'c') {
			cout << "请输入账户ID: ";
			cin >> id;
			cout << "请输入依次输入账户额度，利率，年费：\n";
			cin >> credit >> rate >> fee;
			if (rate > 0.5) {
				runtime_error r("\n你以为你谁啊，你这么搞我们银行要暴富的呀，利率不许超过0.5\n\n");
				throw r;
				break;
			}
			if (credit > 20000) {
				runtime_error r("\n你以为你谁啊,我们行长都没这么大额度，额度不许超过20000\n\n");
				throw r;
				break;
			}
			if (fee < 200) {
				runtime_error r("\n你以为你谁啊，你这么搞我们银行要破产的呀，要恰饭的嘛，年费不得少于200\n\n");
				throw r;
				break;
			}

			Nuser->CreateCAccount(date, id, credit, rate, fee);
			out << cmd << ' ' << type << ' ' << id << ' ' << credit << ' ' << rate << ' ' << fee<<'\n';
			cout << "操作成功,进行下一项操作\n";
			//accounts.push_back(account);
		}
		else
		{
			runtime_error r("Wrong Command!\n还想要别的指令？？？来键盘给你你来写\n");
			throw r;
			break;
		}
		out << '\n';
		break;
	case 'd'://存入现金
		//out << ' ' << index << ' ' << amount;
		Display(Nuser);
		cin >> index;
		if (index > Nuser->Count() - 1) {
			runtime_error r("非法索引\n");
			throw r;
			break;
		}
		cout << "输入存款: ";
		cin >> amount;
		cout << "输入用途：";
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

	case 'w'://取出现金
		Display(Nuser);
		cin >> index;
		if (index > Nuser->Count() - 1) {
			throw runtime_error("非法索引\n");
			break;
		}
		cout << "输入取款: ";
		cin >> amount;
		cout << "输入用途：";
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
	case 's'://查询各账户信息
		Nuser->Display();
		break;
	case 'c'://改变日期
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
		cout << "当前日期：";
		date.show();
		SetConsoleCursorPosition(hOut, store_PP);
		cout << "操作成功,进行下一项操作\n";
		out << cmd << ' ' << day << '\n';
		Sleep(1000);
		//out << '\n';
		break;
	case 'n'://进入下个月
		//out << cmd;
		if (date.getMonth() == 12)
			date = Date(date.getYear() + 1, 1, 1);
		else
			date = Date(date.getYear(), date.getMonth() + 1, 1);
		GetConsoleScreenBufferInfo(hOut, &store_p);
		store_PP = store_p.dwCursorPosition;
		SetConsoleCursorPosition(hOut, date_p);
		cout << "当前日期：";
		date.show();
		SetConsoleCursorPosition(hOut, store_PP);
		Settle(users,date);
		//for (vector<Account*>::iterator iter = accounts.begin(); iter != accounts.end(); ++iter)
		//	(*iter)->settle(date);
		out << cmd <<'\n';
		cout << "操作成功,进行下一项操作\n";
		break;
	case 'q'://查询一段时间内的账目
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
	SetConsoleTitleA("银行系统");
	do {
		while (!Inital(cmd,Nuser,users,date));
		if (cmd == 'e')break;
		do {
			try {
				Opreation(cmd,Nuser,date,users);
			}
			catch (AccoutException & e) {
				cerr << "异常账户："<<e.Err()<<'\n'<<e.what();
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
	cout << "\n\n\n期待您的下次使用"; 
	Sleep(3000);
}