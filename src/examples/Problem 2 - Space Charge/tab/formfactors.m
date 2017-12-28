%------------------------------------------------------
%
% Formfactors for selected aY,aZ:
% 
%------------------------------------------------------
Ry_Rx=.99
Rx_Rz=.055
aY=Ry_Rx
aZ=1/Rx_Rz

func= @(x) sqrt((aY^2+x).*(aZ^2+x).*(1+x).^3).^(-1);       % Sretensky
Mx=aY*aZ*quadgk(func,0,inf)                                % Sretensky 
func= @(x) sqrt((1+x).*(aZ^2+x).*(aY^2+x).^3).^(-1);       % Sretensky 
My=aY*aZ*quadgk(func,0,inf)                                % Sretensky 
func= @(x) sqrt((1+x).*(aY^2+x).*(aZ^2+x).^3).^(-1); % Sretensky 
Mz=aY*aZ*quadgk(func,0,inf)                                      % Sretensky 

%------------------------------------------------------
%
% Tabulation of the formfactors:
%        
%------------------------------------------------------
aZmaxL=1;
aZmaxR=100;
nZ=100;
stepZL=aZmaxL/nZ;
stepZR=aZmaxR/nZ;

aYmaxL=1;
aYmaxR=100;
nY=100;
stepYL=aYmaxL/nY;
stepYR=aYmaxR/nY;

z=zeros(2*nZ-1,1);
y=zeros(2*nY-1,1);
formMx=zeros(2*nZ-1,2*nY-1);
formMy=zeros(2*nZ-1,2*nY-1);
formMz=zeros(2*nZ-1,2*nY-1);

for k=1:2*nY-1
    aY=stepYL*k;
    if (k > nY) 
        aY=stepYR*(k-(nY-1));
    end 
    y(k)=aY;
    for n=1:2*nZ-1
        aZ=stepZL*n;
        if (n > nZ) 
            aZ=stepZR*(n-(nZ-1));
        end    
        z(n)=1/aZ;
%        func= @(x) sqrt((1+x).*(aY^2+x).*(aZ^2+x).^3).^(-1);       % Kapchinsky
%        formM(n,k)=1-aY*aZ/2*quadgk(func,0,inf);                   % Kapchinsky
%        func= @(x) sqrt((aZ^2+x).*((1+x).*(aY^2+x)).^3).^(-1);     % Kapchinsky
%        formN(n,k)=aY*(1-aY^2)/4*quadgk(func,0,inf);               % Kapchinsky
        func= @(x) sqrt((aY^2+x).*(aZ^2+x).*(1+x).^3).^(-1);        % Sretensky
        formMx(n,k)=aY*aZ*quadgk(func,0,inf);                       % Sretensky 
        func= @(x) sqrt((1+x).*(aZ^2+x).*(aY^2+x).^3).^(-1);        % Sretensky 
        formMy(n,k)=aY*aZ*quadgk(func,0,inf);                       % Sretensky 
        func= @(x) sqrt((1+x).*(aY^2+x).*(aZ^2+x).^3).^(-1);  % Sretensky 
        formMz(n,k)=aY*aZ*quadgk(func,0,inf);                             % Sretensky 
    end
    display(['y(',int2str(k),')=',num2str(y(k),'%6.3f')])
end

figure (100)
plot (z,formMx(:,1),'-r',z,formMx(:,40),'b',z,formMx(:,80),'-m',z,formMx(:,100),'-g', ...
     z,formMx(:,120),'-.r',z,formMx(:,160),'-.b',z,formMx(:,199),'-.m','LineWidth',2)
grid on
title('Ellipsoid with Semiaxes R_x, R_y, R_z: Formfactor M_x','FontSize',16,'Color','m')
xlabel('R_x/ R_z','FontSize',16,'Color','m')
ylabel('M_x','FontSize',16,'Color','m')
xlim([0,5])
legend(['R_y/R_x=',num2str(y(1),'%5.3f')],['R_y/R_x=',num2str(y(40),'%5.3f')],['R_y/R_x=',num2str(y(80),'%5.3f')], ...
        ['R_y/R_x=',num2str(y(100),'%5.3f')],['R_y/R_x=',num2str(y(120),'%5.3f')],['R_y/R_x=',num2str(y(160),'%5.3f')], ...
        ['R_y/R_x=',num2str(y(199),'%5.3f')],'Location','ne')

figure (110)
plot (z,formMx(:,1),'-r',z,formMx(:,10),'-b',z,formMx(:,20),'-m',z,formMx(:,40),'-g', ...
      z,formMx(:,60),'-.r',z,formMx(:,80),'-.b',z,formMx(:,100),'-.m',z,formMx(:,101),'-.g',...
      z,formMx(:,104),'-r',z,formMx(:,109),'-b','LineWidth',2)
% For test of fitting:
plot (z,formMx(:,109),'xb','LineWidth',2)
grid on
title('Ellipsoid with Semiaxes R_x, R_y, R_z: Formfactor M_x','FontSize',16,'Color','m')
xlabel('R_x/ R_z','FontSize',16,'Color','m')
ylabel('M_x','FontSize',16,'Color','m')
xlim([0,5])
% ylim([0,.05])
legend(['R_y/R_x=',num2str(y(1),'%5.3f')],['R_y/R_x=',num2str(y(10),'%5.3f')],['R_y/R_x=',num2str(y(20),'%5.3f')], ...
        ['R_y/R_x=',num2str(y(40),'%5.3f')],['R_y/R_x=',num2str(y(60),'%5.3f')],['R_y/R_x=',num2str(y(80),'%5.3f')], ...
        ['R_y/R_x=',num2str(y(100),'%5.3f')],['R_y/R_x=',num2str(y(101),'%5.3f')],['R_y/R_x=',num2str(y(104),'%5.3f')], ...
        ['R_y/R_x=',num2str(y(109),'%5.3f')],'Location','ne')

