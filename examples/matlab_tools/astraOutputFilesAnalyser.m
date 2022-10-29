%
% This code analyzes output ASTRA files
%
twoPi=2*3.14159265358;
% mc2MeV=938.272046;       %    proton mass, MeV
mc2=0.51099893e6;          %    electronn mass, eV

%====================================================
%
% Where are the ASTRA's output files for Hellweg:
%
%====================================================

fnalHome=1;    % =1 for FNAL and =0 fore home
if (fnalHome == 0)
    cd ('C:/Users/Yury/My Personal Documents/My WORK/ESS/ASTRA/highBeta') 
    dirname='C:/Users/Yury/My Personal Documents/My WORK/ESS/ASTRA/highBeta/';
elseif (fnalHome == 1)
    cd ('/home/eidelyur/radiaSoft/astra/comparison_Hellweg/Hellweg3D') 
    dirname='/home/eidelyur/radiaSoft/astra/comparison_Hellweg/Hellweg3D/';
end

%------------------------------------------------
%
%   File comparisonHellweg_6cells.track.NNrun: 2
%
%------------------------------------------------------
% filename=strcat(dirname,'TWS_SbandTracks.data')
filename=strcat(dirname,'comparisonHellweg_6cells.track.002')
%
% Each line in the file:
%     1st column - number of particle
%     2nd column - statflag
%     3rd column - z, m
%     4th column - x, mm
%     5th column - y, mm
%     6th column - Ez, V/m
%     7th column - Er or Ex, V/m
%     8th column - 0.0 or Ey, V/m
%
fileID=fopen(filename,'r')

while ~feof(fileID)
    tline = fgetl(fileID);
    if tline ~= -1
        [C,count] = fscanf(fileID, '%d %d %e %e %e %e %e %e',inf);
    end
%     display(['count=',num2str(count)]);
end
fclose(fileID)
count;

lines=count/8

A=sscanf(tline,'%d %d %e %e %e %e %e %e');
nParticles=A(1);

points=round((lines+1)/nParticles)

statFlag=zeros(nParticles,points);
z=zeros(nParticles,points);                 % m
x=zeros(nParticles,points);                 % mm
xd=zeros(nParticles,points);                % mrad
y=zeros(nParticles,points);                 % mm
Ez=zeros(nParticles,points);                % V/m
Ex=zeros(nParticles,points);                % V/m 
Ey=zeros(nParticles,points);                % V/m 
r=zeros(nParticles,points);                 % mm

statFlag(1)=A(2);
z(nParticles,1)=A(3);
x(nParticles,1)=A(4);
y(nParticles,1)=A(5);
r(nParticles,1)=sqrt(x(nParticles,1)^2+y(nParticles,1)^2);   
Ez(nParticles,1)=A(6);
Ex(nParticles,1)=A(7);
Ey(nParticles,1)=A(8);

n=1;
currPoint=2;
for k=1:lines
    part=C(n);                                   % 
    statFlag(part,currPoint)=C(n+1);             % 
    z(part,currPoint)=C(n+2);                    % m
    x(part,currPoint)=C(n+3);                    % mm
    xd(part,currPoint-1)=(x(part,currPoint)-x(part,currPoint-1))/ ...
                         (z(part,currPoint)-z(part,currPoint-1));       % mrad
    y(part,currPoint)=C(n+4);                    % mm
    r(part,currPoint)=sqrt(x(part,currPoint)^2+y(part,currPoint)^2);    % mm
    Ez(part,currPoint)=C(n+5);                   % V/m
    Ex(part,currPoint)=C(n+6);                   % V/m
    Ey(part,currPoint)=C(n+7);                   % V/m
    n=n+8;
    if (part == nParticles)
        currPoint=currPoint+1;
    end
end

particlesXY=100;

figure (5)
plot (x(:,1),y(:,1),'.r')
grid on
xlabel('x, mm','FontSize',16,'Color','m')
ylabel('y, mm','FontSize',16,'Color','m')
title(['Initial Distribution: ',int2str(particles), ' Particles'],'FontSize',16,'Color','m')
axis square

