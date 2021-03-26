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
		Increase();
	}
	String_cnt(const String_cnt& s) {
		delete[]m_data;
		m_data = new char[strlen(s.m_data) + 1];
		Increase();
	}
	String_cnt& operator=(const String_cnt& s) {
		if (this != &s) 
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
		pn->Increase();
	}
	String(const String& s) :pn(s.pn) { pn->Increase(); }
	String& operator=(const String& s) {
		if (this != &s) {
			pn->Decrease();
			pn = s.pn;
			pn->Increase();
		}
		return *this;
	}
	~String() { pn->Decrease(); }
	void ToUpper() {
		String_cnt* tmp = new String_cnt(pn->m_data);
		pn->Decrease();
		pn = tmp;
		pn->Increase();
		char* str = pn->m_data;
		while (*str) {
			if (*str >= 'a' && *str <= 'z')
				*str -= 32;
			str++;
		}
	}
	void ToLower() {
		String_cnt* tmp = new String_cnt(pn->m_data);
		pn->Decrease();
		pn = tmp;
		pn->Increase();
		char* str = pn->m_data;
		while (*str) {
			if (*str >= 'A' && *str <= 'Z')
				*str += 32;
			str++;
		}
	}
private:
	String_cnt* pn;
};

ostream& operator<<(ostream& out, const String& s) {
	out << s.pn->GetData();
	return out;
}
int main()
{
	String s1("adutsdutfsogfsif");
	String s2(s1);
	cout << s1 << endl;
	String s3;
	s3 = s1;
	s3.ToUpper();
	cout << s1 << endl;
	cout << s2 << endl;
	cout << s3 << endl;
	s3.ToLower();
	cout << s1 << endl;
	cout << s3 << endl;
	return 0;
}
