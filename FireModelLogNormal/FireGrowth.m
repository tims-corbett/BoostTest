function FireSpread()
clc
clear
format long

%   1 Non-Fire      r
%   2 Sustained     b
%   3 Vigorous      g
%   4 Interactive   k
%   5 Remote        m       pink/purple
%   6 Full room     c


Lifetime = 45;
N=20000;
dt=Lifetime/N;

StateCount = 6;

%FireModel_Berlin
%FireModel_Lognormal
%FireModel_Markov


%fpath = 'FireModel_Markov.dat';
%fpath = 'FireModel_Berlin.dat';
fpath = 'FireModel_BerlinExp.dat';
X = load( fpath  );
clr = {'r', 'b', 'g', 'k', 'm', 'c','b','k'};

T = X(:, 1);
for i = 2:7
    plot(T, X(:,i), clr{i-1});
    hold on;
end;

axis([0 T(end) 10^-8 10^0]);
%grid on;
set(gca,'YMinorGrid', 'off');
set(gca, 'YScale', 'log');

MaxProbabiltiies(X);
FireSeverity(X);



function MaxProbabiltiies(X)
for i = 1:6
    [y(i), v(i)] = max(X(:,i+1));
    m(i) = X(v(i),1);
end;
[m; y]'



function FireSeverity(X)

figure;
plot(X(:,1),1 - X(:,2)- X(:,7));
axis([0 45 0 1]);

%set(gca, 'YScale', 'log');