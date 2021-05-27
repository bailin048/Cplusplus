#include <vector>
#include <stack>
#include <iostream>
using namespace std;

typedef enum { RED = 0, BLACK }COLOR;
template <class Ty>
class RBTree;

template<class Ty>
class RBNode {
	friend class RBTree<Ty>;
public:
	RBNode() :val(Ty()), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
	RBNode(Ty _V = Ty()) :val(_V), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
	~RBNode() {
		//val = Ty();
		//color = RED;
		//left = right = parent = nullptr;
	}
private:
	Ty val;
	COLOR color;
	RBNode* left;
	RBNode* right;
	RBNode* parent;
};

template <class Ty>
class RBTree {
	typedef RBNode<Ty> Node;
public:
	RBTree() :root(Nil), Nil(_BuyNode(Ty())) {
		Nil->left = Nil->right = Nil->parent = nullptr;
		Nil->color = BLACK;
	}
	RBTree(vector<Ty> v) :root(Nil), Nil(_BuyNode(Ty())) {
		Nil->left = Nil->right = Nil->parent = nullptr;
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
		//�Ȱ�BST����
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

		//����ƽ��
		Insert_Fixup(t, p);
		return true;
	}
	bool Remove(Node*& t, const Ty& key) {
		//���ҽڵ�
		Node* p = t, * c;
		while (p != Nil) {
			if (p->val == key)
				break;
			if (key > p->val)
				p = p->right;
			else
				p = p->left;
		}
		if (p == Nil)//������
			return false;
		if (p->left != Nil && p->right != Nil) {//ת��Ϊɾֻ��һ�����ӵĽڵ�orҶ��
			Node* q = p->right;
			while (q->left != Nil)
				q = q->left;
			p->val = q->val;
			p = q;
		}
		if (p->left != Nil)//ȡ������
			c = p->left;
		else
			c = p->right;
		c->parent = p->parent;
		//����
		if (p->parent == Nil)
			t = c;
		else if (p == p->parent->left)
			p->parent->left = c;
		else
			p->parent->right = c;
		//����ƽ��
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
private:
	Node* _BuyNode(const Ty& data) {
		Node* node = new Node(data);
		node->left = node->right = node->parent = Nil;
		return node;
	}
	void Insert_Fixup(Node*& t, Node* x) {
		while (x->parent->color == RED) {
			Node* uncle = Nil;
			if (x->parent == x->parent->parent->left) {//���֧
				uncle = x->parent->parent->right;//����
				if (uncle->color == RED) {//����Ϊ��
					x->parent->color = BLACK;
					uncle->color = BLACK;
					uncle->parent->color = RED;//үүΪ��
					x = uncle->parent;//���ϻ���
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
			else {//�ҷ�֧
				uncle = x->parent->parent->left;//����
				if (uncle->color == RED) {//����Ϊ��
					x->parent->color = BLACK;
					uncle->color = BLACK;
					uncle->parent->color = RED;//үүΪ��
					x = uncle->parent;//���ϻ���
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
		//x==t˵��xΪ���ڵ�,������Ϊ�ڿ�����Ҫ����
		Node* w;//�ֵܽڵ�
		while (x != t && x->color == BLACK) {
			if (x == x->parent->left) {//����
				w = x->parent->right;
				if (w->color == RED) {//�ֵ�Ϊ�죬���ױغ�
					w->color = BLACK;
					x->parent->color = RED;
					w = w->left;//x���ֵܱ���
					RotateL(t, x->parent);
				}
				//�ֵ�Ϊ��
				if (w->left->color == BLACK && w->right->color == BLACK) {
					//�ֵ��޺���Ů
					w->color = RED;
					x = x->parent;
				}
				else {
					if (w->left->color == RED) {//�ֵ�����Ϊ�죬�Һ���Ϊ��
						w->color = RED;
						w->left->color = BLACK;
						w = w->left;
						RotateR(t, w->parent);
					}
					w->color = x->parent->color;
					x->parent->color = BLACK;
					w->right->color = BLACK;
					RotateL(t, x->parent);
					x = t;//���ü��ɽ���ѭ��
				}
			}
			else {//����
				w = x->parent->left;
				if (w->color == RED) {
					w->color = BLACK;
					x->parent->color = RED;
					w = w->right;//x���ֵܱ���
					RotateR(t, x->parent);
				}
				//�ֵ�Ϊ��
				if (w->left->color == BLACK && w->right->color == BLACK) {
					//�ֵ��޺���Ů
					w->color = RED;
					x = x->parent;
				}
				else {
					if (w->right->color == RED) {//�ֵ��Һ���Ϊ�죬����Ϊ��
						w->color = RED;
						w->right->color = BLACK;
						w = w->right;
						RotateL(t, w->parent);
					}
					w->color = x->parent->color;
					x->parent->color = BLACK;
					w->left->color = BLACK;
					RotateR(t, x->parent);
					x = t;//���ü��ɽ���ѭ��
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
	Node* Nil;
	Node* root;
};

int main() {
	vector<int> iv{ 10, 7, 8, 15, 5, 6, 11, 13, 12 };
	RBTree<int> rb(iv);
	rb.InOrder(); cout << endl;
	rb.Remove(6);
	rb.InOrder(); cout << endl;
	rb.Remove(10);
	rb.InOrder(); cout << endl;
	rb.Remove(8);
	rb.InOrder(); cout << endl;
	rb.Remove(11);
	rb.InOrder(); cout << endl;
	rb.Remove(13);
	rb.InOrder(); cout << endl;
	rb.Remove(5);
	rb.InOrder(); cout << endl;
	rb.Remove(15);
	rb.InOrder(); cout << endl;
	rb.Remove(7);
	rb.InOrder(); cout << endl;
	rb.Remove(12);
	rb.InOrder(); cout << endl;

	return 0;
}