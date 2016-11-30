%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Garnett-Wrangler charge distribution (taking into account 
% different number of terms in expanding to series): 
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

macroPart=1000;

rand1=zeros(macroPart,1);
rand2=zeros(macroPart,1);

x=zeros(macroPart,1);
y=zeros(macroPart,1);
z=zeros(macroPart,1);
to=zeros(macroPart,1);

for i=1:3
    rand1=rand(1,macroPart);
    rand2=rand(1,macroPart);
    if (i==1)
        for k=1:macroPart
            x(k)=sqrt(-2*log(rand1(k)))*cos(2*pi*rand2(k));
        end
    end
    if (i==2)
        for k=1:macroPart
            y(k)=sqrt(-2*log(rand1(k)))*cos(2*pi*rand2(k));
        end
    end
    if (i==3)
        for k=1:macroPart
            z(k)=sqrt(-2*log(rand1(k)))*cos(2*pi*rand2(k));
        end
    end
end

std(x)
std(y)
std(z)

t0=x.*x+y.*y+z.*z;
T=max(t0)

i=0;
for k=1:macroPart
    if (abs(t0(k)) > 1e-5)
        i=i+1;
        t0(i)=t0(k);
    end
end
Nmacro=i

a0=0;
a1=0;
a2=0;
a3=0;
a4=0;
a5=0;
a6=0;
a7=0;
a8=0;
a9=0;
a10=0;
a11=0;
a12=0;
a13=0;
a14=0;
a15=0;

for k=1:Nmacro
    a0=a0+1/sqrt(t0(k));
    a1=a1+cos(pi*t0(k)/T)/sqrt(t0(k));
    a2=a2+cos(2*pi*t0(k)/T)/sqrt(t0(k));
    a3=a3+cos(3*pi*t0(k)/T)/sqrt(t0(k));
    a4=a4+cos(4*pi*t0(k)/T)/sqrt(t0(k));
    a5=a5+cos(5*pi*t0(k)/T)/sqrt(t0(k));
    a6=a6+cos(6*pi*t0(k)/T)/sqrt(t0(k));
    a7=a7+cos(7*pi*t0(k)/T)/sqrt(t0(k));
    a8=a8+cos(8*pi*t0(k)/T)/sqrt(t0(k));
    a9=a9+cos(9*pi*t0(k)/T)/sqrt(t0(k));
    a10=a10+cos(10*pi*t0(k)/T)/sqrt(t0(k));
    a11=a11+cos(11*pi*t0(k)/T)/sqrt(t0(k));
    a12=a12+cos(12*pi*t0(k)/T)/sqrt(t0(k));
    a13=a13+cos(13*pi*t0(k)/T)/sqrt(t0(k));
    a14=a14+cos(14*pi*t0(k)/T)/sqrt(t0(k));
    a15=a15+cos(15*pi*t0(k)/T)/sqrt(t0(k));
end

a0
a1
a2
a3
a4
a5
a6
a7
a8
a9
a10
a11
a12
a13
a14
a15
    

totalT=T;
nT0=100;
iT0=zeros(nT0,1);
ro4=zeros(nT0,1);
ro5=zeros(nT0,1);
ro6=zeros(nT0,1);
ro7=zeros(nT0,1);
ro8=zeros(nT0,1);
ro9=zeros(nT0,1);
ro10=zeros(nT0,1);
ro11=zeros(nT0,1);
ro12=zeros(nT0,1);
ro13=zeros(nT0,1);
ro14=zeros(nT0,1);
ro15=zeros(nT0,1);

