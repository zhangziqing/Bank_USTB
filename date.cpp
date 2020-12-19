#include "date.h"
#include<iostream>
#include<cstdio>
#include<stdexcept>

Date::Date(int y, int m, int d) :year(y), month(m), day(d) {
	totalDays = days();
	if (m > 12 || d > getMaxDay()) {
		std::runtime_error r("Invalid Day\n");
		throw r;
	}
}
const int Date::getMaxDay() {
	int a[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if (month != 2)return  a[month - 1];
	else return a[1] + isLeapYear();
}
const int Date::isLeapYear() {
	int& y = year;
	if (((y % 4 == 0) && y % 100) || y % 400 == 0)
		return 1;
	else
		return 0;
}
const void Date::show() {
	printf("%d-%d-%d  ", year, month, day);
}
const int Date::days() {
	int& y = year;
	int& m = month;
	int& d1 = day;
	int d[] = { 0,31,59,90,120,151,181,212,243,273,304,334,365 };
	return 365 * (y - 1) + (y - 1) / 4 - (y - 1) / 100 + (y - 1) / 400 + d1 - 1 + (m > 2 ? isLeapYear() + d[m - 1] : d[m - 1]);
}
const int Date::distance(Date date) {
	return this->days() - date.days();
}
Date& Date::read() {
	int a, b, c;
	std::cin >> a >> b >> c;
	Date tmp(a, b, c);
	return tmp;
}
bool Date::operator <( const Date& date)const {
	return this->totalDays < date.totalDays;
}bool Date::operator <=(const Date& date)const {
	return this->totalDays <= date.totalDays;
}
bool Date::operator >=(const Date& date)const {
	return this->totalDays >= date.totalDays;
}