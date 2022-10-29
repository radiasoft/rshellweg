twoPi=2*3.14159265358;
cLight=2.9979246e8;     % light speed, m/s
freqRF=2856e6;          % RF frequency, Hz
mc2=0.51099893e6;       % electronn mass, eV
lambdaZ=100.0;          % cm
xAxisZ=0.0;             % x-shift of Z axis, cm

lambda=cLight/freqRF    % m

%
% Reading output data from Hellweg2D:
%
fnalHome=1;    % =1 for FNAL and =0 fore home
if (fnalHome == 0)
%    cd ('C:/Users/Yury/My Personal Documents/My WORK/ESS/') 
%    dirname='C:/Users/Yury/My Personal Documents/My WORK/ESS/';
elseif (fnalHome == 1)
    cd ('/home/eidelyur/radiaSoft/astra/comparison_Hellweg/Hellweg3D/') 
    dirname='/home/eidelyur/radiaSoft/astra/comparison_Hellweg/Hellweg3D/';
end
fileFromHellweg=strcat(dirname,'beamHellweg.data')
%
% The structure of the file:
%   1st  line - title
%   2nd  line: Current [A], nParticles
%   3rd  line - title
%   4th  line: Rsph [sm], Rcatch [sm], <W> [GeV], kT [eV]
%   5th  line - title
%   6th  line: Gx.limit, Gx.mean, Gx.sigma
%   7th  line - title
%   8th  line: Gy.limit, Gy.mean, Gy.sigma
%   9th  line - title
%   10th line: Gv.limit, Gv.mean, Gv.sigma
%   11th line - title
%
% Each line after that 
%     1st column - particle number
%     2nd column - X [m]
%     3rd column - theta [0 - pi]
%     4th column - V [d/l]
%     5th column - r of particle [m]
%     6th column - beta.r of particle [d/l]
%     7th column - theta of particle 
%     8th column - beta.Th of particle [d/l]
%     9th column - beta.z of particle [d/l]
%
fileID=fopen(fileFromHellweg,'r')

tline=fgetl(fileID)
C=textscan(fileID,'%f %d')
beamCurrent=C{1}                  % A
partInputFile=C{2}

tline=fgetl(fileID)
C=textscan(fileID,'%f %f %f %f')
averEnergy=1e6*C{3}               % eV

for n=1:6
    tline=fgetl(fileID);
end

while ~feof(fileID)
    tline = fgetl(fileID);
    if tline ~= -1
        [BM,count] = fscanf(fileID, '%d %e %e %e %e %e %e %e %e',inf);
    end
%     display(['count=',num2str(count)]);
end
fclose(fileID)
count;

lines=count/9

beamX=zeros(lines,1);
beamY=zeros(lines,1);
beamV=zeros(lines,1);
beamR=zeros(lines,1);
beamBr=zeros(lines,1);
beamTh=zeros(lines,1);
beamBth=zeros(lines,1);
beamBz=zeros(lines,1);

n=2;
for k=1:lines
    beamX(k)=BM(n);                      % X [m]
    beamY(k)=BM(n+1);                    % theta [0 - pi]
    beamV(k)=BM(n+2);                    % V [d/l]
    beamR(k)=BM(n+3);                    % r [m]
    beamBr(k)=BM(n+4);                   % beta_r of particle [d/l]
    beamTh(k)=BM(n+5);                   % theta of particle [rad]
    beamBth(k)=BM(n+6);                  % beta_theta of particle [d/l]
    beamBz(k)=BM(n+7);                   % beta_z of particle [d/l]
    n=n+9;
end

figure (5)
plot(1:1000,1000*beamX,'.r','LineWidth',2)
grid on
ylabel('x, mm','FontSize',16,'Color','m')
xlabel('Particle','FontSize',16,'Color','m')
title('Coordinate','FontSize',16,'Color','m')

figure (7)
plot(1:1000,1000*beamR,'.r','LineWidth',2)
grid on
xlabel('Particle','FontSize',16,'Color','m')
ylabel('r, mm','FontSize',16,'Color','m')
title('Radius','FontSize',16,'Color','m')

minX=1000*min(beamX);
maxX=1000*max(beamX);
nBin=30;
edgeBinX=zeros(nBin+1,1);
edgeBinX(1)=minX;
stepX=(maxX-minX)/nBin;