for i=1:nT0
    iT0(i)=totalT/(nT0-1)*(i-1);
    ro4(i)=a0/2+a1*cos(pi*iT0(i)/totalT)+a2*cos(2*pi*iT0(i)/totalT)+ ...
                a3*cos(3*pi*iT0(i)/totalT)+a4*cos(4*pi*iT0(i)/totalT); 
    ro5(i)=ro4(i)+a5*cos(5*pi*iT0(i)/T);
    ro6(i)=ro5(i)+a6*cos(6*pi*iT0(i)/T);
    ro7(i)=ro6(i)+a7*cos(7*pi*iT0(i)/T);
    ro8(i)=ro7(i)+a8*cos(8*pi*iT0(i)/T);
    ro9(i)=ro8(i)+a9*cos(9*pi*iT0(i)/T);
    ro10(i)=ro9(i)+a10*cos(10*pi*iT0(i)/T);
    ro11(i)=ro10(i)+a11*cos(11*pi*iT0(i)/T);
    ro12(i)=ro11(i)+a12*cos(12*pi*iT0(i)/T);
    ro13(i)=ro12(i)+a13*cos(13*pi*iT0(i)/T);
    ro14(i)=ro13(i)+a14*cos(14*pi*iT0(i)/T);
    ro15(i)=ro14(i)+a15*cos(15*pi*iT0(i)/T);
end


figure(10)
plot(iT0/T,ro4,'-r',iT0/T,ro5,'xb',iT0/T,ro6,'-k','LineWidth',2)
xlabel('t_0/T','Color','m','FontSize',16)
ylabel('\rho(t_0)','Color','m','FontSize',16)
title('1000 Macroparticles:  \rho(t_0)=a_0/2+Sum_{k=1}^L[a_k\cdotcos(k\cdot\pi\cdott_0/T)]','Color','m','FontSize',16)
legend('L=4','L=5','L=6')
grid on

figure(20)
plot(iT0/T,ro6,'-r',iT0/T,ro7,'xb',iT0/T,ro8,'-k','LineWidth',2)
xlabel('t_0/T','Color','m','FontSize',16)
ylabel('\rho(t_0)','Color','m','FontSize',16)
title('1000 Macroparticles:  \rho(t_0)=a_0/2+Sum_{k=1}^L[a_k\cdotcos(k\cdot\pi\cdott_0/T)]','Color','m','FontSize',16)
legend('L=6','L=7','L=8')
grid on

figure(30)
plot(iT0/T,ro8,'-k',iT0/T,ro9,'-r',iT0/T,ro10,'xm','LineWidth',2)
xlabel('t_0/T','Color','m','FontSize',16)
ylabel('\rho(t_0)','Color','m','FontSize',16)
title('1000 Macroparticles:  \rho(t_0)=a_0/2+Sum_{k=1}^L[a_k\cdotcos(k\cdot\pi\cdott_0/T)]','Color','m','FontSize',16)
legend('L=8','L=9','10')
grid on


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%
%%%%%%%%   Garnett-Wrangler approach to calculate space charge effect
%%%%%%%%   using Newton-Cotes quadrature:
%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%
% Points (psi_i) and weigths (w_i) to integrate for interval from -1 to 1:
%
% To integrate for interval from 0 to 1 it is necessary to change points
% psi_i with points ksi_i=(1+psi_i)/2;
%
psi5=[-0.9061798 -0.5384093 0.0       0.5384093 0.9061798]';
w5  =[ 0.2369269  0.4786287 0.5688889 0.4786287 0.2369269]';
psi6=[-0.9324695 -0.6612094 -0.2386192 0.2386192 0.6612094 0.9324695]';
w6  =[ 0.1713245  0.3607616  0.4679139 0.4679139 0.3607616 0.1713245]';
psi7=[-0.9491079 -0.7415312 -0.4058452 0.0       0.4058452 0.7415312 0.9491079]';
w7  =[ 0.1294850  0.2797054  0.3818301 0.4179592 0.3818301 0.2797054 0.1294850]';
psi8=[-0.9602899 -0.7966665 -0.5255324 -0.1834346 0.1834346 0.5255324 0.7966665 0.9602899]';
w8  =[ 0.1012285  0.2223810  0.3137066  0.3626838 0.3626838 0.3137066 0.2223810 0.1012285]';
psi9=[-0.9681602 -0.8360311 -0.6133714 -0.3242534 0.0       0.3242534 0.6133714 0.8360311 0.9681602]';
w9  =[ 0.0812744  0.1806482  0.2606107  0.3123471 0.3302394 0.3123471 0.2606107 0.1806482 0.0812744]';
psi10=[-0.9739065 -0.8650634 -0.6794096 -0.4333954 -0.1488743 0.1488743 0.4333954 0.6794096 0.8650634 0.9739065]';
w10  =[ 0.0666713  0.1494513  0.2190864  0.2692602  0.2955242 0.2955242 0.2692602 0.2190864 0.1494513 0.0666713]';
psi11=[-0.9782287 -0.8870626 -0.7301520 -0.5190961 -0.2695432 0.0 ...
        0.2695432  0.5190961  0.7301520  0.8870626  0.9782287]';
