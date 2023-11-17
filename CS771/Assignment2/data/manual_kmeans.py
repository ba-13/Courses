import os
import numpy as np
import pandas as pd
from utils import k_means
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

folder_name = "manual_kmeans_figs"
os.makedirs(f"./{folder_name}", exist_ok=True)
df = pd.read_csv("./kmeans_data.txt", delim_whitespace=True, header=None)

def hand_crafted_feature(row:pd.Series):
    row = row.to_numpy()
    return np.dot(row, row)

if __name__ == "__main__":
    df[2] = df.apply(hand_crafted_feature, axis=1)
    labels = k_means(df, 2)
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    df_1 = df[labels == 1]
    df_0 = df[labels == 0]
    ax.scatter(df_0[0], df_0[1], df_0[2], c='r', marker='o')
    ax.scatter(df_1[0], df_1[1], df_1[2], c='g', marker='o')
    plt.savefig(f"./{folder_name}/3d.png")
    plt.figure()
    plt.scatter(df_0[0], df_0[1], s=1, c='r')
    plt.scatter(df_1[0], df_1[1], s=1, c='g')
    plt.savefig(f"./{folder_name}/2d.png")
    print(f"Check {folder_name} for figures")
