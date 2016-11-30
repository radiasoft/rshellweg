%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%
%%%%%%%%%%   This is code to create a files with initial data for 
%%%%%%%%%%    PC codes "ASTRA" and "Hellweg2D": 
%%%%%%%%%%   created file names are "astra_hellweg2D.ini" and 
%%%%%%%%%%                          "hellweg2D_astra.dat"
%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

fnalHome=1;    % =1 for FNAL and =0 fore home

if (fnalHome == 0)
    cd ('C:/Users/Yury/My Personal Documents/My WORK/ESS/ASTRA/MARScomparison/spokeSingleModule/') 
    dirname='C:/Users/Yury/My Personal Documents/My WORK/ESS/ASTRA/MARScomparison/spokeSingleModule/';
elseif (fnalHome == 1)
    cd ('/home/eidelyur/radiaSoft/astra/comparison_Hellweg/') 
    dirname='/home/eidelyur/radiaSoft/astra/comparison_Hellweg/';
end

twoPi=2*3.14159265358;
mc2=0.51099893e6;       %    electronn mass, eV
% freqRF=2856e6;          %    RF frequency, Hz
lambdaZ=100.0;          % cm
cLight=2.999792458e10;  % cm/s
xAxisZ=0.0;             %    x-shift of Z axis, cm

nParticles=1000;                       % number of not reference particles
xAstra=zeros(nParticles,1);           % x-coordinate, m
yAstra=zeros(nParticles,1);           % y-coordinate, m
zAstra=zeros(nParticles,1);           % z-coordinate, m
energyAstra=zeros(nParticles,1);      % kinetic energy, eV
pAstra=zeros(nParticles,1);           % total momentum, eV
pzAstra=zeros(nParticles,1);          % pz-momentum, eV
pxAstra=zeros(nParticles,1);          % px-momentum, eV
pyAstra=zeros(nParticles,1);          % py-momentum, eV
prAstra=zeros(nParticles,1);          % pr-momentum, eV
pzAstraRel=zeros(nParticles,1);       % relative (regarding reference) pz-momentum, eV/c
alphaAzim=zeros(nParticles,1);        % uniform distributon ftom 0 till 2*pi, rad
clockAstra=0.;                        % clock value,   ns

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%       Reference particle:
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

EkinRef=30.000000e3;                         % eV
pzRef=sqrt(EkinRef*(EkinRef+2*mc2));         % eV
disp(['Ekin (eV)=',num2str(EkinRef,'%12.6e'),'; pz (eV)=',num2str(pzRef,'%12.6e')])
Rx=5e-3;                                     % x-size of the beam, m
Ry=5e-3;                                     % y-size of the beam, m
sigmaE=30;                                % sigma of Ekin, eV
sigmaAngle=5e-3;                             % sigma of initial transverse angle, rad


%
% Uniform distributions:
%
xAstra=Rx*(2*rand(nParticles,1)-1);
yAstra=Ry*(2*rand(nParticles,1)-1);

%
% Checking of distribution of initial particles:
%
figure (10)
plot(1e3*xAstra,1e3*yAstra,'.r')
grid on
xlabel('x, mm','FontSize',16,'Color','m')
ylabel('y, mm','FontSize',16,'Color','m')
title(['Initial Uniform Disribution: R_x=', ...
       num2str(1e3*Rx,'%4.2f'),' mm, R_y=', ...
       num2str(1e3*Ry,'%4.2f'),' mm'],'FontSize',16,'Color','m')

%
% Kinetic energy uniform distribution:
%
energyAstra=EkinRef+sigmaE*(2*rand(nParticles,1)-1);   % eV

%
% Checking of distribution of energy:
%
figure (20)
plot(1:nParticles,1e-3*energyAstra,'.r')
grid on
xlabel('Particle','FontSize',16,'Color','m')
ylabel('E_{kin}, keV','FontSize',16,'Color','m')
title(['Initial Disribution: E_{ref}=', ...
       num2str(1e-6*EkinRef,'%7.3f'),' MeV, \sigma_E=', ...
       num2str(1e-3*sigmaE,'%4.2f'),' keV'],'FontSize',16,'Color','m')
ylim(1.e-3*[EkinRef-1.1*sigmaE,EkinRef+1.1*sigmaE])

pAstra=sqrt(energyAstra.*(energyAstra+2*mc2));   % eV

%
% Checking of distribution of p-total:
%
figure (30)
plot(1:nParticles,1e-3*pAstra,'.r')
grid on
xlabel('Particle','FontSize',16,'Color','m')
ylabel('p_{total}, keV/c','FontSize',16,'Color','m')
title(['Initial Disribution: (p_z)_{ref}=', ...
       num2str(1e-6*pzRef,'%7.3f'),' MeV, \sigma_E=', ...
       num2str(1e-3*sigmaE,'%4.2f'),' keV'],'FontSize',16,'Color','m')