w11  =[ 0.0556686  0.1255804  0.1862902  0.2551938  0.2628045 0.2729251 ...
        0.2628045  0.2331938  0.1862902  0.1255804  0.0556686]';
psi12=[-0.9815606 -0.9041173 -0.7699027 -0.5873180 -0.3678315 -0.1253334 ...
        0.1253334  0.3678315  0.5873180  0.7699027  0.9041173  0.9815606]';
w12  =[ 0.0471753  0.1069393  0.1600783  0.2031674  0.2334925  0.2491470 ...
        0.2491470  0.2334925  0.2031674  0.1600783  0.1069393  0.0471753]';
psi16=[-0.9894009 -0.9445750 -0.8656312 -0.7554044 -0.6178762 -0.4580168 -0.2816036 -0.0950125 ...
        0.0950125  0.2816036  0.4580168  0.6178762  0.7554044  0.8656312  0.9445750  0.9894009]';
w16  =[ 0.0271525  0.0622535  0.0951585  0.1246290  0.1495960  0.1691565  0.1826034  0.1894506 ...
        0.1894506  0.1826034  0.1691565  0.1495960  0.1246290  0.0951585  0.0622535  0.0271525]';

Rb=0.00287883;
Lb=0.493179;
r=0.0013119;
z=0.428186;

Rb2=Rb*Rb
Lb2=Lb*Lb

d=(Rb2*Lb)^(1/3)
d2=d*d

ksi5=zeros(5,1);
ksi6=zeros(6,1);
ksi7=zeros(7,1);
ksi8=zeros(8,1);
ksi9=zeros(9,1);
ksi10=zeros(10,1);
ksi11=zeros(11,1);
ksi12=zeros(12,1);
ksi16=zeros(16,1);

GaussIntR5=0;
GaussIntZ5=0;
for i=1:5
    ksi5(i)=.5*(psi5(i)+1);
    s=d2*(1/ksi5(i)-1);
    t=r*r/(Rb2+s)+z*z/(Lb2+s);
    display (['i=',int2str(i),' t=',num2str(t,'%12.6g')])
        if (t <= 5) 
           func=sqrt(ksi5(i)/((Lb2/d2-1)*ksi5(i)+1))/abs(((Rb2/d2-1)*ksi5(i)+1));
           GaussIntR5=GaussIntR5+.5*r*w5(i)*func/abs(((Rb2/d2-1)*ksi5(i)+1));
           GaussIntZ5=GaussIntZ5+.5*z*w5(i)*func/abs(((Lb2/d2-1)*ksi5(i)+1));
        else 
            display (['i=',int2str(i),' t=',num2str(t,'%12.6g')])
        end
end

