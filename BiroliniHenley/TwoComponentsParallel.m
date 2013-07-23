%Compares Markov, RBD and SMP for a parallel subsystem

function CompareMethods()
clc;
clear;
CompareProbRuleSMP('WblExp');    
%MarkovTestFourStates();       


function MarkovTestFourStates();       
    Lifespan = 0:0.1:60;
    aSize =  4;
    p0 = zeros(1,aSize)';
    p0(4)=1;
    dpdt = ode5(@mssode_TwoComponentsParallel, Lifespan , p0);
    T = 1:10:length(Lifespan);
    plot( Lifespan(T), dpdt(T, 1),'b-'  )
    xlabel('time');
    ylabel('System Unavailability');
    hold on;
    grid on;

    
    
function CompareProbRuleSMP(dist);    
    Y = load(cell2mat({dist 'RedundantSystem.dat'}));
    sc =1; max(Y(:,2));
    plot(Y(:,1), Y(:,2)/sc, 'r');
    hold on;
    grid on;
    figure;
    C = load(cell2mat({dist 'RedundantComponent.dat'}));
    sc =1; max(C(:,2))^2;
    plot(C(:,1), C(:,2).*C(:,3)/sc, 'g');
    grid on;