pzAstra=pAstra.*cos(sigmaAngle*rand(nParticles,1));   % eV
pzAstraRel=pzAstra-pzRef;                             % eV

figure (35)
plot(1:nParticles,1e-3*pAstra,'.r',1:nParticles,1e-3*pzAstra,'.b')
grid on
xlabel('Particle','FontSize',16,'Color','m')
ylabel('p_{total} & p_z, keV','FontSize',16,'Color','m')
title(['Initial Disribution: (p_z)_{ref}=', ...
       num2str(1e-6*pzRef,'%7.3f'),' MeV, \sigma_E=', ...
       num2str(1e-3*sigmaE,'%4.2f'),' keV'],'FontSize',16,'Color','m')
legend('Total momentum','Longitudinal momemtum','Location','s')
%
% Checking of distribution of relative pz:
%
figure (40)
plot(1:nParticles,1e-3*pzAstraRel,'.r')
grid on
xlabel('Particle','FontSize',16,'Color','m')
ylabel('p_z-p_{ref}, keV','FontSize',16,'Color','m')
title(['Initial Disribution: (p_z)_{ref}=', ...
       num2str(1e-3*pzRef,'%7.3f'),' keV/c, \sigma_E=', ...
       num2str(1e-3*sigmaE,'%4.2f'),' keV'],'FontSize',16,'Color','m')

   
alphaAzim=twoPi*rand(nParticles,1);

figure (50)
plot(cos(alphaAzim),sin(alphaAzim),'.r')
grid on
axis square
                             
prAstra=sqrt(pAstra.^2-pzAstra.^2);                      % eV

figure (60)
plot(1:nParticles,1e-3*prAstra,'.r')
grid on
xlabel('Particle','FontSize',16,'Color','m')
ylabel('p_r, keV','FontSize',16,'Color','m')
title(['Initial Disribution: E_{ref}=', ...
       num2str(1e-3*EkinRef,'%7.3f'),' keV, \sigma_E=', ...
       num2str(1e-3*sigmaE,'%4.2f'),' keV'],'FontSize',16,'Color','m')

for n=1:nParticles
    pxAstra(n)=prAstra(n)*cos(alphaAzim(n));            % eV
    pyAstra(n)=prAstra(n)*sin(alphaAzim(n));            % eV
end

figure (70)
plot(1e-3*pxAstra,1e-3*pyAstra,'.r')
grid on
xlabel('p_x, keV','FontSize',16,'Color','m')
ylabel('p_y, keV','FontSize',16,'Color','m')
axis square
xlim([-0.8,0.8])
ylim([-1.0,1.0])
title(['Initial Disribution: E_{ref}=', ...
       num2str(1e-3*EkinRef,'%7.3f'),' keV, \sigma_E=', ...
       num2str(1e-3*sigmaE,'%4.2f'),' keV'],'FontSize',16,'Color','m')

clockAstra=0.;                        % clock value,   ns
particleIndex=1;                      % electron
% particleIndex=3;                      % proton

beamCurrent=100;                                              % mA
writtenParticles=1000;                                          % number of macroparticles
chargeAstra=1e6*beamCurrent*lambdaZ/cLight/writtenParticles   % charge of macroparticle,  nC

str='astra_hellweg2D_';
str=strcat(str,int2str(beamCurrent),'_',int2str(writtenParticles),'.ini')
filenameWR=strcat(dirname,str)

%
% Structure of file 'astra_hellweg2D_current_particles.ini'
%
% 1st  column - x-coordinate, m
% 2nd  column - y-coordinate, m
% 3rd  column - z-coordinate, m
% 4th  column - px-coordinate, eV/c
% 5th  column - py-coordinate, eV/c
% 6th  column - pz-coordinate, eV/c
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

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%
%%%%%%%%%% Creating initial file with positions and angles of the particles 
%%%%%%%%%% for Hellweg2D which is equivalence to the initial file for ASTRA
%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

xHellweg=zeros(nParticles,1);             % x-coordinate, m
angHellweg=zeros(nParticles,1);           % x-coordinate, m

xHellweg=xAstra;
angHellweg=sign(pxAstra).*atan2(prAstra,pzAstra);

figure (75)
plot(1:nParticles,1000*xHellweg,'.r')
grid on
xlabel('Particle','FontSize',16,'Color','m')
ylabel('x, mm','FontSize',16,'Color','m')
title(['Initial Uniform Disribution: R_x=', ...
       num2str(1e3*Rx,'%7.3f'),' mm, x_{RMS}=', ...
       num2str(1e3*std(xHellweg),'%7.3f'),' mm'],'FontSize',16,'Color','m')


