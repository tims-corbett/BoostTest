
function test()
clc;
clear;

LLess = 5.60757; GLess = 1.71711;       % cov 0.6 'red'
LMore = 5.62281; GMore= 2.6957;         % cov 0.4 'blue'

MeanLess = LLess* gamma(1+1/GLess)
MeanMore = LMore* gamma(1+1/GMore)

SDLess = sqrt( (LLess^2 ) * (gamma(1+2/GLess) - gamma(1+1/GLess)^2  ))
SDMore = sqrt( (LMore^2 ) * (gamma(1+2/GMore) - gamma(1+1/GMore)^2 ))

covLess = SDLess / MeanLess
covMore = SDMore / MeanMore


% figure;
% hold on;
% plothaz(LLess, GLess,'r');
% plothaz(LMore, GMore,'b');

figure;
hold on;
PlotPDFs(LLess, GLess,'r');
PlotPDFs(LMore, GMore,'b');


function PlotPDFs(L, G, c)
t=0:0.01:10;
plot(t,  wblpdf(t,L,G),c);
grid on;
set(gca, 'YScale', 'log');
axis([0 10 10^-2, 10^0])


function plothaz(L,G, c)
t=0:0.001:1;
plot(t,  G*L*(t.*L).^(G-1),c);
%plot(t,wblpdf(t,L,G) ./ (1-wblpdf(t,L,G)));
grid on;
set(gca, 'YScale', 'log');
axis([0 60 10^-3, 10^2])
