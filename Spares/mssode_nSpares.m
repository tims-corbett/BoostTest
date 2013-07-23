function dpdt = mssode_nSpares(t, p)

global SpareCount;

NTransformers = 12;
nSpares = SpareCount;
nStates = nSpares+2;

L = NTransformers * 0.03;
M = 4;
spareNo = nSpares;

a = zeros(nStates, nStates);

for i = 2:nStates
    a(i, i-1) = L;
    
    if i ~= 2 && nSpares > 0
        a(i-1, i) = spareNo * M;
        spareNo = spareNo - 1;
    end;
end;


aSize = length(a);

for i = 1:aSize
    a(i,i) = -sum(a(i,:));
end;


% for i = 1:aSize     % Horz
%     txt=' ';
%     for j = 1:aSize % Vert
%         if ( i~= j) 
%             txt = strcat( txt, num2str(a(j,i)), 'p', num2str(j), '(t) + ');
%         end;
%     end;
%     txt = strcat( 'dp', num2str(i), '(t)/dt = ', txt, num2str(a(i,i)), 'p', num2str(i), '(t)')
% end;

dpdt =  (p'*a)';
