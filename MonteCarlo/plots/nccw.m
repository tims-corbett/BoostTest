function plots()

    clc;
    clear;
    idx = [0 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2 1.3];    
    col = [8 9 10];

    for i=1:length(col)
        h1=MCSystemPlot(idx(col(i)),'b');
        h2=SMPSystemPlot(col(i), 'r');
    end;
    legend([h1 h2]);
        
    
function h=MCSystemPlot(cv, clr)
    X = load(cell2mat({'NCCWSystem' num2str(cv) '.dat'}));
    h=plot(X(:,1), X(:,2),'bo');
    set(gca, 'YScale', 'log');
    hold on;
    grid on;
    
function h=SMPSystemPlot( col, clr)
    X = load('../../nccw/nccw.dat');
    h=plot(X(:,1), X(:,col),'b');
    set(gca, 'YScale', 'log');
    hold on;
    grid on;