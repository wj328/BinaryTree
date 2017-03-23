#include <iostream>
#include <stack>
#include <queue>
#include <math.h>
using namespace std;

#define END '#'

typedef char ElemType;
typedef struct BtNode
{
	ElemType data;
	BtNode *leftchild;
	BtNode *rightchild;
}BtNode, *BinaryTree;

BtNode* BuyNode()
{
	BtNode* s = (BtNode*)malloc(sizeof(BtNode));
	if (NULL == s) exit(1);
	memset(s, 0, sizeof(BtNode));//置空
	return s;
}

/*
创建二叉树
*/
//创建1
BtNode * CreateTree1()
{
	ElemType x;
	cin>>x;

	BtNode *s = NULL;
	if (x != END)
	{
		s = BuyNode();
		s->data = x;
		s->leftchild = CreateTree1();
		s->rightchild = CreateTree1();
	}
	return s;
}

void CreateTree2(BtNode *&ptr)//BtNode *ptr创建不了(临时变量)
{
	ElemType x;
	cin>>x;

	if (x != END)
	{
		ptr = BuyNode();
		ptr->data = x;
		CreateTree2(ptr->leftchild);
		CreateTree2(ptr->rightchild);
	}
	return ;
}

void CreateTree3(BtNode ** const ptr)
{
	ElemType x;
	cin>>x;
	if (x == END) *ptr = NULL;
	else
	{
		*ptr = BuyNode();
		(*ptr)->data = x;
		CreateTree3(&(*ptr)->leftchild);
		CreateTree3(&(*ptr)->rightchild);
	}
}

/*
下面的BtNode *str创建的树不正确((临时变量)画图)
		A
	B		C
C
*/
BtNode* CreateTree4(ElemType *&str)
{
	BtNode *s = NULL;
	if (str != NULL && *str != END)
	{
		s = BuyNode();
		s->data = *str;
		s->leftchild = CreateTree4(++str);
		s->rightchild = CreateTree4(++str);
	}
	return s;
}

BtNode* CreateTree5(ElemType ** const pstr)
{
	BtNode *s = NULL;
	if (pstr != NULL && *pstr != NULL && **pstr != END)
	{
		s = BuyNode();
		s->data = **pstr;
		s->leftchild = CreateTree5(&++*pstr);
		s->rightchild = CreateTree5(&++*pstr);
	}
	return s;
}

int Index(ElemType *is, ElemType x, int n)
{
	for (int i=0; i<n; ++i)
	{
		if (is[i] == x)
		{
			return i;
		}
	}
	return -1;
}

//已知前中后序遍历结果,创建二叉树（创建2）
BtNode* CreatePI(ElemType *ps, ElemType *is, int n)
{
	BtNode *s = NULL;
	if (n > 0)
	{
		s = BuyNode();
		s->data = ps[0];
		int pos = Index(is, ps[0], n);
		if(pos == -1) exit(1);
		s->leftchild = CreatePI(ps+1, is, pos);
		s->rightchild = CreatePI(ps+pos+1,is+pos+1, n-pos-1);
	}
	return s;
}

BtNode* CreateTreePI(ElemType *ps, ElemType *is, int n)
{
	if (ps == NULL || is == NULL || n < 1)
		return NULL;
	else
		return CreatePI(ps, is, n);
}

BtNode* CreateLI(ElemType *is, ElemType *ls, int n)
{
	BtNode *s = NULL;
	if (n > 0)
	{
		s  = BuyNode();
		s->data = ls[n-1];
		int pos = Index(is, ls[n-1], n);
		if (pos == -1) exit(1);
		s->leftchild = CreateLI(is, ls, pos);
		s->rightchild = CreateLI(is+pos+1, ls+pos, n-pos-1);
	}
	return s;
}

BtNode* CreateTreeLI(ElemType *is, ElemType *ls, int n)
{
	if (is == NULL || ls == NULL || n < 1)
		return NULL;
	else
		return CreateLI(is, ls, n);
}
/*
作业:
1.ps[] = "ABCDAFGH",用A替换E
2.用非递归创建
*/

