import subprocess
import matplotlib.pyplot as plt
import sys

x = 120
z = 120
y_values = range(int(sys.argv[2]), int(sys.argv[3]))
results = {
    "FIFO": [],
    "LRU": [],
    "Random": [],
}

for y in range(int(sys.argv[2]), int(sys.argv[3])):
    cmd = ['fifo.exe', str(x), str(y), str(z), sys.argv[1]]
    output = subprocess.check_output(cmd, shell=True)
    # print(output.decode('utf-8'))
    percentages = output.decode().strip().split()
    results["FIFO"].append(float(percentages[0]))
    # results["LRU"].append(float(percentages[1]))
    # results["Random"].append(float(percentages[2]))
for y in range(int(sys.argv[2]), int(sys.argv[3])):
    cmd = ['lru.exe', str(x), str(y), str(z), sys.argv[1]]
    output = subprocess.check_output(cmd, shell=True)
    # print(output.decode('utf-8'))
    percentages = output.decode().strip().split()
    # results["FIFO"].append(float(percentages[0]))
    results["LRU"].append(float(percentages[0]))
    # results["Random"].append(float(percentages[2]))
for y in range(int(sys.argv[2]), int(sys.argv[3])):
    cmd = ['random.exe', str(x), str(y), str(z), sys.argv[1]]
    output = subprocess.check_output(cmd, shell=True)
    # print(output.decode('utf-8'))
    percentages = output.decode().strip().split()
    # results["FIFO"].append(float(percentages[0]))
    # results["LRU"].append(float(percentages[1]))
    results["Random"].append(float(percentages[0]))

plt.plot(y_values, results["FIFO"], label="FIFO")
plt.plot(y_values, results["LRU"], label="LRU")
plt.plot(y_values, results["Random"], label="Random")
plt.xlabel("No. of frames in memory")
plt.ylabel("Page faults")
plt.title(f"FIFO v/s LRU v/s Random") #(x={x}, z={z})
plt.legend()
plt.show()