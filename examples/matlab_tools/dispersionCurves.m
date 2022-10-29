%
% Source of equations and expressions:
% E.P. Burstein and G.V. Voskresensky (BV),
% "Linear accelerators for electrons with intensive beams"
%
%-------------------------------------------------------
%
% General parameters:
%
twoPi=2*3.14159265358;
mc2=0.51099893e6;             % electronn mass, eV
cLight=2.9979246e10;          % light speed, cm/s
%c
% Geometry of the disk-loaded waveGuide:
%
irisRadius=1.98;              % cm; parameter "a" of BV
wguideRadius=6.515;           % cm; parameter "b" of BV
dist_bw_iris=4.9;             % cm; parameter "d" of BV
periodWGuide=5.5;             % cm; parameter "D" of BV

%
% Wave parameters:
%
freqRF=2856e6;                % RF frequency, Hz
lambda=cLight/freqRF;         % cm
waveNumber=twoPi/lambda;      % 1/cm; parameter "k" of BV

nRadialHarm=1;                % Max of number of radial harmonics
orderDet=5;                   % order of determinant for dispersion equation

numberStep_gammaD=3;        
step_gammaD=twoPi/2/numberStep_gammaD;  % d/l; parameter "hD" of BV (figure 1.2)

numberStep_wNumber=3;
step_wGuideD=twoPi/numberStep_wNumber;  % d/l; parameter "kD" of BV (figure 1.2)

%
% Working arrays:
%
etaS=zeros(orderDet,1);               % 1/cm; parameter "eta_s" of BV (1.22)
hD=zeros(numberStep_gammaD,1);        % d/l; current value of parameter "hD" of BV (figure 1.2)
kD=zeros(numberStep_wNumber,1);       % d/l; current value of parameter "kD" of BV (figure 1.2)
alphaZ=zeros(nRadialHarm,orderDet);   % d/l; factor alpha_1 of BV (1.18)
alphaA=zeros(nRadialHarm,orderDet);   % d/l; factor alpha_2 of BV (1.18)
betaZ=zeros(orderDet,orderDet);       % d/l; factor beta_1 of BV (1.25)
betaA=zeros(orderDet,orderDet);       % d/l; factor beta_2 of BV (1.25)
I0=zeros(nRadialHarm,1);              % d/l; Bessel functions I_0
I0d=zeros(nRadialHarm,1);             % d/l; Bessel functions I'_0 = -I_1
Z0=zeros(orderDet,1);                 % d/l; construction from Bessel functions J_0 and Y_0 from BV (1.23)  
Z0d=zeros(orderDet,1);                % d/l; construction from Bessel functions J_0, J'_0 and Y_0, Y'_0 from BV (1.23)
matrC=zeros(orderDet,orderDet);       % d/l; matrix C_st from BV (1.30)
detC=zeros(numberStep_wNumber,numberStep_gammaD); % d/l; determimant of the matrix C_st from BV (1.37)

kappaS=zeros(numberStep_wNumber,orderDet);

for m=1:orderDet
    etaS(m)=twoPi/2*(m-1)/dist_bw_iris;           % 1/cm; parameter "eta_s" of BV (1.22)
end
    
for i=1:numberStep_wNumber
    kD(i)=step_wGuideD*i;                    % d/l; current value of parameter "kD" of BV (figure 1.2)
    for j=1:numberStep_gammaD
        hD(j)=step_gammaD*j;                 % d/l; current value of parameter "hD" of BV (figure 1.2)
        for n=1:nRadialHarm
% roN - current value of parapmeter "p_n" from BV (1.22) ( 1/cm):
            roN=sqrt((hD(j)+twoPi*n)^2-kD(i)^2)/periodWGuide;    
%
% Bessel "functions" I0, I0d=I'_0=-I_1;
%
            I0(n)=besseli(0,roN*irisRadius);            % I_0(ro_n*a) from BV
            I0d(n)=-besseli(1,roN*irisRadius);          % I'_0(ro_n*a)=-I_1(ro_n*a) from BV
            for m=1:orderDet
                kappaS(i,m)=sqrt((kD(i)/dist_bw_iris)^2-etaS(m)^2);  % 1/cm; parameter "kappa_s" of BV (1.22)
