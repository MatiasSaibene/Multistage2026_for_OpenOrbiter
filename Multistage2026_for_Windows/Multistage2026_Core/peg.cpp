#define STRICT
#include "../../include/Orbitersdk.h"
#include "Multistage2026.hpp"

VECTOR3 Multistage2026::GetAPTargets(){

	return _V(tgtapo, tgtperi, tgtinc);

}

//Get Vehicle Acceleration along Velocity Vector
double Multistage2026::getacc() {

	double acc;
	VECTOR3 force_vec, acc_vec, spd_vec;
	VECTOR3 weight_vec, thrust_vec, drag_vec;
	// Get the vectors 
	//GetShipAirspeedVector(spd_vec);
	GetGroundspeedVector(FRAME_HORIZON, spd_vec);
	GetThrustVector(thrust_vec);
	GetWeightVector(weight_vec);
	GetDragVector(drag_vec);
	force_vec.x = thrust_vec.x + weight_vec.x + drag_vec.x;
	force_vec.y = thrust_vec.y + weight_vec.y + drag_vec.y;
	force_vec.z = thrust_vec.z + weight_vec.z + drag_vec.z;
	// Normalize the speed vector
	normalise(spd_vec);
	// Calculate the acceleration vector
	acc_vec = force_vec / GetMass();

	// Take the dot product
	acc = dotp(acc_vec, spd_vec);

	return acc;
}

double Multistage2026::getabsacc(){
	VECTOR3 ThrustVec, DragVec;
	GetThrustVector(ThrustVec);
	GetDragVector(DragVec);
	return (length(ThrustVec) - length(DragVec)) / GetMass();
}

//Initialize PEG
void Multistage2026::InitPEG(){

	oapiWriteLog(const_cast<char*>("RESET PEG"));

	for (int i = 1; i <= nStages; i++) {
		v_e.at(i) = stage.at(i - 1).isp;
		a_.at(i) = stage.at(i - 1).thrust / GetMassAtStage(i - 1, false);
		a_fin.at(i) = stage.at(i - 1).thrust / GetMassAtStage(i - 1, true);
		tau_.at(i) = v_e.at(i) / a_.at(i);
		A.at(i) = 0;
		B.at(i) = 0;
	}

	NN = nStages;
	for (int i = 0; i < 10; i++){
		T.at(i) = 0;
		OmegaS.at(i) = 0;
		delta_rdot.at(i) = 0;
		rdot_in.at(i) = 0;
		delta_r.at(i) = 0;
		r_in.at(i) = 0;
		r_T.at(i) = 0;
		rdot_T.at(i) = 0;
		VthetaAtStaging.at(i) = 0;
	}

}

void Multistage2026::CalculateTargets(){
	if ((tgtabside > tgtapo) || (tgtabside < tgtperi)){
		if (tgtperi >= 80000){
			tgtabside = tgtperi;
		} else {
			tgtabside = tgtapo;
		}
	}
	double etarget = (tgtapo + rt - tgtperi - rt) / (tgtapo + 2 * rt + tgtperi);
	double atarget = (tgtapo + rt) / (1 + etarget);
	epsfin = -mu / (2 * atarget);
	double vr_target = sqrt(mu * (2 / (tgtabside + rt) - 1 / (0.5 * (tgtapo + tgtperi + 2 * rt))));
	if (etarget == 0){
		tgtvv = 0;
	} else {
		if ((tgtabside == tgtperi) || (tgtabside == tgtapo)) {
			tgtvv = 0;
		} else {
			double cosTA = (atarget * (1 - (etarget * etarget)) - (tgtabside + rt)) / ((tgtabside + rt) * etarget);
			double tgtTrueAnomaly = acos(cosTA);
			tgtvv = vr_target * (etarget * sin(tgtTrueAnomaly)) / (1 + etarget * cos(tgtTrueAnomaly));
		}
	}
}

bool Multistage2026::CutoffCheck(){
	OBJHANDLE hearth = GetSurfaceRef();
	GetElements(hearth, el, &op, 0, 1);
	eps = -mu / (2 * el.a);
	
    if(eps >= epsfin){
		killAP();
		runningPeg = false;
		return true;
	}
	return false;
}

