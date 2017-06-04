/* 
  dimention:2
  x-range:[0,1]
  y-range:[0,1]
  objection:(0.835,0.714)

  fitness fuction: d=sqrt((x1-0.835)^2+(x2-0.714)^2) //Euclid distance
  threshold condition: d(pbest1,pbest2)<=0.001

  weight:50%
  individual constant:25%
  social constant:25%

  initial velocity:(random v1,random v2) //v1,v2 between 0 to 1
  initial location:(random x1,random x2) //x1,x2 between 0 to 1
  initial pbest= initial location
  initial gbest= (0,0)
  
*/


#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include"math.h"
#include"mpi.h"

#define dim 2 //dimention
#define num 10 //granule numerble
#define thresh 0.0001 //break thresh

#define weight 0.5
#define c1 0.25
#define c2 0.25

//need to fit the dimention...........
float distance(float x1,float x2);  //fitness fuction



//god process to figure out gbest and broadcast to others



void main(int argc,char *argv[])
{
  //srand((unsigned)time(NULL));
  int myrank; 
  
  
  MPI_Status status;

  MPI_Init(&argc,&argv);
  //srand((unsigned)time(NULL));
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);


  //rank initial
  float g_loc[dim]={0.0}; //group location
  float gbest[dim]={0.0};  // global group best
  float pbest[dim]={0.0};  //used best
  float iloc[dim];         // my location
  float ivel[dim];         //my velocity
       
  float message[dim+1]; //broad message:now location + now rank

  
      
  
      
  //initial location and velocity

  srand(getpid()); //random seed

  for (int i = 0; i <dim; ++i)
  
      iloc[i]=rand()/(RAND_MAX+1.0);  //0~1 random location
         
       
       
  for (int i = 0; i < dim; ++i)
       
      ivel[i]=rand()/(RAND_MAX+1.0); //0~1 random velocity
       
  //start iteration
  
  while(distance(gbest[0],gbest[1])>=thresh)
  {
    printf("gbest[0],gbest[1]:%f,%f,%d\n", gbest[0],gbest[1],myrank);
    

    if(myrank!=0)
    {
      //UPDATE PBEST

      if(distance(iloc[0],iloc[1])<distance(pbest[0],pbest[1]))
      {
        for (int i = 0; i < dim; ++i)
        
          pbest[i]=iloc[i];
        
          
      }
      
      
      
      
      for (int i = 0; i < dim; ++i)
      
           message[i]=pbest[i];   
      
     
      message[dim]=(float)myrank;
      printf("send:location:%f,%f,rank:%f\n",message[0],message[1],message[2]);
        
      
        //send to god process
      MPI_Send(&message,dim+1,MPI_FLOAT,0,myrank,MPI_COMM_WORLD);  
    
      
      
      
      //update
      MPI_Recv(message,dim+1, MPI_FLOAT, 0, myrank, MPI_COMM_WORLD, &status);
      printf("Recved:%f,%f\n",message[0],message[1] );
     
      
      for (int i = 0; i < dim; ++i)
         gbest[i]=message[i];
   
      
      //update velocity
      for (int v = 0; v < dim; ++v)
      
        ivel[v]=weight*ivel[v]+c1*rand()/(RAND_MAX+1.0)*(pbest[v]-iloc[v])+c2*rand()/(RAND_MAX+1.0)*(gbest[v]-iloc[v]);
      
      
      
      //update location
      for (int l = 0; l < dim; ++l)
      {
        iloc[l]=iloc[l]+ivel[l];
        printf("new iloc:%f\n", iloc[l]);
      }

      
    }  
      
  
    
  

    


    if(myrank==0)
    {
        
        printf("God! \n" );
    
        int now_rank=0;
        int best_rank=0;
    
        float gd=999;
        
    
       
        
              
        for (int i = 0; i < num; ++i)
        {
          //sleep(1);
          MPI_Recv(&message, dim+1, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
          printf(" GOD Recv:location:%f,%f,rank:%f\n",message[0],message[1],message[2]);
              
          now_rank=(int)message[2];
             
          
          for(int i=0;i<dim;i++)
          { 
              g_loc[i]=message[i];//save location
          }
        
          float d=distance(g_loc[0],g_loc[1]);
          if(d<gd)
          {
              gd=d;
              best_rank=now_rank;
              gbest[0]=g_loc[0];
              gbest[1]=g_loc[1];
              printf("now best_rank:%d\n",best_rank );
          }
        }

        if(best_rank!=0)
        {
      
           for (int k = 0; k < dim; ++k)
           {
              message[k]=gbest[k];  
              printf("god send:%f\n",message[k] ); 
           }
            message[dim]=(float)best_rank;
            
            for (int i = 1; i <= num; ++i)
            
              MPI_Send(message,3,MPI_FLOAT,i,i,MPI_COMM_WORLD);
            
        }
            
        else
            printf("error best rank");
     }
  }
  
  printf("ilocation1:%f,ilocation2:%f",gbest[0],gbest[1]);
  MPI_Finalize();

}

float distance(float x1,float x2)
{
  float nowDistance;
  nowDistance=sqrt((x1-0.835)*(x1-0.835)+(x2-0.714)*(x2-0.714));
  return nowDistance;
}

