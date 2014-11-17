Points = [0 0; 0 1; 0 2; 1 0; 1 1; 1 2; 2 0; 2 1; 2 2];
Triangles = [1 4 5; 1 5 2; 4 7 8; 4 8 5; 2 5 6; 2 6 3; 5 8 9; 5 9 6];

csvwrite('Points.csv',Points);
csvwrite('Triangles.csv',Triangles);