void Multistage2026::CalcMECO(){
	
    double MECO = 0;
	for (int i = 1; i <= NN; i++){
		MECO += T.at(i);
	}

	MECO += MET;
	if (MECO_Counter >= 100){
        MECO_Counter = 0;
    }

	double delta = MECO - MECO_TEST;
	coeff.at(MECO_Counter) = delta / UpdatePegTimer;
	double transcoeff = 0;
	for (int h = 0; h < 100; h++){
		transcoeff += coeff.at(h);
	}
	avgcoeff = transcoeff / 100;
	MECO_TEST = MECO;
	MECO_Counter++;

	if (avgcoeff < 1) {
        TMeco = MET + (MECO - MET) / (1 - avgcoeff);
    } else {
        TMeco = MECO;
    }
	
}

void Multistage2026::Navigate(){

	J = currentStage + 1;
	//GetHorizonAirspeedVector(ShipSpeed);
	GetGroundspeedVector(FRAME_HORIZON, ShipSpeed);
	VertVel = ShipSpeed.y;
	GetRelativePos(GetGravityRef(), rvec);
	GetRelativeVel(GetGravityRef(), vvec);
	r = length(rvec);
	v = length(vvec);
	hvec = crossp(rvec, vvec);
	h = length(hvec);
	thrust = stage.at(currentStage).thrust;
	mass = GetMass();

	omega = h / (r * r);
	if (wBoosters) {
        a_.at(J) = thrust / mass;
    } else {
        a_.at(J) = getabsacc();
    }
	v_e.at(J) = stage.at(J - 1).isp;
	tau_.at(J) = v_e.at(J) / a_.at(J);
	NN = coc.stage + 1;

	for (int i = 1; i <= NN; i++){
		T.at(i) = coc.remBT.at(i - 1);
	}

	g = -mu / (r * r);
	cent = omega * omega * r;
	g_term = -g / a_.at(J);
	cent_term = -cent / a_.at(J);

	A.at(J - 1) = 0;
	B.at(J - 1) = 0;
	DeltaA.at(J - 1) = 0;
	DeltaB.at(J - 1) = 0;
	DeltaA.at(NN) = 0;
	DeltaB.at(NN) = 0;

}

double Multistage2026::b0(int j, double t_j){

	return -v_e.at(j) * log(1 - t_j / tau_.at(j));

}

double Multistage2026::b_(int n, int j, double t_j){
	
    if (n == 0){
        return b0(j, t_j);
    }
	
    return b_(n - 1, j, t_j) * tau_.at(j) - v_e.at(j) * pow(t_j, n) / n;
}

double Multistage2026::c0(int j, double t_j){

	return b0(j, t_j) * t_j - b_(1, j, t_j);

}

double Multistage2026::c_(int n, int j, double t_j){
	
    if (n == 0){
        return c0(j, t_j);
    }

	return c_(n - 1, j, t_j) * tau_.at(j) - v_e.at(j) * pow(t_j, n + 1) / (n * (n + 1));
}

double Multistage2026::a(int j, double t_j){

	return a_.at(j) / (1 - t_j / tau_.at(j));

}

void Multistage2026::FEstimate(){

	for (int i = 0; i < 10; i++){
		coc.remBT.at(i) = 0;
	}

	double T_MECO = 0;
	double RemDvToDo = finalv(tgtabside, tgtapo, tgtperi) - GetOS();
	double RemDv = RemDvToDo;
	int q = 0;

	while((RemDv > StageRemDv(currentStage + q)) && (currentStage + q < nStages - 1)){
		RemDv -= StageRemDv(currentStage + q);
		coc.remBT.at(currentStage + q) = RemBurnTime(currentStage + q, 1);
		T_MECO += RemBurnTime(currentStage + q, 1);
		q++;
	}

	T_MECO += RemDv * GetPropellantMass(stage.at(currentStage + q).tank) / (stage.at(currentStage + q).thrust * log((GetMassAtStage(currentStage + q, true) + GetPropellantMass(stage.at(currentStage + q).tank)) / GetMassAtStage(currentStage + q, true)));
	coc.remBT.at(currentStage + q) = RemDv * GetPropellantMass(stage.at(currentStage + q).tank) / (stage.at(currentStage + q).thrust * log((GetMassAtStage(currentStage + q, true) + GetPropellantMass(stage.at(currentStage + q).tank)) / GetMassAtStage(currentStage + q, true)));

	coc.T = T_MECO;
	coc.stage = currentStage + q;
	coc.rem_stages = q;

	return;
}

