function [ambisonicB, ambisonicA, Fs] = get_ambisonicB(plain_audio_path, h1, h2, h3, h4)
    [y, Fs] = audioread(plain_audio_path);
    y = y(1:16384); % taking nearly 1.024s

    c1 = conv(h1, y, "full");
    c2 = conv(h2, y, "full");
    c3 = conv(h3, y, "full");
    c4 = conv(h4, y, "full");
    
    %   Convolved length will be 16384 + 4096 - 1
    W = c1 + c2 + c3 + c4;
    X = c1 - c2 - c3 + c4;
    Y = c1 + c2 - c3 - c4;
    Z = c1 - c2 + c3 - c4;
    ambisonicA = [c1, c2, c3, c4];
    ambisonicB = [W, X, Y, Z];
end