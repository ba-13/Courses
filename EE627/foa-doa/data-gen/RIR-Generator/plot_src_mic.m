function plot_src_mic(meta)
    plot3([0, meta.r(1), meta.s(1), meta.L(1)], ...
          [0, meta.r(2), meta.s(2), meta.L(2)], ...
          [0, meta.r(3), meta.s(3), meta.L(3)], '.');
    grid on;
end