figure (80)
plot(1:nParticles,1000*angHellweg,'.r')
grid on
xlabel('Particle','FontSize',16,'Color','m')
ylabel('\theta_x, mrad','FontSize',16,'Color','m')
title(['Init. Uniform Disribution of \theta_x: max=', ...
       num2str(1e3*sigmaAngle,'%7.3f'),'mrad, RMS=', ...
       num2str(1e3*std(angHellweg),'%7.3f'),' mrad'],'FontSize',16,'Color','m')
   
beamCurrent=100;                                              % mA
writtenParticles=1000;                                          % number of macroparticles

str='cst_x_Astra-Hellweg_';
str=strcat(str,int2str(beamCurrent),'_',int2str(writtenParticles),'.txt')
filenameWR=strcat(dirname,str)
%
% Structure of file 'cst_x_*.txt':
%
%  First line: "          x-position, [cm]           z-angle, [rad]",
% second line: "--------------------------------------------------";
%
% Any next line:
% 1st  column - x-coordinate, cm
% 2nd  column - z-angle, rad
%

fileID=fopen(filenameWR,'w')
fprintf(fileID,'          x-position, [m]           z-angle, [rad]\n');
fprintf(fileID,'--------------------------------------------------\n');
for n=1:writtenParticles 
    fprintf(fileID,'%12.4e  %12.4e \n',xHellweg(n),angHellweg(n));
end

fclose (fileID)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%
%%%%%%%%%% Creating initial file with energy of the particles 
%%%%%%%%%% for Hellweg2D which is equivalence to the initial file for ASTRA
%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Structure of file 'energyDistribution.txt':
%
%  First line: "   energy [MeV]         ",
% second line: "------------------------";
%
% Any next line:
% 1st  column - energy of the particle, MeV
%
filenameWR=strcat(dirname,'energyDistribution.txt')

fileID=fopen(filenameWR,'w')
fprintf(fileID,'Energy [MeV]\n');
fprintf(fileID,'------------\n');
for n=1:nParticles 
%    fprintf(fileID,'%6d  %12.6g \n',n,1e-6*energyAstra(n));
    fprintf(fileID,'%12.6g \n',1e-6*energyAstra(n));
end

fclose (fileID)






%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%
%%%%%%%%%% Creating initial file for MARS which is equivalence to 
%%%%%%%%%% the initial file for ASTRA
%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Structure of file 'spokeInitialASTRAparticles.data'
%
% 1st  column - x-coordinate, cm
% 2nd  column - y-coordinate, cm
% 3rd  column - z-coordinate, cm
% 4th  column - dcx=cos(Phi)*sin(Theta) - cosine x-direction=px/pTotal
% 5th  column - dcy=sin(Phi)*sin(Theta) - cosine y-direction=py/pTotal
% 6th  column - dcz=cos(Theta)          - cosine z-direction=pz/pTotal
% 7th  column - energy, GeV
%
xMARS=zeros(nParticles,1);            % x-coordinate, cm
yMARS=zeros(nParticles,1);            % y-coordinate, cm
zMARS=zeros(nParticles,1);            % z-coordinate, cm
dcxMARS=zeros(nParticles,1);          % cosine x-direction
dcyMARS=zeros(nParticles,1);          % cosine y-direction
dczMARS=zeros(nParticles,1);          % cosine z-direction
energyMARS=zeros(nParticles,1);       % kinetic energy, GeV

for n=1:nParticles
    xMARS(n)=100.*xAstra(n);  
    yMARS(n)=100.*yAstra(n);  
    zMARS(n)=100.*zAstra(n);  
    dcxMARS(n)=pxAstra(n)/pAstra(n);
    dcyMARS(n)=pyAstra(n)/pAstra(n);
    dczMARS(n)=pzAstra(n)/pAstra(n);
    energyMARS(n)=1.e-9*energyAstra(n);
end

if (fnalHome == 0)
    cd ('C:/Users/Yury/My Personal Documents/My WORK/ESS/ASTRA/MARScomparison/spokeSingleModule/') 
    dirname='C:/Users/Yury/My Personal Documents/My WORK/ESS/ASTRA/MARScomparison/spokeSingleModule/';
elseif (fnalHome == 1)
    cd ('/home/eidelyur/ess/astra/MARScomparison/spokeSingleModule/') 
    dirname='/home/eidelyur/ess/astra/MARScomparison/spokeSingleModule/';
end

filenameWR=strcat(dirname,'spokeInitialASTRAparticles.data')

fileID=fopen(filenameWR,'w')

for n=1:20    % nParticles 
    fprintf(fileID,'%14.8e %14.8e %14.8e %14.8e %14.8e %14.8e %14.8e \n',...
            xMARS(n),yMARS(n),zMARS(n),dcxMARS(n),dcyMARS(n),dczMARS(n),energyMARS(n));
end

fclose (fileID)


