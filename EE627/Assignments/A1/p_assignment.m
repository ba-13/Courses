load('./ex2M1.mat');
fs = 10000;
range = 0:(1/fs):(0.025-1/fs);
figure;
plot(range, speech1_10k);
title('Vowel of 25ms duration');
xlabel('Time');
ylabel('Amplitude');

n = 1024;
Y = fft(speech1_10k, n);
Y_abs = abs(Y);
freq_range = 0:pi/n:pi-pi/n;
s_freq_range = -pi/2:pi/n:pi/2-pi/n;
s_Y = fftshift(Y);
figure;
plot(s_freq_range, log10(abs(s_Y)));
title('FFT Shifted');
xlabel('Frequency');
ylabel('Magnitude (Log10 Scaled)');

ham_10 = zeros(250);
ham_10(125-50:125+49) = hamming(100, "symmetric");
window_10 = (speech1_10k' .* ham_10);
fft_win10 = fftshift(fft(window_10, 1024));
figure;
plot(s_freq_range, log10(abs(fft_win10)));
title('10 ms Hamming Window');
xlabel('Frequency');
ylabel('Magnitude (Log10 Scaled)');

ham_25 = hamming(250, "symmetric");
window_25 = (speech1_10k' .* ham_25);
fft_win25 = fftshift(fft(window_25, 1024));
figure;
plot(s_freq_range, log10(abs(fft_win25)));
title('25 ms Hamming Window');
xlabel('Frequency');
ylabel('Magnitude (Log10 Scaled)');