void Multistage2026::FStaging(){
	r_T.at(J) = r + VertVel * T.at(J) + c0(1, T.at(J)) * A.at(J) + c_(1, 1, T.at(J)) * B.at(J);
	rdot_T.at(J) = VertVel + b0(1, T.at(J)) * A.at(J) + b_(1, 1, T.at(J)) * B.at(J);

	r_in.at(J) = r;
	rdot_in.at(J) = VertVel;

	for (int i = J + 1; i < NN; i++){
		delta_rdot.at(i) = b0(i, T.at(i)) * A.at(i) + b_(1, i, T.at(i)) * B.at(i);
		delta_r.at(i) = rdot_in.at(i) * T.at(i) + c0(i, T.at(i)) * A.at(i) + c_(1, i, T.at(i)) * B.at(i);
	}

	for (int i = J + 1; i < NN; i++){
		rdot_in.at(i) = rdot_in.at(J);
		r_in.at(i) = r_in.at(J);
		for (int j = J + 1; j <= i - 1; j++){
			rdot_in.at(i) += delta_rdot.at(j);
			r_in.at(i) += delta_r.at(j);
		}
	}

	for (int j = J + 1; j < NN; j++){
		rdot_T.at(j) = rdot_in.at(j);
		r_T.at(j) = r_in.at(j);
	}

	r_T.at(NN) = tgtabside + rt;//tgtapo+rt;
	rdot_T.at(NN) = tgtvv;//0;

	VthetaAtStaging.at(J) = GetOS() + StageRemDv(J - 1);
	for (int i = J + 1; i <= NN; i++){
		VthetaAtStaging.at(i) = VthetaAtStaging.at(i - 1) + StageRemDv(i - 1);
	}
	for (int i = J; i <= NN; i++){
		OmegaS.at(i) = VthetaAtStaging.at(i) / r_T.at(i);//FredEstimateStaging().r_T.at(i);
	}

	return;
}

