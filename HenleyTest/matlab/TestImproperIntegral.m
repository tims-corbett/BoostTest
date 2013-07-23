function TestImproperIntegral

clc;
clear;

n = 1e6; % more points = more accuracy. Relative error goes as 1/sqrt(n)
opt.maxArray = 1e6;
opt.noerror = 0;
%opt.tol = 2e-4;
opt.time = 60;
opt.warnOff = 0;
%opt.mcintPar = [];

hident.b = [0,Inf];
hident.cond = {};

hident.funcs = { @TransProb2 @TransProb3};
[I,info] = mcint(hident, n, opt);
I


function f = TransProb3(x, par)

[L1 G1] = GetWblParams(1/40, 1);
[L2 G2] = GetWblParams(1/30, 0.3);

f = x.*(wblpdf(x, 1/L1, G1).*(1-wblcdf(x, 1/L2, G2)));



function f = TransProb2(x, par)

[L2 G2] = GetWblParams(1/40, 1);
[L1 G1] = GetWblParams(1/30, 0.3);

f = x.*(wblpdf(x, 1/L1, G1).*(1-wblcdf(x, 1/L2, G2))) ;


