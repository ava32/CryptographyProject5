#ifndef LARGENUM_H
#define LARGENUM_H
#define SYSTEM 10		/*为方便输入输出，大数采用了十进制存储方式*/
#define MAX_LEN 1024	/*大数最长位数（十进制下）*/
#include <iostream>
#include <string.h>
#include <string>
#include <vector>
using namespace std;
class LargeNum
{
public:
	LargeNum():m_Sign('+'),m_Lenth(1)
	{
		memset(m_Value,0,sizeof(int)*MAX_LEN);
	};
	LargeNum(string value)
	{
		m_Sign='+';
		if ('-'==value.at(0))
		{
			m_Sign='-';
			value=value.substr(1);
		}
		for (int i=0;i<value.length();i++)
		{
			m_Value[i]=value.at(value.length()-1-i)-'0';
		}
		m_Lenth=value.length();
		memset(m_Value+m_Lenth,0,sizeof(int)*(MAX_LEN-m_Lenth));
	};
	~LargeNum(){};
	void SetSign(char sign){m_Sign=sign;};
	void SetLenth(int lenth){m_Lenth=lenth;};
	void SetValueAt(int index,int value){m_Value[index]=value;};
	char GetSign()const{return m_Sign;};
	int GetLenth()const{return m_Lenth;};
	int GetValueAt(int index)const{return m_Value[index];};
	friend ostream& operator<<(ostream &cout,const LargeNum &N);
	friend istream& operator>>(istream &cin,LargeNum &N);
	static LargeNum ConvertToLargeNum(int N);
	static LargeNum Abs(const LargeNum &N);
	bool operator>(const LargeNum &N)const;
	bool operator<(const LargeNum &N)const;
	bool operator==(const LargeNum &N)const;
	bool operator==(const int &N)const;
	bool operator!=(const LargeNum &N)const;
	bool operator!=(const int &N)const;
	LargeNum operator=(const LargeNum &N);
	LargeNum operator=(const int &N);
	LargeNum operator+(const LargeNum &N)const;
	LargeNum operator+(const int &N)const;
	LargeNum operator-(const LargeNum &N)const;
	LargeNum operator-(const int &N) const;
	LargeNum operator*(const LargeNum &N)const;
	LargeNum operator/(const LargeNum &N)const;
	LargeNum operator/(const int &N)const;
	LargeNum operator%(const LargeNum &N)const;
	LargeNum operator%(const int &N)const;

protected:
private:
	char m_Sign;		/*符号位*/
	int m_Lenth;		/*数的位数*/
	int m_Value[MAX_LEN];		/*数值*/
};
#endif