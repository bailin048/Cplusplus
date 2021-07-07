#include <iostream>
#include <vector>
using namespace std;

namespace BL {
	template <size_t _N>
	class bitset {
	public:
		typedef unsigned long _Ty;//无符号8字节
	public:
		bitset() { _Tidy(); }
		bitset<_N>& set(){
			_Tidy(~(_Ty)0);//按位全1
			return (*this);
		}
		bitset<_N>& set(size_t _P, bool _X = true){
			if (_X)//设置_P为比特为1
				_A[_P / _Nb] |= (_Ty)1 << _P % _Nb;
			else//设置_P为比特为0
				_A[_P / _Nb] &= ~((_Ty)1 << _P % _Nb);
			return (*this);
		}
		bitset& flip(){ // 所有比特翻转
			for (size_t _I = 0; _I <= _Nw; ++_I) {
				_A[_I] = ~_A[_I];
			}
			_Trim();
			return *this;
		}
		size_t size() const { return (_N); }
		bool test(size_t _P) const{
			//第_P位是否为1，是则true
			return ((_A[_P / _Nb] & ((_Ty)1 << _P % _Nb)) != 0);
		}
		bool any() const{
			//是否存在非0位
			for (int _I = _Nw; 0 <= _I; --_I)
				if (_A[_I] != 0)
					return (true);
			return (false);
		}
		bool none() const { return (!any());}
		size_t count() const{
			//计算1的个数
			size_t _V = 0;
			for (int _I = _Nw; 0 <= _I; --_I)
				for (_Ty _X = _A[_I]; _X != 0; _X >>= 4)
					_V += "\0\1\1\2\1\2\2\3"
					"\1\2\2\3\2\3\3\4"[_X & 0xF];
			return (_V);
		}
		class reference{
			friend class bitset<_N>;
		public:
			reference& operator=(bool _X){
				//第_P位赋_X
				_Pbs->set(_Off, _X);
				return (*this);
			}
			reference& operator=(const reference& _Bs){
				_Pbs->set(_Off, bool(_Bs));
				return (*this);
			}
			reference& flip(){
				//取反
				_Pbs->flip(_Off);
				return (*this);
			}
			bool operator~() const{
				//是否为0
				return (!_Pbs->test(_Off));
			}
			operator bool() const{
				//是否为1
				return (_Pbs->test(_Off));
			}
		private:
			reference(bitset<_N>& _X, size_t _P)
				: _Pbs(&_X), _Off(_P) {}
			bitset<_N>* _Pbs;
			size_t _Off;
		};
		reference operator[](size_t _P) { return (reference(*this, _P)); }
	private:
		void _Tidy(_Ty _X = 0) {//重置数据为全0
			for (int _i = _Nw; _i >= 0; --_i)//置为_X
				_A[_i] = _X;
			if (_X != 0)//多出的字节置0(大于_N的字节)
				_Trim();
		}
		void _Trim() {
			if (_N % _Nb != 0) //仅保留合理范围内的1
				_A[_Nw] &= ((_Ty)1 <<  _N % _Nb) - 1;
		}
	private:
		enum {
			_Nb = CHAR_BIT * sizeof(_Ty),//数组单元素所占比特数:32
			_Nw = _N == 0 ? 0 : (_N -1)/ _Nb
			//计算需要几个4字节空间，之所以_N-1的原因在于：
			//_Nw代表可以完全占据一个_Ty空间的数目
			//eg.127,(127-1)/32=3余30,_Nw=1
			//eg.128,(128-1)/32=3余31，_Nw=1
			//之所以使用(_N-1)/_Nb,其思路绝对保证完整_Ty数目，减1之后，
			//剩下的数目必然只有两种可能，刚好一个完整Ty/不足一个完整_Ty
			//将剩下的数目交给申请数组时处理，有归一化处理的涵义
		};
		_Ty _A[_Nw+ 1];//因此加1补不足
	private:
		friend ostream& operator<<(ostream& _O, const bitset<_N>& _R){
			//重载输出符
			for (size_t _P = _N; 0 < _P;)
				_O << (_R.test(--_P) ? '1' : '0');
			return (_O);
		}
	};
};

int main() {
	BL::bitset<100> bt;
	bt.set(3);
	bt.flip();
	return 0;
}