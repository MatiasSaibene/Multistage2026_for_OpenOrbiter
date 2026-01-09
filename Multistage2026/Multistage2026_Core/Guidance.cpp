#include "../../include/Orbitersdk.h"
#include "Multistage2026.hpp"
#include <algorithm>
#include <array>
#include <format>

double Multistage2026::finalv(double Abside, double Apo,double Peri){

	return sqrt(mu*(2/(Abside+rt)-1/(0.5*(Apo+Peri+2*rt))));

}

double Multistage2026::CalcAzimuth(){
    
    double heading;

    double lon,lat,rad;
    GetEquPos(lon,lat,rad);
    const double wearth=2*PI/oapiGetPlanetPeriod(GetSurfaceRef()); 
	double EastVel=abs((oapiGetSize(GetSurfaceRef())+GetAltitude())*wearth*cos(lat));
	VECTOR3 hspd;
//	GetHorizonAirspeedVector(hspd);
	GetGroundspeedVector(FRAME_HORIZON,hspd);
	EastVel+=(hspd.x);
	double NorthVel=hspd.z;
	
	if(GetAltitude()>5000){
		if(SignOf(tgtinc)!=SignOf(NorthVel)) ///CHECK FOR THE PARTICULAR AMBIGUITY
        {
			tgtinc=-tgtinc;
		}	
	}

	heading=atan2((finalv(tgtabside,tgtapo,tgtperi))*(sin(asin(cos(tgtinc)/cos(lat))))-EastVel,((finalv(tgtabside,tgtapo,tgtperi))*(cos(asin(cos(tgtinc)/cos(lat))))-abs(NorthVel)));
	
	heading+=(90*RAD-heading)*(1-SignOf(tgtinc));
	
	return heading;
}

double Multistage2026::GetProperHeading(){

	double heading;
	if(!runningPeg){
		if(GetAltitude()<3000){ //JUST A SAFETY IN CASE ROLL HAS SOMETHING BAD
		heading=VinkaAzimuth;
		} else {
		oapiGetHeading(GetHandle(),&heading);
		}

	} else {
	
	    double lon,lat,rad;
	    GetEquPos(lon,lat,rad);

	    if(abs(tgtinc)<abs(lat)){
		    if(lat<0){
                tgtinc=abs(lat);	
            } else {
                tgtinc=-abs(lat);
            }
	    }
	
	    heading=CalcAzimuth();
	}
	return heading;
}

double Multistage2026::GetVPerp(){

	OBJHANDLE hearth=GetSurfaceRef();
	VECTOR3 relvel,loc_rvel,horvel,glob_vpos;
	GetRelativeVel(hearth,relvel);
	GetGlobalPos(glob_vpos);
	Global2Local((relvel + glob_vpos), loc_rvel);
 
	HorizonRot(loc_rvel,horvel);
	//horvel.x=0;
	//horvel.z=0;
	
	return horvel.y;
}

double Multistage2026::GetProperRoll(double RequestedRoll=0){

	if(RequestedRoll<0){
        RequestedRoll+=2*PI;
    }
	double GB=GetBank();
	if(GB<0){
        GB+=2*PI;
    }
	double roll;
	roll=GB-RequestedRoll;
	if(roll>PI){
        roll-=2*PI;
    }
	return roll;
}

//Attitude Function! Got the inspiration from the OrientForBurn of Hlynkacg

void Multistage2026::Attitude(double pitch,double roll, double heading,double pitchrate=2, double rollrate=8, double yawrate=5){
	
    if(AttCtrl){
        double pdb;								//Deadband for rates, way higher if Time Acceleration is applied
    //	if(oapiGetTimeAcceleration()>1){
        //	pdb=0.1;
        //}else{
        pdb=0.1;
        //}


        VECTOR3 PMI;
        GetPMI(PMI);
        double ReqTime=oapiGetSimStep()*oapiGetTimeAcceleration();
        VECTOR3 ReqAngularAcc=_V(0,0,0);
        VECTOR3 ReqTorque=_V(0,0,0);
        
        VECTOR3 thetain=_V(cos(pitch)*sin(heading),sin(pitch),cos(pitch)*cos(heading));  // Getting the proper input vector from pitch and heading
        
        VECTOR3 tgtVector;
        HorizonInvRot(thetain,tgtVector);


        normalise (tgtVector);
        VECTOR3 ThrustVector = {0,0,1}; 
        VECTOR3 input = crossp(tgtVector,ThrustVector);
        
        input.z=GetProperRoll(roll);

        
        if(abs(input.z)>1){
            //input.z=input.z/abs(input.z);
            input.z=SignOf(input.z);
        }
        VECTOR3 aVel; 
        GetAngularVel(aVel);
        //aVel=operator/(aVel,oapiGetTimeAcceleration());

        double PitchV = aVel.x*DEG;	
        double RollV = aVel.z*DEG;	
        double YawV = aVel.y*DEG;	
        
        // PITCH
        double CurrentRate = PitchV;
        double CommandedRate = asin(input.x)*DEG;
        CommandedRate=((min(abs(CommandedRate),pitchrate))*SignOf(CommandedRate));
        double pDelta = (CommandedRate - CurrentRate); 
        

        VECTOR3 tgtrotlevel;
        GetAttitudeRotLevel(tgtrotlevel);


        if(PitchCtrl){
        if(abs(pDelta)>pdb){
                        
            ReqAngularAcc.x=pDelta*RAD/ReqTime;
            ReqTorque.x=PMI.x*GetMass()*ReqAngularAcc.x;
            tgtrotlevel.x=min(abs(ReqTorque.x/MaxTorque.x),1)*SignOf(ReqTorque.x);
        
            } else {
                tgtrotlevel.x=0;
            }
            SetAttitudeRotLevel(0,tgtrotlevel.x);
        }
        
        // ROLL
        CurrentRate = RollV;
        CommandedRate = asin(input.z)*DEG;
        CommandedRate=((min(abs(CommandedRate),rollrate))*SignOf(CommandedRate));
        double rDelta = (CommandedRate - CurrentRate); 
        
        if(RollCtrl){
        
            if(abs(rDelta)>pdb){
                ReqAngularAcc.z=rDelta*RAD/ReqTime;
                ReqTorque.z=PMI.z*GetMass()*ReqAngularAcc.z;
                tgtrotlevel.z=min(abs(ReqTorque.z/MaxTorque.z),1)*SignOf(ReqTorque.z);
            } else {
                tgtrotlevel.z=0;
            }
            SetAttitudeRotLevel(2,tgtrotlevel.z);
        }
        
        // YAW
        CurrentRate = YawV; 
        CommandedRate = asin(input.y)*DEG;
        CommandedRate=((min(abs(CommandedRate),yawrate))*SignOf(CommandedRate));
        double yDelta = (CommandedRate - CurrentRate); 
        
        if(YawCtrl){
            if(abs(yDelta)>pdb){
            ReqAngularAcc.y=yDelta*RAD/ReqTime;
            ReqTorque.y=PMI.y*GetMass()*ReqAngularAcc.y;
            tgtrotlevel.y=min(abs(ReqTorque.y/MaxTorque.y),1)*SignOf(ReqTorque.y);
            } else {
                tgtrotlevel.y=0;
            }
            SetAttitudeRotLevel(1,tgtrotlevel.y);
        }

	}
	
}

