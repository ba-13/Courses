import numpy as np
from utils_ import accuracy, get_seen_means, inference, base_path

def calculate_unseen_means_convex(ca_unseen, ca_seen, mus_seen):
    mus_new = np.zeros((ca_unseen.shape[0], mus_seen.shape[1]))
    for i, cau in enumerate(ca_unseen):
        total_dot_products = 0
        for j, ca in enumerate(ca_seen):
            dot_product = np.dot(ca, cau)
            total_dot_products += dot_product
            mus_new[i] += dot_product * mus_seen[j] # convex combination
        mus_new[i] /= total_dot_products # normalize
    return mus_new

if __name__ == "__main__":
    mus = get_seen_means()
    class_attributes_seen = np.load(base_path + 'class_attributes_seen.npy')
    class_attributes_unseen = np.load(base_path + 'class_attributes_unseen.npy')
    Xtest = np.load(base_path + 'Xtest.npy')
    Ytest = np.load(base_path + 'Ytest.npy')
    mus_new = calculate_unseen_means_convex(class_attributes_unseen, 
                                            class_attributes_seen, mus)
    ypred = inference(Xtest, mus_new)
    acc = accuracy(ypred, Ytest)
    print(f"[{__file__}] Accuracy of LwP for unknown classes: {100*acc:.2f}%")
