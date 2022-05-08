import numpy as np
import scipy.signal


def intensity_vector(AmbisonicsB_freq: np.ndarray):
    """
    Expects Ambisonics B format in frequency domain, STFT to be precise.
    AmbisonicsB_freq = [W(t,f), X(t,f), Y(t,f), Z(t,f)]
    """
    W = AmbisonicsB_freq[0]
    X = AmbisonicsB_freq[1]
    Y = AmbisonicsB_freq[2]
    Z = AmbisonicsB_freq[3]
    # I_a = [(W.conjugate() * X).real, (W.conjugate() * Y).real, (W.conjugate() * Z).real]
    # I_b = [(W.conjugate() * X).imag, (W.conjugate() * Y).imag, (W.conjugate() * Z).imag]
    I = [1e6*np.multiply(W.conjugate(), X).real, 1e6*np.multiply(W.conjugate(), Y).real, 1e6*np.multiply(W.conjugate(), Z).real,
         1e6*np.multiply(W.conjugate(), X).imag, 1e6*np.multiply(W.conjugate(), Y).imag, 1e6*np.multiply(W.conjugate(), Z).imag]
    return np.array(I)


def angles(src_loc: np.ndarray, mic_loc: np.ndarray):
    coord = src_loc - mic_loc
    r = np.sqrt(np.dot(coord, coord))
    if r == 0:
        raise ValueError("Mic and src overlap")
    if coord[0] == 0:
        coord[0] = 1e-8

    a = np.arctan(coord[1]/coord[0])
    elevation = np.arcsin(coord[2]/r)

    if coord[0] > 0 and coord[1] < 0:
        azimuth = a
    elif coord[0] > 0 and coord[1] < 0:
        azimuth = 2*np.pi + a
    elif coord[0] < 0 and coord[1] > 0:
        azimuth = np.pi + a
    elif coord[0] < 0 and coord[1] < 0:
        azimuth = np.pi + a

    return azimuth, elevation
