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
  int subtasks;
  int Servicetimes[32];
  int Loadbalance;
  struct Nodes * next; 

} ArrivalNode;

typedef struct Nodes2
{
  int Time;
  struct Nodes2 * next;

} DepartureNode;

ArrivalNode * Arrival_createNode(FILE * filepointer);


int GetTime(float LamOrServ);

int GetTime(float LamOrServ)
{
  double X = (double)rand()/ RAND_MAX; // Generates X value between 0 and 1
  int R0 = ceil(-(1/LamOrServ) * log(1-X));  // Generates IA time R value
  return R0;
}


ArrivalNode * Arrival_createNode(FILE * filepointer)

{
  ArrivalNode * NewNode = malloc(sizeof(ArrivalNode));
  fscanf (filepointer, "%d %d %d " , &NewNode->Time, &NewNode->priority, &NewNode->subtasks);
  int I = 0 ,C = 0, sum = 0, max = 0, min = 0;
  double avg = 0;
  for (I = 0; I <  NewNode->subtasks; I++)
  {
    fscanf(filepointer, "%d", &NewNode->Servicetimes[I]);
  }

  min = NewNode->Servicetimes[0];
  max = NewNode->Servicetimes[0];
  for (C =0; C < NewNode->subtasks; C++)
  {
    if (NewNode->Servicetimes[C] < min){min = NewNode->Servicetimes[C];}
    if (NewNode->Servicetimes[C] > max){max = NewNode->Servicetimes[C];}
    sum = sum + NewNode->Servicetimes[C];
  }
  avg = ( (double) sum ) / ( (double) NewNode->subtasks);

  NewNode->Loadbalance =  ( (double) (max - min) ) / ((double) avg);

  NewNode->next = NULL;
  printf("Creating Arrival node\n" );
  return NewNode;
}



