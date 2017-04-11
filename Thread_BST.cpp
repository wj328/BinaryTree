#include <iostream>
using namespace std;

//线索二叉树
#define END '#'
typedef char Elemtype;
typedef enum{LINK = 0, THREAD = 1}PointTag;

typedef struct BiThrNode
{
	struct BiThrNode *leftchild;
	struct BiThrNode *rightchild;
	PointTag LTag;
	PointTag Rtag;
	Elemtype data;
}BiThrNode, *PThreadTree;

BiThrNode* BuyNode()
{
	BiThrNode *s = (BiThrNode*)malloc(sizeof(BiThrNode));
	if(NULL == s) exit(1);
	memset(s,0,sizeof(BiThrNode));
	return s;
}
BiThrNode* CreateTree(Elemtype *&str)
{
	BiThrNode *s = NULL;
	if(*str != END)
	{
		s = BuyNode();
		s->data = *str;
		s->LTag = s->LTag = LINK;
		s->leftchild = CreateTree(++str);
		s->rightchild = CreateTree(++str);
	}
	return s;
}
void InOrder(BiThrNode *ptr)
{
	if(ptr != NULL)
	{
		InOrder(ptr->leftchild);
		cout<<ptr->data<<" ";
		InOrder(ptr->rightchild);
	}
}

void Make(BiThrNode *p, BiThrNode *&ptr)
{//ptr为前一个结点，p为后一个节点
	if(p != NULL)
	{
		Make(p->leftchild, ptr);
		if(p->leftchild == NULL)
		{
			p->LTag = THREAD;
			p->leftchild = ptr;
		}
		if(ptr != NULL && ptr->rightchild == NULL)
		{
			ptr->Rtag = THREAD;
			ptr->rightchild = p;
		}
		ptr = p;
		Make(p->rightchild, ptr);
	}
}
void MakeThread(BiThrNode *p)
{
	BiThrNode *ptr = NULL;
	Make(p, ptr);
	ptr->rightchild = NULL;
	ptr->Rtag = THREAD;
}
BiThrNode *first(BiThrNode *p)
{
	while(p != NULL && p->LTag != THREAD)
	{
		p = p->leftchild;
	}
	return p;
}
BiThrNode *next(BiThrNode *p)
{
	if (p == NULL) return NULL;
	if(p->Rtag == THREAD)
	{
		return p->rightchild;
	}else
	{
		return first(p->rightchild);
	}
}

BiThrNode *last(BiThrNode *p)
{
	while(p != NULL && p->Rtag != THREAD)
	{
		p = p->rightchild;
	}
	return p;
}
BiThrNode *prev(BiThrNode *p)
{
	if(p == NULL) return NULL;
	if(p->LTag == THREAD)
	{
		return p->leftchild;
	}else
	{
		return last(p->leftchild);
	}
}

void NiceInorder(BiThrNode *ptr)
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
	for(BiThrNode *p=first(ptr); p!=NULL; p=next(p))
	{
		cout<<p->data<<" ";
	}
	cout<<endl;
}
void ResNiceInorder(BiThrNode *ptr)
{
	for(BiThrNode *p=last(ptr); p!=NULL; p=prev(p))
	{
		cout<<p->data<<" ";
	}
	cout<<endl;
}
void main()
{
	PThreadTree root;
	Elemtype *str="ABC##DE##F##G#H##";
	root = CreateTree(str);
	
	InOrder(root); cout<<endl;
	MakeThread(root);
	NiceInorder(root);
	ResNiceInorder(root);
}

#if 0
/*
搜索二叉树 BST树 C++
*/
template <typename Type>
class BSTTree
{
protected:
	struct BstNode
	{
		BstNode *parent;
		BstNode *leftchild;
		BstNode *rightchild;
		Type key;
	};
public:
	BSTTree():root(NULL){}
	~BSTTree(){}

	BstNode* FindValue(const Type &kx)const
	{
		return FindValue(root, kx);
	}
	BstNode* SerchValue(const Type &kx)const
	{
		return FindValue(root, kx);
	}
	void NiceInOrder()
	{
		for(BstNode *p = First(root); p != NULL; p = Next(p))
		{
			cout<<p->key<<" ";
		}
		cout<<endl;
	}
	void InOrder()
	{
		InOrder(root);
	}
	bool Insert(const Type &kx)
	{
		return Insert(root, NULL, kx);
	}
	bool Remove(const Type &kx)
	{
		return Remove(root, kx);
	}
private:
	BstNode *root;
private:
	static void InOrder(BstNode *p)
	{
		if (p != NULL)
		{
			InOrder(p->leftchild);
			cout<<p->key<<" ";
			InOrder(p->rightchild);
		}
	}
	static bool Insert(BstNode *&p, BstNode *pa, const Type &kx)
	{
		bool res = false;
		if (p == NULL)
		{
			p = BuyNode();
			p->key = kx;
			p->parent = pa;
			res = true;
		}else if (kx < p->key)
		{
			res = Insert(p->leftchild, p, kx);
		}
		else if (kx > p->key)
		{
			res = Insert(p->rightchild, p, kx);
		}
		return res;
	}

