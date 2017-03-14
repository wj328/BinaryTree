#include<iostream>
#include<stack>
#include<queue>
using namespace std;
// B_
#define M 5
#define MAX (M-1)
#define MIN (M/2) 

typedef char KeyType;
typedef struct {}Record;
typedef struct 
{
	KeyType key;
	Record *recptr;
}ElemType; // key_value;

typedef struct BNode
{
	int num; // 
	BNode *parent;
	ElemType data[M+1];// 0// 1 2 3 4 5  // M-1
	BNode * sub[M+1];  ///   0 1 2 3 4 5 // M
}BNode, *BTree;
typedef struct
{
	BNode *pnode;
	int   index;
	bool  tag;
}Result;

BNode * Buynode()
{
	BNode *s = (BNode*)malloc(sizeof(BNode));
	if(NULL == s) exit(1);
	memset(s,0,sizeof(BNode));
	return s;
}
void Freenode(BNode *p)
{
	free(p);
}
Result FindValue(BNode *ptr,KeyType kx)
{
	Result res = { NULL,-1,false};
	while(ptr != NULL)
	{
		int i = ptr->num;
		ptr->data[0].key = kx;
		while(kx < ptr->data[i].key) --i;
		res.pnode = ptr;
		res.index = i;
		if(i > 0 && ptr->data[i].key == kx)
		{
			res.tag = true;
			break;
		}
		ptr = ptr->sub[i];
	}
	return res;
}
Result SearchValue(BNode *ptr,KeyType kx)
{
	Result res = {NULL,-1,false};
	if(ptr != NULL)
	{
		int i = ptr->num;
		ptr->data[0].key = kx;
		while(kx < ptr->data[i].key) --i;
		res.pnode = ptr;
		res.index = i;
		if(i > 0 && ptr->data[i].key == kx)
		{
			res.tag = true;
		}else if(ptr->sub[i] != NULL)
		{
			res = SearchValue(ptr->sub[i],kx);
		}
	}
	return res;
}
BNode * MakeRoot(ElemType x,BNode *left,BNode *right)
{
	BNode *s = Buynode();
	s->num = 1;
	s->parent = NULL;
	s->data[1] = x;
	s->sub[0] = left; //
	if(left != NULL) left->parent = s;
	s->sub[1] = right; //
	if(right != NULL) right->parent = s;
	return s;
}
void Insert_Item(BNode *ptr,int pos,ElemType x,BNode *right)
{
	for(int i = ptr->num; i>pos; --i)
	{
		ptr->data[i+1] = ptr->data[i];
		ptr->sub[i+1] = ptr->sub[i];
	}
	ptr->data[pos+1] = x;
	ptr->sub[pos+1] = right; //
	if(right != NULL) right->parent = ptr;// 
	ptr->num+=1;
}
ElemType Move_Item(BNode *ptr, BNode *s, int pos)
{
	for(int i = 0,j = pos+1;j<=ptr->num;++i,++j)
	{
		s->data[i] = ptr->data[j];
		s->sub[i] = ptr->sub[j];
		if(s->sub[i] != NULL)
		{
			s->sub[i]->parent = s;
		}
	}
	s->num = MIN;
	ptr->num = MIN;
	s->parent = ptr->parent;
	return s->data[0];
}
BNode * Splice(BNode *ptr)// ptr->num > MAX;
{
	BNode *s = Buynode();
	ElemType x = Move_Item(ptr,s,MIN);
	if(ptr->parent == NULL)
	{
		return MakeRoot(x,ptr,s);
	}
	ptr = ptr->parent;
	int pos = ptr->num;
	ptr->data[0] = x;
	while(x.key < ptr->data[pos].key) --pos;
	Insert_Item(ptr,pos,x,s);
	if(ptr->num > MAX)
	{
		return Splice(ptr);
	}else
	{
		return NULL;
	}
}
bool Insert(BTree *ptree, ElemType x)
{
	if(ptree == NULL) return false;
	if(*ptree == NULL)// root
	{
		*ptree =  MakeRoot(x,NULL,NULL);
		return true;
	}
	Result res = FindValue(*ptree,x.key);
	if(res.pnode == NULL || res.tag) 
		return false;
	BNode *ptr = res.pnode;
	int pos = res.index;
	Insert_Item(ptr,pos,x,NULL);
	if(ptr->num > MAX)
	{
		BNode *newroot = Splice(ptr);
		if(newroot != NULL)
		{
			*ptree = newroot;
		}
	}
	return true;
}
void InOrder(BNode *ptr)
{
	if(ptr != NULL)
	{
		InOrder(ptr->sub[0]);
		for(int i = 1;i<=ptr->num;++i)
		{
			
			cout<<ptr->data[i].key;
			InOrder(ptr->sub[i]);
		}
	}
}
//////////////////////////////////////////////////////////
BNode * FindPrev(BNode *ptr)
{
	while(ptr != NULL && ptr->sub[ptr->num] != NULL)
	{
		ptr = ptr->sub[ptr->num];
	}
	return ptr;
}
BNode * FindNext(BNode *ptr)
{
	while(ptr != NULL && ptr->sub[0] != NULL)
	{
		ptr = ptr->sub[0];
	}
	return ptr;
}

