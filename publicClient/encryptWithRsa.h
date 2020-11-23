#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"power.h"

int encrypt(int plaintext, int e, int n)
{
	int ciphertext;

	ciphertext = power(plaintext, e, n);

	return ciphertext;
}

