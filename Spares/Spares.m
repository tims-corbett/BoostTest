function Spares()
clc
clear
format long
nTransformers = 12;
cv = 0.4;
figure;

%CompareWblExp();

clr = {'r', 'b', 'g', 'k', 'y', 'c','r', 'b', 'g', 'k', 'y', 'c'};
for nSpares = 0:4
    fpath = {'Spares_'  num2str(nTransformers) '_' num2str(nSpares) '_' num2str(cv) '.dat'};
    X = load( cell2mat(fpath)  );

    T = X(:, 1);
    plot( T, X(:,2), clr{nSpares+1});
    reliability(nSpares+1, :) = [nSpares X(end,2) 1-X(end,2)];

    set(gca, 'YScale', 'log');
    grid on;
    set(gca,'YMinorGrid', 'off');
    hold on;
end;
format long e ;
reliability


function CompareWblExp()
nTransformers=12;
nSpares=2;

cvs = [0.4, 1.0];
clr = {'r', 'b'};

for i = 1:2
    fpath = {'Spares_'  num2str(nTransformers) '_' num2str(nSpares) '_' num2str(cvs(i)) '.dat'};
    X = load( cell2mat(fpath)  );

    T = X(:, 1);
    plot( T, X(:,2), clr{i});
    hold on;
end;
    
    set(gca, 'YScale', 'log');
    grid on;
    set(gca,'YMinorGrid', 'off');
