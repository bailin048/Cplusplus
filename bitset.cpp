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
	// 将which比特位置1
	void set(size_t which) {
		assert(which < _bitCount);
		int index = which >> 3;//属于第几个元素
		int pos = which % 8;//计算位于第几位
		_bit[index] |= (1 << pos);
		++_size;
	}
	// 将which比特位置0
	void reset(size_t which) {
		assert(which < _bitCount);
		int index = which >> 3;//属于第几个元素
		int pos = which % 8;//计算位于第几位
		_bit[index] &= (~(1 << pos));
		--_size;
	}
	// 检测位图中which是否为1
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