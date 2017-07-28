//ABC for TSP
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<iostream>  
#include<math.h>
#include<algorithm>
#include<fstream>
using namespace std;

#define Nc 52
#define T 500
#define no_change_count_limit 5
#define part_no 4
#define part_size 13
#define B 30

struct City
{
int x;
int y;
};
City city[Nc];

struct Emp_Bee
{
int position[Nc];
double tourlength;
int no_change_count;
};
Emp_Bee empbee[B];

struct Onlkr_Bee
{
int global_best_position[Nc];
double global_best_tourlength;
};
Onlkr_Bee obee;

Emp_Bee tempos[part_no];

double city_dist[Nc][Nc];

double dist(int, int);
double dist(int i, int j)
{
  double distance = sqrt((city[i].x - city[j].x)*(city[i].x - city[j].x)+(city[i].y - city[j].y)*(city[i].y - city[j].y));
  return distance;
}

double cal_tour_length(int);
double cal_tour_length(int j)
{
      double t; 
      double sumt = 0.0;
      for(int i = 0; i<Nc-1; i++)
      {
      t = dist(empbee[j].position[i], empbee[j].position[i+1]);
      sumt += t;
      }
      return sumt;
}

double cal_tour_length_tempo(int c);
double cal_tour_length_tempo(int c)
{
      double t; 
      double sumt = 0.0;
      for(int i = 0; i<Nc-1; i++)
      {
      t = dist(tempos[c].position[i], tempos[c].position[i+1]);
      sumt += t;
      }
      return sumt;
}

double cal_tour_length_globalbest();
double cal_tour_length_globalbest()
{
      double t; 
      double sumt = 0.0;
      for(int i = 0; i<Nc-1; i++)
      {
      t = dist(obee.global_best_position[i], obee.global_best_position[i+1]);
      sumt += t;
      }
      return sumt;
}