GaussIntR6=0;
GaussIntZ6=0;
for i=1:6
    ksi6(i)=.5*(psi6(i)+1);
    s=d2*(1/ksi6(i)-1);
    t=r*r/(Rb2+s)+z*z/(Lb2+s);
    display (['i=',int2str(i),' t=',num2str(t,'%12.6g')])
        if (t <= 5) 
           func=sqrt(ksi6(i)/((Lb2/d2-1)*ksi6(i)+1))/abs(((Rb2/d2-1)*ksi6(i)+1));
           GaussIntR6=GaussIntR6+.5*r*w6(i)*func/abs(((Rb2/d2-1)*ksi6(i)+1));
           GaussIntZ6=GaussIntZ6+.5*z*w6(i)*func/abs(((Lb2/d2-1)*ksi6(i)+1));
        else 
            display (['i=',int2str(i),' t=',num2str(t,'%12.6g')])
        end
end

GaussIntR7=0;
GaussIntZ7=0;
for i=1:7
    ksi7(i)=.5*(psi7(i)+1);
    s=d2*(1/ksi7(i)-1);
    t=r*r/(Rb2+s)+z*z/(Lb2+s);
    display (['i=',int2str(i),' t=',num2str(t,'%12.6g')])
        if (t <= 5) 
           func=sqrt(ksi7(i)/((Lb2/d2-1)*ksi7(i)+1))/abs(((Rb2/d2-1)*ksi7(i)+1));
           GaussIntR7=GaussIntR7+.5*r*w7(i)*func/abs(((Rb2/d2-1)*ksi7(i)+1));
           GaussIntZ7=GaussIntZ7+.5*z*w7(i)*func/abs(((Lb2/d2-1)*ksi7(i)+1));
        else 
            display (['i=',int2str(i),' t=',num2str(t,'%12.6g')])
        end
end

GaussIntR8=0;
GaussIntZ8=0;
for i=1:8
    ksi8(i)=.5*(psi8(i)+1);
    s=d2*(1/ksi8(i)-1);
    t=r*r/(Rb2+s)+z*z/(Lb2+s);
    display (['i=',int2str(i),' t=',num2str(t,'%12.6g')])
        if (t <= 5) 
           func=sqrt(ksi8(i)/((Lb2/d2-1)*ksi8(i)+1))/abs(((Rb2/d2-1)*ksi8(i)+1));
           GaussIntR8=GaussIntR8+.5*r*w8(i)*func/abs(((Rb2/d2-1)*ksi8(i)+1));
           GaussIntZ8=GaussIntZ8+.5*z*w8(i)*func/abs(((Lb2/d2-1)*ksi8(i)+1));
        else 
            display (['i=',int2str(i),' t=',num2str(t,'%12.6g')])
        end
end

GaussIntR9=0;
GaussIntZ9=0;
for i=1:9
    ksi9(i)=.5*(psi9(i)+1);
    s=d2*(1/ksi9(i)-1);
    t=r*r/(Rb2+s)+z*z/(Lb2+s);
    display (['i=',int2str(i),' t=',num2str(t,'%12.6g')])
        if (t <= 5) 
           func=sqrt(ksi9(i)/((Lb2/d2-1)*ksi9(i)+1))/abs(((Rb2/d2-1)*ksi9(i)+1));
           GaussIntR9=GaussIntR9+.5*r*w9(i)*func/abs(((Rb2/d2-1)*ksi9(i)+1));
           GaussIntZ9=GaussIntZ9+.5*z*w9(i)*func/abs(((Lb2/d2-1)*ksi9(i)+1));
        else 
            display (['i=',int2str(i),' t=',num2str(t,'%12.6g')])
        end
end

GaussIntR10=0;
GaussIntZ10=0;
for i=1:10
    ksi10(i)=.5*(psi10(i)+1);
    s=d2*(1/ksi10(i)-1);
    t=r*r/(Rb2+s)+z*z/(Lb2+s);
    display (['i=',int2str(i),' t=',num2str(t,'%12.6g')])
        if (t <= 5) 
           func=sqrt(ksi10(i)/((Lb2/d2-1)*ksi10(i)+1))/abs(((Rb2/d2-1)*ksi10(i)+1));
           GaussIntR10=GaussIntR10+.5*r*w10(i)*func/abs(((Rb2/d2-1)*ksi10(i)+1));
           GaussIntZ10=GaussIntZ10+.5*z*w10(i)*func/abs(((Lb2/d2-1)*ksi10(i)+1));
        else 
            display (['i=',int2str(i),' t=',num2str(t,'%12.6g')])
        end
