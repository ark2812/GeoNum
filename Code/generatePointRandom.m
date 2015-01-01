%% script to generate random points


%Uncomment this line to choose the number of points you want
M=100;


ax=-10;
bx=10;

ay=-10;
by=10;

X = ax + (bx-ax)*rand([M,1]);
Y = ay + (by-ay)*rand([M,1]);


gam = fopen('datas.txt', 'w');
fprintf(gam, 'nbrePoints %d\n',M);
fclose(gam);

fileID = fopen('magicdataX.bin','w');
fwrite(fileID,X,'double');
fclose(fileID);
fileID = fopen('magicdataY.bin','w');
fwrite(fileID,Y,'double');
fclose(fileID);
