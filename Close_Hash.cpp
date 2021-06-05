// 注意：假如实现的哈希表中元素唯一，即key相同的元素不再进行插入
// 为了实现简单，此哈希表中我们将比较直接与元素绑定在一起
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

namespace Close_Hash{
	enum State { EMPTY=0, EXIST, DELETE };
	template<class K, class V>
	class HashTable{
		struct Elem{//表内数据成员
			pair<K, V> _val;
			State _state;//状态
		};
	public:
		HashTable(size_t capacity = 8): _ht(capacity), _size(0), _totalSize(0){
			for (size_t i = 0; i < capacity; ++i)
				_ht[i]._state = EMPTY;
		}
		// 插入
		bool Insert(const pair<K, V>& val) {
			size_t hashaddr = HashFunc(val.first);
			//检测底层空间是否充足
			CheckCapacity();
			while (_ht[hashaddr]._state != EMPTY) {
				if (_ht[hashaddr]._state == EXIST &&
					_ht[hashaddr]._val.first == val.first) {
					return false;//同一键值不允许被多次插入
				}
				++hashaddr;//线性探测
				if (hashaddr == _ht.capacity())//找到尾部，从首部开始找
					hashaddr = 0;
			}
			//插入元素
			_ht[hashaddr]._state = EXIST;
			_ht[hashaddr]._val = val;
			++_size;
			++_totalSize;
			return true;
		}
		// 查找
		size_t Find(const K& key) {
			size_t hashaddr = HashFunc(key);
			while (_ht[hashaddr]._state != EMPTY) {
				if (_ht[hashaddr]._state == EXIST &&
					_ht[hashaddr]._val.first == key)
					return hashaddr;
				//线性探测
				++hashaddr;
				if (hashaddr == _ht.capacity())
					hashaddr = 0;
			}
			return -1;
		}
		// 删除
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
		size_t _totalSize;  // 哈希表中的所有元素：有效和已删除, 扩容时候要用到
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
