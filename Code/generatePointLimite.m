%% script to generate worst case scenario

%Uncomment this line to choose the number of points you want
M=1000;

x0 = 0;
y0 = 0;

X = zeros(M,1);
Y = zeros(M,1);

for j=1:2:M-1
  X(j) =  x0 + (j-1)*1/2;
  X(j+1) =  x0 + (j-1)*1/2;
end
parfor j =1:M
   Y(j) = y0 + mod(j,2);
end


gam = fopen('datasLimit.txt', 'w');
fprintf(gam, 'nbrePoints %d\n',M);
fclose(gam);

fileID = fopen('magicdataLimitX.bin','w');
fwrite(fileID,X,'double');
fclose(fileID);
fileID = fopen('magicdataLimitY.bin','w');
fwrite(fileID,Y,'double');
fclose(fileID);