/*
前中后序遍历(递归非递归)
*/
void InOrder(BtNode *ptr)
{
	if (ptr != NULL)
	{
		InOrder(ptr->leftchild);
		cout<<ptr->data<<" ";
		InOrder(ptr->rightchild);
	}
}

void PreOrder(BtNode *ptr)
{
	if (ptr != NULL)
	{
		cout<<ptr->data<<" ";
		PreOrder(ptr->leftchild);
		PreOrder(ptr->rightchild);
	}
}

void PastOrder(BtNode *ptr)
{
	if (ptr != NULL)
	{
		PastOrder(ptr->leftchild);
		PastOrder(ptr->rightchild);
		cout<<ptr->data<<" ";
	}
}

//非递归
void NiceInOrder(BtNode *ptr)
{
	if (ptr == NULL) return ;
	stack<BtNode *> st;
	while (ptr != NULL || !st.empty())
	{
		while (ptr != NULL)
		{
			st.push(ptr);
			ptr = ptr->leftchild;
		}
		ptr = st.top(); st.pop();
		cout<<ptr->data<<" ";
		ptr = ptr->rightchild;
	}
}

//如果先入栈的话，中序遍历       
void NiceInOrder2(BtNode *ptr)
{
	if (ptr == NULL)return;
	stack<BtNode *> st;
	BtNode *root = ptr;
	st.push(ptr);
	while (!st.empty())
	{
		while (ptr != NULL)
		{
			st.push(ptr);
			ptr = ptr->leftchild;
		}
		ptr = st.top(); st.pop();
		cout<<ptr->data<<" ";
		ptr = ptr->rightchild;

		if(st.size() == 1 && st.top() == root && ptr == NULL)
		{
			st.pop();
		}
	}
}

void NicePreOrder(BtNode *ptr)
{
	if (ptr == NULL) return ;
	stack<BtNode *> st;
	st.push(ptr);

	while(!st.empty())
	{
		ptr = st.top();
		st.pop();
		cout<<ptr->data<<" ";
		
		if(ptr->rightchild != NULL)
		{
			st.push(ptr->rightchild);
		}
		if(ptr->leftchild != NULL)
		{
			st.push(ptr->leftchild);
		}
	}
}

void NicePastOrder(BtNode *ptr)
{
	if (ptr == NULL) return ;
	stack<BtNode *> st;
	BtNode *flag = NULL;
	
	while (ptr != NULL || !st.empty())
	{
		while(ptr != NULL)
		{
			st.push(ptr);
			ptr = ptr->leftchild;
		}
		ptr = st.top(); st.pop();
		if(ptr->rightchild == NULL || ptr->rightchild == flag)
		{
			cout<<ptr->data<<" ";
			flag = ptr;
			ptr = NULL;
		}
		else
		{
			st.push(ptr);
			ptr = ptr->rightchild;
		}
	}
}
//另外一种非递归方法
struct StkNode
{
	BtNode *pnode;
	int popnum;
public:
	StkNode(BtNode *p = NULL):pnode(p),popnum(0){};
};

void StkInOrder(BtNode *ptr)
{
	if(ptr == NULL)return ;
	stack<StkNode> st;
	StkNode node;

	st.push(StkNode(ptr));
	while (!st.empty())
	{
		node = st.top(); st.pop();
		if (++node.popnum == 2)
		{
			cout<<node.pnode->data<<" ";
			if(node.pnode->rightchild != NULL)
			{
				st.push(StkNode(node.pnode->rightchild));
			}
			continue;;
		}
		
		st.push(node);
		if (node.popnum == 1 && node.pnode->leftchild != NULL)
		{
			st.push(StkNode(node.pnode->leftchild));
		}
	}
}