figure (200)
plot (z,formMy(:,1),'-r',z,formMy(:,40),'b',z,formMy(:,80),'-m',z,formMy(:,100),'-g', ...
     z,formMy(:,120),'-.r',z,formMy(:,160),'-b.',z,formMy(:,199),'-m.','LineWidth',2)
grid on
title('Ellipsoid with Semiaxes R_x, R_y, R_z: Formfactor M_y','FontSize',16,'Color','m')
xlabel('R_x/ R_z','FontSize',16,'Color','m')
ylabel('M_y','FontSize',16,'Color','m')
xlim([0,5])
legend(['R_y/R_x=',num2str(y(1),'%5.3f')],['R_y/R_x=',num2str(y(40),'%5.3f')],['R_y/R_x=',num2str(y(80),'%5.3f')], ...
        ['R_y/R_x=',num2str(y(100),'%5.3f')],['R_y/R_x=',num2str(y(120),'%5.3f')],['R_y/R_x=',num2str(y(160),'%5.3f')], ...
        ['R_y/R_x=',num2str(y(199),'%5.3f')],'Location','e')

figure (210)
plot (z,formMy(:,1),'-r',z,formMy(:,10),'-b',z,formMy(:,20),'-m',z,formMy(:,40),'-g', ...
     z,formMy(:,60),'-.r',z,formMy(:,80),'-.b',z,formMy(:,100),'-.m',z,formMy(:,101),'-.g',...
     z,formMy(:,104),'-r',z,formMy(:,109),'-b','LineWidth',2)
grid on
title('Ellipsoid with Semiaxes R_x, R_y, R_z: Formfactor M_x','FontSize',16,'Color','m')
xlabel('R_x/ R_z','FontSize',16,'Color','m')
ylabel('M_y','FontSize',16,'Color','m')
xlim([0,5])
legend(['R_y/R_x=',num2str(y(1),'%5.3f')],['R_y/R_x=',num2str(y(10),'%5.3f')],['R_y/R_x=',num2str(y(20),'%5.3f')], ...
        ['R_y/R_x=',num2str(y(40),'%5.3f')],['R_y/R_x=',num2str(y(60),'%5.3f')],['R_y/R_x=',num2str(y(80),'%5.3f')], ...
        ['R_y/R_x=',num2str(y(100),'%5.3f')],['R_y/R_x=',num2str(y(101),'%5.3f')],['R_y/R_x=',num2str(y(104),'%5.3f')], ...
        ['R_y/R_x=',num2str(y(109),'%5.3f')],'Location','ne')

figure (300)
plot (z,formMz(:,1),'-r',z,formMz(:,40),'b',z,formMz(:,80),'-m',z,formMz(:,100),'-g', ...
     z,formMz(:,120),'-.r',z,formMz(:,160),'-.b',z,formMz(:,199),'-.m','LineWidth',2)
grid on
title('Ellipsoid with Semiaxes R_x, R_y, R_z: Formfactor M_z','FontSize',16,'Color','m')
xlabel('R_x/ R_z','FontSize',16,'Color','m')
ylabel('M_z','FontSize',16,'Color','m')
xlim([0,5])
legend(['R_y/R_x=',num2str(y(1),'%5.3f')],['R_y/R_x=',num2str(y(40),'%5.3f')],['R_y/R_x=',num2str(y(80),'%5.3f')], ...
        ['R_y/R_x=',num2str(y(100),'%5.3f')],['R_y/R_x=',num2str(y(120),'%5.3f')],['R_y/R_x=',num2str(y(160),'%5.3f')], ...
        ['R_y/R_x=',num2str(y(199),'%5.3f')],'Location','e')

figure (310)
% plot (z,formMz(:,1),'.r',z,formMz(:,8),'-b',z,formMz(:,20),'-m',z,formMz(:,40),'-g', ...
%      z,formMz(:,60),'-.r',z,formMz(:,80),'-.b',z,formMz(:,100),'-.m',z,formMz(:,101),'-.g',...
%      z,formMz(:,104),'-r',z,formMz(:,109),'-b','LineWidth',2)
plot (z,formMz(:,109),'-b','LineWidth',2)
grid on
title('Ellipsoid with Semiaxes R_x, R_y, R_z: Formfactor M_x','FontSize',16,'Color','m')
xlabel('R_x/ R_z','FontSize',16,'Color','m')
ylabel('M_z','FontSize',16,'Color','m')
xlim([0,5])
legend(['R_y/R_x=',num2str(y(1),'%5.3f')],['R_y/R_x=',num2str(y(8),'%5.3f')],['R_y/R_x=',num2str(y(20),'%5.3f')], ...
        ['R_y/R_x=',num2str(y(40),'%5.3f')],['R_y/R_x=',num2str(y(60),'%5.3f')],['R_y/R_x=',num2str(y(80),'%5.3f')], ...
        ['R_y/R_x=',num2str(y(100),'%5.3f')],['R_y/R_x=',num2str(y(101),'%5.3f')],['R_y/R_x=',num2str(y(104),'%5.3f')], ...
        ['R_y/R_x=',num2str(y(109),'%5.3f')],'Location','ne')

