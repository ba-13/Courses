import torch
import torch.utils.data as data
import pandas as pd
import os
import scipy.io
import numpy as np


class ToTensor_(object):
    def __call__(self, sample):
        vec, label = sample['input'], sample['label']
        return {'input': torch.from_numpy(vec), 'label': torch.from_numpy(label)}


class FOADataset(data.Dataset):
    def __init__(self, csv_file, root_dir, transform=None) -> None:
        super(FOADataset, self).__init__()
        self.doa_labels = pd.read_csv(csv_file)
        self.root_dir = root_dir
        self.transform = transform

    def __len__(self):
        return len(self.doa_labels)

    def __getitem__(self, idx):
        if torch.is_tensor(idx):
            idx = idx.tolist()

        file_ = os.path.join(self.root_dir, self.doa_labels.iloc[idx, 1])
        file_ = scipy.io.loadmat(file_)
        I = file_["I"]
        # I is of form 6x512x38
        # I = I.transpose((2, 1, 0))

        doa_label = self.doa_labels.iloc[idx, -1]
        doa_ = np.zeros(36*19)
        doa_[doa_label] = 1
        doa_label = np.array(doa_).astype('float')
        sample = {'input': I, 'label': doa_label}

        if self.transform:
            sample = self.transform(sample)

        return sample


if __name__ == '__main__':
    doa_dataset = FOADataset(
        csv_file="../data-gen/labels.csv", root_dir='../data/generated/', transform=ToTensor_())
    test_sample = doa_dataset[12]
    print(test_sample)
    # dataloader = data.DataLoader(doa_dataset, batch_size=4,
    #                              shuffle=True, num_workers=0)
