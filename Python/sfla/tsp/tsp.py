import random
import math
Nc=52
F=20
memeplex_no=4
T=100
small_iteration=10
part_no=13
part_size=4


class City:
    x=0
    y=0

city=[]
for i in range(Nc):
    a=City()
    city.append(a)

city_dist=[[0 for x in range(Nc)] for y in range(Nc)]
flow=[[0 for x in range(Nc)] for y in range(Nc)]
position_opt=[0  for y in range(Nc)]

def dist(i,j):
    distance = math.sqrt((city[i].x - city[j].x)*(city[i].x - city[j].x)+(city[i].y - city[j].y)*(city[i].y - city[j].y))
    return distance;

class Frog:
    position=[]
    tourlength=0

frog=[]
for i in range(F):
    f=Frog()
    f.position=[0 for i in range(Nc)]
    frog.append(f)

tempos=[]

for i in range(part_no):
    f=Frog()
    f.position=[0 for i in range(Nc)]
    tempos.append(f)

class Memeplex:
    worst=0
    best=0
    no_change_count=0

global_best=0
    
memeplex=[]
for i in range(memeplex_no):
    f=Memeplex()
    
    memeplex.append(f)

def swap(i):
    

    temposi=[0 for x in range(Nc)]
    tempi=frog[i].tourlength;
    for k in range(Nc):
           temposi[k] = frog[i].position[k]
    
    frog[i].tourlength=frog[i+1].tourlength;
    for k in range(Nc):
            frog[i].position[k] = frog[i+1].position[k]
    
    frog[i+1].tourlength=tempi;
    for k in range(Nc):
            frog[i+1].position[k] = temposi[k]

def bubble_sort():
    n=F
    for j in range(1,n):
        for i in range(n-j):
            if(frog[i].tourlength>frog[i+1].tourlength):
                     
                     swap(i)
               
    """for i in range(F):
            print "FROG %d : %d tourlength \n"%( i, frog[i].tourlength)
            for j in range(Nc):
                print "%d \t"%( frog[i].position[j])
            print "\n"
   """
    

def cal_tour_length(j):
    sumt=0.0
    for i in range(Nc-1):
          t = dist(frog[j].position[i], frog[j].position[i+1])
          sumt += t
    return sumt


def cal_tour_length_tempo(j):
      sumt = 0.0
      
      for i in range(Nc-1):
      

          t = dist(frog[j].position[i], frog[j].position[i+1])
          sumt += t
      
      return sumt



#Main

buff=[]
fp = open('testresult.txt', 'w')

size=Nc*Nc
ID=[]
count=0

#read input into ID
with open("input.txt") as f:
    for line in f:
        ID.extend(map(int,line.split()))
cities=[[0 for i in range(2)] for j in range(Nc)]

for i in range(Nc):
      cities[i][0] = ID[3*i+1]
      cities[i][1] = ID[3*i+2]
        

for i in range(Nc):
    city[i].x = cities[i][0]
    city[i].y = cities[i][1]

for i in range(Nc):
    print "city %d :\n x : %d  \n y : %d \n\n\n"%(i, city[i].x, city[i].y)


for i in range(Nc):
    for j in range(i):
        city_dist[i][j] = dist(i,j)
        city_dist[j][i] = city_dist[i][j]
    city_dist[i][i] = 0


"""for i in range(Nc):
    for j in range(Nc):
        print "%0.2f \t"%(city_dist[i][j])
    print "\n\n"
"""

for i in range(F):
    for j in range(Nc):
        frog[i].position[j] = j

    

for i in range(F):
    print " frog %d :\n"%(i)
    random.shuffle(frog[i].position)
    """for j in range(Nc):
        print "%d \t"%(frog[i].position[j])
    print "\n"
    """ 
for i in range(F):
    frog[i].tourlength = cal_tour_length(i)
    print " frog %d tour length: %d\n "%(i, frog[i].tourlength)
    fp.write("initial frog %d tour length: %d\n "%(i, frog[i].tourlength))
    print "\n"
    
for i in range(F):
    print " frog %d tour length: %d \n "%(i, frog[i].tourlength)
    for j in range(Nc):
        """print "%d \t"%(frog[i].position[j])"""
    print "\n"
        

temp_better=0

#read output into IDA
IDA=[]
with open("output.txt") as f:
    for line in f:
        IDA.extend(map(int,line.split()))

print "optimal route \n"
for i in range(Nc):
    print "%d \t"%(IDA[i]-1)

for k in range(Nc):
    tempos[0].position[k] = IDA[k]-1
    tempos[0].tourlength = cal_tour_length_tempo(0)
fp.write("\n \n ORIGINAL OPTIMAL TOURLENGTH %0.2f \n \n "%(tempos[0].tourlength))  

#iterations
def slice_shuffle(a,start,stop):
    b=a[start:stop]
    random.shuffle(b)
    ans=a[:start]+b+a[stop:]
    return ans

