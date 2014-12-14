% script pour g�n�rer des points al�atoire

M=10;

ax=-100;
bx=100;

ay=-100;
by=100;

X = ax + (bx-ax)*rand([M,1]);
Y = ay + (by-ay)*rand([M,1]);

%plot(X,Y,'r.');
%Xtot = ['nbrePoints';X];
%Ytot = [M;Y];
gam = fopen('datas.txt', 'w');
fprintf(gam, 'nbrePoints %d\n',M);
%dlmwrite('data3.txt',[X,Y],'-append');

fileID = fopen('magicdataX.bin','w');
fwrite(fileID,X,'double');
fclose(fileID);
fileID = fopen('magicdataY.bin','w');
fwrite(fileID,Y,'double');
fclose(fileID);