particlesR=100;

figure (10)
for k=1:particlesR
    plot (100*z(k,:),r(k,:),'r')
    hold on
end
grid on
xlabel('z, cm','FontSize',16,'Color','m')
ylabel('r, mm','FontSize',16,'Color','m')
title(['Trajectories: ',int2str(particlesR), ' Particles'],'FontSize',16,'Color','m')

particlesE=1;

figure (15)
for k=1:particlesE
    if (k == 1)
        plot (100*z(k,:),Ez(k,:),'r')
    else
        plot (100*z(k,:),Ez(k,:),'.b')
    end
    hold on
end
grid on
xlabel('z, cm','FontSize',16,'Color','m')
ylabel('E_z, V/m','FontSize',16,'Color','m')
title(['Field: ',int2str(particlesE), ' Particles'],'FontSize',16,'Color','m')

particlesX=100;

figure (20)
for k=1:particlesX
    plot (100*z(k,:),x(k,:),'r')
    hold on
end
grid on
xlabel('z, cm','FontSize',16,'Color','m')
ylabel('x, mm','FontSize',16,'Color','m')
title(['Trajectories: ',int2str(particlesX), ' Particles'],'FontSize',16,'Color','m')
xlim([0,27])

%------------------------------------------------
%
%   File comparisonHellweg_6cells.track.NNrun: 1
%
%------------------------------------------------------
% filename=strcat(dirname,'TWS_SbandTracks.data')
filename=strcat(dirname,'comparisonHellweg_6cells.track.001')
%
% Each line in the file:
%     1st column - number of particle
%     2nd column - statflag
%     3rd column - z, m
%     4th column - x, mm
%     5th column - y, mm
%     6th column - Ez, V/m
%     7th column - Er or Ex, V/m
%     8th column - 0.0 or Ey, V/m
%
fileID=fopen(filename,'r')

while ~feof(fileID)
    tline = fgetl(fileID);
    if tline ~= -1
        [C,count] = fscanf(fileID, '%d %d %e %e %e %e %e %e',inf);
    end
%     display(['count=',num2str(count)]);
end
fclose(fileID)
count;

lines=count/8

A=sscanf(tline,'%d %d %e %e %e %e %e %e');
nParticles=A(1);

points=round((lines+1)/nParticles)

statFlag=zeros(nParticles,points);
z=zeros(nParticles,points);                 % m
x=zeros(nParticles,points);                 % mm
xd=zeros(nParticles,points);                % mrad
y=zeros(nParticles,points);                 % mm
Ez=zeros(nParticles,points);                % V/m
Ex=zeros(nParticles,points);                % V/m 
Ey=zeros(nParticles,points);                % V/m 
r=zeros(nParticles,points);                 % mm

statFlag(1)=A(2);
z(nParticles,1)=A(3);                       % m
x(nParticles,1)=A(4);                       % mm
y(nParticles,1)=A(5);                       % mm
r(nParticles,1)=sqrt(x(nParticles,1)^2+y(nParticles,1)^2);      % mm   
Ez(nParticles,1)=A(6);                      % V/m
Ex(nParticles,1)=A(7);                      % V/m
Ey(nParticles,1)=A(8);                      % V/m

n=1;
currPoint=1;
for k=1:lines
    part=C(n);                                   % 
    statFlag(part,currPoint)=C(n+1);             % 
    z(part,currPoint)=C(n+2);                    % m
    x(part,currPoint)=C(n+3);                    % mm
    if (currPoint > 1)
        xd(part,currPoint-1)=(x(part,currPoint)-x(part,currPoint-1))/ ...
                             (z(part,currPoint)-z(part,currPoint-1));   % mrad
    end
    y(part,currPoint)=C(n+4);                    % mm
    r(part,currPoint)=sqrt(x(part,currPoint)^2+y(part,currPoint)^2);    % mm
    Ez(part,currPoint)=C(n+5);                   % V/m
    Ex(part,currPoint)=C(n+6);                   % V/m
    Ey(part,currPoint)=C(n+7);                   % V/m
    n=n+8;
    if (part == nParticles)
        currPoint=currPoint+1;
    end
