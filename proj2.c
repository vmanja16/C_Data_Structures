// Noah Smith smit1988
// Vikram Manja vmanja

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

int main (int argc, char ** argv)
{

//printf("time: %lf\n",time);

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


   // DECLARATIONS
   
  int Service_Array[NumberofLines];
  int Arrival_Counter;
  Arrival_Counter = 0;
  Service_Array[0] = 0;

  FILE * fp;
  int Server = 64, Queue_0 = 0, Queue_1 = 0, i,j, k, r = 0, NumberOfTimes, a =0, b =0, c =0;
  int FEL_real_size = 0;
      
  int QueueLength = 0;
  double swap1 = 0, swap2 = 0, swap3 =0;
  double time = 0, start_time = 0, ServiceTimeStart = 0, AverageCPU = 0;
  double CumulWaitingTime1 = 0, CumulWaitingTime0 = 0;
  double Previous_T = 0, Previous_Queue0 = 0, Previous_Queue1 = 0;
  double Mu_Min = (AvgServiceTime * 3), Mu_Max = 0, Load_Bal_Fac = 0;
  double Cumul_Load_Bal = 0, Previous_Server = 0, CumulServer = 0;

  int FEL_size = (TasksRemain * 2) + 68;
  double FEL[FEL_size][2]; // Future events list
  double Current_FEL[68][2];  // Current Events list
  double Queue_List [FEL_size][3]; // Current Queue

  // INITIALIZING FEL  and QUEUE_LIST
  for(i = 0; i < (FEL_size); i++)
    {      
      FEL[i][0] = 2;
      FEL[i][1] = -1;
      Queue_List[i][0] = 2;
      Queue_List[i][1] = -1;
      Queue_List[i][2] = 65;
    }

  // INITIALLIZING FEL
  if(argc > 2)
    {
      FEL[FEL_size - 1][0] = 0;
      FEL[FEL_size - 1][1] = 0;
      FEL[FEL_size - 2][0] = 1;
      FEL[FEL_size - 2][1] = 0;

      FEL_real_size = 2;

      TasksRemain1--;
      TasksRemain0--; // Decrementing # arrivals
    }

  if ( argc == 2)        INITIALLIZING FEL;
  {
  	for(i = NumberofLines; i  > 0; i--)        //have to initialize FEL_real_size
  	{
	  FEL[i][1] = atoi(fgetc(fp2));   //time
	  FEL[i][0] = atoi(fgetc(fp2));   //Priority
	  FEL[i][2] = atoi(fgetc(fp2));   //Subtasks
	  fgets(buffer,1023,fp2);     // move to next line
	    	 	
  	}
  	fseek(fp2, 0, SEEK_SET);
  }


  start_time = time;
  ServiceTimeStart = time;

  //BEGINNING OF WHILE LOOP
  while( ( (TasksRemain0 > 0) || (TasksRemain1 > 0) ) && (NumberofLines > 0) ) // Need condition for mode2
    {


      //Check for all of the current time events
      NumberOfTimes = 0;//Current number of time events
      for(i = 0; i < FEL_real_size; i++)
	{
	  if(FEL[FEL_size - 1 - i][1] == time)
	    {
	      //Copy one line of the FEL to the Current_FEL
	      Current_FEL[NumberOfTimes][0] = FEL[FEL_size - 1 - i][0];
	      Current_FEL[NumberOfTimes][1] = FEL[FEL_size - 1 - i][1];
	      NumberOfTimes++;
	    }
	}


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
			  Server++;
			  /*
			  r = FEL_size - 1;
			  while (FEL[r][1] >= time)
			    {
			      r--;
			    }
			  FEL_real_size = r + 2 - FEL_size;
			  r = FEL_size - 1;
			  while(Queue_List[r][1] != -1)
			    {
			      r--;
			    }
			  if(FEL_real_size < (r + 2 - FEL_size))
			    FEL_real_size = r + 2 - FEL_size;
			  */

			  for (b = 0; b < (FEL_real_size); b++)  // SORTING QUEUE_LIST by time
			    {
			      for (c = b + 1; c < (FEL_real_size); c++)
				{
				  if (Queue_List[FEL_size - 1 - b][1] < Queue_List[FEL_size - 1 - c][1]) 
				    {
				      swap1 =  Queue_List[FEL_size - 1 - c][1];
				      swap2 = Queue_List[FEL_size - 1 - c][0];
				      swap3 = Queue_List[FEL_size - 1 - c][2];
				      Queue_List[FEL_size - 1 - c][1] = Queue_List[FEL_size - 1 - b][1];
				      Queue_List[FEL_size - 1 - c][0] = Queue_List[FEL_size - 1 - b][0];
				      Queue_List[FEL_size - 1 - c][2] = Queue_List[FEL_size - 1 - b][2];
				      Queue_List[FEL_size - 1 - b][1] = swap1;
				      Queue_List[FEL_size - 1 - b][0] = swap2;
				      Queue_List[FEL_size - 1 - b][2] = swap3;
				    }
				}
			    }

			  c = 0;
			  for(a = FEL_real_size - 1; a >= 0; a--)
			    {
			      if (Queue_List[FEL_size - 1 - a][0] == c ) //if priority c 
				{
				  if (Queue_List[FEL_size - 1 - a][2] <= Server ) // if it can be serviced
				    {
				      Server -= Queue_List[FEL_size - 1 - a][2];
				      if (c ==0)
					{
					  Queue_0--;
					}
				      if (c ==1)
					{
					  Queue_1--;
					}

				      for (b =0; b < Queue_List[FEL_size - 1 - a][2]; b++) // generate departure time for subtasks
					{
					  //x=0;
					  r = FEL_size - 1;
					  while (FEL[r][1] >= time)
					    {
					      r--;
					      //x++;
					    }
					  //x+1 is the new fel real size
					  //x = r - (felsize -1)
					  //FEL_real_size = r + 2 - FEL_size;
					  FEL_real_size += 1;
					  if(argc > 2)
					    {
					      FEL[r][1] = time + GetTime(AvgServiceTime);
					      //Finding Lambda min and mix
					      if(Mu_Min < (FEL[r][1] - time))
						Mu_Min = (FEL[r][1] - time);
					      //Mu_Min = 1/(umin) which is CPU time taken by longest sub-task (backwardsish)
					      if(Mu_Max > (FEL[r][1] - time))
						Mu_Max = (FEL[r][1] - time);
					      Load_Bal_Fac = (Mu_Min - Mu_Max)/AvgServiceTime;
					      FEL[r][0] = -1;
					    }
					}
				      Cumul_Load_Bal += Load_Bal_Fac;
				      Queue_List[FEL_size - 1 - a][0] = 2;
				      Queue_List[FEL_size - 1 - a][1] = -1;
				      Queue_List[FEL_size - 1 - a][2] = 65;
				    }
				}
			      if((c==0) && (a==0))
				{
				  c = 1;
				  a = FEL_real_size;
				}
			    } 				  	
			}
  /*+++++++++++++++++++++++++++++PRIORITY 0 ARRIVAL++++++++++++++++++++++++++++++++++++++++*/     
		      
		      else if(i == 0) 
			{
			  r = FEL_size - 1;
			  while(Queue_List[r][1] != -1)
			    {
			      r--;       //finds garbage in Quelist
			    }
			  FEL_real_size += 1;

			  Queue_List[r][0] = 0;       // Add Priority Arrival 0 to QueueList
			  Queue_List[r][1] = time;
			  if (argc  > 2 ){Queue_List[r][2] = rand() % 32 + 1;}
			  if (argc == 2 ){Queue_List[r][2] = FEL[k][2];}
			  Queue_0++;

			  /*
			  r = FEL_size - 1;
			  while (FEL[r][1] >= time)
			    {
			      r--;
			    }
			  FEL_real_size = r + 2 - FEL_size;
			  r = FEL_size - 1;
			  while(Queue_List[r][1] != -1)
			    {
			      r--;
			    }
			  if(FEL_real_size < (r + 2 - FEL_size))
			    FEL_real_size = r + 2 - FEL_size;
			  */

			  for(b = 0; b < (FEL_real_size); b++)  // SORTING QUEUE_LIST by time
			    {
			      for(c = b + 1; c < (FEL_real_size); c++)
				{
				  if(Queue_List[FEL_size - 1 - b][1] < Queue_List[FEL_size - 1 - c][1]) 
				    {
				      swap1 =  Queue_List[FEL_size - 1 - b][1];
				      swap2 = Queue_List[FEL_size - 1 - b][0];
				      swap3 = Queue_List[FEL_size - 1 - b][2];
				      Queue_List[FEL_size - 1 - b][1] = Queue_List[FEL_size - 1 - c][1];
				      Queue_List[FEL_size - 1 - b][0] = Queue_List[FEL_size - 1 - c][0];
				      Queue_List[FEL_size - 1 - b][2] = Queue_List[FEL_size - 1 - c][2];
				      Queue_List[FEL_size - 1 - c][1] = swap1;
				      Queue_List[FEL_size - 1 - c][0] = swap2;
				      Queue_List[FEL_size - 1 - c][2] = swap3;
				    }
				}
			    }

			  //This segment finds the exact FEL_real_size
			  r = FEL_size - 1;
                          while (FEL[r][1] >= time)
                            {
                              r--;
                            }
                          FEL_real_size = FEL_size - r;
                          r = FEL_size - 1;
                          while(Queue_List[r][1] != -1)
                            {
                              r--;
                            }
                          if(FEL_real_size < (r + 2 - FEL_size))
                            FEL_real_size = FEL_size - r;


			  c = 0;
			  for(a = FEL_real_size - 1; a >= 0; a--)
			    {
			      if(Queue_List[FEL_size - 1 - a][0] == c ) //if priority c 
				{
				  if(Queue_List[FEL_size - 1 - a][2] <= Server ) // if it can be serviced
				    {
				      Server -= Queue_List[FEL_size - 1 - a][2];
				      if(c ==0)
					{
					  Queue_0--;
					}
				      if(c ==1)
					{
					  Queue_1--;
					}
				      for(b =0; b < Queue_List[FEL_size - 1 - a][2]; b++) // generate departure time for subtasks
					{
					  r = FEL_size - 1;
					  while(FEL[r][1] >= time)
					    {
					      r--;
					    }
					  FEL_real_size += 1;
					  if(argc > 2)
					    {
					      FEL[r][1] = time + GetTime(AvgServiceTime);
					      //Finding Lambda min and mix
                                              if(Mu_Min < (FEL[r][1] - time))
                                                Mu_Min = (FEL[r][1] - time);
                                              //Mu_Min = 1/(umin) which is CPU time taken by longest sub-task (backwardsish)
						if(Mu_Max > (FEL[r][1] - time))
						  Mu_Max = (FEL[r][1] - time);
                                              Load_Bal_Fac = (Mu_Min - Mu_Max)/AvgServiceTime;
					      FEL[r][0] = -1;
					    }
					}
                                      Cumul_Load_Bal += Load_Bal_Fac;
				      Queue_List[FEL_size - 1 - a][0] = 2;
				      Queue_List[FEL_size - 1 - a][1] = -1;
				      Queue_List[FEL_size - 1 - a][2] = 65;
				    }
				}
			      if((c==0) && (a==0))
				{
				  c=1;
				  a=FEL_real_size;
				}
			    }

			  r = FEL_size - 1;
			  while (FEL[r][1] >= time)
			    {
			      r--;
			    }
			  FEL_real_size += 1;
			  FEL[r][1] = time + GetTime(Lambda0);
			  FEL[r][0] = 0;
			  TasksRemain0--;
					  	
			} // i == 0


		      else
			{
			  r = FEL_size - 1;
			  while(Queue_List[r][1] != -1)
			    {
			      r--;
			    }
			  FEL_real_size += 1;

			  Queue_List[r][0] = 1;
			  Queue_List[r][1] = time;
			  Queue_List[r][2] = rand() % 32 + 1;
			  Queue_1++;

			  /*
			  r = FEL_size - 1;
			  while (FEL[r][1] >= time)
			    {
			      r--;
			    }
			  FEL_real_size = r + 2 - FEL_size;
			  r = FEL_size - 1;
			  while(Queue_List[r][1] != -1)
			    {
			      r--;
			    }
			  if(FEL_real_size < (r + 2 - FEL_size))
			    FEL_real_size = r + 2 - FEL_size;
			  */

			  for(b = 0; b < (FEL_real_size); b++)  // SORTING QUEUE_LIST by time
			    {
			      for(c = b + 1; c < (FEL_real_size); c++)
				{
				  if(Queue_List[FEL_size - 1 - b][1] < Queue_List[FEL_size - 1 - c][1]) 
				    {
				      swap1 =  Queue_List[FEL_size - 1 - b][1];
				      swap2 = Queue_List[FEL_size - 1 - b][0];
				      swap3 = Queue_List[FEL_size - 1 - b][2];
				      Queue_List[FEL_size - 1 - b][1] = Queue_List[FEL_size - 1 - c][1];
				      Queue_List[FEL_size - 1 - b][0] = Queue_List[FEL_size - 1 - c][0];
				      Queue_List[FEL_size - 1 - b][2] = Queue_List[FEL_size - 1 - c][2];
				      Queue_List[FEL_size - 1 - c][1] = swap1;
				      Queue_List[FEL_size - 1 - c][0] = swap2;
				      Queue_List[FEL_size - 1 - c][2] = swap3;
				    }
				}
			    }

			  r = FEL_size - 1;
                          while (FEL[r][1] >= time)
                            {
                              r--;
                            }
                          FEL_real_size = -r + FEL_size;
                          r = FEL_size - 1;
                         

                          while(Queue_List[r][1] != -1)
                            {
                              r--;
                            }
                          if(FEL_real_size < (FEL_size - r))
                            FEL_real_size = -r + FEL_size;


			  c = 0;
			  for(a = FEL_real_size - 1; a >= 0; a--)
			    {
			      if(Queue_List[FEL_size - 1 - a][0] == c ) //if priority c 
				{
				  if(Queue_List[FEL_size - 1 - a][2] <= Server ) // if it can be serviced
				    {
				      Server -= Queue_List[FEL_size - 1 - a][2];
				      if(c ==0)
					{
					  Queue_0--;
					}
				      if(c ==1)
					{
					  Queue_1--;
					}
				      for(b =0; b < Queue_List[FEL_size - 1 - a][2]; b++) // generate departure time for subtasks
					{
					  r = FEL_size - 1;
					  while(FEL[r][1] >= time)
					    {
					      r--;
					    }
					  FEL_real_size += 1;
					  if(argc > 2)
					    {
					      FEL[r][1] = time + GetTime(AvgServiceTime);
                                              //Finding Lambda min and mix
                                              if(Mu_Min < (FEL[r][1] - time))
                                                Mu_Min = (FEL[r][1] - time);
                                              //Mu_Min = 1/(umin) which is CPU time taken by longest sub-task (backwardsish)
					      if(Mu_Max > (FEL[r][1] - time))
						Mu_Max = (FEL[r][1] - time);
                                              Load_Bal_Fac = (Mu_Min - Mu_Max)/AvgServiceTime;
					      FEL[r][0] = -1;
					    }
					}
                                      Cumul_Load_Bal += Load_Bal_Fac;
				      Queue_List[FEL_size - 1 - a][0] = 2;
				      Queue_List[FEL_size - 1 - a][1] = -1;
				      Queue_List[FEL_size - 1 - a][2] = 65;
				    }
				}
			      if((c==0) && (a==0))
				{
				  c=1;
				  a=FEL_real_size;
				}
			    }

			  r = FEL_size - 1;
			  while (FEL[r][1] >= time)
			    {
			      r--;
			    }
			  FEL_real_size += 1;
		
			  FEL[r][1] = time + GetTime(Lambda1);
			  FEL[r][0] = 1;
			  TasksRemain1--;
					  	
			} // else (i == 1)
		    } 
		} 
	    }  
	}     


      //update FEL_real_size to number of non garbage in Queue_List or FEL whichever is larger
      //if there was a departure, Queue_List only decreased and FEL_real_size has already been updated
      /*
      r = FEL_size - 1;
      while (FEL[r][1] >= time)
	{
	  r--;
	}
      FEL_real_size = r + 2 - FEL_size;
      r = FEL_size - 1;
      while(Queue_List[r][1] != -1)
	{
	  r--;
	}
      if(FEL_real_size < (r + 2 - FEL_size))
	FEL_real_size = r + 2 - FEL_size;
      */

      CumulWaitingTime0 += (time - Previous_T) * Previous_Queue0;
      CumulWaitingTime1 += (time - Previous_T) * Previous_Queue1;
      Previous_Queue0 = Queue_0;
      Previous_Queue1 = Queue_1;
      //Add up FREE server time
      CumulServer += (time - Previous_T) * Previous_Server;
      Previous_Server = Server;
      Previous_T = time;

      //Sort FEL
      for (i = 0; i < (FEL_real_size); i++)
	{
	  for (j = i + 1; j < (FEL_size); j++)
	    {
	      if (FEL[FEL_size - 1 - i][1] < FEL[FEL_size - 1 - j][1])
	   	 {
		   swap1 =  FEL[FEL_size - 1 - i][1];
		   swap2 = FEL[FEL_size - 1 - i][0];
		   FEL[FEL_size - 1 - i][1] = FEL[FEL_size - 1 - j][1];
		   FEL[FEL_size - 1 - i][0] = FEL[FEL_size - 1 - j][0];
		   FEL[FEL_size - 1 - j][1] = swap1;
		   FEL[FEL_size - 1 - j][0] = swap2;
		 }
	    }
	}

      r = FEL_size - 4 - FEL_real_size;
      while(FEL[r][1] <= time)
	{
	  r++;
	  if(r == FEL_size - 1)
	    break;
	}
      //last value
      time = FEL[r][1];


    //  printf("time: %lf\n",time);
    } //END of while loop

  double AverageQueLength =0;
  double AverageLoadFactor = 0;
  double AverageUtil = 0;
  double AverageWatitingTime1 = 0;
  double AverageWatitingTime0 = 0;

  AverageUtil = 1 - (CumulServer / (64*(time - start_time)));
  AverageLoadFactor = Cumul_Load_Bal / (((double)TasksRemain) * 2);
  AverageQueLength = (CumulWaitingTime1 + CumulWaitingTime0)/(time - start_time);
  AverageWatitingTime1 = CumulWaitingTime1/((double)TasksRemain);
  AverageWatitingTime0 = CumulWaitingTime0/((double)TasksRemain);

  printf("AverageUtil: AverageLoadFactor: AverageQueLength: AverageWatitingTime0: AverageWatitingTime1:\n");
  printf("%lf       %lf             %lf            %lf                %lf\n",AverageUtil, AverageLoadFactor, AverageQueLength, AverageWatitingTime0, AverageWatitingTime1);
  
  if (argc ==2)
    {
      fclose(fp2);
    }


  return 0;
}     // End of main