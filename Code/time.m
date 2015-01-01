% Script to reproduce the results shown in the report
close all; clear all;

%% Time computation random
% use the generatePointRandom function
n = 6;
step = zeros(n,1);
timeRequired = zeros(n,1);
for i =1:n
    step(i) = 10^i;
end

for i=1:n
   M = step(i);
generatePointRandom    
[~,giveIt] = system(' ./myDelaunay 0 1 0 0')
giveIt = str2num(giveIt);
timeRequired(i) = giveIt
end

step;
ansStep = step.*log(step);
rap = mean(timeRequired(2:end,:)./ansStep(2:end,:));
ansStep = rap*ansStep;

figure;
loglog(step,ansStep,'b'); hold on;
loglog(step,timeRequired,'r');
legend('nlog(n)','myDelaunay','Location','NorthWest');
xlabel('number of points');
ylabel('time required');
%saveas(gcf,'timeRandom0','epsc');



%% Time computation, worst case scenario
% use the generatePointLimite function

close all; clear all;

n = 5;
step = zeros(n,1);
timeRequired = zeros(n,1);
for i =1:n
    step(i) = 10^i;
end

for i=1:n
   M = step(i);
generatePointLimite    
[~,giveIt] = system('./myDelaunay 1 0 0 1 ');
giveIt = str2num(giveIt);
timeRequired(i) = giveIt
end

step;
ansStep = step.*2;
rap = mean(timeRequired(2:end,:)./ansStep(2:end,:));
ansStep = rap*ansStep;

figure;
loglog(step,ansStep,'b'); hold on;
loglog(step,timeRequired,'r');
legend('n^2','myDelaunay','Location','NorthWest');
xlabel('number of points');
ylabel('time required');
%saveas(gcf,'timeLimit0','epsc');


%% Space computation show the memory used in the command window

close all; clear all;

n = 4;
step = zeros(n,1);
timeRequired = zeros(n,1);
for i =1:n
    step(i) = 10^i;
end

for i=1:n
   M = step(i);
generatePointRandom   
system('valgrind ./myDelaunay 0 0 0 0');
end
