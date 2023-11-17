import numpy as np
import pandas as pd

def kernel(x:np.ndarray, y:np.ndarray):
    gamma = 0.1
    diff = x - y
    return np.exp(-gamma * np.dot(diff, diff))

def initialize_centroids(X:pd.DataFrame, K):
    # centroids = X.sample(n=K).reset_index(drop=True)
    centroids = X.iloc[0:K]
    return centroids

def assign_to_clusters(X:pd.DataFrame, centroids:pd.DataFrame):
    distances = np.zeros((len(centroids), len(X)))
    for i, centroid in centroids.iterrows():
        distances[i] = np.linalg.norm(X - centroid.values, axis=1)
    labels = np.argmin(distances, axis=0)
    return labels

def update_centroids(X:pd.DataFrame, labels:np.ndarray, K):
    centroids = np.zeros((K, len(X.iloc[0])))
    for k in range(K):
        centroids[k] = X[labels == k].mean()
    return pd.DataFrame(centroids, columns=X.columns)

def k_means(X, K, max_iterations=100):
    centroids = initialize_centroids(X, K)

    for iteration in range(max_iterations):
        labels = assign_to_clusters(X, centroids)
        new_centroids = update_centroids(X, labels, K)
        if np.allclose(new_centroids, centroids, rtol=0.1):
            print(f"[k_means] Done at iteration#{iteration}")
            break
        centroids = new_centroids
    return labels