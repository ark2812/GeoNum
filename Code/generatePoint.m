% script pour generer des points aleatoire

M=20;

ax=-10;
bx=10;

ay=-10;
by=10;

X = ax + (bx-ax)*rand([M,1]);
Y = ay + (by-ay)*rand([M,1]);

%plot(X,Y,'r.');
%Xtot = ['nbrePoints';X];
%Ytot = [M;Y];
gam = fopen('data3.txt', 'w');
fprintf(gam, 'nbrePoints %d\n',M);
fclose(gam);
%dlmwrite('data3.txt',[X,Y],'-append');

fileID = fopen('magicdataX.bin','w');
fwrite(fileID,X,'double');
fclose(fileID);
fileID = fopen('magicdataY.bin','w');
fwrite(fileID,Y,'double');
fclose(fileID);