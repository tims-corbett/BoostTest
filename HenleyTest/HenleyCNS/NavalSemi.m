%ifft(fft(x).*fft(h))

function [phi] = NavalSemi(StateSpace, Lifetime, N)
s = warning('off','MATLAB:divideByZero');

scale = StateSpace.scale;
shape = StateSpace.shape;

dt=Lifetime/N;
dtHalf = dt/2;
StateCount=length(scale);
phi{1} = eye(StateCount); 
phi{N+1} = zeros(StateCount);
dtHalfPhi0 = dtHalf * phi{1};

h=waitbar(0, 'Setting up buffers, please wait...');

HBuf = num2cell(0:dt:Lifetime);
HBuf = cellfun( @(ndt) HoldingMatrix(ndt, StateSpace, StateCount), HBuf, 'UniformOutput', false)  ; 


waitbar(1/2);

WBuf = num2cell(0:dt:Lifetime);
WBuf = cellfun( @(ndt) (exp(-sum( (StateSpace.scale*ndt).^StateSpace.shape,2))), WBuf,'UniformOutput', false );

% wBuf = num2cell(1:(N+1));
% wBuf = cellfun( @(n) sum(HBuf{n},2) , wBuf, 'UniformOutput', false );
% wBuf = cell2mat(wBuf);
% 
% WBufTest = num2cell(1:(N+1));
% WBufTest = cellfun(@(n) 1 - trapz( 0:dt:(n-1)*dt , wBuf(:, 1:n), 2 ), WBufTest,  'UniformOutput', false );



waitbar(1); close(h);

'Computing state probabilities...';
offset = inv(eye(StateCount) - dtHalf * HBuf{1});

mat1 = []; 
mat2 = phi{1};

maxN = N+1;
h=waitbar(0, 'Computing state probabilities, please wait...');

for n = 2:maxN
    mat1 = horzcat(mat1, HBuf{n});
    phi{n} = offset * (diag(WBuf{n}) + dt * ( mat1  * mat2)- HBuf{n}*dtHalfPhi0 );        
    
    mat2 = vertcat(phi{ n }, mat2);
    
    if mod(n,1000) == 0
        waitbar(n/maxN, h, num2str(n) );
    end;
    
end;
close(h);
s = warning('on','MATLAB:divideByZero');
