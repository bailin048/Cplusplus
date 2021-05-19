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
		//先按BST插入节点，寻找插入位置过程中存储轨迹
		Node* parent = nullptr;
		Node* p = t;
		stack<Node*> st;
		while (p){
			if (p->val == data)//重复数据不可插入
				return false;
			parent = p;
			st.push(parent);
			if (data > p->val)
				p = p->right;
			else
				p = p->left;
		}
		p = new Node(data);
		if (!parent) {//空树
			t = p;
			return true;
		}
		if (p->val > parent->val)//左小右大插入
			parent->right = p;
		else
			parent->left = p;
		//调整树，使BST->AVL
		while (!st.empty()) {
			parent = st.top();
			st.pop();
			if (p == parent->left)//左树升高
				--parent->bf;
			else
				++parent->bf;
			if (!parent->bf)//父节点平衡
				break;
			else if (parent->bf == 1 || parent->bf == -1) 
				p = parent;//向上回溯
			else {//本节点不平衡，需要调整
				if (parent->bf < 0) {
					if (p->bf < 0)//右旋(/)
						RorateR(parent);
					else//左右双旋(<)
						RorateLR(parent);
				}
				else {
					if (p->bf > 0)//左旋(\)
						RorateL(parent);
					else//右左双旋(>)
						RorateRL(parent);
				}
				break;//已平衡，跳出
			}
		}
		//因旋转导致的原不平衡的节点被两次指向，需要调整
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
		t = subL->right;//子节点将变父节点
		subL->right = t->left;//子节点的左树成为父节点的右树
		t->left = subL;//父节点左旋
		subL->bf = t->bf = 0;
	}
	void RorateR(Node*& t) {
		Node* subR = t;
		t = subR->left;//子节点将变父节点
		subR->left= t->right;//子节点的右树成为父节点的左树
		t->right = subR;//父节点右旋
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

