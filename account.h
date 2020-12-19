#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_
#include<string>
#include<map>
#include<stdexcept>
#include"date.h"
#include"accumulator.h"
extern class User;
class AccountRecord;
class Account {
	std::string id;
	double balance;
	static double total;
	static std::multimap<Date, AccountRecord*> recordMap;
protected:
	Account( std::string szId,Date vDate);
	void record(Date vDate, double dAmount, std::string disc);
	void error(std::string szMsg);
public:
	virtual void deposit(Date, double, std::string)=0;
	virtual void withdraw(Date, double, std::string)=0;
	virtual void settle(Date) = 0;
	std::string getId() const{ return id; }
	const double getBalance()const { return balance; }
	static double getTotal() { return total; }
	virtual const void show() = 0;
	static void query(Date& date1, Date& date2, User* Nuser);
	static void query(Date& date1, User* Nuser);
};
class SavingsAccount :public Account{
	double rate;
	Accumulator acc;
public:
	SavingsAccount(Date, std::string, double);
	void deposit(Date, double, std::string);
	void withdraw(Date, double, std::string);
	void settle(Date);
	const void show();
};
class CreditAccount :public Account{
	Accumulator acc;
	double credit;
	double rate;
	double fee;
	const double getDebt();
public:
	const double getCredit() const{ return credit; }
	const double getRate() const{ return rate; }
	const double getFee() const{ return fee; }
	const double getAvailableCredit() { return credit + getBalance(); };
	void deposit(Date, double, std::string);
	void withdraw(Date, double, std::string);
	void settle(Date);
	const void show();
	CreditAccount(Date vDate, std::string szId, double dCreidt, double dRate, double dFee);
};
class AccountRecord {
	Date date;
	const Account* account;
	double amount;
	double balance;
	std::string desc;
public :
	AccountRecord()=default;
	AccountRecord(Date, Account*, double, double, std::string);
	void display();
	const Account* belong() const{ return account; }
};
class AccoutException:public std::runtime_error{
	Account* p;
public:
	AccoutException(std::string err, Account* x) :std::runtime_error(err), p(x) {}
	std::string Err() {
		return p->getId();
	}
};
#endif