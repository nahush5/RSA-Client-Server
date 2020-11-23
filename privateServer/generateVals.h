#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include"gcd_extended.h"
#include"millerRabin.h"
#include"file.h"

#define MAX 100

int n, e; // public key

void generateVals()
{
	int p, q, phi;
	/* step 1 choose two large random num and check whether they are prime, then get n = pq*/

	srand(time(NULL));

	p =  ( MAX + rand() % (MAX + 1)) ;

	while( !isPrime(p) )
	{
		p = (MAX + rand() % (MAX + 1)) ;
	}

	q = (rand() % (MAX + 1)) ;

	while( !isPrime(q) )
	{
		q = (rand() % (MAX + 1)) ;
	}

	n = p * q;

	/*step 2: get phi(n) */

	phi = (p - 1) * (q - 1);

	/*step 3: get public key exponent*/

	l:
	
	e = rand() % phi;

	while( !isPrime(e) )
	{
		e = ( rand() % phi ) ;
	}

	if( phi % e == 0 ) goto l;

	/*step 4: get private key */

	int d = 0, y = 0, gcd;
	
	gcd = gcd_extended(e, phi, &d, &y);

	if( d < 0 )
	{
		int v1 = -1 * ( (-1 * d) % phi );

		d = (phi + v1) % phi;
	}

	FILE* f;

	f = fopen(PRIVATE, "w"); // writing d and phi to the private file

// we assume the sender does not know the location of this file or anybody ensuring privacy of private key d.

	char str[30];

	sprintf(str, "%d %d (d and phi respectively)", d, phi);

	for (int i = 0; str[i] != '\0'; i++)
	{
        /* write to file using fputc() function */
        fputc(str[i], f);
    }
        
    fclose(f);

    f = fopen(PUBLIC, "w");

	sprintf(str, "%d %d (n and e respectively)", n, e);

	for (int i = 0; str[i] != '\0'; i++)
	{
        /* write to file using fputc() function */
        fputc(str[i], f);
    }
        
    fclose(f);


/*
	printf("p: %d \n", p);
	printf("q: %d \n", q);
    	printf("n: %d \n", n);
    	printf("phi: %d \n", phi);
    	printf("e: %d \n", e);
    	printf("gcd : %d\n", gcd);
	printf("d: %d, y: %d \n\n", d, y);f
*/
	/* now ready to encrypt */

	return;
}

