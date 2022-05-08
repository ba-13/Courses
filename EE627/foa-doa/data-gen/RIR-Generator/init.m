meta = struct();

meta.fs = 16000;                 % Sample frequency (samples/s)
fs = meta.fs;
meta.r = [2 1.5 2];              % Receiver position [x y z] (m)
r = meta.r;
meta.s = [2 2 11];                % Source position [x y z] (m)
s = meta.s;
meta.L = [5 4 10];               % Room dimensions [x y z] (m)
L = meta.L;
meta.n = 4096;                   % Number of samples
n = meta.n;
meta.beta = 0.4;                 % Reverberation time (s)
beta = meta.beta;
meta.orientation = [pi/2 0];     % Microphone orientation (rad)

[h1, h2, h3, h4] = generate_srir(meta);
[ambisonicB, ambisonicA, Fs] = get_ambisonicB('./sample.flac', h1, h2, h3, h4);
[I, norm] = intensity_vector(ambisonicB, Fs);
plot(h1)
title("Spatial Room Impulse Response")
xlabel("Samples (Time)")
ylabel("Amplitude")
% i1 = I(1, :, :);
% i1 = reshape(i1, [512, 38]);

% save('../../data/generated/sample.mat', "I", "r", "s", "L", "n", "beta", "fs");