void Del_Brch_Item(BNode *ptr, int pos)
{
	for(int i = pos; i < ptr->num; ++i)
	{
		ptr->data[i] = ptr->data[i+1];
		ptr->sub[i] = ptr->sub[i+1];
	}
	ptr->num-=1;
}
void Del_Leaf_Item(BNode * ptr,int pos)
{
	Del_Brch_Item(ptr,pos);
}
///////////////////////////////
void Rotate_Brch_Left(BNode *leftbro,BNode *ptr,BNode *pa,int pos)
{
	ptr->data[0] = pa->data[pos];
	for(int i = ptr->num+1;i > 0;--i)
	{
		ptr->data[i] = ptr->data[i-1];
		ptr->sub[i] = ptr->sub[i-1];//
	}
	ptr->num+=1;
	pa->data[pos] = leftbro->data[leftbro->num];
	ptr->sub[0] = leftbro->sub[leftbro->num];//
	leftbro->num -=1;
	if(ptr->sub[0] != NULL) 
		ptr->sub[0]->parent = ptr;//

}
void Rotate_Leaf_Left(BNode *leftbro,BNode *ptr,BNode *pa,int pos)
{
	Rotate_Brch_Left(leftbro,ptr,pa,pos);
}
/////////////////////////////////////////////
void Rotate_Brch_Right(BNode *ptr,BNode *rightbro,BNode *pa,int pos)
{
	ptr->num+=1;
	ptr->data[ptr->num] = pa->data[pos+1];
	ptr->sub[ptr->num] = rightbro->sub[0];
	if(ptr->sub[ptr->num] != NULL)
	{
		ptr->sub[ptr->num]->parent = ptr;
	}
	////////////////////////////////////////
	Del_Brch_Item(rightbro,0);
	pa->data[pos+1] = rightbro->data[0];
}
void Rotate_Leaf_Right(BNode *ptr,BNode *rightbro,BNode *pa,int pos)
{
	Rotate_Brch_Right(ptr,rightbro,pa,pos);
}
////////////////////////////////////////////////
void Merge_Brch_Left(BNode *leftbro,BNode *ptr,BNode *pa,int pos)
{
	ptr->data[0] = pa->data[pos];
	for(int i = leftbro->num+1,j = 0;j<=ptr->num;++i,++j)
	{
		leftbro->data[i] = ptr->data[j];
		leftbro->sub[i] = ptr->sub[j];
		if(leftbro->sub[i] != NULL)
			leftbro->sub[i]->parent = leftbro;
	}
	leftbro->num = leftbro->num + ptr->num + 1;
	Del_Brch_Item(pa,pos);

}
void Merge_Leaf_Left(BNode *leftbro,BNode *ptr,BNode *pa,int pos)
{
	Merge_Brch_Left(leftbro,ptr,pa,pos);
}
//////////////////////////
BNode * Adjust_Leaf(BNode *ptr);
BNode * Adjust_Brch(BNode *ptr)
{
	return Adjust_Leaf(ptr);
}
BNode * Adjust_Leaf(BNode *ptr)
{
	BNode *pa = ptr->parent;
	BNode *leftbro = NULL, *rightbro = NULL;
	int pos = pa->num;
	while(pos >= 0 && pa->sub[pos] != ptr) --pos;
	if(pos < 0) exit(1);
	if(pos > 0)
	{
		leftbro = pa->sub[pos-1];
	}
	if(pos < pa->num)
	{
		rightbro = pa->sub[pos+1];
	}
	/////////////////////////////
	if(leftbro != NULL && leftbro->num > MIN)
	{
		Rotate_Leaf_Left(leftbro,ptr,pa,pos);
	}else if(rightbro != NULL && rightbro->num > MIN)
	{
		Rotate_Leaf_Right(ptr,rightbro,pa,pos);
	}else if(leftbro != NULL)
	{
		Merge_Leaf_Left(leftbro,ptr,pa,pos);
		Freenode(ptr);
		ptr = leftbro; ///
	}else if(rightbro != NULL)
	{
		Merge_Leaf_Left(ptr,rightbro,pa,pos+1);
		Freenode(rightbro);
		rightbro = NULL;
	}
	if(pa->parent != NULL && pa->num < MIN)
	{
		return Adjust_Brch(pa);
	}else if(pa->parent == NULL && pa->num <= 0)
	{
		Freenode(pa);
		ptr->parent = NULL;
		return ptr;
	}
	return NULL;
}
bool Remove(BTree *ptree,KeyType kx)
{
	if(ptree == NULL || *ptree == NULL)
		return false;
	Result res = FindValue(*ptree,kx);
	if(res.pnode == NULL || !res.tag)
		return false;
	BNode *ptr = res.pnode;
	int    pos = res.index;
	BNode *pr = FindPrev(ptr->sub[pos-1]);
	BNode *nt = FindNext(ptr->sub[pos]);
	if(pr != NULL && pr->num > MIN)
	{
		ptr->data[pos] = pr->data[pr->num];
		ptr = pr;
		pos = pr->num;
	}else if(nt != NULL && nt->num > MIN)
	{
		ptr->data[pos] = nt->data[1];
		ptr = nt;
		pos = 1;
	}else if(pr != NULL)
	{
		ptr->data[pos] = pr->data[pr->num];
		ptr = pr;
		pos = pr->num;
	}else if(nt != NULL)
	{////   
		ptr->data[pos] = nt->data[1];
		ptr = nt;
		pos = 1;
	}
	//////////////////////////
	Del_Leaf_Item(ptr,pos);
	if(ptr->parent != NULL && ptr->num < MIN)
	{
		BNode * newroot = Adjust_Leaf(ptr);
		if(newroot != NULL)
		{
			*ptree = newroot;
		}
	}else if(ptr->parent == NULL && ptr->num <= 0)
	{
		Freenode(ptr);
		*ptree = NULL;
	}
	return true;
}
void main()
{
	KeyType ar[]={"qw9erty8uio7pas2dfg3hjk1l5zx4cv6bnm"};
	int n = sizeof(ar)/sizeof(ar[0]) -1;
	ElemType item;
	BTree root = NULL;
	for(int i = 0;i<n; ++i)
	{
		item.key = ar[i];
		item.recptr = NULL;
		cout<<Insert(&root,item);
	}
	cout<<endl;
	InOrder(root);
	cout<<endl;
	KeyType kx;
	while(cin>>kx, kx != '#')
	{
		cout<<Remove(&root,kx)<<" : ";
		InOrder(root);
		cout<<endl;
	}
}