//void Multistage2015::PEG(){
void Multistage2026::MajorCycle(){
	CalculateTargets();
	Navigate();
	FStaging();
	FEstimate();
	CalcMECO();

	for (int i = currentStage + 1; i < NN; i++){
		DeltaA.at(i) = (mu / (r_T.at(i) * r_T.at(i)) - OmegaS.at(i) * OmegaS.at(i) * r_T.at(i)) * ((1 / (a_fin.at(i)) - (1 / (a_.at(i + 1)))));
		DeltaB.at(i) = (mu / (r_T.at(i) * r_T.at(i)) - OmegaS.at(i) * OmegaS.at(i) * r_T.at(i)) * ((1 / v_e.at(i)) - (1 / v_e.at(i + 1))) + ((3 * OmegaS.at(i) * OmegaS.at(i) - 2 * mu / (r_T.at(i) * r_T.at(i) * r_T.at(i))) * rdot_T.at(i)) * ((1 / (a_fin.at(i)) - (1 / (a_.at(i + 1)))));
	}

	double alpha = 0;
	for (int l = J; l <= NN; l++){
		alpha += b0(l, T.at(l));
	}

	double beta = 0;
	for (int l = J; l <= NN; l++){
		double term = 0;
		for (int k = J; k <= l - 1; k++){
			term += T.at(k);
		}
		beta += b_(1, l, T.at(l)) + b0(l, T.at(l)) * term;
	}

	double gamma = 0;
	for (int l = J; l <= NN; l++){
		double term1 = 0;
		for (int k = J; k <= l - 1; k++){
			double term2 = 0;
			for (int i = J; i <= k - 1; i++){
				term2 += DeltaB.at(i);
			}
			term1 += b0(l, T.at(l)) * DeltaA.at(k) + b0(l, T.at(l)) * T.at(k) * term2 + b_(1, l, T.at(l)) * DeltaB.at(k);
		}
		gamma += term1;
	}

	gamma = gamma * (-1);
	gamma += tgtvv - VertVel;

	double epsilon = 0;
	for (int l = J; l <= NN; l++){
		double term = 0;
		for (int k = J; k <= l - 1; k++){
			term += b0(k, T.at(k));
		}
		epsilon += c0(l, T.at(l)) + T.at(l) * term;
	}


	double zeta = 0;

	for (int l = J; l <= NN; l++){
		double term2 = 0;
		for (int k = J; k <= l - 1; k++){
			double term1 = 0;
			for (int i = J; i <= k - 1; i++){
				term1 += T.at(i);
			}
			term2 += c0(l, T.at(l)) * T.at(k) + b_(1, k, T.at(k)) * T.at(l) + b0(k, T.at(k)) * T.at(l) * term1;
		}
		zeta += c_(1, l, T.at(l)) + term2;
	}

	double iota = 0;
	for (int l = J; l <= NN; l++){
		double term3 = 0;
		for (int k = J; k <= l - 1; k++){
			double term2 = 0;
			for (int i = J; i <= k - 1; i++){
				double term1 = 0;
				for (int m = J; m <= i - 1; m++){
					term1 += DeltaB[m];
				}
				term2 += b0(k, T.at(k)) * T.at(l) * DeltaA.at(i) + b0(k, T.at(k)) * T.at(i) * T.at(l) * term1 + b_(1, k, T.at(k)) * T.at(l) * DeltaB.at(i) + c0(l, T.at(l)) * T.at(k) * DeltaB.at(i);
			}
			term3 += c0(l, T.at(l)) * DeltaA.at(k) + c_(1, l, T.at(l)) * DeltaB.at(k) + term2;
		}
		iota += term3;
	}

	double k23 = 0;
	for (int l = J; l <= NN; l++){
		k23 += T.at(l);
	}

	k23 = k23 * VertVel;
	double lambda = (tgtabside + rt) - k23 - (GetAltitude() + rt) - iota;

	A[J] = (gamma * zeta - beta * lambda) / (alpha * zeta - beta * epsilon);
	B[J] = (alpha * lambda - gamma * epsilon) / (alpha * zeta - beta * epsilon);

	double transvarA = 0;
	for (int l = J; l <= NN; l++){
		double term1 = 0;
		for (int k = J; k < l - 1; k++){
			term1 += DeltaB.at(k);
		}
		transvarA += DeltaA.at(l) + B.at(1) * T.at(l) + T.at(l) * term1;
	}

	double transvarB = 0;
	for (int l = J; l <= NN; l++){
		transvarB += DeltaB.at(l);
	}

	for (int l = J + 1; l <= NN; l++){
		A.at(l) = A.at(l - 1) + transvarA;
		B.at(l) = B.at(l - 1) + transvarB;
	}

}

void Multistage2026::PEG() {

	UpdatePegTimer += oapiGetSimStep();

	if (UpdatePegTimer >= PegMajorCycleInterval) //0.1 was here
	{
		MajorCycle();
		UpdatePegTimer = 0;
	}



	double arg = 0;

	if ((A.at(J) < 10) && (A.at(J) > -10)){
		//arg=A[J]+B[J]*oapiGetSimStep()+g_term+cent_term;
		arg = A.at(J) + B.at(J) * UpdatePegTimer + g_term + cent_term;
	} else {
		InitPEG();
	}

	if (abs(arg) < sin(PegPitchLimit)){
		PegDesPitch = asin(arg);
	} else {
		PegDesPitch = PegPitchLimit * SignOf(arg);//*(abs(arg)/arg);
	}

	if (abs(arg) < 1){
		TgtPitch = asin(arg);
	} else {
		TgtPitch = 90 * RAD * SignOf(arg);
	}

	// ACCOUNT FOR THRUST NOT IN INLINE WITH VEHICLE
	VECTOR3 thrustVector;
	GetThrustVector(thrustVector);
	if (length(thrustVector) > 0){
		normalise(thrustVector);
		PegDesPitch -= asin(thrustVector.y);
		TgtPitch -= asin(thrustVector.y);
	}

	Attitude(PegDesPitch, (0.5 * (1 - VinkaMode) * PI), GetProperHeading(), 8, 5, 8);

}

void Multistage2026::SetPegMajorCycleInterval(double newinterval){
	PegMajorCycleInterval = newinterval;
}

void Multistage2026::SetPegPitchLimit(double newlimit){
	PegPitchLimit = newlimit;
}

