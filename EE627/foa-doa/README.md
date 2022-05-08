# CRNN Implementation for Ambisonics Intensity Vector

Data generated through MATLAB, Model trained over Python.
To generate your own data, run by customising (if needed):

```bash
matlab.exe ./data-gen/RIR-Generator/main.m
```

This will by default generate 4000 `.mat` files with Intensity vector and meta data included in them at `./data/generated/` so make sure there is such a folder

To generate labels for these generated "raw" data, run

```bash
python ./gen_csv.py
```

This will generate `labels.csv` at the root directory, containing labels like :

```
Number, File_Name, Azimuth, Elevation, Grid_ID
6,1003_17.mat,170.0,0.0,674
7,1005_15.mat,80.0,-70.0,496
```

Now you can directly run `./model/train.py` to train the model.
Your model will get saved as `./model/crnn.pth`
For testing, load the model again, and use `model.predict` on your chosen input.
Grid_ID used is based on the formulae `((azimuth_class+180)/10)*19+(elevation_class+90)/10`, giving your a number between 0 to 684, which would be the prediction of the model.
