#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>


int N = 100;    //The number of individuals
int tolerability = 1;
double b = 3, c = 1, gamma = 0.3, beta = 2;   //delta=0.2    beta=1.621(0.1)      beta=2.387(0.2)     beta=2.9448(0.3)
double s = 0.2, mu = 0.02;       //s: selection intensity,  mu: mutation rate
double aveP = 0, aveC = 0, aveD = 0;   //the frequency of the three type of players
int number[3];   //record the number of individuals (P, C, D) in each generation

double fitness[5000];    //record the fitness of each individual
int strategy[5000];   //record the strategy of each individual (0 represent P, 1 represent C, 2 represent D)


void evolution()
{
	long long times;
	int diePlayer, birthPlayer;
	double imitate_probability;
	for (times = 1; times <= 10000000; times++)      //generations of iterations
	{
		diePlayer = rand() % N;    
		birthPlayer = rand() % N;   

		while (birthPlayer == diePlayer)
		{
			birthPlayer = rand() % N;
		}


		double temprand;
		temprand = rand() * 1.0 / RAND_MAX;

		if ((temprand > mu) && strategy[birthPlayer] == strategy[diePlayer])
		{
			// if no mutation occurs and the two player selected have the same strategy, the strategy composition will not change
			aveP = aveP * (times - 1) * 1.0 / times + number[0] * 1.0 / (times);
			aveC = aveC * (times - 1) * 1.0 / times + number[1] * 1.0 / (times);
			aveD = aveD * (times - 1) * 1.0 / times + number[2] * 1.0 / (times);
			continue;
		}
		

		aveP = aveP * (times - 1) * 1.0 / times + number[0] * 1.0 / (times);
		aveC = aveC * (times - 1) * 1.0 / times + number[1] * 1.0 / (times);
		aveD = aveD * (times - 1) * 1.0 / times + number[2] * 1.0 / (times);

		//fitness of birthplayer
		if (strategy[birthPlayer] == 0)
		{
			fitness[birthPlayer] = exp(s * ((tolerability > number[2] ? 1 : (tolerability * 1.0 / (number[2] + 1))) * (b - c) - gamma * number[2] * 1.0 / (N - 1)));
		}
		else if (strategy[birthPlayer] == 1)
		{
			fitness[birthPlayer] = exp(s * (tolerability > number[2] ? 1 : (tolerability * 1.0 / (number[2] + 1))) * (b - c));
		}
		else
		{
			fitness[birthPlayer] = exp(s * ((tolerability > number[2] ? 1 : (tolerability * 1.0 / number[2])) * b - beta * number[0] * 1.0 / (N - 1)));
		}


		//fitness of dieplayer
		if (strategy[diePlayer] == 0)
		{
			fitness[diePlayer] = exp(s * ((tolerability > number[2] ? 1 : (tolerability * 1.0 / (number[2] + 1))) * (b - c) - gamma * number[2] * 1.0 / (N - 1)));
		}
		else if (strategy[diePlayer] == 1)
		{
			fitness[diePlayer] = exp(s * (tolerability > number[2] ? 1 : (tolerability * 1.0 / (number[2] + 1))) * (b - c));
		}
		else
		{
			fitness[diePlayer] = exp(s * ((tolerability > number[2] ? 1 : (tolerability * 1.0 / number[2])) * b - beta * number[0] * 1.0 / (N - 1)));
		}


		imitate_probability = 1.0 / (1 + fitness[diePlayer] * 1.0 / fitness[birthPlayer]);

		//updating procee
		if (temprand > mu) //imitate
		{
			if (rand() * 1.0 / RAND_MAX <= imitate_probability)
			{
				number[strategy[birthPlayer]]++;
				number[strategy[diePlayer]]--;
				strategy[diePlayer] = strategy[birthPlayer];
			}

		}
		else  //mutate
		{
			int tempstrat = rand() % 3;
			number[tempstrat]++;
			number[strategy[diePlayer]]--;
			strategy[diePlayer] = tempstrat;

		}


	}

	aveP = aveP * 1.0 / N;
	aveC = aveC * 1.0 / N;
	aveD = aveD * 1.0 / N;
}

int main()
{
	
	srand((unsigned)time(NULL));
	int i, k;
	FILE* fp;
	void srand(unsigned int seed);
	fp = fopen("generations.txt", "w");
	if (fp == NULL)
	{
		printf("error");
	}

	for (i = 0; i <= 20; i++)
	{
		//initialize population composition
		aveP = 0, aveC = 0, aveD = 0;
		for (k = 1; k <= N; k++)
		{
			strategy[k - 1] = rand() % 3;
		}
		number[0] = 0; number[1] = 0; number[2] = 0;
		for (k = 1; k <= N; k++)
		{
			if (strategy[k - 1] == 0)
				number[0]++;
			else if (strategy[k - 1] == 1)
				number[1]++;
			else
				number[2]++;
		}

		// begin evolution
		s = pow(10, -3 + i * 0.2);
		evolution();

		//print result
		fprintf(fp, "%f	%f	%f	%f\n", s, aveP, aveC, aveD);
		
	}

	fclose(fp);
}
