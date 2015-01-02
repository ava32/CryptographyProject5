#include "stdafx.h"
#include "LargeNum.h"

LargeNum LargeNum::ConvertToLargeNum(int N)
{
	LargeNum LN("0"); 
	int i=0;
	LN.SetSign(N>=0?'+':'-');
	N=N<0?-N:N;
	do
	{
		LN.SetValueAt(i++,N%SYSTEM);
		N/=SYSTEM;
	}while(N!=0);
	LN.SetLenth(i);
	return LN;
}

LargeNum LargeNum::Abs(const LargeNum &N)
{
	LargeNum X=N;
	X.m_Sign='+';
	return X;
}

istream& operator>>(istream &cin,LargeNum &N)
{
	string num;
	cin>>num;
	
	if ('-'==num.at(0))
	{
		N.m_Sign='-';
		num=num.substr(1);
	}
	for(int i=0;i<num.length();i++)
	{
		N.m_Value[i]=num.at(num.length()-i-1)-'0';
	}
	N.m_Lenth=num.length();
	return cin;
}

ostream& operator<<(ostream &cout,const LargeNum &N)
{
	if ('-'==N.m_Sign)
	{
		cout<<'-';
	}
	for (int i=N.m_Lenth-1;i>=0;i--)
	{
		cout<<N.m_Value[i];
	}
	return cout;
}

bool LargeNum::operator>(const LargeNum &N)const
{
	if ('+'==this->m_Sign)	/*为正数时*/
	{
		if (N.m_Sign=='-')	/*正数必然大于负数*/
			return true;
		if (this->m_Lenth>N.m_Lenth)	/*都为正数时位数多的大*/
			return true;
		if (this->m_Lenth<N.m_Lenth)
			return false;
		for (int i=m_Lenth-1;i>=0;i--)	/*位数相同时从高位开始比较*/
		{
			if (m_Value[i]>N.m_Value[i])
				return true;
			if (m_Value[i]<N.m_Value[i])
				return false;
		}
		return false;
	}
	else	/*为负数时*/
	{
		if ('+'==N.m_Sign)	/*正数必然大于负数*/
			return false;
		if (this->m_Lenth<N.m_Lenth)	/*都为负数时位数少的大*/
			return true;
		if (this->m_Lenth<N.m_Lenth)
			return false;
		for (int i=m_Lenth-1;i>=0;i--)	/*位数相同时从高位开始比较*/
		{
			if (m_Value[i]<N.m_Value[i])
				return true;
			if (m_Value[i]>N.m_Value[i])
				return false;
		}
		return false;
	}
}

bool LargeNum::operator==(const LargeNum &N)const
{
	return m_Sign==N.m_Sign&&m_Lenth==N.m_Lenth&&0==memcmp(m_Value,N.m_Value,sizeof(int)*MAX_LEN);
}

bool LargeNum::operator==(const int &N)const
{
	return *this==ConvertToLargeNum(N);
}

bool LargeNum::operator!=(const LargeNum &N)const
{
	return !(*this==N);
}

bool LargeNum::operator!=(const int &N)const
{
	return !(*this==N);
}

bool LargeNum::operator<(const LargeNum &N)const
{
	return !(*this==N||*this>N);
}

LargeNum LargeNum::operator=(const LargeNum &N)
{
	m_Sign=N.m_Sign;
	m_Lenth=N.m_Lenth;
	memcpy(m_Value,N.m_Value,sizeof(int)*m_Lenth);
	memset(m_Value+m_Lenth,0,sizeof(int)*(MAX_LEN-m_Lenth));
	return *this;
}

LargeNum LargeNum::operator=(const int &N)
{
	*this=ConvertToLargeNum(N);
	return *this;
}

LargeNum LargeNum::operator+(const LargeNum &N)const
{
	LargeNum X;
	if (this->m_Sign==N.m_Sign)			/*符号相同，直接相加*/
	{
		int carry=0;
		X=*this;
		X.m_Lenth=m_Lenth>N.m_Lenth?m_Lenth:N.m_Lenth;
		for (int i=0;i<X.m_Lenth;i++)
		{
			X.m_Value[i]=(m_Value[i]+N.m_Value[i]+carry)%SYSTEM;
			carry=(m_Value[i]+N.m_Value[i]+carry)/SYSTEM;
		}
		if (carry)
		{
			if (X.m_Lenth<MAX_LEN)
			{
				X.m_Value[X.m_Lenth]=carry;
				X.m_Lenth++;
			}
			else
			{
				cout<<"数据溢出！"<<endl;
			}
		}
	} 
	else	/*符号不同，改变符号相减*/
	{
		X=N;
		X.m_Sign='+'==N.m_Sign?'-':'+';
//		X=(*this)-X;
	}
	return X;
}

LargeNum LargeNum::operator+(const int &N)const
{
	return *this+ConvertToLargeNum(N);
}