for i=1:nBin
    edgeBinX(i+1)=edgeBinX(i)+stepX;
end

nnX=histc(1000*beamX,edgeBinX);

figure (10)
plot(edgeBinX,nnX,'r','LineWidth',2)
grid on
ylabel('Particle','FontSize',16,'Color','m')
xlabel('X, mm','FontSize',16,'Color','m')
title('X-Distribution','FontSize',16,'Color','m')
xlim([minX,maxX])

minY=min(beamY);
maxY=max(beamY);
nBin=30;
edgeBinY=zeros(nBin+1,1);
edgeBinY(1)=minY;
stepY=(maxY-minY)/nBin;

for i=1:nBin
    edgeBinY(i+1)=edgeBinY(i)+stepY;
end

nnY=histc(beamY,edgeBinY);

figure (20)
plot(edgeBinY,nnY,'r','LineWidth',2)
grid on
ylabel('Particle','FontSize',16,'Color','m')
xlabel('\theta','FontSize',16,'Color','m')
title('Theta-Distribution','FontSize',16,'Color','m')
xlim([minY,maxY])


minV=min(beamV);
maxV=max(beamV);
nBin=30;
edgeBinV=zeros(nBin+1,1);
edgeBinV(1)=minV;
stepV=(maxV-minV)/nBin;

for i=1:nBin
    edgeBinV(i+1)=edgeBinV(i)+stepV;
end

nnV=histc(beamV,edgeBinV);

figure (30)
plot(edgeBinV,nnV,'r','LineWidth',2)
grid on
ylabel('Particle','FontSize',16,'Color','m')
xlabel('V, m/s','FontSize',16,'Color','m')
title('Velocity-Distribution','FontSize',16,'Color','m')
xlim([minV,maxV])


minR=1000*min(beamR);
maxR=1000*max(beamR);
nBin=30;
edgeBinR=zeros(nBin+1,1);
edgeBinR(1)=minR;
stepR=(maxR-minR)/nBin;

for i=1:nBin
    edgeBinR(i+1)=edgeBinR(i)+stepR;
end

nnR=histc(1000*beamR,edgeBinR);

figure (40)
plot(edgeBinR,nnR,'r','LineWidth',2)
grid on
ylabel('Particle','FontSize',16,'Color','m')
xlabel('r, mm','FontSize',16,'Color','m')
title('r-Distribution','FontSize',16,'Color','m')
xlim([minR,maxR])

minTh=min(beamTh);
maxTh=max(beamTh);
nBin=30;
edgeBinTh=zeros(nBin+1,1);
edgeBinTh(1)=minTh;
stepTh=(maxTh-minTh)/nBin;

for i=1:nBin
    edgeBinTh(i+1)=edgeBinTh(i)+stepTh;
end

nnTh=histc(beamTh,edgeBinTh);

figure (50)
plot(edgeBinTh,nnTh,'r','LineWidth',2)
grid on
ylabel('Particle','FontSize',16,'Color','m')
xlabel('\theta','FontSize',16,'Color','m')
title('\theta-Distribution','FontSize',16,'Color','m')
xlim([minTh,maxTh])

minBr=min(beamBr);
maxBr=max(beamBr);
nBin=30;
edgeBinBr=zeros(nBin+1,1);
edgeBinBr(1)=minBr;
stepBr=(maxBr-minBr)/nBin;

for i=1:nBin
    edgeBinBr(i+1)=edgeBinBr(i)+stepBr;
end

nnBr=histc(beamBr,edgeBinBr);

figure (60)
plot(edgeBinBr,nnBr,'r','LineWidth',2)
grid on
ylabel('Particle','FontSize',16,'Color','m')
xlabel('\beta_r','FontSize',16,'Color','m')
title('\beta_r-Distribution','FontSize',16,'Color','m')
xlim([minBr,maxBr])

minBz=min(beamBz);
maxBz=max(beamBz);
nBin=30;
edgeBinBz=zeros(nBin+1,1);
edgeBinBz(1)=minBz;
stepBz=(maxBz-minBz)/nBin;

for i=1:nBin
    edgeBinBz(i+1)=edgeBinBz(i)+stepBz;
end