fnalHome=1;    % =1 for FNAL and =0 fore home
if (fnalHome == 0)
    cd ('C:/Users/Yury/My Personal Documents/My WORK/ESS/ASTRA/highBeta') 
    dirname='C:/Users/Yury/My Personal Documents/My WORK/ESS/ASTRA/highBeta/';
elseif (fnalHome == 1)
%  cd ('/home/eidelyur/radiaSoft/astra/comparison_Hellweg/') 
%    dirname='/home/eidelyur/radiaSoft/astra/comparison_Hellweg/';
    dirname='/';
end

%------------------------------------------------
%
%   Files for formfactors:
%
%------------------------------------------------------
%     1st line - title
%     2nd line - empty
%     3rd line - 10 values of R_y/R_x 
%     4th line - empty
%     Each next line has the following astructure:
%         1st column - R_x/R_z
%         2nd - 11th column - Mx for corresponding R_y/R_x
%
filename=strcat(dirname,'formfactorMx1.data')

indxRyRx=[1 10 20 40 60 80 100 101 104 109]';
indxRxRz=[199 109 101 100 67 50 40 33 28 25 22 20]';

fileID=fopen(filename,'w')

fprintf(fileID,'\n   Column:      %8d       %8d       %8d       %8d       %8d       %8d       %8d       %8d       %8d       %8d\n',...
        1,2,3,4,5,6,7,8,9,10)
fprintf(fileID,'     Ry/Rx:    %14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e\n\n',...
        y(indxRyRx(1)),y(indxRyRx(2)),y(indxRyRx(3)),y(indxRyRx(4)),y(indxRyRx(5)), ...
        y(indxRyRx(6)),y(indxRyRx(7)),y(indxRyRx(8)),y(indxRyRx(9)),y(indxRyRx(10)));
fprintf(fileID,'     Rx/Rz         Mx(:,1)        Mx(:,2)        Mx(:,3)       Mx(:,4)        Mx(:,5)        Mx(:,6)          Mx(:,7)       Mx(:,8)        Mx(:,9)         Mx(:,10)\n\n')    

for k=1:length(indxRxRz)
    n=indxRxRz(k);
    fprintf(fileID,'%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e\n',...
            z(n),formMx(n,indxRyRx(1)),formMx(n,indxRyRx(2)),formMx(n,indxRyRx(3)),formMx(n,indxRyRx(4)), ...
                 formMx(n,indxRyRx(5)),formMx(n,indxRyRx(6)),formMx(n,indxRyRx(7)),formMx(n,indxRyRx(8)), ...
                 formMx(n,indxRyRx(9)),formMx(n,indxRyRx(10)));
end

fclose(fileID)

filename=strcat(dirname,'formfactorMy1.data')

indxRyRx=[1 10 20 40 60 80 100 101 104 109]';
fileID=fopen(filename,'w')

fprintf(fileID,'\n   Column:      %8d       %8d       %8d       %8d       %8d       %8d       %8d       %8d       %8d       %8d\n',...
        1,2,3,4,5,6,7,8,9,10)
fprintf(fileID,'     Ry/Rx:    %14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e\n\n',...
        y(indxRyRx(1)),y(indxRyRx(2)),y(indxRyRx(3)),y(indxRyRx(4)),y(indxRyRx(5)), ...
        y(indxRyRx(6)),y(indxRyRx(7)),y(indxRyRx(8)),y(indxRyRx(9)),y(indxRyRx(10)));
fprintf(fileID,'     Rx/Rz         My(:,1)        My(:,2)        My(:,3)       My(:,4)        My(:,5)        My(:,6)          My(:,7)       My(:,8)        My(:,9)         My(:,10)\n\n')    

for k=1:length(indxRxRz)
    n=indxRxRz(k);
    fprintf(fileID,'%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e\n',...
            z(n),formMy(n,indxRyRx(1)),formMy(n,indxRyRx(2)),formMy(n,indxRyRx(3)),formMy(n,indxRyRx(4)), ...
                 formMy(n,indxRyRx(5)),formMy(n,indxRyRx(6)),formMy(n,indxRyRx(7)),formMy(n,indxRyRx(8)), ...
                 formMy(n,indxRyRx(9)),formMy(n,indxRyRx(10)));
end

fclose(fileID)

filename=strcat(dirname,'formfactorMz1.data')

indxRyRx=[1 10 8 40 60 80 100 101 104 109]';
fileID=fopen(filename,'w')

fprintf(fileID,'\n   Column:      %8d       %8d       %8d       %8d       %8d       %8d       %8d       %8d       %8d       %8d\n',...
        1,2,3,4,5,6,7,8,9,10)
fprintf(fileID,'     Ry/Rx:    %14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e,%14.8e\n\n',...
        y(indxRyRx(1)),y(indxRyRx(2)),y(indxRyRx(3)),y(indxRyRx(4)),y(indxRyRx(5)), ...
        y(indxRyRx(6)),y(indxRyRx(7)),y(indxRyRx(8)),y(indxRyRx(9)),y(indxRyRx(10)));
