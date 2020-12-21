#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <gmp.h>
#include <string.h>

#define MEMSIZE 209715200
#define TESTLENGTH 10000000


// BANNER ////////////////////////////////////////////////////////////////////////////

void banner()
{
	printf("\n      ::::::::   ::::::::  :::::::::  ::::::::::\n");
	printf("    :+:    :+: :+:    :+: :+:    :+: :+:\n");
	printf("   +:+        +:+    +:+ +:+    +:+ +:+\n");
	printf("  +#+        +#+    +:+ +#++:++#:  +#++:++#\n");
	printf(" +#+        +#+    +#+ +#+    +#+ +#+\n");
	printf("#+#    #+# #+#    #+# #+#    #+# #+#\n");
	printf("########   ########  ###    ### ##########\n");
	printf("      :::::::::  :::::::::: ::::    :::  ::::::::  :::    :::\n");
	printf("     :+:    :+: :+:        :+:+:   :+: :+:    :+: :+:    :+:\n");
	printf("    +:+    +:+ +:+        :+:+:+  +:+ +:+        +:+    +:+\n");
	printf("   +#++:++#+  +#++:++#   +#+ +:+ +#+ +#+        +#++:++#++\n");
	printf("  +#+    +#+ +#+        +#+  +#+#+# +#+        +#+    +#+\n");
	printf(" #+#    #+# #+#        #+#   #+#+# #+#    #+# #+#    #+#\n");
	printf("#########  ########## ###    ####  ########  ###    ###\n\n\n");
}

// PRIMETEST ////////////////////////////////////////////////////////////////////////////

unsigned long int primetest()
{
	unsigned long int fact,j,n=0, numberToTest=0, lastPrimeFound;  // Calculation vars.

	printf("** Prime bench **\n");
	usleep(1000000);
	printf("Finding prime numbers...\n");

	clock_t start = clock(), diff;  // Start timer.
	while(clock()-start < TESTLENGTH){  // Loop char print while no key is pressed.

		//calculation
		numberToTest++;
		fact=0;
		for(j=1; j<=numberToTest; j++){
			if(numberToTest%j==0)
				fact++;
		}
		if(fact==2){
			//printf("%lu\r",numberToTest);
			lastPrimeFound = numberToTest;
			continue;
		}
	}
	diff = clock() - start;  // Stop timer.
	usleep(100000);  // Pause to avoid buggy terminal stats output.

	// *** Calculate ***
	unsigned long int msec = diff * 1000 / CLOCKS_PER_SEC;  // Calculate elapsed time.
	unsigned long int primescore = numberToTest / 20; // Calculate prime score.

	printf("Highest prime number found: %lu\n",numberToTest);
	printf("Elapsed time: %lu seconds %lu milliseconds\n", msec/1000, msec%1000);
	printf("Bench score: %lu\n",primescore);
	printf("************************\n");

	return primescore;

}


// FLOATTEST ////////////////////////////////////////////////////////////////////////////

unsigned long int floattest()
{
	unsigned long int calculationsMade = 0;
	float addresult, multresult, divresult, x=0.0001, y=0.001;  // Calculation vars.

	printf("\n** Float bench **\n");
	usleep(1000000);
	printf("Doing float calculations...\n");

	clock_t start = clock(), diff;  // Start timer.
	while(clock()-start < TESTLENGTH){  // Loop char print while no key is pressed.

		//calculation
		addresult = x + y;
		multresult = x * y;
		divresult = x / y;
		x += 0.0001;
		y += 0.001;
		++calculationsMade;
		//printf("add:%f  mult:%f  div:%f  calculations:%lu\r",addresult,multresult,divresult,calculationsMade);
	}
	diff = clock() - start;  // Stop timer.
	usleep(100000);  // Pause to avoid buggy terminal stats output.

	// *** Calculate ***
	unsigned long int msec = diff * 1000 / CLOCKS_PER_SEC;  // Calculate elapsed time.
	unsigned long int floatscore = calculationsMade / 50000; // Calculate float score.

	printf("Float combinations calculated: %lu\n",calculationsMade);
	printf("Elapsed time: %lu seconds %lu milliseconds\n", msec/1000, msec%1000);
	printf("Bench score: %lu\n",floatscore);
	printf("************************\n");

	return floatscore;

}