void StkPastOrder(BtNode *ptr)
{
	if(ptr == NULL) return ;
	StkNode node;

	stack<StkNode> st;
	st.push(StkNode(ptr));
	while(!st.empty())
	{
		node = st.top(); st.pop();
		
		if (++node.popnum == 3)
		{
			cout<<node.pnode->data<<" ";
			continue;
		}

		st.push(node);
		if (node.popnum == 1 && node.pnode->leftchild != NULL)
		{
			st.push(StkNode(node.pnode->leftchild));
		}else if (node.popnum == 2 && node.pnode->rightchild != NULL)
		{
			st.push(StkNode(node.pnode->rightchild));
		}
	}
}
//无意义
void StkPreOrder(BtNode *ptr)
{
	if(ptr == NULL) return ;
	StkNode node;

	stack<StkNode> st;
	st.push(StkNode(ptr));
	while(!st.empty())
	{
		node = st.top(); st.pop();
		cout<<node.pnode->data<<" ";
		if(node.pnode->rightchild != NULL)
		{
			st.push(node.pnode->rightchild);
		}
		if(node.pnode->leftchild != NULL)
		{
			st.push(node.pnode->leftchild);
		}
	}
}

int Size(BtNode *ptr)
{
	if (ptr != NULL)
	{
		return Size(ptr->leftchild)+Size(ptr->rightchild)+1;
	}
	return 0;
}
int Max(int a, int b)
{
	return a>b ?a:b;
}

int Depth(BtNode *ptr)
{
	if (ptr != NULL)
	{
		return Max(Depth(ptr->leftchild), Depth(ptr->rightchild))+1;
	}
	return 0;
}

BtNode* FindValue(BtNode *ptr, ElemType x)
{
	if (ptr == NULL || ptr->data == x)return ptr;
	else
	{
		BtNode *p = FindValue(ptr->leftchild, x);
		if (NULL == p)
		{
			p = FindValue(ptr->rightchild, x);
		}
		return p;
	}
}

bool Is_Full_BinaryTree(BtNode *ptr)
{
	if (ptr == NULL)
	{
		return true;
	}
	else if(ptr->leftchild==NULL&&ptr->rightchild==NULL)
	{
		return true;
	}
	else if(ptr->leftchild!=NULL&&ptr->rightchild!=NULL)
	{
		return Is_Full_BinaryTree(ptr->leftchild) &&Is_Full_BinaryTree(ptr->rightchild)&&Depth(ptr->leftchild) == Depth(ptr->rightchild)
				&&Size(ptr->leftchild) == Size(ptr->rightchild);
	}
	else
	{
		return false;
	}
}

bool Is_Full_BinaryTree2(BtNode *ptr)
{
	return Is_Full_BinaryTree(ptr->leftchild) && Is_Full_BinaryTree(ptr->rightchild);
}

//根据空洞判断(若为完全二叉树则null在层次遍历所有节点最后出现)
bool Is_Comp_BinaryTree(BtNode *ptr)
{
	queue<BtNode *> que;
	que.push(ptr);

	//层次遍历，将NULL入队
	while((ptr = que.front())!= NULL)
	{
		que.pop();
		que.push(ptr->leftchild);
		que.push(ptr->rightchild);
	}

	//判断是否还有未被访问到的节点  
	while(!que.empty())
	{
		ptr = que.front();
		que.pop();

		//前面有空节点，不是完全二叉树
		if(ptr != NULL)
		{
			return false;
		}
	}
	return true;
}

BtNode* Parent(BtNode  *ptr, BtNode *child)
{
	if (ptr == NULL || ptr->leftchild == child || ptr->rightchild == child)
		return ptr;
	else
	{
		BtNode *p = Parent(ptr->leftchild, child);
		if(NULL == p)
		{
			p = Parent(ptr->rightchild, child);
		}
		return p;
	}
}

