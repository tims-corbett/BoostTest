% Leak inspection interval set to 1, 5 and 10 years
% Rupture probability is compared
% Flaw inspection interval remains at 10 years
% Flaw growth cov is set to 0.6

% FlemingPipingWbl_TLI_0.6_1.dat
% FlemingPipingWbl_TLI_0.6_5.dat
% FlemingPipingWbl_TLI_0.6_10.dat


function InspectionIntervalTest()

clc;
clear;

PlotRuptureProbFor(1, 'b'); % flaw inspection interval of 10 yrs
PlotRuptureProbFor(5, 'k'); % flaw inspection interval of 5 yrs
PlotRuptureProbFor(10, 'r'); % flaw inspection interval of 5 yrs


function PlotRuptureProbFor(InspInterval, clr)

Z = load( cell2mat({'FlemingPipingWbl_TLI_0.6_' num2str(InspInterval) '.dat'}));

t = 1:400:length(Z);
T = Z(t,1);

RuptureStateCol = 5;    %column in data file where rupture prob sits
plot(T, Z(t,RuptureStateCol),clr);        % State 

grid on;
set(gca, 'YScale', 'log');
axis([0 100 10^-9 10^-6]);
set(gca,'YMinorGrid', 'off');
hold on;

