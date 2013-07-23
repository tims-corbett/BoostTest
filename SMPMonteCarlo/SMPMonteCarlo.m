function SMPMonteCarlo()
clc;
clear;
h1=TestMCComponent();
h2=TestSMPComponent();
legend([h1 h2])

function h=TestMCComponent()
    Z = load(cell2mat({'Component.dat'}));
    T = 1:5:length(Z);
    t = Z(T,1);
    for i = 2:5
       h= plot(t, Z(T,i),'o');
        hold on;
    end;
    grid on;
    
function h=TestSMPComponent()    
    Z = load(cell2mat({'..\\HenleyTest\\matlab\\WblExpComponents.dat'}));
    t = Z(:,1);
    for i = 2:5
        h=plot(t, Z(:,i));
        hold on;
    end;

