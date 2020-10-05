
#include "stdio.h"
int sushu(int dat)
{
	int i;
	for (i = 2; i < dat; i++)
	{
		if (dat % i == 0) return 0;	
	}
	return 1;
}
int main()
{
	int i, result;
	for (i = 2; i < 100; i++)
	{
		if(i==2) printf("%d ", i);
		else if (sushu(i) == 1)
			printf("%d ", i);
	}
	return 0;
}/**/