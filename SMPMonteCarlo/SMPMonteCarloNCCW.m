function SMPMonteCarloNCCW()

    clc;
    clear;
    
    idx = [0 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2 1.3];    
    col = [8 9 10];
   
    for i = 1:3
        h1=MCSystemPlot(num2str(idx(col(i))));
        h2=SMPSystemPlot(col(i),'b');
    end;
    legend([h1 h2])

function h=MCSystemPlot(cov)
    Z = load(cell2mat({'NCCW' cov '.dat'}));
    T = 1:5:length(Z);
    t = Z(T,1);
    h= plot(t, Z(T,2),'o');
    hold on;
    grid on;
    
function h=SMPSystemPlot( col, clr)
    X = load('../nccw/nccw.dat');
    h=plot(X(:,1), X(:,col),clr);
    set(gca, 'YScale', 'log');
    hold on;
    grid on;