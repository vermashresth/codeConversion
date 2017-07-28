//SFLA for QAP
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<iostream>  
#include<math.h>
#include<algorithm>
#include<fstream>
using namespace std;

#define Nc 16
#define F 30
#define memeplex_no 5
#define T 40
#define small_iteration 10
#define part_no 4
#define part_size 4

struct City
{
int x;
int y;
};
City city[Nc];

int city_dist[Nc][Nc];
int flow[Nc][Nc];
int position_opt[Nc];

struct Frog
{
int position[Nc];
int tourlength;
};
Frog frog[F];
Frog tempos[part_no];

struct Memeplex
{
       int worst;
       int best;
       int no_change_count;
};
Memeplex memeplex[memeplex_no];

int global_best;

void swap(int i);
void bubble_sort(int arr[]);

void bubble_sort(){
   int i, j;
   int n = F;
   for(j=1;j<n;j++)
     {
          for(i=0; i<n-j; i++)
          {
               if(frog[i].tourlength>frog[i+1].tourlength)
               {
                     swap(i);
               } 
          }
     }
       
     
   for (i = 0; i<F; i++)
   {printf("FROG %d : %d tourlength \n", i, frog[i].tourlength);
   for(j=0; j<Nc; j++)
   {printf("%d \t", frog[i].position[j]);}
   printf("\n");
   }
   
}

void swap(int i)
{
    int tempi;
    int temposi[Nc];
    
    tempi=frog[i].tourlength;
    for(int k = 0; k<Nc; k++)
    {temposi[k] = frog[i].position[k];}
    
    frog[i].tourlength=frog[i+1].tourlength;
    for(int k = 0; k<Nc; k++)
    {frog[i].position[k] = frog[i+1].position[k];}
    
    frog[i+1].tourlength=tempi;
    for(int k = 0; k<Nc; k++)
    {frog[i+1].position[k] = temposi[k];}
}

int cal_tour_length(int);
int cal_tour_length(int k)
{
      int sum = 0;
      int p1, p2;
      for(int i = 0; i<Nc; i++)
      {  for(int j = 0; j<Nc; j++)
         { 
                 p1 = frog[k].position[i];
                 p2 = frog[k].position[j];
                 sum = sum + (flow[p1][p2] * city_dist[i][j]) ;
         }
      }
      return sum;
}
 
int cal_tour_length_tempo(int);
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
     
     for ( int i = 0; i < F; i++ )
    {   
      //  printf(" frog %d : \n ", i);
        for ( int j = 0; j < Nc; j++ )
        {   frog[i].position[j] = j;
           // printf("%d \t", frog[i].position[j]);
        }
       // printf("\n");
    }
    
    srand(time(NULL));
    
    for ( int i = 0; i < F; i++ )
    {   
        printf(" frog %d : \n ", i);
        random_shuffle(frog[i].position, (frog[i].position)+Nc);
        for ( int j = 0; j < Nc; j++ )
        { 
            printf("%d \t", frog[i].position[j]);
        }
        printf("\n");
    }
     
    for ( int i = 0; i < F; i++ )
    {   
        frog[i].tourlength = cal_tour_length(i);
        printf(" frog %d tour length: %d\n ", i, frog[i].tourlength);
        fprintf(fp, " frog %d tour length: %d\n ", i, frog[i].tourlength);
        printf("\n");
    }
    
    for ( int i = 0; i < F; i++ )
    {   
        printf(" frog %d tour length: %d \n ", i, frog[i].tourlength);
        for ( int j = 0; j < Nc; j++ )
        {
            printf("%d \t", frog[i].position[j]);
        }
        printf("\n");
    }
    
    
  int temp_better;
