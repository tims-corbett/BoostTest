function dpdt = mssode(t, p)

L=1 / 30;
M= 1 / 0.5;

% a = [
%     0 2*M 0; 
%     L 0 M;
%     0 2*L 0
%     ];

a = [
    0 0 0; 
    L 0 M;
    0 2*L 0
    ];


aSize = length(a);

s = sum(a,2);
a = a-diag(s);

% 
% 
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
