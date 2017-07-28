//SFLA for TSP
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<iostream>  
#include<math.h>
#include<algorithm>
#include<fstream>
using namespace std;

#define Nc 52
#define F 20
#define memeplex_no 4
#define T 100
#define small_iteration 10
#define part_no 13
#define part_size 4

struct City
{
int x;
int y;
};
City city[Nc];

double city_dist[Nc][Nc];

double dist(int, int);
double dist(int i, int j)
{
  double distance = sqrt((city[i].x - city[j].x)*(city[i].x - city[j].x)+(city[i].y - city[j].y)*(city[i].y - city[j].y));
  return distance;
}

struct Frog
{
int position[Nc];
double tourlength;
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
       
     
  /* for (i = 0; i<F; i++)
   {printf("FROG %d : %0.2f tourlength \n", i, frog[i].tourlength);
                 for(j=0; j<Nc; j++)
                 {printf("%d \t", frog[i].position[j]);}
                 printf("\n");
   } */
   
}

void swap(int i)
{
    double tempi;
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

double cal_tour_length(int);
double cal_tour_length(int j)
{
      double t; 
      double sumt = 0.0;
      for(int i = 0; i<Nc-1; i++)
      {
     // printf("%d %d ", frog[j].position[i], frog[j].position[i+1]);
      t = dist(frog[j].position[i], frog[j].position[i+1]);
     // printf("%0.2f \t ", t );
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
    //  printf("%d %d ", tempos[c].position[i], tempos[c].position[i+1]);
      t = dist(tempos[c].position[i], tempos[c].position[i+1]);
    //  printf("%0.2f \t ", t );
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
    
    int cities[Nc][2];
    for (int i = 0; i<Nc; i++)
    { cities[i][0] = ID[3*i+1];
      cities[i][1] = ID[3*i+2];
    }
    
    for ( int i = 0; i < Nc; i++ )
    {
    city[i].x = cities[i][0];
    city[i].y = cities[i][1];
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
        printf(" frog %d tour length: %0.2f\n ", i, frog[i].tourlength);
        fprintf(fp, "Initial frog %d tour length: %0.2f\n ", i, frog[i].tourlength);
        printf("\n");
    }
    
    for ( int i = 0; i < F; i++ )
    {   
        printf(" frog %d tour length: %0.2f \n ", i, frog[i].tourlength);
        for ( int j = 0; j < Nc; j++ )
        {printf("%d \t", frog[i].position[j]);}
        printf("\n");
    }
    
    //OPTIMAL TOURLENGTH 
    const int size_an = Nc;
    int IDA[size_an];
    int count_an = 0;
    ifstream employees;

    employees.open("output.txt");
    while(count_an < size_an && employees >> IDA[count_an]) {
        count_an++;
    }

    employees.close();

    printf(" optimal route \n ");
    for (int i = 0; i<Nc; i++)
    { printf(" %d \t ", IDA[i]-1);
    }
    
    for(int k = 0; k<Nc; k++)
    { tempos[0].position[k] = IDA[k]-1;}
    tempos[0].tourlength = cal_tour_length_tempo(0);
    fprintf(fp, "\n \n ORIGINAL OPTIMAL TOURLENGTH %0.2f \n \n ", tempos[0].tourlength);  
    
   double temp_better;
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
   printf("global best : %d \n", global_best);
 //  fprintf(fp, "global best before modification: %0.2f \n", frog[global_best].tourlength);
   
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
   
   printf("global best : %0.2f \n MODIFIED 1: %0.2f , 2: %0.2f , 3: %0.2f , 4: %0.2f \n\n", frog[global_best].tourlength, tempos[0].tourlength, tempos[1].tourlength, tempos[2].tourlength, tempos[3].tourlength);
    
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
  // temp_better = frog[global_best].tourlength;
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
        //  fprintf(fp, "global best after hill climbing: %0.2f \n", frog[global_best].tourlength);
        
  
   //END 
   for (int i = 0; i< memeplex_no; i++)
   {   if(memeplex[i].no_change_count < 5) //constant
       {
               for(int k = 0; k<Nc; k++)
               { for(int j = 0; j<part_no; j++)
                 {tempos[j].position[k] = frog[memeplex[i].best].position[k];}
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
} fprintf(fp, "Iteration %d - global best: %0.2f \n", z, frog[global_best].tourlength);
printf("Iteration %d - global best: %0.2f \n", z, frog[global_best].tourlength);
} 
 
    fclose(fp);
    int lx;
    printf("hello");   
    scanf(" %d", &lx);
    return 0;
}
