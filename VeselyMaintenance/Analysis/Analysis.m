%State 1 - Operational
%State 2 - Degraded
%State 3 - Maintenance
%State 4 - Failure

%% Plot 1a: State probabilities for r_od=10, cov=0.6, T_m=1 year
%% Plot 1b: State probabilities for r_od=10, cov=1.0, T_m=1 year
%% Plot 1c: State probabilities for r_od=10, cov=0.6, T_m=1 week
%% Plot 1d: State probabilities for r_od=10, cov=1.0, T_m=1 week
%% Effect of variability in time to degradation and degradation ratio on degradation
%  probability, r_od=3,10; cov=0.6,1
%% Plot 5: DegradationProbabilities Vs Maintenance, cov = 0.4, 0.6, 1;
%% r_od=3,10
%% Table: Operational unavailability for r_or=3,10 all T_m


function Analysis()
clc
clear
%OperationalUnavailability();
%StateProbabilities();
DegradationProbabilitiesVsMaintenance();
%AllFailures();

function StateProbabilities()
%Time   1
od=2;om=3;of=4;dm=5;df=6;mo=7;md=8;mf=9;fo=10;fd=11;

% 1 Year = 8760 hrs
% 1 Week = 168 hrs
T_m = 8760;
cov = 1;
r_od = 10;
clr = {'r', 'b', 'k', 'g', 'y', 'c','m', 'w', 'g', 'k', 'y', 'c'};

fpath = {'VeselyMaintenance_'  num2str(cov) '_' num2str(T_m) '_' num2str(r_od)  '.dat'};
X = load( cell2mat(fpath)  );

k=1500;
l=1;
T = X(1:k:end,1) / (24*365);
T1 = X(1:l:end,1) / (24*365);

figure; hold on;
plot(T, X(1:k:end, od), ['>' clr{1}]  );
plot(T, X(1:k:end, om), ['*' clr{2}]  );
plot(T, X(1:k:end, of), ['+' clr{3}]  );
plot(T, (X(1:k:end, od) + X(1:k:end, om) + X(1:k:end, of)), ['o' clr{4}]  );

plot(T1, X(1:l:end, od), ['-' clr{1}]  );
plot(T1, X(1:l:end, om), ['-' clr{2}]  );
plot(T1, X(1:l:end, of), ['-' clr{3}]  );
plot(T1, (X(1:l:end, od) + X(1:l:end, om) + X(1:l:end, of)), ['-' clr{4}]  );

legend('\phi_{od}(t) Degraded', '\phi_{om}(t) Maintenance', '\phi_{of}(t) Failure', '\phi_{od}(t)+\phi_{om}(t)+\phi_{of}(t) Operational Unavailability');

axis([0 3 1e-5 1]);
%set(gca, 'XScale', 'log');
set(gca, 'YScale', 'log');


function AllFailures()
%Time   1
od=2;om=3;of=4;dm=5;df=6;mo=7;md=8;mf=9;fo=10;fd=11;
months = [0.25 0.5 1.00 3.00 6.00 12.00 30.00 60.00 100.00];
T_m = [168 336 730 2190 4380 8760 21900 43800 73000];

cov = 1;
r_od = 3;
clr = {'r', 'b', 'k', 'g', 'y', 'c','m', 'w', 'g', 'k', 'y', 'c'};
figure; hold on;

for i = 1 : length(T_m)

    fpath = {'VeselyMaintenance_'  num2str(cov) '_' num2str(T_m(i)) '_' num2str(r_od)  '.dat'};
    X = load( cell2mat(fpath)  );

    k=250;
    l=1;
    T = X(1:k:end,1) / (24*365);
    T1 = X(1:l:end,1) / (24*365);

    plot(T, X(1:k:end, of), ['+' clr{i}]  ); hold on;
    plot(T1, X(1:l:end, of), ['-' clr{i}]  );

    axis([0 5 1e-5 1e-4]);
    %set(gca, 'XScale', 'log');
    %set(gca, 'YScale', 'log');
end;


function DegradationProbabilitiesVsMaintenance()
%Time   1
od=2;om=3;of=4;dm=5;df=6;mo=7;md=8;mf=9;fo=10;fd=11;

cov = [0.6 1.0];
months = [0.25 0.5 1.00 3.00 6.00 12.00 30.00 60.00 100.00];
T_m = [168 336 730 2190 4380 8760 21900 43800 73000];
r_od = [3 10];
figure; hold on;
   
clr = {'r', 'b', 'k', 'g', 'y', 'c','m', 'w', 'g', 'k', 'y', 'c'};
clr_i = 1;

for k = 1:length(cov)
    for j = 1:length(r_od)
        for i = 1 : length(T_m)
            fpath = {'VeselyMaintenance_'  num2str(cov(k)) '_' num2str(T_m(i)) '_' num2str(r_od(j))  '.dat'};
            X = load( cell2mat(fpath)  );
            p_d(i) =   X(end,od);
        end;

        %plot(months, p_d, ['>-' clr{clr_i}]);
        [cov(k) r_od(j) clr_i p_d(end)]
        
        clr_i = clr_i+1;
        axis([0 10^2 0 0.25]);
        set(gca, 'XScale', 'log');
        %set(gca, 'YScale', 'log');
    end;
end;
legend('r_{od}=3, cov=0.6','r_{od}=10, cov=0.6','r_{od}=3, cov=1','r_{od}=10, cov=1' );

%cv    r_od  clr
%0.6   3     r
%0.6   10    b
%1     3     k
%1     10    g


function OperationalUnavailability()
%Time   1
od=2;om=3;of=4;dm=5;df=6;mo=7;md=8;mf=9;fo=10;fd=11;

gamma = [1.0 0.6 0.4];
T_m = [168 336 730 2190 4380 8760 21900 43800 73000];
r_od = 3;
    
clr = {'r', 'b', 'k', 'g', 'y', 'c','m', 'w', 'g', 'k', 'y', 'c'};
figure; hold on;
months = [0.25 0.5 1.00 3.00 6.00 12.00 30.00 60.00 100.00];


%Operational Unavailability
for cov = 1:length(gamma)
    for i = 1 : length(T_m)
        fpath = {'VeselyMaintenance_'  num2str(gamma(cov)) '_' num2str(T_m(i)) '_' num2str(r_od)  '.dat'};
        X = load( cell2mat(fpath)  );
        U(i) =  ( X(end,od) + X(end,om) + X(end,of) );
    end;

    plot(months, U, ['*-' clr{cov}]);
    axis([0 100 1E-3 1.00]);
    set(gca, 'XScale', 'log');
    set(gca, 'YScale', 'log');
end;
legend('1.0', '0.6', '0.4');