// PITEST /////////////////////////////////////////////////////////////////////////////

mpz_t tmp1, tmp2, t5, t239, pows;
unsigned long int noOfDecimals = 0;

void actan(mpz_t res, unsigned long base, mpz_t pows, clock_t *piclock)
{
	int i, neg = 1;
	mpz_tdiv_q_ui(res, pows, base);
	mpz_set(tmp1, res);
	for (i = 3; ; i += 2) {

		// break if timer reaches 10 secs
		if(clock() - *piclock >= TESTLENGTH)
			break;

		mpz_tdiv_q_ui(tmp1, tmp1, base * base);
		mpz_tdiv_q_ui(tmp2, tmp1, i);
		if (mpz_cmp_ui(tmp2, 0) == 0)
			break;
		if (neg)
			mpz_sub(res, res, tmp2);
		else
			mpz_add(res, res, tmp2);
		neg = !neg;

		// decimal counter
		noOfDecimals++;
	}
}

char *get_digits(int n, size_t* len, clock_t *piclock)
{
	mpz_ui_pow_ui(pows, 10, n + 20);

	actan(t5, 5, pows, piclock);
	mpz_mul_ui(t5, t5, 16);

	actan(t239, 239, pows, piclock);
	mpz_mul_ui(t239, t239, 4);

	mpz_sub(t5, t5, t239);
	mpz_ui_pow_ui(pows, 10, 20);
	mpz_tdiv_q(t5, t5, pows);

	*len = mpz_sizeinbase(t5, 10);
	return mpz_get_str(0, 0, t5);
}

unsigned long int pitest()
{

	unsigned long accu = 16384, done = 0;
	size_t got;
	char *s;

	mpz_init(tmp1);
	mpz_init(tmp2);
	mpz_init(t5);
	mpz_init(t239);
	mpz_init(pows);

	printf("\n** PI bench **\n");
 	usleep(1000000);
 	printf("Calculating PI decimals...\n");

 	clock_t piclock = clock(), diff;  // Start timer.

	while(clock()-piclock < TESTLENGTH){
		s = get_digits(accu, &got, &piclock);

		// write out digits up to the last one not preceding a 0 or 9
		got -= 2;  // -2: length estimate may be longer than actual
		while (s[got] == '0' || s[got] == '9')
			got--;

		//printf("%.*s", (int)(got - done), s + done);
		free(s);

		done = got;

		// double the desired digits; slows down at least cubically
		accu *= 2;
	}

	diff = clock() - piclock;  // Stop timer.

	// *** Calculate ***
	unsigned long int msec = diff * 1000 / CLOCKS_PER_SEC;  // Calculate elapsed time.
	unsigned long int piscore = noOfDecimals / 100; // Calculate pi score.

	printf("No of pi decimals found: %lu\n", noOfDecimals);
	printf("Elapsed time: %lu seconds %lu milliseconds\n", msec/1000, msec%1000);
	printf("Bench score: %lu\n", piscore);
	printf("************************\n");

	return piscore;
}


int main()
{
	banner();
	usleep(2000000);
	unsigned long int primescore = primetest(); // Run prime test.
	usleep(2000000);
	unsigned long int floatscore = floattest(); // Run mem test.
	usleep(2000000);
	unsigned long int piscore = pitest(); // Run pi test.
	usleep(2000000);
	unsigned long int totalscore = primescore + floatscore + piscore;
	
	printf("\nPrime score:           %lu\n", primescore);
	printf("Float score:           %lu\n", floatscore);
	printf("PI score:              %lu\n", piscore);
	printf("Corebench total score: %lu\n", totalscore);
	return 0;
}
