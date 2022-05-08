## Current Plan for EE627 Term Paper

The Paper alloted is "CRNN-BASED JOINT AZIMUTH AND ELEVATION LOCALIZATION WITH THE AMBISONICS INTENSITY VECTOR" by Laureline Perotin et al.

Essentially, the paper directs to use first-order ambisonics technique for classification based direction of arrival estimation in terms of azimuthal and elevation angle.
This used to be done by calculating Ambisonics-B format : {W, X, Y, Z} from Ambisonics-A format.
Ambisonics-A format data is received as the input generated from convolution of
clean speech with SRIRs generated from image method, which gives a 4D output, based on the orientation of the microphones, which should be in tetrahedral manner. The order matters here.
This is now converted to Ambisonics B format, i.e. {W, X, Y, Z}, whose STFT is taken and fed to the neural network whose architecture is as shown in the [paper](../notes/alloted-paper.pdf).

Using Ambisonic B format, calculate the Intensity Vector I<sub>a</sub> and I<sub>r</sub>, and normalize the obtained into a 6-dim vector.

Realize how data is generated:

- Generate SRIRs for 4 directions by using the 4 tetrahedral directions, separately done for now, src and mic locations and all meta data with these 4 SRIRs saved in .mat
- These are read in python, plain speech is also read, and convolved with each 4 SRIRs.
- These 4 are together used to generate Ambisonics B format {W, X, Y, Z}.
- Each of them is of length `SPEECH_LENGTH + num_srir - 1`, adjusted `SPEECH_LENGTH` to 16384, thus perform stft with 1024 data-points and half overlap, this yields 512 unique complex values.
- Using this calculate the 6-component intensity vector, which is a function of the four channel values at each (f, t). This gives 38x513x6 image like structure with 6 channels.
- Now to generate the dataset,
  - Assumed 40 room configurations, with 40 sampled (with replacement) speech sounds out of 13 files.
  - Generated 100 different (x,y,z) configurations by generating linear sequence of x,y,z and shuffle-matching them to get a generally-equal number of examples for all (el, az).
