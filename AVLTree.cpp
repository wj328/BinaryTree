#include <iostream>
using namespace std;

#if 0
template <typename Type>
class AVLTree
{
protected:
	struct AVLNode
	{
		AVLNode *parent;
		AVLNode *leftchild;
		AVLNode *rightchild;
		int balance;//平衡因子 -1 0 1
		Type key;
	};
public:
	AVLTree():root(NULL){}
	~AVLTree(){}
	bool Insert(const Type &kx)
	{
		bool teller = false;
		return Insert(root, NULL, kx, teller);
	}
	void InOrder()
	{
		InOrder(root);
	}
	bool Nice_Insert(AVLNode *&ptr, const Type &kx)
	{

	}
private:
	AVLNode *root;

	//封装是为了代码的强壮性,调整代码时只需关注这一处
	static AVLNode* BuyNode()
	{
		AVLNode *s = (AVLNode*)malloc(sizeof(AVLNode));
		if (NULL == s) exit(1);
		memset(s, 0, sizeof(AVLNode));
		return s;
	}
	static void FreeNode(AVLNode *ptr)
	{
		free(ptr);
	}
	static void InOrder(AVLNode *ptr)
	{
		if (ptr != NULL)
		{
			InOrder(ptr->leftchild);
			cout<<ptr->key<<" ";
			InOrder(ptr->rightchild);
		}
	}
	static void RotateLeft(AVLNode *&ptr)
	{
		if (NULL == ptr) return ;
		AVLNode *newroot = ptr->rightchild;
		newroot->parent = ptr->parent;
		ptr->rightchild = newroot->leftchild;

		if (newroot->leftchild != NULL)
		{
			newroot->leftchild->parent = ptr;
		}

		newroot->leftchild = ptr;
		ptr->parent = newroot;
		ptr = newroot;//换根
	}
	static void RotateRight(AVLNode *&ptr)
	{
		if (NULL == ptr) return ;

		AVLNode *newroot = ptr->leftchild;
		newroot->parent = ptr->parent;
		ptr->leftchild = newroot->rightchild;

		if (newroot->rightchild != NULL)
		{
			newroot->rightchild->parent = ptr;
		}

		newroot->rightchild = ptr;
		ptr->parent = newroot;
		ptr = newroot;//
	}
	static void LeftBalance(AVLNode *&ptr)
	{
		AVLNode *leftsub = ptr->leftchild;
		AVLNode *rightsub = NULL;

		switch(leftsub->balance)
		{
		case 0: cout<<"left is balance"; break;
		case -1: 
			leftsub->balance = ptr->balance = 0; 
			RotateRight(ptr);
			break;
		case 1:
			rightsub = leftsub->rightchild;
			switch(rightsub->balance)
			{
			case 0: ptr->balance = leftsub->balance = 0; break;
			case -1: 
				ptr->balance = 1;
				leftsub->balance = 0;
			case 1:
				ptr->balance = 0;
				leftsub->balance = -1;
			}
			rightsub->balance = 0;
			RotateLeft(ptr->leftchild);//leftsub的话上边必须用引用
			RotateRight(ptr);
			break;
		}
	}
	static void RightBalance(AVLNode *&ptr)
	{
		AVLNode *rightsub = ptr->rightchild;
		AVLNode *leftsub = NULL;

		switch(rightsub->balance)
		{
		case 0: cout<<"right is balance"; break;
		case 1: 
			rightsub->balance = ptr->balance = 0; 
			RotateLeft(ptr);
			break;
		case -1:
			leftsub = rightsub->leftchild;
			switch(leftsub->balance)
			{
			case 0: ptr->balance = rightsub->balance = 0; break;
			case -1: 
				ptr->balance = 0;
				rightsub->balance = 1;
			case 1:
				ptr->balance = -1;
				rightsub->balance = 0;
			}
			leftsub->balance = 0;
			RotateRight(ptr->rightchild);
			RotateLeft(ptr);
			break;
		}
	}
	//递归插入
	static bool Insert(AVLNode *&ptr, AVLNode *pa, const Type &kx, bool &teller)
	{
		bool res = false;
		if (ptr == NULL)
		{
			ptr = BuyNode();
			ptr->key = kx;
			ptr->parent = pa;
			res = true;
			teller = true;
		}else if (kx < ptr->key)
		{
			res = Insert(ptr->leftchild, ptr, kx, teller);
			if (teller)
			{
				switch(ptr->balance)
				{
				case 0: ptr->balance = -1; break;
				case 1: ptr->balance = 0; teller = false; break;
				case -1:
					LeftBalance(ptr);
					teller = false;
					break;
				}
			}
		}
		else if (kx > ptr->key)
		{
			res = Insert(ptr->rightchild, ptr, kx, teller);
			if (teller)
			{
				switch(ptr->balance)
				{
				case 0: ptr->balance = 1; break;
				case -1: ptr->balance = 0; teller = false; break;
				case 1:
					RightBalance(ptr);
					teller = false;//不需调整
					break;
				}
			}
		}
		return res;
	}
};

void main()
{
	int ar[] = {16,3,7,11,9,26,18,14,15};
	int n = sizeof(ar)/sizeof(ar[0]);

	AVLTree<int> myt;
 	for (int i=0; i<n; ++i)
	{
		cout<<myt.Insert(ar[i]);
	}
}
#endif

#if 0

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
}
#endif

#include <vector>
#include <string>
void Change(vector<string> str, int n)
{
	vector<string> res;
	

	for(int i=0; i<n; ++i)
	{
		string s;
		int m = str[i].size();
		for(int j=0; j<m; ++j)
		{
			
			if(str[i][j] == '_')
				s.push_back('-');
			if(str[i][j] == '/')
			{	
				s.push_back('+');
				++j;
			}
			else if(str[i][j] == '-')
			{
				s.push_back('_');
			}
			if(str[i][j] == '+')
			{
				s.push_back('/');
				s.push_back('\\');
			}
		}
		res.push_back(s);
	}
	for(int i=0; i<n; ++i)
	{
		cout<<res[i]<<endl;
	}
}

int main()
{
	int n = 0;
	cin>>n;
	vector<string> str;
	char *name = (char *)malloc(128);

	for(int i=0; i<n; ++i)
	{
		cin>>name;
		str.push_back(string(name));
	}
	Change(str, n);
	return 0;
}