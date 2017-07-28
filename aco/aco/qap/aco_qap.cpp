//ACO for QAP
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<iostream>  
#include<math.h>
#include<algorithm>
#include<fstream>

using namespace std;
#define Nc 32
#define Na 20
#define T 100


int tem_position[Nc];
int globalbest_position[Nc];
int globalbest_tourlength;
double pheromone[Nc][Nc];
double temp_pher[Nc][Nc];
int city_dist[Nc][Nc];
int flow[Nc][Nc];

struct Ant
{
       int city;
       int tour[Nc];
       double probability[Nc];
       int tourlength;
};
Ant ant[Na];

int cal_tour_length_temp();
int cal_tour_length_temp()
{
    
      int sum = 0;
      int p1, p2;
      for(int i = 0; i<Nc; i++)
      {  for(int j = 0; j<Nc; j++)
         { 
                 p1 = tem_position[i];
                 p2 = tem_position[j];
                 sum = sum + (flow[p1][p2] * city_dist[i][j]) ;
         }
      }
      return sum;
}

int cal_tour_length(int);
int cal_tour_length(int k)
{
      int sum = 0;
      int p1, p2;
      for(int i = 0; i<Nc; i++)
      {  for(int j = 0; j<Nc; j++)
         { 
                 p1 = ant[k].tour[i];
                 p2 = ant[k].tour[j];
                 sum = sum + (flow[p1][p2] * city_dist[i][j]) ;
         }
      }
      return sum;
}

//constants
float alpha = 1;
float beta = 2;
float rho = 0.5;
float mu = 0.3;

double transprob(int, int, int);
double transprob(int id, int frst, int scnd)
{     
      int p1, p2, p3;
      p1 = ant[id].tour[frst];
      p2 = ant[id].tour[scnd];
      double prob = 0.0;
      double cost = 0.0;
      for(int i = 0; i<= frst; i++ )
      {    p3 = ant[id].tour[i];
           cost = cost + city_dist[i][frst+1]*flow[p3][p2] + city_dist[frst+1][i]*flow[p2][p3];
      }
    //  printf("cost : %0.2f \n", cost);
      
      prob = pow(pheromone[frst+1][p2],alpha) * pow(2000/cost,beta);
      
    //  printf("prob : %0.2f \n", prob);
      /* MESH UP 
       if (frst == scnd)
       {prob = 0.0;}
       else
       {
          if(flow[p1][p2] == 0 || city_dist[frst][frst+1] == 0)
          {prob = pow(pheromone[frst+1][p2],5);}
          else
          {   prob = pow(pheromone[frst+1][p2],alpha) * pow((100/(city_dist[frst][frst+1]*flow[p1][p2])),beta);
              prob = mu * pheromone[frst+1][p2] + (1-mu)*(300/(city_dist[frst][frst+1]*flow[p1][p2]));
          }
       } */
     
     
      return prob;
}

int main()
{    
    int k;
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
    
    FILE *fp;
    char buff[1000];
    fp = fopen("testresult.txt", "w");
    
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
    { tem_position[i] = IDO[i]-1;
    }
    fprintf(fp, " optimal route \n ");
    for (int i = 0; i<Nc; i++)
    { fprintf(fp, " %3d ", tem_position[i]);
    }
    fprintf(fp, " \n ");
    
    int sum  = cal_tour_length_temp();
   
     fprintf(fp, "\n \n original optimal solution %d \n \n ", sum); 
     printf("\n \n original optimal solution %d \n \n ", sum); 

    double init_pheromone = 1.00;
    for ( int i = 0; i < Nc; i++ )
    {
        for ( int j = 0; j < Nc; j++ )
        { pheromone[i][j] = init_pheromone;
        }
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
    }  */
     
   for ( int i = 0; i < Na; i++ )
    {   
        for ( int j = 0; j < Nc; j++ )
        {   ant[i].tour[j] = j;
        }
    }
    
    srand(time(NULL));
    
    for ( int i = 0; i < Na; i++ )
    {   
      //  printf(" Ant %d : \n ", i);
        random_shuffle(ant[i].tour, (ant[i].tour)+Nc);
      //  for ( int j = 0; j < Nc; j++ )
      //  { printf("%d \t", ant[i].tour[j]);}
      //   printf("\n"); 
    }
    
    for ( int i = 0; i < Na; i++ )
    { 
         ant[i].city = ant[i].tour[0];
        // printf("ant %d city %d \n",i,ant[i].city);
         ant[i].probability[0] = 0.0;
         for ( int j = 1; j < Nc; j++ )
         { //  ant[i].probability[j] = transprob(ant[i].tour[0],ant[i].tour[j]);
             ant[i].probability[j] = transprob(i, 0, j);
            // printf("prob : %0.2f \t ", ant[i].probability[j]);
         }
       //  printf("\n");
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
            int d = j+1;
            for ( int k = j+2; k < Nc; k++ )
            {
                ant[i].probability[k] = transprob(i, d, k);
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
    
    int best_tourlength = ant[0].tourlength;
    int s;
    for ( int i = 0; i < Na; i++ )
    {
        if(ant[i].tourlength < best_tourlength)
        {
            best_tourlength = ant[i].tourlength;
            s = i;
        }
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
    
   /* // update for all ants
     for ( int i = 0; i < Na; i++ )
    {
        for ( int j = 0; j < Nc; j++ )
        {   int m, n;
            m = j;
            n = ant[i].tour[j];
            temp_pher[m][n] = temp_pher[m][n] + (Q/ant[i].tourlength);
        }
    } */
    
    //update for best ant only
    for ( int j = 0; j < Nc; j++ )
    {   int m, n;
        m = j;
        n = ant[s].tour[j];
        temp_pher[m][n] = temp_pher[m][n] + (Q/ant[s].tourlength);
    }
    
    for ( int i = 0; i < Nc; i++ )
    {
        for ( int j = 0; j < Nc; j++ )
        {  
            if (i == j)
            {pheromone[i][i] = 0;}
            else
            {pheromone[i][j] = (1-rho)* pheromone[i][j] + temp_pher[i][j];}
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
    fprintf(fp, "Iteration %d : best %d \n", t, best_tourlength);
    for ( int i = 0; i < Nc; i++ )
    {   
        fprintf(fp, " %d -> " ,ant[s].tour[i]);
    }
    fprintf(fp, " \n\n " );
    printf("Best tour length : %d \n", best_tourlength);
    fprintf(fp, "Iteration %d : global best %d \n", t, globalbest_tourlength);
    for ( int i = 0; i < Nc; i++ )
    {   
        fprintf(fp, " %d -> " ,globalbest_position[i]);
    }
    fprintf(fp, " \n\n " );
    printf("Iteration %d : global best %d \n", t, globalbest_tourlength);
   
      
    } 
    
    fclose(fp); 
    printf("hello");   
    scanf(" %d", &k);
    return 0;
}
