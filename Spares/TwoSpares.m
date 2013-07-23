clc
clear
format long;

s = warning('off','MATLAB:divideByZero');

Lifetime = 60; 

%----------------------------continuous semi markov

% Markov result
hold on;
N=30000;
dt=Lifetime/N;

StateCount=4;
p0 = zeros(1,StateCount)';
p0(4)=1;
dpdt_system = ode5(@mssode, 0:dt:Lifetime, p0);
plot(0:dt:Lifetime, dpdt_system(:,1),'r' );

set(gca, 'YScale', 'log');
grid on;
set(gca,'YMinorGrid', 'off');
hold on;