nnBz=histc(beamBz,edgeBinBz);

figure (70)
plot(edgeBinBz,nnBz,'r','LineWidth',2)
grid on
ylabel('Particle','FontSize',16,'Color','m')
xlabel('\beta_z','FontSize',16,'Color','m')
title('\beta_z-Distribution','FontSize',16,'Color','m')
xlim([minBz,maxBz])

%
% Preparing data for input file for ASTRA:
%
nParticles=lines;                     % number of not reference particles
xAstra=zeros(nParticles,1);           % x-coordinate, m
xdAstra=zeros(nParticles,1);          % x'-coordinate, mrad
yAstra=zeros(nParticles,1);           % y-coordinate, m
zAstra=zeros(nParticles,1);           % z-coordinate, m
energyAstra=zeros(nParticles,1);      % kinetic energy, eV
pAstra=zeros(nParticles,1);           % total momentum, eV
pzAstra=zeros(nParticles,1);          % pz-momentum, eV
pxAstra=zeros(nParticles,1);          % px-momentum, eV
pyAstra=zeros(nParticles,1);          % py-momentum, eV
prAstra=zeros(nParticles,1);          % pr-momentum, eV
pzAstraRel=zeros(nParticles,1);       % relative (regarding reference) pz-momentum, eV/c
alphaAzim=zeros(nParticles,1);        % uniform distributon from 0 till 2*pi, rad
clockAstra=0.;                        % clock value,   ns
gammaAstra=zeros(nParticles,1);

pzRef=sqrt(averEnergy*(averEnergy+2*mc2));    % eV
disp(['averEnergy (MeV)=',num2str(averEnergy,'%12.6e'),'; pz (eV)=',num2str(pzRef,'%12.6e')])

for i=1:nParticles
    xAstra(i)=beamR(i)*cos(beamTh(i));        % m
    yAstra(i)=beamR(i)*sin(beamTh(i));        % m
    beta2=beamBr(i)^2+beamBz(i)^2;
    gammaAstra(i)=1/sqrt(1-beta2);
    energyAstra(i)=mc2*(gammaAstra(i)-1);   % eV
end
%
% Checking of distribution of initial particles:
%
figure (100)
plot(1e3*xAstra,1e3*yAstra,'.r')
grid on
xlabel('x, mm','FontSize',16,'Color','m')
ylabel('y, mm','FontSize',16,'Color','m')
% title(['Initial Uniform Disribution: R_x=', ...
%       num2str(1e3*Rx,'%4.2f'),' mm, R_y=', ...
%       num2str(1e3*Ry,'%4.2f'),' mm'],'FontSize',16,'Color','m')
title('Initial Disribution from Hellweg2D','FontSize',16,'Color','m')
axis square

%
% Checking of distribution of energy:
%
minEnergy=min(energyAstra);
maxEnergy=max(energyAstra);
figure (110)
plot(1:nParticles,1e-3*energyAstra,'.r')
grid on
xlabel('Particle','FontSize',16,'Color','m')
ylabel('E_{kin}, keV','FontSize',16,'Color','m')
title(['Initial Disribution of yhe Enerrgy: E_{ref}=', ...
       num2str(1e-6*averEnergy,'%7.3f'),' MeV'],'FontSize',16,'Color','m')
ylim(1.e-3*[minEnergy,maxEnergy])

pAstra=sqrt(energyAstra.*(energyAstra+2*mc2));   % eV
%
% Checking of distribution of p-total:
%
minPtotal=min(pAstra);
maxPtotal=max(pAstra);
figure (120)
plot(1:nParticles,1e-3*pAstra,'.r')
grid on
xlabel('Particle','FontSize',16,'Color','m')
ylabel('p_{total}, keV/c','FontSize',16,'Color','m')
title(['Initial Disribution: Total Momentum: (p_z)_{ref}=', ...
       num2str(1e-6*pzRef,'%7.3f'),' MeV'],'FontSize',16,'Color','m')
ylim(1.e-3*[minPtotal,maxPtotal])

%
% Distributions of p_z and p_z_relative (concerning reference particle):
%
pzAstra=mc2*gammaAstra.*beamBz;                       % eV
%
% Checking that pAstra > pzAstra:
%
for i=1:nParticles
    if pzAstra(i) >= pAstra(i)
        i
        pzAstra(i)
        pAstra(i)
    end