end

particlesXY=1000;

zMin=z(1,1);
zMax=z(1,points);

figure (105)
plot (x(:,1),y(:,1),'.r',x(:,points),y(:,points),'.b')
grid on
xlabel('x, mm','FontSize',16,'Color','m')
ylabel('y, mm','FontSize',16,'Color','m')
title(['Initial and Final Distributions: ',int2str(particlesXY), ' Particles'],'FontSize',16,'Color','m')
axis square
legend(['z_i=',num2str(100*zMin,'%6.3f'),' cm'],['z_f=',num2str(100*zMax,'%6.3f'),' cm'],'Location','se')

particlesR=100;

figure (110)
for k=1:particlesR
    plot (100*z(k,:),r(k,:),'.r')
    hold on
end
grid on
xlabel('z, cm','FontSize',16,'Color','m')
ylabel('r, mm','FontSize',16,'Color','m')
title(['TW 6 cells Structure (Tracks): ',int2str(particlesR), ' Particles'],'FontSize',16,'Color','m')

particlesE=100;

figure (115)
for k=1:particlesE
    if (k == 1)
        plot (100*z(k,:),Ez(k,:),'.r')
    else
        plot (100*z(k,:),Ez(k,:),'.b')
    end
    hold on
end
grid on
xlabel('z, cm','FontSize',16,'Color','m')
ylabel('E_z, V/m','FontSize',16,'Color','m')
title(['Field: ',int2str(particlesE), ' Particles'],'FontSize',16,'Color','m')

particlesX=100;

figure (120)
for k=1:particlesX
    plot (100*z(k,:),x(k,:),'.r')
    hold on
end
grid on
xlabel('z, cm','FontSize',16,'Color','m')
ylabel('x, mm','FontSize',16,'Color','m')
title(['TW 6 cells Structure (Tracks): ',int2str(particlesX), ' Particles'],'FontSize',16,'Color','m')

zSect1=199;              % number of the first z-crossection
zSect2=points-1;         % number of the last z-crossection
zSect1=425;              % number of the first z-crossection
zSect2=425;         % number for exit from linac


figure (130)
% plot (x(:,zSect1),xd(:,zSect1),'.r',x(:,zSect2),xd(:,zSect2),'.b')
plot (x(:,zSect1),xd(:,zSect1),'.r')
grid on
xlabel('x, mm','FontSize',16,'Color','m')
ylabel('x'', mrad','FontSize',16,'Color','m')
% title(['Initial (z= ',num2str(100*z(1,zSect1),'%6.3f'), ' cm) and Final (z=', ...
%       num2str(100*z(1,zSect2),'%6.3f'),' cm) Distributions'], ...
%       'FontSize',16,'Color','m')
% legend('Initial','Final','Location','se')
 title('Transverse Phase Space: Exit from Linac', 'FontSize',16,'Color','m')
xlim([-5,5])
ylim([-3.5,3.50])

%
% "Movie" with distibutions:
%
zSect1=415;              % number inside of 6th cell
zSect2=425;         % number for exit from linac

xMax=max(max(x(:,zSect1:zSect2)));
xMin=min(min(x(:,zSect1:zSect2)));
dxMax=max(max(xd(:,zSect1:zSect2)));
dxMin=min(min(xd(:,zSect1:zSect2)));
zCell=zeros(8,1);
zCell(1)=5.2464;                   % cm
for k=2:7
    zCell(k)=zCell(k-1)+3.5;       % cm
end
zCell(8)=zCell(7)+5.2464;          % cm

