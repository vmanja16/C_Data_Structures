#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

 int GetServiceTime(float avgservicetime);
 int GetPriorityOneTime(float lambda1); 
 int GetPriorityZeroTime(float lamda0);


     
int GetPriorityZeroTime(float lambda0)
{ 
float X = (float)rand()/ RAND_MAX; // Generates X value between 0 and 1
float R0 = -1/lambda0 * log(1-X);  // Generates IA time R value
int IAT0 = ceil(lambda0 * exp(-lambda0 *R0) ); 
return IAT0;
}

int GetPriorityOneTime(float lambda1) 
{
float X = (float)rand()/ RAND_MAX; // Generates X value between 0 and 1
float R1 = -1/lambda1 * log(1-X);  // Generates IA time R value
int IAT1 = ceil(lambda1 * exp(-lambda1 *R1) ); 
return IAT1;
}

int GetServiceTime(float avgservicetime)
{
float X = (float)rand()/ RAND_MAX; // Generates X value between 0 and 1
float RS= -1/avgservicetime * log(1-X);  // Generates Service time R value
int   ST = ceil(avgservicetime * exp(-avgservicetime *RS) );
return ST;
} 


int main(int argc, char ** argv)

{
  
  if (argc > 2)  // MODE 1
    {
      float Lambda0, Lambda1, AvgServiceTime, NumberOfTasks;

      Lambda0 = atof(argv[1]);
      Lambda1 = atof(argv[2]);
      AvgServiceTime = atof(argv[3]);
      NumberOfTasks = atof(argv[4]);

      int Server = 0, Queue_0 = 0, Queue_1 = 0, time = 0, i, k, r, c,NumberOfTimes;
      int NoServiceTime=0;
      int TasksRemain = atoi(argv[4]);
      int TasksRemain0 = TasksRemain;
      int TaskRemain1 = TasksRemain;
      int QueLength = 0, QueLength0 = 0, QueLength1 = 0;
      //FEL take numberoftasks * 5
      //FEL will be int array with 2 arguments: event (0 arrival,1 arrival,-1 departure, 
      //2 executed) and time
      int FEL[TasksRemain * 2][2];
      int Current_FEL[4][2]; //array of all the current time events
 
      for(i = 0; i <= NumberOfTasks * 2 -1; i++)
      {
      	FEL[i][0] = 2;
      	FEL[i][1] = -1;
      	//Current_FEL[i][0] = 2;
      	//Current_FEL[i][1] = -1;
      }



     FEL[0][0] = 0; // INITIAL CONDITIONS
     FEL[0][1] = 0;
     FEL[1][0] = 1;
     FEL[1][1] = 0;

     TaskRemain1--;
     TasksRemain0--; // Decrementing # arrivals

      while( (TasksRemain0 > 0) || (TaskRemain1 > 0) )
	{
	  //Check for all of the current time events
	  NumberOfTimes = 0;//Current number of time events
	  for(i = 0; i <= NumberOfTasks * 2 - 1; i++)
	    {
	      if(FEL[i][1] == time)
		{
		  //Copy one line of the FEL to the Current_FEL
		  Current_FEL[NumberOfTimes][0] = FEL[i][0];
		  Current_FEL[NumberOfTimes][1] = FEL[i][1];
		  NumberOfTimes++;
		}
	    }
	  //At this point Current_FEL contains all the time events, there are NumberOfTimes

	  if ((Server == 0) && (NumberOfTimes == 0)) {NoServiceTime++;}

	  if(NumberOfTimes != 0)
	    {
	      //Departures > priorities > nonpriorities
	      //After an event, change Current_FEL[x][0] to 2
	      for(i = -1; i <= 1; i++)//Look in order
		{
		  for(k = 0; k < NumberOfTimes; k++)//Go through all looking for i
		    {
		      if(Current_FEL[k][0] == i)
			{
			  //Do the event corresponding to i
			  if(i == -1)
			    {
			      if(Queue_0  > 0)
			      	{Queue_0--;
			      			while (FEL[r][1] >= time)
			      		{
			      		r++;
			      		}
			      		FEL[r][1] = time + GetServiceTime(AvgServiceTime);
			      		FEL[r][0] = -1;
			        }

			      else if(Queue_1  > 0)
			      	{Queue_1--;
			      			while (FEL[r][1] >= time)
			      		{
			      		r++;
			      		}
			      		FEL[r][1] = time + GetServiceTime(AvgServiceTime);
			      		FEL[r][0] = -1;
			        }

			        else
			        {
			        	NoServiceTime++;
			        	Server = 0;
			        }

			    }
			  else if(i == 0)
			    {
			      if (Server ==1)
			      {Queue_0++;}

			      else 
			      { 
			      	Server = 1;
			      	while (FEL[r][1] >= time)
			      		{
			      		r++;
			      		}
			      		FEL[r][1] = time + GetServiceTime(AvgServiceTime);
			      		FEL[r][0] = -1;
			        }
			    	

			      	if (TasksRemain0 > 0) 
			      	{ 
			      		r= 0;
			      		while (FEL[r][1] >= time)	{r++;}
			      		FEL[r][1] = time + GetPriorityZeroTime(Lambda0);
			      		FEL[r][0] = 0; 
			      		TasksRemain0--;
			      	}

			    }
			}
			   else
			     {
			        if (Server ==1) {Queue_1++; }

			        else { 
			      		Server = 1;
			      		while (FEL[r][1] >= time) {r++;}
			      		FEL[r][1] = time + GetServiceTime(AvgServiceTime);
			      		FEL[r][0] = -1;
			        }
		
			      	

			      	if (TasksRemain1 > 0)
			      	{
			      		r = 0;
			      		while (FEL[r][1] >= time) 	{r++;}
			      		FEL[r][1] = time + GetPriorityOneTime(Lambda1);
			      		FEL[r][0] = 1; 
			        	TaskRemain1--;
			        }

			     }
			}
		    }
	
	    QueLength = QueLength + Queue_0 + Queue_1; //Accumulating Que Length
	    QueLength0 = QueLength0 + Queue_0;
	    QueLength1 = QueLength1 + Queue_1;
	    time++; 
	}

	// CUMULATIVE STATISTICS

	int AverageQueLength =0;
	int AverageWatitingTime0 = 0;
	int AverageWatitingTime1 = 0;
	int AverageCPU = 0;

	AverageQueLength = QueLength / time ;
	AverageWatitingTime1 = QueLength1/(NumberOfTasks);
	AverageWatitingTime0 = QueLength0/(NumberOfTasks);
	AverageCPU = (time - NoServiceTime) / time;

	printf("%d\t%d\t%d\t%d\t", AverageQueLength, AverageWatitingTime0, AverageWatitingTime1, AverageCPU);

    }
  return 0;

}