    clc;
    clear;

    % cov=1, r_od=3, T_m=73000
    
    Lifetime=365 * 24 * 25;
    N=25000;
    dt=Lifetime/N;
    StateCount=4;
    p0 = zeros(1,StateCount)';
    p0(1)=1;


    dpdt_system = ode5(@mssode_Chap2App, 0:dt:Lifetime, p0);    %mssode_xing

    plot(0:dt:Lifetime, dpdt_system(:,4) , '-.');
    axis([0 Lifetime 1E-2 1E-1]);
    set(gca, 'YScale', 'log');

