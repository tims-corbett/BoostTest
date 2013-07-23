function G = HoldingMatrix(t, StateSpace, StateCount)

scale = StateSpace.scale;
%StateCount = length(scale);

PdfMat = wblpdf( t, 1./scale, StateSpace.shape );

NaNIndices = isnan(PdfMat);
PdfMat(NaNIndices)=0;

% if StateCount == 2          %only 2 states; No competing risk
%     G=PdfMat;
%     return;
% end;

OneMinusCdfMat = 1 - wblcdf( t, 1./scale, StateSpace.shape );
NaNIndices = isnan(OneMinusCdfMat);
OneMinusCdfMat(NaNIndices)=1;

for c = 1:StateCount
    cols = 1:StateCount;
    cols(c) = [];
    prodOneMinusCdf(:,c) = prod(OneMinusCdfMat(:, cols),2);
end;

G = PdfMat.*prodOneMinusCdf;
% NaNIndices = isnan(G);
% G(NaNIndices)=0;