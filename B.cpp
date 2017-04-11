#include<iostream>
using namespace std;

#define M 5//最好为奇数，方便分裂
#define MAX (M-1) //0位不存数据，个数大于4为5时需要分裂
#define MIN (M/2) //除根的非叶子节点关键字的最少个数

typedef char KeyType;
typedef struct{}Record;

typedef struct 
{
	KeyType key;
	Record *recptr;
}ElemItem;

typedef struct BNode
{
	int num;
	ElemItem data[M+1]; //0不存数据/1 2 3 4 5最后一位用来分裂
	BNode *sub[M+1];//0 1 2 3 4 5
	BNode *parent;
}BNode, *BTree;

typedef struct 
{
	BNode *pnode;
	int index;
	bool tag;
}Result;

BNode* BuyNode()
{
	BNode *s = (BNode*)malloc(sizeof(BNode));
	if(s == NULL) exit(1);
	memset(s,0,sizeof(BNode));
	return s;
}

void FreeNode(BNode *ptr)
{
	free(ptr);
}

Result FindValue(BNode *ptr, KeyType x)//非递归
{
	Result res = {NULL, -1, false};
	while(ptr != NULL)
	{
		int i = ptr->num;
		ptr->data[0].key = x;
		while(x < ptr->data[i].key) --i;
		
		res.pnode = ptr;
		res.index = i;
		if(i>0 && x == ptr->data[i].key)
		{		
			res.tag = true;
			break;
		}
		ptr = ptr->sub[i];
	}
	return res;
}

Result SearchValue(BNode *ptr, KeyType x)
{
	Result res = {NULL, -1, false};
	if(ptr != NULL)
	{
		int i = ptr->num;
		ptr->data[0].key = x;
		while(x < ptr->data[i].key) --i;
		
		res.pnode = ptr;
		res.index = i;
		if(i>0 && x == ptr->data[i].key)
		{
			res.tag = true;
		}
		else if(ptr->sub[i] != NULL)
		{
			res = SearchValue(ptr->sub[i], x);
		}
	}
	return res;
}
//MakeRoot(x, ptr, s)
BNode* MakeRoot(ElemItem x, BNode *left, BNode *right)
{
	BNode *s = BuyNode();
	s->num = 1;
	s->parent = NULL;
	s->data[1] = x;
	s->sub[0] = left;
	if(left != NULL) left->parent = s;//分裂后产生新根会有左右子
	s->sub[1] = right;
	if(right != NULL) right->parent = s;//
	return s;
}
//Insert_Item(ptr, pos, x, NULL);Insert_Item(ptr, pos, x, s)
void Insert_Item(BNode *ptr, int pos, ElemItem x, BNode *right)
{
	for(int i=ptr->num; i>pos; --i)
	{
		ptr->data[i+1] = ptr->data[i];
		ptr->sub[i+1] = ptr->sub[i];
	}
	ptr->data[pos+1] = x;
	ptr->sub[pos+1] = right;
	if(right != NULL) right->parent = ptr;//分裂后往上插入的话有新的右子
	ptr->num += 1;
}
//Move_Item(ptr, s, MIN)
ElemItem Move_Item(BNode *ptr, BNode *s, int pos)//pos为往右转移的个数
{
	for(int i=0, j=pos+1; j<=ptr->num; ++i,++j)
	{
		s->data[i] = ptr->data[j];
		s->sub[i] = ptr->sub[j];
		if(s->sub[i] != NULL)
		{
			s->sub[i]->parent = s;
		}
	}
	s->parent = ptr->parent;
	ptr->num = MIN;
	s->num = MIN;
	return s->data[0];//往上提的值
}
BNode* Splice(BNode *ptr)
{
	BNode *s = BuyNode();//首先申请空间供分裂
	ElemItem x = Move_Item(ptr, s, MIN);//分别为左右子,移动的个数,x为往上插入的值
	if(ptr->parent == NULL)//只有一个叶子的情况
	{
		return MakeRoot(x, ptr, s);
	}
	ptr = ptr->parent;
	int pos = ptr->num;
	ptr->data[0] = x;///////保证插入的位置在0以后
	while(x.key < ptr->data[pos].key) --pos;
	Insert_Item(ptr, pos, x, s);
	if(ptr->num > MAX)
	{
		return Splice(ptr);
	}
	else
	{
		return NULL;
	}
}

bool Insert(BTree *ptree, ElemItem x)
{
	if(NULL == ptree) return false;//NULL
	if(*ptree == NULL)//根
	{
		*ptree = MakeRoot(x,NULL,NULL);
		return true;
	}
	Result res = FindValue(*ptree, x.key);
	if(res.pnode == NULL || res.tag)//错误情况 和 找到了
		return false;
	
	BNode *ptr = res.pnode;
	int pos = res.index;//找到的插入位置pos+1
	Insert_Item(ptr, pos, x, NULL);
	if(ptr->num > MAX)
	{
		BNode *newroot = Splice(ptr);
		if(newroot != NULL)//产生新根
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
		for(int i=1; i<=ptr->num; ++i)
		{
			cout<<ptr->data[i].key;//数值和右孩子下表统一 
			InOrder(ptr->sub[i]);
		}
	}
}

int main()
{
	KeyType ar[]={"qw9erty8uio7pas2dfg3hjk1l5zx4cv6bnm"};
	int n = sizeof(ar)/sizeof(ar[0])-1;//多一个空格
	
	ElemItem item;
	BTree root = NULL;
	for(int i=0; i<n; ++i)
	{
		item.key = ar[i];
		item.recptr = NULL;
		cout<<Insert(&root, item);
	}
	cout<<endl;
	InOrder(root);
	cout<<endl;	
	return 0;
}

#if 0
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
	BNode *nt = FindNext(ptr->sub[pos] );
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
	/*KeyType kx;
	while(cin>>kx, kx != '#')
	{
		cout<<Remove(&root,kx)<<" : ";
		InOrder(root);
		cout<<endl;
	}*/
}
#endif