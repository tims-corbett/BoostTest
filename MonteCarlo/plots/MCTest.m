%mean=30; cov = 10
%weibull [33.2358,3.71391]

function MCTest()
    clc;
    clear;

%     MCSystemPlot();
%     SMPSystemPlot();

  %   MCSubSystemPlot(2);    %subsystem1 = 1, subsystem2=2
  %   SMPSubSystemPlot(4);    %subsystem1 =col2,3; subsystem2=col4,5
    
    MC2Components('C1'); 
    SMP2Components(2);
    MC2Components('C2'); 
    SMP2Components(3);
    MC2Components('C3'); 
    SMP2Components(4);
    h1=MC2Components('C4'); 
    h2=SMP2Components(5);
     legend([h1 h2]);

    %SteadyC2();
    
function SMPSystemPlot()
    Y = load(cell2mat({'WblExpComponents.dat'}));
    t = Y(:,1);
    grid on;
    Q = 1 - ((1-Y(:,2).*Y(:,3)) .* (1-Y(:,4).*Y(:,5))); %From components
    plot( Y(:,1), Q);

    
function MCSystemPlot()    
    Z = load(cell2mat({'System.dat'}));
    T = 1:10:length(Z(:,1));
    t = Z(T,1);
    h=plot(t, Z(T,2),'o');
    hold on;
    grid on;


function SMPSubSystemPlot(subsys)
    Y = load(cell2mat({'WblExpComponents.dat'}));
    t = Y(:,1);
    grid on;
    Q = Y(:,subsys).*Y(:,subsys+1); %From components
    plot( Y(:,1), Q);

    
function MCSubSystemPlot(subsys)    
    Z = load(cell2mat({'Subsystem' num2str(subsys) '.dat'}));
    t = Z(:,1);
    h=plot(t, Z(:,2));
    hold on;
    grid on;
    
    
function SteadyC2()
    Z = load(cell2mat({'C2.dat'}));
    t = Z(:,1);
    h=plot(t, Z(:,3));
    hold on;
    grid on;
    plot(t, repmat(0.0322879, length(t),1 ));

    
function h=MC2Components(file)    
    Z = load(cell2mat({file '.dat'}));
    t = Z(:,1);
    h=plot(t, Z(:,2),'o');
    hold on;
    grid on;
    
function  h=SMP2Components(s)
    Z = load(cell2mat({'WblExpComponents.dat'}));
    t = Z(:,1);
    h=plot(t,Z(:,s));
    hold on;
    grid on;
    
function WriteTimeLines    ()
% a = [
% 3      1       30.0195
% 3      -1      30.2037
% 3      1       33.2826
% 3      -1      33.6788
% 3      1       43.4714
% 3      -1      43.5758
% 3      1       67.3238
% 3      -1      69.7963];

% a=[
% 4      1       32.0616
% 4      -1      35.4109
% 4      1       64.1478
% 4      -1      64.3193
% 4      1       87.5023];
% 

% a=[
% 2      1       38.9459
% 2      -1      39.0817
% 2      1       85.5416
% 2      -1      87.9878];

a=[ 
1      1       67.4363
1      -1      67.5091];

format bank;
x=0;
disp(['\begin{picture}(200,75)		\centering'])
for i = 1:length(a)
    disp(['\put(' num2str(x) ',22){\line(0,1){6}}' ])
    x=x+40;
end;
x=0;
for i = 1:size(a,1)
    if a(i,2)==1
        disp(  [ '\put(' num2str(x) ',0){\makebox(0,0)[b]{' num2str(a(i,3)) '}}'])
    else
         disp(  [ '\put(' num2str(x) ',0){\makebox(0,0)[b]{\textit{' num2str(a(i,3)) '}}}'])
    end;
    x=x+40;
end;

disp(['\put(0,25){\vector(1,0){' num2str(x) '}}'])
disp(['\put(' num2str(x) ',5){\makebox(0,0)[b]{$t$}}'])
disp(['\end{picture}'])