void Multistage2026::ToggleAP(){
	
    if(APstat){
	    APstat=false;
	} else {
	    APstat=true;
	}
}

void Multistage2026::ToggleAttCtrl(bool Pitch, bool Yaw, bool Roll){

	if((Pitch)&&(Yaw)&&(Roll)){

	    if(AttCtrl){
    		AttCtrl=false;
    	} else {
		    AttCtrl=true;
	    }
	} else if((Pitch)&&(!Yaw)&&(!Roll)){
		if(PitchCtrl){
			PitchCtrl=false;
		} else {
			PitchCtrl=true;
		}
	} else if((!Pitch)&&(Yaw)&&(!Roll)){
		if(YawCtrl){
			YawCtrl=false;
		} else {
			YawCtrl=true;
		}
	} else if((!Pitch)&&(!Yaw)&&(Roll)){
		if(RollCtrl){
			RollCtrl=false;
		} else {
			RollCtrl=true;
		}
	}
}

void Multistage2026::killAP(){
	SetThrusterGroupLevel(THGROUP_MAIN,0);
	SetThrusterGroupLevel(THGROUP_ATT_PITCHUP,0);
	SetThrusterGroupLevel(THGROUP_ATT_PITCHDOWN,0);
	SetThrusterGroupLevel(THGROUP_ATT_YAWLEFT,0);
	SetThrusterGroupLevel(THGROUP_ATT_YAWRIGHT,0);
	SetThrusterGroupLevel(THGROUP_ATT_BANKRIGHT,0);
	SetThrusterGroupLevel(THGROUP_ATT_BANKLEFT,0);
	ActivateNavmode(NAVMODE_KILLROT);
}

void Multistage2026::VinkaUpdateRollTime(){

	Gnc_step.at(VinkaFindRoll()).time_fin=Gnc_step.at(VinkaFindFirstPitch()).time-1;

}

