#pragma once
#include "global.h"

typedef struct {
	UINT4 state[4];                                   /* state (ABCD) */
	UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64];                         /* input buffer */
} MD4_CTX;

void MD4Init(MD4_CTX *);
void MD4Update(MD4_CTX *, unsigned char *, unsigned int);
void MD4Final(unsigned char[16], MD4_CTX *);