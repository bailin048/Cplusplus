#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

namespace OpenHash
{
	template<class T>
	class HashFunc{
	public:
		size_t operator()(const T& val) { return val; }
	};

	template<>
	class HashFunc<string>{
	public:
		size_t operator()(const string& s){
			const char* str = s.c_str();
			unsigned int seed = 131; // 31 131 1313 13131 131313
			unsigned int hash = 0;
			while (*str)
				hash = hash * seed + (*str++);
			return hash;
		}
	};

	template<class V>
	struct HashBucketNode{
		HashBucketNode(const V& data)
			: _pNext(nullptr), _data(data)
		{}
		HashBucketNode<V>* _pNext;
		V _data;
	};

	// ������ʵ�ֵĹ�ϣͰ��key��Ψһ��
	template<class V, class HF = HashFunc<V>>
	class HashBucket{
		typedef HashBucketNode<V> Node;
		typedef Node* PNode;
		typedef HashBucket<V, HF> Self;
	public:
		HashBucket(size_t capacity)
			: _table(GetNextPrime(capacity))
			, _size(0){}

		~HashBucket() { Clear(); }

		// ��ϣͰ�е�Ԫ�ز����ظ�
		Node* Insert(const V& data) {
			//�������
			CheckCapacity();
			//����Ͱ��
			size_t bucketNo = HashFunc(data);
			//����Ƿ��Ԫ���Ѵ�����������
			Node* pcur = _table[bucketNo];
			while (pcur) {
				if (pcur->data == data)
					return nullptr;//��������β���
				pcur = pcur->_pNext;
			}
			//��������ͷ��
			pcur = new Node(data);
			pcur->_pNext = _table[bucketNo];
			_table[bucketNo] = pcur;
			++_size;
			return pcur;
		}

		// ɾ����ϣͰ��Ϊdata��Ԫ��(data�����ظ�)
		bool Erase(const V& data) {
			size_t bucketNo = HashFunc(data);
			Node* pcur = _table[bucketNo];
			Node* ppre = nullptr;
			while (pcur) {
				if (data = pcur->_data) {
					if (_table[bucketNo] == pcur)
						_table[bucketNo] = pcur->_pNext;
					else
						ppre->_pNext = pcur->_pNext;
					delete pcur;
					--_size;
					return true;
				}
				ppre = pcur;
				pcur = pcur->_pNext;
			}
			return false;
		}

		Node* Find(const V& data) {
			size_t bucketNo = HashFunc(data);
			Node* pcur = _table[bucketNo];
			while (pcur) {
				if (data = pcur->_data)
					return pcur;
				pcur = pcur->_pNext;
			}
			return nullptr;
		}

		size_t Size()const { return _size; }

		bool Empty()const { return 0 == _size; }

		void Clear() {
			for (inr i = 0; i < _table.capacity(); ++i) {
				Node* pcur = _table[i];
				while (pcur) {
					_table[i] = pcur->_pNext;
					delete pcur;
					pcur = _table[i];
				}
			}
			_size = 0;
		}

		size_t BucketCount()const { return _table.capacity(); }

		void Swap(Self& ht){
			_table.swap(ht._table);
			swap(_size, ht._size);
		}

	private:
		size_t HashFunc(const V& data){
			return HF()(data) % _table.capacity();
		}

		void CheckCapacity() {
			if (_size == _table.capacity()) {
				HashBucket<T> ht(_size * 2);
				//ת������
				for (size_t i = 0; i < _table.capacity(); ++i) {
					Node* pcur = _table[i];
					while (pcur) {
						ht.Insert(pcur->_data);
						pcur = pcur->_pNext;
					}
				}
				Swap(ht);
			}
		}

	private:
		vector<Node*> _table;
		size_t _size;      // ��ϣ������ЧԪ�صĸ��� 
	};
}
