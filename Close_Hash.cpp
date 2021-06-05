// ע�⣺����ʵ�ֵĹ�ϣ����Ԫ��Ψһ����key��ͬ��Ԫ�ز��ٽ��в���
// Ϊ��ʵ�ּ򵥣��˹�ϣ�������ǽ��Ƚ�ֱ����Ԫ�ذ���һ��
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

namespace Close_Hash{
	enum State { EMPTY=0, EXIST, DELETE };
	template<class K, class V>
	class HashTable{
		struct Elem{//�������ݳ�Ա
			pair<K, V> _val;
			State _state;//״̬
		};
	public:
		HashTable(size_t capacity = 8): _ht(capacity), _size(0), _totalSize(0){
			for (size_t i = 0; i < capacity; ++i)
				_ht[i]._state = EMPTY;
		}
		// ����
		bool Insert(const pair<K, V>& val) {
			size_t hashaddr = HashFunc(val.first);
			//���ײ�ռ��Ƿ����
			CheckCapacity();
			while (_ht[hashaddr]._state != EMPTY) {
				if (_ht[hashaddr]._state == EXIST &&
					_ht[hashaddr]._val.first == val.first) {
					return false;//ͬһ��ֵ��������β���
				}
				++hashaddr;//����̽��
				if (hashaddr == _ht.capacity())//�ҵ�β�������ײ���ʼ��
					hashaddr = 0;
			}
			//����Ԫ��
			_ht[hashaddr]._state = EXIST;
			_ht[hashaddr]._val = val;
			++_size;
			++_totalSize;
			return true;
		}
		// ����
		size_t Find(const K& key) {
			size_t hashaddr = HashFunc(key);
			while (_ht[hashaddr]._state != EMPTY) {
				if (_ht[hashaddr]._state == EXIST &&
					_ht[hashaddr]._val.first == key)
					return hashaddr;
				//����̽��
				++hashaddr;
				if (hashaddr == _ht.capacity())
					hashaddr = 0;
			}
			return -1;
		}
		// ɾ��
		bool Erase(const K& key) {
			size_t hashaddr = Find(key);
			if (hashaddr != -1) {
				_ht[hashaddr]._state = DELETE;
				--_size;
				return true;
			}
			return false;
		}
		size_t Size()const		{
			return _size;
		}
		bool Empty() const{
			return _size == 0;
		}
		void Swap(HashTable<K, V>& ht)		{
			swap(_size, ht._size);
			swap(_totalSize, ht._totalSize);
			_ht.swap(ht._ht);
		}
		void Show() {
			int n = _ht.capacity();
			for (size_t i = 0; i < n; ++i)
				cout << "[" << i << "]:" << _ht[i]._val.second << " :" << _ht[i]._state << endl;
		}
	private:
		size_t HashFunc(const K& key){
			return key % _ht.capacity();
		}
		void CheckCapacity() {
			if (_totalSize  >= _ht.capacity() * 0.7) {
				HashTable<K, V> newHT(GetNextPrime(_ht.capacity()));
				for (size_t i = 0; i < _ht.capacity(); ++i) {
					if (_ht[i]._state == EXIST)
						newHT.Insert(_ht[i]._val);
				}
				this->Swap(newHT);
			}
		}
	private:
		unsigned long GetNextPrime(const unsigned long& sz) {
			static const unsigned long _prime_list[] =
			{
			  53,         97,           193,         389,       769,
			  1543,       3079,         6151,        12289,     24593,
			  49157,      98317,        196613,      393241,    786433,
			  1572869,    3145739,      6291469,     12582917,  25165843,
			  50331653,   100663319,    201326611,   402653189, 805306457,
			  1610612741, 3221225473ul, 4294967291ul
			};
			static int list_index = 0;
			return _prime_list[list_index++];
		}
	private:
		vector<Elem> _ht;
		size_t _size;
		size_t _totalSize;  // ��ϣ���е�����Ԫ�أ���Ч����ɾ��, ����ʱ��Ҫ�õ�
	};
}
void TestInsertAndRemove() {
	Close_Hash::HashTable<int, int> ht(8);
	ht.Insert(make_pair(1, 10));
	ht.Insert(make_pair(2, 20));
	ht.Insert(make_pair(3, 30));
	ht.Insert(make_pair(4, 40));
	ht.Insert(make_pair(5, 50));
	ht.Insert(make_pair(6, 60));
	ht.Insert(make_pair(7, 70));
	ht.Insert(make_pair(9, 70));
	ht.Insert(make_pair(10, 80));
	ht.Erase(1);
	ht.Erase(2);
	ht.Erase(3);
	ht.Erase(4);
	ht.Erase(5);
	ht.Erase(6);
	ht.Erase(7);
	ht.Erase(9);
}
int main() {
	TestInsertAndRemove();
	return 0;
}