end

GaussIntR11=0;
GaussIntZ11=0;
for i=1:11
    ksi11(i)=.5*(psi11(i)+1);
    s=d2*(1/ksi11(i)-1);
    t=r*r/(Rb2+s)+z*z/(Lb2+s);
    display (['i=',int2str(i),' t=',num2str(t,'%12.6g')])
        if (t <= 5) 
           func=sqrt(ksi11(i)/((Lb2/d2-1)*ksi11(i)+1))/abs(((Rb2/d2-1)*ksi11(i)+1));
           GaussIntR11=GaussIntR11+.5*r*w11(i)*func/abs(((Rb2/d2-1)*ksi11(i)+1));
           GaussIntZ11=GaussIntZ11+.5*z*w11(i)*func/abs(((Lb2/d2-1)*ksi11(i)+1));
        else 
            display (['i=',int2str(i),' t=',num2str(t,'%12.6g')])
        end
end

GaussIntR12=0;
GaussIntZ12=0;
for i=1:12
    ksi12(i)=.5*(psi12(i)+1);
    s=d2*(1/ksi12(i)-1);
    t=r*r/(Rb2+s)+z*z/(Lb2+s);
    display (['i=',int2str(i),' t=',num2str(t,'%12.6g')])
        if (t <= 5) 
           func=sqrt(ksi12(i)/((Lb2/d2-1)*ksi12(i)+1))/abs(((Rb2/d2-1)*ksi12(i)+1));
           GaussIntR12=GaussIntR12+.5*r*w12(i)*func/abs(((Rb2/d2-1)*ksi12(i)+1));
           GaussIntZ12=GaussIntZ12+.5*z*w12(i)*func/abs(((Lb2/d2-1)*ksi12(i)+1));
        else 
            display (['i=',int2str(i),' t=',num2str(t,'%12.6g')])
        end
end

GaussIntR16=0;
GaussIntZ16=0;
for i=1:16
    ksi16(i)=.5*(psi16(i)+1);
    s=d2*(1/ksi16(i)-1);
    t=r*r/(Rb2+s)+z*z/(Lb2+s);
    display (['i=',int2str(i),' t=',num2str(t,'%12.6g')])
        if (t <= 5) 
           func=sqrt(ksi16(i)/((Lb2/d2-1)*ksi16(i)+1))/abs(((Rb2/d2-1)*ksi16(i)+1));
           GaussIntR16=GaussIntR16+.5*r*w16(i)*func/abs(((Rb2/d2-1)*ksi16(i)+1));
           GaussIntZ16=GaussIntZ16+.5*z*w16(i)*func/abs(((Lb2/d2-1)*ksi16(i)+1));
        else 
            display (['i=',int2str(i),' t=',num2str(t,'%12.6g')])
        end
end

GaussIntRmatlab=quadgk(@myfunFr,0,1)
LobattoIntRmatlab=quadl(@myfunFr,0,1)

GaussIntZmatlab=quadgk(@myfunFz,0,1)
LobattoIntZmatlab=quadl(@myfunFz,0,1)