end
%
% Checking of distribution of p_z:
%
figure (130)
plot(1:nParticles,1e-3*pAstra,'.r',1:nParticles,1e-3*pzAstra,'.b')
grid on
xlabel('Particle','FontSize',16,'Color','m')
ylabel('p_{total} & p_z, keV','FontSize',16,'Color','m')
title(['Total and Longitudinal Momenta: (p_z)_{ref}=', ...
       num2str(1e-6*pzRef,'%7.3f'),' MeV'],'FontSize',16,'Color','m')
legend('Total momentum','Longitudinal momemtum','Location','s')
ylim(1.e-3*[.99925*minPtotal,maxPtotal])

pzAstraRel=pzAstra-pzRef;                             % eV

%
% Checking of distribution of relative p_z:
%
minPzRel=min(pzAstraRel);
maxPzRel=max(pzAstraRel);
figure (140)
plot(1:nParticles,1e-3*pzAstraRel,'.r')
grid on
xlabel('Particle','FontSize',16,'Color','m')
ylabel('p_z-p_{z_{ref}}, keV','FontSize',16,'Color','m')
title(['Relative Longitudinal Momentum: (p_z)_{ref}=', ...
       num2str(1e-3*pzRef,'%7.3f'),' keV/c'],'FontSize',16,'Color','m')
ylim(1.e-3*[minPzRel,maxPzRel])

alphaAzim=beamTh;

figure (150)
plot(cos(alphaAzim),sin(alphaAzim),'.r')
grid on
axis square
                             
%
% Checking of distribution of p_r:
%
prAstra=mc2*gammaAstra.*beamBr;                       % eV

minPr=min(prAstra);
maxPr=max(prAstra);
figure (160)
plot(1:nParticles,1e-3*prAstra,'.r')
grid on
xlabel('Particle','FontSize',16,'Color','m')
ylabel('p_r, keV','FontSize',16,'Color','m')
title(['Initial Disribution of the Transvesal Momentum: E_{ref}=', ...
       num2str(1e-3*averEnergy,'%7.3f'),' keV'],'FontSize',16,'Color','m')
ylim(1.e-3*[minPr,maxPr])

%
% Checking of distributions of p_x, p_y:
%
for n=1:nParticles
    pxAstra(n)=prAstra(n)*cos(alphaAzim(n));            % eV
    pyAstra(n)=prAstra(n)*sin(alphaAzim(n));            % eV
end

figure (170)
plot(1e-3*pxAstra,1e-3*pyAstra,'.r')
grid on
xlabel('p_x, keV','FontSize',16,'Color','m')
ylabel('p_y, keV','FontSize',16,'Color','m')
axis square
title(['Initial Disribution: E_{ref}=', ...
       num2str(1e-3*averEnergy,'%7.3f'),' keV'],'FontSize',16,'Color','m')
xlim([-1.0,1.0])
ylim([-1.0,1.0])

%
% Checking initial x-x' distribution:
%
xdAstra=pxAstra./pAstra;                 % rad

figure (180)
plot(1000*xAstra,1000*xdAstra,'.r')
grid on
xlabel('x, mm','FontSize',16,'Color','m')
ylabel('x'', mrad','FontSize',16,'Color','m')
title(['Initial Disribution: E_{ref}=', ...
       num2str(1e-3*averEnergy,'%7.3f'),' keV'],'FontSize',16,'Color','m')
   
%
% Initial x-emittance:
%
xAstra_std=1000*std(xAstra);                             % mm
xdAstra_std=1000*std(xdAstra);                           % mrad
xDxAstra=zeros(nParticles,1);
xDxAstra_std=(1000*xAstra).*(1000*xdAstra);              % mm*mrad = um
xDxAstra_std=std(xDxAstra);
x_emit=sqrt(xAstra_std^2*xdAstra_std^2-xDxAstra_std^2)   % mm*mrad = um

%----------------------------------------------------------
% Results (for kT=1 eV):
%
% xAstra_std = 2.74956647870433                          % mm
% xdAstra_std = 1.86515983129141                         % mrad
% xDxAstra_std = 0                                       % mm*mrad = um
% x_emit = 5.12838094954468                              % mm*mrad = um
%----------------------------------------------------------