	static bool Remove(BstNode *&p, const Type &kx)
	{
		bool res = false;
		if (p == NULL) return res;

		if (kx < p->key)
		{
			res = Remove(p->leftchild, kx);
		}else if (kx > p->key)
		{
			res = Remove(p->rightchild, kx);
		}else if (p->leftchild != NULL && p->rightchild != NULL)
		{
			BstNode *s = Next(p);
			p->key = s->key;
			res = Remove(p->rightchild, s->key);//(p, p->key)无限递归
		}else
		{
			BstNode *child = p->leftchild!=NULL?p->leftchild:p->rightchild;
			if (child != NULL) child->parent = p->parent;
			BstNode *q = p;	
			p = child;
			FreeNode(q);
			res = true;
		}
		return res;
	}

	static BstNode* BuyNode()
	{
		BstNode *s = (BstNode*)malloc(sizeof(BstNode));
		if (NULL == s) exit(1);
		memset(s, 0, sizeof(BstNode));
		return s;
	}

	static void FreeNode(BstNode *ptr)
	{
		free(ptr);
	}

	//循环查找
	static BstNode* FindValue(BstNode *ptr, Type kx)
	{
		while(ptr != NULL && kx != ptr->key)
		{
			ptr = kx<ptr->key?ptr->leftchild:ptr->rightchild;
		}
		return ptr;
	}
	//递归查找
	static BstNode* SerchValue(BstNode *ptr, Type kx)
	{
		if (ptr == NULL || kx == ptr->key) {return ptr;}
		if (kx < ptr->key)
		{
			return SerchValue(ptr->leftchild, kx);
		}
		else
		{
			return SerchValue(ptr->rightchild, kx);
		}
	}
	
	static BstNode* First(BstNode *p)
	{
		while (p != NULL && p->leftchild != NULL)
		{
			p = p->leftchild;
		}
		return p;
	}

	static BstNode* Next(BstNode *p)
	{
		if (p == NULL) return NULL;
		if (p->rightchild != NULL)
		{
			return First(p->rightchild);
		}
		else
		{
			BstNode *pa = p->parent;
			while(pa != NULL && p != pa->leftchild)
			{
				p = pa;
				pa = p->parent;
			}
			return pa;
		}
	}
};

void main()
{
	//int ar[] = {53,17,78,9,45,65,87,23,81,94,88,100,17,23};
	int ar[] = {53,17,78,9,45,65};
	int n = sizeof(ar)/sizeof(ar[0]);
	
	BSTTree<int> myt;
	for(int i=0; i<n; ++i)
	{
		cout<<myt.Insert(ar[i]);
	}
	cout<<endl;
	myt.NiceInOrder();
	myt.InOrder(); cout<<endl;

	int x = 0;
	while (x != -1)
	{
		cin>>x;
		cout<<myt.Remove(x)<<"->";
		myt.NiceInOrder();cout<<endl;
	}
}

//搜索二叉树BST
typedef int KeyType;

typedef struct BstNode
{
	BstNode *parent;
	BstNode *leftchild;
	BstNode *rightchild;
	KeyType key;
}BstNode, *BSTree;

BstNode* BuyNode()
{
	BstNode *s = (BstNode*)malloc(sizeof(BstNode));
	if (NULL == s) exit(1);
	memset(s, 0, sizeof(BstNode));
	return s;
}

void FreeNode(BstNode *ptr)
{
	free(ptr);
}

//循环查找
BstNode* FindValue(BstNode *ptr, KeyType kx)
{
	while(ptr != NULL && kx != ptr->key)
	{
		ptr = kx<ptr->key?ptr->leftchild:ptr->rightchild;
	}
	return ptr;
}
//递归查找
BstNode* SerchValue(BstNode *ptr, KeyType kx)
{
	if (ptr == NULL || kx == ptr->key) {return ptr;}
	if (kx < ptr->key)
	{
		//ptr = SerchValue(ptr->leftchild, kx);//瘸腿程序
		return SerchValue(ptr->leftchild, kx);
	}
	else
	{
		//ptr = SerchValue(ptr->rightchild, kx);
		return SerchValue(ptr->rightchild, kx);
	}
}