/*
template<class Type>
class AVLTree
{
protected:
	struct AVLNode
	{
		AVLNode *leftchild;
		AVLNode *rightchild;
		AVLNode *parent;
		int balance; // -1 0 1
		Type key;
	};
public:
	AVLNode *root;

	static AVLNode * _Buynode()
	{
		AVLNode *s = (AVLNode*)malloc(sizeof(AVLNode));
		if(NULL == s) exit(1);
		memset(s,0,sizeof(AVLNode));
		return s;
	}
	static AVLNode * _Freenode(AVLNode *p)
	{
		free(p);
	}

	static void RotateLeft(AVLNode *&ptr)
	{
		AVLNode * newroot = ptr->rightchild;
		newroot->parent = ptr->parent; ///1
		ptr->rightchild = newroot->leftchild;
		if(newroot->leftchild != NULL)
		{
			newroot->leftchild->parent = ptr; //2
		}
		newroot->leftchild = ptr;
		ptr->parent = newroot; // 3
		ptr = newroot;
	}
	static void RotateRight(AVLNode *&ptr)
	{
		AVLNode *newroot = ptr->leftchild;
		newroot->parent = ptr->parent; //1
		ptr->leftchild = newroot->rightchild;
		if(newroot->rightchild != NULL)
		{
			newroot->rightchild->parent = ptr;//2
		}
		newroot->rightchild = ptr;
		ptr->parent = newroot;  //3
		ptr = newroot;
	}
	static void LeftBalance(AVLNode *&ptr)
	{
		AVLNode *leftsub = ptr->leftchild, *rightsub = NULL;
		switch(leftsub->balance)
		{
		case 0: cout<<"left already balance"<<endl; break;
		case -1:
			ptr->balance = leftsub->balance = 0;
			RotateRight(ptr);
			break;
		case 1:
			rightsub = leftsub->rightchild;
			switch(rightsub->balance)
			{
			case 1:
				ptr->balance = 0;
				leftsub->balance = -1;
				break;
			case -1:
				ptr->balance = 1;
				leftsub->balance = 0;
				break;
			case 0:
				ptr->balance = leftsub->balance = 0;
				break;
			}
			rightsub->balance = 0;
			RotateLeft(ptr->leftchild);//
			RotateRight(ptr);
			break;
		}
	}
	static void RightBalance(AVLNode *&ptr)
	{
		AVLNode *rightsub= ptr->rightchild, *leftsub = NULL;
		switch(rightsub->balance)
		{
		case 0: cout<<"right already balance"<<endl; break;
		case 1:
			ptr->balance = rightsub->balance = 0;
			RotateLeft(ptr);
			break;
		case -1:
			leftsub = rightsub->leftchild;
			switch(leftsub->balance)
			{
			case 1:
				ptr->balance = -1;
				rightsub->balance = 0;
				break;
			case -1:
				ptr->balance = 0;
				rightsub->balance = 1;
				break;
			case 0:
				ptr->balance = rightsub->balance = 0;
				break;
			}
			leftsub->balance = 0;
			RotateRight(ptr->rightchild);
			RotateLeft(ptr);
			break;
		}
	}
public:
	AVLTree():root(NULL) {}
	~AVLTree() {}
public:

	static bool Insert(AVLNode *&ptr,const Type &x,
		AVLNode *pa,int &taller)
	{
		bool res = false;
		if(ptr == NULL)
		{
			ptr = _Buynode();
			ptr->key = x;
			ptr->parent = pa;
			res = true;
			taller = 1;
		}else if(x < ptr->key)
		{
			res = Insert(ptr->leftchild,x,ptr,taller);
			if(res && taller == 1)
			{
				switch(ptr->balance)
				{
				case 0: ptr->balance = -1; break;
				case 1: ptr->balance = 0; taller = 0; break;
				case -1:
					LeftBalance(ptr);
					taller = 0;
					break;
				}
			}
		}else if(x > ptr->key)
		{
			res = Insert(ptr->rightchild,x,ptr,taller);
			if(res && taller == 1)
			{
				switch(ptr->balance)
				{
				case 0: ptr->balance = 1; break;
				case -1: ptr->balance = 0;  taller = 0;break;
				case 1: 
					RightBalance(ptr);
					taller = 0;
					break;
				}
			}
		}
		return res;
	}
	bool Insert(const Type &x)
	{
		int taller = 0;
		return Insert(root,x,NULL,taller);
	}
};
void main()
{
	int ar[]={16, 3, 7, 11, 9, 26, 18, 14, 15,3,7 };
	int n = sizeof(ar)/sizeof(ar[0]);
	AVLTree<int> myt;
	for(int i = 0;i<n;++i)
	{
		myt.Insert(ar[i]);
	}


}


template<class Type>
class BSTree
{
protected:
	struct BstNode
	{
		BstNode *parent;
		BstNode *leftchild;
		BstNode *rightchild;
		Type  key;
	};
private:
	BstNode *root;

	static BstNode * _Buynode()
	{
		BstNode *s = (BstNode *)malloc(sizeof(BstNode));
		if(NULL == s) exit(1);
		memset(s,0,sizeof(BstNode));
		return s;
	}
	static void _Freenode(BstNode *p)
	{
		free(p);
	}
	static BstNode * FindValue(BstNode *ptr, Type kx)
	{
		while(ptr != NULL && ptr->key != kx)
		{
			ptr = kx < ptr->key ? ptr->leftchild:ptr->rightchild;
		}
		return ptr;
	}
	static BstNode * SearchValue(BstNode *ptr,Type kx)
	{
		if(ptr == NULL || ptr->key == kx) 
			return ptr;
		else if(kx < ptr->key)
		{
			return SearchValue(ptr->leftchild,kx);

		}else if(kx > ptr->key)
		{
			return SearchValue(ptr->rightchild,kx);
		}
	}
    static BstNode *First(BstNode *ptr)
	{
		while(ptr != NULL && ptr->leftchild != NULL)
		{
			ptr = ptr->leftchild;
		}
		return ptr;
	}
	static BstNode * Next(BstNode *ptr)
	{
		if(ptr == NULL) return NULL;
		if(ptr->rightchild != NULL)
		{
			return First(ptr->rightchild);
		}else
		{
			BstNode *pa = ptr->parent;
			while(pa != NULL && ptr != pa->leftchild)
			{
				ptr = pa;
				pa = pa->parent;
			}
			return pa;
		}
	}
	static BstNode * Last(BstNode *ptr)
	{
		while(ptr != NULL && ptr->rightchild != NULL)
		{
			ptr = ptr->rightchild;
		}
		return ptr;
	}
	static BstNode *Prev(BstNode *ptr)
	{
		if(ptr == NULL) return NULL;
		if(ptr->leftchild != NULL)
		{
			return Last(ptr->leftchild);
		}else
		{
			BstNode *pa = ptr->parent;
			while(pa != NULL && ptr != pa->rightchild)
			{
				ptr = pa;
				pa = pa->parent;
			}
			return pa;
		}
	}
	static bool Insert(BstNode *&ptr,const Type &x,BstNode *pa)
	{
		bool res = false;
		if(ptr == NULL)
		{
			ptr = _Buynode();
			ptr->parent = pa;
			ptr->key = x;
			res = true;
		}else if(x < ptr->key)
		{
			res = Insert(ptr->leftchild,x,ptr);
		}else if(x > ptr->key)
		{
			res = Insert(ptr->rightchild,x,ptr);
		}
		return res;
	}
public:
	BSTree():root(NULL) {}
	~BSTree() {}

	BstNode * FindValue(const Type &x) const
	{
		return FindValue(root,x);
	}
	BstNode * SearchValue(const Type &x) const
	{
		return SearchValue(root,x);
	}


	void NiceInOrder()
	{
		for(BstNode *p = First(root); p != NULL; p = Next(p))
		{
			cout<<p->key<<" ";
		}
		cout<<endl;
	}


	void ResNiceInOrder()
	{
		for(BstNode *p = Last(root); p != NULL; p = Prev(p))
		{
			cout<<p->key<<" ";
		}
		cout<<endl;
	}


	bool Insert(const Type &x)
	{
		return Insert(root,x,NULL);
	}
	static bool Remove(BstNode *&ptr,const Type &x)
	{
		bool res = false;
		if(ptr == NULL)  return res;
		if(x < ptr->key)
		{
			res = Remove(ptr->leftchild,x);
		}else if(x > ptr->key)
		{
			res = Remove(ptr->rightchild,x);
		}else if(ptr->leftchild != NULL && ptr->rightchild != NULL)
		{
			BstNode *p = Next(ptr);
			ptr->key = p->key;
			res = Remove(p,p->key);////???
		}else
		{
			BstNode *child = ptr->leftchild != NULL? 
				ptr->leftchild:ptr->rightchild;
			if(child != NULL) 
				child->parent = ptr->parent;
			BstNode *q = ptr;
			ptr = child;
			_Freenode(q);
			res = true;
		}
		return res;
	}
	bool Remove(const Type &x)
	{
		return Remove(root,x);
	}
};

void main()
{
	int ar[]={53,17,78,9,45,65,87,23,81,94,88,100,17,23};
	int n = sizeof(ar)/sizeof(ar[0]);
	BSTree<int> myt;
	for(int i = 0;i<n;++i)
	{
		cout<<myt.Insert(ar[i]);
	}
	cout<<endl;
	myt.NiceInOrder();
	int kx;
	while(cin>>kx,kx != -1)
	{
		cout<<myt.Remove(kx)<<": ";
		myt.NiceInOrder();
	}
}

typedef int KeyType;

typedef struct BstNode
{
	BstNode *parent;
	BstNode *leftchild;
	BstNode *rightchild;
	KeyType  key;
}BstNode, * BSTree;

BstNode * _Buynode()
{
	BstNode *s = (BstNode *)malloc(sizeof(BstNode));
	if(NULL == s) exit(1);
	memset(s,0,sizeof(BstNode));
	return s;
}
void _Freenode(BstNode *p)
{
	free(p);
}

BstNode * FindValue(BstNode *ptr, KeyType kx)
{
	while(ptr != NULL && ptr->key != kx)
	{
		ptr = kx < ptr->key ? ptr->leftchild:ptr->rightchild;
	}
	return ptr;
}
BstNode * SearchValue(BstNode *ptr,KeyType kx)
{
	if(ptr == NULL || ptr->key == kx) 
		return ptr;
	else if(kx < ptr->key)
	{
		return SearchValue(ptr->leftchild,kx);

	}else if(kx > ptr->key)
	{
		return SearchValue(ptr->rightchild,kx);
	}
}
bool Insert_BST(BSTree *ptree,KeyType kx)
{
	if(ptree == NULL) return false;
	BstNode *pa = NULL;
	BstNode *p = *ptree;
	while(p != NULL && p->key != kx)
	{
		pa = p;
		p = kx < p->key? p->leftchild:p->rightchild;
	}
	if(p != NULL && p->key == kx)  return false;
	p = _Buynode();
	p->key = kx;
	p->parent = pa;
	if(pa == NULL)
	{
		*ptree = p;
	}
	else
	{
		if(p->key < pa->key)
		{
			pa->leftchild = p;
		}else
		{
			pa->rightchild = p;
		}
	}
	return true;
}
void InOrder(BstNode *p)
{
	if(p != NULL)
	{
		InOrder(p->leftchild);
		cout<<p->key<<" ";
		InOrder(p->rightchild);
	}
}
BstNode *First(BstNode *ptr)
{
	while(ptr != NULL && ptr->leftchild != NULL)
	{
		ptr = ptr->leftchild;
	}
	return ptr;
}
BstNode * Next(BstNode *ptr)
{
	if(ptr == NULL) return NULL;
	if(ptr->rightchild != NULL)
	{
		return First(ptr->rightchild);
	}else
	{
		BstNode *pa = ptr->parent;
		while(pa != NULL && ptr != pa->leftchild)
		{
			ptr = pa;
			pa = pa->parent;
		}
		return pa;
	}
}
BstNode * Last(BstNode *ptr)
{
	while(ptr != NULL && ptr->rightchild != NULL)
	{
		ptr = ptr->rightchild;
	}
	return ptr;
}
BstNode *Prev(BstNode *ptr)
{
	if(ptr == NULL) return NULL;
	if(ptr->leftchild != NULL)
	{
		return Last(ptr->leftchild);
	}else
	{
		BstNode *pa = ptr->parent;
		while(pa != NULL && ptr != pa->rightchild)
		{
			ptr = pa;
			pa = pa->parent;
		}
		return pa;
	}
}
void NiceInOrder(BstNode *ptr)
{
	for(BstNode *p = First(ptr); p != NULL; p = Next(p))
	{
		cout<<p->key<<" ";
	}
	cout<<endl;
}


void ResNiceInOrder(BstNode *ptr)
{
	for(BstNode *p = Last(ptr); p != NULL; p = Prev(p))
	{
		cout<<p->key<<" ";
	}
	cout<<endl;
}

bool Remove_BST(BSTree *ptree,KeyType kx)
{
	if(ptree == NULL || *ptree == NULL)
		return false;
	BstNode * p = FindValue(*ptree,kx);
	if(p == NULL) return false;
	///////////////////////////////
	if(p->leftchild != NULL && p->rightchild != NULL)
	{
		BstNode *q = Next(p);
		p->key = q->key;
		p = q;
	}
	BstNode *pa = p->parent;
	BstNode *child = p->leftchild != NULL? 
		    p->leftchild:p->rightchild;
	if(child != NULL) child->parent = pa;
	if(pa == NULL)
	{
		*ptree = child;
	}else
	{
		if(pa->leftchild == p)
		{
			pa->leftchild = child;
		}else
		{
			pa->rightchild = child;
		}
	}
	_Freenode(p);

	return true;
}
void main()
{
	KeyType ar[]={53,17,78,9,45,65,87,23,81,94,88,100,17,23};
	int n = sizeof(ar)/sizeof(ar[0]);
	BSTree root = NULL;
	for(int i = 0;i<n;++i)
	{
		cout<<Insert_BST(&root,ar[i]);
	}
	cout<<endl;

	InOrder(root);
	cout<<endl;
	NiceInOrder(root);
	ResNiceInOrder(root);
	KeyType kx;
	while(cin>>kx,kx != -1)
	{
		cout<<Remove_BST(&root,kx)<<"-> ";
		NiceInOrder(root);
	}

}

typedef char ElemType;
#define END '#'
typedef enum{LINK = 0, THREAD = 1} PointerTag;
typedef struct BiThrNode
{
	BiThrNode *leftchild;
	BiThrNode *rightchild;
	PointerTag Ltag,Rtag;
	ElemType data;
}BiThrNode, *BinaryThreadTree;

BiThrNode * _Buynode()
{
	BiThrNode *s = (BiThrNode *)malloc(sizeof(BiThrNode));
	if(NULL == s) exit(1);
	memset(s,0,sizeof(BiThrNode));
	return s;
}
BiThrNode * CreateTree(ElemType *&str)
{
	BiThrNode *s = NULL;
	if(*str != END)
	{
		s = _Buynode();
		s->data = *str;
		s->Ltag = s->Rtag = LINK;
		s->leftchild = CreateTree(++str);
		s->rightchild = CreateTree(++str);
	}
	return s;
}
void InOrder(BiThrNode *p)
{
	if(p != NULL)
	{
		InOrder(p->leftchild);
		cout<<p->data<<" ";
		InOrder(p->rightchild);
	}
}


void Make(BiThrNode *p,BiThrNode *&ptr)
{
	if(p != NULL)
	{
		Make(p->leftchild,ptr);
		if(p->leftchild == NULL)
		{
			p->leftchild = ptr;
			p->Ltag = THREAD;
		}
		if(ptr != NULL && ptr->rightchild == NULL)
		{
			ptr->rightchild = p;
			ptr->Rtag = THREAD;
		}
		ptr = p;
		Make(p->rightchild,ptr);
	}
}


void MakeThread(BiThrNode *p)
{
	BiThrNode *ptr = NULL;
	Make(p,ptr);
	ptr->Rtag = THREAD;
	ptr->rightchild = NULL;
}

BiThrNode * First(BiThrNode *ptr)
{
	while(ptr != NULL && ptr->Ltag != THREAD)
	{
		ptr = ptr->leftchild;
	}
	return ptr;
}
BiThrNode * Next(BiThrNode *ptr)
{
	if(ptr == NULL) return NULL;
	if(ptr->Rtag == THREAD)
	{
		return ptr->rightchild;
	}else
	{
		return First(ptr->rightchild);
	}
}
BiThrNode * Last(BiThrNode *ptr)
{
	while(ptr != NULL && ptr->Rtag != THREAD)
	{
		ptr = ptr->rightchild;
	}
	return ptr;
}
BiThrNode *Prev(BiThrNode *ptr)
{
	if(ptr == NULL) return NULL;
	if(ptr->Ltag == THREAD)
	{
		return ptr->leftchild;
	}else
	{
		return Last(ptr->leftchild);
	}
}

void NiceInOrder(BiThrNode *ptr)
{
	for(BiThrNode *p = First(ptr); p != NULL; p = Next(p))
	{
		cout<<p->data<<" ";
	}
	cout<<endl;
}
void ResNiceInOrder(BiThrNode *ptr)
{
	for(BiThrNode *p = Last(ptr); p != NULL; p =Prev(p))
	{
		cout<<p->data<<" ";
	}
	cout<<endl;
}
void main()
{
	BinaryThreadTree root;
	ElemType *str="ABC##DE##F##G#H##";
	root = CreateTree(str);
	InOrder(root);
	cout<<endl;
	MakeThread(root);

	NiceInOrder(root);
	ResNiceInOrder(root);

}


template<class _Ty>
class BinaryTree
{
protected:
	struct BtNode 
	{
		BtNode *leftchild;
		BtNode *rightchild;
		_Ty     data;
	};
private:
	BtNode *root;
	_Ty     RefValue;// #define END '#'
	static BtNode * _Buynode()
	{
		BtNode * s = (BtNode*)malloc(sizeof(BtNode));
		if(NULL == s) exit(1);
		memset(s,0,sizeof(BtNode));
		return s;
	}
	static void _Freenode(BtNode *p)
	{
		free(p);
	}


	BtNode * Create(const _Ty *&str)
	{
		BtNode *s = NULL;
		if(*str != RefValue)
		{
			s = _Buynode();
			s->data = *str;
			s->leftchild = Create(++str);
			s->rightchild = Create(++str);
		}
		return s;
	}
	
	static void Clear(BtNode *p)
	{
		if(p != NULL)
		{
			Clear(p->leftchild);
			Clear(p->rightchild);
			_Freenode(p);
		}
	} 
	static void InOrder(BtNode *p)
	{
		if(p != NULL)
		{
			InOrder(p->leftchild);
			cout<<p->data<<" ";
			InOrder(p->rightchild);
		}
	}
	static BtNode * Copy(BtNode *ptr)
	{
		BtNode *s = NULL;
		if(ptr != NULL)
		{
			s = _Buynode();
			s->data = ptr->data;
			s->leftchild = Copy(ptr->leftchild);
			s->rightchild = Copy(ptr->rightchild);
		}
		return s;
	}
	static BtNode * FindValue(BtNode *ptr,const _Ty &x)
	{
		if(ptr == NULL || ptr->data == x) return ptr;
		else
		{
			BtNode *p = FindValue(ptr->leftchild,x);
			if(NULL == p)
			{
				p = FindValue(ptr->rightchild,x);
			}
			return p;
		}
	}
	static bool equal(BtNode *pa,BtNode *pb)
	{
		return (pa == NULL && pb == NULL) || 
			(pa != NULL && pb != NULL && 
			pa->data == pb->data &&
			equal(pa->leftchild,pb->leftchild)&& 
			equal(pa->rightchild,pb->rightchild));
	}
public:
	static int num;
	typedef struct BtNode * PBtNode;
	BtNode * GetRoot() const { return root;}
public:
	BinaryTree(const _Ty &x):root(NULL),RefValue(x) {}
	BinaryTree(const BinaryTree<_Ty> &bt)
	{
		root = Copy(bt.root);
		RefValue = bt.RefValue;
	}
	BinaryTree<_Ty> operator=(const BinaryTree<_Ty> &bt)
	{
		if(this != &bt)
		{
			Clear();
			this->root = Copy(bt.root);
			RefValue = bt.RefValue;
		}
		return s;
	}
  
	~BinaryTree() { Clear();}
	void Clear()// this -> root; 
	{
		Clear(root);
		root = NULL;
	}

	void CreateTree(const _Ty *str)
	{
		if(str != NULL)
		{
			root = Create(str);
		}
	}
	void InOrder() const
	{
		InOrder(root);
		cout<<endl;
	}


	BtNode * FindValue(const _Ty &x) const
	{
		return FindValur(root,x);
	}


	bool operator==(const BinaryTree<_Ty> &bt) const
	{
		return equal(this->root,bt.root);
	}
	bool operator!=(const BinaryTree<_Ty> &bt) const
	{
		return !(*this == bt);
	}
};

/////////////////////////////////////////////

template<class _Ty>
class TreeIterator
{
protected:
	typename BinaryTree<_Ty>::PBtNode _Ptr;
	BinaryTree<_Ty> &tree;
public:
	TreeIterator(BinaryTree<_Ty> &bt):tree(bt),_Ptr(NULL) 
	{

	}
	virtual ~TreeIterator() {}
	_Ty & operator*() { return _Ptr->data;}
	const _Ty & operator*() const { return _Ptr->data;}
	bool IsDone() const { return _Ptr == NULL;}
	virtual void First() = 0;
	virtual void operator++() = 0;
};

template<class _Ty>
struct StkNode
{
	typename BinaryTree<_Ty>::PBtNode pnode;
	int popnum;
public:
	StkNode(typename BinaryTree<_Ty>::PBtNode p= NULL)
		:pnode(p),popnum(0) {}
};

template<class _Ty>
class PastIterator : public TreeIterator<_Ty>
{
protected:
	stack<StkNode<_Ty> > st;
public:
	PastIterator(BinaryTree<_Ty> &bt):TreeIterator<_Ty>(bt) 
	{

	}
	~PastIterator() {}
	void First()
	{
		if(tree.GetRoot() != NULL)
		{
			st.push(StkNode<_Ty>(tree.GetRoot()));
			this->operator++();
		}
	}
	void operator++()
	{
		if(st.empty())
		{
			_Ptr = NULL;
			return ;
		}
		for(;;)
		{
			StkNode<_Ty> node = st.top(); 
			st.pop();
			if(++node.popnum == 3)
			{
				_Ptr = node.pnode;
				return ;
			}
			st.push(node);
			if(node.popnum == 1 && node.pnode->leftchild != NULL)
			{
				st.push(StkNode<_Ty>(node.pnode->leftchild));
			}else if(node.popnum == 2 && node.pnode->rightchild != NULL)
			{
				st.push(StkNode<_Ty>(node.pnode->rightchild));
			}
		}
	}
};

template<class _Ty>
class InIterator :public PastIterator<_Ty>
{
public:
	InIterator(BinaryTree<_Ty> &bt):PastIterator<_Ty>(bt) 
	{
	}
	~InIterator() {}
	void operator++()
	{
		if(st.empty())
		{
			_Ptr = NULL;
			return ;
		}
		for(;;)
		{
			StkNode<_Ty> node = st.top(); 
			st.pop();
			if(++node.popnum == 2)
			{
				_Ptr = node.pnode;
				if(node.pnode->rightchild != NULL)
				{
					st.push(StkNode<_Ty>(node.pnode->rightchild));
				}
				return ;
			}
			st.push(node);
			if(node.popnum == 1 && node.pnode->leftchild != NULL)
			{
				st.push(StkNode<_Ty>(node.pnode->leftchild));
			}
		}
	}
	
};

template<class _Ty>
class PreIterator :public TreeIterator<_Ty>
{
protected:
	stack<typename BinaryTree<_Ty>::PBtNode> st;
public:
	PreIterator(BinaryTree<_Ty> &bt):TreeIterator<_Ty>(bt) {}
	~PreIterator() {}
	void First()
	{
		if(tree.GetRoot() != NULL)
		{
			st.push(tree.GetRoot());
			operator++();
		}
	}
	void operator++()
	{
		if(st.empty())
		{
			_Ptr = NULL;
			return ;
		}
		_Ptr = st.top(); st.pop();
		if(_Ptr->rightchild != NULL)
			st.push(_Ptr->rightchild);
		if(_Ptr->leftchild != NULL)
			st.push(_Ptr->leftchild);
	}

};

template<class _Ty>
class LevelIterator : public TreeIterator<_Ty>
{
protected:
	queue<typename BinaryTree<_Ty>::PBtNode> st;
public:
	LevelIterator(BinaryTree<_Ty> &bt):TreeIterator<_Ty>(bt) {}
	~LevelIterator() {}
public:
	void First()
	{
		if(tree.GetRoot() != NULL)
		{
			st.push(tree.GetRoot());
			operator++();
		}
	}
	void operator++()
	{
		if(st.empty())
		{
			_Ptr = NULL;
			return ;
		}
		_Ptr = st.front(); st.pop();
		if(_Ptr->leftchild != NULL)
			st.push(_Ptr->leftchild);
		if(_Ptr->rightchild != NULL)
			st.push(_Ptr->rightchild);

	}
};
////////////////////////////////////////////////
template<class _Ty>
class SInIterator : public TreeIterator<_Ty>
{
protected:
	stack<typename BinaryTree<_Ty>::PBtNode> st;
	typename BinaryTree<_Ty>::PBtNode p;
public:
	SInIterator(BinaryTree<_Ty> &bt):TreeIterator<_Ty>(bt),p(NULL) {}
	~SInIterator() {}
	void First()
	{
		if(tree.GetRoot() != NULL)
		{
			p = tree.GetRoot();
			operator++();
		}
	}
	void operator++()
	{
		if(p == NULL && st.empty())
		{
			_Ptr = NULL;
			return ;
		}
		while(p != NULL)
		{
			st.push(p);
			p = p->leftchild;
		}
		p = st.top(); st.pop();
		_Ptr = p;
		p = p->rightchild;
	}
};

template<class _Ty>
class SPastIterator : public SInIterator<_Ty>
{
protected:
	typename BinaryTree<_Ty>::PBtNode tag;
public:
	SPastIterator(BinaryTree<_Ty> &bt):SInIterator<_Ty>(bt),tag(NULL) {}
	~SPastIterator() {}
	void First()
	{
		if(tree.GetRoot() != NULL)
		{
			tag = NULL;
			p = tree.GetRoot();
			operator++();
		}
	}
	void operator++()
	{
		if(p == NULL && st.empty())
		{
			_Ptr = NULL;
			return;
		}
		for(;;)
		{
			while(p != NULL)
			{
				st.push(p);
				p = p->leftchild;
			}
			p = st.top(); st.pop();
			if(p->rightchild == NULL || p->rightchild == tag)
			{
				_Ptr = p;
				tag = p;
				p = NULL;
				return ;
			}else
			{
				st.push(p);
				p = p->rightchild;
			}
		}
	}
};

template<class _Ty>
void Print_Tree(TreeIterator<_Ty> &bt)
{
	bt.First();
	while(!bt.IsDone())
	{
		cout<<*bt<<" ";
		++bt;
	}
	cout<<endl;
}

void main()
{
	char *str="ABC##DE##F##G#H##";
    BinaryTree<char> myt('#');
	myt.CreateTree(str);
	SInIterator<char> sii(myt);
	SPastIterator<char> sp(myt);
	Print_Tree(sii);
	Print_Tree(sp);
}

void main()
{
	char *str="ABC##DE##F##G#H##";
	int   ar[]={1,2,3,-1,-1,4,5,-1,-1,6,-1,-1,7,-1,8,-1,-1};
	BinaryTree<char> myt('#');
	myt.CreateTree(str);


	PastIterator<char> pi(myt);
	InIterator<char> ii(myt);
	PreIterator<char> si(myt);
	LevelIterator<char> lv(myt);
	Print_Tree(pi);
	Print_Tree(ii);
	Print_Tree(si);
	Print_Tree(lv);
}



typedef char ElemType;
#define END '#'
typedef struct BtNode
{
	BtNode *leftchild;
	BtNode *rightchild;
	ElemType data;
}BtNode , *BinaryTree;

BtNode * _Buynode();
void _Freenode(BtNode *p);
void PreOrder(BtNode *p);
void InOrder(BtNode *p);
void PastOrder(BtNode *p);

BtNode * CreateTree1();
BtNode * CreateTree2(ElemType *&str);
BtNode * CreateTree3(ElemType ** const pstr);
void CreateTree4(BtNode *&ptr,ElemType *&str);
void CreateTree5(BtNode ** const ptr,ElemType *&str);

BtNode * CreateTreeIL(ElemType *is,ElemType *ls,int n);

int Size(BtNode *p);
int Depth(BtNode *p);

bool Is_Full_BinaryTree(BtNode *p);
bool Is_Comp_BinaryTree(BtNode *p);
bool Is_Balance_BinaryTree(BtNode *p);

BtNode * FindValue(BtNode *ptr,ElemType x);
BtNode * FindParent(BtNode *ptr,BtNode *child);
BtNode * NearParent(BtNode *ptr, BtNode *child1,BtNode *child2);

void NicePreOrder(BtNode *ptr);

void NicePastOrder(BtNode *ptr);
void NiceLevelOrder(BtNode *ptr);

void StkInOrder(BtNode *ptr);
void StkPastOrder(BtNode *ptr);
void Print_K_Level_Item(BtNode *ptr,int k);




BtNode * CreatePI(ElemType *pi,ElemType *si,int n)
{
	BtNode *s = NULL;
	if(n > 0)
	{
		s = _Buynode();
		s->data = pi[0];
		int pos = FindIndex(si,n,pi[0]);
		if(pos == -1) exit(1);
		s->leftchild = CreatePI(pi+1,is,pos);
		s->rightchild = CreatePI(pi+pos+1,is+pos+1,n - pos -1);
	}
	reutrn s;
}
BtNode * CreateTreePI(ElemType *pi,ElemType *si,int n)
{
	if(pi == NULL || si == NULL || n <= 0)
		return NULL;
	else
		return CreatePI(pi,si,n);
}
void NiceInOrder(BtNode *ptr)
{
	if(ptr == NULL) return ;
	stack<BtNode *> st;
	while(ptr != NULL || !st.empty())
	{
		while(ptr != NULL)
		{
			st.push(ptr);
			ptr = ptr->leftchild;
		}
		ptr = st.top(); st.pop();
		cout<<ptr->data<<" ";
		ptr = ptr->rightchild;
		
	}
}

void NicePastOrder(BtNode *ptr)
{
	if(ptr == NULL) return ;
	stack<BtNode *> st;
	BtNode *tag = NULL;
	while(ptr != NULL || !st.empty())
	{
		while(ptr != NULL)
		{
			st.push(ptr);
			ptr = ptr->leftchild;
		}
		ptr = st.top(); st.pop();
		if(ptr->rightchild == NULL || ptr->rightchild == tag)
		{
			cout<<ptr->data<<" ";
			tag = ptr;
			ptr = NULL;
		}else
		{
			st.push(ptr);
			ptr = ptr->rightchild;
		}
		
	}
}
void main()
{
	BinaryTree root = NULL;
	ElemType *pi="ABCDEFGH";
	ElemType *si="CBEDFAGH";
	ElemType *ls="CEFDBHGA";

	int n = strlen(pi);
	root = CreateTreePI(pi,si,n);

}
*/

