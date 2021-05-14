#include <iostream>
#include <unistd.h>
#include <vector>
#include <semaphore.h>
using namespace std;
#define MAX_QUEUE 5

class CycleQueue{
    public:
        CycleQueue(int cap = MAX_QUEUE):_capacity(cap),_front(0),_rear(0),_q(cap){
            sem_init(&_sem_empty,0,cap);//初始化信号量
            sem_init(&_sem_data,0,0);
            sem_init(&_sem_lock,0,1);
        }
        ~CycleQueue(){
            sem_destroy(&_sem_empty);//销毁信号量
            sem_destroy(&_sem_data);
            sem_destroy(&_sem_lock);
        }
        bool Push(const int& data){
            sem_wait(&_sem_empty);//空闲节点-1
            sem_wait(&_sem_lock);//加锁
            _q[_rear] = data;//入队
            _rear = (_rear+1)%_capacity;
            sem_post(&_sem_lock);//解锁
            sem_post(&_sem_data);//数据节点+1
            return true;
        }
        bool Pop(int* data){
            sem_wait(&_sem_data);//数据节点-1
            sem_wait(&_sem_lock);//加锁
            *data = _q[_front];//出队
            _front = (_front+1)%_capacity;
            sem_post(&_sem_lock);//解锁
            sem_post(&_sem_empty);//空闲节点+1
            return true;
        }
    private:
        size_t _capacity;//队列容量
        vector<int> _q;//数据队列
        int _front;//头指针
        int _rear;//尾指针
        sem_t _sem_empty;//空闲节点计数
        sem_t _sem_data;//数据节点计数
        sem_t _sem_lock;//实现互斥
};
void* productor(void* arg){
    CycleQueue *q = (CycleQueue*)arg;
    int data = 0;
    while(1){
        q->Push(data);
        cout<<"push data："<<data<<endl;
        ++data;
    }
    return NULL;
}
void* customer(void* arg){
    CycleQueue *q = (CycleQueue*)arg;
    while(1){
        int data = 0;
        q->Pop(&data);
        cout<<"pop data："<<data<<endl;
    }
    return NULL;
}
int main(){
    pthread_t protid[4],custid[4];
    int ret;
    CycleQueue q;
    for(int i = 0;i<4;++i){
        ret = pthread_create(&protid[i],NULL,productor,&q);
        if(ret){
            cout<<"create thread failed!"<<endl;
            return -1;
        }
        ret = pthread_create(&custid[i],NULL,customer,&q);
        if(ret){
            cout<<"create thread failed!"<<endl;
            return -1;
        }
    }
    for(int i = 0;i<4;++i){
        pthread_join(protid[i],NULL);
        pthread_join(custid[i],NULL);
    }
    return 0;
}
