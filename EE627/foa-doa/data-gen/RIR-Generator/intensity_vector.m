function [vec, norm] = intensity_vector(ambisonicB, Fs)
    s1 = stft(ambisonicB(:, 1), Fs, 'Window',hamming(1024,"periodic"), 'OverlapLength',512, 'FFTLength',1024);
    s2 = stft(ambisonicB(:, 2), Fs, 'Window',hamming(1024,"periodic"), 'OverlapLength',512, 'FFTLength',1024);
    s3 = stft(ambisonicB(:, 3), Fs, 'Window',hamming(1024,"periodic"), 'OverlapLength',512, 'FFTLength',1024);
    s4 = stft(ambisonicB(:, 4), Fs, 'Window',hamming(1024,"periodic"), 'OverlapLength',512, 'FFTLength',1024);
    W = s1(1:513, :);
    X = s2(1:513, :);
    Y = s3(1:513, :);
    Z = s4(1:513, :);
    sz = size(W);

    vec = zeros(6, sz(1), sz(2));
    norm = abs(W).^2 + (1/3)*(abs(X).^2+abs(Y).^2+abs(Z).^2);
    vec(1, :, :) = real(conj(W).*X)./norm;
    vec(2, :, :) = real(conj(W).*Y)./norm;
    vec(3, :, :) = real(conj(W).*Z)./norm;
    vec(4, :, :) = imag(conj(W).*X)./norm;
    vec(5, :, :) = imag(conj(W).*Y)./norm;
    vec(6, :, :) = imag(conj(W).*Z)./norm;
end