function PMTest()
    clc;
    clear;
    
    RunCase();
    
    
function RunCase()    
    Z = load(cell2mat({'pm0.7.dat'}));
    t = Z(:,1);
    figure;
    hold on;
    
    clrs = {'b', 'r', 'g', 'k', 'y', 'm', 'c'};
    j=1;

    for i=3:8
        plot(t, Z(:,i), clrs{j});
        j=j+1;
    end;
    
    grid on;
    
%[1 2   3     4   5   6   7   8   9   10  11  ]
%[T 10  20    30  40  50  60  70  80  90  100 ]