void Multistage2026::VinkaComposeGNCSteps(){

	for(int i=0;i<=nsteps;i++){
		Gnc_step.at(i).executed=false;
		
        std::transform(Gnc_step.at(i).Comand.begin(), Gnc_step.at(i).Comand.end(), Gnc_step.at(i).Comand.begin(),
        [](unsigned char c){ return std::tolower(c); }
        );
        
		if(Gnc_step.at(i).Comand == "engine"){
			Gnc_step.at(i).GNC_Comand = CM_ENGINE;
		} else if(Gnc_step.at(i).Comand == "roll"){
			Gnc_step.at(i).GNC_Comand=CM_ROLL;
		}else if(Gnc_step.at(i).Comand == "pitch"){
			Gnc_step.at(i).GNC_Comand=CM_PITCH;
		}else if(Gnc_step.at(i).Comand == "fairing"){
			Gnc_step.at(i).GNC_Comand=CM_FAIRING;
		}else if(Gnc_step.at(i).Comand == "les"){
			Gnc_step.at(i).GNC_Comand=CM_LES;
		}else if(Gnc_step.at(i).Comand == "disablepitch"){
			Gnc_step.at(i).GNC_Comand=CM_DISABLE_PITCH;
		}else if(Gnc_step.at(i).Comand == "disableroll"){
			Gnc_step.at(i).GNC_Comand=CM_DISABLE_ROLL;
		}else if(Gnc_step.at(i).Comand == "disablejettison"){
			Gnc_step.at(i).GNC_Comand=CM_DISABLE_JETTISON;
		}else if(Gnc_step.at(i).Comand == "jettison"){
			Gnc_step.at(i).GNC_Comand=CM_JETTISON;
		}else if(Gnc_step.at(i).Comand == "target"){
			Gnc_step.at(i).GNC_Comand=CM_TARGET;
		}else if(Gnc_step.at(i).Comand == "aoa"){
			Gnc_step.at(i).GNC_Comand=CM_AOA;
		}else if(Gnc_step.at(i).Comand == "attitude"){
			Gnc_step.at(i).GNC_Comand=CM_ATTITUDE;
		}else if(Gnc_step.at(i).Comand == "spin"){
			Gnc_step.at(i).GNC_Comand=CM_SPIN;
		}else if(Gnc_step.at(i).Comand == "inverse"){
			Gnc_step.at(i).GNC_Comand=CM_INVERSE;
		}else if(Gnc_step.at(i).Comand == "engineout"){
			Gnc_step.at(i).GNC_Comand=CM_ENGINEOUT;
		}else if(Gnc_step.at(i).Comand == "orbit"){
			Gnc_step.at(i).GNC_Comand=CM_ORBIT;
		}else if(Gnc_step.at(i).Comand == "defap"){
			Gnc_step.at(i).GNC_Comand=CM_DEFAP;
		}else if(Gnc_step.at(i).Comand == "glimit"){
			Gnc_step.at(i).GNC_Comand=CM_GLIMIT;
		}else if(Gnc_step.at(i).Comand == "destroy"){
			Gnc_step.at(i).GNC_Comand=CM_DESTROY;
		}else if(Gnc_step.at(i).Comand == "explode"){
			Gnc_step.at(i).GNC_Comand=CM_EXPLODE;
		}else if(Gnc_step.at(i).Comand == "noline"){
			Gnc_step.at(i).GNC_Comand=CM_NOLINE;
		}

        //CM_ROLL,CM_PITCH,CM_ENGINE,CM_FAIRING,CM_DISABLE,CM_PLAY,CM_JETTISON,CM_TARGET,CM_AOA,CM_ATTITUDE,CM_SPIN

		Gnc_step.at(0).time_fin=-10000;

		switch(Gnc_step.at(i).GNC_Comand){
		
            case CM_ENGINE:
                Gnc_step.at(i).val_init=Gnc_step.at(i).trval1;
                Gnc_step.at(i).val_fin=Gnc_step.at(i).trval2;
                if(Gnc_step.at(i).val_fin==0){
                    Gnc_step.at(i).val_fin=-1;
                }
                Gnc_step.at(i).time_init=Gnc_step.at(i).time;
                Gnc_step.at(i).duration=Gnc_step.at(i).trval3;
                if(Gnc_step.at(i).duration==0){
                    Gnc_step.at(i).duration=0.01;
                }
                Gnc_step.at(i).time_fin=Gnc_step.at(i).time_init+Gnc_step.at(i).duration;
                break;

            case CM_ROLL:
                Gnc_step.at(i).time_init=Gnc_step.at(i).time;
                Gnc_step.at(i).val_init=Gnc_step.at(i).trval2;
                Gnc_step.at(i).val_fin=Gnc_step.at(i).trval4;
                Gnc_step.at(i).duration=60;//Gnc_step.at(i).trval1;
                Gnc_step.at(i).time_fin=Gnc_step.at(i).time_init+Gnc_step.at(i).duration;
                
                //VinkaAzimuth=Gnc_step.at(i).trval3*RAD;
                //VinkaMode=Gnc_step.at(i).trval5;
            
                
                break;
            
            case CM_PITCH:
                Gnc_step.at(i).time_init=Gnc_step.at(i).time;
                Gnc_step.at(i).val_init=Gnc_step.at(i).trval1;
                Gnc_step.at(i).val_fin=Gnc_step.at(i).trval2;
                Gnc_step.at(i).duration=Gnc_step.at(i).trval3;
                Gnc_step.at(i).time_fin=Gnc_step.at(i).time_init+Gnc_step.at(i).duration;
                break;

                
            case CM_FAIRING:
                Gnc_step.at(i).time_init=Gnc_step.at(i).time;
                Gnc_step.at(i).val_init=Gnc_step.at(i).trval1*1000;
                Gnc_step.at(i).time_fin=Gnc_step.at(i).time_init+1000;
                break;
            case CM_LES:
                Gnc_step.at(i).time_init=Gnc_step.at(i).time;
                Gnc_step.at(i).val_init=Gnc_step.at(i).trval1*1000;
                Gnc_step.at(i).time_fin=Gnc_step.at(i).time_init+1000;
                break;
            case CM_DISABLE_JETTISON:
                Gnc_step.at(i).time_init=Gnc_step.at(i).time;
                Gnc_step.at(i).time_fin=Gnc_step.at(i).time_init+2;
                break;
            case CM_DISABLE_PITCH:
                Gnc_step.at(i).time_init=Gnc_step.at(i).time;
                Gnc_step.at(i).time_fin=Gnc_step.at(i).time_init+2;
                break;
            case CM_DISABLE_ROLL:
                Gnc_step.at(i).time_init=Gnc_step.at(i).time;
                Gnc_step.at(i).time_fin=Gnc_step.at(i).time_init+2;
                break;
            case CM_JETTISON:
                Gnc_step.at(i).time_init=Gnc_step.at(i).time;
                Gnc_step.at(i).time_fin=Gnc_step.at(i).time_init+2;
                break;
            case CM_AOA:
                Gnc_step.at(i).time_init=Gnc_step.at(i).time;
                Gnc_step.at(i).duration=Gnc_step.at(i).trval2;
                
                if(Gnc_step.at(i).duration<=0){
            
                    if(i<nsteps){
                        Gnc_step.at(i).time_fin=Gnc_step.at(i + 1).time;
                    }else{
                        Gnc_step.at(i).time_fin=Gnc_step.at(i).time_init+60;
                    }
                } else {
                    Gnc_step.at(i).time_fin=Gnc_step.at(i).time_init+Gnc_step.at(i).duration;
                }

                
                Gnc_step.at(i).val_init=Gnc_step.at(i).trval1*RAD;
                
                
                break;
            case CM_ATTITUDE:
                Gnc_step.at(i).time_init=Gnc_step.at(i).time;
                Gnc_step.at(i).duration=Gnc_step.at(i).trval4;
                if(Gnc_step.at(i).duration<=0){
                    
                    if(i<nsteps){
                        Gnc_step.at(i).time_fin=Gnc_step[i+1].time;
                    } else {
                        Gnc_step.at(i).time_fin=Gnc_step.at(i).time_init+60;
                    }
                } else {
                    Gnc_step.at(i).time_fin=Gnc_step.at(i).time_init+Gnc_step.at(i).duration;
                }
                break;
            case CM_SPIN:
                Gnc_step.at(i).time_init=Gnc_step.at(i).time;

                if(i<nsteps){
                        Gnc_step.at(i).time_fin=Gnc_step[i+1].time;
                    } else {
                        Gnc_step.at(i).time_fin=Gnc_step.at(i).time_init+60;
                    }
                Gnc_step.at(i).val_init=Gnc_step.at(i).trval1;
                break;
            case CM_TARGET:
                Gnc_step.at(i).time_init=Gnc_step.at(i).time;
                Gnc_step.at(i).time_fin=Gnc_step.at(i).time_init+5*60;
                Gnc_step.at(i).val_init=Gnc_step.at(i).trval1;
                break;
            case CM_INVERSE:
                Gnc_step.at(i).time_init=Gnc_step.at(i).time;
                Gnc_step.at(i).time_fin=Gnc_step.at(i).time_init+2;
                break;
            case CM_ENGINEOUT:
                Gnc_step.at(i).time_init=Gnc_step.at(i).time;
                Gnc_step.at(i).time_fin=Gnc_step.at(i).time_init+1;
                Gnc_step.at(i).val_init=Gnc_step.at(i).trval1;
                break;
            case CM_ORBIT:
                Gnc_step.at(i).time_init=Gnc_step.at(i).time;
                Gnc_step.at(i).time_fin=Gnc_step.at(i).time_init+10000;
                tgtapo=Gnc_step.at(i).trval2*1000;
                tgtperi=Gnc_step.at(i).trval1*1000;
                tgtinc=Gnc_step.at(i).trval3*RAD;
                VinkaMode=Gnc_step.at(i).trval4;
                if(VinkaMode==0){
                    VinkaMode=1;
                }
                GT_InitPitch=Gnc_step.at(i).trval5*RAD;
                tgtabside=Gnc_step.at(i).trval6*1000;
                wPeg=true;
                CalculateTargets();
                logbuff = std::format("{}: Orbit Call Found! Targets: Apogee:{:.1f} Perigee:{:.1f} Inclination:{:.1f} Mode:{:.1f} GT initial Pitch: {:.1f} Abside:{:.1f}", GetName(),tgtapo,tgtperi,tgtinc*DEG,VinkaMode,GT_InitPitch*DEG,tgtabside);
                oapiWriteLog(const_cast<char *>(logbuff.c_str()));
                break;
            case CM_DEFAP:
                Gnc_step.at(i).time_init=Gnc_step.at(i).time;
                Gnc_step.at(i).time_fin=Gnc_step.at(i).time_init+2;
                break;
            case CM_GLIMIT:
                Gnc_step.at(i).time_init=Gnc_step.at(i).time;
                Gnc_step.at(i).time_fin=Gnc_step.at(i).time_init+250;
                break;
            case CM_DESTROY:
                Gnc_step.at(i).time_init=Gnc_step.at(i).time;
                Gnc_step.at(i).time_fin=Gnc_step.at(i).time_init+2;
                break;
            case CM_EXPLODE:
                Gnc_step.at(i).time_init=Gnc_step.at(i).time;
                Gnc_step.at(i).time_fin=Gnc_step.at(i).time_init+2;
                break;
            case CM_NOLINE:
                
                break;

            }
        }

	    if(!wPeg){
	        VinkaUpdateRollTime();
	    }
	return;
}

