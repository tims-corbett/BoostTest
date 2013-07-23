function imChain =  ImbddedChainStationaryDistribution(TransitionMatrix)

len = length(TransitionMatrix);
p_ij = MarkovTransitionProbability(TransitionMatrix);
p_ij = p_ij - eye(len);
p_ij(:,end) = 1;
z = zeros(len, 1);
z(end) = 1;

opts.TRANSA = true;
imChain = linsolve(p_ij, z,opts)';

%Example

% LA=0.08; MA=0.082;
% A = [ 0 2*LA 0; MA 0 LA; 0 2*MA 0];
% imChain =  ImbddedChainStationaryDistribution(A);