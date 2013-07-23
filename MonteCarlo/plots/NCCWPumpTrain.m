function plots()

    clc;
    clear;
    idx = [0.3 1 1.3];    
    col = [2 9 12];
    clrs = {'r' 'b' 'k'};
    
    for i=1:length(col)
        hmc=MCSystemPlot(idx(i),'b');
        hsmp=SMPSystemPlot(col(i), clrs{i});
    end;
    legend([hmc,  hsmp])
    
    function h= MCSystemPlot(cv, clr)
    X = load(cell2mat({'NCCWSystemPT' num2str(cv) '.dat'}));
    h=plot(X(:,1), X(:,2),clr);
    set(gca, 'YScale', 'log');
    hold on;
    grid on;
    
function h=SMPSystemPlot( col, clr)
    X = load('../../nccw/NCCWPT.dat');
    h=plot(X(:,1), X(:,col),clr);
    set(gca, 'YScale', 'log');
    hold on;
    grid on;