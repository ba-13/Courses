function [h1, h2, h3, h4] = generate_srir(meta)
    orientation1 = [atan(1/sqrt(2)) pi/4];
    orientation2 = [-atan(1/sqrt(2)) 3*pi/4];
    orientation3 = [atan(1/sqrt(2)) -3*pi/4];
    orientation4 = [-atan(1/sqrt(2)) -pi/4];
    h1 = rir_generator(340, meta.fs, meta.r, meta.s, meta.L, meta.beta, ...
        meta.n, 'spherical', -1, 3, orientation1, 0);
    h2 = rir_generator(340, meta.fs, meta.r, meta.s, meta.L, meta.beta, ...
        meta.n, 'spherical', -1, 3, orientation2, 0);
    h3 = rir_generator(340, meta.fs, meta.r, meta.s, meta.L, meta.beta, ...
        meta.n, 'spherical', -1, 3, orientation3, 0);
    h4 = rir_generator(340, meta.fs, meta.r, meta.s, meta.L, meta.beta, ...
        meta.n, 'spherical', -1, 3, orientation4, 0);
end