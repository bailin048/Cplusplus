#include <iostream>
#include <vector>
#include <stack>
#include <vld.h>
using namespace std;

template<class Ty>
class AVLTree;

template<class Ty>
class AVLNode {
	friend class AVLTree<Ty>;
public:
	AVLNode() :val(Ty()), bf(0),left(nullptr),right(nullptr){}
	AVLNode(Ty _val = Ty(),int _bf = 0, AVLNode<Ty>* l = nullptr, AVLNode<Ty>* r = nullptr) :
		val(_val), bf(_bf),left(l), right(r) {}
	~AVLNode() {
		left = nullptr;
		right = nullptr;
		bf = 0;
	}
private:
	AVLNode<Ty>* left;
	AVLNode<Ty>* right;
	Ty val;
	int bf;
};

template<class Ty>
class AVLTree {
	typedef AVLNode<Ty> Node;
public:
	AVLTree() :root(nullptr) {}
	AVLTree(const vector<Ty>& v) :root(nullptr) {
		for (auto& e : v)
			Insert(e);
	}
	~AVLTree() {
		while (root!=nullptr)
			Remove(root->val);
	}
public:
	bool Insert(const Ty& data) { return Insert(root, data); }
	bool Remove(const Ty& key) { return Remove(root, key); }
	void InOrder() { return InOrder(root); }
protected:
	bool Insert(Node*& t, const Ty& data) {
		//�Ȱ�BST����ڵ㣬Ѱ�Ҳ���λ�ù����д洢�켣
		Node* parent = nullptr;
		Node* p = t;
		stack<Node*> st;
		while (p){
			if (p->val == data)//�ظ����ݲ��ɲ���
				return false;
			parent = p;
			st.push(parent);
			if (data > p->val)
				p = p->right;
			else
				p = p->left;
		}
		p = new Node(data);
		if (!parent) {//����
			t = p;
			return true;
		}
		if (p->val > parent->val)//��С�Ҵ����
			parent->right = p;
		else
			parent->left = p;
		//��������ʹBST->AVL
		while (!st.empty()) {
			parent = st.top();
			st.pop();
			if (p == parent->left)//��������
				--parent->bf;
			else
				++parent->bf;
			if (!parent->bf)//���ڵ�ƽ��
				break;
			else if (parent->bf == 1 || parent->bf == -1) 
				p = parent;//���ϻ���
			else {//���ڵ㲻ƽ�⣬��Ҫ����
				if (parent->bf < 0) {
					if (p->bf < 0)//����(/)
						RotateR(parent);
					else//����˫��(<)
						RotateLR(parent);
				}
				else {
					if (p->bf > 0)//����(\)
						RotateL(parent);
					else//����˫��(>)
						RotateRL(parent);
				}
				break;//��ƽ�⣬����
			}
		}
		//����ת���µ�ԭ��ƽ��Ľڵ㱻����ָ����Ҫ����
		if (st.empty())
			t = parent;
		else {
			Node* q = st.top();
			if (parent->val < q->val)
				q->left = parent;
			else
				q->right = parent;
		}
		return true;
	}
	bool Remove(Node*& t, const Ty& key) {
		Node* parent = nullptr;
		Node* p = t, * q = nullptr;
		stack<Node*> st;
		while (p) {		//���ҵ��ڵ㣬�洢�켣
			if (p->val == key)
				break;
			parent = p;
			st.push(parent);
			if (key > p->val)
				p = p->right;
			else
				p = p->left;
		}
		if (p==nullptr)//ɾ��Ŀ�겻����
			return false;
		//ɾ���ڵ�
		if (p->left && p->right) {//ɾ��Ŀ�����ҽ���
			parent = p;
			q = p->left;
			st.push(parent);
			while (q->right) {
				parent = q;
				st.push(parent);
				q = q->right;
			}
			p->val = q->val;
			p = q;//ת��Ϊɾǰ��
		}
		//p��ɾ��Ŀ��,q��ɾ��Ŀ�����Ů�ڵ�
		if (p->left)
			q = p->left;
		else
			q = p->right;
		bool Isleft = false;
		if (parent==nullptr)//ɾ��Ŀ��������AVL�ĸ��ڵ�
			t = q;
		else {
			//�Ͽ�p�ڵ㣬����������Ůq
			if (parent->left == p) { //ɾ��Ŀ��������
				parent->left = q;
				Isleft = true;
			}
			else//ɾ��Ŀ�����Һ���
				parent->right = q;
		}
		//����ƽ��
		bool isbanlanced = false;
		while (!st.empty()) {
			parent = st.top();
			st.pop();
			if(Isleft)
				++parent->bf;
			else
				--parent->bf;
			if (parent->bf == 1 || parent->bf == -1)//ƽ��
				break;
			if (0 == parent->bf) //���ϻ��ݵ���
				q = parent;
			else {//parentʧ��
				if (parent->bf > 0)//��qָ��ϸߵ���
					q = parent->right;
				else
					q = parent->left;
				if (q->bf == 0){//����ת
					if (parent->bf < 0){
						RotateR(parent);
						parent->bf = 1;
						parent->right->bf = -1;
					}
					else{
						RotateL(parent);
						parent->bf = -1;
						parent->left->bf = 1;
					}
					isbanlanced = true;
				}
				else {
					if (parent->bf < 0) {
						if (q->bf < 0)  //   /
							RotateR(parent);
						else           //   <
							RotateLR(parent);
					}
					else {
						if (q->bf > 0)  //   \ 
							RotateL(parent);
						else           //   >
							RotateRL(parent);
					}
				}
				//��������
				if (st.empty())
					t = parent;
				else {
					q = st.top();
					if (parent->val < q->val)
						q->left = parent;
					else
						q->right = parent;
				}
				if (isbanlanced)
					break;
			}
		}
		delete p;
		return true;
	}
	void InOrder(Node*& root) {
		if (root) {
			InOrder(root->left);
			cout << root->val << " ";
			InOrder(root->right);
		}
	}
private:
	void RotateL(Node*& t) {
		Node* subL = t;
		t = subL->right;//�ӽڵ㽫�丸�ڵ�
		subL->right = t->left;//�ӽڵ��������Ϊ���ڵ������
		t->left = subL;//���ڵ�����
		subL->bf = t->bf = 0;
	}
	void RotateR(Node*& t) {
		Node* subR = t;
		t = subR->left;//�ӽڵ㽫�丸�ڵ�
		subR->left= t->right;//�ӽڵ��������Ϊ���ڵ������
		t->right = subR;//���ڵ�����
		subR->bf = t->bf = 0;
	}
	void RotateLR(Node*& t) {
		Node* subL, * subR;
		subR = t;
		subL = subR->left;
		t = subL->right;

		subL->right = t->left;
		t->left = subL;
		if (t->bf <= 0)
			subL->bf = 0;
		else 
			subL->bf = -1;
		subR->left = t->right;
		t->right = subR;
		if (t->bf >= 0)
			subR->bf = 0;
		else
			subR->bf = 1;
		t->bf = 0;
	}
	void RotateRL(Node*& t) {
		Node* subL, * subR;
		subL = t;
		subR = subL->right;
		t = subR->left;

		subR->left =  t->right;
		t->right = subR;
		if (t->bf >= 0)
			subR->bf = 0;
		else
			subR->bf = 1;

		subL->right = t->left;
		t->left = subL;
		if (t->bf <= 0)
			subL->bf = 0;
		else
			subL->bf = -1;

		t->bf = 0;
	}
private:
	Node* root;
};
int main() {
	vector<int> iv{ 16, 3, 7 , 11, 9, 26, 18, 14, 15 };
	AVLTree<int> avl(iv);
	avl.Remove(7);
	avl.Remove(16);
	avl.Remove(3);
	avl.Remove(15);
	avl.Remove(3);
	avl.Remove(18);
	avl.Remove(14);
	avl.Remove(26);
	avl.Remove(9);
	return 0;
}