#include <queue>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib> 
using namespace std;

#define MAX_QUEUE 10
#define MAX_THREAD 5

typedef void(*handler_t)(int);
class ThreadTask{ //线程任务类
    public:
        ThreadTask(){}
        ThreadTask(int data, handler_t handler):_data(data),_handler(handler){}
        void Run(){ _handler(_data); }//该任务如何处理
    private:
        int _data;
        handler_t _handler;
};

class BlockQueue{//阻塞队列
    public:
        BlockQueue(int que = MAX_QUEUE):_capacity(que){
            pthread_mutex_init(&_mutex,NULL);//初始化互斥锁与条件变量
            pthread_cond_init(&_product,NULL);
            pthread_cond_init(&_customer,NULL);
        }
        ~BlockQueue(){
            pthread_mutex_destroy(&_mutex);//销毁互斥锁与条件变量
            pthread_cond_destroy(&_product);
            pthread_cond_destroy(&_customer);
        }
        bool Push(const ThreadTask& data){//任务进队：加锁->队列已满?->是，陷入等待
            pthread_mutex_lock(&_mutex);//                           ->否，任务入队，唤醒处理线程->解锁
            while(_q.size() == _capacity)
                pthread_cond_wait(&_product,&_mutex);
            _q.push(data);
            pthread_cond_signal(&_customer);
            pthread_mutex_unlock(&_mutex);
            return true;
        }
        bool Pop(ThreadTask* data){//任务出队：加锁->队列空?->是，陷入等待
            pthread_mutex_lock(&_mutex);//                  ->否，任务出队，唤醒任务生成线程->解锁
            while(_q.empty())
                pthread_cond_wait(&_customer,&_mutex);
            *data = _q.front();
            _q.pop();
            pthread_cond_signal(&_product);
            pthread_mutex_unlock(&_mutex);
        }
    private:
        pthread_mutex_t _mutex;//互斥锁
        pthread_cond_t _product;//生产者条件变量
        pthread_cond_t _customer;//消费者条件变量
        queue<ThreadTask> _q;//数据队列
        size_t _capacity;
};

class ThreadPool{
    public:
        ThreadPool(int maxq=MAX_QUEUE,int maxt = MAX_THREAD):_max_thread(maxt),_que(maxq){
            int ret;
            pthread_t tid;
            for(int i = 0;i<_max_thread; ++i){ //生成_max_thread个线程
                ret = pthread_create(&tid,NULL,thr_entry,this);
                if(ret)
                    exit(-1);
                pthread_detach(tid);//线程脱离
            }
        }
        bool TaskPush(const ThreadTask& task){//增加新任务
            _que.Push(task);
            return true;
        }
    private:
        static void* thr_entry(void* arg){
            ThreadPool* p = (ThreadPool*)arg;
            while(true){
                ThreadTask task;
                p->_que.Pop(&task);
                task.Run();
            }
            return NULL;
        }
    private:
        int _max_thread;
        BlockQueue _que;
};

void testfunc(int data)
{
    printf("%p--get data:%d, sleep %d sec\n", pthread_self(),data, (data % 3) + 1);
    sleep((data % 3) + 1);

}
int main()
{
    ThreadPool pool;
    for (int i = 0; i < 10; i++) {
        ThreadTask task(i, testfunc);
        pool.TaskPush(task);

    }
    while(1)
        sleep(1);
    return 0;
}