//iterations
for(int z=1; z<=T; z++)
{
   bubble_sort();
   for (int i = 0; i < memeplex_no; i++ )
   {memeplex[i].no_change_count = 0;}
   int kx;
   for (int l = 1; l<= small_iteration; l++) 
   {
   
   for ( int i = 0; i < memeplex_no; i++ )
   {
       memeplex[i].best=i;
       memeplex[i].worst=i;
       kx = i;
       int xup = F/memeplex_no;
       for  ( int j = 1; j < xup; j++ ) 
       {
            kx = kx + memeplex_no;
            if (frog[kx].tourlength < frog[memeplex[i].best].tourlength)
            {memeplex[i].best = kx;}
            if (frog[kx].tourlength > frog[memeplex[i].worst].tourlength)
            {memeplex[i].worst = kx;}
       }
   } 
   
   global_best = memeplex[0].best;
   for (int s = 1; s< memeplex_no; s++)
   {
       if(frog[memeplex[s].best].tourlength < frog[global_best].tourlength)
       { global_best = memeplex[s].best;}
   }
   
   //MODIFICATION CODE
   for(int k = 0; k<Nc; k++)
   {      for(int j = 0; j<part_no; j++)
           {tempos[j].position[k] = frog[global_best].position[k];}
   }
   
   for(int j = 0; j<part_no; j++)
   { random_shuffle(tempos[j].position+(j*part_size),(tempos[j].position)+((j+1)*part_size));
   }
     
   for(int j = 0; j<part_no; j++)
   {tempos[j].tourlength = cal_tour_length_tempo(j);}
   
  // printf("global best : %d \n MODIFIED 1: %d , 2: %d , 3: %d \n\n", frog[global_best].tourlength, tempos[0].tourlength, tempos[1].tourlength, tempos[2].tourlength);
   
   temp_better = tempos[0].tourlength;
   int temp_better_index = 0;
   for(int h = 1; h<part_no; h++)
    { if(tempos[h].tourlength < temp_better )
      {temp_better = tempos[h].tourlength;
      temp_better_index=h;}
    }
    if( temp_better < frog[global_best].tourlength)
    { for(int k = 0; k<Nc; k++)
      {frog[global_best].position[k] = tempos[temp_better_index].position[k];}
      frog[global_best].tourlength  = temp_better;
    }
    
   for(int k = 0; k<Nc; k++)
   {     tempos[0].position[k] = frog[global_best].position[k];
   }

  int xchng;
   for(int k = 0; k<(Nc/2); k++)
   {       
           xchng = frog[global_best].position[k];
           frog[global_best].position[k]= frog[global_best].position[Nc-1-k];
           frog[global_best].position[Nc-1-k] = xchng; 
           temp_better = cal_tour_length(global_best);
           if(temp_better < frog[global_best].tourlength)
           {frog[global_best].tourlength = temp_better; }
           else 
           {xchng = frog[global_best].position[k];
           frog[global_best].position[k]= frog[global_best].position[Nc-1-k];
           frog[global_best].position[Nc-1-k] = xchng;}
           
   }
   
   //HILL CLIMBING APPROACH 
    int rn;
    rn = rand() % Nc; 
    for ( int i = 0; i < Nc; i++ )
    { 
        if(i!=rn)
        {
           xchng = frog[global_best].position[rn];
           frog[global_best].position[rn]= frog[global_best].position[i];
           frog[global_best].position[i] = xchng; 
           temp_better = cal_tour_length(global_best);
           if(temp_better < frog[global_best].tourlength)
           {frog[global_best].tourlength = temp_better; }
           else 
           {xchng = frog[global_best].position[rn];
           frog[global_best].position[rn]= frog[global_best].position[i];
           frog[global_best].position[i] = xchng;}  
        }        
    }
  
   //END 
   for (int i = 0; i< memeplex_no; i++)
   {   if(memeplex[i].no_change_count < 5) //constant
       {
               for(int k = 0; k<Nc; k++)
               { for(int j = 0; j<part_no; j++)
                 {tempos[j].position[k] = frog[global_best].position[k];}
               }
       
               for(int j = 0; j<part_no; j++)
               { random_shuffle(tempos[j].position+(j*part_size),(tempos[j].position)+((j+1)*part_size));}
               
               for(int j = 0; j<part_no; j++)
               {tempos[j].tourlength = cal_tour_length_tempo(j);}
               
               temp_better = tempos[0].tourlength;
               int temp_better_index = 0;
               for(int h = 1; h<part_no; h++)
               { if(tempos[h].tourlength < temp_better )
               {temp_better = tempos[h].tourlength;
               temp_better_index=h;}
               }
      
               if(temp_better < frog[memeplex[i].worst].tourlength)
               {
                   for(int k = 0; k<Nc; k++)
                   { frog[memeplex[i].worst].position[k]=tempos[temp_better_index].position[k];}
                     frog[memeplex[i].worst].tourlength=temp_better;
               }
               else
               {memeplex[i].no_change_count++;}
       }
       else
       {
               
             for(int k = 0; k<Nc; k++)
               { for(int j = 0; j<part_no; j++)
                 {tempos[j].position[k] = frog[global_best].position[k];}
               }
      
               for(int j = 0; j<part_no; j++)
               { random_shuffle(tempos[j].position+(j*part_size),(tempos[j].position)+((j+1)*part_size));}
               for(int j = 0; j<part_no; j++)
               {tempos[j].tourlength = cal_tour_length_tempo(j);}
               
               temp_better = tempos[0].tourlength;
               int temp_better_index = 0;
               for(int h = 1; h<part_no; h++)
               { if(tempos[h].tourlength < temp_better )
               {temp_better = tempos[h].tourlength;
               temp_better_index=h;}
               }
               
               if(temp_better < frog[memeplex[i].worst].tourlength)
               {
                   for(int k = 0; k<Nc; k++)
                   { frog[memeplex[i].worst].position[k]=tempos[temp_better_index].position[k];}
                     frog[memeplex[i].worst].tourlength=temp_better;
               }   
       } 
   } 
}
fprintf(fp, "Iteration %d global best : %d \n", z, frog[global_best].tourlength);
printf("Iteration %d global best : %d \n", z, frog[global_best].tourlength);
} 
 
    fclose(fp);
    int lx;
    printf("hello");   
    scanf(" %d", &lx);
    return 0;
}