fprintf(fileID,'     Rx/Rz         Mz(:,1)        Mz(:,2)        Mz(:,3)       Mz(:,4)        Mz(:,5)        Mz(:,6)          z(:,7)       Mz(:,8)        Mz(:,9)         Mz(:,10)\n\n')    

for k=1:length(indxRxRz)
    n=indxRxRz(k);
    fprintf(fileID,'%14.8e %14.8e %14.8e %14.8e %14.8e %14.8e %14.8e  %14.8e %14.8e %14.8e  %14.8e\n',...
            z(n),formMz(n,indxRyRx(1)),formMz(n,indxRyRx(2)),formMz(n,indxRyRx(3)),formMz(n,indxRyRx(4)), ...
                 formMz(n,indxRyRx(5)),formMz(n,indxRyRx(6)),formMz(n,indxRyRx(7)),formMz(n,indxRyRx(8)), ...
                 formMz(n,indxRyRx(9)),formMz(n,indxRyRx(10)));
end

fclose(fileID)

%------------------------------------------------
%
%   Checking of written files:
%
%------------------------------------------------------

RyRx=zeros(10,1);

filename=strcat(dirname,'formfactorMx1.data')

MxReaded=[];
rX_rZ=zeros(100,1);

fileID=fopen(filename,'r')

tline = fgetl(fileID);
tline = fgetl(fileID);
tline = fgetl(fileID);
tline = fgetl(fileID);
tline = fgetl(fileID);
% A=textscan(fileID,'%s %e %e %e %e %e %e %e %e %e %e','delimiter',',');
% fclose(fileID)
% for k =1:10
%     RyRx(k)=A(k+1);
% end
nLine=0;
while ~feof(fileID)
    tline = fgetl(fileID)
    if tline ~= -1
        nLine=nLine+1;
        A=strfind(tline,',');
        rX_rZ(nLine)=str2num(tline(1:A(1)-1));
        counter=length(A)+1;
        B=zeros(counter,1);
        for n=1:counter-1
            B(n,1)=A(n);
        end
        B(counter,1)=length(tline)+1;
        for n=1:counter-1
            MxReaded(nLine,n)=str2num(tline(B(n)+1:B(n+1)-1));
        end
    end
%     display(['count=',num2str(count)]);
end
fclose(fileID)

%------------------------------------------------
%
%   Formfactors for different lambda:
%
%------------------------------------------------------
lambdaMin1=0;
lambdaMax1=90;
nLambda1=10;
lambdaMin2=100;
lambdaMax2=1000;
nLambda2=10;
lambdaMin3=1500;
lambdaMax3=6000;
nLambda3=10;

totLambda=nLambda1+nLambda2+nLambda3;
lambda=zeros(totLambda,1);
for n=1:nLambda1
    lambda(n)=lambdaMin1+(lambdaMax1-lambdaMin1)*(n-1)/(nLambda1-1);
end
for n=1:nLambda2
    lambda(nLambda1+n)=lambdaMin2+(lambdaMax2-lambdaMin2)*(n-1)/(nLambda2-1);
end
for n=1:nLambda3
    lambda(nLambda1+nLambda2+n)=lambdaMin3+(lambdaMax3-lambdaMin3)*(n-1)/(nLambda3-1);
end

aZmaxL=1;
aZmaxR=100;
nZ=100;
stepZL=aZmaxL/nZ;
stepZR=aZmaxR/nZ;

z=zeros(2*nZ-1,1);
zr=zeros(2*nZ-1,1);
for n=1:2*nZ-1
    aZ=stepZL*n;
    if (n > nZ) 
        aZ=stepZR*(n-(nZ-1));
    end    
    z(n)=aZ;
    zr(n)=1/aZ;
end

aYmaxL=1;
aYmaxR=100;
nY=100;
stepYL=aYmaxL/nY;
stepYR=aYmaxR/nY;

y=zeros(2*nY-1,1);
for k=1:2*nY-1
    aY=stepYL*k;
    if (k > nY) 
        aY=stepYR*(k-(nY-1));
    end 
    y(k)=aY;
end

formMxL=zeros(2*nZ-1,2*nY-1,totLambda);
formMyL=zeros(2*nZ-1,2*nY-1,totLambda);
formMzL=zeros(2*nZ-1,2*nY-1,totLambda);

totTime=0;
for j=1:totLambda
    curLambda=lambda(j);
    totTimeLambda=0;
    for k=1:2*nY-1
        tStart=tic;
        aY=y(k);
        for n=1:2*nZ-1
            aZ=z(n);
            func= @(x) sqrt((aY^2+x).*(aZ^2+x).*(1+x).^3).^(-1);                        % Sretensky
            formMxL(n,k,j)=aY*aZ*quadgk(func,curLambda,inf,'MaxIntervalCount',100000);  % Sretensky 
            func= @(x) sqrt((1+x).*(aZ^2+x).*(aY^2+x).^3).^(-1);                        % Sretensky         
            formMyL(n,k,j)=aY*aZ*quadgk(func,curLambda,inf,'MaxIntervalCount',100000);  % Sretensky 
            func= @(x) aY*aZ*sqrt((1+x).*(aY^2+x).*(aZ^2+x).^3).^(-1);                  % Sretensky 
            formMzL(n,k,j)=aY*aZ*quadgk(func,curLambda,inf,'MaxIntervalCount',100000);  % Sretensky 
        end
        elpsdTime=toc(tStart);
        totTimeLambda=totTimeLambda+elpsdTime;
        totTime=totTime+elpsdTime;
        display(['lambda=',num2str(curLambda,'%6.3f'),' y(',int2str(k),')=',num2str(y(k),'%6.3f'), ...
                 '; time=',num2str(elpsdTime,'%6.3f'),' sec'])
    end
