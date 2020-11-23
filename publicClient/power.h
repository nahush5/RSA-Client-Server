#include<stdio.h>
#include<stdlib.h>

int power(int a, int b,	int n)
{
	if(b == 0) return 1;
	if(a == 1 || b == 1) return a;

	int ans = power(a, b/2, n);

	if(b % 2 == 0)
	{
		return ( ans * ans ) % n;
	}
	else
	{
		return (  ( ( ans * ans) % n )  * a ) % n;
	}
}
