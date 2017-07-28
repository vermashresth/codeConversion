import random

Nc=16
T=50
no_change_count_limit=5
part_no=4
part_size=4
B=30

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

class Emp_Bee:
    position=None
    tourlength=0
    no_change_count=0

empbee=[]
for i in range(B):
    a=Emp_Bee()
    a.position=[0 for x in range(Nc)]
    empbee.append(a)

class Onlkr_Bee:
    global_best_position=None
    global_best_tourlength=0

obee=Onlkr_Bee()
obee.global_best_position=[0 for x in range(Nc)]

tempos=[]
for i in range(part_no):
    a=Emp_Bee()
    a.position=[0 for x in range(Nc)]
    tempos.append(a)

def cal_tour_length(k):
    sum=0
    for i in range(Nc):
        for j in range(Nc):
            p1=empbee[k].position[i]
            p2=empbee[k].position[j]
            sum=sum+(flow[p1][p2]*city_dist[i][j])
    return sum

def cal_tour_length_tempo(k):
    sum=0
    for i in range(Nc):
        for j in range(Nc):
            p1=tempos[k].position[i]
            p2=tempos[k].position[j]
            sum=sum+(flow[p1][p2]*city_dist[i][j])
    return sum

def cal_tour_length_globalbest():
    sum=0
    for i in range(Nc):
        for j in range(Nc):            
            p1 = obee.global_best_position[i];
            p2 = obee.global_best_position[j];
            sum = sum + (flow[p1][p2] * city_dist[i][j])
    return sum


#Main

buff=[]
fp = open('testresult.txt', 'w')

size=Nc*Nc
ID=[]
count=0

#read input1 into ID
with open("input1.txt") as f:
    for line in f:
        ID.extend(map(int,line.split()))

for i in range(Nc):
    for j in range(Nc):
        city_dist[i][j]=ID[(Nc*i)+j]
        

for i in range(Nc):
    for j in range(Nc):
        print city_dist[i][j],
    print

#read input2 into ID        
ID=[]
with open("input2.txt") as f:
    for line in f:
        ID.extend(map(int,line.split()))

for i in range(Nc):
    for j in range(Nc):
        flow[i][j]=ID[(Nc*i)+j]
        
print

for i in range(Nc):
    for j in range(Nc):
        print flow[i][j],
    print



#GIVEN OPTIMAL SOLUTON FOR QAP

size_opt=Nc
ID0=[]
count_opt=0

#read input3 into ID0
with open("input3.txt") as f:
    for line in f:
        ID0.extend(map(int,line.split()))

for i in range(Nc):
    position_opt[i]=ID0[i]-1


fp.write(" optimal route \n")
for i in range(Nc):
    fp.write("%3d" % (position_opt[i]))
fp.write(" \n ")

sum=0
for i in range(Nc):
    for j in range(Nc):
        p1=position_opt[i]
        p2=position_opt[j]
        sum=sum+(flow[p1][p2]*city_dist[i][j])
fp.write("original optimal solution %d \n \n " %(sum))


for i in range(B):
    for j in range(Nc):
        empbee[i].position[j]=j

for i in range(B):
    print " Bee %d :\n"%(i)
    random.shuffle(empbee[i].position)
    for j in range(Nc):
        print "%d \t"%(empbee[i].position[j])
    print "\n"
        
for i in range(B):
    empbee[i].tourlength = cal_tour_length(i)
    print " bee %d tour length: %d\n "%(i, empbee[i].tourlength)
    fp.write("Initial bee %d tour length: %d\n "%(i, empbee[i].tourlength))
    print "\n"
    
for i in range(B):
    print " bee %d tour length: %d \n "%(i, empbee[i].tourlength)
    for j in range(Nc):
        print "%d \t"%(empbee[i].position[j])
    print "\n"
        

temp_better=0
for i in range(B):
    empbee[i].no_change_count=0
