% Script that plot the Delaunay triangulation stocked in the files
% 'Triangles.csv' and 'Points.csv' : 
%       - 'Triangles.csv'   = matrix of size n*3. Each row correspond to a
%       triangle; and each colomn is the indice of the point in the file
%       'Points.csv'
%       - 'Points.csv'      = matrix of size m*2. Each row correspond to a
%       point; the first and the second column are the X and Y coordinate 
%       of the point.
%
close all;
fichierALire = 'data3.txt';

%[numFloor, Triangles1, Triangles2, Triangles3]  = textread('Evolution.csv','%d: %d %d %d');
[x,y]      = textread(fichierALire,'%f,%f','headerlines',1);
xTot = [3*min(x);3*max(x); 0; x];
yTot = [3*min(y);3*min(y); 3*max(y); y];
Points = [x,y];
PointsTot = [xTot,yTot];
%Triangles = [Triangles1,Triangles2,Triangles3] +1;
%[a b] = size(Triangles);
[c d] = size(PointsTot);
%numFloor = numFloor +1;
%[num, indexFloor] = sort(numFloor);
%lenNumBack = 1;
%figure(1)
%N = max(numFloor);
%h = zeros(N,1);
%clr = lines(N); 
%figure, hold on
% for i = 1:max(numFloor)
% lenNum = length(num(num<=i));
% t      =  Triangles(indexFloor(lenNumBack:lenNum),:);
% lenNumBack = lenNum;
% triplot(t,xTot,yTot,'Color',clr(i,:)); hold on;
% labels = cellstr( num2str([0:c-1]'));
%    plot(PointsTot(:,1),PointsTot(:,2),'r.') ;
%    text(PointsTot(:,1),PointsTot(:,2), labels, 'VerticalAlignment','bottom', ...
%                              'HorizontalAlignment','right')
% 
% pause();
% end
%hold off
%legend(num2str((1:N)','iter-%d')) 

[Triangles1,Triangles2,Triangles3] = textread('Triangles.csv', '%d %d %d');
Triangles = [Triangles1,Triangles2,Triangles3] -2;
figure(2)
triplot(Triangles,x,y); hold on;
labels = cellstr( num2str([3:c-1]'));
   plot(Points(:,1),Points(:,2),'r.') ;
   text(Points(:,1), Points(:,2), labels, 'VerticalAlignment','bottom', ...
                             'HorizontalAlignment','right')

%saveas(gcf,'TriwithLegalizeEdge','png');

%for i=1:a %on parcours les triangles
%    A=[Points(Triangles(i,1),1) Points(Triangles(i,2),1) Points(Triangles(i,3),1) Points(Triangles(i,1),1)]; 
%    B=[Points(Triangles(i,1),2) Points(Triangles(i,2),2) Points(Triangles(i,3),2) Points(Triangles(i,1),2)]; 
%    plot(A,B,'b'); hold on;
%end


[x,y] = textread(fichierALire,'%f,%f','headerlines',1);%
%tri = DelaunayTri(x,y);
%figure(3)
%triplot(tri,x,y);
%-3,2
%[x,y] = textread('datas.txt','%f,%f','headerlines',1);
%tri = DelaunayTri(x,y);
%figure()
%triplot(tri,x,y);

