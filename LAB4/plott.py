import numpy as np
import matplotlib.pyplot as plt 

data ={"Average Response Time" : {"SRTF" : [ 129.714, 52.2222, 435.167], "SJF" : [ 324.571, 97.111, 424.583]}, 
       "Average Turnaround Time" : {"SRTF" : [612.714, 168.667, 945.5], "SJF" : [755.286 , 237.111, 1026.33]}, 
       "Average Penalty Ratio" : {"SRTF" : [2.65334, 4.81375, 4.81162], "SJF" : [6.91622, 8.81763, 5.1408]},
       "System throughput" : {"SRTF" : [0.00489853, 0.018595, 0.00573888], "SJF" : [0.00489853, 0.0186722, 0.00574438]},}

for key in data:
    X = ['Process1','Process2','Process3']
    SJF = data[key]['SJF']
    SRTF = data[key]['SRTF']
    X_axis = np.arange(len(X))
    
    plt.bar(X_axis - 0.2, SJF, 0.4, label = 'SJF', color='red')
    plt.bar(X_axis + 0.2, SRTF, 0.4, label = 'SRTF', color='blue')
    
    plt.xticks(X_axis, X)
    plt.xlabel("Process Data No.")
    plt.ylabel(key)
    plt.title(""+key)
    plt.legend()
    # plt.savefig("" + key + ".png")
    plt.show()