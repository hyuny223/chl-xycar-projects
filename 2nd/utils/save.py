import pandas as pd
import numpy as np
import matplotlib.pyplot as plt



def save_pos(line_tmp):
    line = pd.DataFrame(line_tmp)
    line = line.transpose()
    line.to_csv('/home/chl/Documents/line.csv', header=False, index=False)

    ############################# 그래프 작성 #############################
    ####################################################################
    dp1 = pd.read_csv("/home/chl/Documents/a.csv")
    dp2 = pd.read_csv("/home/chl/Documents/line.csv")

    plt.figure(figsize=(100,12))
    plt.plot(dp1['index'], dp1['new lposl'], label="lposl_line")
    plt.plot(dp1['index'], dp1['new lposr'], label="lposr_line")
    plt.scatter(dp2['index'], dp2['lpos'], label="lpos")
    plt.legend()
    plt.xticks(np.arange(0,109,1))

    plt.xlabel("index")
    plt.ylabel("lpos")
    plt.title("Tracking Lpos")

    plt.show()

    plt.figure(figsize=(100,12))
    plt.plot(dp1['index'], dp1['new rposl'], label="rposl_line")
    plt.plot(dp1['index'], dp1['new rposr'], label="rposr_line")
    plt.scatter(dp2['index'], dp2['rpos'], label="rpos")
    plt.legend()
    plt.xticks(np.arange(0,109,1))

    plt.xlabel("index")
    plt.ylabel("rpos")
    plt.title("Tracking Rpos")

    plt.show()
