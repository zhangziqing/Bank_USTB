#include "account.h"
#include "date.h"
#include"user.h"
#include<iostream>
#include<iomanip>
#include<cmath>

using std::cout;
///Account
///
///
Account::Account(std::string szId,Date vDate) :id(szId) ,balance(0){
	vDate.show();
	cout << "\t#" << szId << " created" << '\n';
}
std::multimap<Date, AccountRecord*> Account::recordMap;
void Account::record(Date iDate, double dAmount, std::string disc) {
	//accumulation = accumulate(iDate);
	//lastDate = iDate;
	balance += dAmount;
	iDate.show();
	cout << "\t#" << id << '\t' << std::left << std::setw(8) << dAmount << std::left << std::setw(8) << balance << disc << '\n';
	total += dAmount;
	AccountRecord *tmp=new AccountRecord(iDate, this, dAmount, balance, disc);
	recordMap.insert({iDate, tmp});
}
void Account::error(std::string szMsg) {
	cout << szMsg;
}
void Account::query(Date& date1, Date& date2,User *Nuser) {
	auto ilt = recordMap.begin();
	while (ilt->first <= date2&&ilt != recordMap.end()) {
		if(Nuser->s.find(ilt->second->belong()->getId())!= Nuser->s.end())
		if (ilt->first >= date1) {
			ilt->second->display();
		}
		ilt++;
	}
}
void Account::query(Date& date1,User* Nuser) {
	int y, m, d;
	y = date1.getYear() - (date1.getMonth() <= 3);
	m = date1.getMonth() <= 3 ? date1.getMonth() + 12 - 3 : date1.getMonth() - 3;
	d = date1.getDay();
	Date date(y, m, d);
	for(auto a:recordMap) {
		auto x = Nuser->s.find(a.second->belong()->getId());
		if (x == Nuser->s.end())continue;
		if (a.first >= date&&a.first<=date1) {
			a.second->display();
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///SavingsAccount
///
///
SavingsAccount::SavingsAccount(Date  vDate, std::string iId, double dRate) :rate(dRate), Account(iId,vDate), acc(vDate, 0) {
	
}
double Account::total = 0;
void SavingsAccount::deposit(Date iDate, double dAmount, std::string disc) {
	double value = acc.getValue();
	acc.change(iDate, value+dAmount);
	record(iDate, dAmount, disc);
}

void SavingsAccount::withdraw(Date iDate, double dAmount, std::string disc) {
	double value = acc.getValue();
	if (dAmount > getBalance()) {
		AccoutException r("你没有那么多钱，醒醒",this);
		throw r;
	}
	acc.change(iDate, value - dAmount);
	record(iDate, -dAmount, disc);
}


void  SavingsAccount::settle(Date iDate) {
	if (!iDate.IsSASettleDay())return;
	double dAmount = (acc.getSum(iDate) * rate) / (365 + acc.getDate().isLeapYear());
	dAmount = floor(dAmount * 100 + 0.5) / 100;
	record(iDate, dAmount, "interest");
	acc.reset(iDate, getBalance());
}
const void SavingsAccount::show() {
	cout << getId() << "       " << "Balance: " <<getBalance();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///CreditAccount
///
///
CreditAccount::CreditAccount(Date vDate, std::string szId, double dCreidt, double dRate, double dFee) :
	credit(dCreidt), rate(dRate), fee(dFee), Account(szId,vDate), acc(vDate, 0){
}
const void CreditAccount::show() {
	cout << getId() << "       " << "Balance: " << getBalance()<<'\t'<<"Available Credit: "<<getAvailableCredit();
}
void CreditAccount::withdraw(Date vDate, double dAmount, std::string disc) {
	double value = acc.getValue();
	double x = getAvailableCredit();
	if (dAmount > x) {
		AccoutException e("你没有辣么多额度可以用", this);
		throw e;
	}
	record(vDate, -dAmount, disc);
	acc.change(vDate, getBalance());
}
void CreditAccount::deposit(Date vDate, double dAmount, std::string disc) {
	double value = acc.getValue();
	record(vDate, dAmount, disc);
	acc.change(vDate, getBalance());
}
void CreditAccount::settle(Date iDate) {
	if (!iDate.IsCASettleDay())return;
	else {
		if (iDate.IsSASettleDay()&&getBalance()==0) {
			double dAmount = -50;
			record(iDate, dAmount, "annual fee");
			acc.change(iDate, getBalance());
		}
		else if(getBalance() != 0&&(iDate.IsSASettleDay())){
			double dAmount = (acc.getSum(iDate) * rate) / (365 + acc.getDate().isLeapYear()) -50;
			dAmount = floor(dAmount * 100 + 0.5) / 100;
			record(iDate, dAmount, "annual fee & interest");
			acc.change(iDate, getBalance()<0?getBalance():0);
		}
		else {
			double dAmount =(acc.getSum(iDate) * rate) /(365 + acc.getDate().isLeapYear());
			dAmount = floor(dAmount * 100 + 0.5) / 100;
			record(iDate, dAmount, "interest");
			acc.change(iDate, getBalance() < 0 ? getBalance() : 0);
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///AccountRecord
///
///
AccountRecord::AccountRecord(Date vDate, Account* pAccount, double dAmount, double dBalance, std::string szDesc) :
	date(vDate),account(pAccount),amount(dAmount),balance(dBalance),desc(szDesc){}
void AccountRecord::display() {
	date.show();
	cout << "\t#" << account->getId() << '\t'<<amount<<'\t'<<balance<<'\t'<<desc<<std::endl;
}


