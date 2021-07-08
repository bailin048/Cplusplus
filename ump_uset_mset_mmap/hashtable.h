#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <vld.h>
using namespace std;

template <class Value>
struct HashTable_node {//哈希表节点
	Value val;
	HashTable_node* next;
};

struct MyHash {
	int operator()(int key) { return key; }
};

struct MyExtractKey {
	int operator()(int key) { return key; }
};

//质数表
static const int __stl_num_primes = 28;
static const unsigned long __stl_prime_list[__stl_num_primes] ={
  53,         97,           193,         389,       769,
  1543,       3079,         6151,        12289,     24593,
  49157,      98317,        196613,      393241,    786433,
  1572869,    3145739,      6291469,     12582917,  25165843,
  50331653,   100663319,    201326611,   402653189, 805306457,
  1610612741, 3221225473ul, 4294967291ul
};
//获取下一个质数
inline unsigned long __stl_next_prime(unsigned long n){
	const unsigned long* first = __stl_prime_list;
	const unsigned long* last = __stl_prime_list + __stl_num_primes;

	while (first != last){
		if (*first >= n)
			break;
		++first;
	}
	return first == last ? *(last - 1) : *first;
}

template <class Key, class Value, class HashFcn, class ExtractKey>
class HashTable; 
	
template <class Key, class Value, class HashFcn, class ExtractKey>
class HashTable_iterator {
public:
	typedef HashTable_iterator<Key, Value, HashFcn, ExtractKey> self;
	typedef HashTable_node<Value> node;
	typedef HashTable<Key, Value, HashFcn, ExtractKey> hashtable;
public:
	HashTable_iterator(node* n, hashtable* tab) :cur(n), ht(tab) {}
public:
	Value& operator*() { return cur->val; }
	Value* operator->() { return &(cur->val); }
	self& operator++() {
		const node* old = cur;
		cur = cur->next;
		if (!cur) {
			size_t bucket = ht->bkt_num(old->val);
			while (!cur && ++bucket < ht->buckets.size())
				cur = ht->buckets[bucket];
		}
		return *this;
	}
	bool operator!=(const self& s) { return cur != s.cur; }
	bool operator==(const self& s) { return cur == s.cur; }
public:
	node* cur;
	hashtable* ht;
};

