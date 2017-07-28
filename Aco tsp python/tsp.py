Nc=52
Na=25
T=50
import math
import random

class City:
    x=0
    y=0

city=[]
for i in range(Nc):
    a=City()
    city.append(a)
    
    
tem_position=[0 for x in range(Nc)]
globalbest_position=[0 for x in range(Nc)]
globalbest_tourlength=0
pheromone=[[0 for x in range(Nc)] for y in range(Nc)]
temp_pher=[[0 for x in range(Nc)] for y in range(Nc)]
city_dist=[[0 for x in range(Nc)] for y in range(Nc)]


class Ant:
    city=0
    tour=[]
    probability=[]
    tourlength=0

ant=[]
for i in range(Na):
    a=Ant()
    ant.append(a)
    ant[i].tour=[0 for x in range(Nc)]
    ant[i].probability=[0 for x in range(Nc)]

def cal_tour_length_temp():
      sumt = 0.0
      
      for i in range(Nc-1):
      

          t = city_dist[tem_position[i]][tem_position[i+1]]
          sumt += t
      
      return sumt


def cal_tour_length(k):
    sumt=0.0
    for i in range(Nc-1):
          t = city_dist[ant[k].tour[i]][ant[k].tour[i+1]]
          sumt += t
    return sumt

def dist(x,y):
    distance=math.sqrt((city[i].x - city[j].x)*(city[i].x - city[j].x)+(city[i].y - city[j].y)*(city[i].y - city[j].y))
    return distance
#constants
alpha =1
beta=2
rho=0.5


def transprob(i,j):
      
     if i==j:

         prob=0
     else:
         prob = math.pow(pheromone[i][j],alpha) * math.pow((1000/city_dist[i][j]),beta)
     return prob


#main
size=Nc*3
ID=[]
count=0

#Read input
with open("input.txt") as f:
    for line in f:
        ID.extend(map(int,line.split()))

for i in range(Nc):
    city[i].x = ID[3*i+1]
    city[i].y = ID[3*i+2]

for i in range(Nc):
    print "city %d :\n x : %d  \n y : %d \n\n\n"%(i, city[i].x, city[i].y)

for i in range(Nc):
    for j in range(i):

        city_dist[i][j] = dist(i,j)
        city_dist[j][i] = city_dist[i][j]
    city_dist[i][i] = 0

for i in range(Nc):
    for j in range(Nc):
        print "%0.2f \t"%(city_dist[i][j])
    print '\n \n'

#Read output
IDA=[]
with open("output.txt") as f:
    for line in f:
        IDA.extend(map(int,line.split()))


fp = open('testresult.txt', 'w')
fp.write(" optimal route \n")
for i in range(Nc):
    tem_position[i] = IDA[i]-1
    fp.write( " %d -> "%( tem_position[i]))
fp.write('\n')

temp_tourlength=cal_tour_length_temp()

fp.write("\n \n original optimal solution %0.2f \n \n "%(temp_tourlength))
print "\n \n original optimal solution %0.2f \n \n "%(temp_tourlength)

init_pheromone = 1.00

for i in range(Nc):
    for j in range(i):
        pheromone[i][j]=init_pheromone

        pheromone[j][i]=init_pheromone
    pheromone[i][i]=0








for t in range(1,T+1):
     for i in range(Na):
         for j in range(Nc):
             ant[i].tour[j]=j

     for i in range(Na):
        random.shuffle(ant[i].tour)

     for i in range(Na):

            ant[i].city=ant[i].tour[0]
            for j in range(Nc):

                ant[i].probability[j] = transprob(ant[i].tour[0],ant[i].tour[j])
            
     temp_tour=0
     temp_prob=0
     ind=0
     for j in range(Nc-2):
        for i in range(Na):
            temp_prob = ant[i].probability[j+1]
            
            for k in range(j+2,Nc):
            
                if(ant[i].probability[k]>temp_prob):
                
                    temp_prob = ant[i].probability[k]
                    ind = k
                
            
            temp_tour = ant[i].tour[j+1]
            ant[i].tour[j+1] = ant[i].tour[ind]
            ant[i].tour[ind] = temp_tour
            ant[i].probability[j+1] = 0
            
            
            for k in range(j+2,Nc):
                    ant[i].probability[k] = transprob(ant[i].tour[j+1], ant[i].tour[k])
             
     for i in range(Na):
        ant[i].tourlength = cal_tour_length(i)


     
        

     for i in range(Nc):
         for j in range(Nc):
             temp_pher[i][j]=0.00
     Q=1000.00

     for i in range(Na):
            for j in range(Nc-1):
                m = ant[i].tour[j]
                n = ant[i].tour[j+1]
                temp_pher[m][n] = temp_pher[m][n] + (Q/ant[i].tourlength)
                temp_pher[n][m] = temp_pher[m][n]                                      

     for i in range(Nc):

                for j in range(i):

                   pheromone[i][j] = (1-rho)* pheromone[i][j] + temp_pher[i][j]
                   pheromone[j][i] = pheromone[i][j]                                   
                pheromone[i][i] = 0

     best_tourlength=ant[0].tourlength
     for i in range(Na):
        if(ant[i].tourlength<best_tourlength):
            best_tourlength = ant[i].tourlength;
            s = i
                                                      
     if t==1:
         globalbest_tourlength = best_tourlength
         for i in range(Nc):
             globalbest_position[i] = ant[s].tour[i]
     else:
         if(best_tourlength < globalbest_tourlength):
             globalbest_tourlength = best_tourlength;
             for i in range(Nc):
                 globalbest_position[i] = ant[s].tour[i]
     
     
     print "Best tour length : %0.2f \n"%(best_tourlength)
     fp.write("Iteration %d : global best %0.2f\n"%(t, globalbest_tourlength))
     for i in range(Nc):
        fp.write(" %d -> "%(globalbest_position[i]))
     fp.write(" \n\n ")
     print "Iteration %d : global best %0.2f \n"%(t, globalbest_tourlength)

fp.close()
print 'hello'
    






















      












