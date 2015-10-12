// Noah Smith smit1988
// Vikram Manja vmanja





#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef struct Nodes
{
  
  int priority;
  int Time;
  int Servicetimes[32];
  struct Nodes * next; 


} Nodetype;



int main (int argc, char ** argv) 
{

  int TasksRemain = 0;
  int NumberofLines = 2;
 // int TasksRemain0 = 2;
  //int TasksRemain1 = 2;
  //float Lambda0, Lambda1, AvgServiceTime;
  FILE * fp2 = NULL;
  char buffer[1024];

  
Nodetype * Arrival_createNode(int priorityofarrival, int T, FILE * filepointer)

{
        Nodetype * NewNode = malloc(sizeof(Nodetype));
        NewNode->priority = priorityofarrival;
        NewNode->Time = T;

        char buffer2[100];
    //    char Number = fgetc(filepointer);

/*        int I = 0;
        while ( Number != '\n')
        {
          fscanf(fp2, "%d", NewNode->Servicetimes[I]));
          
        }  
*/     int I = 0;
        while (fgetc(filepointer) != '\n')
        {
          fseek(filepointer, -1, SEEK_CUR);


        //fgets(buffer2, 2, filepointer);
        
        fscanf(filepointer, "%d", &NewNode->Servicetimes[I]); 

        I++;
        
        }
        NewNode->next = NULL;
        return NewNode;
}





  if ( argc == 2)
  {
   fp2 = fopen(argv[1], "r"); // opening file

      while (feof(fp2) == 0) // counting lines
	{
	  fgets(buffer,1023,fp2);
	  TasksRemain++;
	}
    
    TasksRemain--;
    fseek(fp2, 0, SEEK_SET); // fp2 @ beginning of file

    NumberofLines = TasksRemain;
   


  Nodetype * Head = Arrival_createNode(1, 1, fp2);
  Nodetype * Head2 = Arrival_createNode(2, 5, fp2);

  printf(" %d %d %d %d %d ", Head->Servicetimes[0], Head->Servicetimes[1], Head->Servicetimes[2],Head->Servicetimes[3], Head->Servicetimes[4] );
  printf(" %d %d %d %d %d ", Head2->Servicetimes[0], Head2->Servicetimes[1], Head2->Servicetimes[2],Head2->Servicetimes[3], Head2->Servicetimes[4] );








  


  }










if (argc == 2) 
{
 fclose(fp2);
}

  return 0;
}
