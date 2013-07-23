function dpdt = mssode(t, p)

a=[
0               3e-006          0.00595148      1e-007
0               0               0.0119035       1.28571e-006
0.0134722       0.000277778     0               0.000138889
0.00265753      8.21918e-005    0               0
];

aSize = length(a);

for i = 1:aSize
    a(i,i) = -sum(a(i,:));
end;
% 

for i = 1:aSize     % Horz
    txt=' ';
    for j = 1:aSize % Vert
        if ( i~= j) 
            txt = strcat( txt, num2str(a(j,i)), 'p_{', num2str(j), '}(t) + ');
        end;
    end;
    txt = strcat( '\frac{dp_{', num2str(i), '}(t)}{dt} = ', txt, num2str(a(i,i)), 'p_{', num2str(i), '}(t)')
end;

dpdt =  (p'*a)';
