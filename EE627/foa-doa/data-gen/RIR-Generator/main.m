src = zeros(12, 100, 3);
for i=1:40
    meta = struct();
    allowance = 2;
    meta.L = round([2 + 3*rand(), 2.5 + 7.5*rand(), 2.5 + 7.5*rand()]);
    L = meta.L;
    disp(L);
    meta.r = L/2 + allowance*(rand(1,3) - 1/2);
    r = meta.r;
    meta.n = 4096;
    n = meta.n;
    meta.beta = 0.2 + 0.6*rand(1,1);
    beta = meta.beta;
    meta.fs = 16000;
    fs = meta.fs;
    x = linspace(0.25, L(1)-0.25, 100);
    x = x(randperm(length(x)));
    y = linspace(0.25, L(2)-0.25, 100);
    y = y(randperm(length(y)));
    z = linspace(0.25, L(3)-0.25, 100);
    z = z(randperm(length(z)));
    for j=1:length(x)
        meta.s = [x(j), y(j), z(j)];
        s = meta.s;
        [az, el, r_] = cart2sph(x(j)-r(1), y(j)-r(2), z(j)-r(3));
        
        [h1, h2, h3, h4] = generate_srir(meta);
        audio_path = random_speech_file('../../data/speech_short');
        
        ambisonicB = get_ambisonicB(audio_path, h1, h2, h3, h4);
        [I, norm] = intensity_vector(ambisonicB, fs);
        saving_path = strcat('../../data/generated/', string(i*j), "_", string(i), ".mat");
        save(saving_path, "I", "r", "s", "L", "beta", "az", "el");
        disp(audio_path)
        disp(strcat(string(az), " ", string(el)))
    end
end