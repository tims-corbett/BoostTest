function mtp = MarkovTransitionProbability(TransitionMatrix)

len = length(TransitionMatrix);
mtp = zeros(len);

% zero out all diagonal entries
TransitionMatrix = TransitionMatrix-diag(diag(TransitionMatrix));

rowsum = sum(TransitionMatrix,2);

for i = 1:len
    for j = 1:len
        mtp(i,j) = TransitionMatrix(i,j) / rowsum(i);
    end;
end;