import pandas as pd
import numpy as np
from reader import read_mat_generated
import os

data_folder_path = './data/generated/'
files = os.listdir(data_folder_path)
final = []
for file in files:
    I, data = read_mat_generated(data_folder_path + file)

    az = np.floor(data["az"]*180/np.pi)
    az = np.round(az/10)*10
    el = np.floor(data["el"]*180/np.pi)
    el = np.round(el/10)*10
    # grid formed by (0 to 35)*19 + (0 to 19)
    if az+180 == 360:
        az = 0
    label = int((az+180)/10 * 19 + (el+90)/10)

    tmp = [file, az, el, label]
    print(tmp)
    final.append(tmp)

df = pd.DataFrame(final)
df.to_csv('./labels.csv')
