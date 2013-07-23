function plots()

    clc;
    clear;

   
    figure;
    SubSystemPlots('IdenticalExpExp','b');
    figure
    SystemPlot('IdenticalExpExp');
    
    figure;
    CompareSystemPlots('IdenticalExpExp');

%    SubSystemPlots('MCExpExp','r');
%    SystemFromComponents('ExpExp');
%    ComponentPlots('WblExp', 'b');
%    ComponentPlots('MCWblExp', 'r');
%    SubsystemDistributionTest();        %multiple distributions
%    ComponentDistributionTest();       %multiple distributions
%    RepairTest();                      %multiple repair dist


function ComponentDistributionTest()
X = load('WblExpComponents.dat');
Y = load('GammaExpComponents.dat');
Z = load('LogNormalExpComponents.dat');

%scaleX=1; scaleY=1; scaleZ=1;

T = 1:100:length(X);

clrs = {'bd', 'r', 'k', 'g'};

P = 1 - ((1-X(:,2).*X(:,3)) .* (1-X(:,4).*X(:,5))); %From components
Q = 1 - ((1-Y(:,2).*Y(:,3)) .* (1-Y(:,4).*Y(:,5))); %From components
R = 1 - ((1-Z(:,2).*Z(:,3)) .* (1-Z(:,4).*Z(:,5))); %From components

scaleX =   max(P);
scaleY =  max(Q);
scaleZ =   max(R);

figure;
hold on;
plot(X(T,1), P(T)./scaleX ,clrs{1});
plot(X(T,1), Q(T)./scaleY,clrs{2});
plot(X(T,1), R(T)./scaleZ,clrs{3});

grid on;



function SubsystemDistributionTest()

X = load('WblExpSystem.dat');
Y = load('GammaExpSystem.dat');
Z = load('LogNormalExpSystem.dat');


scaleX =   max(X(:,4));
scaleY =  max(Y(:,4));
scaleZ =   max(Z(:,4));

%scaleX=1; scaleY=1; scaleZ=1;

T = 1:100:length(X);

clrs = {'bd', 'r', 'k', 'g'};

figure;
hold on;
plot(X(T,1), X(T,4)./scaleX,clrs{1});
plot(X(T,1), Y(T,4)./scaleY,clrs{2});
plot(X(T,1), Z(T,4)./scaleZ,clrs{3});

grid on;


function fr = failurerate(X,Y)
    dt = X(2)-X(1);
    R = 1-Y;
    diffR = -diff(R);
    
    fr = diffR ./ ( R(2:end) * dt);
    fr(end+1) = fr(end);

%-----------------------------------------------------
function RepairTest()
    X = load('GammaExpSystem.dat');
    Y = load('GammaLogNmlsystem.dat');
   Z = load('system.dat');
 
    scaleX =  1; max(X(:,2));
    scaleY =  1; max(Y(:,2));
    scaleZ = 1;  max(Z(:,2));
    
    clrs = {'bd', 'r', 'k', 'g'};

    figure;
    hold on;
    plot(X(1:100:end,1), X(1:100:end,2)./scaleX,clrs{1});
    plot(X(:,1), Y(:,2)./scaleY,clrs{2});
    plot(X(:,1), Z(:,2)./scaleZ,clrs{3});

    grid on;

%-----------------------------------------------------
function SubSystemPlots(dist, clr)
    S = load( cell2mat({dist 'Components.dat'}) );
    Z = load( cell2mat({dist 'System.dat'}) );
    t = 1:100:length(Z);
    
    T = Z(t,1);
    X = Z(t,3); %//subsystem 1
    Y = Z(t,4); %//subsystem 2
        
    clrs = {'b', 'r', 'k', 'g'};
    hold on;
    plot(T, X, 'r'); %clrs{1});
    plot(T, Y, 'b'); %clrs{2});
    
    plot(S(:,1), S(:,2).*S(:,3), 'r-.' );
    plot(S(:,1), S(:,4).*S(:,5), 'b-.');
    grid on;
    title('subsystem plots from components and subsystems');


%-----------------------------------------------------
function SystemPlot(dist)

    Z = load(cell2mat({dist 'System.dat'}));
    t = 1:100:length(Z);
       
    T = Z(t,1);
    X = Z(t,2);
        
    clrs = {'b', 'r', 'k', 'g'};

    plot(T, X, clrs{1});  
    grid on;
    title('system plot');

%-----------------------------------------------------
function  CompareSystemPlots(dist)
    hold on;
    
    Z = load(cell2mat({dist 'System.dat'}));
    X = load(cell2mat({dist 'Components.dat'}));
    
    T = Z(:, 1);
    P = Z(:, 2);        %From subsystems
    Q = 1 - ((1-X(:,2).*X(:,3)) .* (1-X(:,4).*X(:,5))); %From components

    scalingP = 1;  max(P);
    scalingQ = 1;  max(Q);

    plot(T, P/scalingP, 'r.-');
    plot(T, Q/scalingQ, 'b');
    
    title('Compare System Plots');
    grid on;

%-----------------------------------------------------
function ComponentPlots(dist, clr)
Y = load( cell2mat({dist 'Components.dat'}) );
%figure;
hold on;

for i = 2 : size(Y,2)    
    plot(Y(:,1), Y(:,i),clr);
end;
grid on;
title('component plots');

%-----------------------------------------------------
function SystemFromComponents(dist)
Y = load(cell2mat({dist 'Components.dat'}));
clrs = {'b', 'r', 'k', 'g'};

figure;
hold on;

Q = 1 - ((1-Y(:,2).*Y(:,3)) .* (1-Y(:,4).*Y(:,5))); %From components
plot( Y(:,1), Q);

grid on;
title('system from components');

%-----------------------------------------------------
function CompareFailureDistributions()

for i = 2 : size(Y,2)    
    scaling =  max(Y(:,i));
    plot(Y(:,1), Y(:,i)./scaling,clrsys{i-1});
end;

r = 1 - ((1-X(:,2).*X(:,3)) .* (1-X(:,4).*X(:,5)));
scaling =  max(r);
%plot(X(:,1),r./scaling, 'r-.')
title('System plot');
grid on;

    