BtNode* FindParent(BtNode *ptr, BtNode *child)
{
	//注意此处判断条件,只有一个根节点时,无父节点
	if (ptr == NULL || child == NULL || ptr == child)
		return NULL;
	else
		return Parent(ptr, child);
}
//求二叉树任一条最长路径，并输出结点
void PathMaxLenth(BtNode *ptr)
{
	if(ptr != NULL)
	{
		cout<<ptr->data<<" ";
		if(Depth(ptr->leftchild) > Depth(ptr->rightchild))
		{
			PathMaxLenth(ptr->leftchild);
		}
		else
		{
			PathMaxLenth(ptr->rightchild);
		}
	}
}
bool Is_Balance_BinaryTree(BtNode *p)
{ 
	bool res = false;
	if(p != NULL)
	{
		if(abs(Depth(p->leftchild)-Depth(p->rightchild))<=1)
		{
			res = true;
		}
	}
	return res;
}

//找最近的公共结点（普通二叉树）搜索二叉树&带有parent无root(hash逆向存储/链表的最后一个公共结点)的简单
BtNode * NearParent(BtNode *ptr, BtNode *child1,BtNode *child2)
{
	if(ptr == NULL || child1 == NULL || child2 == NULL)
		return NULL;
	if(ptr == child1 || ptr == child2)
		return ptr;

	BtNode *left = NearParent(ptr->leftchild, child1, child2);
	BtNode *right = NearParent(ptr->rightchild, child1, child2);
	if(left && right)
		return ptr;
	return left ? left : right;
}


//打第K行的数据
void Print_K_Level_Item(BtNode *ptr, int k)
{
	if (ptr == NULL || k < 0 || k > Depth(ptr))
	{
		return ;
	}
	if (k == 0 && ptr != NULL)
	{
		cout<<ptr->data<<" ";
	}else
	{
		Print_K_Level_Item(ptr->leftchild, k-1);
		Print_K_Level_Item(ptr->rightchild, k-1);
	}
}
//层次遍历
void LevelOrder(BtNode *ptr)
{
	int n = Depth(ptr);
	if (ptr == NULL)
	{
		return ;
	}
	for(int i=0; i<n; ++i)
	{
		Print_K_Level_Item(ptr, i);
	}
	cout<<endl;
}

void NiceLevelOrder(BtNode *ptr)
{
	if (ptr == NULL){return ;}

	queue<BtNode*> que;
	que.push(ptr);
	while (!que.empty())
	{
		ptr = que.front(); que.pop();
		cout<<ptr->data<<" ";
		
		if(ptr->leftchild != NULL)
		{
			que.push(ptr->leftchild);
		}
		if (ptr->rightchild != NULL)
		{
			que.push(ptr->rightchild);
		}
	}
	cout<<endl;
}

void main()
{
	//创建1
	char *str = "ABC##DE##F##G#H##";
	BinaryTree root = NULL;
	/*
	root = CreateTree1();
	
	CreateTree2(root);
	CreateTree3(&root);
	root = CreateTree4(str);
	root = CreateTree5(&str);
	*/
	//创建2
	char ps[] = {"ABCDEFGH"};
	char is[] = {"CBEDFAGH"};
	char ls[] = {"CEFDBHGA"};
	int n = sizeof(is)/sizeof(is[0])-1;
 	
	root = CreateTreePI(ps, is, n);
	/*
	root = CreateTreeLI(is, ls, n);

	PreOrder(root); cout<<endl;
	InOrder(root); cout<<endl;
	PastOrder(root); cout<<endl;

	NiceInOrder2(root); cout<<endl;
	StkInOrder(root); cout<<endl;
	NiceInOrder(root); cout<<endl;

	StkPastOrder(root); cout<<endl;
	NicePastOrder(root); cout<<endl;

	StkPreOrder(root); cout<<endl;
	NicePreOrder(root); cout<<endl;
	
	cout<<Is_Full_BinaryTree2(root)<<endl;
	PathMaxLenth(root); cout<<endl;
	cout<<Is_Balance_BinaryTree(root)<<endl;

	BtNode *child1 = FindValue(root, 'G');
	BtNode *child2 = FindValue(root, 'E');
	BtNode *ancestor= NearParent(root, child1, child2);
	if (ancestor != NULL)
	{
		cout<<ancestor->data<<endl;
	}
	
	cout<<Is_Comp_BinaryTree(root)<<endl;
	*/
}