double Multistage2026::VinkaFindEndTime(){
	double EndTime=0;
	
	for(int q=0;q<=nsteps;q++){
		if(Gnc_step.at(q).time_fin>EndTime){
			EndTime=Gnc_step.at(q).time_fin;
		}
	}
	
	return EndTime;
}

int Multistage2026::VinkaGetStep(double met){
	int n=0;
	for(int i=1;i<=nsteps;i++){
		if((met>=Gnc_step.at(i).time)&&(Gnc_step.at(i).GNC_Comand!=CM_NOLINE)){
			n+=1;
		}
	}
	return n;
}

void Multistage2026::VinkaCheckInitialMet(){
	
    if(Configuration==0){
		MET=Gnc_step[1].time;
	}

	if(nsteps==0){
		MET=-1;
	}
}

int Multistage2026::VinkaFindRoll(){
	int n=0;
	for(int i=1;i<=nsteps;i++){
        n+=1;
        if(Gnc_step.at(i).GNC_Comand==CM_ROLL){		
            break;
        }
    }

    return n;
}

int Multistage2026::VinkaFindFirstPitch(){
    int n=0;
    for(int i=1;i<=nsteps;i++){
        n+=1;
        if(Gnc_step.at(i).GNC_Comand==CM_PITCH){        
            break;
        }
    }
    return n;
}

