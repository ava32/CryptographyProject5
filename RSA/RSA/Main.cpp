#include "stdafx.h"
#include <conio.h>
#include <fstream>
#include "RSA.h"
int main()
{
	cout<<"\t\t����������������RSA�ӽ��ܹ��ߡ���������������\n\n";
	RSA rsa;
	char OpNum=0;
	PublicKey PU;
	PrivateKey PR;
	LargeNum p,q,d,n,p_1modq,q_1modp;
	while(1)
	{
		cout<<"����������ţ�1-������Կ 2-���� 3-���� 4-�˳�"<<endl;
		do 
		{
			OpNum=getch();
		} while (OpNum<'1'||OpNum>'4');
		switch(OpNum)
		{
		case '1':
			int pNum,qNum;
			cout<<"������ѡ�������p��q��ʮ����λ����"<<endl;
			while(1)
			{
				cout<<"p��";
				cin>>pNum;
				if (pNum<1||pNum>1024)
				{
					cout<<"�����λ�����������������룡"<<endl;
					continue;
				}
				break;
			}
			while(1)
			{
				cout<<"q��";
				cin>>qNum;
				if (qNum<1||qNum>MAX_LEN/2)
				{
					cout<<"�����λ�����������������룡"<<endl;
					continue;
				}
				break;
			}
			cout<<"��Կ�����У����Ժ򡭡�"<<endl;
			rsa.RandKey(pNum,qNum,PU,PR);
			cout<<"ѡ��Ĵ�����p��qΪ��p="<<PR.GetP()<<",q="<<PR.GetQ()<<endl;
			cout<<"���ɵ���Կ���£�"<<endl;
			cout<<"��Կ��PU={e,n}={"<<PU.GetE()<<','<<PU.GetN()<<'}'<<endl;
			cout<<"˽Կ��PR={d,n}={"<<PR.GetD()<<','<<PR.GetN()<<'}'<<endl;
			cout<<"�Ƿ񵼳���Կ��(Y/N)"<<endl;
			do 
			{
				OpNum=getch();
			} while (OpNum!='Y'&&OpNum!='y'&&OpNum!='N'&&OpNum!='n');
			if ('Y'==OpNum||'y'==OpNum)
			{
				ofstream fout("Key.txt");
				fout<<"��Կ��PU={e,n}={"<<PU.GetE()<<','<<PU.GetN()<<'}'<<endl;
				fout<<"˽Կ��PR={d,n}={"<<PR.GetD()<<','<<PR.GetN()<<'}'<<endl;
				fout.close();
				cout<<"��Կ�ѵ���\"Key.txt\"�ļ���"<<endl;
			}
			break;
		case '2':
			FILE *fin,*fout;
			char FileName[255];
			while (1)
			{
				cout<<"������ѡ��Ĵ�����p��q��"<<endl;
				cout<<"p=";
				cin>>p;
				cout<<"q=";
				cin>>q;
				if (!RSA::IsPrime(p)||!RSA::IsPrime(q))
				{
					cout<<"p��q��������������������"<<endl;
				}
				else if (p.GetLenth()<4*BLOCK_SIZE||q.GetLenth()<4*BLOCK_SIZE)
				{
					cout<<"p��qλ��̫�٣�����������"<<endl;
					continue;
				}
				break;
			}
			cout<<"����������Կ�����Ժ򡭡�"<<endl;
			rsa.CreateKey(p,q);
			cout<<"���ɵĹ�ԿPU={e,n}={"<<rsa.PU.GetE()<<","<<rsa.PU.GetN()<<'}'<<endl;
			cout<<"���ɵ�˽ԿPR={d,n}={"<<rsa.PR.GetD()<<","<<rsa.PR.GetN()<<'}'<<endl;
			cout<<"������Ҫ���ܵ��ļ�����"<<endl;
			cin>>FileName;
			while(NULL==(fin=fopen(FileName,"r")))
			{
				cout<<"���ļ�ʧ�ܣ������ԣ�"<<endl;	
				cout<<"������Ҫ���ܵ��ļ�����"<<endl;
				cin>>FileName;
			}
			fout=fopen("ciphertext.txt","w");
			cout<<"���ڼ��ܣ����Ժ򡭡�"<<endl;
			rsa.EncryptFile(fin,fout);
			fclose(fin);
			fclose(fout);
			cout<<"���ܺ�Ľ���������\"ciphertext.txt\"�ļ���"<<endl;
			break;
		case '3':
			cout<<"������˽ԿPR={d,n}��"<<endl;
			cout<<"d=";
			cin>>d;
			cout<<"n=";
			cin>>n;
			while (1)
			{
				cout<<"������ѡ��Ĵ�����p��q��"<<endl;
				cout<<"p=";
				cin>>p;
				cout<<"q=";
				cin>>q;
				if (!RSA::IsPrime(p)||!RSA::IsPrime(q))
				{
					cout<<"p��q��������������������"<<endl;
				}
				else if (p.GetLenth()<4*BLOCK_SIZE||q.GetLenth()<4*BLOCK_SIZE)
				{
					cout<<"p��qλ��̫�٣�����������"<<endl;
					continue;
				}
				break;
			}
			RSA::Extend_Euclid(p,q,p_1modq,q_1modp);
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
			PR.SetD(d);
			PR.SetN(n);
			PR.SetP(p);
			PR.SetQ(q);
			PR.Setdmodp_1(d%(p-1));
			PR.Setdmodq_1(d%(q-1));
			PR.SetXp(q*q_1modp);
			PR.SetXq(p*p_1modq);
			rsa.SetPR(PR);
			cout<<"������Ҫ���ܵ��ļ�����"<<endl;
			cin>>FileName;
			while(NULL==(fin=fopen(FileName,"r")))
			{
				cout<<"���ļ�ʧ�ܣ������ԣ�"<<endl;	
				cout<<"������Ҫ���ܵ��ļ�����"<<endl;
				cin>>FileName;
			}
			fout=fopen("plaintext.txt","w");
			cout<<"���ڽ��ܣ����Ժ򡭡�"<<endl;
			rsa.DecryptFile(fin,fout);
			fclose(fin);
			fclose(fout);
			cout<<"���ܺ�Ľ���������\"plaintext.txt\"�ļ���"<<endl;
			break;
		case '4':
			exit(0);
			break;
		default:
			break;
		}
	}
	return 0;
}