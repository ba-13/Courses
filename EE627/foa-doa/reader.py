import scipy.io
import numpy as np
import matplotlib.pyplot as plt
import os


def read_mat_generated(path):
    mat = scipy.io.loadmat(path)
    d = {}
    I = mat["I"]
    print(type(I))
    sz = I.shape
    d["mic"] = mat["r"][0]
    d["src"] = mat["s"][0]
    d["room_dim"] = mat["L"][0]
    d["reverb"] = mat["beta"][0][0]
    d["az"] = mat["az"][0][0]
    d["el"] = mat["el"][0][0]
    I = I.transpose((2, 1, 0))
    return I, d


if __name__ == '__main__':
    path = '../data/generated/'
    # dir_ = os.listdir(path)
    # for l in dir_:
    # path_ = path + l
    # I, meta = read_mat_generated(path_)
    # print(meta)
    # print(len(dir_))
    I, meta = read_mat_generated("../data/sample.mat")
    print(I.shape)
    # plt.imshow(I[:, :, 0])
    # plt.gray()
    # plt.show()
