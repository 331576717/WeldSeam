function [ST] = fnd_stP(BW)
sz = size(BW);
r = 0;c = 0;
sign = 0;
for i = 1 : sz(1)
    for j = 1 : sz(2)
        if BW(i,j) == 1
            r = i;c = j;
            sign = 1;
            break;
        end
    end
    if sign == 1,break,end
end
if (r==0) | (c==0)
    error('There is no white point in "BW".');
else
    ST = [r,c];
end