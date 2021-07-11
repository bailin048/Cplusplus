#include <iostream>
#include <vld.h>
#include <assert.h>
using namespace std;
using namespace boost;


namespace C98 {
	/// ԭʼ����ָ��
	template <class _Ty>
	class auto_ptr {
	public:
		auto_ptr(_Ty* p = 0) : _P(p), _Owns(p != 0)
		{}
		auto_ptr(const auto_ptr<_Ty>& ap):_Owns(ap._Owns),_P(ap.release())
		{}
		~auto_ptr() {
			if (_Owns)
				delete _P;
		}
		_Ty& operator*() { return (*get()); }
		_Ty* operator->() { return get(); }
		auto_ptr<_Ty>& operator=(const auto_ptr<_Ty>& ap) {
			if (this != &ap) {//�����Լ����Լ���ֵ
				if (_P != ap.get()) {//����Ĳ���ͬһ��ռ�
					if (_Owns) //ӵ�й���Ȩ���ͷ�ԭ�ռ�
						delete _P;
					_Owns = ap._Owns;//����Ȩ����
				}
				else if (ap._Owns)//�������ͬһ��ռ䵫apӵ�й���Ȩ
						_Owns = true;//����Ȩ����
				_P = ap.release();//�ռ��ƽ�
			}
			return *this;
		}
	private:
		_Ty* get()const { return( _P); }
		_Ty* release()const  {
			((auto_ptr<_Ty>*)this)->_Owns = false;
			return _P;
		}
	private:
		bool _Owns;//ӵ��Ȩ
		_Ty* _P;//����ռ�
	};

}
namespace VS {
	/// VS�汾�µ�����ָ��,����ת��
	template<class _Ty>
	class auto_ptr {
	public:
		typedef auto_ptr<_Ty> _Myt;
	public:
		auto_ptr(_Ty* p):_ptr(p) 
		{}
		~auto_ptr() {
			delete _ptr;
		}
		auto_ptr(_Myt& _Right): _ptr(_Right.release())
		{}
		_Myt& operator=(_Myt& _Right){
			reset(_Right.release());
			return (*this);
		}
		_Ty& operator*()const { return (*get()); }
		_Ty* operator->()const { return (get()); }
	private:
		_Ty* get() { return _ptr; }
		_Ty* release() {
			_Ty* tmp = _ptr;
			_ptr = 0;
			return (tmp);
		}
		void reset(_Ty* pt = 0) {
			if (pt != _ptr)
				delete _ptr;
			_ptr = pt;
		}
	private:
		_Ty* _ptr;
	};
};
namespace MyBoost {
	template<class T> class scoped_ptr {
	private:
		T* px;
		scoped_ptr(scoped_ptr const&);
		scoped_ptr& operator=(scoped_ptr const&);

		typedef scoped_ptr<T> this_type;

		void operator==(scoped_ptr const&) const;
		void operator!=(scoped_ptr const&) const;
	public:
		typedef T element_type;

		explicit scoped_ptr(T* p = 0) : px(p)
		{}
		explicit scoped_ptr(std::auto_ptr<T> p) : px(p.release()) {}
		~scoped_ptr() {
			release(px);
		}

		void reset(T* p = 0) {
			this_type(p).swap(*this);
		}

		T& operator*() const {
			return *px;
		}

		T* operator->() const {
			return px;
		}

		T* get() const {
			return px;
		}
		void swap(scoped_ptr& b) {
			T* tmp = b.px;
			b.px = px;
			px = tmp;
		}
		void release(T* p) {
			delete p;
		}
	};
	template<class T> 
	class scoped_array {
	private:
		T* px;
		scoped_array(scoped_array const&);
		scoped_array& operator=(scoped_array const&);

		typedef scoped_array<T> this_type;

		void operator==(scoped_array const&) const;
		void operator!=(scoped_array const&) const;

	public:
		typedef T element_type;
		explicit scoped_array(T* p = 0) : px(p) // never throws
		{}

		~scoped_array(){
			delete[] px;
		}

		void reset(T* p = 0){
			this_type(p).swap(*this);
		}

		T& operator[](std::ptrdiff_t i) const {
			assert(px != 0);
			assert(i >= 0);
			return px[i];
		}

		T* get() const { return px; }
		void swap(scoped_array& b) {
			T* tmp = b.px;
			b.px = px;
			px = tmp;
		}
	};
}
int main() {
	int* p = new int[10]{1,2,3,4,5,6,7,8,9,10};
	MyBoost::scoped_array<int> ps(p);
	for (int i = 0; i < 10; ++i)
		cout << ps[i] << " ";
	cout << endl;


	return 0;
}
