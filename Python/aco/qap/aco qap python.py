Nc=32
Na=20
T=100
import math
import random
tem_position=[0 for x in range(Nc)]
globalbest_position=[0 for x in range(Nc)]
globalbest_tourlength=0
pheromone=[[0 for x in range(Nc)] for y in range(Nc)]
temp_pher=[[0 for x in range(Nc)] for y in range(Nc)]
city_dist=[[0 for x in range(Nc)] for y in range(Nc)]
flow=[[0 for x in range(Nc)] for y in range(Nc)]

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
      sum = 0
      
      for i in range(Nc):
        for j in range(Nc):
                 p1 = tem_position[i]
                 p2 = tem_position[j]
                 sum = sum + (flow[p1][p2] * city_dist[i][j]) 
      return sum


def cal_tour_length(k):
    sum=0
    for i in range(Nc):
        for j in range(Nc):
                p1 = ant[k].tour[i]
                p2 = ant[k].tour[j]
                sum = sum + (flow[p1][p2] * city_dist[i][j]) 
    return sum

#constants
alpha =1
beta=2
rho=0.5
mu=0.3

def transprob(id,frst,scnd):
      
      p1 = ant[id].tour[frst];
      p2 = ant[id].tour[scnd];
      prob = 0.0;
      cost = 0.0;
      for i in range(frst+1):
           p3 = ant[id].tour[i]
           cost = cost + city_dist[i][frst+1]*flow[p3][p2] + city_dist[frst+1][i]*flow[p2][p3]
      if cost==0:

          
          prob=0.0
      else:
          prob = math.pow(pheromone[frst+1][p2],alpha) * math.pow(2000/cost,beta)
      return prob



#main
size=Nc*Nc
ID=[]
count=0

#Read input1
with open("input1.txt") as f:
    for line in f:
        ID.extend(map(int,line.split()))

for i in range(Nc):
    for j in range(Nc):
        city_dist[i][j] = ID[(Nc*i)+j]

"""for i in range(Nc):
    for j in range(Nc):
        print "%4d"%(city_dist[i][j])
    print "\n"
"""


#read input2 
ID=[]
with open("input2.txt") as f:
    for line in f:
        ID.extend(map(int,line.split()))

for i in range(Nc):
    for j in range(Nc):
        flow[i][j] = ID[(Nc*i)+j]

for i in range(Nc):
    for j in range(Nc):
        print "%4d"%(flow[i][j])
    print '\n'


#read input3
size_opt=Nc
ID0=[]
with open("input3.txt") as f:
    for line in f:
        ID0.extend(map(int,line.split()))

for i in range(Nc):
    tem_position[i]=ID0[i]-1

fp = open('testresult.txt', 'w')
fp.write(" optimal route \n")
for i in range(Nc):
    fp.write("%3d"%(tem_position[i]))
fp.write('\n')

sum=cal_tour_length_temp()

fp.write("\n \n original optimal solution %d \n \n "%(sum))
print "\n \n original optimal solution %d \n \n "%(sum)

init_pheromone = 1.00

for i in range(Nc):
    for j in range(Nc):
        pheromone[i][j]=init_pheromone

for t in range(1,T+1):
     for i in range(Na):
         for j in range(Nc):
             ant[i].tour[j]=j

     for i in range(Na):
        random.shuffle(ant[i].tour)

     for i in range(Na):
         ant[i].city = ant[i].tour[0]
        
         ant[i].probability[0] = 0.0
         
         for j in range(1,Nc):
         
             ant[i].probability[j] = transprob(i, 0, j)
            
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
            d = j+1
            
            for k in range(j+2,Nc):
                    ant[i].probability[k] = transprob(i, d, k)
             
     for i in range(Na):

        ant[i].tourlength = cal_tour_length(i)


     best_tourlength = ant[0].tourlength
     
     for i in range(Na):
    
        if(ant[i].tourlength < best_tourlength):
        
            best_tourlength = ant[i].tourlength
            s = i
        

     for i in range(Nc):
         for j in range(Nc):
             temp_pher[i][j]=0.00
     Q=1000.00

     for j in range(Nc):
        m = j
        n = ant[s].tour[j]
        temp_pher[m][n] = temp_pher[m][n] + (Q/ant[s].tourlength)

     for i in range(Nc):

        for j in range(Nc):
            if i==j:
                pheromone[i][i]=0
            else:
                pheromone[i][i]=(1-rho)* pheromone[i][j] + temp_pher[i][j]
    
     if t==1:
         globalbest_tourlength = best_tourlength
         for i in range(Nc):
             globalbest_position[i] = ant[s].tour[i]
     else:
         if(best_tourlength < globalbest_tourlength):
             globalbest_tourlength = best_tourlength;
             for i in range(Nc):
                 globalbest_position[i] = ant[s].tour[i]
     fp.write("Iteration %d : best %d \n"%(t, best_tourlength))
     for i in range(Nc):
        fp.write(" %d -> "%(ant[s].tour[i]))
     fp.write(" \n\n ")
     print "Best tour length : %d \n"%(best_tourlength)
     fp.write("Iteration %d : global best %d \n"%(t, globalbest_tourlength))
     for i in range(Nc):
        fp.write(" %d -> "%(globalbest_position[i]))
     fp.write(" \n\n ")
     print "Iteration %d : global best %d \n"%(t, globalbest_tourlength)

fp.close()
print 'hello'
    






















      












