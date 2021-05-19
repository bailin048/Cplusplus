#include <iostream>
#include <vector>
#include <stack>
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
public:
	bool Insert(const Ty& data) { return Insert(root, data); }
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
						RorateR(parent);
					else//����˫��(<)
						RorateLR(parent);
				}
				else {
					if (p->bf > 0)//����(\)
						RorateL(parent);
					else//����˫��(>)
						RorateRL(parent);
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
private:
	void RorateL(Node*& t) {
		Node* subL = t;
		t = subL->right;//�ӽڵ㽫�丸�ڵ�
		subL->right = t->left;//�ӽڵ��������Ϊ���ڵ������
		t->left = subL;//���ڵ�����
		subL->bf = t->bf = 0;
	}
	void RorateR(Node*& t) {
		Node* subR = t;
		t = subR->left;//�ӽڵ㽫�丸�ڵ�
		subR->left= t->right;//�ӽڵ��������Ϊ���ڵ������
		t->right = subR;//���ڵ�����
		subR->bf = t->bf = 0;
	}
	void RorateLR(Node*& t) {
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
	void RorateRL(Node*& t) {
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
	vector<int> iv{ 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	AVLTree<int> avl(iv);
	return 0;
}

