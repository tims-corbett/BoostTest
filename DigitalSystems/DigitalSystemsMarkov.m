clc
clear
format long;

s = warning('off','MATLAB:divideByZero');

Lifetime =24; 
N=8000;
dt=Lifetime/N;

StateCount = 5;

p0 = zeros(1,StateCount)';
p0(1)=1;
dpdt_system = ode5(@mssode_DIC, 0:dt:Lifetime, p0);

figure;
plot(0:dt:Lifetime, dpdt_system(:,2),'r' ); hold on;
%plot(0:dt:Lifetime, dpdt_system(:,3),'b' );
plot(0:dt:Lifetime, dpdt_system(:,4),'g' );
plot(0:dt:Lifetime, dpdt_system(:,5),'k' );
hold on;

%plot(0:350*dt:Lifetime, expcdf(0:350*dt:Lifetime, 1/4.2e-5),'b-.' );

set(gca, 'YScale', 'log');
grid on;
set(gca,'YMinorGrid', 'off');

%axis([0 24 10^-5 10^-2]);

