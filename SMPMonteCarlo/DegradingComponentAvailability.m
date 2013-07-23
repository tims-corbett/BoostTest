% A degrading component's availability
% State 3->2 - perfect -> degraded
% State 2->0 - degraded -> dead
% State 0->3 - dead -> perfect (corrective action)
% State 2->1 - degraded -> repair (maintenance)
% State 1->3 - repair -> perfect (repair)

function DegradingComponentAvailability()

clc;
clear;

%SMPTest();
%MCTest();
%AllStates();
UnavailabilityCOVTest();


function UnavailabilityCOVTest()
figure;
hold on;

Z = load('DegradingComponentTestCOV_30.dat');
t = 1:10:length(Z);
T = Z(t,1);
clrs = {'r', 'b', 'g', 'k'};

for i = 2:size(Z,2)
    plot(T, Z(t,i),clrs{i-1});        %1
end;


function AllStates()
figure;
hold on;

Z = load('DegradingComponentAllStates_30.dat');
t = 1:10:length(Z);
T = Z(t,1);

%plot(T, Z(t,2),'r');        %State 0
plot(T, Z(t,3),'b');        %1
plot(T, Z(t,4),'k');        %2
hold on;



function SMPTest()

Z = load('DegradingComponentAvailability_30.dat');

t = 1:10:length(Z);
T = Z(t,1);

plot(T, Z(t,2));
hold on;


function MCTest()

Z = load('DegradingComponentAvailabilityMC_30.dat');

t = 1:1:length(Z);
T = Z(t,1);

plot(T, Z(:,2));
