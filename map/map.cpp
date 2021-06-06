#include "RBTree_map.h"

namespace BL {
	template <class K, class V>
	class map {
		typedef pair<K, V> ValTy;
		struct KeyofValue {
			const K& operator()(const ValTy& v) { return v.first; }
		};
		typedef RBTree<K, ValTy, KeyofValue> RBTree;
	public:
		typedef typename RBTree::iterator iterator;
	public:
		map() {}
		iterator Begin() { return _t.begin(); }
		iterator End() { return _t.end(); }
		size_t Size()const { return _t.Size(); }
		bool Empty()const { return _t.Empty(); }
		V& operator[](const K& key) {
			return (*_t.Insert(ValTy(key, V()))).second;
		}
		bool Erase(const K& key) { return _t.Remove(key); }
		iterator Insert(const ValTy& data) { return _t.Insert(data); }
		void Clear() { _t.Clear(); }
		iterator Find(const K& key) { return _t.Find(key); }
	private:
		RBTree _t;
	};
}

int main() {
	BL::map<int, int> mp;
	pair<int, int> s = make_pair(1, 10);
	mp.Insert(s);
	pair<int, int> s2 = make_pair(2, 20);
	mp.Insert(s2);
	pair<int, int> s3 = make_pair(4, 40);
	mp.Insert(s3);
	pair<int, int> s4 = make_pair(7, 70);
	mp.Insert(s4);
	pair<int, int> s5 = make_pair(3, 30);
	mp.Insert(s5);
	pair<int, int> s6 = make_pair(10, 100);
	auto y = *mp.Insert(s6);
	++mp[2];
	//mp.Clear();
	BL::map<int, int>::iterator it1 = mp.Begin();
	BL::map<int, int>::iterator it2 = mp.End();
	for (; it1 != mp.End(); ++it1) 
		cout << "key:" << (*it1).first << " " << "val:" << (*it1).second << endl;
	return 0;
}