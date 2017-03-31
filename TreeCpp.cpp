#include <iostream>
#include <stack>
#include <queue>
using namespace std;

template <typename _T>
class BinaryTree
{
protected:
	struct BtNode
	{
		BtNode *leftchild;
		BtNode *rightchild;
		_T data;
	};
private:
	BtNode *root;
	_T RefValue;//#define END '#'
	static BtNode* BuyNode()
	{
		BtNode* s = (BtNode*)malloc(sizeof(BtNode));
		if(NULL == s)exit(1);
		memset(s,0,sizeof(BtNode));
		return s;
	}
	static void FreeNode(BtNode *p)
	{
		free(p);
	}
	BtNode* Create(const _T *&str)
	{
		BtNode *s = NULL;
		if(*str != RefValue)
		{
			s = BuyNode();
			s->data = *str;
			s->leftchild = Create(++str);
			s->rightchild = Create(++str);
		}
		return s;
	}
	static void Clear(BtNode* p)
	{
		if(p != NULL)
		{
			Clear(p->leftchild);
			Clear(p->rightchild);
			FreeNode(p);
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
	static BtNode* Copy(BtNode *p)
	{
		BtNode *s = NULL;
		if(p != NULL)
		{
			s = BuyNode();
			s->data = p->data;
			s->leftchild = Copy(p->leftchild);
			s->rightchild = Copy(p->rightchild);
		}
		return s;
	}
	BtNode* FindValue(BtNode *p, const _T &x)
	{
		if(p == NULL || p->data == x) return p;
		else
		{
			BtNode *pnode = FindValue(p->leftchild, x);
			if(pnode == NULL)
			{
				pnode = FindValue(p->rightchild, x);
			}
			return pnode;
		}
	}
	bool equal(BtNode *pa, BtNode *pb)
	{
		return (pa == NULL && pb == NULL) ||
			   (pa != NULL && pb != NULL &&
			    pa->data == pb->data     &&
				equal(pa->leftchild, pb->leftchild)&&
				equal(pa->rightchild, pb->rightchild)); 
	}
public:
	typedef struct BtNode* PNode;
	BtNode* GetRoot()const{return root;}
public:
	BinaryTree(const _T &x):root(NULL),RefValue(x){}
	~BinaryTree()
	{
		Clear();
	}
	BinaryTree(const BinaryTree<_T> &src)
	{
		root = Copy(src.root);
		RefValue = src.RefValue;
	}
	BinaryTree<_T> operator=(const BinaryTree<_T> &src)
	{
		if(this != &src)
		{
			Clear();
			root = Copy(src.root);
			RefValue = src.RefValue;
		}
		return *this;
	}
	void CreateTree(const _T *str)
	{
		if(str != NULL)
		{
			root = Create(str);
		}
	}
	void Clear()
	{
		Clear(root);
		root = NULL;
	}
	void InOrder()
	{
		InOrder(root);
		cout<<endl;
	}
	BtNode* FindValue(const _T &x)const
	{
		return FindValue(root, x);
	}
	bool operator==(const BinaryTree<_T> &src)const
	{
		return equal(this->root, src.root);
	}
	bool operator!=(const BinaryTree<_T> &src)const
	{
		return !(*this == src);
	}
};

template<typename _T>
class TreeIterator
{
protected:
	typename BinaryTree<_T>::PNode _Ptr;
	BinaryTree<_T> &tree;
public:
	TreeIterator(BinaryTree<_T> &bt):tree(bt), _Ptr(NULL){}
	virtual ~TreeIterator(){}
	_T& operator*(){return _Ptr->data;}
	const _T& operator*()const {return _Ptr->data;}
	bool IsDone()const {return _Ptr == NULL;}
	virtual void First()=0;
	virtual void operator++()=0;
};

template<typename _T>
class PreIterator:public TreeIterator<_T>
{
protected:
	stack<typename BinaryTree<_T>::PNode> st;
public:
	PreIterator(BinaryTree<_T> &bt):TreeIterator<_T>(bt){}
	~PreIterator(){}
	void First()
	{
		if(tree.GetRoot())
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
		_Ptr = st.top();
		st.pop();
		if(_Ptr->rightchild != NULL)
		{
			st.push(_Ptr->rightchild);
		}
		if(_Ptr->leftchild != NULL)
		{
			st.push(_Ptr->leftchild);
		}
	}
};

template<typename _T>
class LevelIterator:public TreeIterator<_T>
{
protected:
	queue<typename BinaryTree<_T>::PNode> que;
public:
	LevelIterator(BinaryTree<_T> &bt):TreeIterator(bt){}
	~LevelIterator(){}
	void First()
	{
		if(tree.GetRoot())
		{
			que.push(tree.GetRoot());
			operator++();
		}
	}
	void operator++()
	{
		if(que.empty())
		{
			_Ptr = NULL;
			return ;
		}
		_Ptr = que.front();
		que.pop();
		if(_Ptr->leftchild != NULL)
		{
			que.push(_Ptr->leftchild);
		}
		if(_Ptr->rightchild != NULL)
		{
			que.push(_Ptr->rightchild);
		}
	}
};

template<typename _T>
class SInIterator:public TreeIterator<_T>
{
protected:
	stack<typename BinaryTree<_T>::PNode> st;
	typename BinaryTree<_T>::PNode p;
public:
	SInIterator(BinaryTree<_T> &bt):TreeIterator<_T>(bt){}
	~SInIterator(){}
	void First()
	{
		if(tree.GetRoot())
		{
			p = tree.GetRoot();
			operator++();
		}
	}
	void operator++()
	{
		if(p == NULL && st.empty())
		{		
			_Ptr = NULL;//IsDone
			return ;
		}
		while(p != NULL)
		{
			st.push(p);
			p = p->leftchild;
		}
		p = st.top(); st.pop();
		_Ptr = p;//*_Ptr
		p = p->rightchild;
	}
};

template<typename _T>
class SPastIterator:public SInIterator<_T>
{
protected:
	typename BinaryTree<_T>::PNode tag;
public:
	SPastIterator(BinaryTree<_T> &bt):SInIterator(bt){}
	~SPastIterator(){}
	void First()
	{
		if(tree.GetRoot())
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
			return ;
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

template<typename _T>
struct StkNode
{
	typename BinaryTree<_T>::PNode pnode;
	int popnum;
public:
	StkNode(typename BinaryTree<_T>::PNode p = NULL):pnode(p),popnum(0){}
};
template<typename _T>
class InIterator:public TreeIterator<_T>
{
protected:
	stack<StkNode<_T>> st;
public:
	InIterator(BinaryTree<_T> &bt):TreeIterator(bt){}
	~InIterator(){}
	void First()
	{
		if(tree.GetRoot())
		{
			st.push(StkNode<_T>(tree.GetRoot()));
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
		for(; ;)
		{
			StkNode<_T> node = st.top(); st.pop();
			if(++node.popnum ==2)
			{
				_Ptr = node.pnode;
				if(node.pnode->rightchild != NULL)
				{
					st.push(StkNode<_T>(node.pnode->rightchild));
				}
				return ;
			}
			st.push(node);
			if(node.popnum == 1 && node.pnode->leftchild != NULL)
			{
				st.push(StkNode<_T>(node.pnode->leftchild));
			}
		}
	}
};
template<typename _T>
class PastIterator:public InIterator<_T>
{
public:
	PastIterator(BinaryTree<_T> &bt):InIterator<_T>(bt){}
	~PastIterator(){}
	void First()
	{
		if(tree.GetRoot())
		{
			st.push(StkNode<_T>(tree.GetRoot()));
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
		for(; ;)
		{
			StkNode<_T> node = st.top(); st.pop();
			if(++node.popnum == 3)
			{
				_Ptr = node.pnode;
				return ;
			}
			st.push(node);
			if(node.popnum == 1 && node.pnode->leftchild != NULL)
			{
				st.push(StkNode<_T>(node.pnode->leftchild));
			}else if(node.popnum == 2 && node.pnode->rightchild != NULL)
			{
				st.push(StkNode<_T>(node.pnode->rightchild));
			}
		}
	
	}
};
template<typename _T>
void Print_Tree(TreeIterator<_T> &bt)
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
	char *str = "ABC##DE##F##G#H##";
	int ar[] = {1,2,3,-1,-1,4,5,-1,-1,6,-1,-1,7,-1,8,-1,-1};
	
	BinaryTree<int> myt2(-1);
	myt2.CreateTree(ar);
	PreIterator<int> pr(myt2);
	Print_Tree(pr);

	BinaryTree<char> myt('#');
	myt.CreateTree(str);

	PreIterator<char> pi(myt);
	LevelIterator<char> li(myt);
	SInIterator<char> sii(myt);
	InIterator<char> si(myt); 
 	SPastIterator<char> spi(myt);
	PastIterator<char> pii(myt);

	Print_Tree(pi);
	Print_Tree(li);
	Print_Tree(sii);
	Print_Tree(si);
	Print_Tree(spi);
	Print_Tree(pii);
}
