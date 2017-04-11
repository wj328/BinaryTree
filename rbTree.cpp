#include <iostream>
using namespace std;

template<typename Type>
class RB_Tree
{	
private:
	typedef enum{RED = 0, BLACK = 1}ColorType;
	struct rb_node
	{
		rb_node *parent;
		rb_node *leftchild;
		rb_node *rightchild;
		ColorType color;
		Type value;
	};

	rb_node *head;//parent->root,left->min,right->max
	static rb_node *nilptr;
	unsigned int size;
private:
	rb_node* BuyNode(rb_node *p, ColorType color)
	{
		rb_node *s = (rb_node*)malloc(sizeof(rb_node));
		if(NULL == s) exit(1);
		memset(s, 0, sizeof(rb_node));
		s->parent = p;
		s->color = color;
		return s;
	}
	void init()
	{
		if(nilptr == NULL)
		{
			nilptr = BuyNode(NULL, BLACK);
		}
		head = BuyNode(nilptr, RED);
		head->leftchild = head->rightchild = nilptr;
	}
public:
	rb_node* Root(){return head->parent;}
	RB_Tree():size(0)
	{
		init();
	}
	void Lrotate(rb_node *_X)
	{
		rb_node *_Y = _X->rightchild;//新根
		_X->rightchild = _Y->leftchild;
		if(_Y->leftchild != nilptr)
		{
			_Y->leftchild->parent = _X;
		}

		if(_X == Root())
		{
			head->parent = _Y;
		}
		else
		{
			if(_X == _X->parent->leftchild)
			{
				_X->parent->leftchild = _Y;
			}
			else
			{
				_X->parent->rightchild = _Y;
			}
		}
		_Y->parent = _X->parent;

		_Y->leftchild = _X;
		_X->parent = _Y;
	}
	void Rrotate(rb_node *_X)
	{
		rb_node *_Y = _X->leftchild;//_Y新根
		_X->leftchild = _Y->rightchild;
		if(_Y->rightchild != nilptr)
		{
			_Y->rightchild->parent = _X;//1
		}

		if(_X == Root())
		{
			head->parent = _Y;
		}
		else
		{
			if(_X == _X->parent->leftchild)
			{
				_X->parent->leftchild = _Y;
			}
			else
			{
				_X->parent->rightchild = _Y;
			}
		}
		_Y->parent = _X->parent;//2

		_Y->rightchild = _X;
		_X->parent = _Y;//3
	}
	void insert(rb_node *_Y, const Type &x)
	{
		//_Z：要插入的节点
		rb_node *_Z  = BuyNode(_Y, RED);
		_Z->leftchild = _Z->rightchild = nilptr;
		_Z->value = x;

		if(_Y == head)
		{//NULL rb_tree
			_Y->parent = _Z;
			_Y->leftchild = _Y->rightchild = _Z;//维护head左右
		}
		else
		{//not NULL
			if(_Z->value < _Y->value)
			{
				_Y->leftchild = _Z;
			}
			else
			{
				_Y->rightchild = _Z;
			}
		}
		//维护head左右
		if(_Z->value < head->leftchild->value)
		{
			head->leftchild = _Z;
		}
		else if(_Z->value > head->rightchild->value)
		{
			head->rightchild = _Z;
		}
		
		//调整的条件（两个红结点不能相邻）
		for(rb_node *_X=_Z; _X!=Root() && _X->parent->color==RED; )
		{
			//对_X->pa->pa为左边插入
			if(_X->parent == _X->parent->parent->leftchild)
			{
				_Y = _X->parent->parent->rightchild;
				
				//两种处理方法
				if(_Y->color == RED)//只改变颜色
				{
					_X->parent->parent->color = RED;
					_X->parent->color = BLACK;
					_Y->color = BLACK;
					_X = _X->parent->parent;//回溯
				}
				else//旋转
				{
					//判断单旋还是双旋
					if(_X == _X->parent->rightchild)
					{
						_X = _X->parent;//***
						Lrotate(_X);
					}
					_X->parent->color = BLACK;
					_X->parent->parent->color = RED;
					Rrotate(_X->parent->parent);
				}
			}
			else
			{
				_Y = _X->parent->parent->leftchild;
				if(_Y->color == RED)
				{
					_X->parent->parent->color = RED;
					_X->parent->color = BLACK;
					_Y->color = BLACK;
					_X = _X->parent->parent;
				}
				else
				{
					if(_X == _X->parent->leftchild)
					{
						_X = _X->parent;
						Rrotate(_X);
					}
					_X->parent->color = BLACK;
					_X->parent->parent->color = RED;
					Lrotate(_X->parent->parent);
				}
			}
			
		}
		//根节点始终为黑色结点
		Root()->color = BLACK; 
	}
	bool insert(const Type &x)
	{
		//_Y为最终插入节点的父节点
		rb_node *_Y = head;
		rb_node *_X = Root();
		while(_X != nilptr)
		{
			_Y = _X;
			_X = x>_X->value?_X->rightchild:_X->leftchild;
			if(x == _X->value)//避免重复插入
			{
				return false;
			}
		}
		//认为不对，无法和中间节点判断，验证确实
		//if(_Y != NULL && _Y != head && _Y->value == x)//避免重复插入
		//{
		//	return false;
		//}
		insert(_Y, x);
		return true;
	}
	void inorder(rb_node *p)const
	{
		if(p != nilptr)
		{
			inorder(p->leftchild);
			if(p->color == RED)
			{
				cout<<p->value<<"RED"<<" ";
			}
			else
			{
				cout<<p->value<<"BLACK"<<" ";
			}
			inorder(p->rightchild);
		}
	}
	void inorder()const
	{
		if(head->parent != nilptr)
		{
			inorder(head->parent);	
		}
		cout<<endl;
	}
};

template<typename Type>
typename RB_Tree<Type>::rb_node *RB_Tree<Type>::nilptr = NULL;

int main()
{
	RB_Tree<int> rbtree;
	int x;
	while(cin>>x, x!=-1)
	{
		cout<<rbtree.insert(x)<<endl;
		rbtree.inorder();
	}
	return 0;
}

//删除 黑结点需要调整（个数改变）同BST树
//1.空树2.没找到3.叶子、单分支、双分支（删RED,BLACK）