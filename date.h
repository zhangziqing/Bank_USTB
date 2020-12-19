#ifndef _DATE_H_
#define _DATE_H_

class Date {
	int year;
	int month;
	int day;
	int totalDays;
	const int mouthDays();
public:
	Date() = default;
	Date(int y, int m, int d);
	static Date& read();
	const int getYear() { return year; }
	const int getMonth() { return month; }
	const int getDay() { return day; }
	const int getMaxDay();
	const int isLeapYear();
	const void show();
	const int distance(Date date);
	const int days();
	bool IsCASettleDay() { return  day == 1; }
	bool IsSASettleDay() {return month == 1 && day == 1;}
	bool operator <(const Date& date)const;
	bool operator <=(const Date& date)const;
	bool operator >=(const Date& date)const;
};

#endif	
