#include <vector>
#include <stack>
#include <iostream>
using namespace std;

typedef enum { RED = 0, BLACK }COLOR;
template <class Ty>
class RBTree;
template <class Ty>
class RB_iterator;

template<class Ty>
class RBNode {
	friend class RBTree<Ty>;
	friend class RB_iterator<Ty>;
public:
	RBNode() :val(Ty()), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
	RBNode(Ty _V = Ty()) :val(_V), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
	~RBNode() {
		val = Ty();
		color = RED;
		left = right = parent = nullptr;
	}
private:
	Ty val;
	COLOR color;
	RBNode* left;
	RBNode* right;
	RBNode* parent;
};

template<class Ty>
class RB_iterator {
	typedef RBNode<Ty> Node;
	typedef RB_iterator<Ty> Self;
public:
	RB_iterator(Node* Ni = nullptr, Node* c = nullptr, Node* s = nullptr,Node* e = nullptr) :
		Nil(Ni), cur(c), strn(s), endn(e) {}
public:
	Ty& operator*() { return cur->val; }
	bool operator==(const Self& it) { return cur == it.cur; }
	bool operator!=(const Self& it) { return cur != it.cur; }
	Self& operator++() {
		increment();
		return *this;
	}
	Self& operator++(int) {
		Self tmp(*this);
		increment();
		return tmp;
	}
	Self& operator--() {
		decrement();
		return *this;
	}
	Self& operator--(int) {
		Self tmp(*this);
		decrement();
		return tmp;
	}
	Ty* operator->() { return &(operator*()); }
protected:
	void increment() {
		if (endn->parent == cur) {//传进来的节点时最后一个节点
			cur = endn;
			return;
		}
		else if (cur->right != Nil) {
			cur = cur->right;
			if (cur == Nil) 
				return;
			while (cur->left != Nil)
				cur = cur->left;
		}
		else {
			Node* pr = cur->parent;
			while (cur == pr->right) {//当前节点属于右树
				cur = pr;//向上回溯
				pr = cur->parent;
			}
			if (pr->right != cur)
				cur = pr;
		}
	}
	void decrement() {
		if (cur == endn) {
			cur = cur->parent;
			return;
		}
		if (cur->left != Nil) {
			Node* p = cur->left;
			while (p->right != Nil)
				p = p->right;
			cur = p;
		}
		else {
			Node* p = cur->parent;
			while (cur == p->left) {
				cur = p;
				p = cur->parent;
			}
			cur = p;
		}
	}
	public:
	Node* Nil;
	Node* cur;
	Node* strn;
	Node* endn;
};