#include <stdio.h>
#include <stdlib.h>

struct AVLNode;
typedef struct AVLNode *AVLNodePtr;

struct AVLNode
{
	int element;
	AVLNodePtr left;
	AVLNodePtr right;
	int height;
};

void makeempty(AVLNodePtr T)
{
	if (T == NULL)
		return;
	else
	{
		makeempty(T->left);
		makeempty(T->right);
		free(T);
	}
}

static int height(AVLNodePtr p)
{
	if (p == NULL)
		return -1;
	else
		return p->height;
}

static int Max(int ln, int rn)
{
	return ln > rn ? ln : rn;
}

/* return pointer to the new root */
static AVLNodePtr singlerotateLL(AVLNodePtr k2)
{
	AVLNodePtr k1 = k2->left;
	k2->left = k1->right;
	k1->right = k2;
	k2->height = Max(height(k2->left), height(k2->right)) + 1;
	k1->height = Max(height(k1->left), k2->height) + 1;
	return k1;
}

static AVLNodePtr singlerotateRR(AVLNodePtr k1)
{
	AVLNodePtr k2 = k1->right;
	k1->right = k2->left;
	k2->left = k1;
	k1->height = Max(height(k1->left), height(k1->right)) + 1;
	k2->height = Max(k1->height, height(k2->right)) + 1;
	return k2;
}

