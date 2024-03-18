import subprocess
import sys
import matplotlib.pyplot as plt

argc = len(sys.argv)

if argc != 3:
    print("Usage: python3 graph_plot.py <number of pages> <filename>")
    exit(1)

num_pages = int(sys.argv[1])
filename = sys.argv[2]

yFIFO = []
yLRU = []
yRandom = []

# pages = [10, 20, 40, 80, 100]

x = []

for i in range(1, num_pages+1):
    x.append(i)
    FIFO = subprocess.run(["./fifo", str(num_pages), str(i),
                           str(num_pages), filename], stdout=subprocess.PIPE)
    FIFO = FIFO.stdout.decode('utf-8').split(" ")[2]
    yFIFO.append(int(FIFO))

    LRU = subprocess.run(["./lru", str(num_pages), str(i),
                          str(num_pages), filename], stdout=subprocess.PIPE)
    LRU = LRU.stdout.decode('utf-8').split(" ")[2]
    yLRU.append(int(LRU))

    Random = subprocess.run(["./random", str(num_pages), str(i),
                             str(num_pages), filename], stdout=subprocess.PIPE)
    Random = Random.stdout.decode('utf-8').split(" ")[2]
    yRandom.append(int(Random))

plt.plot(x, yFIFO, label="FIFO")
plt.plot(x, yLRU, label="LRU")
plt.plot(x, yRandom, label="Random")

plt.xlabel("Number of Frames")
plt.ylabel("Page Faults")
plt.title("Page Replacement Algorithms")
plt.legend()
plt.show()
