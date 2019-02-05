function y = normalizeIR(x)


y = (x)/(max(abs(x)));
% y = (x-min(x))/(max(x)-min(x));

end