static AVLNodePtr doublerotateRL(AVLNodePtr k3)
{
	k3->left = singlerotateRR(k3->left);
	return singlerotateLL(k3);
}

static AVLNodePtr doublerotateLR(AVLNodePtr k3)
{
	k3->right = singlerotateLL(k3->right);
	return singlerotateRR(k3);
}


AVLNodePtr insert(int X, AVLNodePtr T)
{
	if (T == NULL)
	{
		/* create and return a one-node tree */
		T = (AVLNodePtr)malloc(sizeof(struct AVLNode));
		if (T == NULL)
		{
			printf("out of space!");
			exit(1);
		}
		else
		{
			T->element = X;
			T->height = 0;
			T->left = T->right = NULL;
		}
	}

	else if (X < T->element)
	{
		T->left = insert(X, T->left);
		if (height(T->left) - height(T->right) == 2)
		{
			if (X < T->left->element)
				T = singlerotateLL(T);
			else
				T = doublerotateRL(T);
		}
	}

	else if (X > T->element)
	{
		T->right = insert(X, T->right);
		if (height(T->right) - height(T->left) == 2)
		{
			if (X > T->right->element)
				T = singlerotateRR(T);
			else
				T = doublerotateLR(T);
		}
	}
	/* else X is in the tree already; we'll do nothing */
	T->height = Max(height(T->left), height(T->right)) + 1;
	return T;
}

void inorder(AVLNodePtr T)
{
	if (T == NULL)
		return;
	else
	{
		inorder(T->left);
		printf("%d ", T->element);
		inorder(T->right);
	}
}

int main(void)
{
	int arr[] = {3, 2, 1, 4, 5, 6, 7, 16, 15, 14, 13, 12, 11, 10, 8, 9};
	AVLNodePtr T = NULL;
	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
		T = insert(arr[i], T);

	inorder(T);
	makeempty(T);
	return 0;
