// Vikram Manja
// vmanja

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>

void quickSort( int[], int, int);
void Insertionsort(int [], int, int );
int partition( int[], int, int);
void sort(int list[], int n);

void sort(int list[], int n)
{
  int i=0, j=0;
  int count = 0; 
 
  //printf(" N is = %d\n", n);

   for (j = 0; j <= 7; j++)
   {
    i = j * n/10;
    while(list[i] <= list[i+1])
        { 
          if (i ==(n-1) ) {break;}
          count++; 
          i++; 
        if (count > n/5) {break;}

        }
   }
   //printf(" COUNT IS %d\n", count );

  if (count > n/5) 
  {
   //quickSort( list, 0,n-1);
  
  Insertionsort( list, 0, n-1);
  }
  else 
  {
        quickSort( list, 0,n-1);
  }
  
}


void Insertionsort(int list[], int lb, int ub) 
{
   int I;
   int J;
   int Temp;

  for (I = lb; I < ub; I++)
  { 
      J = I;
      while ( (J > lb) && (list[J] < list[J-1]) ) 
   {
     Temp = list[J-1];
    list[J-1] = list[J];
    list[J] = Temp;
    J--;
    }

  }

}


/*void quickSort( int a[], int lb, int ub)
{
   int i;

   if( lb < ub ) 
   {
       i = partition( a, lb, ub);
       quickSort( a, lb, i-1);
       quickSort( a, i+1, ub);
   }
*/  





void quickSort( int a[], int lb, int ub)
{

  int sTack[ub - lb  + 1];   // initializing sTack
  int index = 0;
  int i;

  sTack[0] = lb;               // push lowerbound
  sTack[++index] = ub;        // push upperbound

  while ( index >= 0) 
  {
    ub = sTack[ index--];
    lb = sTack[ index--];

    i = partition( a, lb, ub);

    if ( (i -1) > lb)        // left
    {
      sTack[ ++index] = lb;
      sTack[ ++index] = i -1;
    }

    if ( ( i + 1) < ub )     // right
    {
       sTack[ ++index] = i + 1;
      sTack[ ++index] = ub;
    }


  }
}


int partition( int a[], int Left, int Right) 
{
   int pivot, temp, index;
  // pivot = a[lb];

   index =  Left + (Right - Left)/2 ;  // Middle pivot
   pivot = a[index];

    
   while( Left <= Right)
   {
      while( (a[Left] <= pivot)  && (Left <= Right)){Left++;} // move left
      while( (a[Right] > pivot) ) {Right--;}             // move right
   
    
      if ( Right < Left){break;}

      if ( Right == index) { index = Left;}
    
    
      temp = a[Left];              // exchange right and left
      a[Left] = a[Right];
      a[Right] = temp; 
      
    }

   //temp = pivot;         // exchange pivot w/ right
   a[ index] = a[Right]; 
   a[Right] = pivot;
   return Right;      // Right is index of positioned pivot
}



int main (void)
{
  int n = 1000000;
  int A[n];
   int i;
  for (i = 0; i < n; i++){ A[i] = rand(); }  

    clock_t start, end;
    double Timer;
   start = clock(); 
  quickSort(A, 0, n-1);
  end = clock();
  Timer = (double) (end - start)/( (double) CLOCKS_PER_SEC );
  printf(" TIme for mergesort is %lf\n", Timer);

for (i = 1; i < n; i++){ assert( A[i] >= A[i-1] ); }
return 0;
}









