//ABC for QAP
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<iostream>  
#include<math.h>
#include<algorithm>
#include<fstream>
using namespace std;

#define Nc 16
#define T 50
#define no_change_count_limit 5
#define part_no 4
#define part_size 4
#define B 30

struct City
{
int x;
int y;
};
City city[Nc];

int city_dist[Nc][Nc];
int flow[Nc][Nc];
int position_opt[Nc];

struct Emp_Bee
{
int position[Nc];
int tourlength;
int no_change_count;
};
Emp_Bee empbee[B];

struct Onlkr_Bee
{
int global_best_position[Nc];
int global_best_tourlength;
};
Onlkr_Bee obee;

Emp_Bee tempos[part_no];

int cal_tour_length(int);
int cal_tour_length(int k)
{
       int sum = 0;
      int p1, p2;
      for(int i = 0; i<Nc; i++)
      {  for(int j = 0; j<Nc; j++)
         { 
                 p1 = empbee[k].position[i];
                 p2 = empbee[k].position[j];
                 sum = sum + (flow[p1][p2] * city_dist[i][j]) ;
         }
      }
      return sum;
}

int cal_tour_length_tempo(int k);
int cal_tour_length_tempo(int k)
{
      int sum = 0;
      int p1, p2;
      for(int i = 0; i<Nc; i++)
      {  for(int j = 0; j<Nc; j++)
         { 
                 p1 = tempos[k].position[i];
                 p2 = tempos[k].position[j];
                 sum = sum + (flow[p1][p2] * city_dist[i][j]) ;
         }
      }
      return sum;
}

int cal_tour_length_globalbest();
int cal_tour_length_globalbest()
{
     int sum = 0;
      int p1, p2;
      for(int i = 0; i<Nc; i++)
      {  for(int j = 0; j<Nc; j++)
         { 
                 p1 = obee.global_best_position[i];
                 p2 = obee.global_best_position[j];
                 sum = sum + (flow[p1][p2] * city_dist[i][j]) ;
         }
      }
      return sum;
}

int main()
{
     FILE *fp;
    char buff[1000];
    fp = fopen("testresult.txt", "w");
    
    const int size = Nc * Nc;
    int ID[size];
    int count = 0;
    ifstream dataset1;
    dataset1.open("input1.txt");
    while(count < size && dataset1 >> ID[count]) {
        count++;
    }
    dataset1.close(); 
    
    for (int i = 0; i<Nc; i++)
    {
        for (int j = 0; j<Nc; j++)
        { city_dist[i][j] = ID[(Nc*i)+j];}
    }
    
    for ( int i = 0; i < Nc; i++ )
    {
        for ( int j = 0; j < Nc; j++ )
        { printf("%4d", city_dist[i][j]);
        }
        printf("\n");
    } 
        
    ifstream dataset2;
    count = 0;
    dataset2.open("input2.txt");
    while(count < size && dataset2 >> ID[count]) {
        count++;
    }
    dataset2.close();
   
    for (int i = 0; i<Nc; i++)
    {
        for (int j = 0; j<Nc; j++)
        { flow[i][j] = ID[(Nc*i)+j];}
    }
    
    cout<<"\n\n"; 
    for ( int i = 0; i < Nc; i++ )
    {
        for ( int j = 0; j < Nc; j++ )
        { printf("%4d", flow[i][j]);
        }
        printf("\n");
    } 
    
    //GIVEN OPTIMAL SOLUTION FOR QAP 
    const int size_opt = Nc;
    int IDO[size_opt];
    int count_opt = 0;
    ifstream optimal;

   optimal.open("input3.txt");
    while(count_opt < size_opt && optimal >> IDO[count_opt]) {
        count_opt++;
    }

    optimal.close();
    
    for (int i = 0; i<Nc; i++)
    { position_opt[i] = IDO[i]-1;
    }
    fprintf(fp, " optimal route \n ");
    for (int i = 0; i<Nc; i++)
    { fprintf(fp, " %3d ", position_opt[i]);
    }
    fprintf(fp, " \n ");
    
    int sum = 0;
    int p1, p2;
      for(int i = 0; i<Nc; i++)
      {  for(int j = 0; j<Nc; j++)
         { 
                 p1 = position_opt[i];
                 p2 = position_opt[j];
                 sum = sum + (flow[p1][p2] * city_dist[i][j]) ;
         }
      }
   
     fprintf(fp, "original optimal solution %d \n \n ", sum); 
     
   
    
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
        printf(" bee %d tour length: %d\n ", i, empbee[i].tourlength);
        fprintf(fp, "Initial bee %d tour length: %d\n ", i, empbee[i].tourlength);
        printf("\n");
    }
    
    for ( int i = 0; i < B; i++ )
    {   
        printf(" bee %d tour length: %d \n ", i, empbee[i].tourlength);
        for ( int j = 0; j < Nc; j++ )
        {printf("%d \t", empbee[i].position[j]);}
        printf("\n");
    }
     
   
   int temp_better;
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
   
   fprintf(fp, "Iteration %d : global best before hill climbing %d \n", z, obee.global_best_tourlength);
   
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
   
     printf("Iteration %d : global best %d \n", z, obee.global_best_tourlength);
     fprintf(fp, "Iteration %d : global best after hill climbing %d \n", z, obee.global_best_tourlength);
           
} 

    fclose(fp);
    int lx;
    printf("hello");   
    scanf(" %d", &lx);
    return 0;
}