%===================================================================
%
% Start of preparation  file for ASTRA:
%   
clockAstra=0.;                                                % clock value,   ns
particleIndex=1;                                              % electron
% particleIndex=3;                                              % proton

% beam current in A
%
writtenParticles=nParticles;                                  % number of macroparticles
chargeAstra=1e9*beamCurrent*lambda/cLight/writtenParticles    % charge of macroparticle,  nC

str='astra_forHellweg_';
str=strcat(str,int2str(1000*beamCurrent),'_',int2str(writtenParticles),'.ini')
filenameWR=strcat(dirname,str)

%
% Structure of file 'astra_forHellweg_current_particles.ini'
%
% 1st  column - x-coordinate, m
% 2nd  column - y-coordinate, m
% 3rd  column - z-coordinate relative reference particle, m
% 4th  column - px-coordinate, eV/c
% 5th  column - py-coordinate, eV/c
% 6th  column - pz-coordinate relative reference particle, eV/c
% 7th  column - clock, ns
% 8th  column - macro charge, nC
% 9th  column - particle index (=1 for electron, =3 for proton)
% 10th column - status flag (=5 for reference particle, =3 for other initial)
%

fileID=fopen(filenameWR,'w')
%
%  Reference particle:
%
statusFlag=5;                      % reference particle; tracked
fprintf(fileID,'%12.4e  %12.4e %12.4e  %12.4e %12.4e  %12.4e %12.4e  %12.4e %4d %4d\n',...
        0,0,0,0,0,pzRef,clockAstra,chargeAstra,particleIndex,statusFlag);

statusFlag=3;                      % other particles; tracked
for n=1:writtenParticles 
    fprintf(fileID, ...
            '%12.4e  %12.4e %12.4e  %12.4e %12.4e  %12.4e %12.4e  %12.4e %4d %4d\n',...
            xAstra(n),yAstra(n),zAstra(n),pxAstra(n),pyAstra(n),pzAstraRel(n), ...
            clockAstra,chargeAstra,particleIndex,statusFlag);
end

fclose (fileID)

%=====================================================================
%
% Preparation input file type cst_x*.txt for 2D version of Hellweg from
% input data for 3D version:
%   
%=====================================================================
%
% Structure of file 'cst_x_*.txt':
%
%  First line: "          x-position, [m]           z-angle, [rad]",
% second line: "--------------------------------------------------";
%
% Any next line:
% 1st  column - x-coordinate, cm
% 2nd  column - z-angle, rad
%

writtenParticles=nParticles
pTotHellwe2D=zeros(writtenParticles,1);
xHellweg2D=zeros(writtenParticles,1);
angHellweg2D=zeros(writtenParticles,1);
v_r_Hellweg2D=zeros(writtenParticles,1);

xHellweg2D=beamR;                          % cm
pTotHellwe2D=sqrt(beamBr.^2+beamBz.^2);    % d/l
angHellweg2D=beamBr./pTotHellwe2D;         % rad
v_rHellweg2D=beamBr*cLight;                % m/s

%
% Checking initial x-x' distribution:
%
figure (190)
plot(100*xHellweg2D,1000*angHellweg2D,'.r')
grid on
xlabel('x, cm','FontSize',16,'Color','m')
ylabel('x, mrad','FontSize',16,'Color','m')
title(['Initial Disribution: E_{ref}=', ...
       num2str(1e-3*averEnergy,'%7.3f'),' keV'],'FontSize',16,'Color','m')
   
% 
% Writing the file:
%
str='cst_x_Hellweg2D_fromHellweg3D.txt';
filenameWR=strcat(dirname,str)
fileID=fopen(filenameWR,'w')
% fprintf(fileID,'x-position, [m]    z-angle[rad] \n');
fprintf(fileID,'x-position, [m]    V_r [m/s] \n');
fprintf(fileID,'-----------------------------------\n');
for n=1:writtenParticles 
%    fprintf(fileID,'%12.4e      %12.4e \n',xHellweg2D(n),angHellweg2D(n));
    fprintf(fileID,'%12.4e      %12.4e \n',xHellweg2D(n),v_rHellweg2D(n));
end

fclose (fileID)






