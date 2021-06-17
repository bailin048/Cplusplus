#include <iostream>
#include <vector>
using namespace std;

template <size_t N>
class bitset{
public:
	bitset()
		: _size(0), _bitCount(N){
		_bit.resize((N >> 3) + 1);
	}
	// ��which����λ��1
	void set(size_t which) {
		assert(which < _bitCount);
		int index = which >> 3;//���ڵڼ���Ԫ��
		int pos = which % 8;//����λ�ڵڼ�λ
		_bit[index] |= (1 << pos);
		++_size;
	}
	// ��which����λ��0
	void reset(size_t which) {
		assert(which < _bitCount);
		int index = which >> 3;//���ڵڼ���Ԫ��
		int pos = which % 8;//����λ�ڵڼ�λ
		_bit[index] &= (~(1 << pos));
		--_size;
	}
	// ���λͼ��which�Ƿ�Ϊ1
	bool test(size_t which) {
		assert(which < _totalBite);
		size_t index = (which >> 3);
		size_t pos = which % 8;
		return  _bst[index] & (1 << pos);
	}
	size_t size()const { return _bitCount; }
private:
	vector<int> _bit;
	size_t _bitCount;
	size_t _size;
};