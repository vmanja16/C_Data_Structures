/**
 *  Mergesort.c 
 *  Sort the inputted integers, using merge-sort algorithm.
 **/


#include <stdio.h>
#include <stdlib.h>
 #include <time.h>
 #include <assert.h>


/**
 * merge() 
 * Merge two sorted arrays, A with a  integers and 
 * B with b integers, into a sorted array C.
 **/
void 
merge (int *A, int a, int *B, int b, int *C) 
{
  int i,j,k;
  i = 0; 
  j = 0;
  k = 0;
  while (i < a && j < b) {
    if (A[i] <= B[j]) {
	  /* copy A[i] to C[k] and move the pointer i and k forward */
	  C[k] = A[i];
	  i++;
	  k++;
    }
    else {
      /* copy B[j] to C[k] and move the pointer j and k forward */
      C[k] = B[j];
      j++;
      k++;
    }
  }
  /* move the remaining elements in A into C */
  while (i < a) {
    C[k]= A[i];
    i++;
    k++;
  }
  /* move the remaining elements in B into C */
  while (j < b)  {
    C[k]= B[j];
    j++;
    k++;
  }
}  

/**
 * merge_sort()
 * Sort array A with n integers, using merge-sort algorithm.
 **/
void 
merge_sort(int *A, int n) 
{
  int i;
  int *A1, *A2;
  int n1, n2;

  if (n < 2)
    return;   /* the array is sorted when n=1.*/
  
  /* divide A into two array A1 and A2 */
  n1 = n / 2;   /* the number of elements in A1 */
  n2 = n - n1;  /* the number of elements in A2 */
  A1 = (int*)malloc(sizeof(int) * n1);
  A2 = (int*)malloc(sizeof(int) * n2);
  
  /* move the first n/2 elements to A1 */
  for (i =0; i < n1; i++) {
    A1[i] = A[i];
  }
  /* move the rest to A2 */
  for (i = 0; i < n2; i++) {
    A2[i] = A[i+n1];
  }
  /* recursive call */
  merge_sort(A1, n1);
  merge_sort(A2, n2);

  /* conquer */
  merge(A1, n1, A2, n2, A);
  free(A1);
  free(A2);
}


int main(void)
{

	clock_t start, end;
	double Timer;
	 int i, n = 1000000;
  start = clock();
  int *A;
  A = (int *)malloc(sizeof(int) * n);

  for (i = 0; i < n; i++){ A[i] = rand(); }  

  /* merge sort A */

  merge_sort(A, n);
  end = clock();

  Timer = (double) (end - start)/( (double) CLOCKS_PER_SEC );
  printf(" TIme for mergesort is %lf\n", Timer);

for (i = 1; i < n; i++){ assert( A[i] >= A[i-1] ); }




  free (A);

return 0;
}