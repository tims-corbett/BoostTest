%Fleming Piping C++ Results

function FlemingPipingPlots()
clc;
clear;


	R = 5;
	L = 4;
	F = 3;
	S = 2;


    
%PlotExpectedRuptures();

%ComputeHazardWithoutRepair(R, 'Wbl');
%PlotRuptureHazardWithoutRepair('Wbl'); 
    
%ComputeHazard(R, 'Wbl');
PlotRuptureHazard('Wbl');

%AllStatesPlot('Wbl', 1.0);
%StatePlots(S, 'Wbl');

%PlotRuptureHazardRatio('Wbl');


function AllStatesPlot(dist, cv)

fpath = {'FlemingPiping' dist '_' num2str(cv) '.dat'};
X = load( cell2mat(fpath)  );


T = 1:length(X);

clrs = {'b', 'r', 'k', 'g'};

figure;
hold on;

for s = 2:5
    X(1,s) = X(2,s);
        
    f = plot(X(T,1), X(T,s), clrs{s-1});
    axis([0 X(end,1) 10^-8 10^0]);
end;

set(gca, 'YScale', 'log');
grid on;
set(gca,'YMinorGrid', 'off');





function StatePlots(s, dist)
    clrs = {'b', 'r', 'k', 'g'};

    figure;
    hold on;

    for cv = 1.3:-0.1:0.4
        fpath = {'FlemingPiping' dist '_' num2str(cv) '.dat'};
        X = load( cell2mat(fpath)  );

        T = 1:length(X);

        if cv == 1.0
            clr='r';
        else
            clr='b';
        end;

        f = plot(X(T,1), X(T,s), clr);
       % axis([0 X(end,1)  10^-1 10^-0  ]);
    end;

    set(gca, 'YScale', 'log');
    grid on;
    set(gca,'YMinorGrid', 'off');


[1      2       3       4       5       6       7       8       9       10      11     ]
[T      1.3    1.2     1.1     1.0     0.9     0.8     0.7     0.6     0.5     0.4     ]
    
function PlotRuptureHazard(dist)
    fpath = {'FlemingPiping' dist 'RuptureHazard.dat'};
    X = load( cell2mat(fpath)  );
    
   % T = 1:50:size(X,1);
    T = 1:1:size(X,1);
    
    figure;
    hold on;
    title('rupture hazard');
    
    v = 1.3;
    format short e;
    for i = 2:size(X,2)
        plot( X(T,1), X(T,i), 'b');
        p(i-1,:) = [v X(end,i)   X(end,i)/X(end,5)];
        v = v - 0.1;
    end;
    p
    %axis([0 X(end,1)  0 3.5e-7  ]);
    set(gca, 'YScale', 'log');
    grid on;
    set(gca,'YMinorGrid', 'off');


 function PlotRuptureHazardRatio(dist)
    fpath1 = {'FlemingPiping' dist 'RuptureHazard.dat'};
    X1 = load( cell2mat(fpath1)  );
    
   fpath2 = {'FlemingPiping' dist 'RuptureHazardNoRepair.dat'};
    X2 = load( cell2mat(fpath2)  );    

    T = 1:100:size(X1,1);
    
    figure;
    hold on;
    title('rupture hazard');
    
    cvs = [0 1.3 1.2 1.1 1 0.9 0.8 0.7 0.6 0.5 0.4]; %0 for time column
    cols = [2 5 9];
    clrs = {'r', 'b', 'g'}
   idx = 1;
    
    for i = cols
        plot( X1(T,1), X2(T,i) ./ X1(T,i), clrs{idx} );
        idx = idx+1;
    end;
    
    %axis([0 X(end,1)  0 3.5e-7  ]);
     axis([0 30 1 6]);
    set(gca, 'YScale', 'linear');
    grid on;
    set(gca,'YMinorGrid', 'off');
    
    
function ComputeHazard(state, dist)

    savepath = {'FlemingPiping' dist 'RuptureHazard.dat'};

    %time please
    fpath = {'FlemingPiping' dist '_' num2str(1.0) '.dat'};
    X = load( cell2mat(fpath)  );
    
    dt = X(2,1)-X(1,1);
    
    T = 1:60/dt;
    haz(T,1) = X(T,1);
    
    idx = 2;
    for cv = 1.3:-0.1:0.4
        fpath = {'FlemingPiping' dist '_' num2str(cv) '.dat'};
        X = load( cell2mat(fpath)  );
        scaling = 1; %max(X(T,state));
        haz(:,idx) = failurerate(haz(T,1), X(T,state)./scaling);
        idx = idx + 1;
    end;
    save(cell2mat(savepath), 'haz', '-ASCII');
    



    
    
function p = RuptureProbability(state, dist, cv)    
        fpath = {'FlemingPiping' dist '_' num2str(cv) '.dat'};
        X = load( cell2mat(fpath)  );
        T = 1:length(X);
        p = X(T,state);



    
function haz = ComputeHazardForCov(state, dist, cov)

    savepath = {'FlemingPiping' dist 'RuptureHazard.dat'};

    
    cv = 0.6;
    
    fpath = {'FlemingPiping' dist '_' num2str(cv) '.dat'};
    X = load( cell2mat(fpath)  );
    dt = X(2,1)-X(1,1);
    T = 1:60/dt;
    haz = failurerate(X(T,1), X(T,state));



function fr = failurerate(X,Y)
lnR = log(1-Y);
fr = -diff(lnR) ./ diff(X);
fr(end+1)=fr(end);


function ComputeHazardWithoutRepair(state, dist)

    savepath = {'FlemingPiping' dist 'RuptureHazardNoRepair.dat'};

    %time please FlemingPipingNoRepairWbl_1
    fpath = {'FlemingPipingNoRepair' dist '_' num2str(1.0) '.dat'};
    X = load( cell2mat(fpath)  );
    
    dt = X(2,1)-X(1,1);
    
    T = 1:60/dt;
    haz(T,1) = X(T,1);
    
    idx = 2;
    for cv = 1.3:-0.1:0.4
        fpath = {'FlemingPipingNoRepair' dist '_' num2str(cv) '.dat'};
        X = load( cell2mat(fpath)  );
        haz(:,idx) = failurerate(haz(T,1), X(T,state));
        idx = idx + 1;
    end;
    save(cell2mat(savepath), 'haz', '-ASCII');

    
    
function PlotRuptureHazardWithoutRepair(dist)
    fpath = {'FlemingPiping' dist 'RuptureHazardNoRepair.dat'};
    X = load( cell2mat(fpath)  );
    
    T = 1:50:size(X,1);
    
    figure;
    hold on;
    title('rupture hazard');
    for i = 2:size(X,2)
        
        if i==5
            clr='r';
        else
            clr='b';
        end;
        
        plot( X(T,1), X(T,i), clr);
    end;
    
    %axis([0 X(end,1)  0 3.5e-7  ]);
    set(gca, 'YScale', 'log');
    grid on;
    set(gca,'YMinorGrid', 'off');
    


 

