function [ScaleMatrix ShapeMatrix] = GetWblParams(FailureRates, covMatrix)
warning off all;
%Both the inputs are cell matrices
%the data structure used for state reduction technique
% No validation is done on the inputs
% Structures of both the inputs is assumed to be identical



iStateCount = length(FailureRates);

%Just do a dummy copy structure of the input
ScaleMatrix = FailureRates;
ShapeMatrix = FailureRates;

for iRow = 1:iStateCount
    for iCol = 1:iStateCount
        frate = FailureRates{iRow,iCol};
        cov = covMatrix{iRow, iCol};
        for iCell = 1: length(cov)
            
            if frate(iCell) == 0        %scale is zero
                ShapeMatrix{iRow,iCol}(iCell) = 1;
                ScaleMatrix{iRow,iCol}(iCell) = 0;
            else
                x0=1;
                covSquare = cov(iCell)^2;
                gmma = fsolve(@(x) (1+covSquare)*gamma(1+ 1/x)^2 - gamma(1+2/x), x0);
                ShapeMatrix{iRow,iCol}(iCell) = gmma;
                ScaleMatrix{iRow,iCol}(iCell) = frate(iCell)* gamma(1+1/gmma) ;        
            end;
        end;
    end;
end;

ScaleMatrix = cell2mat(ScaleMatrix);
ShapeMatrix = cell2mat(ShapeMatrix);

%Diagnostics
%Computer Mean time to faiures, they all have to be the same
%    [covArray' 1./FailureRates' ((1./ScaleMatrix).*gamma(1+1./ShapeMatrix))']


