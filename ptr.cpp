#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <memory>
using namespace std;
//using std::tr1::shared_ptr;

/*
智能指针
1.特点：栈上的对象出了作用域会自动析构
智能指针对象不要放在堆上
带引用计数(强智能指针) :当有 循环引用时析构不了 最后验证的A B类
*/

/*
单例模式:此类最终只能生成一个对象
首先 私有化构造函数 这样用户就不能在外边定义对象
*/

class CHeapTable
{
public:
	//获取单例对象的地址 产生对象 所以不依赖于对象 为static
	static CHeapTable* getInstance()//instance实例
	{
		static CHeapTable table;//调用构造函数 static初始化只一次
		return &table;
	}
	//如何利用算法来查找 与ptr相匹配的结点？
	void addRef(void *ptr)
	{
		#if 0
		vector<Node>::iterator it = vec.begin();
		for (; it != vec.end(); ++it)
		{
			if (ptr == it->mpaddr)
			{
				cout<<"add"<<endl;
				it->mcount++;
				return ;
			}
		}
		#endif
		
		vector<Node>::iterator it = find(vec.begin(), vec.end(), ptr);
		if (it != vec.end())
		{
			cout<<"Ref"<<endl;
			it->mcount++;
			return ;
		}

		vec.push_back(*(new Node(ptr)));
	}

	void delRef(void *ptr)
	{
		vector<Node>::iterator it = vec.begin();
		for(; it != vec.end(); ++it)
		{
			if (ptr == it->mpaddr)
			{
				cout<<"del"<<endl;
				it->mcount--;
			}
		}
	}

	int getRef(void *ptr)
	{
		vector<Node>::iterator it = vec.begin();
		for (; it != vec.end(); ++it)
		{
			if (ptr == it->mpaddr)
			{
				return it->mcount;
			}
		}
		//throw "";会崩溃
		return 0;//释放mptr == NULL 构造的对象 		
	}
private:
	CHeapTable(){}
	struct Node
	{
		Node(void *ptr = NULL):mpaddr(ptr),mcount(1){}
		//给find提供==的运算符重载(*frist == ptr)
		bool operator==(const Node &src)
		{
			return mpaddr == src.mpaddr;
		}
		void *mpaddr;
		int mcount;
	};
	vector<Node> vec;
};

//当getRef() == 0时 说明它是最后一个使用资源的 要负责析构
template <typename T>
class CSmartPtr
{
public:
	CSmartPtr(T *ptr = NULL):mptr(ptr)
	{
		if (mptr != NULL)
		{
			addRef();
		}
	}
	CSmartPtr(const CSmartPtr<T> &src):mptr(src.mptr)
	{
		if (mptr != NULL)
		{
			addRef();
		}
	}
	CSmartPtr<T>& operator=(const CSmartPtr<T> &src)
	{
		if (&src == this)
		{
			return *this;
		}

		delRef();
		if (0 == getRef())
		{
			delete mptr;
		}

		mptr = src.mptr;
		if (mptr != NULL)
		{
			addRef();
		}
	}
	~CSmartPtr()
	{
		delRef();
		if (0 == getRef())
		{
			delete mptr;
			mptr = NULL;
		}
	}

	T& operator*()
	{
		return *mptr;
	}
	const T& operator*()const
	{
		return *mptr;
	}
	T* operator->()
	{
		return mptr;
	}
	T* operator->()const
	{
		return mptr;
	}
	//给mptr增加，减少引用资源
	/*
	void addRef(){mheaptable.addRef(mptr);}
	void delRef(){mheaptable.delRef(mptr);}
	int getRef(){return mheaptable.getRef(mptr);}
	*/
	void addRef(){mheaptable->addRef(mptr);}
	void delRef(){mheaptable->delRef(mptr);}
	int getRef(){return mheaptable->getRef(mptr);}
private:
	T *mptr;
	//static CHeapTable mheaptable;
	static CHeapTable *mheaptable;
};

//template <typename T>
//CHeapTable CSmartPtr<T>::mheaptable = CHeapTable();

template <typename T>
CHeapTable* CSmartPtr<T>::mheaptable = CHeapTable::getInstance();

class Test
{
public:
	Test(){cout<<"Test()"<<endl;}
	~Test(){cout<<"~Test()"<<endl;}
	void func()
	{
		cout<<"Test::func"<<endl;
	}
};

int main()
{
	CSmartPtr<Test> pointer1(new Test);
	CSmartPtr<Test> pointer2(pointer1);
	
	//CSmartPtr<int> pointer2(new int);
	//*pointer2 = 20;
	pointer2->Test::func();

	CSmartPtr<Test> pointer3(new Test);
	pointer3 = pointer2;

	/*
	存在问题:不同类型实例化的CSmartPtr不共用static 表(依赖于类)
	第二个(char)扫描addRef 并未找到相同的 所以重新开一节点 总共析构两次
	但两次释放的是同一块内存 
	解决方法: 
	1.全局的CHeapTable mheaptable;//尽量不用
	2.用单例模式解决
	*/
	
	int *p = new int;
	CSmartPtr<int> pointer4(p);
	CSmartPtr<char> pointer5((char*)p);

	return 0;
}

#if 0

//析构不了的原因是 提供的为强智能指针 
class B;
class A
{
public:
	A(){cout<<"A()"<<endl;}
	~A(){cout<<"~A()"<<endl;}
	CSmartPtr<B> _ptrb;
	//shared_ptr<B> _ptrb; 
};

class B
{
public:
	B(){cout<<"B()"<<endl;}
	~B(){cout<<"~B()"<<endl;}
	CSmartPtr<A> _ptra;
	//shared_ptr<A> _ptra;//如何使用boost库中的智能指针？	
};

int main()
{
	//shared_ptr<A> pa(new A());
	//shared_ptr<B> pb(new B());
	CSmartPtr<A> pa(new A());
	CSmartPtr<B> pb(new B());

	pa->_ptrb = pb;
	pb->_ptra = pa;
	return 0;
}

#endif
#if 0
/*
错误:在堆上开了几次空间 则需要 几个标志位
否则最终只会析构一次
*/
template<typename T>
class CSmartPtr
{
public:
	CSmartPtr(T *ptr = NULL):mptr(ptr)
	{	
		++flag;
		cout<<flag<<endl;
	}
	~CSmartPtr()
	{
		--flag;
		cout<<flag<<endl;
		if (flag == 0)
		{
			delete mptr;
		}
	}

	T& operator*()
	{
		return *mptr;
	}
	const T& operator*()const
	{
		return *mptr;
	}
	T* operator->()
	{

		return mptr;
	}
	const T* operator->()const
	{
		return mptr;
	}
private:
	T *mptr;
	static int flag;
};

template <typename T>
int CSmartPtr<T>::flag = 0;

class Test
{
public:
	Test(){cout<<"Test()"<<endl;}
	~Test(){cout<<"~Test()"<<endl;}
	void func()
	{
		cout<<"Test::func"<<endl;
	}
};

int main()
{
	CSmartPtr<Test> pointer1(new Test);
	CSmartPtr<Test> pointer2(pointer1);
	//*pointer2 = 20;
	//pointer2->Test::func();

	CSmartPtr<Test> pointer3(new Test);
	pointer3 = pointer2;

	return 0;
}
#endif
