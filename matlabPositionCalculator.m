prompt = "Enter Ax: ";
Ax = input(prompt)
prompt = "Enter Ay: ";
Ay = input(prompt)
prompt = "Enter Az: ";
Az = input(prompt)

prompt = "Enter Bx: ";
Bx = input(prompt)
prompt = "Enter By: ";
By = input(prompt)
prompt = "Enter Bz: ";
Bz = input(prompt)

prompt = "Enter Cx: ";
Cx = input(prompt)
prompt = "Enter Cy: ";
Cy = input(prompt)
prompt = "Enter Cz: ";
Cz = input(prompt)

S = sqrt(power((Ax*(By-Cy)+Bx*(Cy-Ay)+Cx*(Ay-By)), 2) ...
    +power((Ay*(Bz-Cz)+By*(Cz-Az)+Cy*(Az-Bz)), 2) ...
    +power((Az*(Bx-Cx)+Bz*(Cx-Ax)+Cz*(Ax-Bx)), 2))/2

xo = (Ax+Bx+Cx)/3;
yo = (Ay+By+Cy)/3;

% centroid cable lengths, used to subtract when homed at centroid
%uc = 0.7702;
%vc = 0.845;
%wc = 0.7702;

uc = 0.7793;
vc = 0.7602;
wc = 0.7793;

steps = 40000;

%offsetRatio = 0.5;

while 1 == 1
    prompt = "Enter X: ";
    x = input(prompt)
    prompt = "Enter Y: ";
    y = input(prompt)
    prompt = "Enter Z: ";
    

    
    %distFromOrigin = sqrt(power((x-xo),2) + power((y-yo),2));

    %zOffset = distFromOrigin*offsetRatio;

    z = input(prompt)
    %z = z-zOffset;

    STEPERA = (sqrt(power((x*(By-Cy)+Bx*(Cy-y)+Cx*(y-By)), 2)+ ...
        power((y*(Bz-Cz)+By*(Cz-z)+Cy*(z-Bz)), 2)+ ...
        power((z*(Bx-Cx)+Bz*(Cx-x)+Cz*(x-Bx)), 2)) / (2*S) - uc)*steps

    STEPPERB = (sqrt(power((Ax*(y-Cy)+x*(Cy-Ay)+Cx*(Ay-y)), 2) ...
        +power((Ay*(z-Cz)+y*(Cz-Az)+Cy*(Az-z)), 2) ...
        +power((Az*(x-Cx)+z*(Cx-Ax)+Cz*(Ax-x)), 2)) / (2*S) - vc)*steps

    STEPPERC = (sqrt(power((Ax*(By-y)+Bx*(y-Ay)+x*(Ay-By)), 2) ...
        +power((Ay*(Bz-z)+By*(z-Az)+y*(Az-Bz)), 2) ...
        +power((Az*(Bx-x)+Bz*(x-Ax)+z*(Ax-Bx)), 2)) / (2*S) - wc)*steps
    
end