end
display(['Total time=',num2str(totTime,'%10.5f'),' sec'])

indxRyRx=[1 10 20 40 60 80 100 101 104 109]';
indxRxRz=[199 109 101 100 67 50 40 33 28 25 22 20]';
indxLambda=[1 30]';
for j=1:length(indxLambda)
    figure (499+j)
    plot (zr,formMxL(:,indxRyRx(1),indxLambda(j)),'-r', zr,formMxL(:,indxRyRx(2),indxLambda(j)),'-b', zr,formMxL(:,indxRyRx(3),indxLambda(j)),'-m',  ...
          zr,formMxL(:,indxRyRx(4),indxLambda(j)),'-g', zr,formMxL(:,indxRyRx(5),indxLambda(j)),'-.r',zr,formMxL(:,indxRyRx(6),indxLambda(j)),'-.b', ...
          zr,formMxL(:,indxRyRx(7),indxLambda(j)),'-.m',zr,formMxL(:,indxRyRx(8),indxLambda(j)),'-r', zr,formMxL(:,indxRyRx(9),indxLambda(j)),'-b', ...
          zr,formMxL(:,indxRyRx(10),indxLambda(j)),'-m','LineWidth',2)
    grid on
    title(['Formfactor M_x: \lambda=',int2str(lambda(indxLambda(j)))],'FontSize',16,'Color','m')
    xlabel('R_x/ R_z','FontSize',16,'Color','m')
    ylabel('M_x','FontSize',16,'Color','m')
%    xlim([0,5])
%    legend(['R_y/R_x=',num2str(y(indxRyRx(1)),'%5.3f')],['R_y/R_x=',num2str(y(indxRyRx(2)),'%5.3f')], ...
%           ['R_y/R_x=',num2str(y(indxRyRx(3)),'%5.3f')],['R_y/R_x=',num2str(y(indxRyRx(4)),'%5.3f')], ...
%           ['R_y/R_x=',num2str(y(indxRyRx(5)),'%5.3f')],['R_y/R_x=',num2str(y(indxRyRx(6)),'%5.3f')], ...
%           ['R_y/R_x=',num2str(y(indxRyRx(7)),'%5.3f')],['R_y/R_x=',num2str(y(indxRyRx(8)),'%5.3f')], ...
%           ['R_y/R_x=',num2str(y(indxRyRx(9)),'%5.3f')],['R_y/R_x=',num2str(y(indxRyRx(10)),'%5.3f')],'Location','ne')
end

%--------------------------------------
%
% "Lambda"-alignment:
%
%-------------------------------------
logLambda=zeros(totLambda,1);
logLambda(2:totLambda)=log10(lambda(2:totLambda));
Mx1=zeros(totLambda,1);
Mx2=zeros(totLambda,1);
Mx3=zeros(totLambda,1);
Mx4=zeros(totLambda,1);
Mx5=zeros(totLambda,1);
Mx6=zeros(totLambda,1);
Mx7=zeros(totLambda,1);
Mx8=zeros(totLambda,1);
Mx9=zeros(totLambda,1);
Mx10=zeros(totLambda,1);

Mx1log=zeros(totLambda,1);
Mx2log=zeros(totLambda,1);
Mx3log=zeros(totLambda,1);
Mx4log=zeros(totLambda,1);
Mx5log=zeros(totLambda,1);
Mx6log=zeros(totLambda,1);
Mx7log=zeros(totLambda,1);
Mx8log=zeros(totLambda,1);
Mx9log=zeros(totLambda,1);
Mx10log=zeros(totLambda,1);
%
% Mx:
%
for n=1:totLambda
    M1log(n)=log10(formMxL(indxRyRx(10),indxRxRz(1),n));
    M2log(n)=log10(formMxL(indxRyRx(10),indxRxRz(2),n));
    M3log(n)=log10(formMxL(indxRyRx(10),indxRxRz(3),n));
    M4log(n)=log10(formMxL(indxRyRx(10),indxRxRz(4),n));
    M5log(n)=log10(formMxL(indxRyRx(10),indxRxRz(5),n));
    M6log(n)=log10(formMxL(indxRyRx(10),indxRxRz(6),n));
    M7log(n)=log10(formMxL(indxRyRx(10),indxRxRz(7),n));
    M8log(n)=log10(formMxL(indxRyRx(10),indxRxRz(8),n));
    M9log(n)=log10(formMxL(indxRyRx(10),indxRxRz(9),n));
    M10log(n)=log10(formMxL(indxRyRx(10),indxRxRz(10),n));
    
    M1(n)=formMxL(indxRyRx(10),indxRxRz(1),n);
    M2(n)=formMxL(indxRyRx(10),indxRxRz(2),n);
    M3(n)=formMxL(indxRyRx(10),indxRxRz(3),n);
    M4(n)=formMxL(indxRyRx(10),indxRxRz(4),n);
    M5(n)=formMxL(indxRyRx(10),indxRxRz(5),n);
    M6(n)=formMxL(indxRyRx(10),indxRxRz(6),n);
    M7(n)=formMxL(indxRyRx(10),indxRxRz(7),n);
    M8(n)=formMxL(indxRyRx(10),indxRxRz(8),n);
    M9(n)=formMxL(indxRyRx(10),indxRxRz(9),n);
    M10(n)=formMxL(indxRyRx(10),indxRxRz(10),n);
