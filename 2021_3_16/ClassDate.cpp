#include <iostream>
#include <time.h>
using namespace std;

bool IsLeapYear(int year);
namespace Days {//申请13个的目的：月份=索引
	int days[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31 };
};

using namespace Days;
class Date
{
	friend bool IsLeapYear(int year);
public:
	int GetMonthDay(int year, int month) {
		bool  flag = IsLeapYear(year);
		return flag? month==2?days[month]+1:days[month]:days[month];//闰年2月+1天
	}
	void DateShow() {
		cout << _year << "年" << _month << "月" << _day << "日" << endl;
	}
	Date(int year = 1900, int month = 1, int day = 1) {	//全缺省默认构造函数
		_year = year;
		_month = month;
		_day = day;
	}
	Date(const Date &t) {// 拷贝构造函数
		_year = t._year;
		_month = t._month;
		_day = t._day;
	}
	Date& operator=(const Date& d) {//赋值运算符重载
		if (this != &d) {
			_year = d._year;
			_month = d._month;
			_day = d._day;
		}
		return *this;
	}
	~Date() {}
	Date& operator+=(int day) {
		*this = *this + day;
		return *this;
	}
	Date operator+(int day) {
		Date p(*this);
		if (day < 0) {
			p -= -day;
			return p;
		}
		p._day += day;
		while (p._day > GetMonthDay(p._year, p._month)) {
			p._day -= GetMonthDay(p._year, p._month);
			++p._month;
			if (p._month == 13) {
				p._month = 1;
				++p._year;
			}
		}
		return p;
	}
	Date operator-(int day) {
		Date p(*this);//新对象
		if (day < 0) {
			p += -day;
			return p;
		}
		p._day -= day;
		int month = 0;
		while (p._day <= 0) {
			month = p._month == 1 ? 12 : p._month - 1;//月份为1则减到上一年12月
			p._day += GetMonthDay(p._year, month);//加上上一月天数
			--p._month;//月份倒退
			if (p._month == 0) {//年份倒退
				p._month = 12;
				--p._year;
			}
		}
		return p;
	}
	Date& operator-=(int day) {
		*this = *this - day;
		return *this;
	}
	Date& operator++() {
		*this = *this + 1;
		return *this;
	}
	Date operator++(int) {
		Date tmp(*this);
		*this += 1;
		return tmp;
	}
	Date operator--(int) {
		Date tmp(*this);
		*this -= 1;
		return tmp;
	}
	Date& operator--() {
		*this -= 1;
		return *this;
	}
	bool operator>(const Date& d) {
		if (d._year > (*this)._year)
			return false;
		if (d._year < (*this)._year)
			return true;
		if (d._month > (*this)._month)//到达此处，说明年份一样
			return false;
		if (d._month < (*this)._month)
			return true;
		if (d._day > (*this)._day)//达到此处，说明月份一样
			return false;
		if (d._day < (*this)._day)
			return true;
		return true;
	}
	bool operator==(const Date& d) {
		return d._year == (*this)._year &&
			d._month == (*this)._month &&
			d._day == (*this)._day;
	}
	inline bool operator >= (const Date& d) {
		return *this > d || *this == d;
	}
	bool operator < (const Date& d) {
		return !(*this >= d);//<的互斥函数为>=
	}
	bool operator <= (const Date& d) {
		return !(*this > d);//<= 的互斥事件为>
	}
	bool operator != (const Date& d) {
		return !((*this) == d);//!= 的互斥事件为 ==
	}
	int operator-(const Date& d) {
		Date Min;
		Date Max;
		*this < d ? Max = d, Min = *this : Max = *this, Min = d;
		int res = 0;
		while (Min < Max) 
			++res, ++Min;
		return res;
	}
private:
	int _year;
	int _month;
	int _day;
};
bool IsLeapYear(int year) {
	if (year % 400 == 0 && year % 100 != 0)//世纪闰年
		return true;
	if (year % 4 == 0)//普通闰年
		return true;
	return false;
}
int main()
{
	Date d1(2021, 1, 31);
	Date d2;
	d2 = d1 + 31;
	d2.DateShow();
	return 0;
}