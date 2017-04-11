#include <iostream>
#include <iomanip>
#include <bitset>
#include <string>
#include <vector>

using namespace std;

//��Ϊ����洢��ʽ,��1��ʼ left=pa*2 right=pa*2+1 pa=child/2
#define HEAPSIZE 100
template<typename Type>
class HeapMin
{
private:
	Type *data;
	int cursize;
	int maxsize;
	//ɾ������е���//����һ��ֵ����Ϊ��С��
	static void FilterDown(Type *ar, int pos, int n)
	{
		int i = pos;
		int j = pos*2;
		ar[0] = ar[i];

		while(j <= n)
		{
			if(j<n && ar[j]>ar[j+1]) ++j;//�����Сֵ���±�
			if(ar[0] <= ar[j])
				break;
			ar[i] = ar[j];
			i = j;
			j = j*2;
		}
		ar[i] = ar[0];
	}
	//����һ��ֵʱ���ϵ���
	static void FilterUp(Type *ar, int pos, int end)//end=1
	{
		int j = pos;
		int i = j/2;
		ar[0] = ar[j];

		while(i >= end)
		{
			if(ar[0] >= ar[i])
				break;
			ar[j] = ar[i];
			j = i;
			i = i/2;
		}
		ar[j] = ar[0];
	} 
public:
	HeapMin(int sz = HEAPSIZE):cursize(0)
	{
		maxsize = sz>HEAPSIZE?sz:HEAPSIZE;//���ٿ�100
		data = (Type *)malloc(sizeof(Type)*(maxsize+1));//��1λ��ʼ������
		//0λ�ڱ���
	}
	HeapMin(Type *ar, int n)
	{
		maxsize = n > HEAPSIZE?n:HEAPSIZE;
		cursize = n;
		data = (Type*)malloc(sizeof(Type)*(maxsize+1));
		for(int i=0; i<n; i++)
		{
			data[i+1] = ar[i];
		}
		//�γ�С����
		int pos = n/2;//��--pos,FilterDown���������γ�С����
		while(pos > 0)
		{
			FilterDown(data, pos, n);
			--pos;
		}
	}
	bool Remove(Type &x)
	{
		if(cursize <= 0)
			return false;
		x = data[1];
		data[1] = data[cursize--];
		FilterDown(data,1,cursize);
		return true;
	}

	bool Insert(const Type &x)
	{
		if(cursize >= maxsize)
			return false;
		data[++cursize] = x;
		FilterUp(data, cursize, 1);
		return true;
	}
};
/*
int main()
{
	int ar[] = {23,45,67,78,90,80,12,34,78};
	int n = sizeof(ar)/sizeof(ar[0]);

	//HeapMin<int> myh(ar, n);
	HeapMin<int> myh;
	int x;
	while(cin>>x, x!=-1)
	{
		myh.Insert(x);
	}
	while(myh.Remove(x))
	{
		cout<<x<<" ";
	}
	cout<<endl;
	return 0;
}
*/
#define HUFFSIZE 8
#define HUFFTOTAL (HUFFSIZE*2)//Ҷ��+��֧+��1��ʼ
typedef unsigned int WeightType;

struct HuffNode
{
	WeightType weight;
	int parent;
	int left;
	int right;
};
typedef HuffNode HuffTree[HUFFTOTAL];
//��typedef HuffTree����Ϊ����,Ԫ��ΪHuffNode,����ΪHUFFTOATAL
//���Զ�������HuffTree huff,û��typedef��Ϊȫ������

struct Code
{
	char ch;
	char code[HUFFSIZE+1];
};
typedef Code HuffCode[HUFFSIZE+1];

void Init_Huff(HuffTree &ht)
{
	memset(ht, 0, sizeof(HuffTree));
	for(int i=1; i<=HUFFSIZE; ++i)
	{
		cin>>ht[i].weight;
	}
}
void Init_Code(HuffCode &hc)
{
	memset(hc, 0, sizeof(HuffCode));
	for(int i=1; i<=HUFFSIZE; i++)
	{
		cin>>hc[i].ch;
	}
}
void Print(HuffTree &ht)
{
	for(int i=1; i<HUFFTOTAL; ++i)
	{
		cout<<setw(2)<<i;
		cout<<setw(5)<<ht[i].weight;
		cout<<setw(3)<<ht[i].parent;
		cout<<setw(3)<<ht[i].left;
		cout<<setw(3)<<ht[i].right<<endl;
	}
}
void PrintCode(HuffCode &hc)
{
	for(int i=1; i<=HUFFSIZE; i++)
	{
		cout<<hc[i].ch<<":"<<hc[i].code<<endl;
	}
}
//{}����ֵstruct class��ԱȫΪpublic,�������Լ�д�Ĺ���,��������(��һ��)
void Create_Huff(HuffTree &ht)
{
	struct IndexNode
	{
		int index;
		WeightType w;
		operator WeightType(){return w;}//WeightType���������
	};
	HeapMin<IndexNode> hm;
	for(int i=1; i<=HUFFSIZE; i++)
	{
		IndexNode node={i, ht[i].weight};
		hm.Insert(node);
	}
	
	IndexNode left, right;
	int i = HUFFSIZE+1;

	while(i < HUFFTOTAL)//����9�±�
	{
		if(hm.Remove(left) && hm.Remove(right))//���Ҿ�����
		{
			ht[i].weight = ht[left.index].weight + ht[right.index].weight;
			ht[i].left = left.index;
			ht[i].right = right.index;
			ht[left.index].parent = i;
			ht[right.index].parent = i;

			IndexNode node = {i, ht[i].weight};
			hm.Insert(node);
			i++;
		}
	}
}
void Create_Coding(HuffTree &ht, HuffCode &hc)
{
	char code[HUFFSIZE+1];
	int pa;
	for(int i=1; i<=HUFFSIZE; i++)
	{
		int k = HUFFSIZE;
		code[k] = '\0';
		int j = i;
		while((pa = ht[j].parent) != 0)
		{
			code[--k] = j==ht[pa].left?'0':'1';
			j = pa;
		}
		strcpy(hc[i].code, &code[k]);
	}
}

/*
vector<char> vc(32);//��α�Ϊbit�洢��
//bitset<32> bitvec;bitset<32> bitvec(str, 0, 3);//��str[0]��ʼ�����ַ�
//posΪ�±�,nΪ����
void HuffCoding(HuffTree &ht, int pos, int n, char c)
{
	if(pos != 0)
	{
		if (n>0)
			vc[n-1] = c;
		else
			vc[n] = c;
		HuffCoding(ht, ht[pos].left, n+1, '0');//��
		if(1 <= pos && pos <= HUFFSIZE)
		{
			cout<<ht[pos].weight<<" ";
			//bitvec(vc,0,n);
			for(int i=0; i<n; ++i)
			{
				cout<<vc[i];
			}
			cout<<endl;
		}
		HuffCoding(ht, ht[pos].right, n+1, '1');//��
	}
}
*/


void main()
{
	HuffTree ht;
	HuffCode hc;
	Init_Huff(ht);
	Init_Code(hc);
	Create_Huff(ht);
	Create_Coding(ht, hc);
	Print(ht);
	PrintCode(hc);
	//HuffCoding(ht, HUFFTOTAL-1, 0, '0');
}
