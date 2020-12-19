#ifndef _ACCUMULATOR_H_
#define _ACCUMULATOR_H_

#include <string>
#include"date.h"
class Accumulator {
	Date lastDate;
	double value;
	double sum;
public:
	Accumulator(Date iDate, double dValue);
	double getSum (Date idate);
	double getValue() const{ return value; }
	void change(Date iDate, double value);
	void reset(Date iDate, double value);
	Date getDate() { return lastDate; }
};

#endif // !_ACCUMULATOR_H_