int main (int argc, char ** argv) 
{

  int TasksRemain = 0;
  int NumberofLines = 2;
  FILE * fp2 = NULL;
  char buffer[1024];
  int TasksRemain0 = 2;
  int TasksRemain1 = 2;
  float Lambda0, Lambda1, AvgServiceTime;
  int i = 0;
  int simtime;
  int time;
  int j = 0;
  int newdeptime;
  int worked = 0;
  double Average_Load_Balance = 0;
  double Cumulative_wait_time0 = 0;
  double Cumulative_wait_time1 = 0;
  double Previous_Queue0 = 0;
  double Previous_Queue1 = 0;
  double Previous_T = 0;
  double CumulServer = 0;
  double Previous_Server = 0;
  ArrivalNode Arrival_head, List1_head, List0_head;
  DepartureNode Departure_head;
  int Free_Servers = 64;

  int Max = 0;
  int Min = 0;
  

  ArrivalNode * ptr1 = NULL, * CurrentNode = NULL, * ptr2= NULL, * NextNode = NULL, * ptr3 = NULL;
  DepartureNode * Dep_ptr1 = NULL, * Dep_ptr2 = NULL;

  int Indicator=0, QueueLength0 = 0, QueueLength1 = 0;

int time1 = 0;
/*---------------------------Creating ARRIVAL NODE-----------------------------*/  


/**************************************-MODE 1*************************/
/***************------------------------MODE 1******---------------*******/
  if (argc > 2)  // MODE 1
    {
      Lambda0 = atof(argv[1]);
      Lambda1 = atof(argv[2]);
      AvgServiceTime = atof(argv[3]);

      TasksRemain = atoi(argv[4]);
      TasksRemain0 = TasksRemain;
      TasksRemain1 = TasksRemain;

      printf("%d", TasksRemain);
    
// INITIALIZING First nodes



List1_head.next = malloc(sizeof(ArrivalNode));
ptr1 = List1_head.next;
ptr1->Time = 0;
ptr1->priority = 1;
ptr1->subtasks = rand() % 32 + 1;
for (i=0; i < ptr1->subtasks; i++)
{
  ptr1->Servicetimes[i] = GetTime(AvgServiceTime);  // ptr on first node
}




List0_head.next = malloc(sizeof(ArrivalNode));
ptr2 = List0_head.next;
ptr2->Time = 0;
ptr2->priority = 0;
ptr2->subtasks = rand() % 32 + 1;
for (i=0; i < ptr2->subtasks; i++)
{
  ptr2->Servicetimes[i] = GetTime(AvgServiceTime); // ptr on first node
} 
                                      

// CREATING ARRIVAL LIST
time = 0;
time1 = 0;


for (j = 1; j < TasksRemain; j++)
{

ptr1->next = malloc(sizeof(ArrivalNode));
ptr1 = ptr1->next;
ptr1->Time = GetTime(Lambda1) + time1;
time1 = ptr1->Time;
ptr1->priority = 1;
ptr1->subtasks = rand() % 32 + 1;
for (i=0; i < ptr1->subtasks; i++)
{
  ptr1->Servicetimes[i] = GetTime(AvgServiceTime);
}

ptr1->next = NULL;
//printf("Time 1: %d\n", time1);
///printf("%d\n", time);

//printf ("Line 187\n");

ptr2->next = malloc(sizeof(ArrivalNode));
ptr2 = ptr2->next;
ptr2->Time = GetTime(Lambda0) + time;
time = ptr2->Time;
ptr2->priority = 0;
ptr2->subtasks = rand() % 32 + 1;
for (i=0; i < ptr2->subtasks; i++)
{
  ptr2->Servicetimes[i] = GetTime(AvgServiceTime);
}

ptr2->next = NULL;

//printf("%d\n", time1);
//printf("Time 0 : %d\n", time);

}

ptr1 = List1_head.next;
while (ptr1 != NULL){printf("%d\n", ptr1->Time ); ptr1 = ptr1->next;}
//List1_head 
//List0_head
//printf("Line 207\n");

ptr1 = List1_head.next;
ptr1 = ptr1->next; // ptr on 2nd node of list 1
ptr2  = List0_head.next;
ptr2 = ptr2->next; // ptr2 on 2nd node of list 0
Arrival_head.next = List0_head.next;
ptr3 = Arrival_head.next;
ptr3->next = List1_head.next;
ptr3 = ptr3->next; // ptr3 on 2nd node of arrival

//printf("Line 218\n");


while ( (ptr1 != NULL) && (ptr2 != NULL) )
{
  if (ptr1->Time < ptr2->Time)
  {
    ptr3->next = ptr1;
    ptr1 = ptr1->next;
    ptr3 = ptr3->next;
 
  }
  else
  {
    ptr3->next = ptr2;
    ptr2 = ptr2->next;
    ptr3 = ptr3->next;
  }

}

if (ptr1 != NULL){ ptr3->next = ptr1;}
else {ptr3->next = ptr2;}

// # ARRIVAL LIST HAS BEEN CREATED ! *

// Calculatin LOAD balance

//printf("Line 246\n");

ptr3 = Arrival_head.next;

//while (ptr3 != NULL){printf("%d\n", ptr3->Time); ptr3 = ptr3->next;}

ptr3 = Arrival_head.next;

  while (ptr3 != NULL)
  {
    Min = ptr3->Servicetimes[0];
    Max = ptr3->Servicetimes[0];
    for (i =0; i < ptr3->subtasks; i++)
    {
      if (ptr3->Servicetimes[i] < Min){Min = ptr3->Servicetimes[i];}
      if (ptr3->Servicetimes[i] > Max){Max = ptr3->Servicetimes[i];}
    }
   Average_Load_Balance = Average_Load_Balance + ( (double) (Max - Min)/ ( (double) AvgServiceTime) ) ;
   ptr3 = ptr3->next;
  }

Average_Load_Balance = (Average_Load_Balance / (TasksRemain0 * 2) );

//printf("Line 265\n");

} //end if argc > 2


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

 //   printf("Starting Node creation\n");
  /*-----------------------------------CREATING LINKED LIST OF ARRIVALS--------------------------------*/
    Arrival_head.next = Arrival_createNode(fp2);
    ptr1 = Arrival_head.next;

  //  printf("Created first node\n");
    for (i = 1; i < NumberofLines -1; i++)
    {
      ptr1->next = Arrival_createNode(fp2);
      ptr1 = ptr1->next; 
      //printf("%d", ptr1->Time);
    }

  //  printf("THE # of lines is %d\n\n" ,NumberofLines);

   ptr1 = Arrival_head.next;
   while (ptr1 != NULL){ Average_Load_Balance = Average_Load_Balance + ptr1->Loadbalance; ptr1 = ptr1->next;}
   Average_Load_Balance = Average_Load_Balance/ ((double) (NumberofLines-1)); 
    
    } // END MODE 2


    //Initializing Times
    
    CurrentNode = Arrival_head.next;
    simtime = CurrentNode->Time;
    Previous_T = simtime;
    time = CurrentNode->Time;
    Departure_head.next = NULL;
