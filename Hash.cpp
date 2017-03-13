#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <functional>
using namespace std;

template <typename T>
class Hash
{
public:
	Hash(int initSize):_buketsNum(initSize){}
	int hash(T &obj)
	{
		int sum = 0;
		int size = sizeof(T);
		for (int i=0; i<size; ++i)
		{
			sum += *(char*)&obj +i; 
		}
		if (sum < 0)
		{
			sum = -sum;
		}
		return sum%_buketsNum;
	}
	void setBucketNum(int num)
	{
		_buketsNum = num;
	}
private:
	int _buketsNum;
};

template <typename K, typename V, typename HASH=Hash<K>>
class HashMap
{
public:
	HashMap(int initBucketsNum=200)
	:_hashmap(initBucketsNum)
	,_total(initBucketsNum)
	,_size(0)
	{
		//_map.reserve(initBucketsNum);//预留空间
		_phash = new HASH(initBucketsNum);
	}
	~HashMap()
	{
		delete _phash;
	}
	void put(K key, V value)
	{
		int index = _phash->hash(key);
		cout<<"key:"<<key<<" index:"<<index<<endl;	
		list<Node> &lis = _hashmap[index];
		list<Node>::iterator it = find(lis.begin(), lis.end(), key);
		if(it == lis.end())
		{
			if (lis.empty())
			{
				_size++;//使用了的桶数
			}
			
			lis.push_front(Node(key, value));
			//加载因子 当_size/_total > 0.75(查找速率下降) 桶要增长
			double loadFactor = _size/(double)_total;
			cout<<"loadFactor:"<<loadFactor<<endl;
			if (loadFactor > 0.75)
			{
				resize();//调不起来，要不size不正确
			}
		}
		else
		{
			it->_value = value;
		}
	}
	void remove(K key)
	{
		int index = _phash->hash(key);
		list<Node> &lis = _hashmap[index];

		list<Node>::iterator it = find(lis.begin(), lis.end(), key);
		if (it != lis.end())
		{
			lis.erase(it);
		}
	}
	V& operator[](K key)
	{	
		int index = _phash->hash(key);
		list<Node> &lis = _hashmap[index];

		list<Node>::iterator it = find(lis.begin(), lis.end(), key);
		if (it == lis.end())
		{
			put(key, V());
			return get(key);
		}
		return it->_value;
	}
	V& get(K key)
	{
		int index = _phash->hash(key);
		list<Node> &lis = _hashmap[index];
		list<Node>::iterator it = find(lis.begin(), lis.end(), key);
		if (it != lis.end())
		{
			return it->_value;
		}
		return V();//默认构造
	}
	void resize()
	{
		cout<<"resize()"<<endl;
		
		vector<list<Node>> tmp(_total*2);
		_phash->setBucketNum(_total*2);
		//HashMap(_total*2);

		vector<list<Node>>::iterator start = _hashmap.begin();
		for (; start != _hashmap.end(); ++start)
		{
			list<Node>::iterator first = start->begin();
			for(; first != start->end(); ++first)
			{			
				int index = _phash->hash(first->_key);
				list<Node> &lis = tmp[index];
				lis.push_front(Node(first->_key,first->_value));
/*				list<Node>::iterator it = find(lis.begin(), lis.end(), first->_key);
				if(it == lis.end())
				{
					if (lis.empty())
					{
						_size++;//使用了的桶数
					}

					lis.push_front(Node(first->_key,first->_value));
				}*/
			}
		}
		_hashmap = tmp;
	}
	int getsize()
	{
		return _size;
	}
private:
	struct Node
	{
		Node (K key=K(), V value=V()):_key(key),_value(value),_pnext(NULL){}
		bool operator==(const Node &src)
		{
			return _key == src._key;
		}

		K _key;
		V _value;
		Node *_pnext;
	};
	vector<list<Node>> _hashmap;
	HASH *_phash;
	int _size;
	int _total;

	friend ostream& operator<<(ostream &out, HashMap<K,V> &map)
	{
		vector<list<Node>>::iterator it = map._hashmap.begin();
		for (; it != map._hashmap.end(); ++it)
		{
			list<Node>::iterator first = it->begin();
			for (; first != it->end(); ++first)
			{
				out<<first->_key<<" "<<first->_value<<endl;
			}
		}
		return out;
	}
};

int main()
{

	HashMap<int, string> map1;
	/*map1.put(23, "hello");
	map1.put(45, "world");
	map1.put(32, "china");
	map1.put(24, "aaa");
	map1.put(26, "hello");
	map1.put(56, "world");
	map1.put(78, "china");
	map1.put(89, "aaa");
	map1.put(134, "hello");
	map1.put(15, "world");
	map1.put(1, "china");
	map1.put(21, "aaa");*/
	for (int i=0; i<50; ++i)
	{
		map1.put(rand()%(893*5), "hello");
	}
	cout<<map1.getsize()<<endl;
	

	/*map1.remove(32);
	map1[45] = "love";*/

	cout<<map1<<endl;
	return 0;
}