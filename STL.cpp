#if 0

#include <iostream>
#include <algorithm>//泛型算法
#include <functional>//函数对象
#include <vector>
#include <string>
using namespace std;

//Container为类型
template <typename Container>
void ShowContainer(Container &con)
{
	Container::iterator it = con.begin();//Container<T> 模板
	for (; it!=con.end(); ++it)
	{
		cout<<*it<<" ";
	}
	cout<<endl;
}

int main()
{
	//1.
	vector<int> vec1;
	for (int i=0; i<15; ++i)
	{
		vec1.push_back(rand()%100);
	}
	ShowContainer(vec1);

	//2.由小到大排
	sort(vec1.begin(), vec1.end());	
	ShowContainer(vec1);

	//使用函数对象 由大到小排
	sort(vec1.begin(), vec1.end(), greater<int>());	
	ShowContainer(vec1);

	//第一个小于50的数，并打印出来，采用绑定器和greater
	vector<int>::iterator it1 = find_if(vec1.begin(), vec1.end(), bind1st(greater<int>(), 50));
	if (it1 == vec1.end())
	{
		cout<<"do not exit"<<endl;
	}
	else
	{
		cout<<"exit:"<<*it1<<endl;
	}

	vector<int>::iterator it2 = find_if(vec1.begin(), vec1.end(), not1(bind2nd(greater<int>(), 50)));
	if (it2 == vec1.end())
	{
		cout<<"do not exit"<<endl;
	}
	else
	{
		cout<<"exit:"<<*it2<<endl;
	}

	//第一个小于50 采用绑定器 less 和取反器
	vector<int>::iterator it3 = find_if(vec1.begin(), vec1.end(), bind2nd(less<int>(), 50));
	if (it3 == vec1.end())
	{
		cout<<"do not exit"<<endl;
	}
	else
	{
		cout<<"exit:"<<*it3<<endl;
	}

	vector<int>::iterator it4 = find_if(vec1.begin(), vec1.end(), not1(bind1st(less<int>(), 50)));
	if (it4 == vec1.end())
	{
		cout<<"do not exit"<<endl;
	}
	else
	{
		cout<<"exit:"<<*it4<<endl;
	}
	
	//copy
	vector<int> vec2;
	copy(vec1.begin(), vec1.end(), back_inserter(vec2));
	
	//在新vec2中把40按大小顺序插入  原由小到大排                  bind2nd(greater<int>(), 40)
	vector<int>::iterator it5 = find_if(vec2.begin(), vec2.end(), bind1st(less<int>(), 40));
	vec2.insert(it5, 40);
	ShowContainer(vec2);

	//在新vec2中把40按大小顺序插入  原由大到小排                  bind1st(greater<int>(), 40)
	//vector<int>::iterator it6 = find_if(vec2.begin(), vec2.end(), bind2nd(less<int>(), 40));
	//vec2.insert(it6, 40);
	//ShowContainer(vec2);

	//删50-60之间第一个数[)
	vector<int>::iterator it7 = find_if(vec2.begin(), vec2.end(), bind2nd(greater<int>(), 50));
	if (it7 == vec2.end() || *it7 >= 60)
	{
		cout<<"无元素可删"<<endl;
		exit(0);
	}
	vec2.erase(it7);
	ShowContainer(vec2);

	//删30-60之间所有数 由小到大排
	vector<int>::iterator it8 = find_if(vec2.begin(), vec2.end(), bind2nd(greater<int>(), 30));
	vector<int>::iterator it9 = find_if(vec2.begin(), vec2.end(), bind2nd(greater<int>(), 60));
	if (it8 == vec2.end())
	{
		cout<<"无元素可删"<<endl;
		exit(0);
	}
	vec2.erase(it8, it9);
	ShowContainer(vec2);

	//由大到小排[)
	//vector<int>::iterator it11 = find_if(vec2.begin(), vec2.end(), bind2nd(less<int>(), 30));
	//vector<int>::iterator it12 = find_if(vec2.begin(), vec2.end(), bind2nd(less<int>(), 60));
	//if (it12 == vec2.end())
	//{
	//	cout<<"无元素可删"<<endl;
	//	exit(0);
	//}
	//vec2.erase(it12, it11);////注意
	//ShowContainer(vec2);

	//求长度及清空
	cout<<vec2.size()<<endl;
	vec2.clear();

	ShowContainer(vec2);//为空

	return 0;
}
#endif


