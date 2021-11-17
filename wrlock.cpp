#include <vector>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
using namespace std;

volatile int tickets = 1000;
pthread_rwlock_t rwlock;

struct ThreadAttr{
    pthread_t tid;
    string id;
};

void* reader(void* arg){
    char* id = (char*)arg;
    while(1){
        pthread_rwlock_rdlock(&rwlock);
        if(tickets <= 0){
            pthread_rwlock_unlock(&rwlock);
            break;
        }
        printf("%s: %d\n",id,tickets);
        pthread_rwlock_unlock(&rwlock);
        usleep(1);
    }
    return nullptr;
}

void* writer(void* arg){
    char* id = (char*)arg;
    while(1){
        pthread_rwlock_wrlock(&rwlock);
        if(tickets <= 0){
            pthread_rwlock_wrlock(&rwlock);
            break;
        }
        printf("%s: %d\n",id, --tickets);
        pthread_rwlock_unlock(&rwlock);
        usleep(1);
    }
    return nullptr;
}

string create_writer_id(size_t i){
    ostringstream oss("thread writer ", ios_base::ate);
    oss<<i;
    return oss.str();
}

string create_reader_id(size_t i){
    ostringstream oss("thread reader ", ios_base::ate);
    oss<<i;
    return oss.str();
}
void init_rwlock(){
#if 0//写优先
    pthread_rwlockattr_t attr;
    pthread_rwlockattr_init(&attr);
    pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
    pthread_rwlockattr_init(&attr);
    pthread_rwlockattr_destroy(&attr);
#else
    pthread_rwlock_init(&rwlock,nullptr);
#endif
}

void init_readers(vector<ThreadAttr>& vec){
    for(size_t i = 0; i < vec.size(); ++i){
        vec[i].id = create_reader_id(i);
        pthread_create(&vec[i].tid, nullptr, reader, (void*)vec[i].id.c_str());
    }
}

void init_writers(vector<ThreadAttr>& vec){
    for(size_t i = 0; i < vec.size(); ++i){
        vec[i].id = create_writer_id(i);
        pthread_create(&vec[i].tid,nullptr,writer,(void*)vec[i].id.c_str());
    }
}
void join_threaders(vector<ThreadAttr> const& vec){
    for(vector<ThreadAttr>::const_reverse_iterator it = vec.rbegin(); it != vec.rend(); ++it){
        pthread_t const& tid = it->tid;
        pthread_join(tid, nullptr);
    }
}
int main(){
    const size_t reader_nr = 1000;
    const size_t writer_nr = 2;
    vector<ThreadAttr> readers(reader_nr);
    vector<ThreadAttr> writers(writer_nr);

    init_rwlock();

    init_readers(readers);
    init_writers(writers);

    join_threaders(writers);
    join_threaders(readers);

    pthread_rwlock_destroy(&rwlock);
}
