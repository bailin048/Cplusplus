#include <iostream>
#include <mutex>
#include <thread>
#include <assert.h>
using namespace std;

namespace BL {
	//����ģʽ��������ģʽ
	class Singleton {
		static Singleton* GetInstance() { return &m_instance; }
	private:
		Singleton() {}//���캯��˽�л�
		Singleton(const Singleton&) = delete;//������
		Singleton& operator=(const Singleton&) = delete;
		static Singleton m_instance;//��������
	};
	Singleton Singleton::m_instance;//�����ʼ�����ڳ������֮ǰ�ͳ�ʼ��

	//����ģʽ��������ģʽ
	class Singleton2 {
	public:
		static Singleton2* Getinstance() {
			if (nullptr == m_pinstance) {
				_mutex.lock();//����
				if (nullptr == m_pinstance)//˫����
					m_pinstance = new Singleton2();
				_mutex.unlock();

			}
			return m_pinstance;
		}
		//��Ƕһ������������
		class CGarbo{
		public:
			~CGarbo(){
				if (Singleton2::m_pinstance)
					delete m_pinstance;
			}
		};
		//����һ����̬��Ա�������������ʱ��ϵͳ���Զ����������������ͷŵ�������
		static CGarbo Garbo;
	private:
			Singleton2() {}//����˽�л�
			Singleton2(const Singleton2&) = delete;//������
			Singleton2& operator=(const Singleton&) = delete;
			static Singleton2* m_pinstance;//��������ָ��
			static mutex _mutex;//������
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