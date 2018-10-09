function [y] = heaviside(x)
if x > 0
    y = 1;
elseif x == 0 
    y = .5;    
elseif x<0
    y = 0;
end

end