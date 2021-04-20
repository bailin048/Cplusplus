#include <iostream>
#include <stdio.h>
#include<vector>
#include <algorithm>
#include<functional>
#pragma warning(disable:4996)
using namespace std;

namespace BL{
    template <class T, class Container = vector<T>, class Compare = less<T> >
    class priority_queue{
    public:
        priority_queue() {}
        template <class InputIterator>
        priority_queue(InputIterator first, InputIterator last):c(first,last) {
            int curpos = c.size() / 2 - 1;
            while (curpos >= 0) 
                _AdjustDown(curpos--);
        }
        bool empty() const { return c.empty(); }
        size_t size() const { return c.size(); }
        T& top() const { return c.front(); }
        void push(const T& x) { 
            c.push_back(x);
            _AdjustUp(c.size() - 1);
        }
        void pop() {
            if (c.empty())
                return;
            swap(c.front(), c.back());
            cout << c.back() << endl;
            c.pop_back();
            _AdjustDown(0);
        }
    private:
        void _AdjustDown(int parent) {
            int n = c.size();
            if (!n)
                return;
            int tmp = c[parent];
            int child = parent * 2 + 1;
            while (child < n) {
                if (child + 1 < n && comp(c[child], c[child + 1]))
                    ++child;
                if (!comp(c[parent], c[child]))
                    break;
                c[parent] = c[child];
                parent = child;
                child = parent * 2 + 1;
            }
            c[parent] = tmp;
        }
        void _AdjustUp(int child) {
            int tmp = c[child];
            int parent = (child-1) / 2;
            while (child>0) {
                if (comp(tmp,c[parent]))
                    break;
                c[child] = c[parent];
                child = parent;
                parent = (child - 1) / 2;
            }
            c[child] = tmp;
        }
    private:
        Container c;
        Compare comp;
    };
    template<class T, class Con = deque<T>>
    class stack {
    public:
        stack() {}
        void push(const T& x) { _c.push_back(x); }
        void pop() { _c.pop_back(); }
        T& top() { return _c.back(); }
        const T& top()const { return _c.back(); }
        size_t size()const { return _c.size(); }
        bool empty()const { return _c.empty(); }
    private:
        Con _c;
    };
    template<class T, class Con = deque<T>>
    class queue {
    public:
        queue() {}
        void push(const T& x) { _c.push_back(x); }
        void pop() { _c.pop_front(); }
        T& back() { return _c.back(); }
        const T& back()const { return _c.back(); }
        T& front() { return _c.front(); }
        const T& front()const { return _c.front(); }
        size_t size()const { return _c.size(); }
        bool empty()const { return _c.empty(); }
    private:
        Con _c;
    };
};
