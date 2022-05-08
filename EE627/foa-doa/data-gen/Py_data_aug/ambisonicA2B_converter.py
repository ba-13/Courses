import scipy.io
import scipy.signal
import numpy as np
import matplotlib.pyplot as plt
import soundfile as sf
import sounddevice as sd
# from ..params import SPEECH_LENGTH
from intensity_vector import intensity_vector


def ambisonicA_reader(file_path):
    """
    Reads Ambisonic-A format array from .mat file, with array saved as 'responses'
    """
    mat = scipy.io.loadmat(file_path)

    ambisonicA = mat["responses"]
    mic_loc = mat["mic_loc"][0]
    src_loc = mat["src_loc"][0]
    num_samples_srir = mat["num_samples"][0][0]
    reverb = mat["reverb"][0][0]
    room_dim = mat["room_dim"][0]
    return ambisonicA, {"mic": mic_loc, "src": src_loc, "num_srir": num_samples_srir, "reverb": reverb, "room": room_dim}


def ambisonicA_to_B(ambisonicA):
    """
    Conversion from Format A to B
    """
    ambisonicB = np.zeros(ambisonicA.shape)
    ambisonicB[0] = np.sum(ambisonicA, axis=0)      # W
    ambisonicB[1] = ambisonicA[0] - ambisonicA[1] - \
        ambisonicA[2] + ambisonicA[3]               # X
    ambisonicB[2] = ambisonicA[0] + ambisonicA[1] - \
        ambisonicA[2] - ambisonicA[3]               # Y
    ambisonicB[3] = ambisonicA[0] - ambisonicA[1] + \
        ambisonicA[2] - ambisonicA[3]               # Z
    return ambisonicB


def read_audio(path, should_plot=False):
    data, sample_rate = sf.read(path)
    # if 16384 < len(data):
    #     data = data[0: 16384]  # 2^14
    # else:
    #     raise ValueError("Sample Speech too short!")
    if should_plot:
        plt.figure()
        plt.plot(range(len(data)), data)
        plt.title('Read Audio File')
        plt.show()
    return data, sample_rate


def plot_audio(audio):
    plt.figure()
    plt.plot(range(len(audio)), audio)
    plt.title("Audio")
    plt.show()


def convolve_plain_ambisonics(ambisonicB, pure_audio):
    # pure_audio = np.reshape(pure_audio, (1, -1))
    result = []
    result.append(np.convolve(ambisonicB[0], pure_audio))
    result.append(np.convolve(ambisonicB[1], pure_audio))
    result.append(np.convolve(ambisonicB[2], pure_audio))
    result.append(np.convolve(ambisonicB[3], pure_audio))
    return np.array(result)


def extract_intensity_vectors(audio):
    # audio is of length 4x16384 with 16000 sampling rate.
    # f, t, Zxx = scipy.signal.stft(audio, fs=16000, window="hamming", nfft=1024)
    f, t, Zxx = scipy.signal.stft(
        audio, fs=16000, window="hamming", nperseg=1024, noverlap=512)
    result = intensity_vector(Zxx)
    print(result.shape)
    print(result[0, :, 1])

    plt.figure(0)
    plt.subplot(3, 2, 1)
    plt.pcolormesh(t, f, np.abs(
        result[0]), vmin=-1, vmax=1, shading='gouraud')
    plt.title('STFT Magnitude')
    plt.ylabel('Frequency [Hz]')
    plt.xlabel('Time [sec]')

    plt.subplot(3, 2, 2)
    plt.pcolormesh(t, f, np.abs(
        result[1]), vmin=-1, vmax=1, shading='gouraud')
    plt.title('STFT Magnitude')
    plt.ylabel('Frequency [Hz]')
    plt.xlabel('Time [sec]')

    plt.subplot(3, 2, 3)
    plt.pcolormesh(t, f, np.abs(
        result[2]), vmin=-1, vmax=1, shading='gouraud')
    plt.title('STFT Magnitude')
    plt.ylabel('Frequency [Hz]')
    plt.xlabel('Time [sec]')

    plt.subplot(3, 2, 4)
    plt.pcolormesh(t, f, np.abs(
        result[3]), vmin=-1, vmax=1, shading='gouraud')
    plt.title('STFT Magnitude')
    plt.ylabel('Frequency [Hz]')
    plt.xlabel('Time [sec]')

    plt.subplot(3, 2, 5)
    plt.pcolormesh(t, f, np.abs(
        result[4]), vmin=-1, vmax=1, shading='gouraud')
    plt.title('STFT Magnitude')
    plt.ylabel('Frequency [Hz]')
    plt.xlabel('Time [sec]')

    plt.subplot(3, 2, 6)
    plt.pcolormesh(t, f, np.abs(
        result[5]), vmin=-1, vmax=1, shading='gouraud')
    plt.title('STFT Magnitude')
    plt.ylabel('Frequency [Hz]')
    plt.xlabel('Time [sec]')
    plt.show()


if __name__ == '__main__':
    data, Fs = read_audio('./RIR-Generator/sample.flac')
    print(">>> Read Non-directional Audio")
    ambisonicA_srir, meta = ambisonicA_reader('./RIR-Generator/foa1.mat')
    print(">>> Read .mat")
    ambisonicA = convolve_plain_ambisonics(
        ambisonicB=ambisonicA_srir, pure_audio=data)
    print(">>> Convolved SRIRs with Plain Audio.")
    ambisonicB = ambisonicA_to_B(ambisonicA=ambisonicA)
    print(">>> Converted to B format")

    print("Playing W")
    sd.play(ambisonicA[0], Fs)
    sd.wait()
    print("Playing X")
    sd.play(ambisonicA[1], Fs)
    sd.wait()
    print("Playing Y")
    sd.play(ambisonicA[2], Fs)
    sd.wait()
    print("Playing Z")
    sd.play(ambisonicA[3], Fs)
    sd.wait()

    extract_intensity_vectors(ambisonicB)
