// RSA.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include "RSA.h"
LargeNum RSA::ExpMod(const LargeNum &a,const LargeNum &p,const LargeNum &n)
{
	int i,j;
	vector<int> bin;
	LargeNum t=p,f("1");
	for (i=0;t!=0;i++)
	{
		bin.push_back((t%2).GetValueAt(0));		/*ת���ɶ�����*/
		t=t/2;
	}
	for (j=i-1;j>=0;j--)
	{
		f=(f*f)%n;
		if (1==bin.at(j))
		{
			f=(f*a)%n;
		}
	}
	return f;
}

bool RSA::IsPrime(const LargeNum &N)	/*���Բ��ԣ�����Lehmann�㷨*/
{
	LargeNum RandNum,T;
	for (int i=0;i<TEST_TIME;i++)
	{
		RandNum=Rand(N);
		T=ExpMod(RandNum,(N-1)/2,N);
		if (T!=1&&T!=N-1)
		{
			return false;
		}
	}
	return true;
}

LargeNum RSA::Rand(const LargeNum &N)	/*����[0,N)�������*/
{
	LargeNum RandNum;
	int i,j;
	srand((unsigned int)time(NULL));
	RandNum.SetLenth(N.GetLenth());
	for (i=0;0==N.GetValueAt(i)&&i<N.GetLenth();i++)
	{
		RandNum.SetValueAt(i,rand()%SYSTEM);
	}
	if (i<N.GetLenth())
	{
		RandNum.SetValueAt(i,rand()%N.GetValueAt(i));
	}
	for (j=i+1;j<N.GetLenth();j++)
	{
		RandNum.SetValueAt(j,rand()%(N.GetValueAt(j)+1));
	}
	while(0==RandNum.GetValueAt(RandNum.GetLenth()-1)&&RandNum.GetLenth()>1)
	{
		RandNum.SetLenth(RandNum.GetLenth()-1);
	}
	return RandNum;
}

LargeNum RSA::RandPrime(const int &BitNum)	/*����BitNumλ���������*/
{
	LargeNum RandNum;
	RandNum.SetLenth(BitNum);
	srand((unsigned int)time(NULL));
	do
	{
		RandNum.SetValueAt(0,2*(rand()%5)+1);		/*�������һλһ��������*/
		for (int i=1;i<BitNum-1;i++)
		{
			RandNum.SetValueAt(i,rand()%SYSTEM);
		}
		RandNum.SetValueAt(BitNum-1,rand()%(SYSTEM-1)+1);	/*���һλ������0*/
	}while(!IsPrime(RandNum));
	return RandNum;
}

LargeNum RSA::Extend_Euclid(LargeNum a,LargeNum b,LargeNum &x,LargeNum &y)/*��չŷ������㷨*/
{
	if(b==0)
	{
		x=1;
		y=0;
		return a;
	}
	LargeNum r;
	LargeNum x_i_2("1"),y_i_2,x_i_1,y_i_1("1"),x_i,y_i;
	while((r=a%b)!=0)
	{
		x_i=x_i_2-(a/b)*x_i_1;
		y_i=y_i_2-(a/b)*y_i_1;
		x_i_2=x_i_1;
		y_i_2=y_i_1;
		x_i_1=x_i;
		y_i_1=y_i;
		a=b;
		b=r;
	}
	x=x_i;
	y=y_i;
	return b;
}

void RSA::CreateKey(LargeNum &p,LargeNum &q)
{
	LargeNum n=p*q,EurlerN=(p-1)*(q-1);
	LargeNum e,d,t;
  	do 
  	{
  		e=Rand(EurlerN-2)+1;	/*���ѡȡeʹ1<e<��(n)*/
  	} while (Extend_Euclid(EurlerN,e,t,d)!=1);
	if ('-'==d.GetSign())
	{
		d.SetSign('+');
		d=EurlerN-d;
	}
	LargeNum p_1modq,q_1modp;
	Extend_Euclid(p,q,p_1modq,q_1modp);
	if ('-'==p_1modq.GetSign())
	{
		p_1modq.SetSign('+');
		p_1modq=q-p_1modq;
	}
	if ('-'==q_1modp.GetSign())
	{
		q_1modp.SetSign('+');
		q_1modp=p-q_1modp;
	}
	PU.SetE(e);
	PU.SetN(n);
	PR.SetD(d);
	PR.SetN(n);
	PR.SetP(p);
	PR.SetQ(q);
	PR.Setdmodp_1(d%(p-1));
	PR.Setdmodq_1(d%(q-1));
	PR.SetXp(q*q_1modp);
	PR.SetXq(p*p_1modq);
}