bool Insert_BST(BSTree *ptree, KeyType kx)
{
	if (ptree == NULL) return false;
	/*
	if (*ptree == NULL)//空树
	{
		BstNode *s = BuyNode();
		s->key = kx;
		*ptree = s;
		return true;
	}
	*/
	BstNode *pa = NULL; 
	BstNode *p = *ptree;
	while (p != NULL && kx != p->key)
	{
		pa = p;
		p = kx<p->key?p->leftchild:p->rightchild;
	}
	if (p != NULL && kx == p->key) return false;//排除相同结点,没有的话会改变原来的该节点子树
	p = BuyNode();
	p->key = kx;
	p->parent = pa;

	if (pa == NULL)
	{
		*ptree = p;
	} //空树 必须是else if否则pa->key会挂掉
	else if (p->key < pa->key)
	{
		pa->leftchild = p;
	}
	else
	{
		pa->rightchild = p;
	}
	return true;
}

void InOrder(BstNode *p)
{
	if (p != NULL)
	{
		InOrder(p->leftchild);
		cout<<p->key<<" ";
		InOrder(p->rightchild);
	}
}

BstNode* First(BstNode *p)
{
	while (p != NULL && p->leftchild != NULL)
	{
		p = p->leftchild;
	}
	return p;
}

BstNode* Next(BstNode *p)
{
	if (p == NULL) return NULL;
	if (p->rightchild != NULL)
	{
		return First(p->rightchild);
	}
	else
	{
		BstNode *pa = p->parent;
		while(pa != NULL && p != pa->leftchild)
		{
			p = pa;
			pa = p->parent;
		}
		return pa;
	}
}

void NiceInOrder(BstNode *p)
{
	for(p = First(p); p != NULL; p = Next(p))
	{
		cout<<p->key<<" ";
	}
	cout<<endl;
}

BstNode* Last(BstNode *p)
{
	while (p != NULL && p->rightchild != NULL)
	{
		p = p->rightchild;
	}
	return p;
}

BstNode* Prev(BstNode *p)
{
	if (p == NULL){return NULL;}
	if (p->leftchild != NULL)
	{
		return Last(p->leftchild);
	}
	else
	{
		BstNode *pa = p->parent;
		while(pa != NULL && p != pa->rightchild)
		{
			p = pa;
			pa = p->parent;
		}
		return pa;
	}
}

void ResNiceInOrder(BstNode *p)
{
	for(p = Last(p); p != NULL; p = Prev(p))
	{
		cout<<p->key<<" ";
	}
	cout<<endl;
}

bool Remove_BST(BSTree *ptree, KeyType kx)
{
	if (ptree == NULL) return false;
	if (*ptree == NULL) return false;
	
	BstNode *pa = NULL;
	BstNode *p = *ptree;
	while (p != NULL && p->key != kx)
	{
		pa = p;
		p = kx<p->key?p->leftchild:p->rightchild;
	}
	if (p == NULL) return false;//未找到

	if (p->leftchild != NULL && p->rightchild != NULL)//双分支
	{
		BstNode *s = Next(p);
		p->key = s->key;
		p = s;
		pa = p->parent;
	}

	BstNode *child = p->leftchild != NULL?p->leftchild:p->rightchild;//单分支
	if (child != NULL){child->parent = pa;}

	if (pa == NULL)//根
	{
		*ptree = child;
	}
	else
	{
		if (pa->leftchild == p)
		{
			pa->leftchild = child;
		}
		else
		{
			pa->rightchild = child;
		}
	}
	FreeNode(p);
	return true;
} 

void main()
{
	KeyType ar[] = {53,17,78,9,45,65,87,23,81,94,88,100,17,23};
	int n = sizeof(ar)/sizeof(ar[0]);

	BstNode *root = NULL;
	for(int i=0; i<n; ++i)
	{
		cout<<Insert_BST(&root, ar[i]);
	}
	cout<<endl;

	BstNode *p = SerchValue(root, 17);
	cout<<p->key<<endl;

	InOrder(root);cout<<endl;
	int x = 0;
	while (x != -1)
	{
		cin>>x;
		cout<<Remove_BST(&root, x)<<"->";
		InOrder(root);cout<<endl;
	}
	NiceInOrder(root);
	ResNiceInOrder(root);
}
#endif