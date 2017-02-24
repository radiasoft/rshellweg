ksi5=[-0.9061798,-0.5384093,0.0,0.5384093,0.9061798]';
w5=[0.2369269,0.4786287,0.5688889,0.4786287,0.2369269]';
ksi6=[-0.9324695,-0.6612094,-0.2386192,0.2386192,0.6612094,0.9324695]';
w6=[0.1713245,0.3607616,0.4679139,0.4679139,0.3607616,0.1713245]';

component=1;
Rb2=;
Lb2=;
r=;
z=;
	
Rb2=Rb^2;
Lb2=Lb^2;
d=(Rb2*Lb)^(1/3);
d2=d^2;
	
GInt=0;
for i=1:6
	s = d2*(1/ksi6(i)-1);
	t = r^2/(Rb2+s)+z^2/(Lb2+s);
	if (t <= 5) 
		func = sqrt(abs(ksi6[i]));
		pX = 1/sqrt((Rb2/d2-1)*ksi6(i)+1);
		pY = pX;
		pZ = 1/sqrt((Lb2/d2-1)*ksi6(i)+1);
		func = func*pX*pY*pZ;
		if (component < 3) 
		    func = func*r*pX^2;
        end
		if (component == 3) 
		    func = func*z*pZ^2;
        end
		GInt = GInt+.5*func*w6(i); 
    end 
end 