%
% Where:
%
%--------------------------------------
% function Fz = myfunFz(x)
%     Rb=0.00287883;
%     Lb=0.493179;
%     r=0.0013119;
%     z=0.428186;
% 
%     Rb2=Rb*Rb;
%     Lb2=Lb*Lb;
% 
%    d=(Rb2*Lb)^(1/3);
%    d2=d*d;
% 
%     s=d2*(1./x-1);
%     t=r.*r./(Rb2+s)+z.*z./(Lb2+s);
%     if (t <= 5) 
% 	    func=sqrt(x);
% 		pX=1./sqrt((Rb2/d2-1)*x+1);
% 	    pY=pX;
% 		pZ=1./sqrt((Lb2/d2-1)*x+1);
% 		func=pX.*pY.*pZ.*func;
% 		Fz=z.*pZ.*pZ.*func;
%     end
% 
% end
%---------------------------------
% function Fr = myfunFr(x)
%     Rb=0.00287883;
%     Lb=0.493179;
%     r=0.0013119;
%     z=0.428186;

%     Rb2=Rb*Rb;
%     Lb2=Lb*Lb;
% 
%    d=(Rb2*Lb)^(1/3);
%    d2=d*d;
% 
%     s=d2*(1./x-1);
%     t=r.*r./(Rb2+s)+z.*z./(Lb2+s);
%     if (t <= 5) 
% 	    func=sqrt(x);
% 		pX=1./sqrt((Rb2/d2-1)*x+1);
% 	    pY=pX;
% 		pZ=1./sqrt((Lb2/d2-1)*x+1);
% 		func=pX.*pY.*pZ.*func;
% 		Fr=r.*pX.*pX.*func;
%     end
% 
% end
% 


GaussInt=zeros(9,2);
GaussInt(1,1)=GaussIntR5;
GaussInt(2,1)=GaussIntR6;
GaussInt(3,1)=GaussIntR7;
GaussInt(4,1)=GaussIntR8;
GaussInt(5,1)=GaussIntR9;
GaussInt(6,1)=GaussIntR10;
GaussInt(7,1)=GaussIntR11;
GaussInt(8,1)=GaussIntR12;
GaussInt(9,1)=GaussIntR16;

GaussInt(1,2)=GaussIntZ5;
GaussInt(2,2)=GaussIntZ6;
GaussInt(3,2)=GaussIntZ7;
GaussInt(4,2)=GaussIntZ8;
GaussInt(5,2)=GaussIntZ9;
GaussInt(6,2)=GaussIntZ10;
GaussInt(7,2)=GaussIntZ11;
GaussInt(8,2)=GaussIntZ12;
GaussInt(9,2)=GaussIntZ16;

GaussIntMatlab=zeros(2,2);
GaussIntMatlab(1,1)=GaussIntRmatlab;
GaussIntMatlab(2,1)=GaussIntRmatlab;

GaussIntMatlab(1,2)=GaussIntZmatlab;
GaussIntMatlab(2,2)=GaussIntZmatlab;

pointsNC=[5 6 7 8 9 10 11 12 16]';
pointsGK=[5 16]';

figure (100)
plot(pointsNC,GaussInt(:,1),'-xr',pointsGK,GaussIntMatlab(:,1),'-xb','LineWidth',2)
grid on
xlabel('Points','Fontsize',16,'Color','m')
ylabel('Calculated Integral','Fontsize',16,'Color','m')
title('Numerical Integration over r-Coordinate','Fontsize',16,'Color','m')
legend('Newton-Cotes (NC) Quadrature','Matlab: Gauss-Kronrod (GK) Quadrature','Location','s')
xlim([5,16])

figure (110)
plot(pointsNC,GaussInt(:,2),'-xr',pointsGK,GaussIntMatlab(:,2),'-xb','LineWidth',2)
grid on
xlabel('Points','Fontsize',16,'Color','m')
ylabel('Calculated Integral','Fontsize',16,'Color','m')
title('Numerical Integration over z-Coordinate','Fontsize',16,'Color','m')
legend('Newton-Cotes (NC) Quadrature','Matlab: Gauss-Kronrod (GK) Quadrature','Location','s')
xlim([5,16])

relDev=zeros(9,2);
for i=1:9
    relDev(i,1)=100*(1-GaussInt(i,1)/GaussIntRmatlab);
    relDev(i,2)=100*(1-GaussInt(i,2)/GaussIntZmatlab);
