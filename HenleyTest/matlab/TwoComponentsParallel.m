%Compares Markov, RBD and SMP for a parallel subsystem

function CompareMethods()
clc;
clear;
%CompareSubsystem();    %single subsystem plot
MarkovTestThreeStates();       %Markov de and smp cov = 1
MarkovTestFourStates();       %Markov de and smp cov = 1
%CompareSystem();


function MarkovTestFourStates();       
    Lifespan = 0:0.1:90;
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
    %Y = load('ExpExpSystem.dat');
    %plot(Y(:,1), Y(:,3), 'r');

    
function MarkovTestThreeStates();       
    Lifespan = 0:0.1:90;
    aSize =  3;
    p0 = zeros(1,aSize)';
    p0(3)=1;
    dpdt = ode5(@mssode_TwoIdenticalComponentsParallel, Lifespan , p0);
    T = 1:10:length(Lifespan);
    plot( Lifespan(T), dpdt(T, 1),'b-'  )
    xlabel('time');
    ylabel('System Unavailability');
    hold on;
    grid on;
    %Y = load('ExpExpSystem.dat');
    %plot(Y(:,1), Y(:,3), 'r');
    
function CompareSubsystem()
  %  figure;
    hold on;
    Y = load('ExpExpComponents.dat');
    T = 1:150:length(Y(:,1));
    Q = Y(T,2).*Y(T,3); %From components
    plot( Y(T,1), Q,'bo-');
    grid on;

    Y = load('ExpExpSystem.dat');
    T = 1:50:length(Y(:,1));
    plot(Y(T,1), Y(T,3),'r*-');       %only the first subsystem
    grid on;


function CompareSystem()
    Y = load('ExpExpComponents.dat');
    figure;
    hold on;

    Q = 1 - ((1-Y(:,2).*Y(:,3)) .* (1-Y(:,4).*Y(:,5))); %From components
    plot( Y(:,1), Q);
    grid on;

    Y = load('ExpExpSystem.dat');
    plot( Y(:,1), Y(:,2),'r');
    grid on;
    title('System from components and subsystems');