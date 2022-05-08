from turtle import forward
from unicodedata import bidirectional
import torch
import torch.nn.functional as F
import torch.nn as nn


class BidirectionalLSTM(nn.Module):
    def __init__(self, nIn, nHidden, nOut) -> None:
        super(BidirectionalLSTM, self).__init__()
        self.rnn = nn.LSTM(nIn, nHidden, bidirectional=True)
        self.embedding = nn.Linear(nHidden*2, nOut)

    def forward(self, input):
        recurrent, _ = self.rnn(input)
        T, b, h = recurrent.size()
        t_rec = recurrent.view(T*b, h)
        output = self.embedding(t_rec)
        output = output.view(T, b, -1)
        return output


class NeuralNet(nn.Module):
    def __init__(self, nIn=6) -> None:
        super(NeuralNet, self).__init__()
        # 6x513x38
        self.conv1 = nn.Conv2d(nIn, 64, kernel_size=3,
                               padding=(2, 1), stride=1)
        # 64x513x38
        self.pool1 = nn.MaxPool2d((8, 1), stride=(4, 1))
        self.conv2 = nn.Conv2d(64, 64, kernel_size=3, padding=(2, 1), stride=1)
        self.pool2 = nn.MaxPool2d((8, 1), stride=(4, 1))
        self.conv3 = nn.Conv2d(64, 64, kernel_size=3, padding=(2, 1), stride=1)
        self.pool3 = nn.MaxPool2d((4, 1), stride=(2, 1))
        self.lstm1 = nn.LSTM(
            570, 64, num_layers=2, dropout=0.2, bidirectional=True)
        self.ff1 = nn.Linear(64*2, 100)
        self.ff2 = nn.Linear(100, 684)

    def forward(self, x):
        out = F.relu(self.conv1(x))
        out = self.pool1(out)
        # print(out.shape)
        out = F.relu(self.conv2(out))
        out = self.pool2(out)
        # print(out.shape)
        out = F.relu(self.conv3(out))
        out = self.pool3(out)
        # print(out.shape)
        out = torch.reshape(out, (4, 64, -1))
        # print(out.shape)
        out, _ = self.lstm1(out)
        # print(out.shape)
        out = self.ff1(out)
        out = torch.sigmoid(self.ff2(out))
        # print(out.shape)
        return out
