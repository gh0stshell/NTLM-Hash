// MD4test.cpp : 定义控制台应用程序的入口点。

#include "stdafx.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "md4.h"
#include "global.h"

#define TEST_BLOCK_LEN 1000
#define TEST_BLOCK_COUNT 1000

static void MDString(char *, unsigned int);
static void MDTimeTrial(void);
static void MDTestSuite(void);
//static void MDFile(char *);
static void MDFilter(void);
static void MDPrint(unsigned char[16]);

#define MD_CTX MD4_CTX
#define MDInit MD4Init
#define MDUpdate MD4Update
#define MDFinal MD4Final

/* Main driver.
Arguments (may be any combination):
-sstring - digests string
-t       - runs time trial
-x       - runs test script
filename - digests file
(none)   - digests standard input
*/

// code from https://tools.ietf.org/rfc/rfc1320.txt

int main(int argc, char* argv[])
{
	char *password = "waza1234/";
	printf("NTLM(\"%s\") = ", password);

	// "waza1234/" => {'w','\0','a','\0','z','\0','a','\0','1','\0','2','\0','3','\0','4','\0','\/','\0' }
	int pw_len = strlen(password);
	char *input = new char[pw_len * 2];
	for (int i=0; i < pw_len; i++)
	{
		input[i * 2] = password[i];
		input[i * 2 + 1] = '\0';
	}
	
	// MD4
	MDString(input, pw_len*2);
//	MDString(password, pw_len);

	delete[]input;

/*
	int i;
	if (argc > 1)
		for (i = 1; i < argc; i++)
			if (argv[i][0] == '-' && argv[i][1] == 's')
				MDString(argv[i] + 2);
			else if (strcmp(argv[i], "-t") == 0)
				MDTimeTrial();
			else if (strcmp(argv[i], "-x") == 0)
				MDTestSuite();
			else
				MDFilter();
*/
	char c = getchar();
	return 0;
}

/* Digests a string and prints the result.
*/
static void MDString(char *string, unsigned int len)
{
	MD_CTX context;
	unsigned char digest[16];
//	unsigned int len = strlen(string);

	MDInit(&context);
	MDUpdate(&context, (POINTER)string, len);
	MDFinal(digest, &context);

	MDPrint(digest);
	printf("\n");
}

/* Measures the time to digest TEST_BLOCK_COUNT TEST_BLOCK_LEN-byte
blocks.
*/
static void MDTimeTrial()
{
	MD_CTX context;
	time_t endTime, startTime;
	unsigned char block[TEST_BLOCK_LEN], digest[16];
	unsigned int i;

	/* Initialize block */
	for (i = 0; i < TEST_BLOCK_LEN; i++)
		block[i] = (unsigned char)(i & 0xff);

	/* Start timer */
	time(&startTime);

	/* Digest blocks */
	MDInit(&context);
	for (i = 0; i < TEST_BLOCK_COUNT; i++)
		MDUpdate(&context, block, TEST_BLOCK_LEN);
	MDFinal(digest, &context);

	/* Stop timer */
	time(&endTime);

	printf(" done\n");
	printf("Digest = ");
	MDPrint(digest);

	printf("\nTime = %ld seconds\n", (long)(endTime - startTime));
	printf
	("Speed = %ld bytes/second\n",
		(long)TEST_BLOCK_LEN * (long)TEST_BLOCK_COUNT / (endTime - startTime));
}

/* Digests a reference suite of strings and prints the results.
*/
static void MDTestSuite()
{
	/*
	MDString("");
	MDString("a");
	MDString("abc");
	MDString("message digest");
	MDString("abcdefghijklmnopqrstuvwxyz");
	MDString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	MDString("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
	*/
}

/* Digests a file and prints the result.
*/
//static void MDFile(char *filename)
//{
//	FILE *file;
//	MD_CTX context;
//	int len;
//	unsigned char buffer[1024], digest[16];
//
//	if ((file = fopen(filename, "rb")) == NULL)
//		printf("%s can't be opened\n", filename);
//
//	else {
//		MDInit(&context);
//		while (len = fread(buffer, 1, 1024, file))
//			MDUpdate(&context, buffer, len);
//		MDFinal(digest, &context);
//
//		fclose(file);
//
//		printf("MD%d (%s) = ", MD, filename);
//		MDPrint(digest);
//
//
//		printf("\n");
//	}
//}

/* Digests the standard input and prints the result.
*/
static void MDFilter()
{
	MD_CTX context;
	int len;
	unsigned char buffer[16], digest[16];

	MDInit(&context);
	while (len = fread(buffer, 1, 16, stdin))
		MDUpdate(&context, buffer, len);
	MDFinal(digest, &context);

	MDPrint(digest);
	printf("\n");
}

/* Prints a message digest in hexadecimal.
*/
static void MDPrint(unsigned char digest[16])
{
	unsigned int i;

	for (i = 0; i < 16; i++)
		printf("%02x", digest[i]);
}
