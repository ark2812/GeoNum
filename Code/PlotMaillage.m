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
Triangles  = load('Triangles.csv');
[x,y]      = textread('datas.txt','%f,%f','headerlines',1)
Points = [x,y];
Triangles = Triangles +1
[a b] = size(Triangles);
[c d] = size(Points);

figure()
triplot(Triangles,x,y); hold on;
for i=1:c
   plot(Points(i,1),Points(i,2),'r.') ;
end
saveas(gcf,'TriwithLegalizeEdge','png');

%for i=1:a %on parcours les triangles
%    A=[Points(Triangles(i,1),1) Points(Triangles(i,2),1) Points(Triangles(i,3),1) Points(Triangles(i,1),1)]; 
%    B=[Points(Triangles(i,1),2) Points(Triangles(i,2),2) Points(Triangles(i,3),2) Points(Triangles(i,1),2)]; 
%    plot(A,B,'b'); hold on;
%end


%[x,y] = textread('datas.txt','%f,%f','headerlines',1);
%tri = DelaunayTri(x,y);
%figure()
%triplot(tri,x,y);