void RSA::RandKey(int pNum,int qNum,PublicKey &PU,PrivateKey &PR)
{
	LargeNum p=RandPrime(pNum);		/*����pNumλ�������p*/
	LargeNum q=RandPrime(qNum);		/*����qNumλ�������q*/
	LargeNum n=p*q,EurlerN=(p-1)*(q-1);
	LargeNum e,d,t;
	do 
	{
		e=Rand(EurlerN-2)+1;	/*���ѡȡeʹ1<e<��(n)*/
	} while (Extend_Euclid(EurlerN,e,t,d)!=1);	/*��֤GCD(e,��(n))=1�������e���ڦ�(n)�������d*/
	if ('-'==d.GetSign())		/*�Ը����Ĵ���*/
	{
		d.SetSign('+');
		d=EurlerN-d;
	}
	PU.SetE(e);
	PU.SetN(n);
	PR.SetD(d);
	PR.SetN(n);
	PR.SetP(p);
	PR.SetQ(q);
}

LargeNum RSA::ByteToLargeNum(const char Data[BLOCK_SIZE])		/*��BLOCK_SIZE�ֽ�����ת���ɴ���*/
{
	LargeNum LN;
	for (int i=0;i<BLOCK_SIZE;i++)
	{
		LN.SetValueAt(3*i,Data[i]/100);
		LN.SetValueAt(3*i+1,(Data[i]/10)%10);
		LN.SetValueAt(3*i+2,Data[i]%10);
	}
	LN.SetLenth(BLOCK_SIZE*3);
	return LN;
}

void RSA::EncryptBlock(const char PlainText[BLOCK_SIZE],LargeNum &CipherText)
{
	LargeNum LN;
	LN=ByteToLargeNum(PlainText);
	CipherText=ExpMod(LN,PU.GetE(),PU.GetN());	/*C=M^e mod n*/
}

void RSA::DecrpytBlock(const LargeNum &CipherText,char PlainText[BLOCK_SIZE])
{
	LargeNum Vp=ExpMod(CipherText,PR.Getdmodp_1(),PR.GetP());
	LargeNum Vq=ExpMod(CipherText,PR.Getdmodq_1(),PR.GetQ());
	LargeNum Plain=(Vp*PR.GetXp()+Vq*PR.GetXq())%PR.GetN();
	for(int i=0;i<BLOCK_SIZE;i++)
	{
		PlainText[i]=Plain.GetValueAt(3*i)*100+Plain.GetValueAt(3*i+1)*10+Plain.GetValueAt(3*i+2);
	}
}

void RSA::EncryptFile(FILE *PlainFile,FILE *CipherFile)
{
	int count=0;
	char PlainText[BLOCK_SIZE]={0};
	LargeNum LN;
	while (!feof(PlainFile))
	{
		if(BLOCK_SIZE==(count=fread(PlainText,sizeof(char),BLOCK_SIZE,PlainFile)))
		{
			EncryptBlock(PlainText,LN);
			for (int i=LN.GetLenth()-1;i>=0;i--)
			{
				fprintf(CipherFile,"%d",LN.GetValueAt(i));
			}                     
			fputc('\n',CipherFile);
		}
	}
	if (count)/*˵�����ĳ��Ȳ���BLOCK_SIZE�ı�������Ҫ�����β*/
	{
		memset(PlainText+count,'\0',BLOCK_SIZE-count);
		EncryptBlock(PlainText,LN);
		for (int i=LN.GetLenth()-1;i>=0;i--)
		{
			fprintf(CipherFile,"%d",LN.GetValueAt(i));
		}                     
		fputc('\n',CipherFile);
	}
}

void RSA::DecryptFile(FILE *CipherFile,FILE *PlainFile)
{
	char p[1024];
	while(fgets(p,1024,CipherFile))
	{
		p[strlen(p)-1]=0;
		int i=0;
		char PlainText[BLOCK_SIZE]={0};
		LargeNum LN(p);
		DecrpytBlock(LN,PlainText);
		fwrite(PlainText,sizeof(char),BLOCK_SIZE,PlainFile);
	}
}