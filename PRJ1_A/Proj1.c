
# Noah Smith smit1988
# Vikram Manja vmanja





#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int GetTime(float LamOrServ);

int GetTime(float LamOrServ)
{
  double X = (double)rand()/ RAND_MAX; // Generates X value between 0 and 1
  int R0 = ceil(-(1/LamOrServ) * log(1-X));  // Generates IA time R value
  return R0;
}

int main(int argc, char ** argv)
{
  int TasksRemain = 0;
  int NumberofLines = 2;
  int TasksRemain0 = 2;
  int TasksRemain1 = 2;
  float Lambda0, Lambda1, AvgServiceTime;
  FILE * fp2;
  char buffer[1024];
  fp2 = NULL;
  Lambda0 = 0;
  Lambda1 = 0;
  AvgServiceTime = 0;

  if (argc > 2)  // MODE 1
    {
      Lambda0 = atof(argv[1]);
      Lambda1 = atof(argv[2]);
      AvgServiceTime = atof(argv[3]);

      TasksRemain = atoi(argv[4]);
      TasksRemain0 = TasksRemain;
      TasksRemain1 = TasksRemain;
    }

  if (argc ==2) 
    {
      fp2 = fopen(argv[1], "r");

      while (feof(fp2) == 0)
	{
	  fgets(buffer,1023,fp2);
	  TasksRemain++;
	}
      TasksRemain--;
      fseek(fp2, 0, SEEK_SET); // fp2 @ beginning of file
      NumberofLines = TasksRemain;
    }
  int Service_Array[NumberofLines];
  int Arrival_Counter;
  Arrival_Counter = 0;
  Service_Array[0] = 0;

  FILE * fp;
  int Server = 0, Queue_0 = 0, Queue_1 = 0, i,j, k, r = 0, NumberOfTimes, a =0;
      
  int QueueLength = 0;
  double swap1 = 0, swap2 = 0;
  double time = 0, start_time = 0, ServiceTimeStart = 0, AverageCPU = 0;
  double CumulWaitingTime1 = 0, CumulWaitingTime0 = 0;
  double Previous_T = 0, Previous_Queue0 = 0, Previous_Queue1 = 0;
  //FEL take numberoftasks * 5
  //FEL will be int array with 2 arguments: event (0 arrival,1 arrival,-1 departure, 
  //2 executed) and time
  double FEL[TasksRemain * 2][2];
     
  double Current_FEL[4][2]; //array of all the current time events
 
  for(i = 0; i <= TasksRemain * 2 -1; i++)
    {       
      FEL[i][0] = 2;
      FEL[i][1] = -1;
    }

  if (argc ==2)
    {
      for(i = 0; i < NumberofLines; i++)
        {
          fscanf(fp2,"%lf %lf %d", &FEL[i][1], &FEL[i][0], &Service_Array[i]);
          
	  if (i != 0) 
            {
	      if (FEL[i][1] == FEL[i-1][1])  // if times are equal  
		{
		  if (FEL[i][0] == 0)         // if lower event is priority 0
		    {
		      a = Service_Array[i];
		      Service_Array[i] = Service_Array[i-1];
		      Service_Array[i-1] = a;
		    } // switch service times 
		}
            }
        }
      time = FEL[0][1];//Set first time
      Previous_T = time;
      fseek(fp2, 0, SEEK_SET);
    }

  if (argc > 2)
    {
      FEL[0][0] = 0; // INITIAL CONDITIONS
      FEL[0][1] = 0;
      FEL[1][0] = 1;
      FEL[1][1] = 0;

      TasksRemain1--;
      TasksRemain0--; // Decrementing # arrivals
    }

  start_time = time;
  ServiceTimeStart = time;
  while( ( (TasksRemain0 > 0) || (TasksRemain1 > 0) ) && (NumberofLines > 0) ) // Need condition for mode2
    {
      //Check for all of the current time events
      NumberOfTimes = 0;//Current number of time events
      for(i = 0; i <= TasksRemain * 2 - 1; i++)
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
			      if(argc > 2)
				{
				  FEL[r][1] = time + GetTime(AvgServiceTime);
				}          
			      if (argc ==2)
				{
				  FEL[r][1] = time + Service_Array[Arrival_Counter];
				  Arrival_Counter++;
				}
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

			      if(argc > 2)
				{
				  FEL[r][1] = time + GetTime(AvgServiceTime);
				}
                
			      if(argc ==2)
				{
				  FEL[r][1] = time + Service_Array[Arrival_Counter];
				  Arrival_Counter++;
				}
			      FEL[r][0] = -1;
			    }
			  else
			    {
			      ServiceTimeStart = time;
			      Server = 0;
			    }
			  if(argc == 2)
			    {
			      NumberofLines--;
			    }
			}
		      else if(i == 0) // PRIORITY 0 ARRIVAL
			{
			  if(Server ==1)
			    {
			      Queue_0++;
			    }
			  else
			    {
			      AverageCPU += (time - ServiceTimeStart);
			      Server = 1;
			      r = 0;
			      while (FEL[r][1] >= time)
				{
				  r++;
				}
			      if(argc > 2)
				{
				  FEL[r][1] = time + GetTime(AvgServiceTime);
				}
			      if(argc ==2)
				{
				  FEL[r][1] = time + Service_Array[Arrival_Counter];
				  Arrival_Counter++;
				}
			      FEL[r][0] = -1;
			    }
			  if ( (TasksRemain0 > 0)  && (argc > 2) ) 
			    {
			      r= 0;
			      while (FEL[r][1] >= time)
				{
				  r++;
				}
			      FEL[r][1] = time + GetTime(Lambda0);
			      FEL[r][0] = 0;
			      TasksRemain0--;
			    }
			  QueueLength += Queue_1 + Queue_0;
			}
		      else
			{
			  if (Server ==1)
			    {
			      Queue_1++;
			    }
			  else
			    {
			      AverageCPU += (time - ServiceTimeStart);
			      Server = 1;
			      r = 0;
			      while(FEL[r][1] >= time)
				{
				  r++;
				}
			      if (argc > 2)
				{
				  FEL[r][1] = time + GetTime(AvgServiceTime);
				}
          
			      if (argc ==2)
				{
				  FEL[r][1] = time + Service_Array[Arrival_Counter];
				  Arrival_Counter++;
				}

			      FEL[r][0] = -1;
			    }
    
			  if ( (TasksRemain1 > 0) && ( argc > 2) )
			    {
			      r = 0;
			      while (FEL[r][1] >= time)
				{
				  r++;
				}
			      FEL[r][1] = time + GetTime(Lambda1);
			      FEL[r][0] = 1;
			      TasksRemain1--;
			    }
			  QueueLength += Queue_1 + Queue_0;
			}
		    }
		}
	    }
	}

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
      while (FEL[r][1] <= time)
	{
	  r++;
	  if(r == (TasksRemain * 2 - 1))
	    break;
	}
      //last value
      time = FEL[r][1];
    }

  // CUMULATIVE STATISTICS

  //QueueLength = QueueLength0 + QueueLength1;

  double AverageQueLength =0;
  double AverageWatitingTime0 = 0;
  double AverageWatitingTime1 = 0;

  if(argc == 2)
    TasksRemain /= 2;
  AverageQueLength = (CumulWaitingTime1 + CumulWaitingTime0)/(time - start_time);
  //AverageQueLength = QueueLength / time;//((double)TasksRemain * 2.0);
  //declare cumulwaiting
  AverageWatitingTime1 = CumulWaitingTime1/((double)TasksRemain);
  AverageWatitingTime0 = CumulWaitingTime0/((double)TasksRemain);
  AverageCPU = ((time - start_time) - AverageCPU) / (time - start_time);

  fp = fopen("proj1-a_output", "w");
  fprintf(fp, "%f\n%f\n%f\n%f",AverageWatitingTime0,AverageWatitingTime1,AverageQueLength,AverageCPU);
  fclose(fp);

    
  if (argc ==2)
    {
      fclose(fp2);
    }


  return 0;

}