for n=zSect1:zSect2
    figure (1300)
    plot (x(:,n),xd(:,n),'.r')
    xlim([xMin,xMax])
    ylim([dxMin,dxMax])
    grid on
    xlabel('x, mm','FontSize',16,'Color','m')
    ylabel('x'', mrad','FontSize',16,'Color','m')
    if (100*z(1,n) >= zCell(8))
        title(['Out of linac: z= ',num2str(100*z(1,n),'%6.3f'), ' cm'],'FontSize',16,'Color','m')
    end
    if (100*z(1,n) <= zCell(1))
        title(['Entrance to linac: z= ',num2str(100*z(1,n),'%6.3f'), ' cm'],'FontSize',16,'Color','m')
    end
    if ((100*z(1,n) >= zCell(7)) && (100*z(1,n) < zCell(8)))
        title(['Exit from linac: z= ',num2str(100*z(1,n),'%6.3f'), ' cm'],'FontSize',16,'Color','m')
    end
    if ((100*z(1,n) >= zCell(1)) && (100*z(1,n) < zCell(7)))
        nCell=floor((100*z(1,n)-zCell(1))/3.5)+1;
        title(['Linac, Cell ',int2str(nCell),': z= ',num2str(100*z(1,n),'%6.3f'), ' cm'],'FontSize',16,'Color','m')
    end
    pause(2);
end
%
% Calculated x-emittances:
%
for n=1:points
    for k=1:nParticles
        xDx(k,n)=x(k,n)*xd(k,n);                     % mm * mrad = um
    end
end

% xAstra_std=1000*std(xAstra);                             % mm
% xdAstra_std=1000*std(xdAstra);                           % mrad
% xDxAstra=zeros(nParticles,1);
% xDxAstra_std=(1000*xAstra).*(1000*xdAstra);              % mm*mrad = um
% xDxAstra_std=std(xDxAstra);
% x_emit=sqrt(xAstra_std^2*xdAstra_std^2-xDxAstra_std^2)   % mm*mrad = um

for n=zSect1:zSect2
    xAstra_std(n)=std(x(:,n));                      % mm
    xdAstra_std(n)=std(xd(:,n));                    % mrad
    xDxAstra_std(n)=std(xDx(:,n));                  % mm * mrad = um
    x_emitAstra(n)=sqrt(xAstra_std(n)^2*xdAstra_std(n)^2- ...
                        xDxAstra_std(n)^2);         % mm * mrad = um
end

figure (140)
plot (100*z(1,zSect1:zSect2),x_emitAstra(zSect1:zSect2),'.r')
grid on
xlabel('z, cm','FontSize',16,'Color','m')
ylabel('\epsilon_x, mm \cdot mrad','FontSize',16,'Color','m')
title(['z_1= ',num2str(100*z(1,zSect1),'%6.3f'), ' cm, z_2=',num2str(100*z(1,zSect2),'%6.3f'), ' cm'], ...
      'FontSize',16,'Color','m')
%

%------------------------------------------------
%
%   File comparisonHellweg_6cells.Xemit.NNrun: 2
%
%------------------------------------------------------
% filename=strcat(dirname,'TWS_SbandTracks.data')
filename=strcat(dirname,'comparisonHellweg_6cells.Xemit.001')
%
% Each line in the file:
%     1st column - z, m
%     2nd column - time, ns
%     3rd column - x-average, mm
%     4th column - x, rms mm
%     5th column - x' rms, mm
%     6th column - x-emittance normilyzes, pi mrad*mm
%     7th column - x*x'-average, mrad (?)
%
fileID=fopen(filename,'r')

while ~feof(fileID)
    tline = fgetl(fileID);
    if tline ~= -1
        [C,count] = fscanf(fileID, '%e %e %e %e %e %e %e',inf);
    end
%     display(['count=',num2str(count)]);
end
fclose(fileID)
count;

lines=count/7

A=sscanf(tline,'%e %e %e %e %e %e %e');