end

figure(1000)
plot(logLambda,M1log,'-r',logLambda,M2log,'-b',logLambda,M3log,'-r',logLambda,M4log,'-g',...
       logLambda,M5log,'-m',logLambda,M6log,'-.r',logLambda,M7log,'-.b',logLambda,M8log,'-g.',...
       logLambda,M9log,'-m',logLambda,M10log,'-b','LineWidth',2)
grid on
title(['Formfactor M_x: R_y/R_x=',num2str(y(indxRyRx(10)),'%5.3f')],'FontSize',16,'Color','m')
xlabel('Log_{10}\lambda','FontSize',16,'Color','m')
ylabel('Log_{10}M_x','FontSize',16,'Color','m')
legend(['R_x/R_z=',num2str(zr(1),'%5.3f')],['R_x/R_z=',num2str(zr(2),'%5.3f')], ...
       ['R_x/R_z=',num2str(zr(3),'%5.3f')],['R_x/R_z=',num2str(zr(4),'%5.3f')], ...
       ['R_x/R_z=',num2str(zr(5),'%5.3f')],['R_x/R_z=',num2str(zr(6),'%5.3f')], ...
       ['R_x/R_z=',num2str(zr(7),'%5.3f')],['R_x/R_z=',num2str(zr(8),'%5.3f')], ...
       ['R_x/R_z=',num2str(zr(9),'%5.3f')],['R_x/R_z=',num2str(zr(10),'%5.3f')],'Location','ne')
    
figure(1005)
loglog(lambda,M1,'-r',lambda,M2,'-b',lambda,M3,'-r',lambda,M4,'-g',...
       lambda,M5,'-m',lambda,M6,'-.r',lambda,M7,'-.b',lambda,M8,'-g.',...
       lambda,M9,'-m',lambda,M10,'-b','LineWidth',2)
grid on
title(['Formfactor M_x: R_y/R_x=',num2str(y(indxRyRx(10)),'%5.3f')],'FontSize',16,'Color','m')
xlabel('\lambda','FontSize',16,'Color','m')
ylabel('M_x','FontSize',16,'Color','m')
legend(['R_x/R_z=',num2str(zr(1),'%5.3f')],['R_x/R_z=',num2str(zr(2),'%5.3f')], ...
       ['R_x/R_z=',num2str(zr(3),'%5.3f')],['R_x/R_z=',num2str(zr(4),'%5.3f')], ...
       ['R_x/R_z=',num2str(zr(5),'%5.3f')],['R_x/R_z=',num2str(zr(6),'%5.3f')], ...
       ['R_x/R_z=',num2str(zr(7),'%5.3f')],['R_x/R_z=',num2str(zr(8),'%5.3f')], ...
       ['R_x/R_z=',num2str(zr(9),'%5.3f')],['R_x/R_z=',num2str(zr(10),'%5.3f')],'Location','ne')


figure(1010)
plot(logLambda,M1log,'-r','LineWidth',2)
grid on
title(['Formfactor M_x: R_y/R_x=',num2str(y(indxRyRx(10)),'%5.3f'),', R_x/R_z=',num2str(zr(1),'%5.3f')],'FontSize',16,'Color','m')
xlabel('Log_{10}\lambda','FontSize',16,'Color','m')
ylabel('Log_{10}M_x','FontSize',16,'Color','m')

figure(1020)
plot(logLambda,M2log,'-r','LineWidth',2)
grid on
title(['Formfactor M_x: R_y/R_x=',num2str(y(indxRyRx(10)),'%5.3f'),', R_x/R_z=',num2str(zr(2),'%5.3f')],'FontSize',16,'Color','m')
xlabel('Log_{10}\lambda','FontSize',16,'Color','m')
ylabel('Log_{10}M_x','FontSize',16,'Color','m')

figure(1030)
plot(logLambda,M3log,'-r','LineWidth',2)
grid on
title(['Formfactor M_x: R_y/R_x=',num2str(y(indxRyRx(10)),'%5.3f'),', R_x/R_z=',num2str(zr(3),'%5.3f')],'FontSize',16,'Color','m')
xlabel('Log_{10}\lambda','FontSize',16,'Color','m')
ylabel('Log_{10}M_x','FontSize',16,'Color','m')

figure(1040)
plot(logLambda,M4log,'-r','LineWidth',2)
grid on
title(['Formfactor M_x: R_y/R_x=',num2str(y(indxRyRx(10)),'%5.3f'),', R_x/R_z=',num2str(zr(4),'%5.3f')],'FontSize',16,'Color','m')
xlabel('Log_{10}\lambda','FontSize',16,'Color','m')
ylabel('Log_{10}M_x','FontSize',16,'Color','m')

figure(1050)
plot(logLambda,M5log,'-r','LineWidth',2)
grid on
title(['Formfactor M_x: R_y/R_x=',num2str(y(indxRyRx(10)),'%5.3f'),', R_x/R_z=',num2str(zr(5),'%5.3f')],'FontSize',16,'Color','m')
xlabel('Log_{10}\lambda','FontSize',16,'Color','m')
ylabel('Log_{10}M_x','FontSize',16,'Color','m')

