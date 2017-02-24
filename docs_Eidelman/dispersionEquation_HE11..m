M=zeros(4,4);

xMax=25;
nMax=500;

x=zeros(nMax,1);
modK1=zeros(nMax,1);
modI0=zeros(nMax,1);
modI1=zeros(nMax,1);
modI2=zeros(nMax,1);
modJ0=zeros(nMax,1);
modJ1=zeros(nMax,1);
modJ2=zeros(nMax,1);
modJ1d=zeros(nMax,1);
modI1d=zeros(nMax,1);

for n=1:nMax
    x(n)=xMax/nMax*n;
end
[modJ0,ierr]=besselj(0,x);
[modJ1,ierr]=besselj(1,x);
[modJ2,ierr]=besselj(2,x);
modJ1d=.5*(modJ0-modJ2);


figure(5)
plot(x,modJ0,'-m',x,modJ1d,'-r',x,modJ2,'-b','LineWidth',2)
xlabel('x','FontSize',16,'Color','m')
ylabel('Bessel funcions:','FontSize',16,'Color','m')
grid on
legend('J_0','J_1d','J_2','Location','n')

figure(7)
plot(x,modJ1d,'-r','LineWidth',2)
xlabel('x','FontSize',16,'Color','m')
ylabel('Bessel funcion: J''_1','FontSize',16,'Color','m')
grid on

modI1(n)=besseli(1,x(n));
    modI2(n)=besseli(2,x(n));
    modI1d(n)=9.5*(modI0(n)+modI2(n));
    modK1(n)=besselk(1,x(n));

figure(5)
plot(x,modI0,'-r','LineWidth',2)
xlabel('x','FontSize',16,'Color','m')
ylabel('Bessel funcions:','FontSize',16,'Color','m')
grid on
legend('I_0','I_1','Location','n')

figure(10)
plot(x,modI1,'-r',x,modK1,'-b','LineWidth',2)
xlabel('x','FontSize',16,'Color','m')
ylabel('Bessel funcions:','FontSize',16,'Color','m')
grid on
legend('I_1','K_1','Location','n')

figure(20)
plot(x,modI1d,'-r',x,modI0,'-b',x,modI2,'-b','LineWidth',2)
xlabel('x','FontSize',16,'Color','m')
ylabel('Bessel funcions:','FontSize',16,'Color','m')
grid on
legend('I''_1','I_0','I_2','Location','n')

