#include <iostream>
#include <assert.h>
#include <vld.h>
using namespace std;

namespace BL{
    template<class T>
    class vector{
    public:
        // Vector的迭代器是一个原生指针
        typedef T* iterator;
            typedef const T* const_iterator;
            iterator begin() { return _start; }
            iterator end() { return _finish; }
            const_iterator cbegin()const { return _start; }
            const_iterator cend() const { return _finish; }
            // construct and destroy
            vector() :_start(nullptr),
                _finish(nullptr),
                _endOfStorage(nullptr) {}
            vector(int n, const T& value = T()):_start(nullptr),
                _finish(nullptr),
                _endOfStorage(nullptr){
                reserve(n);
                while (n--)
                    push_back(value);
            }
            template<class InputIterator>
            vector(InputIterator first, InputIterator last) {
                reserve(last - first);
                while (first != last) {
                    push_back(*first);
                    ++first;
                }
            }
            vector(const vector<T>& v) : _start(nullptr),
                _finish(nullptr),
                _endOfStorage(nullptr) {
                reserve(v.capacity());
                iterator it = begin();
                const_iterator vit = v.cbegin();
                while (vit != v.cend())
                    *it = *vit, ++it, ++vit;
            }
            vector<T>& operator= (vector<T> v) {
                vector<T> tmp(v.begin(), v.end());
                swap(tmp);
                return *this;
            }
            ~vector() {
                delete[] _start;
            }
            // capacity
                size_t size() const { return _finish - _start; }
                size_t capacity() const { return _endOfStorage - _start; }
                void reserve(size_t n) {
                    if(n>capacity()){//增容
                        size_t oldSize = size();
                        T* tmp = new T[n];
                        if (_start) {//拷贝数据
                            for (size_t i = 0; i < oldSize; ++i)
                                tmp[i] = _start[i];
                        }
                        delete[] _start;
                        _start = tmp;
                        _finish = tmp + oldSize;
                        _endOfStorage = _start + n;
                    }
                }
                void resize(size_t n, const T& value = T()) {
                    if (n <= size()) {
                        _finish = _start + n;
                        return;
                    }
                    if (n > capacity())
                        reserve(n);
                }
                    ///////////////access///////////////////////////////
                T& operator[](size_t pos) { return _start[pos]; }
                const T& operator[](size_t pos)const { return _start[pos]; }
            ///////////////modify/////////////////////////////
                void push_back(const T& x) { insert(end(), x); }
                void pop_back() { erase( end()-1); }
                void swap(vector<T>& v) {
                    iterator tmp = v._start;
                    v._start = _start;
                    _start = tmp;

                    tmp = v._finish;
                    v._finish = _finish;
                    _finish = tmp;

                    tmp = v._endOfStorage;
                    v._endOfStorage = _endOfStorage;
                    _endOfStorage = tmp;

                }
                iterator insert(iterator pos, const T& x) {
                    assert(pos <= _finish);

                    if (_finish == _endOfStorage) {//增容
                        size_t oldsize = pos - _start;
                        size_t newcapacity = (!capacity()) ? 1 : capacity() * 2;
                        reserve(newcapacity);

                        pos = _start + oldsize;
                    }
                    iterator end = _finish - 1;
                    while (end >= pos) {//[pos，_finish]数据后移
                        *(end + 1) = *end;
                        --end;
                    }

                    *pos = x;
                    ++_finish;
                    return pos;
                }
                iterator erase(iterator pos) {
                    assert(pos != nullptr);
                    if (pos == this->end()) 
                        pop_back();
                    else {
                        iterator begin = pos + 1;
                        while (begin != _finish) {//[pos+1,_finish]数据前移
                            *(begin - 1) = *begin;
                            ++begin;
                        }
                    }
                    --_finish;
                    return pos;
                }
                iterator find(iterator start, iterator end,T val) {
                    while (start != end) {
                        if (*start == val) 
                            return start;
                        ++start;
                    }
                    return nullptr;
                }
    private:
        iterator _start; // 指向数据块的开始
        iterator _finish; // 指向有效数据的尾
        iterator _endOfStorage; // 指向存储容量的尾
    };
}
template <class T>
void PrintVector(const BL::vector<T>& v){
    auto t = v.cbegin();
    while (t != v.cend()) {
        cout << *t << " ";
        ++t;
    }
    cout << endl;
}
void TestVectorCreat() {//构造测试:本vector暂不支持非内置类型数据，都是浅拷贝
    BL::vector<int> v1;//默认构造
    BL::vector<int> v2(4, 100);//n个初值构造
    PrintVector(v2);

    BL::vector<int> v3(v2.begin(), v2.end());//迭代器区间构造
    PrintVector(v3);

    int array[] = { 1,2,3,4,5,6,7,8,9,0 };
    BL::vector<int> v4(array, array + sizeof(array) / sizeof(array[0]));//区间构造
    PrintVector(v4);
}
void TestPP() {
    BL::vector<int> v1;
    v1.push_back(1);
    PrintVector(v1);
    v1.push_back(2);
    v1.push_back(3);
    v1.push_back(4);
    v1.push_back(2);
    PrintVector(v1);
    v1.push_back(1);
    PrintVector(v1);
    v1.push_back(3);
    v1.push_back(4);
    v1.push_back(2);
    v1.push_back(3);
    v1.push_back(4);
    v1.push_back(2);
    v1.push_back(3);
    v1.push_back(4);

    auto it = v1.begin();
    while (it != v1.end()) {//修改数据
        *it *= 2;
        ++it;
    }
    PrintVector(v1);

    for (auto e : v1)
        cout << e << " ";
    cout << endl;

    v1.pop_back();
    v1.pop_back();
    v1.pop_back();
    PrintVector(v1);

    v1.pop_back();
    PrintVector(v1);
}
void TestInsertAndErase(){
    int nums[] = {1,2,3,4};
    BL::vector<int> v(nums, nums + sizeof(nums) / sizeof(nums[0]));
    PrintVector(v);
    auto pos = v.find(v.begin(), v.end(), 3);
    v.insert(pos, 30);
    pos = v.find(v.begin(), v.end(), 3);
    v.insert(pos, 30);
    pos = v.find(v.begin(), v.end(), 1);
    v.insert(pos, -1);
    pos = v.find(v.begin(), v.end(), 4);
    v.insert(pos, 70);
    PrintVector(v);

    pos = v.find(v.begin(), v.end(), 3);
    v.erase(pos);
    PrintVector(v);

    pos = v.find(v.begin(), v.end(), 30);
    v.erase(pos);
    PrintVector(v);
    pos = v.find(v.begin(), v.end(), 30);
    v.erase(pos);
    PrintVector(v);
    pos = v.find(v.begin(), v.end(), 70);
    v.erase(pos);
    PrintVector(v);
    v.erase(v.begin());
    PrintVector(v);
    v.erase(v.end()-1);
    PrintVector(v);
}
int main() {
    //TestVectorCreat();
    //TestPP();
    TestInsertAndErase();
    return 0;
}