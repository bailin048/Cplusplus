#include <iostream>
#include <unistd.h>
#include <pthread.h>

#define  MAX_QUEUE 5
using namespace std;

class CycleQueue {
    public:
        CycleQueue(int sz = MAX_QUEUE):_capacity(sz) {
            _q = new int[_capacity];
            _front = 0;
            _rear = 0;

        }
        ~CycleQueue() {
            delete[] _q;
            _capacity = 0;
            _front = _rear = 0;

        }
        bool empty() { return _rear == _front;  }
        bool full() { return _rear + 1  == _front;  }
        bool push(int data) {
            if (full())
                return false;
            _q[_rear] = data;
            _rear = (_rear + 1) % _capacity;
            return true;

        }
        bool pop() {
            if (empty())
                return false;
            _front = (_front + 1) % _capacity;
            return true;

        }
        int size() { return (_rear + _capacity - _front) % _capacity;  }
        int front() { return _q[_front];  }
    private:
        size_t _capacity;
        int* _q;
        size_t _front;
        size_t _rear;

};

class BlockQueue{
    public:
        BlockQueue(){
            pthread_mutex_init(&_mutex,NULL);//初始化互斥锁
            pthread_cond_init(&_product,NULL);//初始化条件变量
            pthread_cond_init(&_customer,NULL);
        }
        ~BlockQueue(){//销毁条件变量及互斥锁
            pthread_mutex_destroy(&_mutex);
            pthread_cond_destroy(&_product);
            pthread_cond_destroy(&_customer);
        }
        bool Push(const int& data){
            pthread_mutex_lock(&_mutex);//加锁
            while(_q.size() == MAX_QUEUE)//队列已满
                pthread_cond_wait(&_product,&_mutex);//陷入等待
            _q.push(data);//数据入队
            pthread_cond_signal(&_customer);//唤醒消费者
            pthread_mutex_unlock(&_mutex);//解锁
            return true;
        }
        bool Pop(int* data){
            pthread_mutex_lock(&_mutex);//加锁
            while(_q.empty())//队列为空
                pthread_cond_wait(&_customer,&_mutex);//陷入等待
            *data = _q.front();
            _q.pop();
            pthread_cond_signal(&_product);//唤醒生产者
            pthread_mutex_unlock(&_mutex);//解锁
            return true;
        }
    private:
        pthread_mutex_t _mutex;//互斥锁
        pthread_cond_t _product;//生产者条件变量
        pthread_cond_t _customer;//消费者条件变量
        CycleQueue _q;//数据队列
};

void* producter(void* arg){
    BlockQueue *q = (BlockQueue*)arg;//生产者队列
    int data = 0;
    while(1){
        q->Push(data);
        cout<<"push data# "<<data<<endl;
        ++data;
        sleep(1);
    }
    return NULL;
}
void* customer(void* arg){
    BlockQueue* q = (BlockQueue*)arg;//消费者队列
    while(1){
        int data = 0;
        q->Pop(&data);//获取数据
        cout<<"pop data# "<<data<<endl;
        sleep(1);
    }
    return NULL;
}
int main(int arg,char* argv[]){
    pthread_t ptid[4],ctid[4];
    BlockQueue q;
    for(int i = 0;i<4;++i){ //创建消费者与生产者线程
        int ret = pthread_create(&ptid[i],NULL,producter,&q);
        if(ret){
            cout<<"thread creat failed!"<<endl;
            return -1;
        }
        ret = pthread_create(&ctid[i],NULL,customer,&q);
        if(ret){
            cout<<"thread creat failed!"<<endl;
            return -1;
        }
    }
    for(int i = 0;i<4;++i){//线程等待
        pthread_join(ptid[i],NULL);
        pthread_join(ctid[i],NULL);
    }
    return 0;
}
