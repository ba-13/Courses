import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from utils import kernel
import os
from sklearn.metrics import mean_squared_error

folder_name = "landmark_ridge_figs"
os.makedirs(f"./{folder_name}", exist_ok=True)
df = pd.read_csv("./ridgetrain.txt", delim_whitespace=True, header=None)
df_test = pd.read_csv("./ridgetest.txt", delim_whitespace=True, header=None)
n = len(df)
nt = len(df_test)

X = df[0]
y = df[1]
Xt = df_test[0]
yt = df_test[1]

class Landmark_Ridge():

    def __init__(self, L) -> None:
        global X
        self.L = L
        self.XL = np.random.choice(X, L)
        self.Xtilde = self.calculate_Xtilde()
        self.K = self.calculate_XtransposeX(self.Xtilde)
        self.w = self.model()

    def kernelized_vector(self, x:np.ndarray):
        Kvec = np.zeros((self.L))
        for i in range(self.L):
            Kvec[i] = kernel(self.XL[i], x)
        return Kvec

    def calculate_Xtilde(self):
        global n
        global X
        Xtilde = np.zeros((n, self.L))
        for i in range(n):
            Xtilde[i] = self.kernelized_vector(X[i])
        return Xtilde

    def calculate_XtransposeX(self, Xtilde):
        global n
        K = Xtilde.T @ Xtilde
        return K

    def model(self, Lambda=0.1):
        """Generates predictions

        Args:
            K (np.ndarray): X^T X
            x (np.ndarray): test input
            Lambda (_type_): Regularizar hyperparameter

        Returns:
            y: test prediction
        """
        global n
        global y
        mid_mat = np.linalg.inv(self.K + Lambda * np.eye(self.L))
        return mid_mat @ self.Xtilde.T @ y

    def predict(self, x:np.ndarray):
        xtilde = self.kernelized_vector(x)
        return np.dot(self.w, xtilde)

if __name__ == "__main__":
    for L in [2, 5, 20, 50, 100]:
        lr = Landmark_Ridge(L)
        ypred = np.zeros(nt)
        for i in range(nt):
            ypred[i] = lr.predict(Xt[i])
        plt.figure(figsize=(12, 7))
        plt.scatter(Xt, ypred, c="red", s=1)
        plt.scatter(Xt, yt, c="blue", s=1)
        plt.savefig(f"./{folder_name}/L_{L}_.png")
        mse = mean_squared_error(yt, ypred)
        rmse = np.sqrt(mse)
        print(f"L: {L}\tRMSE: {rmse:.3f}")
    print(f"Check ./{folder_name} for figures")