clc
clear
format long;

s = warning('off','MATLAB:divideByZero');

Lifetime = 40; 

%----------------------------continuous semi markov

% Markov result
hold on;
N=8000;
dt=Lifetime/N;
figure;

global SpareCount;

for SpareCount = 0:4;
StateCount = SpareCount+2;

p0 = zeros(1,StateCount)';
p0(StateCount)=1;
dpdt_system = ode5(@mssode_nSpares, 0:dt:Lifetime, p0);
plot(0:dt:Lifetime, dpdt_system(:,1),'b' );

set(gca, 'YScale', 'log');
grid on;
set(gca,'YMinorGrid', 'off');
hold on;

end;