void Multistage2026::VinkaConsumeStep(int step){

	if(Gnc_step.at(step).time_fin<=MET){
        Gnc_step.at(step).executed=true;
    }

	if(Gnc_step.at(step).executed==false){

		switch(Gnc_step.at(step).GNC_Comand){
		case CM_ENGINE:
			VinkaEngine(step);
		break;
		case CM_ROLL:
			VinkaRoll(step);
		break;
		case CM_PITCH:
			VinkaPitch(step);
		break;
		case CM_FAIRING:
			if((wFairing==1)&&(GetAltitude()>=Gnc_step.at(step).val_init)){
			Jettison(TFAIRING,0);
			Gnc_step.at(step).time_fin=MET;
			Gnc_step.at(step).executed=true;
			}
			
		break;
		case CM_LES:
			if((wLes==true)&&(GetAltitude()>=Gnc_step.at(step).val_init)){
			Jettison(TLES,0);
			Gnc_step.at(step).time_fin=MET;
			Gnc_step.at(step).executed=true;
			}
			
		break;
		case CM_DISABLE_JETTISON:
			AJdisabled=true;
			Gnc_step.at(step).executed=true;
		break;
		case CM_DISABLE_ROLL:
			rolldisabled=true;
			Gnc_step.at(step).executed=true;
		break;
		case CM_DISABLE_PITCH:
			pitchdisabled=true;
			Gnc_step.at(step).executed=true;
		break;
		case CM_JETTISON:
		char kstate[256];
		for(int i=0;i<256;i++) kstate[i]=0x00;
		kstate[OAPI_KEY_J]=0x80;
		SendBufferedKey(OAPI_KEY_J,true,kstate);
		Gnc_step.at(step).executed=true;
		break;
		case CM_AOA:
		double DesiredPitch;
		DesiredPitch=GetPitch()-VinkaMode*GetAOA()+VinkaMode*Gnc_step.at(step).val_init;
		TgtPitch=DesiredPitch;
		Attitude(DesiredPitch,(0.5*(1-VinkaMode)*PI),GetProperHeading());
		break;
		case CM_ATTITUDE:
		TgtPitch=Gnc_step.at(step).trval1*RAD;
		Attitude(Gnc_step.at(step).trval1*RAD,-(Gnc_step.at(step).trval3*RAD),Gnc_step.at(step).trval2*RAD);
		break;
		case CM_SPIN:
			spinning=true;
			VECTOR3 AngVel;
			GetAngularVel(AngVel);
		
			if((fabs(fabs(AngVel.z)-fabs(Gnc_step.at(step).val_init*RAD))>0.1)||(AngVel.z/Gnc_step.at(step).val_init<0)){
				TgtPitch=GetPitch();
				Attitude(GetPitch(),GetBank()-Gnc_step.at(step).val_init,GetProperHeading(),0,fabs((GetBank()*DEG+Gnc_step.at(step).val_init)),0);
			} else {
				spinning=false;
				Gnc_step.at(step).executed=true;
            }
		break;
			case CM_TARGET:
				double ApR,Rt,ApD;
				GetApDist(ApR);
				Rt=oapiGetSize(GetSurfaceRef());
				ApD=ApR-Rt;
				ApD=ApD/1000;
				if(ApD>=Gnc_step.at(step).val_init){
					Gnc_step.at(step).time_fin=MET;
					Gnc_step.at(step).executed=true;
				}
		break;
			case CM_INVERSE:
		VinkaMode*=-1;
		Gnc_step.at(step).executed=true;
		break;
			case CM_ENGINEOUT:
		if((Gnc_step.at(step).val_init>0)&&(Gnc_step.at(step).val_init<=(double)stage.at(currentStage).nEngines)){
			SetThrusterLevel(stage.at(currentStage).th_main_h[(int)Gnc_step.at(step).val_init-1],0);
		} else {
			SetThrusterLevel(stage.at(currentStage).th_main_h[stage.at(currentStage).nEngines-1],0);
		}
		Gnc_step.at(step).executed=true;
		break;
			case CM_ORBIT:
				if(Configuration==1){
					runningPeg=true;
		            if(GetAltitude()<altsteps[0]){
			            TgtPitch=90*RAD-Misc.VerticalAngle;
			            Attitude(90*RAD-Misc.VerticalAngle,GetBank(),GetHeading(),8,0,5);
			            //SetAngularVel(_V(0,0,0));

		            } else if((GetAltitude()>=altsteps[0])&&(GetAltitude()<altsteps[1])){
		                //Attitude(90*RAD,0.5*PI,VinkaAzimuth*(0.5*(1-VinkaMode)),8,20,5);

		                TgtPitch=89.9*RAD-Misc.VerticalAngle;
		                Attitude(89.9*RAD-Misc.VerticalAngle,(0.5*(1-VinkaMode)*PI),GetProperHeading(),8,20,5);
		            } else if((GetAltitude()>=altsteps[1])&&(GetAltitude()<altsteps[2])){
		                TgtPitch=GT_InitPitch;

		                double deltaAltitude=altsteps[2]-GetAltitude();
		                double deltaTime=sqrt((2*deltaAltitude)/(1.5*getabsacc()-g0));

		                double PitchRate=abs((GetPitch()-GT_InitPitch)/deltaTime);
		                Attitude(GT_InitPitch,(0.5*(1-VinkaMode)*PI),GetProperHeading(),PitchRate*DEG,20,PitchRate*DEG);

		            } else if((GetAltitude()>=altsteps[2])&&(GetAltitude()<altsteps[3])){
		                DesiredPitch=GetPitch()-VinkaMode*GetAOA()+(-0.7*RAD);
		                TgtPitch=DesiredPitch;
		                Attitude(DesiredPitch,(0.5*(1-VinkaMode)*PI),GetProperHeading(),8,5,8);
		            } else {
		                if(GetThrusterGroupLevel(THGROUP_MAIN)>0.1){
				            PEG();
				            CutoffCheck();
					        if(CutoffCheck()==true){
						        Gnc_step.at(step).time_fin=MET+1;
						        Gnc_step.at(step).executed=true;
					        }
				        }		
			        }
				}

		break;
			case CM_DEFAP:
				ActivateNavmode((int)Gnc_step.at(step).trval1);
				Gnc_step.at(step).executed=true;
				break;
			case CM_GLIMIT:
				if(getacc()>Gnc_step.at(step).trval1*g0){
					double lev=GetThrusterGroupLevel(THGROUP_MAIN);
					SetThrusterGroupLevel(THGROUP_MAIN,lev-0.001);
				}
				break;
			case CM_DESTROY:
				Gnc_step.at(step).executed=true;
				oapiDeleteVessel(GetHandle());
				break;
			case CM_EXPLODE:
				Gnc_step.at(step).executed=true;
				boom();
				break;
			case CM_NOLINE:
		break;

		}
	}
	return;
}

void Multistage2026::VinkaEngine(int step){
	
    double DesiredEngineLevel=(Gnc_step.at(step).val_init+(Gnc_step.at(step).val_fin-Gnc_step.at(step).val_init)*(MET-Gnc_step.at(step).time_init)/(Gnc_step.at(step).time_fin-Gnc_step.at(step).time_init))/100;
	
    SetThrusterGroupLevel(THGROUP_MAIN,DesiredEngineLevel);
}

void Multistage2026::VinkaRoll(int step){
	//double DesiredPitch=(Gnc_step.at(step).val_init+(Gnc_step.at(step).val_fin-Gnc_step.at(step).val_init)*(MET-Gnc_step.at(step).time_init)/((Gnc_step[VinkaFindFirstPitch()].time-1)-Gnc_step.at(step).time_init))*RAD;//88*RAD;
	double DesiredPitch=(Gnc_step.at(step).val_init+(Gnc_step.at(step).val_fin-Gnc_step.at(step).val_init)*(MET-Gnc_step.at(step).time_init)/((Gnc_step.at(step).time_fin)-Gnc_step.at(step).time_init))*RAD;//88*RAD;
	TgtPitch=DesiredPitch;
	double RollRate=180/((Gnc_step.at(step).time_fin-1)-Gnc_step.at(step).time_init);
	VinkaAzimuth=Gnc_step.at(step).trval3*RAD;
	VinkaMode=Gnc_step.at(step).trval5;
	Attitude(DesiredPitch,(0.5*(1-VinkaMode)*PI),VinkaAzimuth,2,RollRate,5);
}

void Multistage2026::VinkaPitch(int step){
	double DesiredPitch=(Gnc_step.at(step).val_init+(Gnc_step.at(step).val_fin-Gnc_step.at(step).val_init)*(MET-Gnc_step.at(step).time_init)/(Gnc_step.at(step).time_fin-Gnc_step.at(step).time_init))*RAD;//Gnc_step.at(step).val_fin*RAD;
	TgtPitch=DesiredPitch;
	if(spinning){
		Attitude(DesiredPitch,GetBank(),GetProperHeading());
	} else {
		Attitude(DesiredPitch,(0.5*(1-VinkaMode)*PI),GetProperHeading(),10,10,10);
    }
}

