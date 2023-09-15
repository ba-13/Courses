# CS771 Autumn 2023 : Assignment 1 Question 6

Current codebase assumes file locations in the following format. To change the `base_path`, change in `utils_.py`

```bash
.
├── 200259 # this directory
│   ├── convex.py
│   ├── README.md
│   ├── regress.py
│   └── utils_.py
└── data # data directory
    ├── AwA_python
    │   ├── class_attributes_seen.npy
    │   ├── class_attributes_unseen.npy
    │   ├── X_seen.npy
    │   ├── Xtest.npy
    │   └── Ytest.npy
    ├── pseudocode.txt
    └── README.txt
```

Ensure you find all the above files, because `convex.py` and `regress.py` imports common functions from `utils_.py`.

## Execution

1. Ensure `base_path` variable inside `utils_.py` points to the directory (relative or absolute path) directly containing `.npy` files. Currently in the above file structure, `base_path = "../data/AwA_python/"`.
2. Run the files `convex.py` and `regress.py` while being in this folder aka `200259`.
3. `convex.py` output:
    ```bash
    $ python convex.py
   [convex.py] Accuracy of LwP for unknown classes: 46.89%
    ```
4. `regress.py` output:
    ```bash
    $ python regress.py
   [regress.py] Accuracy of LwP for unknown classes: 58.09% (lamdba: 0.01)
   [regress.py] Accuracy of LwP for unknown classes: 59.55% (lamdba: 0.1)
   [regress.py] Accuracy of LwP for unknown classes: 67.39% (lamdba: 1)
   [regress.py] Accuracy of LwP for unknown classes: 73.28% (lamdba: 10)
   [regress.py] Accuracy of LwP for unknown classes: 71.68% (lamdba: 20)
   [regress.py] Accuracy of LwP for unknown classes: 65.08% (lamdba: 50)
   [regress.py] Accuracy of LwP for unknown classes: 56.47% (lamdba: 100)
   Best (Lambda, Accuracy): (10, 0.7328478964401295)
    ```