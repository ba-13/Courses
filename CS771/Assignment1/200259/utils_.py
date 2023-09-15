import numpy as np

base_path = "../data/AwA_python/"

def distance(vec1:np.array, vec2:np.array):
    diff = vec1 - vec2
    return np.dot(diff, diff)

def accuracy(pred:np.array, actual:np.array):
    barr = pred == actual
    return barr.sum() / len(barr)

def get_seen_means():
    X_seen = np.load(base_path + "X_seen.npy", allow_pickle=True, 
                     encoding="bytes")
    mus = np.zeros((X_seen.shape[0], X_seen[0].shape[1])) # 40 x 4096
    for i, cl in enumerate(X_seen):
        mus[i] = np.mean(cl, axis=0) # calculate prototypes
    del X_seen
    return mus

def inference(Xtest, mus):
    ypred = np.zeros((Xtest.shape[0], 1))
    for i, xt in enumerate(Xtest):
        min_dist = np.inf
        cls = -1
        for j, cl in enumerate(mus): # iterate through all means
            dist = distance(xt, cl)
            if dist < min_dist:
                cls = j + 1
                min_dist = dist
        ypred[i][0] = cls # find least distant mean
    return ypred