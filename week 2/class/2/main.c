#include <stdio.h>

int main(void)
{
	int i;
	double num;

	for (i=0; i<5; i++) {
		double temp = i;
		num = temp/2+i;
//		num = i/2+i;
		printf("num is %f \n",num);
	}
	return 0;
}
