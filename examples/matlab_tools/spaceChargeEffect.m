workDirectory='/home/eidelyur/radiaSoft/Hellweg2D';
cd (workDirectory);
workDirectory='./';
filename='/home/eidelyur/radiaSoft/Hellweg2D/xHist_100mA_30keV_z0.dat';

fileID=fopen(filename,'r')

tline=0;
while ~feof(fileID)
    if tline ~= -1
        [B,count] = fscanf(fileID, '%d %e  %d',inf);
    end
%     display(['count=',num2str(count)]);
end
fclose(fileID)
count;

Nbins0=count/3;

xBin0=zeros(Nbins0,1);
nBin0=zeros(Nbins0,1);

n=2;
xAv0=0;
for k=1:Nbins0
    xBin0(k)=100*B(n);               % x, cm
    nBin0(k)=B(n+1); 
    xAv0=xAv0+xBin0(k)*nBin0(k);
    n=n+3;
end

Nparticles0=sum(nBin0)

xAv0=xAv0/Nparticles0;

xRMS0=0;
for k=1:Nbins0
    xRMS0= xRMS0+(xBin0(k)-xAv0)^2*nBin0(k);
end

xRMS0=sqrt(xRMS0/Nparticles0)


filename='/home/eidelyur/radiaSoft/Hellweg2D/xHist_100mA_30keV_z25.dat';

fileID=fopen(filename,'r')

tline=0;
while ~feof(fileID)
    if tline ~= -1
        [B,count] = fscanf(fileID, '%d %e %d',inf);
    end
%     display(['count=',num2str(count)]);
end
fclose(fileID)
count;

Nbins25=count/3;

xBin25=zeros(Nbins25,1);
nBin25=zeros(Nbins25,1);

n=2;
xAv25=0;
for k=1:Nbins25
    xBin25(k)=100*B(n);               % x, cm
    nBin25(k)=B(n+1);        
    xAv25=xAv25+xBin25(k)*nBin25(k);
    n=n+3;
end

Nparticles25=sum(nBin25)
xAv25=xAv25/Nparticles25

xRMS25=0;
for k=1:Nbins25
    xRMS25= xRMS25+(xBin25(k)-xAv25)^2*nBin25(k);
end

xRMS25=sqrt(xRMS25/Nparticles25)

figure(5)
bar(10*xBin0,nBin0)
grid on
xlabel('x, mm','FontSize',16,'Color','m')
ylabel('Particle','FontSize',16,'Color','m')
title('Space Charge Effect: I=100 mA, E_{kin}=30 keV','FontSize',16,'Color','m')
text(-2,13,['z = 0 cm, x_{RMS}=',num2str(10*xRMS0,'%4.2f'),' mm'],'FontSize',16,'Color','m')

figure(15)
bar(10*xBin25,nBin25)
grid on
xlabel('x, mm','FontSize',16,'Color','m')
ylabel('Particle','FontSize',16,'Color','m')
title('Space Charge Effect: I=100 mA, E_{kin}=30 keV','FontSize',16,'Color','m')
text(-2,13,['z = 25 cm, x_{RMS}=',num2str(10*xRMS25,'%4.2f'),' mm'],'FontSize',16,'Color','m')

filename='/home/eidelyur/radiaSoft/Hellweg2D/phaseHist_100mA_30keV_z0.dat';
filename='/home/eidelyur/radiaSoft/Hellweg2D/phaseHist_100mA_30keV_z0_N10000.dat';

fileID=fopen(filename,'r')

tline=0;
while ~feof(fileID)
    if tline ~= -1
        [B,count] = fscanf(fileID, '%d %e  %d   %e   %e',inf);
    end
%     display(['count=',num2str(count)]);
end
fclose(fileID)
count;

Nbins0phase=count/5;

phaseBin0=zeros(Nbins0phase,1);
nBin0phase=zeros(Nbins0phase,1);
xAvBin0phase=zeros(Nbins0phase,1);
xRmsBin0phase=zeros(Nbins0phase,1);

