#include <iostream>
#include <string.h>
#include <time.h>
#pragma warning(disable:4996)
using namespace std;

class String;

class String_cnt {
	friend class String;
public:
	String_cnt(const char* str = "") :m_cnt(0) {
		m_data = new char[strlen(str) + 1];
		strcpy(m_data, str);
	}
	String_cnt(const String_cnt& s) {
		m_data = s.m_data;
	}
	String_cnt& operator=(const String_cnt& s) {
		if (this != &s) //浅拷贝
			m_data = s.m_data;
		return *this;
	}
	~String_cnt() {
		delete[]m_data;
		m_data = nullptr;
	}
	void Increase() { ++m_cnt; }
	void Decrease() {
		--m_cnt;
		if (!m_cnt)
			delete this;
	}
	char* GetData() const { return m_data; }

private:
	char* m_data;
	int m_cnt;
};

class String {
	friend ostream& operator<<(ostream& out, const String& s);
public:
	explicit String(const char* str = "") :pn(new String_cnt(str)) {
		pn->Increase();//构造函数，串计数器+1
	}
	String(const String& s) :pn(s.pn) { pn->Increase(); }//拷贝构造，浅拷贝，新串计数器+1
	String& operator=(const String& s) {
		if (this != &s) {
			pn->Decrease();//原串计数器-1
			pn = s.pn;//浅拷贝指向新串
			pn->Increase();//新串计数器+1
		}
		return *this;
	}
	~String() { pn->Decrease(); }//析构函数，只需将串的计数器减1
	void ToUpper() {
		DepthCopy();//写时拷贝
		char* str = pn->m_data;//拿到深拷贝的串地址
		while (*str) {//小写转大写
			if (*str >= 'a' && *str <= 'z')
				*str -= 32;
			str++;
		}
	}
	void ToLower() {
		DepthCopy();//写时拷贝
		char* str = pn->m_data;//拿到深拷贝的串地址
		while (*str) {//大写转小写
			if (*str >= 'A' && *str <= 'Z')
				*str += 32;
			str++;
		}
	}	
	int Size() {
		int length = 0;
		char* p = pn->m_data;
		while (*p)
			++length, ++p;
		return length;
	}
private:
	void DepthCopy() {//深拷贝——仅供写实拷贝使用，私有方法
		String_cnt* tmp = new String_cnt(pn->m_data);
		pn->Decrease();//先将原串的计数器-1
		pn = tmp;//获得新串的地址
		pn->Increase();//新串计数器+1
	}
	String_cnt* pn;
};

ostream& operator<<(ostream& out, const String& s) {
	out << s.pn->GetData();
	return out;
}
int main()
{
	String s1("adutsdutfsogfsif");
	String s2("31231");
	cout << s1.Size() << endl;
	cout << s2.Size() << endl;

	return 0;
}
