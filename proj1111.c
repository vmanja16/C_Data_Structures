#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double GetServiceTime(float avgservicetime);
double GetPriorityOneTime(float lambda1);
double GetPriorityZeroTime(float lamda0);


     
double GetPriorityZeroTime(float lambda0)
{
  double X = (double)rand()/ RAND_MAX; // Generates X value between 0 and 1
  double R0 = -1/lambda0 * log(1-X);  // Generates IA time R value
  double IAT0 = (lambda0 * exp(-lambda0 *R0) ); 
  return IAT0;
}

double GetPriorityOneTime(float lambda1) 
{
  double X = (double)rand()/ RAND_MAX; // Generates X value between 0 and 1
  double R1 = -1/lambda1 * log(1-X);  // Generates IA time R value
  double IAT1 = (lambda1 * exp(-lambda1 *R1) ); 
  return IAT1;
}

double GetServiceTime(float avgservicetime)
{
  double X = (double)rand()/ RAND_MAX; // Generates X value between 0 and 1
  double RS= -1/avgservicetime * log(1-X);  // Generates Service time R value
  double ST = (avgservicetime * exp(-avgservicetime *RS) );
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

      int Server = 0, Queue_0 = 0, Queue_1 = 0, i,j, k, r = 0, NumberOfTimes;
      int TasksRemain = atoi(argv[4]);
      int TasksRemain0 = TasksRemain;
      int TasksRemain1 = TasksRemain;
      int QueueLength = 0, QueueLength0 = 0, QueueLength1 = 0;
      double swap1 = 0, swap2 = 0;
      double time = 0, ServiceTimeStart = 0, AverageCPU = 0;
      double CumulWaitingTime1 = 0, CumulWaitingTime0 = 0;
      double Previous_T = 0, Previous_Queue0 = 0, Previous_Queue1 = 0;
      //FEL take numberoftasks * 5
      //FEL will be int array with 2 arguments: event (0 arrival,1 arrival,-1 departure, 
      //2 executed) and time
      double FEL[TasksRemain * 2][2];
      double Current_FEL[4][2]; //array of all the current time events
 
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

     TasksRemain1--;
     TasksRemain0--; // Decrementing # arrivals

      while( (TasksRemain0 > 0) || (TasksRemain1 > 0) )
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

	  /*
	  if ((Server == 0) && (NumberOfTimes == 0))
	    {
	      NoServiceTime++;
	    }
	  */

	  if(NumberOfTimes != 0)//probably unnecessary
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
			      	{
				  Queue_0--;
				  r = 0;
				  while (FEL[r][1] >= time)
				    {
				      r++;
				    }
				  FEL[r][1] = time + GetServiceTime(AvgServiceTime);
				  FEL[r][0] = -1;
			        }

			      else if(Queue_1  > 0)
			      	{
				  Queue_1--;
				  r = 0;
				  while (FEL[r][1] >= time)
				    {
				      r++;
				    }
				  FEL[r][1] = time + GetServiceTime(AvgServiceTime);
				  FEL[r][0] = -1;
			        }

			        else
				  {
				    ServiceTimeStart = time;
				    Server = 0;
				  }
			    }
			  else if(i == 0)
			    {
			      if (Server ==1)
				{
				  Queue_0++;
				}
			      else 
				{
				  AverageCPU += time - ServiceTimeStart;
				  Server = 1;
				  r = 0;
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
				  while (FEL[r][1] >= time)
				    {
				      r++;
				    }
				  FEL[r][1] = time + GetPriorityZeroTime(Lambda0);
				  FEL[r][0] = 0;
				  TasksRemain0--;
			      	}
			      QueueLength0 = QueueLength0 + Queue_0;
			    }
			  else
			    {
			      if (Server ==1)
				{
				  Queue_1++;
				}
			      else
				{ 
				  AverageCPU += time - ServiceTimeStart;
				  Server = 1;
				  r = 0;
				  while (FEL[r][1] >= time)
				    {
				      r++;
				    }
				  FEL[r][1] = time + GetServiceTime(AvgServiceTime);
				  FEL[r][0] = -1;
			        }
		
			      if (TasksRemain1 > 0)
			      	{
				  r = 0;
				  printf("Line 201\n");
				  while (FEL[r][1] >= time)
				    {
				      r++;
				    }
				  FEL[r][1] = time + GetPriorityOneTime(Lambda1);
				  FEL[r][0] = 1;
				  TasksRemain1--;
			        }
			      QueueLength1 = QueueLength1 + Queue_1;
			    }
			}
		    }
		}
	    }


	  //double Previous_T = 0, Previous_Queue0 = 0, Previous_Queue1 = 0;
	  CumulWaitingTime0 += (time - Previous_T) * Previous_Queue0;
	  CumulWaitingTime1 += (time - Previous_T) * Previous_Queue1;
	  Previous_Queue0 = Queue_0;
          Previous_Queue1 = Queue_1;
	  Previous_T = time;
	  
	  //Sort FEL
	  for (i = 0; i < (TasksRemain * 2); i++)
	    {
	      for (j = i + 1; j < (TasksRemain * 2); j++)
		{
		  if (FEL[i][1] > FEL[j][1])
		    {
		      swap1 =  FEL[i][1];
		      swap2 = FEL[i][0];
		      FEL[i][1] = FEL[j][1];
		      FEL[i][0] = FEL[j][0];
		      FEL[j][1] = swap1;
		      FEL[j][0] = swap2;
		    }
		}
	    }
	  //There will be garbage used times
	  //First credible entry is FEL[r][1] > time
	  //FEL[r][1] is the next time the loop will go to, i.e. time=FEL[r][1]
	  r = 0;
	  while (FEL[r][1] < time)
	    {
	      r++;
	      if(r == (NumberOfTasks * 2 - 1))
		break;
	    }
	  //last value
	  time = FEL[r][1];
	  printf("%f\n",time);

	  /*
	    QueLength = QueLength + Queue_0 + Queue_1; //Accumulating Que Length
	    QueLength0 = QueLength0 + Queue_0;
	    QueLength1 = QueLength1 + Queue_1;
	  */
	}

	// CUMULATIVE STATISTICS

      QueueLength = QueueLength0 + QueueLength1;

      double AverageQueLength =0;
      double AverageWatitingTime0 = 0;
      double AverageWatitingTime1 = 0;

      AverageQueLength = QueueLength / ((double)NumberOfTasks * 2.0);
      //declare cumulwaiting
      AverageWatitingTime1 = CumulWaitingTime1/((double)NumberOfTasks);
      AverageWatitingTime0 = CumulWaitingTime0/((double)NumberOfTasks);
      AverageCPU = (time - AverageCPU) / time;

      printf("%f\t%f\t%f\t%f\t\n", AverageQueLength, AverageWatitingTime0, AverageWatitingTime1, AverageCPU);

    }
  return 0;

}
