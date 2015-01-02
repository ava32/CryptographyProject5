#include "stdafx.h"
#include <conio.h>
#include <fstream>
#include "RSA.h"
int main()
{
	cout<<"\t\t————————RSA加解密工具————————\n\n";
	RSA rsa;
	char OpNum=0;
	PublicKey PU;
	PrivateKey PR;
	LargeNum p,q,d,n,p_1modq,q_1modp;
	while(1)
	{
		cout<<"请输入操作号：1-生成密钥 2-加密 3-解密 4-退出"<<endl;
		do 
		{
			OpNum=getch();
		} while (OpNum<'1'||OpNum>'4');
		switch(OpNum)
		{
		case '1':
			int pNum,qNum;
			cout<<"请输入选择的素数p、q的十进制位数："<<endl;
			while(1)
			{
				cout<<"p：";
				cin>>pNum;
				if (pNum<1||pNum>1024)
				{
					cout<<"输入的位数不合理，请重新输入！"<<endl;
					continue;
				}
				break;
			}
			while(1)
			{
				cout<<"q：";
				cin>>qNum;
				if (qNum<1||qNum>MAX_LEN/2)
				{
					cout<<"输入的位数不合理，请重新输入！"<<endl;
					continue;
				}
				break;
			}
			cout<<"密钥生成中，请稍候……"<<endl;
			rsa.RandKey(pNum,qNum,PU,PR);
			cout<<"选择的大素数p、q为：p="<<PR.GetP()<<",q="<<PR.GetQ()<<endl;
			cout<<"生成的密钥如下："<<endl;
			cout<<"公钥：PU={e,n}={"<<PU.GetE()<<','<<PU.GetN()<<'}'<<endl;
			cout<<"私钥：PR={d,n}={"<<PR.GetD()<<','<<PR.GetN()<<'}'<<endl;
			cout<<"是否导出密钥？(Y/N)"<<endl;
			do 
			{
				OpNum=getch();
			} while (OpNum!='Y'&&OpNum!='y'&&OpNum!='N'&&OpNum!='n');
			if ('Y'==OpNum||'y'==OpNum)
			{
				ofstream fout("Key.txt");
				fout<<"公钥：PU={e,n}={"<<PU.GetE()<<','<<PU.GetN()<<'}'<<endl;
				fout<<"私钥：PR={d,n}={"<<PR.GetD()<<','<<PR.GetN()<<'}'<<endl;
				fout.close();
				cout<<"密钥已导入\"Key.txt\"文件中"<<endl;
			}
			break;
		case '2':
			FILE *fin,*fout;
			char FileName[255];
			while (1)
			{
				cout<<"请输入选择的大素数p、q："<<endl;
				cout<<"p=";
				cin>>p;
				cout<<"q=";
				cin>>q;
				if (!RSA::IsPrime(p)||!RSA::IsPrime(q))
				{
					cout<<"p或q不是素数，请重新输入"<<endl;
				}
				else if (p.GetLenth()<4*BLOCK_SIZE||q.GetLenth()<4*BLOCK_SIZE)
				{
					cout<<"p或q位数太少，请重新输入"<<endl;
					continue;
				}
				break;
			}
			cout<<"正在生成密钥，请稍候……"<<endl;
			rsa.CreateKey(p,q);
			cout<<"生成的公钥PU={e,n}={"<<rsa.PU.GetE()<<","<<rsa.PU.GetN()<<'}'<<endl;
			cout<<"生成的私钥PR={d,n}={"<<rsa.PR.GetD()<<","<<rsa.PR.GetN()<<'}'<<endl;
			cout<<"请输入要加密的文件名："<<endl;
			cin>>FileName;
			while(NULL==(fin=fopen(FileName,"r")))
			{
				cout<<"打开文件失败，请重试！"<<endl;	
				cout<<"请输入要加密的文件名："<<endl;
				cin>>FileName;
			}
			fout=fopen("ciphertext.txt","w");
			cout<<"正在加密，请稍候……"<<endl;
			rsa.EncryptFile(fin,fout);
			fclose(fin);
			fclose(fout);
			cout<<"加密后的结果已输出至\"ciphertext.txt\"文件中"<<endl;
			break;
		case '3':
			cout<<"请输入私钥PR={d,n}："<<endl;
			cout<<"d=";
			cin>>d;
			cout<<"n=";
			cin>>n;
			while (1)
			{
				cout<<"请输入选择的大素数p、q："<<endl;
				cout<<"p=";
				cin>>p;
				cout<<"q=";
				cin>>q;
				if (!RSA::IsPrime(p)||!RSA::IsPrime(q))
				{
					cout<<"p或q不是素数，请重新输入"<<endl;
				}
				else if (p.GetLenth()<4*BLOCK_SIZE||q.GetLenth()<4*BLOCK_SIZE)
				{
					cout<<"p或q位数太少，请重新输入"<<endl;
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
			cout<<"请输入要解密的文件名："<<endl;
			cin>>FileName;
			while(NULL==(fin=fopen(FileName,"r")))
			{
				cout<<"打开文件失败，请重试！"<<endl;	
				cout<<"请输入要加密的文件名："<<endl;
				cin>>FileName;
			}
			fout=fopen("plaintext.txt","w");
			cout<<"正在解密，请稍候……"<<endl;
			rsa.DecryptFile(fin,fout);
			fclose(fin);
			fclose(fout);
			cout<<"解密后的结果已输出至\"plaintext.txt\"文件中"<<endl;
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