figure(1060)
plot(logLambda,M6log,'-r','LineWidth',2)
grid on
title(['Formfactor M_x: R_y/R_x=',num2str(y(indxRyRx(10)),'%5.3f'),', R_x/R_z=',num2str(zr(6),'%5.3f')],'FontSize',16,'Color','m')
xlabel('Log_{10}\lambda','FontSize',16,'Color','m')
ylabel('Log_{10}M_x','FontSize',16,'Color','m')

figure(1070)
plot(logLambda,M7log,'-r','LineWidth',2)
grid on
title(['Formfactor M_x: R_y/R_x=',num2str(y(indxRyRx(10)),'%5.3f'),', R_x/R_z=',num2str(zr(7),'%5.3f')],'FontSize',16,'Color','m')
xlabel('Log_{10}\lambda','FontSize',16,'Color','m')
ylabel('Log_{10}M_x','FontSize',16,'Color','m')

figure(1080)
plot(logLambda,M8log,'-r','LineWidth',2)
grid on
title(['Formfactor M_x: R_y/R_x=',num2str(y(indxRyRx(10)),'%5.3f'),', R_x/R_z=',num2str(zr(8),'%5.3f')],'FontSize',16,'Color','m')
xlabel('Log_{10}\lambda','FontSize',16,'Color','m')
ylabel('Log_{10}M_x','FontSize',16,'Color','m')


figure(1090)
plot(logLambda,M9log,'-r','LineWidth',2)
grid on
title(['Formfactor M_x: R_y/R_x=',num2str(y(indxRyRx(10)),'%5.3f'),', R_x/R_z=',num2str(zr(9),'%5.3f')],'FontSize',16,'Color','m')
xlabel('Log_{10}\lambda','FontSize',16,'Color','m')
ylabel('Log_{10}M_x','FontSize',16,'Color','m')

figure(1100)
plot(logLambda,M10log,'-r','LineWidth',2)
grid on
title(['Formfactor M_x: R_y/R_x=',num2str(y(indxRyRx(10)),'%5.3f'),', R_x/R_z=',num2str(zr(10),'%5.3f')],'FontSize',16,'Color','m')
xlabel('Log_{10}\lambda','FontSize',16,'Color','m')
ylabel('Log_{10}M_x','FontSize',16,'Color','m')

%------------------------------------------------
%
%   Files with all formfactors:
%
%------------------------------------------------------
totTime=0;
for m=1:3
    for j=1:totLambda  
        if m == 1
            namefile='/home/eidelyur/radiaSoft/astra/comparison_Hellweg/formfactors/Mx/formfactorMx_lambda-';
        end
        if m == 2
            namefile='/home/eidelyur/radiaSoft/astra/comparison_Hellweg/formfactors/My/formfactorMy_lambda-';
        end
        if m == 3
            namefile='/home/eidelyur/radiaSoft/astra/comparison_Hellweg/formfactors/Mz/formfactorMz_lambda-';
        end
        tStart=tic;
        namefile=strcat(namefile,int2str(lambda(j)),'.data');

        fileID=fopen(namefile,'w');
        display(['File "',namefile,'" is opened'])
        strOut=horzcat('                                  lambda=',num2str(lambda(j),'%6.3f'));
        fprintf(fileID,strOut);
        strOut=horzcat('\n   Column:','     ');
        for k=1:2*nY-1 
            if (k < 10)
                strOut=horzcat(strOut,'    ',num2str(k,'%8d'),'          ');
            end
            if ((k > 9) && (k < 100))
                strOut=horzcat(strOut,'    ',num2str(k,'%8d'),'         ');
            end
            if (k > 99)
                strOut=horzcat(strOut,'    ',num2str(k,'%8d'),'        ');
            end
        end
        strOut=strcat(strOut,'\n');
        fprintf(fileID,strOut);
        strOut='      Ry/Rx:    ';
        for k=1:2*nY-1
            strOut=horzcat(strOut,num2str(y(k),'%14.8e'),' ');
        end
        strOut=horzcat(strOut,'\n\n     Rx/Rz                                      Formfactor\n');
        fprintf(fileID,strOut);
        for k=1:2*nZ-1 
            strOut=horzcat('\n',num2str(zr(k),'%14.8e'));
            for n=1:2*nY-1
                if m == 1
                    strOut=horzcat(strOut,' ',num2str(formMxL(n,k,j),'%14.8e'));
                end
                if m == 2
                    strOut=horzcat(strOut,' ',num2str(formMyL(n,k,j),'%14.8e'));
                end
                if m == 3
                    strOut=horzcat(strOut,' ',num2str(formMzL(n,k,j),'%14.8e'));
                end
            end
            fprintf(fileID,strOut);
        end
        fclose(fileID);
        elpsdTime=toc(tStart);
        totTime=totTime+elpsdTime;
        display(['File is written (',num2str(elpsdTime,'%10.5f'),' sec)'])
    end
end
display(['Total time=',num2str(totTime,'%10.5f'),' sec'])

%------------------------------------------------------
%
%   Formfactors for 3D-interpolation:
%
%------------------------------------------------------
lmbdMax=3.8;
totLmbd=10;
lambdaInt=zeros(totLmbd,1);
for j=1:totLmbd
    logLmbd=lmbdMax/totLmbd*j;
    lambdaInt(j)=10^logLmbd;
