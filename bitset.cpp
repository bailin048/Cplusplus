#include <iostream>
#include <vector>
using namespace std;

namespace BL {
	template <size_t _N>
	class bitset {
	public:
		typedef unsigned long _Ty;//�޷���8�ֽ�
	public:
		bitset() { _Tidy(); }
		bitset<_N>& set(){
			_Tidy(~(_Ty)0);//��λȫ1
			return (*this);
		}
		bitset<_N>& set(size_t _P, bool _X = true){
			if (_X)//����_PΪ����Ϊ1
				_A[_P / _Nb] |= (_Ty)1 << _P % _Nb;
			else//����_PΪ����Ϊ0
				_A[_P / _Nb] &= ~((_Ty)1 << _P % _Nb);
			return (*this);
		}
		bitset& flip(){ // ���б��ط�ת
			for (size_t _I = 0; _I <= _Nw; ++_I) {
				_A[_I] = ~_A[_I];
			}
			_Trim();
			return *this;
		}
		size_t size() const { return (_N); }
		bool test(size_t _P) const{
			//��_Pλ�Ƿ�Ϊ1������true
			return ((_A[_P / _Nb] & ((_Ty)1 << _P % _Nb)) != 0);
		}
		bool any() const{
			//�Ƿ���ڷ�0λ
			for (int _I = _Nw; 0 <= _I; --_I)
				if (_A[_I] != 0)
					return (true);
			return (false);
		}
		bool none() const { return (!any());}
		size_t count() const{
			//����1�ĸ���
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
				//��_Pλ��_X
				_Pbs->set(_Off, _X);
				return (*this);
			}
			reference& operator=(const reference& _Bs){
				_Pbs->set(_Off, bool(_Bs));
				return (*this);
			}
			reference& flip(){
				//ȡ��
				_Pbs->flip(_Off);
				return (*this);
			}
			bool operator~() const{
				//�Ƿ�Ϊ0
				return (!_Pbs->test(_Off));
			}
			operator bool() const{
				//�Ƿ�Ϊ1
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
		void _Tidy(_Ty _X = 0) {//��������Ϊȫ0
			for (int _i = _Nw; _i >= 0; --_i)//��Ϊ_X
				_A[_i] = _X;
			if (_X != 0)//������ֽ���0(����_N���ֽ�)
				_Trim();
		}
		void _Trim() {
			if (_N % _Nb != 0) //����������Χ�ڵ�1
				_A[_Nw] &= ((_Ty)1 <<  _N % _Nb) - 1;
		}
	private:
		enum {
			_Nb = CHAR_BIT * sizeof(_Ty),//���鵥Ԫ����ռ������:32
			_Nw = _N == 0 ? 0 : (_N -1)/ _Nb
			//������Ҫ����4�ֽڿռ䣬֮����_N-1��ԭ�����ڣ�
			//_Nw���������ȫռ��һ��_Ty�ռ����Ŀ
			//eg.127,(127-1)/32=3��30,_Nw=1
			//eg.128,(128-1)/32=3��31��_Nw=1
			//֮����ʹ��(_N-1)/_Nb,��˼·���Ա�֤����_Ty��Ŀ����1֮��
			//ʣ�µ���Ŀ��Ȼֻ�����ֿ��ܣ��պ�һ������Ty/����һ������_Ty
			//��ʣ�µ���Ŀ������������ʱ�����й�һ������ĺ���
		};
		_Ty _A[_Nw+ 1];//��˼�1������
	private:
		friend ostream& operator<<(ostream& _O, const bitset<_N>& _R){
			//���������
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