#if 0
void PreOrder(BtNode *ptr)
{
	if (ptr != NULL)
	{
		cout<<ptr->data<<" ";
		PreOrder(ptr->leftchild);
		PreOrder(ptr->rightchild);
	}
}
void InOrder(BtNode *ptr)
{
	if (ptr != NULL)
	{
		InOrder(ptr->leftchild);
		cout<<ptr->data<<" ";
		InOrder(ptr->rightchild);
	}
}

//根在中间
//方一：注意mid两种用途
BtNode *CreateTree(int *ar, int len)
{
	BtNode *s = NULL;
	if (ar != NULL && len > 0)
	{
		int n = len/2;
		s = BuyNode(); 
		s->data = ar[n]; //不能减1，会出现负数
		s->leftchild = CreateTree(ar, n);
		s->rightchild = CreateTree(ar+n+1, len-n-1);
	}
	return s;
}
//方二: left right
BtNode *CreateTree2(int *ar, int left, int right)
{
	BtNode *s = NULL;
	if(ar != NULL && left <= right)
	{
		int mid = (right-left)/2+left;
		s = BuyNode();
		s->data = ar[mid];
		s->leftchild = CreateTree2(ar, left, mid-1);
		s->rightchild = CreateTree2(ar, mid+1, right);
	}
	return s;
}
void main()
{
	int ar[] = {12, 23, 34, 45, 56, 67, 78, 89, 90, 100};
	int n = sizeof(ar)/sizeof(ar[0]);

	BinaryTree root = NULL;
	
	root = CreateTree2(ar, 0, n-1);
	PreOrder(root);
}

//中序打印
void PrintInOrder(int *arr, int i,int j)
{
	if (arr != NULL&&i < j)
	{
		PrintInOrder(arr, 2 * i + 1, j);
		cout << arr[i] << " ";
		PrintInOrder(arr, 2 * i + 2, j);
	}
}
void NicePrintInOrder(int *arr, int i, int j)
{
	if (arr == NULL||i>j)
		return;
	stack<int> st;
	while (!st.empty()||arr!=NULL&&i<j)
	{
		while (i < j)
		{
			st.push(i);
			i = i * 2 + 1;
		}
		i = st.top();
		st.pop();
		cout << arr[i] << " ";
		i = i * 2 + 2;
	}
	cout << endl;
}


void main()
{
	//数组桉树的结构中序打印
	int ar[] = { 31,23,12,66,94,5,17,70,62,49,55,88};
	int n = sizeof(ar) / sizeof(ar[0]);
	PrintInOrder(ar, 0, n);
	cout << endl;
	NicePrintInOrder(ar, 0, n );
}
#endif

/*
//节点个数
static int _MySize(BtNode *ptr)
{
	if (ptr != NULL)
	{
		return _MySize(ptr->leftchild)+_MySize(ptr->rightchild)+1;
	}
	return 0;
}
int MySize(BtNode *ptr)
{
 
	if (ptr != NULL)
	{
		return _MySize(ptr);	
	}
}
//深度AB##C##(2)
static int _MyDepth(BtNode *ptr)
{	
	int max = 0;
	int k = 0;
	if (ptr != NULL)
	{
		max = _MyDepth(ptr->leftchild);//左子树深度
		k = _MyDepth(ptr->rightchild);//右子树深度
		if (k > max)
		{
			max = k;
		}
		return max+1;
	}
	return 0;
}
int MyDepth(BtNode *ptr)
{
	if (ptr != NULL)
	{
		return _MyDepth(ptr);
	}
}

BtNode* FindValue(BtNode *ptr, ElemType x)
{
	if (ptr != NULL)
	{
		if (ptr->data == x)
		{
			return ptr;
		}
		//瘸腿(如下用临时变量带回)
		if(FindValue(ptr->leftchild,x) != NULL)//多个相同此节点？
		{
			return FindValue(ptr->leftchild, x);//不能阻止右子树递归
		}
		return FindValue(ptr->rightchild, x);
	}
	else
	{
		return NULL;//没找到
	}
}
*/