void Multistage2026::VinkaAutoPilot(){
	for(int q=1;q<=VinkaGetStep(MET);q++){
	    VinkaConsumeStep(q);
	}
}

void Multistage2026::VinkaDeleteStep(int q){

	if(Gnc_step.at(q).GNC_Comand==CM_ORBIT){
		wPeg=false;
	}
	if(Gnc_step.at(q).GNC_Comand==CM_DISABLE_ROLL){
		rolldisabled=false;
	}
	if(Gnc_step.at(q).GNC_Comand==CM_DISABLE_PITCH){
		pitchdisabled=false;
	}
	if(Gnc_step.at(q).GNC_Comand==CM_DISABLE_JETTISON){
		AJdisabled=false;
	}
	Gnc_step.at(q).GNC_Comand=CM_NOLINE;
	Gnc_step.at(q).Comand.clear();
	VinkaRearrangeSteps();
	nsteps=VinkaCountSteps();
	VinkaCheckInitialMet();
}

void Multistage2026::VinkaRearrangeSteps() {
	int index = 1;

	std::vector<GNC_STEP> trans(150);

	for (int q = 0; q < 150; q++) {
		trans.at(q).GNC_Comand = CM_NOLINE;
		trans.at(q).Comand.clear();
	}
	trans.at(0).time_fin = -10000;

	for (int i = 1; i < 150; i++) {
		if (Gnc_step.at(i).GNC_Comand != CM_NOLINE) {
			trans.at(index) = Gnc_step.at(i);
			index++;
		}
	}

	for (int q = 1; q < 150; q++) {
		Gnc_step.at(q) = trans.at(q);
	}

	for (int k = 1; k < index - 1; k++) {
		for (int j = k + 1; j < index; j++) {
			if (Gnc_step.at(k).time > Gnc_step.at(j).time) {
				GNC_STEP temp = Gnc_step.at(k);
				Gnc_step.at(k) = Gnc_step.at(j);
				Gnc_step.at(j) = temp;
			}
		}
	}
}

int Multistage2026::VinkaCountSteps(){
	int q=0;
	for(int i = 1; i < 150; i++){
		if(Gnc_step.at(i).GNC_Comand!=CM_NOLINE)
		//if(Gnc_step.at(i).GNC_Comand==CM_ROLL||CM_PITCH||CM_ENGINE||CM_FAIRING||CM_LES||CM_DISABLE_PITCH||CM_DISABLE_ROLL||CM_DISABLE_JETTISON||CM_PLAY||CM_JETTISON||CM_TARGET||CM_AOA||CM_ATTITUDE||CM_SPIN||CM_INVERSE||CM_ORBIT)//,CM_NOLINE
		{
			q += 1;
		}
	}

	return q;
}

