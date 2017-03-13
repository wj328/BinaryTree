#include <iostream>
using namespace std;

/*
内存池
*/
#if 0
const int ITEM_SIZE=1000;

template <typename T>
class Queue
{
public:
	Queue();
	~Queue();
	void addQue(const T &val);
	void delQue();
	T top()const;
	bool empty()const;
private:
	class QueueItem
	{
	public:
		QueueItem(T data=T());
		void* operator new(size_t size);
		void operator delete(void *ptr);

		static QueueItem *mpfreelist;//多个结点对象共用的
		T mdata;
		QueueItem *mpnext;
	};
	QueueItem *mpfront;
	QueueItem *mprear;
};

//mpfreelist的初始化
template <typename T>
typename Queue<T>::QueueItem *Queue<T>::QueueItem::mpfreelist = NULL;

template <typename T>
Queue<T>::QueueItem::QueueItem(T data=T()):mdata(data),mpnext(NULL){}
/*
当频繁调用系统new和delete时我们需要 内存池，减少调用系统new和delete的次数
开两个内存池是不会造成内存泄露的，虽mpfreelist指向新的，但和旧的是有联系的
释放不会造成内存泄露
内存池一般不释放，除非进程结束，这时资源全部归还系统
*/
template <typename T>
void* Queue<T>::QueueItem::operator new(size_t size)
{
	QueueItem *p = mpfreelist;
	if (!mpfreelist)
	{
		cout<<"new"<<endl;//查看开了几次内存池

		
		//mpfreelist = new QueueItem[10000],new QueueItem时调用new重载，递归调用
		int allocsize = ITEM_SIZE*size;
		mpfreelist = (QueueItem*)new char[allocsize];

		//形成链表
		p = mpfreelist;
		for (; p<mpfreelist+ITEM_SIZE-1; ++p)
		{ 
			p->mpnext = p+1;//指针加1是按照类型大小进行加的
		}	
		p->mpnext = NULL;//最后一个结点的指针域置空(一直添加至内存池满)
	}

	p = mpfreelist;
	mpfreelist = mpfreelist->mpnext;
	return p;
}

template <typename T>
void Queue<T>::QueueItem::operator delete(void *ptr)
{
	cout<<"delete"<<endl;
	if (ptr == NULL)
	{
		return;
	}

	QueueItem *p = (QueueItem *)ptr;
	p->mpnext = mpfreelist;
	mpfreelist = p;
}

template <typename T>
Queue<T>::Queue()
{
	mpfront = mprear = new QueueItem;
}

template <typename T>
Queue<T>::~Queue()
{
	QueueItem *pcur = mpfront;
	while (pcur != NULL)
	{
		mpfront = mpfront->mpnext;
		delete pcur;

		pcur = mpfront;
	}
}

template <typename T>
void Queue<T>::addQue(const T &val)
{
	QueueItem *ptmp = new QueueItem(val);

	mprear->mpnext = ptmp;
	mprear = ptmp;
}

template <typename T>
void Queue<T>::delQue()
{
	if (empty())
	{
		return;
	}
	
	QueueItem *pcur = mpfront->mpnext;
	if (pcur->mpnext == NULL)
	{
		mprear = mpfront;
	}

	mpfront->mpnext = pcur->mpnext;
	delete pcur;
}

template <typename T>
T Queue<T>::top()const
{
	return mpfront->mpnext->mdata;
}

template <typename T>
bool Queue<T>::empty()const
{
	return mpfront == mprear;
}

int main()
{
	Queue<int> queue;
	for (int i=10; i<20; ++i)
	{
		queue.addQue(i);
	}

	int tmp;
	for (int i=0; i<10; ++i)
	{
		tmp = queue.top();
		cout<<tmp<<endl;
		queue.delQue();
	}

	return 0;
} 
#endif

#if 0
/*
new,delete运算符重载
数组
*/
void* operator new(size_t size)
{
	void *p = NULL;
	if ((p = malloc(size)) == NULL)
		throw "";  // bad_alloc
	return p;
}

void operator delete(void *ptr)
{
	if (ptr == NULL)
		return ;
	free(ptr);
}

//size为申请空间的总大小
void* operator new[](size_t size)
{
	void *p = NULL;
	if ((p = malloc(size)) == NULL)
		throw "";
	return p;
}

void operator delete[](void *ptr)
{
	if (ptr == NULL)
		return ;
	free(ptr);
}

int main()
{
	int *p = new int;
	delete p;

	int *q = new int[10];
	delete []q;

	return 0;
}
#endif
/*
[]与单个混用
内置类型
自定义类型
inline release版本 debug版本
不提供析构时new[],main输出的p相同，反之
*/

class Test
{
public:
	Test(){cout<<"Test()"<<endl;}
	~Test(){cout<<"~Test()"<<endl;}
	void* operator new(size_t size)
	{
		void *p = NULL;
		if ((p = malloc(size)) == NULL)
			throw "";  // bad_alloc
		cout<<"new:"<<p<<endl;
		return p;
	}

	void operator delete(void *ptr)
	{
		if (ptr == NULL)
			return ;
		cout<<"delete ptr:"<<ptr<<endl;
		free(ptr);
	}

	void* operator new[](size_t size)
	{
		void *p = NULL;
		if ((p = malloc(size)) == NULL)
			throw "";
		cout<<"new[]:"<<p<<endl;
		return p;
	}

	void operator delete[](void *ptr)
	{
		if (ptr == NULL)
			return ;
		cout<<"delete[] ptr:"<<ptr<<endl;
		free(ptr);
	}
private:
	int ma;
};

/*
release版本
delete []p:push  offset Test::~Test (0B91000h) 
delete q:push offset string "~Test()" (272154h) 
*/
int main()
{
	Test *p = new Test;
	cout<<"main:"<<p<<endl;
	delete []p; //调用delete[],当数组处理,多释放了4字节

	Test *q = new Test[10];
	cout<<"main:"<<q<<endl;//比new[]中q大4,之下,4byte存放个数,实际开空间44
	delete []q;//释放时是q,只释放了40字节

	
	//告诉了起始地址,不会有内存泄露,析构出问题
	//Test *p = new Test;
	//delete (char*)p;

	//char *q = (char*)new Test;
	//delete q;

	/*
	内存释放有问题,和不提供析构(release版本)的情况一样,只是不会报错
	int *p = new int;
	cout<<"main:"<<p<<endl;
	delete []p;

	int *q = new int[10];
	cout<<"main:"<<q<<endl;
	delete q;
	*/

	return 0;
}

