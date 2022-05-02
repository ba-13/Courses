up = 2048;
N = 1:up;
Fs = 1000;
f = sin(N/100).*randn(1, up);
figure;
plot(N, f);
ifftf = ifft(f);
% fftf_shift = fftshift(fftf);
figure;
plot(N, log10(abs(ifftf)));
% plot(N, log10(abs(fftf_shift)));
player = audioplayer(f, Fs);
play(player);
% player2 = audioplayer(abs(ifftf), Fs);
% play(player2);