GNC_STEP Multistage2026::VinkaComposeSpecificGNCSteps(GNC_STEP gnc){
	
	gnc.executed=false;

	std::transform(gnc.Comand.begin(), gnc.Comand.end(), gnc.Comand.begin(),
        [](unsigned char c){ return std::tolower(c); }
    );
		
	if(gnc.Comand == "engine"){
		gnc.GNC_Comand=CM_ENGINE;
	} else if(gnc.Comand == "roll"){
		gnc.GNC_Comand=CM_ROLL;
	} else if(gnc.Comand == "pitch"){
		gnc.GNC_Comand=CM_PITCH;
	} else if(gnc.Comand == "fairing"){
		gnc.GNC_Comand=CM_FAIRING;
	} else if(gnc.Comand == "les"){
		gnc.GNC_Comand=CM_LES;
	} else if(gnc.Comand == "disablepitch"){
		gnc.GNC_Comand=CM_DISABLE_PITCH;
	} else if(gnc.Comand == "disableroll"){
		gnc.GNC_Comand=CM_DISABLE_ROLL;
	} else if(gnc.Comand == "disablejettison"){
		gnc.GNC_Comand=CM_DISABLE_JETTISON;
	} else if(gnc.Comand == "jettison"){
		gnc.GNC_Comand=CM_JETTISON;
	} else if(gnc.Comand == "target"){
		gnc.GNC_Comand=CM_TARGET;
	} else if(gnc.Comand == "aoa"){
		gnc.GNC_Comand=CM_AOA;
	} else if(gnc.Comand == "attitude"){
		gnc.GNC_Comand=CM_ATTITUDE;
	} else if(gnc.Comand == "spin"){
		gnc.GNC_Comand=CM_SPIN;
	} else if(gnc.Comand == "inverse"){
		gnc.GNC_Comand=CM_INVERSE;
	} else if(gnc.Comand == "engineout"){
		gnc.GNC_Comand=CM_ENGINEOUT;
	} else if(gnc.Comand == "orbit"){
		gnc.GNC_Comand=CM_ORBIT;
	} else if(gnc.Comand == "defap"){
		gnc.GNC_Comand=CM_DEFAP;
	} else if(gnc.Comand == "glimit"){
		gnc.GNC_Comand=CM_GLIMIT;
	} else if(gnc.Comand == "destroy"){
		gnc.GNC_Comand=CM_DESTROY;
	} else if(gnc.Comand == "explode"){
		gnc.GNC_Comand=CM_EXPLODE;
	} else if(gnc.Comand == "noline"){
		gnc.GNC_Comand=CM_NOLINE;
	}

//CM_ROLL,CM_PITCH,CM_ENGINE,CM_FAIRING,CM_DISABLE,CM_PLAY,CM_JETTISON,CM_TARGET,CM_AOA,CM_ATTITUDE,CM_SPIN

		Gnc_step.at(0).time_fin=-10000;

		switch(gnc.GNC_Comand){
		
			case CM_ENGINE:
				gnc.val_init=gnc.trval1;
				gnc.val_fin=gnc.trval2;
				if(gnc.val_fin==0){
					gnc.val_fin=-1;
				}
				gnc.time_init=gnc.time;
				gnc.duration=gnc.trval3;
				if(gnc.duration==0){
					gnc.duration=0.01;
				}
				gnc.time_fin=gnc.time_init+gnc.duration;
				break;

			case CM_ROLL:
				gnc.time_init=gnc.time;
				gnc.val_init=gnc.trval2;
				gnc.val_fin=gnc.trval4;
				gnc.duration=60;//gnc.trval1;
				gnc.time_fin=gnc.time_init+gnc.duration;
				
				//VinkaAzimuth=gnc.trval3*RAD;
			//	VinkaMode=gnc.trval5;
			
				break;
			
			case CM_PITCH:
				gnc.time_init=gnc.time;
				gnc.val_init=gnc.trval1;
				gnc.val_fin=gnc.trval2;
				gnc.duration=gnc.trval3;
				gnc.time_fin=gnc.time_init+gnc.duration;
				break;

				
			case CM_FAIRING:
				gnc.time_init=gnc.time;
				gnc.val_init=gnc.trval1*1000;
				gnc.time_fin=gnc.time_init+1000;
				break;
			case CM_LES:
				gnc.time_init=gnc.time;
				gnc.val_init=gnc.trval1*1000;
				gnc.time_fin=gnc.time_init+1000;
				break;
			case CM_DISABLE_JETTISON:
				gnc.time_init=gnc.time;
				gnc.time_fin=gnc.time_init+2;
				break;
			case CM_DISABLE_PITCH:
				gnc.time_init=gnc.time;
				gnc.time_fin=gnc.time_init+2;
				break;
			case CM_DISABLE_ROLL:
				gnc.time_init=gnc.time;
				gnc.time_fin=gnc.time_init+2;
				break;
			case CM_JETTISON:
				gnc.time_init=gnc.time;
				gnc.time_fin=gnc.time_init+2;
				break;
			case CM_AOA:
				gnc.time_init=gnc.time;
				gnc.duration=gnc.trval2;
				
				if(gnc.duration<=0){
			
				//	if(i<nsteps){
				//		gnc.time_fin=Gnc_step[i+1].time;
				//	}else{
						gnc.time_fin=gnc.time_init+60;
					//}
				}else{
					gnc.time_fin=gnc.time_init+gnc.duration;
				}

				
				gnc.val_init=gnc.trval1*RAD;
				
				
				break;
			case CM_ATTITUDE:
				gnc.time_init=gnc.time;
				gnc.duration=gnc.trval4;
				if(gnc.duration<=0){
					
				//	if(i<nsteps){
					//	gnc.time_fin=Gnc_step[i+1].time;
					//}else{
						gnc.time_fin=gnc.time_init+60;
					//}
				} else {
					gnc.time_fin=gnc.time_init+gnc.duration;
				}
				break;
			case CM_SPIN:
				gnc.time_init=gnc.time;

				//if(i<nsteps){
						//gnc.time_fin=Gnc_step[i+1].time;
					//}else{
						gnc.time_fin=gnc.time_init+60;
					//}
				gnc.val_init=gnc.trval1;
				break;
			case CM_TARGET:
				gnc.time_init=gnc.time;
				gnc.time_fin=gnc.time_init+5*60;
				gnc.val_init=gnc.trval1;
				break;
			case CM_INVERSE:
				gnc.time_init=gnc.time;
				gnc.time_fin=gnc.time_init+2;
				break;
			case CM_ENGINEOUT:
				gnc.time_init=gnc.time;
				gnc.time_fin=gnc.time_init+1;
				gnc.val_init=gnc.trval1;
				break;
			case CM_ORBIT:
				gnc.time_init=gnc.time;
				gnc.time_fin=gnc.time_init+10000;
				tgtapo=gnc.trval2*1000;
				tgtperi=gnc.trval1*1000;
				tgtinc=gnc.trval3*RAD;
				VinkaMode=gnc.trval4;
				if(VinkaMode==0){
					VinkaMode=1;
				}
				GT_InitPitch=gnc.trval5*RAD;
				if(GT_InitPitch==0){
					GT_InitPitch=GT_IP_Calculated;
				}
				tgtabside=gnc.trval6*1000;
				wPeg=true;
				CalculateTargets();
				logbuff = std::format("{}: Orbit Call Found! Targets: Apogee:{:.1f} Perigee:{:.1f} Inclination:{:.1f} Mode:{:.1f} GT initial Pitch: {:.1f} Abside:{:.1f}", GetName(), tgtapo, tgtperi, tgtinc * DEG, VinkaMode, GT_InitPitch * DEG, tgtabside);
				oapiWriteLog(const_cast<char *>(logbuff.c_str()));
				break;
			case CM_DEFAP:
				gnc.time_init=gnc.time;
				gnc.time_fin=gnc.time_init+2;
				break;
			case CM_GLIMIT:
				gnc.time_init=gnc.time;
				gnc.time_fin=gnc.time_init+250;
				break;
			case CM_DESTROY:
				gnc.time_init=gnc.time;
				gnc.time_fin=gnc.time_init+2;
				break;
			case CM_EXPLODE:
				gnc.time_init=gnc.time;
				gnc.time_fin=gnc.time_init+2;
			case CM_NOLINE:
				
				break;

		}
	
	if(!wPeg){
		VinkaUpdateRollTime();
	}

	return gnc;
}