int main()
{
    
    const int size = Nc * 3; //s = array size
    int ID[size];
    int count = 0; //loop counter
    ifstream dataset;
    FILE *fp;
    char buff[1000];
    fp = fopen("testresult.txt", "w");

    dataset.open("input.txt");
    while(count < size && dataset >> ID[count]) {
        count++;
    }

    dataset.close(); //close the file 
    
    for ( int i = 0; i < Nc; i++ )
    {
    city[i].x = ID[3*i+1];
    city[i].y = ID[3*i+2];
    }
    
    for ( int i = 0; i < Nc; i++ )
    {   printf("city %d :\n x : %d  \n y : %d \n\n\n", i, city[i].x, city[i].y);
    }
    
    for ( int i = 0; i < Nc; i++ )
    {
        for ( int j = 0; j < i; j++ )
        { city_dist[i][j] = dist(i,j);
          city_dist[j][i] = city_dist[i][j];
        }
        city_dist[i][i] = 0;
    }
    
     for ( int i = 0; i < Nc; i++ )
    {
        for ( int j = 0; j < Nc; j++ )
        { printf("%0.2f \t", city_dist[i][j]);
        }
        printf("\n\n");
    } 
    
    for ( int i = 0; i < B; i++ )
    {   
        for ( int j = 0; j < Nc; j++ )
        {   empbee[i].position[j] = j;
        }
    }
    
    srand(time(NULL));
    
    for ( int i = 0; i < B; i++ )
    {   
        printf(" Bee %d : \n ", i);
        random_shuffle(empbee[i].position, (empbee[i].position)+Nc);
        for ( int j = 0; j < Nc; j++ )
        { 
            printf("%d \t", empbee[i].position[j]);
        }
        printf("\n");
    }
    
     for ( int i = 0; i < B; i++ )
    {   
        empbee[i].tourlength = cal_tour_length(i);
        printf(" bee %d tour length: %0.2f\n ", i, empbee[i].tourlength);
        fprintf(fp, "Initial bee %d tour length: %0.2f\n ", i, empbee[i].tourlength);
        printf("\n");
    }
    
    for ( int i = 0; i < B; i++ )
    {   
        printf(" bee %d tour length: %0.2f \n ", i, empbee[i].tourlength);
        for ( int j = 0; j < Nc; j++ )
        {printf("%d \t", empbee[i].position[j]);}
        printf("\n");
    }
    
    //OPTIMAL TOURLENGTH 
    const int size_an = Nc;
    int IDA[size_an];
    int count_an = 0;
    ifstream optimal_sol;

    optimal_sol.open("output.txt");
    while(count_an < size_an && optimal_sol >> IDA[count_an]) {
        count_an++;
    }

    optimal_sol.close();

    printf(" optimal route \n ");
    for (int i = 0; i<Nc; i++)
    { printf(" %d \t ", IDA[i]-1);
    }
    
    for(int k = 0; k<Nc; k++)
    { tempos[0].position[k] = IDA[k]-1;}
    tempos[0].tourlength = cal_tour_length_tempo(0);
    fprintf(fp, "\n \n ORIGINAL OPTIMAL TOURLENGTH %0.2f \n \n ", tempos[0].tourlength);  
    
   double temp_better;
   for (int i = 0; i < B; i++ )
   {empbee[i].no_change_count = 0;}
   for(int k = 0; k<Nc; k++)
   {obee.global_best_position[k] = empbee[0].position[k];}
    obee.global_best_tourlength = empbee[0].tourlength;
   
//iterations
for(int z=1; z<=T; z++)
{
        //employeed bee phase
        for (int i = 0; i < B; i++ )
        {
            if(empbee[i].no_change_count > no_change_count_limit) // scout bee phase
            {empbee[i].no_change_count = 0;
            random_shuffle(empbee[i].position, (empbee[i].position)+Nc);
            }
            else
            {
                 for(int k = 0; k<Nc; k++)
                 {      for(int j = 0; j<part_no; j++)
                        {tempos[j].position[k] = empbee[i].position[k];}
                 }
   
                 for(int j = 0; j<part_no; j++)
                 { random_shuffle(tempos[j].position+(j*part_size),(tempos[j].position)+((j+1)*part_size));
                 }
     
                 for(int j = 0; j<part_no; j++)
                 {tempos[j].tourlength = cal_tour_length_tempo(j);}
   
  
                 temp_better = tempos[0].tourlength;
                 int temp_better_index = 0;
                 for(int h = 1; h<part_no; h++)
                 { if(tempos[h].tourlength < temp_better )
                 {temp_better = tempos[h].tourlength;
                 temp_better_index=h;}
                 }
                 
                 if( temp_better < empbee[i].tourlength)
                 { for(int k = 0; k<Nc; k++)
                 {empbee[i].position[k] = tempos[temp_better_index].position[k];}
                 empbee[i].tourlength  = temp_better;
                 }
                 else
                 { empbee[i].no_change_count++;}
            }
            if(empbee[i].tourlength < obee.global_best_tourlength)
            { for(int k = 0; k<Nc; k++)
              {obee.global_best_position[k] = empbee[i].position[k];}
               obee.global_best_tourlength = empbee[i].tourlength;
            }
        }
     //onlooker bee phase    
     for(int k = 0; k<Nc; k++)
     {      for(int j = 0; j<part_no; j++)
            {tempos[j].position[k] = obee.global_best_position[k];}
     }
    
     for(int j = 0; j<part_no; j++)
     { random_shuffle(tempos[j].position+(j*part_size),(tempos[j].position)+((j+1)*part_size));
     }
    
     for(int j = 0; j<part_no; j++)
     {tempos[j].tourlength = cal_tour_length_tempo(j);}
    
    
     temp_better = tempos[0].tourlength;
     int temp_better_index = 0;
     for(int h = 1; h<part_no; h++)
     { if(tempos[h].tourlength < temp_better )
     {temp_better = tempos[h].tourlength;
     temp_better_index=h;}
     }
     
     if( temp_better < obee.global_best_tourlength)
     { for(int k = 0; k<Nc; k++)
     {obee.global_best_position[k] = tempos[temp_better_index].position[k];}
     obee.global_best_tourlength  = temp_better;
     }
     
   int xchng;
   for(int k = 0; k<(Nc/2); k++)
   {       
           xchng = obee.global_best_position[k];
           obee.global_best_position[k] = obee.global_best_position[Nc-1-k];
           obee.global_best_position[Nc-1-k] = xchng; 
           temp_better = cal_tour_length_globalbest();
           if(temp_better < obee.global_best_tourlength)
           {obee.global_best_tourlength = temp_better; }
           else 
           {xchng = obee.global_best_position[k];
           obee.global_best_position[k] = obee.global_best_position[Nc-1-k];
           obee.global_best_position[Nc-1-k] = xchng;}
   } 
   
    fprintf(fp, "Iteration %d : global best before hill climbing %0.2f \n", z, obee.global_best_tourlength);
   
   //HILL CLIMBING APPROACH 
    int rn;
    rn = rand() % Nc; 
    for ( int i = 0; i < Nc; i++ )
    { 
        if(i!=rn)
        {
           xchng = obee.global_best_position[rn];
           obee.global_best_position[rn] = obee.global_best_position[i];
           obee.global_best_position[i] = xchng;   
           temp_better = cal_tour_length_globalbest();  
           if(temp_better < obee.global_best_tourlength)
           {obee.global_best_tourlength = temp_better; }
           else 
           {xchng = obee.global_best_position[rn];
           obee.global_best_position[rn] = obee.global_best_position[i];
           obee.global_best_position[i] = xchng;}  
        }        
    }
   
   // onlooker phase end
   
     printf("Iteration %d : global best %0.2f \n", z, obee.global_best_tourlength);
     fprintf(fp, "Iteration %d : global best after hill climbing %0.2f \n", z, obee.global_best_tourlength);
        
} 

    fclose(fp);
    int lx;
    printf("hello");   
    scanf(" %d", &lx);
    return 0;
}
