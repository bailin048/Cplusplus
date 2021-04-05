#include <iostream>
#include <vld.h>
#pragma warning(disable:4996)
using namespace std;

namespace BL
{
    // List的节点类
    template<class T>
    struct ListNode{
        ListNode(const T& val = T()):_pPre(nullptr),
            _pNext(nullptr),
            _val(val) {}
        ListNode<T>* _pPre;
        ListNode<T>* _pNext;
        T _val;
    };

    //List的迭代器类
    template<class T, class Ref, class Ptr>
    class ListIterator {
        typedef ListNode<T>* PNode;
        typedef ListIterator<T, Ref, Ptr> Self;
    public:
        ListIterator(PNode pNode = nullptr):_pNode(pNode) {}
        ListIterator(const Self& l): _pNode(l._pNode){}
        T& operator*() { return _pNode->_val;}
        T* operator->() { return &(operator*()); }
        Self& operator++() { 
            _pNode = _pNode->_pNext;//后移
            return *this;
        }
        Self operator++(int) {
            Self tmp(*this);
            _pNode = _pNode->_pNext;
            return tmp;
        }
        Self& operator--() {
            _pNode = _pNode->_pPre;
            return *this;
        }
        Self& operator--(int) {
            Self tmp(*this);
            _pNode = _pNode->_pPre;
            return tmp;
        }
        bool operator!=(const Self& l) { return _pNode != l._pNode; }
        bool operator==(const Self& l) { return _pNode == l._pNode; }
        PNode MyNode() { return _pNode; }
    private:
        PNode _pNode;
    };

    //list类
    template<class T>
    class list{
        typedef ListNode<T> Node;
        typedef Node* PNode;
    public:
        typedef ListIterator<T, T&, T*> iterator;
        typedef ListIterator<T, const T&, const T&> const_iterator;
    public:
        ///////////////////////////////////////////////////////////////
        // List的构造
        list() { CreateHead(); }
        list(int n, const T& value = T()) { 
            CreateHead();
            for (int i = 0; i < n; ++i)
                push_back(value);
        }
        template <class Iterator>
        list(Iterator first, Iterator last) {
            CreateHead();
            while (first != last) {
                push_back(*first);
                ++first;
            }
        }
        list(const list<T>& l) {
            CreateHead();
            list<T> tmp(l.begin(), l.end());
            this->swap(tmp);//头指针交换
        }
        list<T>& operator=(const list<T> l) {
            if (this != &l) {
                list<T> tmp(l.begin(), l.end());
                this->swap(tmp);
            }
            return *this;
        }
        ~list() {
            clear();
            delete _pHead;
            _pHead = nullptr;
        }
        ///////////////////////////////////////////////////////////////
        //// List Iterator
        iterator begin() { return iterator(_pHead->_pNext); }
        iterator end() { return iterator(_pHead); }
        const_iterator begin()const { return const_iterator(_pHead->_pNext); }
        const_iterator end()const { return const_iterator(_pHead); }
        /////////////////////////////////////////////////////////////////
        //// List Capacity`
        size_t size()const {
            size_t count = 0;
            auto  p = begin();
            while (p != end()) 
                ++count, ++p;
            return count;
        }
        bool empty()const { return !size(); }
        //////////////////////////////////////////////////////////////
        //// List Access
        T& front() { return *begin(); }
        const T& front()const { return *begin(); }
        T& back() { return *--end(); }
        const T& back()const {return *--end(); }
        //////////////////////////////////////////////////////////////
        //// List Modify
        void push_back(const T& val) { insert(end(), val); }
        void pop_back() { erase(--end()); }
        void push_front(const T& val) { insert(begin(), val); }
        void pop_front() { erase(begin()); }
        //// 在pos位置前插入值为val的节点
        iterator insert(iterator pos, const T& val) {
            PNode q = new Node(val);
            PNode cur = pos.MyNode();//插入位置的节点指针获取
            q->_pNext = cur;//插入新节点
            q->_pPre = cur->_pPre;
            q->_pPre->_pNext = q;
            cur->_pPre = q;
            return iterator(q);//返回新节点的迭代器
        }
        //// 删除pos位置的节点，返回该节点的下一个位置
        iterator erase(iterator pos) {
            PNode cur = pos.MyNode();//当前节点获取
            PNode rear = cur->_pNext;//下一节点
            rear->_pPre = cur->_pPre;//下一节点的前驱设置
            cur->_pPre->_pNext = rear;//前一节点的后驱设置
            delete cur;//删除当前节点
            return iterator(rear);//返回下一节点的迭代器
        }
        void clear() { 
            auto p = begin();
            while (p != end())
                p = erase(p);//删除并更新迭代器
        }
        void swap(list<T>& l) { 
            PNode p = l._pHead;
            l._pHead = _pHead;
            _pHead = p;//交换【头指针】，并不是节点 
        }
    private:
        void CreateHead() { 
            _pHead = new Node;
            _pHead->_pPre = _pHead;
            _pHead->_pNext = _pHead;
        }
        PNode _pHead;
    };
};
template<class T>
void PrintList(const BL::list<T>& l) {
    auto it = l.begin();
    while (it != l.end()) {
        cout << *it << "->";
        ++it;
    }
    cout << endl;
}
void TestListCreat() {
    BL::list<int> L1;//无参构造
    BL::list<int> L2(10,5);//常值初始化构造
    cout << "L2:";
    PrintList(L2);

    int nums[] = { 1,2,3,4,5,6,7,8,9,10 };
    BL::list<int> L3(nums,nums+sizeof(nums)/sizeof(nums[0]));//区间构造
    cout << "L3:";
    PrintList(L3);

    BL::list<int> L4(L3);//拷贝构造
    cout << "L4:";
    PrintList(L4);

    L1 = L4;//赋值测试
    cout << "L1:";
    PrintList(L1);

}
void TestPP() {
    BL::list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    PrintList(l);

    l.pop_back();
    l.pop_back();
    PrintList(l);

    l.pop_back();
    l.push_front(1);
    l.push_front(2);
    l.push_front(3);
    PrintList(l);

    l.pop_front();
    l.pop_front();
    PrintList(l);
}
void TestInsertAndErase() {
    int array[] = { 1,2,3,4,5 };
    BL::list<int> L(array, array + sizeof(array) / sizeof(array[0]));

    auto pos = L.begin();
    L.insert(L.begin(), 0);
    PrintList(L);

    pos = L.begin();
    L.insert(pos, 2);
    PrintList(L);

    pos = L.begin();
    L.erase(L.begin());
    PrintList(L);

    pos = L.begin();
    cout << *pos << endl;

    auto it = L.begin();
    while (it != L.end())
        it = L.erase(it);
    cout << L.size() << endl;
}
int main() {
    //TestListCreat();
    //TestPP();
    TestInsertAndErase();
    return 0;
    
}