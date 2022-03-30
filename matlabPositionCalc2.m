PosVector = zeros(1,3);
PulleyAVector = zeros(1,3);
PulleyBVector = zeros(1,3);
PulleyCVector = zeros(1,3);

StepsPerCM = 101.01;

Xtravel = 224;
Ytravel = 205;
Ztravel = 238;

PulleyAVector = [0, 0, Ztravel];
PulleyBVector = [(Xtravel/2), Ytravel, Ztravel];
PulleyCVector = [(Xtravel), 0, Ztravel];

%enter origin coords
%PosVector = [112,(Ytravel/3),119]; %middle coord
PosVector = [106,106,87]; %origin coord

NewPosVector = [106, 180, 87];

VectorA = PosVector - PulleyAVector;
VectorB = PosVector - PulleyBVector;
VectorC = PosVector - PulleyCVector;

VectorAFinal = NewPosVector - PulleyAVector;
VectorBFinal = NewPosVector - PulleyBVector;
VectorCFinal = NewPosVector - PulleyCVector;



LengthA = norm(VectorA);
LengthB = norm(VectorB);
LengthC = norm(VectorC);

LengthAFinal = norm(VectorAFinal);
LengthBFinal = norm(VectorBFinal);
LengthCFinal = norm(VectorCFinal);

%1cm per 100 steps

DeltaA = (LengthAFinal - LengthA)*-StepsPerCM
DeltaB = (LengthBFinal - LengthB)*-StepsPerCM
DeltaC = (LengthCFinal - LengthC)*-StepsPerCM