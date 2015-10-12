//void sort(int list[], int n);

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>


void sort(int list[], int n);

int main(void) 
{
	int n = 1000000;
  int a[n], b[n];
  int i = 0;
  int t = 0; int J = n-1;


	//srand( time(NULL));		
    for(i=0;i<n;i++){a[i] = rand(); b[i] = random();} 

  clock_t start, end;
  double QUICKTIME;

/*--------------------------RANDOM----------------------*/
  start = clock();
  sort(b,n); 
  end = clock();
  QUICKTIME = (double)(end-start)/( (double) CLOCKS_PER_SEC);
  printf("\n\n random list time = %lf\n\n", QUICKTIME);

  for (i = 1; i < n; i++){assert (b[i] >= b[i-1]);} //CHECK if Sorted



	start = clock();
  sort(a,n); 
  end = clock();
	QUICKTIME = (double)(end-start)/( (double) CLOCKS_PER_SEC);
	printf("\n\n random list time = %lf\n\n", QUICKTIME);

  for (i = 1; i < n; i++){assert (a[i] >= a[i-1]);} //CHECK if Sorted

/*---------------------------SORTED-----------------------------*/
  start = clock();
  for (i = 0; i < 10; i++) { a [i * n/10] = 5; }
  sort(a,n); 
  end = clock();
        QUICKTIME = (double)(end-start)/( (double) CLOCKS_PER_SEC);
        printf("\n\n  FULLY sorted list time = %lf\n\n", QUICKTIME);



	for (i = 1; i < n; i++)//{printf("%d\n ", a[i]);
    {assert (a[i] >= a[i-1]);} //CHECK if Sorted

	/*--------------REVERSE SORTED-----------------*/
	for ( i = 0; i< n/2; i++) { t=a[J]; a[J]=a[i]; a[i]=t; J--;}  
  start = clock();
  sort(a,n);
	end = clock();
        QUICKTIME = (double)(end-start)/( (double) CLOCKS_PER_SEC);
        printf("\n\n  Reverse sorted list time = %lf\n\n", QUICKTIME);

  for (i = 1; i < n; i++){assert (a[i] >= a[i-1]);} //CHECK if Sorted
      

	return 0;
}
