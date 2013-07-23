function HenleyReducedState()
clc;
clear;

%MarkovTest();
SemiMarkovTest();


function SemiMarkovTest()

    
    State0 = 2; State1 = 3; State2 = 4;

    figure;
    hold on;
  
    cv = 0.3:0.1:1.3;


    for i = 1:length(cv)
        fpath = cell2mat({'HenleyWbl' num2str(cv(i)) '.dat'});
        X = load(fpath);
        T = 1:1:length(X);
        P = X(:,State0);
        scaleX = 1; max(P);
        plot(X(T,1), P(T)./scaleX ,'b');
        
        dt=X(2,1)-X(1,1);
        [cv(i) P(ceil(10/dt))]
        
    end;
    
    grid on;
    set(gca,'YMinorTick', 'on');



function MarkovTest()
    Lifespan = 0:0.01:2500;
    aSize = 3;
    p0 = zeros(1,aSize)';
    p0(3)=1;
    dpdt = ode5(@mssode, Lifespan , p0);
    plot( Lifespan, dpdt(:, 1)  )
    xlabel('time');
    ylabel('System unavailability');
    hold on;
    %axis([0 20 1e-4 3e-4]);
    
    

    grid on;
    set(gca,'YMinorGrid', 'off');
    set(gca,'YMinorTick', 'on');