void Multistage2026::VinkaAddStep(std::string input){
	
	std::string line(input);
	
	for(int i = 0; i < 6; i++){
		Gnc_step.at(nsteps+1).wValue.at(i)=false;
	}
		
	std::size_t findEqual=line.find_first_of("=");

	if(findEqual!=line.npos){
		
		std::string mettime=line.substr(0, findEqual);
		
		Gnc_step.at(nsteps+1).time=atof(&mettime[0]);

		std::size_t findLineEnd=line.find_first_of(")");
		if(findLineEnd!=line.npos){
	
		std::size_t findOpenP=line.find_first_of("(");
		if(findOpenP!=line.npos){
			std::string comand=line.substr(findEqual+1,findOpenP-findEqual-1);
			Gnc_step.at(nsteps+1).Comand = comand;
			
			std::string values=line.substr(findOpenP+1,findLineEnd-findOpenP-1);
			Gnc_step.at(nsteps+1).wValue[0]=true;			
			std::size_t findFirstComma=values.find_first_of(",");
			if(findFirstComma!=values.npos){
				value1=values.substr(0,findFirstComma);
				Gnc_step.at(nsteps+1).wValue[1]=true;
			

				std::size_t findSecondComma=values.find_first_of(",",findFirstComma+1);
						
				if(findSecondComma!=values.npos){
				
					value2=values.substr(findFirstComma+1,findSecondComma-findFirstComma-1);
					Gnc_step.at(nsteps+1).wValue[2]=true;

					std::size_t findThirdComma=values.find_first_of(",",findSecondComma+1);
				
					if(findThirdComma!=values.npos){
				
						value3=values.substr(findSecondComma+1,findThirdComma-findSecondComma-1);
						Gnc_step.at(nsteps+1).wValue[3]=true;

						std::size_t findFourthComma=values.find_first_of(",",findThirdComma+1);
						
						if(findFourthComma!=values.npos){	
							value4=values.substr(findThirdComma+1,findFourthComma-findThirdComma-1);
							Gnc_step.at(nsteps+1).wValue[4]=true;
							std::size_t findFifthComma=values.find_first_of(",",findFourthComma+1);
								if(findFifthComma!=values.npos){
									value5=values.substr(findFourthComma+1,findFifthComma-findFourthComma-1);
									value6=values.substr(findFifthComma+1,std::string::npos);
									Gnc_step.at(nsteps+1).wValue[5]=true;
								} else {
									value5=values.substr(findFourthComma+1,std::string::npos);
								}
	} else { 
		value4=values.substr(findThirdComma+1,values.npos);
	}
		} else {
			value3=values.substr(findSecondComma+1,values.npos);
			}
			} else {
				 value2=values.substr(findFirstComma+1,values.npos);
			}
				} else {
					value1=values.substr(0,values.npos);
				}
			}
		}
	
		if(Gnc_step.at(nsteps+1).wValue[0]){	Gnc_step.at(nsteps+1).trval1=atof(&value1[0]);}else{Gnc_step.at(nsteps+1).trval1=0;}
		if(Gnc_step.at(nsteps+1).wValue[1]){	Gnc_step.at(nsteps+1).trval2=atof(&value2[0]);}else{Gnc_step.at(nsteps+1).trval2=0;}
		if(Gnc_step.at(nsteps+1).wValue[2]){	Gnc_step.at(nsteps+1).trval3=atof(&value3[0]);}else{Gnc_step.at(nsteps+1).trval3=0;}
		if(Gnc_step.at(nsteps+1).wValue[3]){	Gnc_step.at(nsteps+1).trval4=atof(&value4[0]);}else{Gnc_step.at(nsteps+1).trval4=0;}
		if(Gnc_step.at(nsteps+1).wValue[4]){	Gnc_step.at(nsteps+1).trval5=atof(&value5[0]);}else{Gnc_step.at(nsteps+1).trval5=0;}
		if(Gnc_step.at(nsteps+1).wValue[5]){	Gnc_step.at(nsteps+1).trval6=atof(&value6[0]);}else{Gnc_step.at(nsteps+1).trval6=0;}

	} else {
			Gnc_step.at(nsteps+1).Comand = "noline";	
			Gnc_step.at(nsteps+1).GNC_Comand=CM_NOLINE;
			Gnc_step.at(nsteps+1).time_fin=-10000;
			Gnc_step.at(nsteps+1).trval1=0;
			Gnc_step.at(nsteps+1).trval2=0;
			Gnc_step.at(nsteps+1).trval3=0;
			Gnc_step.at(nsteps+1).trval4=0;
			Gnc_step.at(nsteps+1).trval5=0;
			Gnc_step.at(nsteps+1).trval6=0;
		}

	
		
		std::transform(line.begin(), line.end(), line.begin(),
			[](unsigned char c){ return std::tolower(c); }
		);
			
		std::size_t foundDisable=line.find("disable");

		if(foundDisable!=std::string::npos){
			std::size_t foundDisPitch=line.find("pitch");
			std::size_t foundDisRoll=line.find("roll");
			std::size_t foundDisJett=line.find("jettison");

			if(foundDisPitch!=std::string::npos){
				Gnc_step.at(nsteps+1).Comand = "disablepitch";
			} else if(foundDisRoll!=std::string::npos){
				Gnc_step.at(nsteps+1).Comand = "disableroll";
			} else if(foundDisJett!=std::string::npos){
				Gnc_step.at(nsteps+1).Comand = "disablejettison";
			}

		}
		
		Gnc_step.at(nsteps+1)=VinkaComposeSpecificGNCSteps(Gnc_step.at(nsteps+1));
	
	VinkaRearrangeSteps();
	nsteps=VinkaCountSteps();	
	VinkaCheckInitialMet();
}

void Multistage2026::WriteGNCFile(){

	FILEHANDLE GncFile;
	std::string filenmbuff;
	filenmbuff = std::format("Multistage2015\\Guidance\\{}_{:.2f}_GNC.txt", GetName(), oapiGetSysMJD());
	
	GncFile = oapiOpenFile(filenmbuff.c_str(), FILE_OUT,CONFIG);

	std::string buffer;

	buffer = "Multistage 2015 Automatically Generated Guidance File";
	
	oapiWriteLine(GncFile, const_cast<char *>(buffer.c_str()));

	buffer = std::format("Vehicle: {}", GetName());

	oapiWriteLine(GncFile, const_cast<char *>(buffer.c_str()));

	for(int i=1;i<=nsteps;i++){
		buffer = std::format("{:.3f} ={}({:.2f},{:.2f},{:.2f},{:.2f},{:.2f},{:.2f})",Gnc_step.at(i).time_init,Gnc_step.at(i).Comand,Gnc_step.at(i).trval1,Gnc_step.at(i).trval2,Gnc_step.at(i).trval3,Gnc_step.at(i).trval4,Gnc_step.at(i).trval5,Gnc_step.at(i).trval6);
		oapiWriteLine(GncFile, const_cast<char *>(buffer.c_str()));
	}

	oapiCloseFile(GncFile,FILE_OUT);

}
