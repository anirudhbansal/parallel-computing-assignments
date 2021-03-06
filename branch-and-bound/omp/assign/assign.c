#include "openmp_template.h"
#include <omp.h>
int n;

int max(int a,int b) { return (a>b)?a:b; }
int min(int a,int b) { return (a<b)?a:b; }


solnptr init_solve()
{
	solnptr sptr = (solnptr)malloc(sizeof(struct solution));
	sptr->num = 0;
	sptr->cost = 0;
	return sptr;
}

solnptr naive_solve()
{
	solnptr sptr = (solnptr)malloc(sizeof(struct solution));
	sptr->num = n;
	sptr->cost = MAX_COST;
	return sptr;
}

int sol_bound(solnptr s)
{
	int bound = 0;
	bool remjob[n+1];

	for(int i = 0 ; i < n ; i++)
		remjob[i] = true;

	for(int i = 0 ; i < s->num ; i++)
		remjob[s->job[i]] = false;

	for(int i = s->num ; i < n ; i++)
	{
		int lbound = INT_MAX;

		for(int j = 0 ; j < n ; j++)
		{
			if(remjob[j])
				lbound = min(lbound , cost[i][j]);
		}
		bound += lbound;
	}

	return bound;
}

int sol_extend(solnptr sol,solnptr new_sol[MAX_SOL])
{
	bool remjob[n+1];

	for(int i = 0 ; i < n ; i++)
		remjob[i] = true;

	for(int i = 0 ; i < sol->num ; i++)
		remjob[sol->job[i]] = false;

	int count = 0;

	for(int i = 0 ; i < n ; i++)
	{
		if(remjob[i])
		{
			solnptr s = (solnptr)malloc(sizeof(struct solution));
			s->cost = sol->cost + cost[sol->num][i];
			s->num = sol->num + 1;

			for(int j = 0 ; j < sol->num ; j++)
				s->job[j] = sol->job[j];

			s->job[sol->num] = i;
			new_sol[count++] = s;			
		}
	}

	return count;
}

bool sol_done(solnptr sol)
{
	return(sol->num == n);
}

void print_sol(solnptr s)
{
	if(s == NULL)
		return;

	printf("Number of agents/tasks is %d\n",s->num);
	for(int i = 0; i < s->num ; i++)
		printf("Person %d is assigned job %d\n",i+1,s->job[i] + 1);
	printf("Optimal cost is %d\n",s->cost);
	printf("\n");
}

int main(int argc, char const *argv[])
{
	scanf("%d", &n);
	int i,j;
	for(i = 0 ; i < n ; i++)
	{
		for(j = 0 ; j < n ; j++)
			scanf("%d" , &cost[i][j]);
	}

	double start = omp_get_wtime();
	solnptr s = evaluate();
	double duration = omp_get_wtime() - start;
	printf("Time taken (in seconds) : %lf\n",duration);
	print_sol(s);

	return 0;
}