template <class Key, class Value, class HashFcn = MyHash, class ExtractKey = MyExtractKey>
class HashTable {
	friend class HashTable_iterator<Key, Value, HashFcn, ExtractKey>;
public:
	typedef HashTable_iterator<Key, Value, HashFcn, ExtractKey> iterator;
public:
	typedef Key keyTy;
	typedef Value valTy;
	typedef HashFcn hasher;
	typedef size_t size_type;
public:
	typedef HashTable_node<Value> node;
public:
	HashTable(size_t n = 53):hash(HashFcn()),get_key(ExtractKey()),num_elements(0){
		initialize_buckets(n);
	}
public:
	size_type bucket(const keyTy& k)const {
		return k % buckets.size();//根据键值获取桶号
	}
	size_type bucket_count()const {
		return buckets.size();//桶的总数
	}
	size_type bucket_size(size_type bucket)const {
		size_type cnt = 0;
		for (node* cur = buckets[bucket]; cur; cur = cur->next)
			++cnt;
		return cnt;//统计第bucket号桶中的元素数目
	}
	size_type size()const {
		return num_elements;//hashtable里总元素数目
	}
	bool insert_equal(const valTy& obj) {
		resize(num_elements + 1);//检查容量
		return insert_equal_noresize(obj);
	}
	bool insert_unique(const valTy& obj) {
		resize(num_elements + 1);
		return insert_unique_noresize(obj);
	}
	void print_hash_table() {
		for (int i = 0; i < buckets.size(); ++i) {
			cout << i << " : ";
			node* first = buckets[i];
			while (first) {
				cout << (first->val).second << "-->";
				first = first->next;
			}
			cout << "Nil." << endl;
		}
	}
	iterator Find(const valTy& obj) {
		const size_type n = bkt_num(obj);
		node* first = buckets[n];
		for (node* cur = first; cur; cur = cur->next) {
			if (cur->val == obj)
				return iterator(cur, this);
		}
		return iterator(nullptr, this);
	}
	bool Erase(const valTy& obj) {
		const size_type n = bkt_num(obj);
		node* par = nullptr;
		node* first = buckets[n];
		if (first->val == obj) {//第一个节点就是obj
			buckets[n] = first->next;
			delete first;
			--num_elements;
			return true;
		}
		for (node* cur = first; cur; cur = cur->next) {
			if (cur->val == obj) {
				par->next = cur->next;
				delete cur;
				--num_elements;
				return true;
			}
			par = cur;
		}
		return false;
	}
	void Clear() {
		const size_type n = buckets.size();
		for (int i = 0; i < n; ++i) {
			while (buckets[i]) {
				node* cur = buckets[i];
				buckets[i] = cur->next;
				delete cur;
			}
		}
	}
public:
	iterator begin() {
		//寻找第一个非空桶
		for (size_type n = 0; n < buckets.size(); ++n) {
			if (buckets[n])
				return iterator(buckets[n], this);
		}
		return end();
	}
	iterator end() {
		return iterator(nullptr, this);
	}
private:
	size_type bkt_num(const valTy& obj) {
		return bkt_num_key(get_key(obj));
	}
	size_type bkt_num(const valTy& obj, size_t n) {
		return  bkt_num_key(get_key(obj, n));
	}
	size_type bkt_num_key(const keyTy& key, size_t n) {
		return hash(key) % n;
	}
	size_type bkt_num_key(const keyTy& key) {
		return bkt_num_key(key, buckets.size());
	}
	node* new_node(const valTy& obj) {
		node* n = (node*)malloc(sizeof(node));
		n->next = nullptr;
		n->val = obj;
		return n;
	}
	bool insert_equal_noresize(const valTy& obj) {
		const size_type n = bkt_num(obj);//获取待入桶号
		node* first = buckets[n];
		for (node* cur = first; cur; cur = cur->next) {
			//同值放一起
			if (cur->val == obj) {
				node* tmp = new_node(obj);
				tmp->next = cur->next;
				cur->next = tmp;
				return true;
			}
		}
		node* tmp = new_node(obj);
		tmp->next = first;
		buckets[n] = tmp;
		++num_elements;
		return true;
	}
	bool insert_unique_noresize(const valTy& obj) {
		const size_type n = bkt_num(obj);
		node* first = buckets[n];
		//插入之前检查是否已存在同值元素
		for (node* cur = first; cur; cur = cur->next) {
			if (obj == cur->val)
				return false;
		}
		node* tmp = new_node(obj);
		tmp->next = first;
		buckets[n] = tmp;
		++num_elements;
		return true;
	}
	void initialize_buckets(size_type n) {
		const size_type n_buckets = next_size(n);//获取桶的数目
		buckets.reserve(n_buckets);//开辟空间
		buckets.insert(buckets.end(), n_buckets, nullptr);
		num_elements = 0;
	}
	size_type next_size(size_type n)const{
		return __stl_next_prime(n);//下一个质数
	}
	void resize(size_type num_elements_hint) {
		const size_type old_n = buckets.size();
		if (num_elements_hint > old_n) {
			const size_type n = next_size(num_elements_hint);
			if (n > old_n) {
				vector<node*> tmp(n, nullptr);
				for (size_type bucket = 0; bucket < old_n; ++bucket) {
					node* first = buckets[bucket];
					while (first) {
						size_type new_bucket = bkt_num(first->val);//计算新桶号
						buckets[bucket] = first->next;//下一个数成为头
						//头插进新桶
						first->next = tmp[new_bucket];
						tmp[new_bucket] = first;
						first = buckets[bucket];
					}
				}
				buckets.swap(tmp);
			}
		}
	}
private:
	hasher hash;
	ExtractKey get_key;
	vector<node*> buckets;
	size_t num_elements;
};
