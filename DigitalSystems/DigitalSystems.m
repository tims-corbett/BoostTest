function DigitalSystems()
clc
clear
format long


% State 1 - Correct output  [BEGIN]     
% State 2 - Low Output                  r
% State 3 - Previous Output             b
% State 4 - High Output                 g
% State 5 - Arbitrary Output            k


Lifetime = 24;
N=20000;
dt=Lifetime/N;

StateCount = 5;

%PlotProb('FeedWaterController_1.dat', 'b-.');
%PlotProb('FeedWaterController_0.6_Prev.dat', 'g'); 
%PlotProb('FeedWaterController_0.4_Prev.dat', 'r'); 
SemiMarkovMissionReliability();

set(gca,'YMinorGrid', 'off');


function PlotProb(fpath, rangu) %rangu means color
%figure;
X = load( fpath  );
clr = {'r', 'b', 'g', 'k', 'm', 'c','b','k'};

{fpath X(end, 3)}

T = X(1:end, 1);
for i = 3 %[2 4 5]
    plot(T, X(1:end,i), rangu);
    hold on;
end;

legend('cov 1.0 [Markov]','cov 0.6','cov 0.4');

xlabel('time (hr)');
ylabel('Controller failure probability');

%axis([0 24 10^-10 10^-3]);
set(gca, 'YScale', 'log');
grid on;
set(gca,'YMinorGrid', 'off');




function SemiMarkovMissionReliability()
    col = 3; %failure state
%   Z = load(cell2mat({'FeedWaterController_1.dat'}));
    Z = load(cell2mat({'FeedWaterController_0.6_Prev.dat'}));
    tm = Z(:,1);
    Y = Z(:,col);
    dt = tm(2);
    Lifetime = tm(end);
    rel = 1 - Y;
    
    
    plot(10:dt:(10+Lifetime), Y , '-.');
    set(gca, 'YScale', 'log');
    hold on;
    xlabel('Time(hr)');
    %ylabel('System failure probability');
    grid on;
    set(gca,'YMinorGrid', 'off');


    relt = 0:dt:Lifetime;
    R = @(t) rel(1 +  ceil(t/dt));
    
    T = 10; %50 hrs already survived
    t = dt:dt:14;      %will it survive 30 more hrs
    
    MR = @(t) R(T+t)./R(T); %mission reliability
    
%    figure;
    plot(10+t, 1-MR(t));
    hold on;
    xlabel('Time(hr)');
    ylabel('Probability');
    set(gca, 'YScale', 'log');
    grid on;
    set(gca,'YMinorGrid', 'off');
    axis([10 24 10^-10 10^-5 ]);   %smp case
   % axis([10 24 10^-5 10^-3 ]);   %markov case

    
    legend('System failure probability','Mission unreliability');