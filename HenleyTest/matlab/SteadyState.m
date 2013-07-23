function SteadState()

clc
clear

Component1Steady  = Component1Steady()
Component2Steady = Component2Steady()
Component3Steady = Component3Steady()
Component4Steady = Component4Steady()


 Subsystem1Steady = Subsystem1Steady()      
 Subsystem2Steady = Subsystem2Steady()   



function steady = Subsystem1Steady()
       
 A = [
0       1       2       0
0.033333333333333       0       0       2
0.025   0       0       1
0       0.025   0.033333333333333       0];

M = [0       0.11111111111111        0.22222222222222        0
4.9743458109518e-006    0       0       0.49999368618397
0.023795359904818       0       0       0.95181439619274
0       7.05348832997944 13.55237707032099 0];

    steady = GetSteadyState(A,sum(M,2)');
    
    

function steady = Subsystem2Steady()
 
A = [
0       1       2       0
0.05    0       0       2
0.033333333333333       0       0       1
0       0.033333333333333       0.05    0];


M=[1.7424589408314,1.5172404304285,1.6868597998525,14.887791014049];

steady = GetSteadyState(A,M);
    
    

function steady = Component1Steady()
    A = [0       2
        0.025   0 ];
    
    steady =    ComponentSteady(A);


function steady = Component2Steady()
    A = [0               1
        0.0333333       0 ];
    

    steady = ComponentSteady(A);
    

function steady = Component3Steady()
    A = [0               2
        0.0333333       0 ];
    
    steady = ComponentSteady(A);

    
    
    
function steady = Component4Steady()
    A = [0       1
        0.05    0 ];
    
    steady = ComponentSteady(A);

    
function steady = ComponentSteady(MKVTransitionMatrix)

    A = MKVTransitionMatrix;

    M = 1./A;
    idx = find(M == Inf);
    M(idx) = 0;

    P = ones(length(A), length(A));

    steady = GetSteadyState(A, sum(M,2));



function steady = GetSteadyState(MKVTransitionMatrix, tauVector)

    piVector =  ImbddedChainStationaryDistribution(MKVTransitionMatrix)';
    steady = (piVector.*tauVector) / sum(piVector.*tauVector);