/*
标准容器（顺序，关联容器的使用）
*/
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <map>
#include <hash_map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <stack>
using namespace std;


//两个问题：multimap[]运算符重载，<string>实例化输入流结束条件
#if 0
/*
容器适配器
1.stack底层实现deque
2.queue底层实现deque
3.priority_queue
优先级队列，默认less,底层实现vector
greater<int>:数越小优先级越高，处于队头
less<int>相反
*/
int main()
{
  	queue<int> que;
	que.push(23);
	que.push(12);
	que.push(19);
	while (!que.empty())
	{
		cout<<que.front()<<" ";
		que.pop();
	}
	cout<<endl;

	stack<int> sta;
	sta.push(32);
	sta.push(23);
	sta.push(54);
	sta.push(21);
	while (!sta.empty())
	{
		cout<<sta.top()<<" ";
		sta.pop();
	}
	cout<<endl;

	//priority_queue<int> prique;
	priority_queue<int, vector<int>, greater<int>> prique;
	prique.push(12);
	prique.push(2);
	prique.push(15);
	prique.push(9);
	
	while(!prique.empty())
	{
		cout<<prique.top()<<" ";
		prique.pop();
	}
	cout<<endl;
	return 0;
}
#endif

#if 0
/*
vector
make_heap
*/
template <typename Container>
void ShowContainer(const Container &con)
{
	Container::const_iterator it = con.begin();
	for(; it != con.end(); ++it)
	{
		cout<<*it<<" ";
	}
	cout<<endl;
}

int main()
{
	//hash_map<int, string> ha;
	

	//vector
	vector<int> vec;
	for (int i=0; i<10; ++i)
	{
		vec.push_back(rand()%100+2);
	}
	vec.push_back(1000);
	ShowContainer(vec);
/*
	vector<int> vec2;
	//back_inserter(vec2)直接往那个对象后插,不能front_inserter因为vector只有push_back
	//也不能写为vec2.begin(),其默认构造不开空间
	//copy(vec.begin(), vec.end(), back_inserter(vec2));
	copy(vec.begin(), vec.end(), inserter(vec2, vec2.begin()));
	copy(vec2.begin(), vec2.end(), ostream_iterator<int>(cout, " "));
	cout<<endl;
	vec2.clear();

	cout<<"istream vec3 data:"<<endl;
	vector<string> vec3;
	//如何停止输入？？？整形实例化的输入一个非int的停止输入
	copy(istream_iterator<string>(cin), istream_iterator<string>(), back_inserter(vec3));
	copy(vec3.begin(), vec3.end(), ostream_iterator<string>(cout, " "));

	copy(vec.begin(), vec.end(), ostream_iterator<int>(cout," "));
	//直接将vec开始到结尾输出至屏幕
	//用int实例化,传入参数cout,绑定" ",每输出一个空一个字符
	cout<<endl;
*/
	//创建堆<algorithm>
	make_heap(vec.begin(), vec.end(), greater<int>());//小根堆
	copy(vec.begin(), vec.end(), ostream_iterator<int>(cout, " "));
	cout<<endl;

	make_heap(vec.begin(), vec.end(), less<int>());//大根堆
	copy(vec.begin(), vec.end(), ostream_iterator<int>(cout, " "));
	cout<<endl;

	vec.push_back(2000);
	push_heap(vec.begin(), vec.end(), less<int>());//调整大根堆
	copy(vec.begin(), vec.end(), ostream_iterator<int>(cout, " "));
	cout<<endl;
	
	//顶端元素换到最后,重新调整大根堆
	pop_heap(vec.begin(), vec.end(), less<int>());
	vec.pop_back();
	copy(vec.begin(), vec.end(), ostream_iterator<int>(cout, " "));
	cout<<endl;

	return 0;
}
#endif 
#if 0
/*
map  pair(key, val)
multimap
*/
int main()
{
	//map
	map<int, string> mymap;
	
	//make_pair是pair类的函数模板，推演参数类型
	mymap.insert(make_pair(10, "hello"));
	mymap.insert(make_pair(29, "world"));
	mymap.insert(make_pair(20, "aaaa"));//键值和上一个相同,没有插进去
	mymap[27] = "wowowo";

	//不能用流，因为没有->运算符重载
	//copy(mymap.begin(), mymap.end(), ostream_iterator<int, string>(cout, " "));
	map<int, string>::iterator it = mymap.begin();
	for (; it != mymap.end(); ++it)
	{
		//key:it->first,val:it-->second
		cout<<"key:"<<it->first<<"val:"<<it->second;
		cout<<endl;
	}
	
	mymap.erase(20);
	map<int, string>::iterator it2 = mymap.find(27);
	if (it2 != mymap.end())
	{
		mymap.erase(it2);
	}

	it = mymap.begin();
	for (; it != mymap.end(); ++it)
	{
		//key:it->first,val:it-->second
		cout<<"key:"<<it->first<<"val:"<<it->second;
		cout<<endl;
	}

	//multimap
	multimap<int, string, greater<int>> myMultiMap;//改变排序方式
	myMultiMap.insert(make_pair(20, "jijiji"));
	myMultiMap.insert(make_pair(30, "gegege"));
	myMultiMap.insert(make_pair(30, "dadada"));

	//myMultiMap.erase(30);
	//myMultiMap[30] = "lalala";//没有[]运算符重载？？？
	multimap<int, string, greater<int>>::iterator it3 = myMultiMap.begin();
	for (; it3 != myMultiMap.end(); ++it3)
	{
		cout<<it3->first<<" "<<it3->second;
		cout<<endl;
	}
	return 0;
}
#endif

