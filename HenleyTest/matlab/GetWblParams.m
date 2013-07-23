function [ScaleMatrix ShapeMatrix] = GetWblParams(FailureRates, covArray)
warning off all;
ArrayCnt = length(FailureRates);
x0 = 1;

for p = 1:ArrayCnt
    covSquare = covArray(p)^2;
    ShapeMatrix(p) = fsolve(@(x) (1+covSquare)*gamma(1+ 1/x)^2 - gamma(1+2/x), x0);
    ScaleMatrix(p) = FailureRates(p)* gamma(1+1/ShapeMatrix(p)) ;          %a=1/lambda
end;

%Diagnostics
%Computer Mean time to faiures, they all have to be the same
%    [covArray' 1./FailureRates' ((1./ScaleMatrix).*gamma(1+1./ShapeMatrix))']

%Samples
%%Lambda     COV     Shape       %Scale
%1           3       0.4113      0.2595
%0.86666     5       .311341     .117157
%0.81616     10      0.233207    0.0262295




