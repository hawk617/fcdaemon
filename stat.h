#ifndef STAT_H
#define STAT_H

template <typename T> T Calman (T new_val, double mass)
{
	static T val=* (new T);
	val=(val+new_val*mass)/(1+mass);
	return val;
}


#endif