for k in range(Nc):
    obee.global_best_position[k] = empbee[0].position[k]
obee.global_best_tourlength = empbee[0].tourlength


#iterations
def slice_shuffle(a,start,stop):
    b=a[start:stop]
    random.shuffle(b)
    ans=a[:start]+b+a[stop:]
    return ans

for z in range(1,T+1):
    for i in range(B):
         if empbee[i].no_change_count > no_change_count_limit: # scout bee phase
            empbee[i].no_change_count = 0
            random.shuffle(empbee[i].position)
         else:
            for k in range(Nc):
                for j in range(part_no):
                    tempos[j].position[k] = empbee[i].position[k]

            for j in range(part_no):
                slice_shuffle(tempos[j].position,j*part_size,(j+1)*part_size)
            

            for j in range(part_no):
                tempos[j].tourlength=cal_tour_length_tempo(j)

            temp_better=tempos[0].tourlength
            temp_better_index=0
            for h in range(part_no):
                if tempos[h].tourlength<temp_better:
                    temp_better=tempos[h].tourlength
                    temp_better_index=h
            
            if temp_better<empbee[i].tourlength:
                for k in range(Nc):
                    empbee[i].position[k]=tempos[temp_better_index].position[k]
                empbee[i].tourlength=temp_better
            else:
                empbee[i].no_change_count+=1

         if empbee[i].tourlength<obee.global_best_tourlength:
            for k in range(Nc):
                 obee.global_best_position[k]=empbee[i].position[k]
            obee.global_best_tourlength=empbee[i].tourlength

    #onlooker bee phase
    for k in range(Nc):
        for j in range(part_no):
            tempos[j].position[k] = obee.global_best_position[k]
        
    for j in range(part_no):
        slice_shuffle(tempos[j].position,j*part_size,(j+1)*part_size)

    for j in range(part_no):
        tempos[j].tourlength = cal_tour_length_tempo(j)
    


    temp_better=tempos[0].tourlength
    temp_better_index=0
    for h in range(1,part_no):
        if tempos[h].tourlength<temp_better:
            temp_better = tempos[h].tourlength
            temp_better_index=h
        
    if temp_better<obee.global_best_tourlength:
        for k in range(Nc):
            obee.global_best_position[k] = tempos[temp_better_index].position[k]
        obee.global_best_tourlength  = temp_better
        

    xchng=0
    k=0
    while k<Nc/2:
        xchng = obee.global_best_position[k]
        obee.global_best_position[k] = obee.global_best_position[Nc-1-k]
        obee.global_best_position[Nc-1-k] = xchng
        temp_better = cal_tour_length_globalbest()
        if temp_better<obee.global_best_tourlength:
            obee.global_best_tourlength=temp_better
        else:
            xchng = obee.global_best_position[k]
            obee.global_best_position[k] = obee.global_best_position[Nc-1-k]
            obee.global_best_position[Nc-1-k] = xchng
        k+=1
    
    fp.write( "Iteration %d : global best before hill climbing %d \n"%( z, obee.global_best_tourlength))


    #hill climb approach
    rn=random.randrange(0,Nc)
    for i in range(Nc):
        if i!=rn:
            xchng = obee.global_best_position[rn]
            obee.global_best_position[rn] = obee.global_best_position[i]
            obee.global_best_position[i] = xchng
            temp_better = cal_tour_length_globalbest()
            if temp_better < obee.global_best_tourlength:
                obee.global_best_tourlength = temp_better
            else:
                xchng = obee.global_best_position[rn]
                obee.global_best_position[rn] = obee.global_best_position[i]
                obee.global_best_position[i] = xchng
    
    
    #onlooker phase end
    print "Iteration %d : global best %d \n"%(z, obee.global_best_tourlength)
    fp.write( "Iteration %d : global best after hill climbing %d \n"%(z, obee.global_best_tourlength))

fp.close()

print "hello"
lx=raw_input()
lx=int(lx)































