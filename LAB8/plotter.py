import matplotlib.pyplot as plt
import os
import sys
os.system("g++ fifo.cpp -o fifo")
os.system("g++ lru.cpp -o lru")
os.system("g++ random.cpp -o random")
mem=[]
fifo=[]
lru=[]
random=[]
for x in range (1,60):
    st=" 60 "+str(x)+" 60 "+ sys.argv[1]
    mem.append(x)
    str1="./fifo"+st
    str2="./lru"+st
    str3="./random"+st
    os.system(str1)
    #take input from file a add to list
    f = open('output.txt', 'r')
    for line in f:
        fifo.append(int(line))
    f.close()
    os.system(str2)
    #take input from file a add to list
    f = open('output.txt', 'r')
    for line in f:
        lru.append(int(line))
    f.close()
    os.system(str3)
    f = open('output.txt', 'r')
    for line in f:
        random.append(int(line))
    f.close()
    
plt.plot(mem,fifo,label="FIFO")
plt.plot(mem,lru,label="LRU")
plt.plot(mem,random,label="RANDOM")
plt.xlabel('no of frames in memory')
plt.ylabel('page faults')
plt.title('Page faults v/s no of frames in memory')
plt.legend()
plt.show()