function HenleyUGF()
clc;
clear;     

SMPComponents();
UGFSubsystems();
UGFSystem();


function UGFSystem()

        State0 = 6;

        figure;
        hold on;
        
        fpath = 'results.dat';
        X = load(fpath);
        T = 1:1:length(X);
        plot(X(T,1), X(T,State0),'b');

        grid on;
        

function UGFSubsystems()

        S1 = 4;
        S2 = 5;
        
        figure;
        hold on;

        fpath = 'results.dat';
        X = load(fpath);
        T = 1:100:length(X);
        plot(X(T,1), X(T,S1),'b');
        plot(X(T,1), X(T,S2),'b');
        
        title('Subsystem');
        grid on;
        
        
function SMPComponents()

        C1 = 2;
        C2 = 3;
        
        figure;
        hold on;

        fpath = 'results.dat';
        X = load(fpath);
        T = 1:100:length(X);
        plot(X(T,1), X(T,C1),'b');
        plot(X(T,1), X(T,C2),'b');
        
        title('Components');
        grid on;
        
