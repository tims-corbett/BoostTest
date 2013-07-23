function plots()

clc;
clear;

SystemPlot('IdenticalExpExp');
figure;
SystemFromComponents('IdenticalExpExp');

figure;
MarkovTest();


function SystemPlot(dist)

    Z = load(cell2mat({dist 'System.dat'}));
    t = 1:100:length(Z);
       
    T = Z(t,1);
    X = Z(t,2);
        
    clrs = {'r', 'r', 'k', 'g'};

    plot(T, X, clrs{1});  
    grid on;
    title('system plot');
    hold on;
    smp = X(end)
    
    
function SystemFromComponents(dist)
Y = load(cell2mat({dist 'Components.dat'}));
Q = Y(:,3).*Y(:,3);
plot( Y(:,1), Q,'b');

grid on;
title('system from components');    
probrule =  Y(end,3).*Y(end,3)


function MarkovTest()
    Lifespan = 0:0.1:90;
    aSize =  3;
    p0 = zeros(1,aSize)';
    p0(3)=1;
    dpdt = ode5(@mssode_TwoIdenticalComponentsParallel, Lifespan , p0);
    T = 1:10:length(Lifespan);
    plot( Lifespan(T), dpdt(T, 1),'b-.'  )
    xlabel('time');
    ylabel('System Unavailability');
    hold on;
    grid on;
