#ifndef _USER_H__
#define _USER_H__
#include <string>
#include<vector>
#include<set>
#include"account.h"
class User {
	std::string name;
	std::string password;
	int count;
public:
	std::set<std::string> s;
	std::vector<SavingsAccount*> SavingAccount;
	std::vector<CreditAccount*> CreditsAccount;
	User(std::string szName = "new_user", std::string szPassword="123456") :name(szName),count(0){
		password = szPassword;
		for (int i = 0; i < password.size(); i++) {
			password[i] =(password[i]+name[(i % name.size())])%90+30;
		}
	}
	User(int n ,std::string szName = "new_user", std::string szPassword = "123456") :name(szName), count(0) {
		password = szPassword;
	}
	bool Log_in(std::string x);
	int border() { return SavingAccount.size(); }
	int Count() { return count; }
	std::string & getName() { return name; }
	std::string& getPassword() { return password; }
	void CreateSAccount(Date  vDate, std::string iId, double dRate);
	void CreateCAccount(Date vDate, std::string szId, double dCreidt, double dRate, double dFee);
	void Check();
	void Display();
	void Show();
	void Settle(Date& date);
};
#endif // !_USER_H__

