//ACO for TSP
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<iostream>  
#include<math.h>
#include<algorithm>
#include<fstream>

using namespace std;
#define Nc 52
#define Na 25
#define T 50

struct City
{
int x;
int y;
};
City city[Nc];
int tem_position[Nc];
int globalbest_position[Nc];
double globalbest_tourlength;
double city_dist[Nc][Nc];
double pheromone[Nc][Nc];
double temp_pher[Nc][Nc];

struct Ant
{
       int city;
       int tour[Nc];
       double probability[Nc];
       double tourlength;
};
Ant ant[Na];

double cal_tour_length_temp();
double cal_tour_length_temp()
{
      double t; 
      double sumt = 0.0;
      for(int i = 0; i<Nc-1; i++)
      {
      t = city_dist[tem_position[i]][tem_position[i+1]];
      sumt += t;
      }
      return sumt;
}

double cal_tour_length(int);
double cal_tour_length(int k)
{
      double t; 
      double sumt = 0.0;
      for(int i = 0; i<Nc-1; i++)
      {
      t = city_dist[ant[k].tour[i]][ant[k].tour[i+1]];
      sumt += t;
      }
      return sumt;
}

double dist(int, int);
double dist(int i, int j)
{
 double distance = sqrt((city[i].x - city[j].x)*(city[i].x - city[j].x)+(city[i].y - city[j].y)*(city[i].y - city[j].y));
 return distance;
}

//constants
float alpha = 1;
float beta = 2;
float rho = 0.5;

double transprob(int, int);
double transprob(int i, int j)
{     
      double prob;
      if (i == j)
      prob = 0;
      else
     { prob = pow(pheromone[i][j],alpha) * pow((1000/city_dist[i][j]),beta);
     }
      return prob;
}