end

figure (120)
plot(pointsNC,relDev(:,1),'-xr',pointsNC,relDev(:,2),'-xb','LineWidth',2)
grid on
xlabel('Points','Fontsize',16,'Color','m')
ylabel('Relative Deviation: (1-NC/GK), %','Fontsize',16,'Color','m')
title('Numerical Integration:  Newton-Cotes (NC) Quadrature','Fontsize',16,'Color','m')
legend('Integration over r-Coordinate','Integration over z-Coordinate','Location','ne')
xlim([5,16])

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Checking a new formulas for SCE:
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Rb=0.00287883;
Lb=0.493179;
r=0.0013119;
z=0.428186;

d=(Rb*Rb*Lb)^(1/3)
d2=d*d

Rb2=2*Rb*Rb/d2
Lb2=2*Lb*Lb/d2

r2=r*r
z2=z*z

ksi12=zeros(12,1);

GaussIntRnew12=0;
GaussIntZnew12=0;
for i=1:12
    ksi12(i)=.5*(psi12(i)+1);
    Fr=1/((1-Rb2)*ksi12(i)+Rb2);
    Fz=1/((1-Lb2)*ksi12(i)+Lb2);
    argExp=(1-ksi12(i))*(r2*Fr+z2*Fz)/d2;
    display (['i=',int2str(i),' argExp=',num2str(argExp,'%12.6g')])
    func=sqrt((1-ksi12(i))*Fz)*exp(-argExp)*Fr;
    GaussIntRnew12=GaussIntRnew12+.5*r*w12(i)*func*Fr;
    GaussIntZnew12=GaussIntZnew12+.5*z*w12(i)*func*Fz;
end

ksi16=zeros(16,1);

GaussIntRnew16=0;
GaussIntZnew16=0;
for i=1:16
    ksi16(i)=.5*(psi16(i)+1);
    Fr=1/((1-Rb2)*ksi16(i)+Rb2);
    Fz=1/((1-Lb2)*ksi16(i)+Lb2);
    argExp=(1-ksi16(i))*(r2*Fr+z2*Fz)/d2;
    display (['i=',int2str(i),' argExp=',num2str(argExp,'%12.6g')])
    func=sqrt((1-ksi16(i))*Fz)*exp(-argExp)*Fr;
    GaussIntRnew16=GaussIntRnew16+.5*r*w16(i)*func*Fr;
    GaussIntZnew16=GaussIntZnew16+.5*z*w16(i)*func*Fz;
end

GaussIntRmatlabNew=quadgk(@myfunFrNew,0,1)
LobattoIntRmatlabNew=quadl(@myfunFrNew,0,1)
GaussIntR12
GaussIntRnew12
GaussIntR16
GaussIntRnew16

GaussIntZmatlabNew=quadgk(@myfunFzNew,0,1)
LobattoIntZmatlabNew=quadl(@myfunFzNew,0,1)
GaussIntZ12
GaussIntZnew16
GaussIntZ12
GaussIntZnew16



GaussIntRmatlabBou=quadgk(@myfunFbou,0,1)
LobattoIntRmatlabBou=quadl(@myfunFbou,0,1)

%------------------------------------------------------
%
%  Comparison ASTRA and Hellweg2D (Lapostolle approach):
%
%------------------------------------------------------

Ibeam=[10 100 1000 10000]';           % mA
rRMS_astra=[4.8 15.7 50.7 144.2]';    % cm
rRMS_hellweg1=[1.3 5.6 22.5 80.0]';   % cm; original E-fields
rRMS_hellweg2=[2.1 8.7 33.5 112]';    % cm; Factors "2" for Er and Ez are taken into account
rRMS_hellweg3=[2.1 9.0 38.1 135.0]';    % cm; Factor "-2" for Ez is taken into account
rRMS_hellweg4=[2.2 9.6 40.2 144.0]';    % cm; without factor 1/gamma^2 for Er