for z in range(1,T+1):
    bubble_sort()
   
    for i in range(memeplex_no):
       memeplex[i].no_change_count = 0
   
    for l in range(1,small_iteration):
   
   
        for i in range(memeplex_no):
       
           memeplex[i].best=i
           memeplex[i].worst=i
           kx = i
           xup = F/memeplex_no
           for j in range(1,xup):
           
                kx = kx + memeplex_no
                if (frog[kx].tourlength < frog[memeplex[i].best].tourlength):
                    memeplex[i].best = kx
                if (frog[kx].tourlength > frog[memeplex[i].worst].tourlength):
                    memeplex[i].worst = kx
           
       
       
        global_best = memeplex[0].best
        for s in range(1,memeplex_no):
       
           if(frog[memeplex[s].best].tourlength < frog[global_best].tourlength):
                global_best = memeplex[s].best
        """print "global best : %d \n"%(global_best)"""

        #modification code
        for k in range(Nc):
            for j in range(part_no):
                tempos[j].position[k] = frog[global_best].position[k]
            
        for j in range(part_no):
            slice_shuffle(tempos[j].position,j*part_size,(j+1)*part_size)

        for j in range(part_no):
            tempos[j].tourlength = cal_tour_length_tempo(j)
        
        print "global best : %0.2f \n MODIFIED 1: %0.2f , 2: %0.2f , 3: %0.2f , 4: %0.2f \n\n"%(frog[global_best].tourlength, tempos[0].tourlength, tempos[1].tourlength, tempos[2].tourlength, tempos[3].tourlength)

        temp_better=tempos[0].tourlength
        temp_better_index=0
        for h in range(1,part_no):
            if tempos[h].tourlength<temp_better:
                temp_better = tempos[h].tourlength
                temp_better_index=h
            
        if temp_better<frog[global_best].tourlength:
            for k in range(Nc):
                frog[global_best].position[k] = tempos[temp_better_index].position[k]
            frog[global_best].tourlength  = temp_better

        for k in range(Nc):
            tempos[0].position[k] = frog[global_best].position[k]

        xchng=0
        k=0
        while k<Nc/2:
            xchng = frog[global_best].position[k]
            frog[global_best].position[k] = frog[global_best].position[Nc-1-k]
            frog[global_best].position[Nc-1-k] = xchng
            temp_better = cal_tour_length(global_best)
            if temp_better<frog[global_best].tourlength:
                frog[global_best].tourlength=temp_better
            else:
                xchng = frog[global_best].position[k]
                frog[global_best].position[k] = frog[global_best].position[Nc-1-k]
                frog[global_best].position[Nc-1-k] = xchng
            k+=1
        
        


        #hill climb approach
        rn=random.randrange(0,Nc)
        for i in range(Nc):
            if i!=rn:
                xchng = frog[global_best].position[rn]
                frog[global_best].position[rn] = frog[global_best].position[i]
                frog[global_best].position[i] = xchng
                temp_better = cal_tour_length(global_best)
                if temp_better < frog[global_best].tourlength:
                    frog[global_best].tourlength = temp_better
                else:
                    xchng = frog[global_best].position[rn]
                    frog[global_best].position[rn] = frog[global_best].position[i]
                    frog[global_best].position[i] = xchng
    
        for i in range(memeplex_no):
            if(memeplex[i].no_change_count < 5): 
       
               for k in range(Nc):
                 for j in range(part_no):
                   tempos[j].position[k] = frog[global_best].position[k]
               
       
               for j in range(part_no):
                 slice_shuffle(tempos[j].position,j*part_size,(j+1)*part_size)
               
               for j in range(part_no):
                 tempos[j].tourlength = cal_tour_length_tempo(j)
               
               temp_better = tempos[0].tourlength
               temp_better_index = 0
               for h in range(1,part_no):
                 if(tempos[h].tourlength < temp_better ):
                       temp_better = tempos[h].tourlength
                       temp_better_index=h
               
      
               if(temp_better < frog[memeplex[i].worst].tourlength):
               
                   for k in range(Nc):
                     frog[memeplex[i].worst].position[k]=tempos[temp_better_index].position[k]
                     frog[memeplex[i].worst].tourlength=temp_better
               
               else:
                   memeplex[i].no_change_count+=1
            else:

               for k in range(Nc):
                    for j in range(part_no):
                         tempos[j].position[k] = frog[global_best].position[k]
               
      
               for j in range(part_no):
                   slice_shuffle(tempos[j].position,j*part_size,(j+1)*part_size)
               for j in range(part_no):
                   tempos[j].tourlength = cal_tour_length_tempo(j)
               
               temp_better = tempos[0].tourlength
               temp_better_index = 0
               for h in range(1,part_no):
                if(tempos[h].tourlength < temp_better ):
                   temp_better = tempos[h].tourlength
                   temp_better_index=h
               
               
               if(temp_better < frog[memeplex[i].worst].tourlength):
               
                   for k in range(Nc):
                     frog[memeplex[i].worst].position[k]=tempos[temp_better_index].position[k]
                     frog[memeplex[i].worst].tourlength=temp_better
               

                 

    
    #onlooker phase end
    print "Iteration %d : global best %0.2f \n"%(z, frog[global_best].tourlength)
    fp.write( "Iteration %d : global best %0.2f \n"%(z, frog[global_best].tourlength))

fp.close()

print "hello"
lx=raw_input()
lx=int(lx)































