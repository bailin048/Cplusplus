#include <iostream>
#include <vector>
using namespace std;

template<class Type>
class BSTree;//声明

template<class Type>
class BSTNode {
	friend class BSTree<Type>;
public:
	BSTNode() :val(Type()), leftchild(nullptr), rightchild(nullptr) {}
	BSTNode(Type _val, BSTNode* _left = nullptr, BSTNode* _right = nullptr) :
		val(_val), leftchild(_left), rightchild(_right) {}
	~BSTNode() {}
private:
	BSTNode* leftchild;
	BSTNode* rightchild;
	Type val;
};

template<class Type>
class BSTree {
public:
	BSTree() :root(nullptr) {}
	BSTree(vector<Type>& v) :root(nullptr) {
		for (const auto& e : v)
			Insert(e);
	}
public:
	bool Insert(const Type& data) { return Insert(root, data); }
	BSTNode<Type>* Min()const { return Min(root); }
	BSTNode<Type>* Max()const { return Max(root); }
	BSTNode<Type>* Search(const Type& key)const { return Search(root, key); }
	void Inorder()const { Inorder(root); }
	BSTNode<Type>* Parent(BSTNode<Type>* key)const { return Parent(root, key); }
	bool Remove(const Type& key) { return Remove(root, key); }
protected:
	bool Insert(BSTNode<Type>*& root, const Type& data) {
		if (!root) {
			root = new BSTNode<Type>(data);
			return true;
		}
		if (data > root->val)
			Insert(root->rightchild, data);
		else if (data < root->val)
			Insert(root->leftchild, data);
		return false;
	}
	BSTNode<Type>* Min(BSTNode<Type>* root) {
		while (root && root->leftchild)
			root = root->leftchild;
		return root;
	}
	BSTNode<Type>* Max(BSTNode<Type>* root) {
		while (root && root->rightchild)
			root = root->rightchild;
		return root;
	}
	BSTNode<Type>* Search(BSTNode<Type>* root, const Type& key) {
		if (!root)
			return root;
		if (root->val > key)
			return Search(root->leftchild, key);
		else if (root->val < key)
			return Search(root->rightchild, key);
		return root;
	}
	void Inorder(BSTNode<Type>* root)const {
		if (root) {
			Inorder(root->leftchild);
			cout << root->val << " ";
			Inorder(root->rightchild);
		}
	}
	BSTNode<Type>* Parent(BSTNode<Type>* root, BSTNode<Type>* key) {
		if (!root || key == root)
			return nullptr;
		if (key == root->leftchild || key == root->rightchild)
			return root;
		if (key->val > root->key)
			return Parent(root->rightchild, key);
		else if (key->val < root->val)
			return Parent(root->leftchild, key);
		return root;
	}
	//bool Remove(BSTNode<Type>*& root, const Type& key) {
	//	//先找到节点
	//	if (!root)//不存在
	//		return false;
	//	if (key > root->val)//进右树找
	//		return Remove(root->rightchild, key);
	//	else if (key < root->val)//进左树找
	//		return Remove(root->leftchild, key);
	//	else {//找到
	//		BSTNode<Type>* p = nullptr;
	//		if (root->leftchild && root->rightchild) {//存在左右子树
	//			p = root->leftchild;
	//			while (p->rightchild) {
	//				p = p->rightchild;
	//			}
	//			root->val = p->val;
	//			Remove(root->leftchild, p->val);
	//		}
	//		else {//有左无右，有右无左，无左无右
	//			p = root;//保存删除目标
	//			if (root->leftchild)//将左树迁移到本节点
	//				root = root->leftchild;
	//			else//将右树迁移到本节点
	//				root = root->rightchild;
	//			delete p;
	//		}
	//		return true;
	//	}
	//}
	bool Remove(BSTNode<Type>*& t, const Type& key) {
		if (!t)//空树
			return false;
		BSTNode<Type>* p = t, * pr = nullptr;//pr为p的父节点
		while (p)//寻找删除目标
		{
			if (key == p->val)//找到
				break;
			pr = p;//父节点更新
			if (key < p->val)//进左树寻找
				p = p->leftchild;
			else//进右树寻找
				p = p->rightchild;
		}

		if (p->leftchild && p->rightchild) {//左右均有
			BSTNode<Type>* q = p->leftchild;//进左树
			while (q->rightchild) {//找左树的最大值
				pr = q;
				q = q->rightchild;
			}
			p->val = q->val;//值覆盖
			p = q;//转化为删除叶子节点
		}

		if (!pr)//没有父节点，说明删除目标是整棵树的根节点
		{
			if (p->leftchild)//左树存在
				t = p->leftchild;//左树迁移
			else
				t = p->rightchild;//右树迁移
		}
		else {
			if (pr->leftchild == p) {//删除目标是父节点的左孩子
				if (p->leftchild)//有左无右
					pr->leftchild = p->leftchild;
				else//有右无左
					pr->leftchild = p->rightchild;
			}
			else {//删除节点是父节点的右孩子
				if (p->leftchild)//有左无右
					pr->rightchild = p->leftchild;
				else//有右无左
					pr->rightchild = p->rightchild;
			}
		}
		delete p;//删除目标
		return true;
	}
private:
	BSTNode<Type>* root;
};

int main()
{
	vector<int> iv{ 50,30,40,10,70,80,2,60,90 };
	BSTree<int> bst(iv);
	bst.Inorder();
	bst.Remove(50);
	bst.Inorder();
	return 0;
}