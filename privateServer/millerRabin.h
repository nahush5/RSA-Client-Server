#include<stdio.h>
#include<stdlib.h>
#include"power.h"

int isPrime(int n) // uses miller rabin
{
	int i = 0;

	if( n == 2 || n == 3 ) return 1;
	if( n == 1 || n % 2 == 0 ) return 0;

	// printf("***************\n");

	int a = (rand() % (n));

	while(i < 64)
	{
		int r = n - 1;

		int x = power(a, r, n);

		while( r % 2 == 0 )
		{
			if( x == 1 || x == n - 1 )
			{
				r = r / 2;
				x = power(a, r, n);
			}
			else
			{
				return 0;
			}
		}

		a++;
		i++;
	}

	//printf("***************\n");

	return 1;
}
