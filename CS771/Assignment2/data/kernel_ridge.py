import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from utils import kernel
import os
from sklearn.metrics import mean_squared_error

folder_name = "kernel_ridge_figs"
os.makedirs(f"./{folder_name}", exist_ok=True)
df = pd.read_csv("./ridgetrain.txt", delim_whitespace=True, header=None)
df_test = pd.read_csv("./ridgetest.txt", delim_whitespace=True, header=None)
n = len(df)
nt = len(df_test)

X = df[0]
y = df[1]
Xt = df_test[0]
yt = df_test[1]

def kernelized_vector(x:np.ndarray):
    global n
    global X
    Kvec = np.zeros((n))
    for i in range(n):
        Kvec[i] = kernel(X[i], x)
    return Kvec

def model(K:np.ndarray, x:np.ndarray, Lambda):
    """Generates predictions

    Args:
        K (np.ndarray): Kernel matrix on training inputs
        x (np.ndarray): test input
        Lambda (_type_): Regularizar hyperparameter

    Returns:
        y: test prediction
    """
    global n
    global y
    mid_mat = np.linalg.inv(K + Lambda * np.eye(n))
    Kvec = kernelized_vector(x)
    return np.dot(y, mid_mat @ Kvec)

def kernelized_matrix():
    global n
    global X
    K = np.zeros((n, n))
    for i in range(n):
        for j in range(n):
            K[i][j] = kernel(X[i], X[j])
    return K

if __name__ == "__main__":
    K = kernelized_matrix()
    for Lambda in [0.1, 1, 10, 100]:
        ypredt = np.zeros(nt)
        for i in range(nt):
            ypredt[i] = model(K, Xt[i], Lambda)
        plt.figure(figsize=(12, 7))
        plt.scatter(Xt, ypredt, c="red", s=1)
        plt.scatter(Xt, yt, c="blue", s=1)
        plt.savefig(f"./{folder_name}/Lambda_{Lambda}_.png")
        mse = mean_squared_error(yt, ypredt)
        rmse = np.sqrt(mse)
        print(f"Lambda: {Lambda}\tRMSE: {rmse:.3f}")
    print(f"Check ./{folder_name} for figures")