#ifndef RSA_H
#define RSA_H
#include "LargeNum.h"
#include <time.h>
#include <stdlib.h>
#define TEST_TIME	10	/*素性测试时测试次数*/
#define BLOCK_SIZE	4	/*分组长度*/
class PublicKey
{
private:
	LargeNum e,n;
public:
	PublicKey(){};
	PublicKey(LargeNum E,LargeNum N):e(E),n(N){};
	void SetE(const LargeNum &inputE){e=inputE;};
	void SetN(const LargeNum &inputN){n=inputN;};
	LargeNum GetE()const{return e;};
	LargeNum GetN()const{return n;};
};
class PrivateKey
{
private:
	LargeNum d,n,p,q;
	LargeNum dmodp_1,dmodq_1;
	LargeNum Xp,Xq;
public:
	PrivateKey(){};
	PrivateKey(LargeNum D,LargeNum N):d(D),n(N){};
	void SetD(const LargeNum &inputD){d=inputD;};
	void SetN(const LargeNum &inputN){n=inputN;};
	void SetP(const LargeNum &inputP){p=inputP;};
	void SetQ(const LargeNum &inputQ){q=inputQ;};
	void Setdmodp_1(const LargeNum &inputDmodP_1){dmodp_1=inputDmodP_1;};
	void Setdmodq_1(const LargeNum &inputDmodQ_1){dmodq_1=inputDmodQ_1;};
	void SetXp(const LargeNum &inputXp){Xp=inputXp;};
	void SetXq(const LargeNum &inputXq){Xq=inputXq;};
	LargeNum GetD()const{return d;};
	LargeNum GetN()const{return n;};
	LargeNum GetP()const{return p;};
	LargeNum GetQ()const{return q;};
	LargeNum Getdmodp_1()const{return dmodp_1;};
	LargeNum Getdmodq_1()const{return dmodq_1;};
	LargeNum GetXp()const{return Xp;};
	LargeNum GetXq()const{return Xq;};
};
class RSA
{
public:
	PublicKey PU;
	PrivateKey PR;
	RSA(){};
	RSA(PublicKey pu,PrivateKey pr):PU(pu),PR(pr){};
	~RSA(){};
	static LargeNum ExpMod(const LargeNum &a,const LargeNum &p,const LargeNum &n);	/*计算a^p mod n*/
	static LargeNum Rand(const LargeNum &N);	/*生成[0,N)的随机数*/
	static bool IsPrime(const LargeNum &N);	/*素性测试，采用Miller-Rabin算法*/
	static LargeNum RandPrime(const int &BitNum);	/*生成BitNum位的随机素数*/
	static LargeNum Extend_Euclid(LargeNum a,LargeNum b,LargeNum &x,LargeNum &y);/*扩展欧几里德算法*/
	LargeNum ByteToLargeNum(const char Data[BLOCK_SIZE]);		/*把字节数据转化成大数*/
	void SetPU(PublicKey &pu){PU=pu;};
	void SetPR(PrivateKey &pr){PR=pr;};
	void CreateKey(LargeNum &p,LargeNum &q);
	void RandKey(int pNum,int qNum,PublicKey &PU,PrivateKey &PR);
	void EncryptBlock(const char PlainText[BLOCK_SIZE],LargeNum &CipherText);
	void DecrpytBlock(const LargeNum &CipherText,char PlainText[BLOCK_SIZE]);
	void EncryptFile(FILE *PlainFile,FILE *CipherFile);
	void DecryptFile(FILE *CipherFile,FILE *PlainFile);
};
#endif