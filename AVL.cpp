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
						RotateR(parent);
					else//左右双旋(<)
						RotateLR(parent);
				}
				else {
					if (p->bf > 0)//左旋(\)
						RotateL(parent);
					else//右左双旋(>)
						RotateRL(parent);
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
	bool Remove(Node*& t, const Ty& key) {
		Node* parent = nullptr;
		Node* p = t, * q = nullptr;
		stack<Node*> st;
		while (p) {		//先找到节点，存储轨迹
			if (p->val == key)
				break;
			parent = p;
			st.push(parent);
			if (key > p->val)
				p = p->right;
			else
				p = p->left;
		}
		if (p==nullptr)//删除目标不存在
			return false;
		//删除节点
		if (p->left && p->right) {//删除目标左右皆有
			parent = p;
			q = p->left;
			st.push(parent);
			while (q->right) {
				parent = q;
				st.push(parent);
				q = q->right;
			}
			p->val = q->val;
			p = q;//转化为删前驱
		}
		//p是删除目标,q是删除目标的子女节点
		if (p->left)
			q = p->left;
		else
			q = p->right;
		bool Isleft = false;
		if (parent==nullptr)//删除目标是整棵AVL的根节点
			t = q;
		else {
			//断开p节点，链接它的子女q
			if (parent->left == p) { //删除目标是左孩子
				parent->left = q;
				Isleft = true;
			}
			else//删除目标是右孩子
				parent->right = q;
		}
		//调整平衡
		bool isbanlanced = false;
		while (!st.empty()) {
			parent = st.top();
			st.pop();
			if(Isleft)
				++parent->bf;
			else
				--parent->bf;
			if (parent->bf == 1 || parent->bf == -1)//平衡
				break;
			if (0 == parent->bf) //向上回溯调整
				q = parent;
			else {//parent失衡
				if (parent->bf > 0)//令q指向较高的树
					q = parent->right;
				else
					q = parent->left;
				if (q->bf == 0){//单旋转
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
				//重新链接
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
		t = subL->right;//子节点将变父节点
		subL->right = t->left;//子节点的左树成为父节点的右树
		t->left = subL;//父节点左旋
		subL->bf = t->bf = 0;
	}
	void RotateR(Node*& t) {
		Node* subR = t;
		t = subR->left;//子节点将变父节点
		subR->left= t->right;//子节点的右树成为父节点的左树
		t->right = subR;//父节点右旋
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