#pragma once

class BloomFilter
{
#define MAXSIZE 16777216 // 2^24
private:
	QBitArray bitArray;
	/* Hash functions */
	//14 Hash functions and 20 times of number of items length for bitArray
	//makes error rate 0.0001
	unsigned int RSHash(char* str, unsigned int len);
	unsigned int JSHash(char* str, unsigned int len);
	unsigned int PJWHash(char* str, unsigned int len);
	unsigned int ELFHash(char* str, unsigned int len);
	unsigned int BKDRHash(char* str, unsigned int len);
	unsigned int SDBMHash(char* str, unsigned int len);
	unsigned int DJBHash(char* str, unsigned int len);
	unsigned int DEKHash(char* str, unsigned int len);
	unsigned int BPHash(char* str, unsigned int len);
	unsigned int FNVHash(char* str, unsigned int len);
	unsigned int APHash(char* str, unsigned int len);
	unsigned int HFLPHash(char* str, unsigned int len);
	unsigned int HFHash(char* str, unsigned int len);
	unsigned int StrHash(char* str, unsigned int len);
	/* End of hash functions */
public:
	BloomFilter();
	bool hasItem(void *key);
	bool addItem(void *key);
};