template <class Ty>
class RBTree {
public:
	typedef RBNode<Ty> Node;
	typedef RB_iterator<Ty> iterator;
public:
	RBTree() :root(Nil), Nil(_BuyNode(Ty())), end_node(_BuyNode()) {
		Nil->left = Nil->right = Nil->parent = nullptr;
		end_node->left = end->right = end->parent = nullptr;
		Nil->color = BLACK;
	}
	RBTree(vector<Ty> v) :root(Nil), Nil(_BuyNode(Ty())), end_node(_BuyNode(Ty())) {
		Nil->left = Nil->right = Nil->parent = nullptr;
		end_node->left = end_node->right = end_node->parent = nullptr;
		Nil->color = BLACK;
		for (auto& it : v)
			Insert(it);
	}
public:
	bool Insert(const Ty& data) { return Insert(root, data); }
	bool Remove(const Ty& key) { return Remove(root, key); }
	void InOrder() { return InOrder(root); }
protected:
	bool Insert(Node*& t, const Ty& data) {
		//先按BST插入
		Node* pr = Nil;
		Node* p = t;
		while (p != Nil) {
			if (data == p->val)
				return false;
			pr = p;
			if (data > p->val)
				p = p->right;
			else
				p = p->left;
		}
		p = _BuyNode(data);
		if (pr == Nil)
			t = p;
		else if (data < pr->val)
			pr->left = p;
		else
			pr->right = p;
		p->parent = pr;

		//调整平衡
		Insert_Fixup(t, p);
		return true;
	}
	bool Remove(Node*& t, const Ty& key) {
		//先找节点
		Node* p = t, * c;
		while (p != Nil) {
			if (p->val == key)
				break;
			if (key > p->val)
				p = p->right;
			else
				p = p->left;
		}
		if (p == Nil)//不存在
			return false;
		if (p->left != Nil && p->right != Nil) {//转化为删只有一个孩子的节点or叶子
			Node* q = p->right;
			while (q->left != Nil)
				q = q->left;
			p->val = q->val;
			p = q;
		}
		if (p->left != Nil)//取到孩子
			c = p->left;
		else
			c = p->right;
		c->parent = p->parent;
		//链接
		if (p->parent == Nil)
			t = c;
		else if (p == p->parent->left)
			p->parent->left = c;
		else
			p->parent->right = c;
		//调整平衡
		if (p->color == BLACK)
			Remove_Fixup(t, c);
		delete p;
		return true;
	}
	void InOrder(Node* t) {
		if (t != Nil) {
			InOrder(t->left);
			cout << t->val << " ";
			InOrder(t->right);
		}
	}
public:
	iterator begin() { 
		set_start_node();
		if(start_node)
			return iterator(Nil, start_node, start_node, end_node);
		return iterator(Nil, nullptr, nullptr, nullptr);
	}
	iterator end() { 
		set_end_node();
		if(end_node->parent)
			return iterator(Nil, end_node, start_node, end_node);
		return iterator(Nil, nullptr, nullptr, nullptr);
	}
	void set_start_node() { start_node = Min(); }
	void set_end_node() { end_node->parent = Max(); }
private:
	Node* _BuyNode(const Ty& data) {
		Node* node = new Node(data);
		node->left = node->right = node->parent = Nil;
		return node;
	}
	void Insert_Fixup(Node*& t, Node* x) {
		while (x->parent->color == RED) {
			Node* uncle = Nil;
			if (x->parent == x->parent->parent->left) {//左分支
				uncle = x->parent->parent->right;//叔叔
				if (uncle->color == RED) {//叔叔为红
					x->parent->color = BLACK;
					uncle->color = BLACK;
					uncle->parent->color = RED;//爷爷为黑
					x = uncle->parent;//向上回溯
				}
				else {
					if (x == x->parent->right) {
						x = x->parent;
						RotateL(t, x);
					}
					x->parent->color = BLACK;
					x->parent->parent->color = RED;
					RotateR(t, x->parent->parent);
				}
			}
			else {//右分支
				uncle = x->parent->parent->left;//叔叔
				if (uncle->color == RED) {//叔叔为红
					x->parent->color = BLACK;
					uncle->color = BLACK;
					uncle->parent->color = RED;//爷爷为黑
					x = uncle->parent;//向上回溯
				}
				else {
					if (x == x->parent->left) {
						x = x->parent;
						RotateR(t, x);
					}
					x->parent->color = BLACK;
					x->parent->parent->color = RED;
					RotateL(t, x->parent->parent);
				}
			}
		}
		t->color = BLACK;
	}
	void Remove_Fixup(Node*& t, Node* x) {
		//x==t说明x为根节点,当孩子为黑可能需要调整
		Node* w;//兄弟节点
		while (x != t && x->color == BLACK) {
			if (x == x->parent->left) {//左树
				w = x->parent->right;
				if (w->color == RED) {//兄弟为红，父亲必黑
					w->color = BLACK;
					x->parent->color = RED;
					w = w->left;//x的兄弟变了
					RotateL(t, x->parent);
				}
				//兄弟为黑
				if (w->left->color == BLACK && w->right->color == BLACK) {
					//兄弟无红子女
					w->color = RED;
					x = x->parent;
				}
				else {
					if (w->left->color == RED) {//兄弟左孩子为红，右孩子为黑
						w->color = RED;
						w->left->color = BLACK;
						w = w->left;
						RotateR(t, w->parent);
					}
					w->color = x->parent->color;
					x->parent->color = BLACK;
					w->right->color = BLACK;
					RotateL(t, x->parent);
					x = t;//设置即可结束循环
				}
			}
			else {//右树
				w = x->parent->left;
				if (w->color == RED) {
					w->color = BLACK;
					x->parent->color = RED;
					w = w->right;//x的兄弟变了
					RotateR(t, x->parent);
				}
				//兄弟为黑
				if (w->left->color == BLACK && w->right->color == BLACK) {
					//兄弟无红子女
					w->color = RED;
					x = x->parent;
				}
				else {
					if (w->right->color == RED) {//兄弟右孩子为红，左孩子为黑
						w->color = RED;
						w->right->color = BLACK;
						w = w->right;
						RotateL(t, w->parent);
					}
					w->color = x->parent->color;
					x->parent->color = BLACK;
					w->left->color = BLACK;
					RotateR(t, x->parent);
					x = t;//设置即可结束循环
				}
			}

		}
		x->color = BLACK;
	}
private:
	void RotateL(Node*& t, Node* x) {
		Node* y = x->right;
		x->right = y->left;
		if (y->left != Nil)
			y->left->parent = x;
		if (x->parent == Nil)
			t = y;
		else if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
		y->parent = x->parent;
		y->left = x;
		x->parent = y;
	}
	void RotateR(Node*& t, Node* x) {
		Node* y = x->left;
		x->left = y->right;
		if (y->right != Nil)
			y->right->parent = x;
		if (x == t)
			t = y;
		else if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
		y->parent = x->parent;
		y->right = x;
		x->parent = y;
	}
private:
	Node* Min() {
		Node* p = root;
		while (root != Nil && p->left != Nil)
			p = p->left;
		return p == Nil ? nullptr : p;
	}
	Node* Max() {
		Node* p = root;
		while (p != Nil && p->right != Nil)
			p = p->right;
		return p == Nil ? nullptr : p;
	}
private:
	Node* Nil;
	Node* root;
	Node* end_node;
	Node* start_node;
};
void TestRBTree(){//插入删除功能测试
	vector<int> iv{ 10, 7, 8, 15, 5, 6, 11, 13, 12 };
	RBTree<int> rb(iv);
	rb.InOrder(); cout << endl;
	rb.Remove(5);
	rb.InOrder(); cout << endl;
	rb.Remove(15);
	rb.InOrder(); cout << endl;
	rb.Remove(8);
	rb.InOrder(); cout << endl;
	rb.Remove(11);
	rb.InOrder(); cout << endl;
	rb.Remove(13);
	rb.InOrder(); cout << endl;
	rb.Remove(7);
	rb.InOrder(); cout << endl;
	rb.Remove(12);
	rb.InOrder(); cout << endl;
	rb.Remove(10);
	rb.InOrder(); cout << endl;
	rb.Remove(6);
	rb.InOrder(); cout << endl;
}
void TestIterator() {//迭代器功能测试
	vector<int> iv{ 10, 7, 8, 15, 5, 6, 11, 13, 12 };
	RBTree<int> rb(iv);
	rb.InOrder(); cout << endl;
	RBTree<int>::iterator it = rb.end();
	RBTree<int>::iterator ot = rb.begin();
	cout << "测试正向迭代" << endl;
	while (ot != rb.end()) {
		cout << *ot << " ";
		ot++;
	}
	cout << endl << "测试反向迭代" << endl;
	do {
		--it;
		cout << *it << " ";
	} while (it != rb.begin());
	cout << endl;
}
int main() {
	cout << "测试迭代器" << endl;
	TestIterator();
	cout << "测试删除" << endl;
	TestRBTree();
	return 0;
}

