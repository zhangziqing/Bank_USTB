#include "accumulator.h"
#include"date.h"
Accumulator::Accumulator(Date vDate, double dValue) :lastDate(vDate), value(dValue) ,sum(0.0){ }
void Accumulator::change(Date vDate, double dValue) {
	sum+= value * (vDate.distance(lastDate));
	lastDate = vDate;
	value = dValue;
}
void Accumulator::reset(Date vDate, double dValue) {
	sum = 0;
	lastDate = vDate;
	value = dValue;
}
double Accumulator::getSum(Date vDate) { 
	sum += value * (vDate.distance(lastDate));
	return sum;
}