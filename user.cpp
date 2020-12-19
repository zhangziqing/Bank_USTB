#include"user.h"
#include<string>
#include<iostream>

bool User::Log_in(std::string x) {
	for (int i = 0; i < x.size(); i++) {
		x[i] = (x[i] + name[(i % name.size())]) % 90 + 30;
	}
	return x == password;
}
void User::CreateSAccount(Date  vDate, std::string iId, double dRate) {
	SavingsAccount* tmp = new SavingsAccount(vDate, iId, dRate);
	SavingAccount.push_back(tmp);
	s.insert(iId);
	count++;
}
void User::CreateCAccount(Date vDate, std::string szId, double dCreidt, double dRate, double dFee) {
	CreditAccount* tmp = new CreditAccount(vDate, szId,dCreidt, dRate,dFee);
	CreditsAccount.push_back(tmp);
	s.insert(szId);
	count++;
}
void User::Check() {
	bool tag = 0;
	std::cout << "您的欠款情况为：\n";
	for (CreditAccount* a : CreditsAccount) {
		if (a->getBalance() < 0) {
			tag = 1;
			a->show();
			std::cout << std::endl;
		}
	}
	if (!tag)std::cout << "您暂时没有欠款\n";
}
void User::Settle(Date &date) {
	for (auto a : SavingAccount) {
		a->settle(date);
		std::cout << std::endl;
	}
	for (auto a : CreditsAccount) {
		a->settle(date);
		std::cout << std::endl;
	}
}
void User::Show() {
	int i;

	for (i = 0; i < SavingAccount.size(); i++) {
		std::cout << "[" << i << "]" << "  " << SavingAccount[i]->getId() << std::endl;
	}
	for (i = 0; i < CreditsAccount.size(); i++) {
		std::cout << "[" << i + SavingAccount.size() << "]" << "  " << CreditsAccount[i]->getId() << std::endl;
	}
}
void User::Display() {
	for (auto a : SavingAccount) {
		a->show();
		std::cout << std::endl;
	}
	for (auto a : CreditsAccount) {
		a->show();
		std::cout << std::endl;
	}
}