n=2;
phaseAv0=0;
for k=1:Nbins0phase
    phaseBin0(k)=B(n);              
    nBin0phase(k)=B(n+1); 
    xAvBin0phase(k)=100*B(n+2);   % cm
    xRmsBin0phase(k)=100*B(n+3);  % cm
    phaseAv0=phaseAv0+phaseBin0(k)*nBin0phase(k);
    n=n+5;
end

Nparticles0phase=sum(nBin0phase)

phaseAv0=phaseAv0/Nparticles0phase;

phaseRMS0=0;
for k=1:Nbins0phase
    phaseRMS0= phaseRMS0+(phaseBin0(k)-phaseAv0)^2*nBin0phase(k);
end

phaseRMS0=sqrt(phaseRMS0/Nparticles0phase)


filename='/home/eidelyur/radiaSoft/Hellweg2D/phaseHist_100mA_30keV_z25.dat';
filename='/home/eidelyur/radiaSoft/Hellweg2D/phaseHist_100mA_30keV_z25_N10000.dat';

fileID=fopen(filename,'r')

tline=0;
while ~feof(fileID)
    if tline ~= -1
        [B,count] = fscanf(fileID, '%d %e %d    %e    %e',inf);
    end
%     display(['count=',num2str(count)]);
end
fclose(fileID)
count;

Nbins25phase=count/5;

phaseBin25=zeros(Nbins25phase,1);
nBin25phase=zeros(Nbins25phase,1);
xAvBin25phase=zeros(Nbins25phase,1);
xRmsBin25phase=zeros(Nbins25phase,1);

n=2;
phaseAv25=0;
for k=1:Nbins25phase
    phaseBin25(k)=B(n);               
    nBin25phase(k)=B(n+1);        
    xAvBin25phase(k)=100*B(n+2);    % cm
    xRmsBin25phase(k)=100*B(n+3);   % cm 
    phaseAv25=phaseAv25+phaseBin25(k)*nBin25phase(k);
    n=n+5;
end

Nparticles25phase=sum(nBin25phase)
phaseAv25=phaseAv25/Nparticles25phase

phaseRMS25=0;
for k=1:Nbins25phase
    phaseRMS25= phaseRMS25+(phaseBin25(k)-phaseAv25)^2*nBin25phase(k);
end

phaseRMS25=sqrt(phaseRMS25/Nparticles25phase)

figure(25)
bar(phaseBin0,nBin0phase)
grid on
xlabel('\phi, deg','FontSize',16,'Color','m')
ylabel('Particle','FontSize',16,'Color','m')
title('Space Charge Effect: I=100 mA, E_{kin}=30 keV','FontSize',16,'Color','m')
% for filename='/home/eidelyur/radiaSoft/Hellweg2D/phaseHist_100mA_30keV_z25.dat';
% text(25,11,['z = 0 cm, \phi_{RMS}=',num2str(phaseRMS0,'%6.2f'),'^o'],'FontSize',16,'Color','m')
% ylim([0,12])
% for filename='/home/eidelyur/radiaSoft/Hellweg2D/phaseHist_100mA_30keV_z25_N10000.dat';
text(-75,1200,['z = 0 cm, \phi_{RMS}=',num2str(phaseRMS0,'%6.2f'),'^o; 10,000 Particles'],'FontSize',16,'Color','m')
ylim([0,1300])
xlim([-120,300])

figure(35)
bar(phaseBin25,nBin25phase)
grid on
xlabel('\phi, deg','FontSize',16,'Color','m')
ylabel('Particle','FontSize',16,'Color','m')
title('Space Charge Effect: I=100 mA, E_{kin}=30 keV','FontSize',16,'Color','m')
% for filename='/home/eidelyur/radiaSoft/Hellweg2D/phaseHist_100mA_30keV_z25.dat';
% text(-175,11,['z = 25 cm, \phi_{RMS}=',num2str(phaseRMS25,'%4.2f'),'^o'],'FontSize',16,'Color','m')
% for filename='/home/eidelyur/radiaSoft/Hellweg2D/phaseHist_100mA_30keV_z25_N10000.dat';
text(-250,1200,['z = 25 cm, \phi_{RMS}=',num2str(phaseRMS25,'%4.2f'),'^o; 10,000 Particles'],'FontSize',16,'Color','m')
ylim([0,1300])
xlim([-300,120])

