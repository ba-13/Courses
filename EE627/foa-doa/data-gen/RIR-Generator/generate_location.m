% function main(num_files, num_srir)
allowance = 1;
L = round([2 + 3*rand(), 2.5 + 7.5*rand(), 2.5 + 7.5*rand()]);
r = L/2 + allowance*rand(1,3);
s = zeros(1,3);
radius = 8*min(r)/9;
phi_set = (0 + 10*(0:35)).*(pi/180);
theta_set = (-90 + 10*(0:18)).*(pi/180);
src = zeros(length(theta_set)*length(phi_set), 3);

for p = 1:length(phi_set)
    for t = 1:length(theta_set)
        s_ = [radius*cos(theta_set(t))*cos(phi_set(p)), ...
            radius*cos(theta_set(t))*sin(phi_set(p)), radius*sin(theta_set(t))];
        s = s_ + r;
        idx = find(L-s < 0);
        if (size(idx) ~= [1 0]) %#ok<BDSCA>
            disp(s);
        end
        index = (p-1)*length(theta_set)+t;
        src(index, :) = s;
    end
end
disp("It did work.");
