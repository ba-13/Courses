function a = h(x)
    x(x>0) = 1;
    x(x<=0) = 0;
    a = x;
end