figure(45)
bar(phaseBin0,10*xAvBin0phase)
grid on
xlabel('\phi, deg','FontSize',16,'Color','m')
ylabel('<r>, mm','FontSize',16,'Color','m')
title('Space Charge Effect: I=100 mA, E_{kin}=30 keV','FontSize',16,'Color','m')
% for filename='/home/eidelyur/radiaSoft/Hellweg2D/phaseHist_100mA_30keV_z25.dat';
% text(75,3.2,'z = 0 cm','FontSize',16,'Color','m')
% for filename='/home/eidelyur/radiaSoft/Hellweg2D/phaseHist_100mA_30keV_z25_N10000.dat';
text(-25,0.36,'z = 0 cm, 10,000 Macroparticles','FontSize',16,'Color','m')
xlim([-120,300])


figure(55)
bar(phaseBin0,10*xRmsBin0phase)
grid on
xlabel('\phi, deg','FontSize',16,'Color','m')
ylabel('r_{rMS}, mm','FontSize',16,'Color','m')
title('Space Charge Effect: I=100 mA, E_{kin}=30 keV','FontSize',16,'Color','m')
% for filename='/home/eidelyur/radiaSoft/Hellweg2D/phaseHist_100mA_30keV_z25.dat';
% text(75,3.6,'z = 0 cm','FontSize',16,'Color','m')
% for filename='/home/eidelyur/radiaSoft/Hellweg2D/phaseHist_100mA_30keV_z25_N10000.dat';
text(0,5.5,'z = 0 cm; 10,000 Particles','FontSize',16,'Color','m')
xlim([-120,300])

figure(65)
bar(phaseBin25,10*xAvBin25phase)
grid on
xlabel('\phi, deg','FontSize',16,'Color','m')
ylabel('<r>, mm','FontSize',16,'Color','m')
title('Space Charge Effect: I=100 mA, E_{kin}=30 keV','FontSize',16,'Color','m')
% for filename='/home/eidelyur/radiaSoft/Hellweg2D/phaseHist_100mA_30keV_z25.dat';
% text(-125,5.25,'z = 25 cm','FontSize',16,'Color','m')
% for filename='/home/eidelyur/radiaSoft/Hellweg2D/phaseHist_100mA_30keV_z25_N10000.dat';
text(-175,0.35,'z = 25 cm; 10,000 Particles','FontSize',16,'Color','m')
xlim([-300,120])

figure(75)
bar(phaseBin25,10*xRmsBin25phase)
grid on
xlabel('\phi, deg','FontSize',16,'Color','m')
ylabel('r_{rMS}, mm','FontSize',16,'Color','m')
title('Space Charge Effect: I=100 mA, E_{kin}=30 keV','FontSize',16,'Color','m')
% for filename='/home/eidelyur/radiaSoft/Hellweg2D/phaseHist_100mA_30keV_z25.dat';
% text(-150,5.5,'z = 25 cm','FontSize',16,'Color','m')
% xlim([-300,120])
% for filename='/home/eidelyur/radiaSoft/Hellweg2D/phaseHist_100mA_30keV_z25_N10000.dat';
text(-200,6.5,'z = 25 cm; 10,000 Particles','FontSize',16,'Color','m')
xlim([-300,120])

%====================================================
%
%  Preparation cst-files for Hellweg2D:
%
%====================================================

nParticles=10000;
rand1=zeros(nParticles,1);
rand2=zeros(nParticles,1);

xTypeDistr=1;       % 1for Gaussin and 2 for equiprobable
xSize=.005;        % m; sigma for xTypeDistr=1 and +-half-width for xTypeDistr=2
nParticles=10000;

xPos=zeros(nParticles,1);

if (xTypeDistr == 1)
    rand1=rand(1,nParticles);
    rand2=rand(1,nParticles);
    for k=1:nParticles
        xPos(k)=xSize*sqrt(-2*log(rand1(k)))*cos(2*pi*rand2(k));
    end
