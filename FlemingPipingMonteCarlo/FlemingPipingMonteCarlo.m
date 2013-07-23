% Input file layout
% Col 1: Time
% 
%   2      3    4       5       6       7       8    
% 1.3   1.2     1.1     1.0     0.9     0.8     0.7  

function FlemingPipingMonteCarlo()
clc;
clear;

figure;
hold on;
h1=MonteCarloResults();
h2=SMPFlemingPipingResults();
axis([0 60 10^-12 10^-4])

legend([h1 h2]);


function h=MonteCarloResults()

  Z = load('FlemingPipingMC_Wbl.dat');

  T = 1:1:length(Z);
  cols = [2 3 4];       %1.3 1.0 0.7
  for col = cols
      h=plot(Z(T,1), Z(T,col));
  end;
  set(gca, 'YScale', 'log');


function h=SMPFlemingPipingResults();
        dist = 'Wbl';
        R = 5;
        L = 4;
        F = 3;
        S = 2;
        
        cvs =  [1.3 1.0 0.7];
        for cv = cvs
            fpath = {'../FlemingPiping/FlemingPiping' dist '_' num2str(cv) '.dat'};
            X = load( cell2mat(fpath)  );
           h= plot(X(:,1), X(:,R));
        end;