int main()
{
    
    int k;
    FILE *fp;
    char buff[1000];
    fp = fopen("testresult.txt", "w");
    
    double temp_tourlength;
    
    const int size = Nc * 3; 
    int ID[size];
    int count = 0; 
    ifstream dataset;
    

    dataset.open("input.txt");
    while(count < size && dataset >> ID[count]) {
        count++;
    }

    dataset.close(); 
    
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
        printf("\n \n");
    }
    
    //OPTIMAL TOURLENGTH 
    const int size_an = Nc;
    int IDA[size_an];
    int count_an = 0;
    ifstream optimal_sol;

    optimal_sol.open("output.txt");
    while(count_an < size_an && optimal_sol >> IDA[count_an]) 
    { count_an++; }

    optimal_sol.close();

    fprintf(fp, " optimal route \n ");
    for (int i = 0; i<Nc; i++)
    { tem_position[i] = IDA[i]-1;
       fprintf(fp, " %d -> ", tem_position[i]);
    }
    
    temp_tourlength = cal_tour_length_temp();
    printf("\n \n ORIGINAL OPTIMAL TOURLENGTH %0.2f \n \n ", temp_tourlength);
    fprintf(fp, "\n \n ORIGINAL OPTIMAL TOURLENGTH %0.2f \n \n ", temp_tourlength);
    
    double init_pheromone = 1.00;
    for ( int i = 0; i < Nc; i++ )
    {
        for ( int j = 0; j < i; j++ )
        { pheromone[i][j] = init_pheromone;
          pheromone[j][i] = init_pheromone;
        }
        pheromone[i][i] = 0;
    }
     for ( int t = 1; t<= T; t++ )
    {
     
   /* fprintf(fp, "\nITERATION %d \n", t);     
    for ( int i = 0; i < Nc; i++ )
    {
        for ( int j = 0; j < Nc; j++ )
        { printf("%0.2f \t", pheromone[i][j]);
          fprintf(fp, "%0.2f \t", pheromone[i][j]);
        }
        printf("\n");
        fprintf(fp, "\n");
    } */
     
   for ( int i = 0; i < Na; i++ )
    {   
        for ( int j = 0; j < Nc; j++ )
        {   ant[i].tour[j] = j;
        }
    }
    
    srand(time(NULL));
    
    for ( int i = 0; i < Na; i++ )
    {   
       // printf(" Ant %d : \n ", i);
        random_shuffle(ant[i].tour, (ant[i].tour)+Nc);
       /* for ( int j = 0; j < Nc; j++ )
        { 
            printf("%d \t", ant[i].tour[j]);
        }
        printf("\n"); */
    }
    
    for ( int i = 0; i < Na; i++ )
    { 
         ant[i].city = ant[i].tour[0];
        // printf("ant %d city %d \n",i,ant[i].city);
         for ( int j = 0; j < Nc; j++ )
         {  ant[i].probability[j] = transprob(ant[i].tour[0],ant[i].tour[j]);
          //   printf(" %0.2f | ", ant[i].probability[j]);
         }
        // printf("\n");
    }
   
    int temp_tour; double temp_prob;
    int ind;
    for ( int j = 0; j < Nc-2; j++ )
    {
        for ( int i = 0; i < Na; i++ )
        { 
            temp_prob = ant[i].probability[j+1];
            for ( int k = j+2; k < Nc; k++ )
            {
                if(ant[i].probability[k]>temp_prob)
                {
                    temp_prob = ant[i].probability[k];
                    ind = k;
                }
            }
            temp_tour = ant[i].tour[j+1];
            ant[i].tour[j+1] = ant[i].tour[ind];
            ant[i].tour[ind] = temp_tour;
            ant[i].probability[j+1] = 0;
            for ( int k = j+2; k < Nc; k++ )
            {
                    ant[i].probability[k] = transprob(ant[i].tour[j+1], ant[i].tour[k]);
            } 
        }
    }
    
   /* for ( int j = 0; j < Na; j++ )
    {
        printf("Ant %d : ", j);
        for ( int i = 0; i < Nc; i++ )
        {printf(" %d -> ", ant[j].tour[i]);}
        printf("\n\n");
    } */
   
    
    for ( int i = 0; i < Na; i++ )
    {
        ant[i].tourlength = cal_tour_length(i);
    }
    
    //pheromone update for each path
    for ( int i = 0; i < Nc; i++ )
    {
        for ( int j = 0; j < Nc; j++ )
        { 
            temp_pher[i][j] = 0.00;
        }
    }
    
    double Q = 1000.00; //constant
     for ( int i = 0; i < Na; i++ )
    {
        for ( int j = 0; j < Nc-1; j++ )
        {   int m, n;
            m = ant[i].tour[j];
            n = ant[i].tour[j+1];
            temp_pher[m][n] = temp_pher[m][n] + (Q/ant[i].tourlength);
            temp_pher[n][m] = temp_pher[m][n];
        }
    }
    
    for ( int i = 0; i < Nc; i++ )
    {
        for ( int j = 0; j < i; j++ )
        {  
            pheromone[i][j] = (1-rho)* pheromone[i][j] + temp_pher[i][j];
            pheromone[j][i] = pheromone[i][j];
        }
        pheromone[i][i] = 0;
    }
    
    double best_tourlength = ant[0].tourlength;
    int s;
    for ( int i = 0; i < Na; i++ )
    {
        if(ant[i].tourlength < best_tourlength)
        {
            best_tourlength = ant[i].tourlength;
            s = i;
        }
    }
    
    if(t == 1)
    {    globalbest_tourlength = best_tourlength;
         for ( int i = 0; i < Nc; i++ )
         {globalbest_position[i] = ant[s].tour[i];}
    }
    else
    {   if(best_tourlength < globalbest_tourlength)
        {    globalbest_tourlength = best_tourlength;
             for ( int i = 0; i < Nc; i++ )
             {globalbest_position[i] = ant[s].tour[i];}
        }
    }
    
    printf("Best tour length : %0.2f \n", best_tourlength);
    fprintf(fp, "Iteration %d : global best %0.2f \n", t, globalbest_tourlength);
    for ( int i = 0; i < Nc; i++ )
    {   
        fprintf(fp, " %d -> " ,globalbest_position[i]);
    }
    fprintf(fp, " \n\n " );
    printf("Iteration %d : global best %0.2f \n", t, globalbest_tourlength);
   
    
    }
    
    fclose(fp); 
    printf("hello");   
    scanf(" %d", &k);
    return 0;
}