zXemit=zeros(lines+1,1);                   % m
timeX=zeros(lines+1,1);                    % ns
xAver=zeros(lines+1,1);                    % mm
xRMS=zeros(lines+1,1);                     % mm
xdRMS=zeros(lines+1,1);                    % mrad
xEmitNorm=zeros(lines+1,1);                % pi mrad*mm 
x_xdAver=zeros(lines+1,1);                 % mrad (?) 

zXemit(1)=A(1);
timeX(1)=A(2);
xAver(1)=A(3);
xEmitRMS(1)=A(4);
xdEmitRMS(1)=A(5);
xEmitNorm(1)=A(6);
x_xdAver(1)=A(7);


n=1;
for k=1:lines
    zXemit(k+1)=C(n);                    % m
    timeX(k+1)=C(n+1);                   % ns
    xAver(k+1)=C(n+2);                   % mm
    xEmitRMS(k+1)=C(n+3);                % mm
    xdEmitRMS(k+1)=C(n+4);               % mrad
    xEmitNorm(k+1)=C(n+5);               % pi mm*mrad
    x_dxAver(k+1)=C(n+6);                % mrad (?)
    n=n+7;
end

figure (25)
plot (zXemit,xAver,'r','LineWidth',2)
grid on
xlabel('z, m','FontSize',16,'Color','m')
ylabel('<x>, mm','FontSize',16,'Color','m')
title('','FontSize',16,'Color','m')
title('TW 6 cells Structure: 1000 Particles','FontSize',16,'Color','m')

%------------------------------------------------
%
%   File comparisonHellweg_6cells.Xemit.NNrun: 1
%
%------------------------------------------------------
% filename=strcat(dirname,'TWS_SbandTracks.data')
filename=strcat(dirname,'comparisonHellweg_6cells.Xemit.001')
%
% Each line in the file:
%     1st column - z, m
%     2nd column - time, ns
%     3rd column - x-average, mm
%     4th column - x, rms mm
%     5th column - x' rms, mm
%     6th column - x-emittance normilyzes, pi mrad*mm
%     7th column - x*x'-average, mrad (?)
%
fileID=fopen(filename,'r')

while ~feof(fileID)
    tline = fgetl(fileID);
    if tline ~= -1
        [C,count] = fscanf(fileID, '%e %e %e %e %e %e %e',inf);
    end
%     display(['count=',num2str(count)]);
end
fclose(fileID)
count;

lines=count/7

A=sscanf(tline,'%e %e %e %e %e %e %e');

zXemit=zeros(lines+1,1);                   % m
timeX=zeros(lines+1,1);                    % ns
xAver=zeros(lines+1,1);                    % mm
xRMS=zeros(lines+1,1);                     % mm
xdRMS=zeros(lines+1,1);                    % mrad
xEmitNorm=zeros(lines+1,1);                % pi mrad*mm 
x_xdAver=zeros(lines+1,1);                 % mrad (?) 

zXemit(1)=A(1);
timeX(1)=A(2);
xAver(1)=A(3);
xEmitRMS(1)=A(4);
xdEmitRMS(1)=A(5);
xEmitNorm(1)=A(6);
x_xdAver(1)=A(7);


n=1;
for k=1:lines
    zXemit(k+1)=C(n);                    % m
    timeX(k+1)=C(n+1);                   % ns
    xAver(k+1)=C(n+2);                   % mm
    xEmitRMS(k+1)=C(n+3);                % mm
    xdEmitRMS(k+1)=C(n+4);               % mrad
    xEmitNorm(k+1)=C(n+5);               % pi mm*mrad
    x_dxAver(k+1)=C(n+6);                % mrad (?)
    n=n+7;
end

figure (125)
plot (100*zXemit,xAver,'r')
grid on
xlabel('z, cm','FontSize',16,'Color','m')
ylabel('<x>, mm','FontSize',16,'Color','m')
title('ASTRA; run 1','FontSize',16,'Color','m')

