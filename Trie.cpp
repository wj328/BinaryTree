#include<iostream>
using namespace std;

#define LINKSIZE 27 //英文字母的个数+0
#define KEYSIZE 24 //假设的单词的最长长度

typedef enum{ELEM = 0, BRCH = 1}NodeType;
typedef struct KeyType
{
	char ch[KEYSIZE];
	int len;
}KeyType;

typedef struct{}Record;
typedef struct 
{
	KeyType key;
	Record *recptr;
}ElemType;

struct TrieNode;
typedef struct
{
	TrieNode *link[LINKSIZE];
}LinkType;

typedef struct TrieNode
{
	NodeType utype;
	union
	{
		ElemType elem;
		LinkType brch;
	};	
}TrieNode;

class TrieTree
{
private:
	TrieNode *root;
	static TrieNode* BuyNode()
	{
		TrieNode *s = (TrieNode*)malloc(sizeof(TrieNode));
		if(NULL == s) exit(1);
		memset(s, 0, sizeof(TrieNode));
		return s;
	}
	static TrieNode* BuyElem(const ElemType &kx)
	{
		TrieNode *s = BuyNode();
		s->utype = ELEM;
		s->elem = kx;
		return s;
	}
	static TrieNode* BuyBrch(TrieNode* ptr, int pos)
	{
		TrieNode *s = BuyNode();
		s->utype = BRCH;
		int index = FindIndex(ptr->elem.key, pos);
		s->brch.link[index] = ptr;
		return s;
	}
	static void FreeNode(TrieNode *p)
	{
		free(p);
	}
public:
	TrieTree():root(NULL){}
	~TrieTree(){}
	static int FindIndex(const KeyType kx, int pos)
	{
		int index = 0;
		if(pos < kx.len)//当pos==len时返回0下标
		{
			index = kx.ch[pos]-'a'+1;
		}
		return index;
	}
	TrieNode* FindValue(const KeyType kx)
	{
		TrieNode *p = root;
		int pos = 0;
		while(p != NULL && p->utype == BRCH)
		{
			int index = FindIndex(kx, pos++);
			p = p->brch.link[index];
		}
		if( p != NULL)//元素
		{
			if(strcmp(p->elem.key.ch, kx.ch) != 0)//没找到
			{
				p = NULL;
			}
		}
		return p;
	}
	static bool Insert(TrieNode *&ptr, const ElemType kx, int pos)
	{
		bool res = false;
		if(ptr == NULL)
		{
			ptr = BuyElem(kx);
			res = true;
		}else if(ptr->utype == ELEM)
		{
			//买分支,//插入元素,如果已经存在，则返回插入失败
			if(strcmp(ptr->elem.key.ch, kx.key.ch) != 0)
			{
				ptr = BuyBrch(ptr, pos);
				int index = FindIndex(kx.key, pos);
				res = Insert(ptr->brch.link[index], kx, pos+1);
			}
		}else if(ptr->utype == BRCH)
		{
			int index = FindIndex(kx.key, pos);		
			res = Insert(ptr->brch.link[index], kx, pos+1);			
		}
		return res;
	}
	bool Insert(const ElemType &kx)
	{
		return Insert(root, kx, 0);
	}
};

int main()
{
	ElemType ar[] = {"data",4,NULL,"eye",3,NULL,"data",4,NULL, "date",4,NULL,"eye",3,NULL};
	int n = sizeof(ar)/sizeof(ar[0]);

	TrieTree tt;
	for(int i=0; i<n; ++i)
	{
		cout<<tt.Insert(ar[i]);
	}
}