figure (200)
semilogx(.001*Ibeam,rRMS_astra,'-xr',.001*Ibeam,rRMS_hellweg1,'-xb','LineWidth',2)
grid on
xlabel('I_{beam}, A','Fontsize',16,'Color','m')
ylabel('RMS Beam Size (Transversal), cm','Fontsize',16,'Color','m')
title('Space Charge Effect: Drift Gap (L=2.5 m)','Fontsize',16,'Color','m')
legend('ASTRA','Hellweg2D','Location','nw')
text(0.12,135,'Initial r_{RMS}=2.8 mm','FontSize',16,'Color','m')

figure (210)
semilogx(.001*Ibeam,rRMS_astra./rRMS_hellweg1,'-xr','LineWidth',2)
grid on
xlabel('I_{beam}, A','Fontsize',16,'Color','m')
ylabel('SCE for RMS Beam Size: "ASTRA"/"Hellweg2D"','Fontsize',16,'Color','m')
title('Space Charge Effect (SCE): Drift Gap (L=2.5 m)','Fontsize',16,'Color','m')
ylim([1.5,4.0])

figure (220)
semilogx(.001*Ibeam,rRMS_astra./rRMS_hellweg1,'-xk',.001*Ibeam,rRMS_astra./rRMS_hellweg2,'-xr', ...
        .001*Ibeam,rRMS_astra./rRMS_hellweg3,'-xb',.001*Ibeam,rRMS_astra./rRMS_hellweg4,'-xm','LineWidth',2)
grid on
xlabel('I_{beam}, A','Fontsize',16,'Color','m')
ylabel('SCE for RMS Beam Size: "ASTRA"/"Hellweg2D"','Fontsize',16,'Color','m')
title('Space Charge Effect (SCE): Drift Gap (L=2.5 m)','Fontsize',16,'Color','m')
legend('Original Hellweg2D','+ Factor "2" for E_r,E_z','+ Factor "-2" for E_z','+ Factor "gamma^2" for E_r','Location','ne')




%
% for i=1:5
%     display(['   ',num2str(psi5(i),'%9.7f'),'   ',num2str(w5(i),'%9.7f')])
% end
% display('  ')
% display('  ')
% display('  ')
% for i=1:6
%     display(['   ',num2str(psi6(i),'%9.7f'),'   ',num2str(w6(i),'%9.7f')])
% end
% display('  ')
% display('  ')
% display('  ')
% for i=1:7
%     display(['   ',num2str(psi7(i),'%9.7f'),'   ',num2str(w7(i),'%9.7f')])
% end
% display('  ')
% display('  ')
% display('  ')
% for i=1:8
%     display(['   ',num2str(psi8(i),'%9.7f'),'   ',num2str(w8(i),'%9.7f')])
% end
% display('  ')
% display('  ')
% display('  ')
% for i=1:9
%     display(['   ',num2str(psi9(i),'%9.7f'),'   ',num2str(w9(i),'%9.7f')])
% end
% display('  ')
% display('  ')
% display('  ')
% for i=1:10
%     display(['   ',num2str(psi10(i),'%9.7f'),'   ',num2str(w10(i),'%9.7f')])
% end
% display('  ')
% display('  ')
% display('  ')
% for i=1:11
%     display(['   ',num2str(psi11(i),'%9.7f'),'   ',num2str(w11(i),'%9.7f')])
% end
% display('  ')
% display('  ')
% display('  ')
f% or i=1:12
%     display(['   ',num2str(psi12(i),'%9.7f'),'   ',num2str(w12(i),'%9.7f')])
% end
% display('  ')
% display('  ')
% display('  ')
% for i=1:16
%     display(['   ',num2str(psi16(i),'%9.7f'),'   ',num2str(w16(i),'%9.7f')])
% end



