figure (135)
plot (100*zXemit,xEmitRMS,'r')
grid on
xlabel('z, cm','FontSize',16,'Color','m')
ylabel('x_{RMS}, mm','FontSize',16,'Color','m')
title('ASTRA; run 1','FontSize',16,'Color','m')

figure (145)
plot (100*zXemit,xdEmitRMS,'r')
grid on
xlabel('z, cm','FontSize',16,'Color','m')
ylabel('x''_{RMS}, mrad','FontSize',16,'Color','m')
title('ASTRA; run 1','FontSize',16,'Color','m')

figure (155)
plot (100*zXemit,xEmitNorm,'r')
grid on
xlabel('z, cm','FontSize',16,'Color','m')
ylabel('\epsilon_{norm}, \pi mm \cdot mrad','FontSize',16,'Color','m')
title('ASTRA; run 1','FontSize',16,'Color','m')

%------------------------------------------------
%
%   File comparisonHellweg_6cells.PScan.run: 1
%
%------------------------------------------------------
filename=strcat(dirname,'comparisonHellweg_6cells.PScan.001')
%
% The structure of the file:
%     1st column - phase, degree
%     2nd column - Ekin, MeV
%     3rd and 4th columns - dummy
%
fileID=fopen(filename,'r')

while ~feof(fileID)
    tline = fgetl(fileID)
    if tline ~= -1
        [APS_C1,count] = fscanf(fileID, '%e %e %e %e',inf);
    end
%     display(['count=',num2str(count)]);
end
fclose(fileID)
count;

linesPS_C1=count/4

zPS_C1=zeros(linesPS_C1,1);
EzPS_C1=zeros(linesPS_C1,1);

n=1;
for k=1:linesPS_C1
    zPS_C1(k)=APS_C1(n);                    % degree
    EzPS_C1(k)=APS_C1(n+1);                 % MeV
    n=n+4;
end

figure (10)
plot(zPS_C1,EzPS_C1,'r','LineWidth',2)
grid on
xlabel('\phi, degree','FontSize',16,'Color','m')
ylabel('E_{kin}, MeV','FontSize',16,'Color','m')
title('ESS High-Beta 5-Cell Cavity (1st)','FontSize',16,'Color','m')


%------------------------------------------------
%
%   File comparisonHellweg_6cells.ref.run: 1
%
%------------------------------------------------------
filename=strcat(dirname,'comparisonHellweg_6cells.ref.001')
%
% The structure of the file:
%     1st column - dummy
%     2nd column - z, m
%     3rd column - pz, MeV/c
%     4-7th columns - dummy
%
fileID=fopen(filename,'r')

while ~feof(fileID)
    tline = fgetl(fileID)
    if tline ~= -1
        [ARef_C1,count] = fscanf(fileID, '%e %e %e %e %e %e %e',inf);
    end
%     display(['count=',num2str(count)]);
end
fclose(fileID)
count;

linesRef_C1=count/7

zRef_C1=zeros(linesRef_C1,1);
pzRef_C1=zeros(linesRef_C1,1);
EkinRef_C1=zeros(linesRef_C1,1);


n=1;
for k=1:linesRef_C1
    zRef_C1(k)=ARef_C1(n+1);                    % m
    pzRef_C1(k)=ARef_C1(n+2);                   % MeV/c
    EkinRef_C1(k)=sqrt(pzRef_C1(k)^2+(1e-6*mc2)^2)-1e-6*mc2;
    n=n+7;
end

figure (20)
zNorm1=1.5/zRef_C1(linesRef_C1);
plot(zNorm1*zRef_C1,EkinRef_C1,'r','LineWidth',2)
grid on
xlabel('z, m','FontSize',16,'Color','m')
ylabel('E_{kin}, MeV','FontSize',16,'Color','m')
title('Acceleration for TWS with 6 Cells','FontSize',16,'Color','m')