%
% Bessel "functions" Z0=J_0(etaS*b)*Y_0(etaS*a)-I_0(etaS*b)*Y0(etaS*a) and
%                    Z0d=:J'_0(etaS*b)*Y_0(etaS*a)-I_0(etaS*b)*Y'0(etaS*a); (J'_0=-J_1, Y'_0=-Y_1)
%
                Z0(m)=besselj(0,kappaS(i,m)*irisRadius)*bessely(0,kappaS(i,m)*wguideRadius)- ...
                      besselj(0,kappaS(i,m)*wguideRadius)*bessely(0,kappaS(i,m)*irisRadius);
                Z0d(m)=-besselj(1,kappaS(i,m)*irisRadius)*bessely(0,kappaS(i,m)*wguideRadius) ...
                       +besselj(0,kappaS(i,m)*wguideRadius)*bessely(1,kappaS(i,m)*irisRadius);
% Alpha factors from BV (1.18): 
                argAlpha=twoPi/2*dist_bw_iris/periodWGuide*((m-1)-(n-2)); % d/l
                if (argAlpha == 0.) 
                    alphaZ(n,m)=1.0;                            % d/l; factor alpha_1 of BV (1.18)
                else
                    alphaZ(n,m)=sin(argAlpha)/argAlpha;         % d/l; factor alpha_1 of BV (1.18)
                end
                alphaA(n,m)=alphaZ(n,m);                        % d/l; factor alpha_1 of BV (1.18)
% Beta factors from BV (1.25): 
                for m1=1:orderDet
                    gammaM1=(hD(j)+twoPi*(m1-3))/periodWGuide;
                    argBetaZAm=(gammaM1-etaS(m))*dist_bw_iris/2; % d/l
                    argBetaZAp=(gammaM1+etaS(m))*dist_bw_iris/2; % d/l
                    if (abs(argBetaZAm) > 1.e-10) 
                        betaZ(m1,m)=gammaM1*dist_bw_iris/2* ...
                                    sin(argBetaZAp)/argBetaZAm/argBetaZAp;  % d/l; factor beta_1 of BV (1.25)
                        betaA(m1,m)=-etaS(m)*dist_bw_iris* ...
                                    sin(argBetaZAp)/argBetaZAm/argBetaZAp;  % d/l; factor beta_2 of BV (1.25)
                    else
                        display(['m=',int2str(m),' m1=',int2str(m1)]);      % something wrong!      
                        continue
                    end
% matrix C_st from BV (begining of (1.30)): 
                    matrC(m1,m)=.25*wguideRadius/kappaS(i,m1)*betaZ(m1,m)*Z0(m)/Z0d(m);  % cm^2
                end
            end
        end
        for m=1:orderDet
            for m1=1:orderDet
                for n=1:nRadialHarm
% roN - current value of parapmeter "p_n" from BV (1.22) ( 1/cm):
                    roN=sqrt((hD(j)+twoPi*n)^2-kD(i)^2)/periodWGuide;
                    matrC(m1,m)=matrC(m1,m)+irisRadius/roN*alphaZ(n,m)*betaZ(m1,m)*I0d(n)/I0(n);  % cm^2
                end
            end
        end
% Determinant of matrix C_st from BV (1.37): 
        C=[matrC(1,1) matrC(2,1) matrC(3,1) matrC(4,1) matrC(5,1); ...
           matrC(1,2) matrC(2,2) matrC(3,2) matrC(4,2) matrC(5,2); ...
           matrC(1,3) matrC(2,3) matrC(3,3) matrC(4,3) matrC(5,3); ... 
           matrC(1,4) matrC(2,4) matrC(3,4) matrC(4,4) matrC(5,4); ...
           matrC(1,5) matrC(2,5) matrC(3,5) matrC(4,5) matrC(5,5)];
           detC(j,i)=det(C);
    end
end
































