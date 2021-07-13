#include <iostream>
#include <mutex>
#include <thread>
#include <assert.h>
using namespace std;

namespace BL {
	//单例模式——饿汉模式
	class Singleton {
		static Singleton* GetInstance() { return &m_instance; }
	private:
		Singleton() {}//构造函数私有化
		Singleton(const Singleton&) = delete;//防拷贝
		Singleton& operator=(const Singleton&) = delete;
		static Singleton m_instance;//单例对象
	};
	Singleton Singleton::m_instance;//对象初始化，在程序入口之前就初始化

	//单例模式——懒汉模式
	class Singleton2 {
	public:
		static Singleton2* Getinstance() {
			if (nullptr == m_pinstance) {
				_mutex.lock();//加锁
				if (nullptr == m_pinstance)//双检锁
					m_pinstance = new Singleton2();
				_mutex.unlock();

			}
			return m_pinstance;
		}
		//内嵌一个垃圾回收类
		class CGarbo{
		public:
			~CGarbo(){
				if (Singleton2::m_pinstance)
					delete m_pinstance;
			}
		};
		//定义一个静态成员变量，程序结束时，系统会自动调用其析构函数释放单例对象
		static CGarbo Garbo;
	private:
			Singleton2() {}//构造私有化
			Singleton2(const Singleton2&) = delete;//防拷贝
			Singleton2& operator=(const Singleton&) = delete;
			static Singleton2* m_pinstance;//单例对象指针
			static mutex _mutex;//互斥锁
	};
	Singleton2* Singleton2::m_pinstance = nullptr;
	Singleton2::CGarbo Garbo;
	mutex Singleton2::_mutex;

	template <typename T>
	class LASingletonTemplateBase {
	private:
		static T* sm_instance;
	protected:
		LASingletonTemplateBase() {
			assert(sm_instance == 0);
			sm_instance = static_cast<T*>(this);
		}
		virtual ~LASingletonTemplateBase(){
			assert(sm_instance != 0);
			sm_instance = 0;
		}
	public:
		static T* get_instance_ptr(){
			if (sm_instance == 0)
				sm_instance = new T();
			return sm_instance;
		}
		static T& get_instance_ref(){
			if (sm_instance == 0)
				sm_instance = new T();
			return *sm_instance;
		}
		static void remove_instance()
		{
			assert(sm_instance);
			if (sm_instance)
				delete sm_instance;
			assert(sm_instance == 0);
		}
	};
	template <typename T>
	T* LASingletonTemplateBase<T>::sm_instance = 0;
}
class Test {};
int main() {
	Test* t = BL::LASingletonTemplateBase<Test> ::get_instance_ptr();
}