% script pour générer des points aléatoire

M=100;

ax=-10;
bx=10;

ay=-10;
by=10;

X = ax + (bx-ax)*rand([M,1]);
Y = ay + (by-ay)*rand([M,1]);

plot(X,Y,'r.');

dlmwrite('data3.txt',[X,Y]);