end

indxRyRx=[1 10 20 40 60 80 100 101 104 109]';
indxRxRz=[199 109 101 100 67 50 40 33 28 25 22 20]';

totRyRx=length(indxRyRx);
yInt=zeros(totRyRx,1);
for k=1:totRyRx
    yInt(k)=y(indxRyRx(k));
end

totRxRz=length(indxRxRz);
zInt=zeros(totRxRz,1);
zrInt=zeros(totRxRz,1);
for k=1:totRxRz
    zInt(k)=z(indxRxRz(k));
    zrInt(k)=1/z(indxRxRz(k));
end

formMxInt=zeros(totRyRx,totRxRz,totLambda);
formMyInt=zeros(totRyRx,totRxRz,totLambda);
formMzInt=zeros(totRyRx,totRxRz,totLambda);

totTime=0;
for j=1:totLmbd
    curLambda=lambdaInt(j);
    totTimeLambda=0;
    for k=1:totRyRx
        tStart=tic;
        aY=y(indxRyRx(k));
        for n=1:totRxRz
            aZ=z(indxRxRz(n));
            func= @(x) sqrt((aY^2+x).*(aZ^2+x).*(1+x).^3).^(-1);                          % Sretensky
            formMxInt(n,k,j)=aY*aZ*quadgk(func,curLambda,inf,'MaxIntervalCount',100000);  % Sretensky 
            func= @(x) sqrt((1+x).*(aZ^2+x).*(aY^2+x).^3).^(-1);                          % Sretensky         
            formMyInt(n,k,j)=aY*aZ*quadgk(func,curLambda,inf,'MaxIntervalCount',100000);  % Sretensky 
            func= @(x) aY*aZ*sqrt((1+x).*(aY^2+x).*(aZ^2+x).^3).^(-1);                    % Sretensky 
            formMzInt(n,k,j)=aY*aZ*quadgk(func,curLambda,inf,'MaxIntervalCount',100000);  % Sretensky 
        end
        elpsdTime=toc(tStart);
        totTimeLambda=totTimeLambda+elpsdTime;
        totTime=totTime+elpsdTime;
        display(['lambda=',num2str(curLambda,'%6.3f'),' y(',int2str(k),')=',num2str(y(indxRyRx(k)),'%6.3f'), ...
                 '; time=',num2str(elpsdTime,'%6.3f'),' sec'])
    end
end
display(['Total time=',num2str(totTime,'%10.5f'),' sec'])

%------------------------------------------------
%
%   Files with all formfactors for 3D-interpolation:
%
%------------------------------------------------------
totTime=0;
for m=1:3
    for j=1:totLmbd  
        if m == 1
            namefile='/home/eidelyur/radiaSoft/astra/comparison_Hellweg/formfactors/Mx/formfactorMxInt_lambda-';
        end
        if m == 2
            namefile='/home/eidelyur/radiaSoft/astra/comparison_Hellweg/formfactors/My/formfactorMyInt_lambda-';
        end
        if m == 3
            namefile='/home/eidelyur/radiaSoft/astra/comparison_Hellweg/formfactors/Mz/formfactorMzInt_lambda-';
        end
        tStart=tic;
        namefile=strcat(namefile,int2str(round(lambdaInt(j))),'.data');

        fileID=fopen(namefile,'w');
        display(['File "',namefile,'" is opened'])
        strOut=horzcat('                                  lambda=',num2str(lambdaInt(j),'%6.3f'));
        fprintf(fileID,strOut);
        strOut=horzcat('\n   Column:','     ');
        for k=1:totRyRx 
            if (indxRyRx(k) < 10)
                strOut=horzcat(strOut,'    ',num2str(indxRyRx(k),'%8d'),'          ');
            end
            if ((indxRyRx(k) > 9) && (indxRyRx(k) < 100))
                strOut=horzcat(strOut,'    ',num2str(indxRyRx(k),'%8d'),'         ');
            end
            if (indxRyRx(k) > 99)
                strOut=horzcat(strOut,'    ',num2str(indxRyRx(k),'%8d'),'        ');
            end
        end
        strOut=strcat(strOut,'\n');
        fprintf(fileID,strOut);
        strOut='      Ry/Rx:    ';
        for k=1:totRyRx
            strOut=horzcat(strOut,num2str(y(indxRyRx(k)),'%14.8e'),' ');
        end
        strOut=horzcat(strOut,'\n\n     Rx/Rz                                      Formfactor\n');
        fprintf(fileID,strOut);
        for k=1:totRxRz 
            strOut=horzcat('\n',num2str(zr(indxRxRz(k)),'%14.8e'));
            for n=1:totRyRx
                if m == 1
                    strOut=horzcat(strOut,' ',num2str(formMxInt(n,k,j),'%14.8e'));
                end
                if m == 2
                    strOut=horzcat(strOut,' ',num2str(formMyInt(n,k,j),'%14.8e'));
                end
                if m == 3
                    strOut=horzcat(strOut,' ',num2str(formMzInt(n,k,j),'%14.8e'));
                end
            end
            fprintf(fileID,strOut);
        end
        fclose(fileID);
        elpsdTime=toc(tStart);
        totTime=totTime+elpsdTime;
        display(['File is written (',num2str(elpsdTime,'%10.5f'),' sec)'])
    end
end
display(['Total time=',num2str(totTime,'%10.5f'),' sec'])