/*
//底层红黑树（二叉排序树）:默认由小到大排序less
set
multiset
*/
int main()
{
	set<int> set1;
	set1.insert(10);
	set1.insert(14);
	set1.insert(30);
	set1.insert(23);
	set1.insert(21);
	set1.insert(30);//未插入
	
	copy(set1.begin(), set1.end(), ostream_iterator<int>(cout, " "));
	cout<<endl;
	cout<<set1.size()<<endl;//5

	multiset<int,greater<int>> myset;//改变排序方式
	//int输入一个字符可以停止输入
	//copy(istream_iterator<int>(cin), istream_iterator<int>(), inserter(myset, myset.begin()));
	//copy(myset.begin(), myset.end(), ostream_iterator<int>(cout, " "))

	myset.insert(10);
	myset.insert(20);
	myset.insert(30);
	myset.insert(20);
	copy(myset.begin(), myset.end(), ostream_iterator<int>(cout, " "));
	cout<<endl;

	cout<<"================"<<endl;

	cout<<myset.count(20)<<endl;//求20的个数
	//myset.erase(20);
	multiset<int, greater<int>>::iterator it = myset.find(20);//只能查找到第一个
	if (it != myset.end())
	{
		myset.erase(it);
	}
	copy(myset.begin(), myset.end(), ostream_iterator<int>(cout, " "));
	cout<<endl;
	
	cout<<"================"<<endl;
	
	myset.insert(79);
	cout<<myset.count(20)<<endl;
	copy(myset.begin(), myset.end(), ostream_iterator<int>(cout, " "));
	cout<<endl;
	cout<<"================"<<endl;
	//set  list
	list<int> mylist;
	//front_inserter为类front_insert_iterator的函数模板
	copy(myset.begin(), myset.end(), front_inserter(mylist));
	copy(mylist.begin(), mylist.end(), ostream_iterator<int>(cout, " "));
	cout<<endl;
	return 0;
 }