LargeNum LargeNum::operator-(const LargeNum &N) const
{
	LargeNum X;
	if(*this==N)
	{
		return X;
	}
	if (m_Sign==N.m_Sign)	/*符号相同直接相减*/
	{
		int carry=0;
		const int *Minuend,*Subtrahend;		/*被减数和减数*/
		if ((*this>N&&'+'==m_Sign)||(*this<N&&'-'==m_Sign))
		{
			Minuend=m_Value;
			Subtrahend=N.m_Value;
			X.m_Lenth=m_Lenth;
		}	
		else
		{
			Minuend=N.m_Value;
			Subtrahend=m_Value;
			X.m_Lenth=N.m_Lenth;
		}
		for (int i=0;i<X.m_Lenth;i++)
		{
			if (Minuend[i]-carry>=Subtrahend[i])
			{
				X.m_Value[i]=Minuend[i]-carry-Subtrahend[i];
				carry=0;
			}
			else
			{
				X.m_Value[i]=SYSTEM+Minuend[i]-carry-Subtrahend[i];
				carry=1;
			}
		}
		while(0==X.m_Value[X.m_Lenth-1])	/*去掉高位0*/
			X.m_Lenth--;
		X.m_Sign=*this>N?'+':'-';
	}
	else		/*符号不同，改变符号相加*/
	{
		X=N;
		X.m_Sign='+'==N.m_Sign?'-':'+';
		X=(*this)+X;
	}
	return X;
}

LargeNum LargeNum::operator-(const int &N) const
{
	return *this-ConvertToLargeNum(N);
}

LargeNum LargeNum::operator*(const LargeNum &N)const
{
	LargeNum X,Y;
	int carry=0,product;
	if (N==0)
	{
		return X;
	}
	for(int i=0;i<N.m_Lenth;i++)
	{
		Y.m_Lenth=m_Lenth;
		carry=0;
		for(int j=0;j<m_Lenth;j++)
		{
			product=N.m_Value[i]*m_Value[j]+carry;
			Y.m_Value[j]=product%SYSTEM;
			carry=product/SYSTEM;
		}
		if (carry&&Y.m_Lenth<MAX_LEN)
		{
			Y.m_Lenth++;
			Y.m_Value[Y.m_Lenth-1]=carry;
		}
		if (Y.m_Lenth+i<=MAX_LEN)
		{
			Y.m_Lenth+=i;
			int k;
			for (k=Y.m_Lenth-1;k>=i;k--)
			{
				Y.m_Value[k]=Y.m_Value[k-i];
			}
			for (k=0;k<i;k++)
			{
				Y.m_Value[k]=0;
			}
		}
		X=X+Y;
	}
	X.m_Sign=m_Sign==N.m_Sign?'+':'-';
	return X;
}

LargeNum LargeNum::operator/(const LargeNum &N)const
{
	if (1==N.m_Lenth&&0==N.m_Value[0])
	{
		cout<<"除数不能为0！"<<endl;
		return N;
	}
	LargeNum X,Y=Abs(*this),Z;
	int len;
	while(Y>Abs(N))	/*被除数大于除数绝对值*/
	{
		if (Y.m_Value[Y.m_Lenth-1]>N.m_Value[N.m_Lenth-1])
		{
			len=Y.m_Lenth-N.m_Lenth;
			Z=Y.m_Value[Y.m_Lenth-1]/(N.m_Value[N.m_Lenth-1]+1);
		}
		else if (Y.m_Lenth>N.m_Lenth)
		{
			len=Y.m_Lenth-N.m_Lenth-1;
			Z=(Y.m_Value[Y.m_Lenth-1]*SYSTEM+Y.m_Value[Y.m_Lenth-2])/(N.m_Value[N.m_Lenth-1]+1);
		}
		else
		{
			X=X+1;
			break;
		}
		Z.m_Lenth+=len;
		for (int i=Z.m_Lenth-1;i>=len;i--)
		{
			Z.m_Value[i]=Z.m_Value[i-len];
		}
		for(int i=0;i<len;i++)
		{
			Z.m_Value[i]=0;
		}
		X=X+Z;
		Z=Z*N;
		Z.m_Sign='+';
		Y=Y-Z;
	}
	if (Y==N)
	{
		X=X+1;
	}
	X.m_Sign=m_Sign==N.m_Sign?'+':'-';
	return X;
}

LargeNum LargeNum::operator/(const int &N)const
{
	return (*this)/ConvertToLargeNum(N);
}

LargeNum LargeNum::operator%(const LargeNum &N)const
{
	if (1==N.m_Lenth&&0==N.m_Value[0])
	{
		cout<<"模数不能为0！"<<endl;
		return N;
	}
	LargeNum X=Abs(*this),Y;
	while(X>Abs(N))	/*被除数大于除数绝对值*/
	{
		int len;
		if (X.m_Value[X.m_Lenth-1]>N.m_Value[N.m_Lenth-1])
		{
			len=X.m_Lenth-N.m_Lenth;
			Y=X.m_Value[X.m_Lenth-1]/(N.m_Value[N.m_Lenth-1]+1);
		}
		else if (X.m_Lenth>N.m_Lenth)
		{
			len=X.m_Lenth-N.m_Lenth-1;
			Y=(X.m_Value[X.m_Lenth-1]*SYSTEM+X.m_Value[X.m_Lenth-2])/(N.m_Value[N.m_Lenth-1]+1);
		}
		else
		{
			X=X-N;
			break;
		}
		Y=Y*N;
		Y.m_Sign='+';
		Y.m_Lenth+=len;
		for (int i=Y.m_Lenth-1;i>=len;i--)
		{
			Y.m_Value[i]=Y.m_Value[i-len];
		}
		for(int i=0;i<len;i++)
		{
			Y.m_Value[i]=0;
		}
		X=X-Y;
	}
	if (X==Abs(N))
	{
		X=0;
		return X;
	}
	return X;
}

LargeNum LargeNum::operator%(const int &N)const
{
	return *this%ConvertToLargeNum(N);
}
