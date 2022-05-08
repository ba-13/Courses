import torch
import torch.nn as nn
import torch.utils.data as data
from dataloader import FOADataset, ToTensor_
from model import NeuralNet
import numpy as np

num_epochs = 1
batch_size = 4
learning_rate = 0.01

if __name__ == '__main__':
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

    doa_dataset = FOADataset(
        csv_file="../labels.csv", root_dir='../data/generated/', transform=ToTensor_())

    dataloader = data.DataLoader(doa_dataset, batch_size=4,
                                 shuffle=True, num_workers=4)

    model = NeuralNet().to(device)

    criterion = nn.CrossEntropyLoss()
    optimizer = torch.optim.NAdam(model.parameters(), lr=learning_rate)

    n_total_steps = len(dataloader)

    print("Starting Training...")
    for epoch in range(num_epochs):
        for i, n in enumerate(dataloader):
            images = n["input"]
            labels = n["label"].type(torch.LongTensor)
            images = images.to(device)
            # print(labels.shape)
            labels = labels.to(device)

            outputs = model(images.float())
            # print(outputs.shape)
            loss = criterion(outputs, labels)

            optimizer.zero_grad()
            loss.backward()
            optimizer.step()

            if (i + 1) % (batch_size * n_total_steps / 10) == 0:
                print(
                    f"Epoch [{epoch+1}/{num_epochs}] - Step [{i+1}/{n_total_steps}] : Loss = {loss.item():.4f}"
                )
    print("Training Done.")
    PATH = "./crnn.pth"
    torch.save(model.state_dict(), PATH)
