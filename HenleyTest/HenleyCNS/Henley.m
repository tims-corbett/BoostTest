clc;
clear;



mean =[
0           1       2           0       0       0       0
0.0333333   0       0           2       0       0       0
0.025       0       0           1       0       0       0
0           0.025   0.0333333   0       0       0.0333333       0.05
0           0       0           0       0       2       1
0           0       0           2       0.0333333       0       0
0           0       0           1       0.05    0       0];

cv=[
1       1       1       1       1       1       1
0.3     1       1       1       1       1       1
1       1       1       1       1       1       1
1       1       0.3     1       1       1       0.25
1       1       1       1       1       1       1
1       1       1       1       1       1       1
1       1       1       1       0.25    1       1];

[HenleyStateSpace.scale HenleyStateSpace.shape] = GetWblParams(num2cell(mean), num2cell(cv));

Lifetime = 80;
N = 2000;
dt = Lifetime / N;
StateCount = length(mean);
phi = NavalSemi(HenleyStateSpace, Lifetime, N );

InitState = 4;
p30 = GetPhi(phi, StateCount, InitState,1);
p34 = GetPhi(phi, StateCount, InitState, 5);

p = p30;
plot(0:dt:Lifetime, p, 'r-');


