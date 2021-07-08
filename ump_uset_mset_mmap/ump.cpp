#include "hashtable.h"
namespace BL {
	template <class Key, class Value, class HashFcn = MyHash, class ExtractKey = MyExtractKey>
	class Unordered_map {
	public:
		struct MapExtractKey {
			Key operator()(pair<Key, Value> item) { return item.first; }
		};
	public:
		typedef pair<Key, Value> ValTy;
		typedef HashTable<Key, ValTy, HashFcn, MapExtractKey> hashtable;
		typedef HashTable_iterator<Key, ValTy, HashFcn, MapExtractKey> iterator;
	public:
		Unordered_map() {}
		~Unordered_map() { }
	public:
		iterator Begin() { return _ht.begin(); }
		iterator End() { return _ht.end(); }
	public:
		bool Insert(const ValTy& v) { return _ht.insert_unique(v); }
		iterator Find(const ValTy& v) { return _ht.Find(v); }
		bool Erase(const ValTy& v) { return _ht.Erase(v); }
		size_t Size()const { return _ht.size(); }
		bool Empty()const { return _ht.size == 0; }
		void Clear() { return _ht.Clear(); }
		void Show() { return _ht.print_hash_table(); }
	private:
		hashtable _ht;
	};

	template <class Key, class Value=Key, class HashFcn = MyHash, class ExtractKey = MyExtractKey>
	class Unordered_set {
	public:
		struct SetExtractKey {
			Key operator()(Key k) { return k; }
		};
	public:
		typedef Value ValTy;
		typedef HashTable<Key, ValTy, HashFcn, SetExtractKey> hashtable;
		typedef HashTable_iterator <Key, ValTy, HashFcn, SetExtractKey> iterator;
	public:
		Unordered_set() {}
		~Unordered_set() {}
		iterator Begin() { return _h.begin(); }
		iterator End() { return _h.end(); }
		void Clear() { return _h.Clear(); }
		bool Insert(const ValTy& v) { return _h.insert_unique(v); }
		bool Erase(const ValTy& v) { return _h.Erase(v); }
		bool Empty() { return _h.size() == 0; }
		void Show() { 
			iterator it = Begin();
			while (it != End()) {
				cout << it.cur->val << " ";
				++it;
			}
			cout << endl;
		}
		size_t Size() { return _h.size(); }
		iterator Find(const ValTy& v) { return _h.Find(v); }
	private:
		hashtable _h;
	};
	
	template <class Key, class Value, class HashFcn = MyHash, class ExtractKey = MyExtractKey>
	class Multimap {
	public:
		struct MapExtractKey {
			Key operator()(pair<Key, Value> item) { return item.first; }
		};
	public:
		typedef pair<Key, Value> ValTy;
		typedef HashTable<Key, ValTy, HashFcn, MapExtractKey> hashtable;
		typedef HashTable_iterator<Key, ValTy, HashFcn, MapExtractKey> iterator;
	public:
		Multimap() {}
		~Multimap() { }
	public:
		iterator Begin() { return _ht.begin(); }
		iterator End() { return _ht.end(); }
	public:
		bool Insert(const ValTy& v) { return _ht.insert_equal(v); }
		iterator Find(const ValTy& v) { return _ht.Find(v); }
		bool Erase(const ValTy& v) { return _ht.Erase(v); }
		size_t Size()const { return _ht.size(); }
		bool Empty()const { return _ht.size == 0; }
		void Clear() { return _ht.Clear(); }
		void Show() { return _ht.print_hash_table(); }
	private:
		hashtable _ht;
	};

	template <class Key, class Value=Key, class HashFcn = MyHash, class ExtractKey = MyExtractKey>
	class Multiset {
	public:
		struct SetExtractKey {
			Key operator()(Key k) { return k; }
		};
	public:
		typedef Value ValTy;
		typedef HashTable<Key, ValTy, HashFcn, SetExtractKey> hashtable;
		typedef HashTable_iterator <Key, ValTy, HashFcn, SetExtractKey> iterator;
	public:
		Multiset() {}
		~Multiset() {}
		iterator Begin() { return _h.begin(); }
		iterator End() { return _h.end(); }
		void Clear() { return _h.Clear(); }
		bool Insert(const ValTy& v) { return _h.insert_unique(v); }
		bool Erase(const ValTy& v) { return _h.Erase(v); }
		bool Empty() { return _h.size() == 0; }
		void Show() {
			iterator it = Begin();
			while (it != End()) {
				cout << it.cur->val << " ";
				++it;
			}
			cout << endl;
		}
		size_t Size() { return _h.size(); }
		iterator Find(const ValTy& v) { return _h.Find(v); }
	private:
		hashtable _h;
	};
}
int main(){
	pair<int, int> v[] = { {1,10},{2,20},{3,30},{2,200} };
	BL::Unordered_map<int,int> mmp;
	mmp.Insert(v[0]);
	mmp.Insert(v[1]);
	mmp.Insert(v[2]);
	mmp.Insert(v[3]);
	bool x = mmp.Erase(pair<int,int>(1,2));
	mmp.Show();
	auto y = mmp.Find(v[3]);
	mmp.Clear();
	mmp.Show();
	return 0;
}

//int main() {
//	int v[] = { 1,10,2,20,3,30,2,200 };
//	BL::Unordered_set<int> uset;
//	uset.Insert(v[0]);
//	uset.Insert(v[1]);
//	uset.Insert(v[2]);
//	uset.Insert(v[3]);
//	uset.Insert(v[4]);
//	uset.Insert(v[5]);
//	uset.Insert(v[6]);
//	uset.Insert(v[7]);
//	uset.Insert(v[7]);
//	uset.Show();
//	bool x = uset.Erase(v[3]);
//	uset.Show();
//	auto y = uset.Find(v[3]);
//	uset.Clear();
//	uset.Show();
//	return 0;
//}

//int main(){
//	pair<int, int> v[] = { {1,10},{2,20},{3,30},{2,200} };
//	BL::Unordered_map<int, int> ump;
//	ump.Insert(v[0]);
//	ump.Insert(v[1]);
//	ump.Insert(v[2]);
//	ump.Insert(v[3]);
//	bool x = ump.Erase(pair<int,int>(1,2));
//	ump.Show();
//	auto y = ump.Find(v[3]);
//	ump.Clear();
//	ump.Show();
//	return 0;
//}