end
if (xTypeDistr == 2)
    xPos=xSize*(rand(1,nParticles)-0.5);
end


xBins=100;

xEdge=zeros(xBins,1);
xDistr=zeros(xBins,1);

xMin=min(xPos);
xMax=max(xPos);
for k=1:xBins
    xEdge(k)=xMin+(xMax-xMin)/xBins*(k-0.5);
end

xDistr=hist(xPos,xBins);

figure(100) 
bar(1000*xEdge,xDistr)
grid on
xlabel('x, mm','FontSize',16,'Color','m')
ylabel('Particle','FontSize',16,'Color','m')
if (xTypeDistr == 1)
    title(['Gaussin Distribution (10,000 Particles): \sigma_x = ',num2str(1000*xSize,'%6.2f'),' mm'],'FontSize',16,'Color','m')
end
if (xTypeDistr == 2)
    title(['Equiprobable Distribution (10,000 Particles): 0.5\cdot width_x = \pm',num2str(xSize,'%6.4f'),' m'],'FontSize',16,'Color','m')
end


aTypeDistr=1;       % 1for Gaussin and 2 for equiprobable
zAngleSize=.001;    % rad; sigma for aTypeDistr=1 and +-half-width for aTypeDistr=2
nParticles=10000;

angl=zeros(nParticles,1);

if (aTypeDistr == 1)
    rand1=rand(1,nParticles);
    rand2=rand(1,nParticles);
    for k=1:nParticles
        angle(k)=zAngleSize*sqrt(-2*log(rand1(k)))*cos(2*pi*rand2(k));
    end
end
if (aTypeDistr == 2)
    angle=zAngleSize*(rand(1,nParticles)-0.5);
end


angleBins=100;

angleEdge=zeros(angleBins,1);
angleDistr=zeros(angleBins,1);

angleMin=min(angle);
angleMax=max(angle);
for k=1:angleBins
    angleEdge(k)=angleMin+(angleMax-angleMin)/angleBins*(k-0.5);
end

angleDistr=hist(angle,angleBins);

figure(110) 
bar(angleEdge,angleDistr)
grid on
xlabel('x'', rad','FontSize',16,'Color','m')
ylabel('Particle','FontSize',16,'Color','m')
if (aTypeDistr == 1)
    title(['Gaussin Distribution: \sigma_{x''} = ',num2str(zAngleSize,'%6.4f'),' rad'],'FontSize',16,'Color','m')
end
if (aTypeDistr == 2)
    title(['Equiprobable Distribution: 0.5\cdot width_{x''} = \pm',num2str(zAngleSize,'%6.4f'),' m'],'FontSize',16,'Color','m')
end

filename='/home/eidelyur/radiaSoft/Hellweg2D/initialBeam_divergence.txt';

fileID=fopen(filename,'w')

strOut='          x-position, [m]           z-angle, [rad]';
fwrite(fileID,strOut);
strOut=sprintf('\n---------------------------------------------------');
fwrite(fileID,strOut);

for k=1:nParticles
    strBegin='';
    strPos=sprintf('\n\t%16.10f',xPos(k));          % m
    strAngle=sprintf('\t %16.10f',angle(k));                % rad
    strOut = strcat(strBegin,strPos,strAngle);   
    fwrite(fileID,strOut);
end
strOut=sprintf('\n');
fwrite(fileID,strOut);

fclose(fileID)

            
filename='/home/eidelyur/radiaSoft/Hellweg2D/initialBeam_flate.txt';

fileID=fopen(filename,'w')

strOut='          x-position, [m]           z-angle, [rad]';
fwrite(fileID,strOut);
strOut=sprintf('\n---------------------------------------------------');
fwrite(fileID,strOut);

for k=1:nParticles
    strBegin='';
    strPos=sprintf('\n\t%16.10f',xPos(k));          % m
    strAngle=sprintf('\t %16.10f',0.0);             % rad
    strOut = strcat(strBegin,strPos,strAngle);   
    fwrite(fileID,strOut);
end
strOut=sprintf('\n');
fwrite(fileID,strOut);

fclose(fileID)
        