%------------------------------------------------
%
%   File comparisonHellweg_6cells.Zemit.run: 1
%
%------------------------------------------------------
filename=strcat(dirname,'comparisonHellweg_6cells.Zemit.001')
%
% The structure of the file:
%     1st column - z, m
%     2nd column - dummy
%     3rd column - Ekin, MeV
%     4-7th columns - dummy
%
fileID=fopen(filename,'r')

while ~feof(fileID)
    tline = fgetl(fileID)
    if tline ~= -1
        [AZe_C1,count] = fscanf(fileID, '%e %e %e %e %e %e %e',inf);
    end
%     display(['count=',num2str(count)]);
end
fclose(fileID)
count;

linesZe_C1=count/7

zZe_C1=zeros(linesZe_C1,1);
EkinZe_C1=zeros(linesZe_C1,1);


n=1;
for k=1:linesZe_C1
    zZe_C1(k)=AZe_C1(n);                    % m
    EkinZe_C1(k)=AZe_C1(n+2);               % MeV
    n=n+7;
end

figure (30)
plot(zZe_C1,EkinZe_C1,'r','LineWidth',2)
grid on
xlabel('z, m','FontSize',16,'Color','m')
ylabel('E_{kin}, MeV','FontSize',16,'Color','m')
title('','FontSize',16,'Color','m')


%------------------------------------------------
%
%   File project.comparisonHellweg_6cells.run: 1
%
%------------------------------------------------------
filename=strcat(dirname,'comparisonHellweg_6cells.Sigma.001')
%
% The structure of the file:
%     1st column - z, m
%     2nd column - Ekin, MeV
%     3-23th columns - dummy
%
fileID=fopen(filename,'r')

while ~feof(fileID)
    tline = fgetl(fileID)
    if tline ~= -1
        [ASi_C1,count] = fscanf(fileID, ...
            '%e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e',inf);
    end
%     display(['count=',num2str(count)]);
end
fclose(fileID)
count;

linesSi_C1=count/23

zSi_C1=zeros(linesSi_C1,1);
EkinSi_C1=zeros(linesSi_C1,1);


n=1;
for k=1:linesSi_C1
    zSi_C1(k)=ASi_C1(n);                    % m
    EkinSi_C1(k)=ASi_C1(n+1);               % MeV
    n=n+23;
end

figure (40)
plot(zSi_C1,EkinSi_C1,'r','LineWidth',2)
grid on
xlabel('z, m','FontSize',16,'Color','m')
ylabel('E_{kin}, MeV','FontSize',16,'Color','m')
title('','FontSize',16,'Color','m')

%------------------------------------------------
%
%   File comparisonHellweg_6cells.PScan.run: 2
%
%------------------------------------------------------
filename=strcat(dirname,'comparisonHellweg_6cells.PScan.002')
%
% The structure of the file:
%     1st column - phase, degree
%     2nd column - Ekin, MeV
%     3rd and 4th columns - dummy
%
fileID=fopen(filename,'r')

while ~feof(fileID)
    tline = fgetl(fileID)
    if tline ~= -1
        [APS_C2,count] = fscanf(fileID, '%e %e %e %e',inf);
    end
%     display(['count=',num2str(count)]);
end
fclose(fileID)
count;

linesPS_C2=count/4

zPS_C2=zeros(linesPS_C2,1);
EzPS_C2=zeros(linesPS_C2,1);

n=1;
for k=1:linesPS_C2
    zPS_C2(k)=APS_C2(n);                    % degree
    EzPS_C2(k)=APS_C2(n+1);                 % MeV
    n=n+4;
end

figure (11)
plot(zPS_C2,EzPS_C2,'r','LineWidth',2)
grid on
xlabel('\phi, degree','FontSize',16,'Color','m')
ylabel('E_{kin}, MeV','FontSize',16,'Color','m')
title('','FontSize',16,'Color','m')


%------------------------------------------------
%
%   File comparisonHellweg_6cells.ref.run: 2
%
%------------------------------------------------------
filename=strcat(dirname,'comparisonHellweg_6cells.ref.002')
%
% The structure of the file:
%     1st column - dummy
%     2nd column - z, m
%     3rd column - pz, MeV/c
%     4-7th columns - dummy
%
fileID=fopen(filename,'r')

