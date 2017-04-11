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
		rb_node *_Y = _X->rightchild;//�¸�
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
		rb_node *_Y = _X->leftchild;//_Y�¸�
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
		//_Z��Ҫ����Ľڵ�
		rb_node *_Z  = BuyNode(_Y, RED);
		_Z->leftchild = _Z->rightchild = nilptr;
		_Z->value = x;

		if(_Y == head)
		{//NULL rb_tree
			_Y->parent = _Z;
			_Y->leftchild = _Y->rightchild = _Z;//ά��head����
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
		//ά��head����
		if(_Z->value < head->leftchild->value)
		{
			head->leftchild = _Z;
		}
		else if(_Z->value > head->rightchild->value)
		{
			head->rightchild = _Z;
		}
		
		//�������������������㲻�����ڣ�
		for(rb_node *_X=_Z; _X!=Root() && _X->parent->color==RED; )
		{
			//��_X->pa->paΪ��߲���
			if(_X->parent == _X->parent->parent->leftchild)
			{
				_Y = _X->parent->parent->rightchild;
				
				//���ִ�����
				if(_Y->color == RED)//ֻ�ı���ɫ
				{
					_X->parent->parent->color = RED;
					_X->parent->color = BLACK;
					_Y->color = BLACK;
					_X = _X->parent->parent;//����
				}
				else//��ת
				{
					//�жϵ�������˫��
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
		//���ڵ�ʼ��Ϊ��ɫ���
		Root()->color = BLACK; 
	}
	bool insert(const Type &x)
	{
		//_YΪ���ղ���ڵ�ĸ��ڵ�
		rb_node *_Y = head;
		rb_node *_X = Root();
		while(_X != nilptr)
		{
			_Y = _X;
			_X = x>_X->value?_X->rightchild:_X->leftchild;
			if(x == _X->value)//�����ظ�����
			{
				return false;
			}
		}
		//��Ϊ���ԣ��޷����м�ڵ��жϣ���֤ȷʵ
		//if(_Y != NULL && _Y != head && _Y->value == x)//�����ظ�����
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

//ɾ�� �ڽ����Ҫ�����������ı䣩ͬBST��
//1.����2.û�ҵ�3.Ҷ�ӡ�����֧��˫��֧��ɾRED,BLACK��