function [p ] = GetPhi(phi, StateCount, r, c)
unpacked = cell2mat(phi);
p = unpacked( r, c:StateCount:end)';
% p = flipud(unpacked( r:StateCount:end, c));