/*************************************************--------------------WHILE LOOP-----------------------------******************/

    while ( (Arrival_head.next != NULL) || (Departure_head.next != NULL) ) //while departures/arrivals still exist
    {
      
    //printf("WHILE\n");

  Cumulative_wait_time0 += (time - Previous_T) * Previous_Queue0;
  Cumulative_wait_time1 += (time - Previous_T) * Previous_Queue1;
  CumulServer += (time - Previous_T) * Previous_Server;
  Previous_T = time;


    /*----------------------DEPARTURES---------------------------*/
      if (Departure_head.next != NULL) // Look for departures if they exist
      { 
        i = 0;

        while ( (Departure_head.next)->Time == time) //If departures occur, pop front node and increment freeservers
        { 
          Free_Servers++;   Previous_Server = Free_Servers;
          Dep_ptr1 = Departure_head.next;
          Dep_ptr2 = Dep_ptr1->next;
          Departure_head.next = Dep_ptr2;
          //Dep_ptr1->next = NULL;
          //free(Dep_ptr1->next);
          free(Dep_ptr1);
          i++;

      //    printf("DEPARTING\n");
          if (Departure_head.next == NULL){ break;}
        }
      }

    //  printf("\n\n %d Departures", i);
   //   printf("Free Servers = %d\n", Free_Servers);
   
    if (Arrival_head.next == NULL){ if (Departure_head.next == NULL){break;} time = (Departure_head.next)->Time; printf("\nTime = %d\n\n", time); continue;}

  //  printf("Free Servers = %d\n", Free_Servers);

    if (time == 45){ ptr1 = &Arrival_head; 
      while (ptr1->next != NULL)
      { 
        //printf( "     %d" ,(ptr1->next)->Time);
        ptr1 = ptr1->next; 
      }
    }

      /*----------------------------------Check queuelist for priority 0----------------------------------*/
      ptr1 = &Arrival_head;
      //printf("%d\n",(ptr1->next)->Time);
      while ( (ptr1->next)->Time < time) // || ( ( Indicator < 0) && (CurrentNode->Time = time) ) )  
      {
      //  printf("checking 0\n");
        if ( (ptr1->next)->priority == 0)
        {
      //    printf("found 0\n");
          
         if ( (ptr1->next)->subtasks <= Free_Servers)
         {
          ptr2 = ptr1->next;
          if (ptr2 == CurrentNode)
            { 
              if (ptr2->next == NULL){ j =1;}
              else if(ptr2->Time != (ptr2->next)->Time ) 
              {
                CurrentNode = ptr1;
              } 
              else 
              {
                CurrentNode = CurrentNode->next;
              }
            }
          QueueLength0--; Previous_Queue0 = QueueLength0;
          Free_Servers = Free_Servers - ptr2->subtasks;   Previous_Server = Free_Servers;
          Dep_ptr1 = &Departure_head;
           // CREATING DEPARTURE NODES
          for(i = 0; i < (ptr2->subtasks) ; i++)
          {
            worked = 0;
            Dep_ptr1 = &Departure_head;
            newdeptime = time + ptr2->Servicetimes[i];
            
            if (Dep_ptr1->next == NULL)        // if Departures empty
              {
              //  printf("Line 183\n");
               Dep_ptr1->next = malloc(sizeof(DepartureNode));
               Dep_ptr1 = Dep_ptr1->next;
               Dep_ptr1->Time = newdeptime;
               Dep_ptr1->next = NULL;
               continue;
              }

            // departures not empty

            while (Dep_ptr1->next != NULL)
            {
             // printf("Line 195\n");
              if ( (Dep_ptr1->next)->Time > newdeptime)
              {
                Dep_ptr2 = Dep_ptr1->next;
                Dep_ptr1->next = malloc(sizeof(DepartureNode));
                Dep_ptr1 = Dep_ptr1->next;
                Dep_ptr1->next = Dep_ptr2;
                Dep_ptr1->Time = newdeptime;
                worked = 1;
                break;
              }
              Dep_ptr1 = Dep_ptr1->next;
             }  

             if (worked == 1){continue;} 

              if (Dep_ptr1->next == NULL)        // if Departures empty
              {
               Dep_ptr1->next = malloc(sizeof(DepartureNode));
               Dep_ptr1 = Dep_ptr1->next;
               Dep_ptr1->Time = newdeptime;
               Dep_ptr1->next = NULL;
               continue;
              }


            }


            
          ptr1->next = ptr2->next;
          //free(ptr2->subtasks);
          free (ptr2); // Pop arrival node
          continue;         
         // if (ptr == NULL) {}
         }
        }
        ptr1 = ptr1->next;
        //Indicator++;
      }

/*-----------------------------Check queuelist for priority 1------------------------------------*/
      //if (Arrival_head.next == NULL){time = (Departure_head.next)->Time; printf("\nTime = %d\n\n", time); continue;}
      if (Arrival_head.next == NULL){ if (Departure_head.next == NULL){break;} time = (Departure_head.next)->Time; printf("\nTime = %d\n\n", time); continue;}

      ptr1 = &Arrival_head;
      while ( (ptr1->next)->Time < time) //|| (( Indicator < 1) && (CurrentNode->Time = time) ) ) 
 
      {
        //printf("Checkin 1 \n");

        if ( (ptr1->next)->priority == 1)
        {
      //   printf("Found 1\n");
         if ( (ptr1->next)->subtasks <= Free_Servers)
         {
          ptr2 = ptr1->next;
          if (ptr2 == CurrentNode){ if (ptr2->next == NULL){j = 1;} else if(ptr2->Time != (ptr2->next)->Time ) {CurrentNode = ptr1;} else {CurrentNode = CurrentNode->next;} }
          QueueLength1--; Previous_Queue1 = QueueLength1;
          Free_Servers = Free_Servers - ptr2->subtasks;   Previous_Server = Free_Servers;
          Dep_ptr1 = &Departure_head;

           // CREATING DEPARTURE NODES
          for(i = 0; i < (ptr2->subtasks) ; i++)
          {
            worked = 0;
            Dep_ptr1 = &Departure_head;
            newdeptime = time + ptr2->Servicetimes[i];
         //   printf("%d\n", newdeptime );
            
            if (Dep_ptr1->next == NULL)        // if Departures empty
              {
               Dep_ptr1->next = malloc(sizeof(DepartureNode));
               Dep_ptr1 = Dep_ptr1->next;
               Dep_ptr1->Time = newdeptime;
               Dep_ptr1->next = NULL;
               continue;
              }

            // departures not empty

            while (Dep_ptr1->next != NULL)
            {
              if ( (Dep_ptr1->next)->Time > newdeptime)
              {
                Dep_ptr2 = Dep_ptr1->next;
                Dep_ptr1->next = malloc(sizeof(DepartureNode));
                Dep_ptr1 = Dep_ptr1->next;
                Dep_ptr1->next = Dep_ptr2;
                Dep_ptr1->Time = newdeptime;
                worked =1;
                break;
              }
              Dep_ptr1=Dep_ptr1->next;
             }  

                  if (worked == 1){continue;} 

              if (Dep_ptr1->next == NULL)        // if Departures empty
              {
               Dep_ptr1->next = malloc(sizeof(DepartureNode));
               Dep_ptr1 = Dep_ptr1->next;
               Dep_ptr1->Time = newdeptime;
               Dep_ptr1->next = NULL;
               continue;
              }


            }


            
          ptr1->next = ptr2->next;
          //free(ptr2->subtasks);
          free (ptr2); // Pop arrival node
          if (ptr1->next == NULL) {break;}
          continue;         
         }
         }
        
        ptr1 = ptr1->next;
        if (ptr1->next == NULL){break;}
     //   Indicator++;
        
      }

 if (time ==45){printf("Line 324\n");}
      Indicator=5; // random positive int
      
      
  /* CHECK IF ARRIVAL(s) OCCURED  */
     // if ((QueueLength1==0) && (QueueLength0 ==0)) { Indicator = 0;}  

      if (CurrentNode->Time == time){ if (CurrentNode->priority == 0) {QueueLength0++; Previous_Queue0 = QueueLength0;} if (CurrentNode->priority == 1){QueueLength1++; Previous_Queue1 = QueueLength1;} 
        if(Indicator==0){Indicator--;} }
      NextNode = CurrentNode->next;
      if (NextNode != NULL)
        {
          if (NextNode->Time == CurrentNode->Time) { CurrentNode = CurrentNode->next;  if (CurrentNode->priority == 0) {QueueLength0++; Previous_Queue0 = QueueLength0;} if (CurrentNode->priority == 1){QueueLength1++; Previous_Queue1 = QueueLength1;} 
        if(Indicator == -1){Indicator--;} }
        }
      
      j = Indicator;    

    //if (Arrival_head.next == NULL){time = (Departure_head.next)->Time; printf("\nTime = %d\n\n", time);continue;}
        if (Arrival_head.next == NULL){ if (Departure_head.next == NULL){break;} time = (Departure_head.next)->Time; printf("\nTime = %d\n\n", time); continue;}
 

       ptr1 = &Arrival_head;
    //  printf("LINE 341\n");
      while ( (ptr1->next)->Time <= time) // || ( ( Indicator < 0) && (CurrentNode->Time = time) ) )  
      {

     //   if ((ptr1->next)->next == NULL) {}

     //   printf("checking 0\n");
        if ( (ptr1->next)->priority == 0)
        {
       //   printf("found 0\n");
          
         if ( (ptr1->next)->subtasks <= Free_Servers)
         {
          ptr2 = ptr1->next;
          if (ptr2 == CurrentNode)
             
            { 
              if (ptr2->next == NULL){j = 1;}
              else if(ptr2->Time != (ptr2->next)->Time ) 
              {
                CurrentNode = ptr1;
              } 
              else 
              {
                CurrentNode = CurrentNode->next;
              } 
            }
          QueueLength0--;Previous_Queue0 = QueueLength0;
          Free_Servers = Free_Servers - ptr2->subtasks;   Previous_Server = Free_Servers;
          Dep_ptr1 = &Departure_head;

          // CREATING DEPARTURE NODES
          for(i = 0; i < (ptr2->subtasks) ; i++)
          {
            worked = 0;
            Dep_ptr1 = &Departure_head;
            newdeptime = time + ptr2->Servicetimes[i];
            //printf("%d\n", newdeptime );
            
            if (Dep_ptr1->next == NULL)        // if Departures empty
              {
               Dep_ptr1->next = malloc(sizeof(DepartureNode));
               Dep_ptr1 = Dep_ptr1->next;
               Dep_ptr1->Time = newdeptime;
               Dep_ptr1->next = NULL;
               continue;
              }

            // departures not empty

            while (Dep_ptr1->next != NULL)
            {
              if ( (Dep_ptr1->next)->Time >= newdeptime)
              {
                Dep_ptr2 = Dep_ptr1->next;
                Dep_ptr1->next = malloc(sizeof(DepartureNode));
                Dep_ptr1 = Dep_ptr1->next;
                Dep_ptr1->next = Dep_ptr2;
                Dep_ptr1->Time = newdeptime;
                worked = 1;
                break;
              }
              Dep_ptr1 = Dep_ptr1->next;
             }  

             if (worked == 1){continue;} 

              if (Dep_ptr1->next == NULL)        // if Departures empty
              {
               Dep_ptr1->next = malloc(sizeof(DepartureNode));
               Dep_ptr1 = Dep_ptr1->next;
               Dep_ptr1->Time = newdeptime;
               Dep_ptr1->next = NULL;
               continue;
              }


            }


            
          ptr1->next = ptr2->next;
          //free(ptr2->subtasks);
          free (ptr2); // Pop arrival node
          if (ptr1->next == NULL){break;}
          continue;         
         }

         }
        
        ptr1 = ptr1->next;
        if(ptr1->next == NULL){break;}
        //Indicator++;
      }
 //if (time ==45){printf("Line 430\n");}

/*-----------------

------------Check queuelist for priority 1------------------------------------*/

     // if (Arrival_head.next == NULL){time = (Departure_head.next)->Time; printf("\nTime = %d\n\n", time); continue;}
          if (Arrival_head.next == NULL){ if (Departure_head.next == NULL){break;} time = (Departure_head.next)->Time; printf("\nTime = %d\n\n", time); continue;}


      ptr1 = &Arrival_head;
      while ( (ptr1->next)->Time <= time) //|| (( Indicator < 1) && (CurrentNode->Time = time) ) ) 
 
      {
     //   printf("Checkin 1 \n");

        if ( (ptr1->next)->priority == 1)
        {
     //    printf("Found 1\n");
         if ( (ptr1->next)->subtasks <= Free_Servers)
         {
          ptr2 = ptr1->next;
          if (ptr2 == CurrentNode){ if (ptr2->next == NULL){j = 1;} else if(ptr2->Time != (ptr2->next)->Time ) {CurrentNode = ptr1;} else {CurrentNode = CurrentNode->next;} }
          QueueLength1--; Previous_Queue1 = QueueLength1;
          Free_Servers = Free_Servers - ptr2->subtasks;   Previous_Server = Free_Servers;
          Dep_ptr1 = &Departure_head;
      // CREATING DEPARTURE NODES
          for(i = 0; i < (ptr2->subtasks) ; i++)
          {
            worked = 0;
            Dep_ptr1 = &Departure_head;
            newdeptime = time + ptr2->Servicetimes[i];
           // printf("%d\n", newdeptime);
            
            if (Dep_ptr1->next == NULL)        // if Departures empty
              {
               Dep_ptr1->next = malloc(sizeof(DepartureNode));
               Dep_ptr1 = Dep_ptr1->next;
               Dep_ptr1->Time = newdeptime;
               Dep_ptr1->next = NULL;
               continue;
              }

            // departures not empty

            while (Dep_ptr1->next != NULL)
            {
              if ( (Dep_ptr1->next)->Time > newdeptime)
              {
                worked = 1;
                Dep_ptr2 = Dep_ptr1->next;
                Dep_ptr1->next = malloc(sizeof(DepartureNode));
                Dep_ptr1 = Dep_ptr1->next;
                Dep_ptr1->next = Dep_ptr2;
                Dep_ptr1->Time = newdeptime;
                break;
              }
              Dep_ptr1 = Dep_ptr1->next;
             }  

              if (worked == 1){continue;} 

              if (Dep_ptr1->next == NULL)        // if Departures empty
              {
               Dep_ptr1->next = malloc(sizeof(DepartureNode));
               Dep_ptr1 = Dep_ptr1->next;
               Dep_ptr1->Time = newdeptime;
               Dep_ptr1->next = NULL;
               continue;
              }


            }


            
          ptr1->next = ptr2->next;
          //free(ptr2->subtasks);
          free (ptr2); // Pop arrival node
          if (ptr1->next == NULL){break;}
          continue;         
          }         
         }
        
        ptr1 = ptr1->next;
        if (ptr1->next == NULL){break;}
     //   Indicator++;
        
      }





 

  //  printf("Time update\n");
/*-----------------------------Update time and the current arrival node--------------*/
  
  if (Arrival_head.next != NULL) 
  { 
    if (CurrentNode->next != NULL)
  {
    while ((CurrentNode->next)->Time <= time) {CurrentNode = CurrentNode->next;}
  }
  } // update current node    

 // printf("Updated current node\n");

  if (Departure_head.next == NULL) 
  {
    CurrentNode=CurrentNode->next;
    time = CurrentNode->Time;
  //  printf(" Time = %d\n", time);
    continue;
  }

  //printf("Update current node/time\n");

  if (  ((Departure_head.next)->Time) < ((CurrentNode->next)->Time) )
  {
    time = (Departure_head.next)->Time;
  }
      // update time
  else 
  {
    CurrentNode=CurrentNode->next;
    time = CurrentNode->Time;
  }


//printf(" \nTime = %d\n\n", time);


    } // end of WHILE

 
  
  double AverageUtil = 0;
  //double AverageLoadFactor = 0;
  double AverageQueLength = 0;
  double AverageWatitingTime1 = 0;
  double AverageWatitingTime0 = 0;

   AverageUtil = 1 - (CumulServer / (64*(time - simtime)));
   //AverageLoadFactor = Average_Load_Balance / (((double) );
   AverageQueLength = (Cumulative_wait_time0 + Cumulative_wait_time1)/(time - simtime);
   AverageWatitingTime1 = Cumulative_wait_time1/( (double) ((NumberofLines-1) / 2.0) );
   AverageWatitingTime0 = Cumulative_wait_time0/( (double) ((NumberofLines-1) / 2.0) );

   printf("\n\n\n\n");
   printf("avgWaitTime0: %lf\n avgWaitTime1: %lf\n avgQLen: %lf\n avgCPUUtil: %lf\n avgBalance: %lf\n", AverageWatitingTime0, AverageWatitingTime1, AverageQueLength, AverageUtil, Average_Load_Balance);




 





if (argc == 2) 
{
 fclose(fp2);
}





//printf("THE Average load balance is \n%lf\n", Average_Load_Balance);
  return 0;
}
