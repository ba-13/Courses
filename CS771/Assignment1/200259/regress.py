import numpy as np
from utils_ import accuracy, get_seen_means, inference, base_path

Lambda = [0.01, 0.1, 1, 10, 20, 50, 100]

def calculate_unseen_means_regress(ca_unseen, ca_seen, mus_seen, lambda_):
    X = ca_seen
    Y = mus_seen
    W = np.linalg.inv(X.T @ X + lambda_ * np.eye(ca_seen.shape[1])) @ X.T @ Y
    mus_new = ca_unseen @ W
    return mus_new

if __name__ == "__main__":
    mus = get_seen_means()
    class_attributes_seen = np.load(base_path + 'class_attributes_seen.npy')
    class_attributes_unseen = np.load(base_path + 'class_attributes_unseen.npy')
    Xtest = np.load(base_path + 'Xtest.npy')
    Ytest = np.load(base_path + 'Ytest.npy')

    best_accuracy = (0, 0)
    for lambda_ in Lambda:
        mus_new = calculate_unseen_means_regress(class_attributes_unseen, 
                                                class_attributes_seen, mus, lambda_)
        ypred = inference(Xtest, mus_new)
        acc = accuracy(ypred, Ytest)
        if best_accuracy[1] < acc:
            best_accuracy = (lambda_, acc)
        print(f"[{__file__}] Accuracy of LwP for unknown classes: {100*acc:.2f}% (lamdba: {lambda_})")  # noqa: E501
    print(f"Best (Lambda, Accuracy): {best_accuracy}")