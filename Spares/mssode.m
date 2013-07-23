function dpdt = mssode(t, p)



L = 12 * (1/90);
M = 1;

a = [
0   0   0       0
L   0   2*M     0
0   L   0       M
0   0   L       0
];


aSize = length(a);

for i = 1:aSize
    a(i,i) = -sum(a(i,:));
end;


for i = 1:aSize     % Horz
    txt=' ';
    for j = 1:aSize % Vert
        if ( i~= j) 
            txt = strcat( txt, num2str(a(j,i)), 'p', num2str(j), '(t) + ');
        end;
    end;
    txt = strcat( 'dp', num2str(i), '(t)/dt = ', txt, num2str(a(i,i)), 'p', num2str(i), '(t)')
end;

dpdt =  (p'*a)';
