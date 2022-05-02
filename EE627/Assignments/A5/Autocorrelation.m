function [autocorr_] = Autocorrelation(y)
    len = length(y);
    y_new = zeros(len*5);
    non_zero_range = 2*len:3*len-1;
    y_new(non_zero_range) = y;
    y = y_new;
    autocorr_ = zeros(len, 1);
    for k=1:length(autocorr_)
        for i=non_zero_range
            autocorr_(k) = autocorr_(k) + y(i)*y(i-k);
        end
    end
end