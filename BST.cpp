#include <iostream>
#include <vector>
using namespace std;

template<class Type>
class BSTree;//����

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
	//	//���ҵ��ڵ�
	//	if (!root)//������
	//		return false;
	//	if (key > root->val)//��������
	//		return Remove(root->rightchild, key);
	//	else if (key < root->val)//��������
	//		return Remove(root->leftchild, key);
	//	else {//�ҵ�
	//		BSTNode<Type>* p = nullptr;
	//		if (root->leftchild && root->rightchild) {//������������
	//			p = root->leftchild;
	//			while (p->rightchild) {
	//				p = p->rightchild;
	//			}
	//			root->val = p->val;
	//			Remove(root->leftchild, p->val);
	//		}
	//		else {//�������ң�����������������
	//			p = root;//����ɾ��Ŀ��
	//			if (root->leftchild)//������Ǩ�Ƶ����ڵ�
	//				root = root->leftchild;
	//			else//������Ǩ�Ƶ����ڵ�
	//				root = root->rightchild;
	//			delete p;
	//		}
	//		return true;
	//	}
	//}
	bool Remove(BSTNode<Type>*& t, const Type& key) {
		if (!t)//����
			return false;
		BSTNode<Type>* p = t, * pr = nullptr;//prΪp�ĸ��ڵ�
		while (p)//Ѱ��ɾ��Ŀ��
		{
			if (key == p->val)//�ҵ�
				break;
			pr = p;//���ڵ����
			if (key < p->val)//������Ѱ��
				p = p->leftchild;
			else//������Ѱ��
				p = p->rightchild;
		}

		if (p->leftchild && p->rightchild) {//���Ҿ���
			BSTNode<Type>* q = p->leftchild;//������
			while (q->rightchild) {//�����������ֵ
				pr = q;
				q = q->rightchild;
			}
			p->val = q->val;//ֵ����
			p = q;//ת��Ϊɾ��Ҷ�ӽڵ�
		}

		if (!pr)//û�и��ڵ㣬˵��ɾ��Ŀ�����������ĸ��ڵ�
		{
			if (p->leftchild)//��������
				t = p->leftchild;//����Ǩ��
			else
				t = p->rightchild;//����Ǩ��
		}
		else {
			if (pr->leftchild == p) {//ɾ��Ŀ���Ǹ��ڵ������
				if (p->leftchild)//��������
					pr->leftchild = p->leftchild;
				else//��������
					pr->leftchild = p->rightchild;
			}
			else {//ɾ���ڵ��Ǹ��ڵ���Һ���
				if (p->leftchild)//��������
					pr->rightchild = p->leftchild;
				else//��������
					pr->rightchild = p->rightchild;
			}
		}
		delete p;//ɾ��Ŀ��
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