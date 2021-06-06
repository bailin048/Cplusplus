#include "RBTree_set.h"

namespace BL{
	template <class K>
	class set {
		typedef K ValTy;
		struct KeyofVal {
			const K& operator()(const ValTy& key) {
				return key;
			}
		};
		typedef RBTree<K, ValTy, KeyofVal> RBTree;
	public:
		typedef typename RBTree::iterator iterator;
	public:
		set() {}
		iterator Begin() { return _t.begin(); }
		iterator End() { return _t.end(); }
		bool Empty() { return _t.Empty(); }
		size_t Size() { return _t.Size(); }
		iterator Find(const ValTy& key) { return _t.Find(key); }
		iterator Insert(const ValTy& data) { return _t.Insert(data); }
		bool Erase(const K& key) { return _t.Remove(key); }
		void Clear() { _t.Clear(); }
	private:
		RBTree _t;
	};
}

int main() {
	BL::set<int> s;
	s.Insert(1);
	s.Insert(2);
	s.Insert(3);
	s.Insert(4);
	s.Erase(4);
	//s.Clear();
	BL::set<int>::iterator it = s.Begin();
	while (it != s.End()) {
		cout << *it << " " << endl;
		++it;
	}
	return 0;
}