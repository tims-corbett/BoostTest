function TruncatedPdf()

clc;
clear;

%lambda, gamma of wbl
l=10; ga = 3.7;     

%to be truncated from a to b
a = 6; b = 10;

[m v]=wblstat(l, ga);
m; %= 9.0245
v; %= 7.3791
s = sqrt(v); % 2.7164
c = s/m; %= 0.3010


F = @(x) wblcdf(x, l, ga);
f = @(x) wblpdf(x, l, ga);
g = @(x) ((x>=a) & (x<=b)).*f(x);
tf = @(x) g(x)./(F(b)-F(a));
tF = @(x) (x<a).*0 + ((x>=a) & (x<=b)).*(F(x)-F(a))./(F(b)-F(a)) + (x>b).*1;

t=0:0.01:20;


plot(t,f(t),'b-.'); % original pdf
hold on;
h=plot(t,g(t),'r'); % section of pdf to truncate
hold on;
plot(t,tf(t),'k-.');    %truncated pdf
legend('pdf','','Truncated pdf');
hasbehavior(h,'legend',false) 

figure;
plot(t,F(t),'b-.'); hold on; % original cdf

plot(t,tF(t),'k-.'); %truncated cdf
TruncatedSMP(); %truncated cdf from smp
axis([0,20, -.1,1.1]);


function TruncatedSMP()
    Z = load(cell2mat({'Truncated.dat'}));
    T = Z(:,1);
    Y = Z(:,2);
    
    plot(T,Y);
