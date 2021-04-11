#include <iostream>
#include <vld.h>
#include <assert.h>
#pragma warning(disable:4996)
using namespace std;
namespace BL {
	class string
	{
		friend ostream& operator<<(ostream& _cout, const BL::string& s);
		friend istream& operator>>(istream& _cin, BL::string& s);
	public:
		typedef char* iterator;
		static size_t npos;
	public:
		//Creat&&Destroy
		string(const char* str = "") {
			if (!str) {
				assert(false);
				return;
			}
			_size = strlen(str);
			_capacity = _size;
			_str = new char[strlen(str) + 1];
			strcpy(_str, str);

		}
		string(const string& s) :_str(nullptr) {
			string tmp(s._str);
			swap(tmp);
		}
		string& operator=(const string& s) {
			if (this != &s) {
				string tmp(s._str);
				swap(tmp);
			}
			return *this;
		}
		~string() {
			if (_str) {
				delete[] _str;
				_str = nullptr;
			}
		}
		//iterator
		iterator begin() { return _str; }
		iterator end() { return _str + _size; }
		iterator begin()const { return _str; }
		iterator end()const { return _str + _size; }
		//modify
		void push_back(char c);
		string& operator+=(char c);
		void append(const char* str);
		string& operator+=(const char* str);
		void clear() {
			_size = 0;
			_str[_size] = '\0';
		}
		void swap(string& s) {
			char* tmp = s._str;
			s._str = _str;
			_str = tmp;

			size_t tmp1 = s._size;
			s._size = _size;
			_size = tmp1;

			size_t tmp2 = s._capacity;
			s._capacity = _capacity;
			_capacity = tmp2;
		}
		const char* c_str()const { return _str; }
		//capacity
		size_t size()const { return _size; }
		size_t capacity()const { return _capacity; }
		bool empty()const { return _size == 0; }
		void resize(size_t n, char c = '\0') {
			if (n > size()) {
				if (n > capacity()) 
					reserve(n);
				memset(_str + _size, c, n - _size);
				_str[n] = '\0';
			}
		}
		void reserve(size_t n) {
			if (n > _capacity) {
				char* str = new char[n + 1];
				strcpy(str, _str);
				delete[] _str;
				_str = str;
				_capacity = n;
			}
		}
		// access
		char& operator[](size_t index) {
			assert(index < _size);
			return _str[index];
		}
		const char& operator[](size_t index)const {
			assert(index < _size);
			return _str[index];
		}
		//relational operators
		bool operator<(const string& s) {
			char* s1 = _str;
			char* s2 = s._str;
			if (!(*s1))
				return true;
			if (!(*s2))
				return false;
			while (*s1 && *s2) {
				if (*s1 < *s2)
					return true;
				else if (*s1 == *s2)
					++s1, ++s2;
				else
					return false;
			}
			return false;
		}
		bool operator<=(const string& s) { return (*this < s) || (*this == s); }
		bool operator>(const string& s) { return !(*this <= s); }
		bool operator>=(const string& s) { return !(*this < s); }
		bool operator==(const string& s) {
			if (_size == s._size) {
				char* s1 = _str;
				char* s2 = s._str;
				while (*s1) {
					if (*s1 != *s2)
						return false;
					++s1, ++s2;
				}
				return true;
			}
			return false;
		}
		bool operator!=(const string& s) { return !(*this == s); }
	public:
		// 返回c在string中第一次出现的位置
		size_t find(char c, size_t pos = 0) const {
			assert(pos > -1 && pos < _size);
			pos == npos ? 0 : pos;
			for (int i = pos; i < _size; ++i) 
				if (c == _str[i]) 
					return i;
			return BL::string::npos;
		}
		// 返回子串s在string中第一次出现的位置
		size_t find(const char* s, size_t pos = 0) const {
			int i = pos, j = 0;
			int sz = strlen(s);
			while (i < _size && j < sz) {
				if (_str[i] == s[j])
					++i, ++j;
				else
					i = i - j + 1, j = 0;
			}
			if (j == sz)
				return i;
			return BL::string::npos;
		}
		// 在pos位置上插入字符c/字符串str，并返回该字符的位置
		string& insert(size_t pos, char c) {
			assert(pos >= 0 && pos <= _size);
			if (_size == _capacity) {//需要扩容
				size_t new_sz = !capacity() ? 1 : 2 * capacity();
				reserve(new_sz);
			}
			for (int i = _size + 1; i >pos; --i)
				_str[i] = _str[i-1];
			_str[pos] = c;
			++_size;
			return *this;
		}
		string& insert(size_t pos, const char* str) {
			assert(pos >= 0 && pos <= _size);
			size_t sz = strlen(str);
			if (_size == _capacity) {//需要扩容
				size_t new_sz = !capacity() ? sz: sz + _size;
				reserve(new_sz);
			}
			for (int i = _size + sz; i > pos; --i)
				_str[i] = _str[i - sz];
			int j = 0;
			for (int i = pos; j <sz; ++i,++j)
				_str[i] = str[j];
			_size += sz;
			return *this;
		}
		// 删除pos位置上的元素，并返回该元素的下一个位置
		string& erase(size_t pos, size_t len) {
			assert(pos >= 0 && pos < _size);
			len = pos + len > _size ? _size - pos : len;
			for (int i = pos; i < _size-len; ++i)
				if (i + len <= _size)
					_str[i] = _str[i + len];
			_size-=len;
			return *this;
		}
	private:
		char* _str;
		size_t _capacity;
		size_t _size;
	};
	size_t BL::string::npos = -1;
	ostream& operator<<(ostream& _cout, const BL::string& s) {
		size_t sz = s.size();
		for (size_t i = 0; i < sz; ++i)
			_cout << s[i];
		return _cout;
	}
	istream& operator>>(istream& _cin, BL::string& s) {
		size_t sz = s.size();
		for (size_t i = 0; i < sz; ++i)
			_cin >> s[i];
		return _cin;
	}
}
using namespace BL;
void TestCreat() {
	BL::string s1;
	cout << s1 << endl;
	BL::string s2("dasdasd");
	cout << s2 << endl;
	BL::string s3(s2);
	cout << s3 << endl;
	BL::string s4 = s1;
	cout << s4 << endl;
	s4 = s2;
	cout << s4 << endl;
}
void TestCompare() {
	BL::string s1("dafdasd");
	BL::string s2("dasdasd");
	cout << "s1==s2:" << (s1 == s2) << endl;
	cout << "s1!=s2:" << (s1 != s2) << endl;
	BL::string s3("a");
	BL::string s4("dasdasd");
	cout << "s3<s4:" << (s3 < s4) << endl;
	cout << "s3>=s4:" << (s3 >= s4) << endl;
	BL::string s5("w");
	BL::string s6("dasdasd");
	cout << "s5>s6:" << (s5 > s6) << endl;
	cout << "s5<=s6:" << (s5 <= s6) << endl;
}
void TestInsertAndErase() {
	BL::string s1;
	s1.insert(0, 'a');//从无到有
	cout << s1 << endl;
	s1.insert(0, 'b');//头插
	cout << s1 << endl;
	s1.insert(1, 'r');//中间插
	cout << s1 << endl;
	s1.insert(3, 't');//尾插
	cout << s1 << endl;

	BL::string s2;
	s2.insert(0, "aa");//从无到有
	cout << s2 << endl;
	s2.insert(0, "bbbbb");//头插
	cout << s2 << endl;
	s2.insert(1, "rrrr");//中间插
	cout << s2 << endl;
	s2.insert(11, "trere");//尾插
	cout << s2 << endl;

	s2.erase(0,3);//头删
	cout << s2 << endl;
	s2.erase(6, 2);//中删
	cout << s2 << endl;
	s2.erase(s2.size()-1,4);//尾删
	cout << s2 << endl;
}
int main() {
	TestCreat();
	TestCompare();
	TestInsertAndErase();
	return 0;
}