while ~feof(fileID)
    tline = fgetl(fileID)
    if tline ~= -1
        [ARef_C2,count] = fscanf(fileID, '%e %e %e %e %e %e %e',inf);
    end
%     display(['count=',num2str(count)]);
end
fclose(fileID)
count;

linesRef_C2=count/7

zRef_C2=zeros(linesRef_C2,1);
pzRef_C2=zeros(linesRef_C2,1);
EkinRef_C2=zeros(linesRef_C2,1);


n=1;
for k=1:linesRef_C2
    zRef_C2(k)=ARef_C2(n+1);                    % m
    pzRef_C2(k)=ARef_C2(n+2);                   % MeV/c
    EkinRef_C2(k)=sqrt(pzRef_C2(k)^2+(1e-6*mc2)^2)-1e-6*mc2;
    n=n+7;
end

figure (21)
zNorm2=1.5/zRef_C2(linesRef_C2);
plot(zNorm2*zRef_C2,EkinRef_C2,'r','LineWidth',2)
grid on
xlabel('z, m','FontSize',16,'Color','m')
ylabel('E_{kin}, MeV','FontSize',16,'Color','m')
title('','FontSize',16,'Color','m')


%------------------------------------------------
%
%   File comparisonHellweg_6cells.Zemit.run: 2
%
%------------------------------------------------------
filename=strcat(dirname,'comparisonHellweg_6cells.Zemit.002')
%
% The structure of the file:
%     1st column - z, m
%     2nd column - dummy
%     3rd column - Ekin, MeV
%     4-7th columns - dummy
%
fileID=fopen(filename,'r')

while ~feof(fileID)
    tline = fgetl(fileID)
    if tline ~= -1
        [AZe_C2,count] = fscanf(fileID, '%e %e %e %e %e %e %e',inf);
    end
%     display(['count=',num2str(count)]);
end
fclose(fileID)
count;

linesZe_C2=count/7

zZe_C2=zeros(linesZe_C2,1);
EkinZe_C2=zeros(linesZe_C2,1);


n=1;
for k=1:linesZe_C2
    zZe_C2(k)=AZe_C2(n);                    % m
    EkinZe_C2(k)=AZe_C2(n+2);               % MeV
    n=n+7;
end

figure (31)
plot(zZe_C2,EkinZe_C2,'r','LineWidth',2)
grid on
xlabel('z, m','FontSize',16,'Color','m')
ylabel('E_{kin}, MeV','FontSize',16,'Color','m')
title('','FontSize',16,'Color','m')


%------------------------------------------------
%
%   File comparisonHellweg_6cells.Sigma.run: 2
%
%------------------------------------------------------
filename=strcat(dirname,'comparisonHellweg_6cells.Sigma.002')
%
% The structure of the file:
%     1st column - z, m
%     2nd column - Ekin, MeV
%     3-23th columns - dummy
%
fileID=fopen(filename,'r')

while ~feof(fileID)
    tline = fgetl(fileID)
    if tline ~= -1
        [ASi_C2,count] = fscanf(fileID, ...
            '%e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e',inf);
    end
%     display(['count=',num2str(count)]);
end
fclose(fileID)
count;

linesSi_C2=count/23

zSi_C2=zeros(linesSi_C2,1);
EkinSi_C2=zeros(linesSi_C2,1);


n=1;
for k=1:linesSi_C2
    zSi_C2(k)=ASi_C2(n);                    % m
    EkinSi_C2(k)=ASi_C2(n+1);               % MeV
    n=n+23;
end

figure (41)
plot(zSi_C2,EkinSi_C2,'r','LineWidth',2)
grid on
xlabel('z, m','FontSize',16,'Color','m')
ylabel('E_{kin}, MeV','FontSize',16,'Color','m')
title('','FontSize',16,'Color','m')

