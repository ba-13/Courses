import numpy as np
import pandas as pd
from utils import kernel, k_means
import os
import matplotlib.pyplot as plt

folder_name = "weird_kmeans_figs"
os.makedirs(f"./{folder_name}", exist_ok=True)
df = pd.read_csv("./kmeans_data.txt", delim_whitespace=True, header=None)
n = len(df)
L = 1

def kernelized_vector(XL:pd.DataFrame, row:np.ndarray):
    Kvec = np.zeros((L))
    for i in range(L):
        Kvec[i] = kernel(XL.iloc[i].values, row)
    return Kvec

if __name__ == "__main__":
    for k in range(10):
        XL = df.sample(n=L).reset_index(drop=True)
        X = np.zeros((n, L))
        for i in range(n):
            X[i] = kernelized_vector(XL, df.iloc[i].values)
        X = pd.DataFrame(X)
        labels = k_means(X, 2)
        df_1 = df[labels == 1]
        df_0 = df[labels == 0]
        plt.figure()
        plt.scatter(df_0[0], df_0[1], c='r', s=1)
        plt.scatter(df_1[0], df_1[1], c='g', s=1)
        plt.scatter(XL[0], XL[1], c="b", s=200, marker='x')
        plt.savefig(f"./{folder_name}/{k}.png")
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')
        ax.scatter(df_0[0], df_0[1], X[labels == 0], c='r', marker='o')
        ax.scatter(df_1[0], df_1[1], X[labels == 1], c='g', marker='o')
        ax.scatter(XL[0], XL[1], kernelized_vector(XL, XL.iloc[0].values), c="b", s=200, marker='x')
        fig.savefig(f"./{folder_name}/3d_{k}.png")
    print(f"Check {folder_name} for figures")