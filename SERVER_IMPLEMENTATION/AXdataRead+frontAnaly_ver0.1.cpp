/*
	AX SQUAT ANALYZER
	FOR FRONT VIEW(RULE:CONSTRAINTS)
		1.FEET SHOULD STAY STATIONARY: LEFT FOOT POINT AND RIGHT FOOT POINT IS LIMITED TO 5PIXELS IN X AXIS 
		2.HIPS SHOULD BE HORIZONTAL: LEFT HIP POINT AND RIGHT HIP POINT IS LIMITED TO 5PIXELS IN X AND Y AXES
		3.KNEES SHOULD STAY STATIONARY: LEFT KNEE POINT AND RIGHT KNEE POINT DIFFERENCE IS LIMITED TO 50PIXELS IN X AND Y AXES
		4.SHOULDERS SHOULD BE HORIZONTAL: LEFT SHOULDER POINT AND RIGHT SHOULDER POINT DIFFERENCE IS LIMITED TO 5PIXELS IN Y AXIS
	FOR SIDE VIEW(RULE:CONSTRAINTS)
		1.KNEES AND FEETFRONT SHOULD BE HORIZONTAL AT SQUAT:
		2.ANGLE BETWEEN THIGH AND CALF SHOULD BE LESS THAN 90DEGREES:
		3.SHOULDER AND NECK SHOULD BE ON ONE POINT THROUGHOUT:
	FOR PLANK(RULE:CONSTRAINTS)
		1.NECK TO HIP AND HIP TO HEEL SHOULD BE STRAIGHT:
		2.ELBOW TO SHOULDER AND ELBOW TO HAND SHOULD BE PERPENDICULAR:
		3.NOSE TO EAR SHOULD BE VERTICLE: 
*/

#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <iomanip>
#include <conio.h>
#include <bits/stdc++.h> 
#include <cmath>

#define PI 3.14159265

using namespace std;
/*
Point class which has the x,y,c data required for calculations, function of this class distribute() distributes the string data to x,y,c accordingly.
*/
class Point{
	public:
		string data;
		double x;
		double y;
		double c;
		void distribute_data(){
			string sx;
			string sy;
			string sc;
			int i = 0;
				for(; data[i] != ','; i++)
					sx.push_back(data[i]);
				i++;	
   				for(; data[i] != ','; i++)
					sy.push_back(data[i]);
				i++;
				for(; i > data.size(); i++)
					sc.push_back(data[i]);
			x = atof(sx.c_str());
			y = atof(sy.c_str());
			c = atof(sc.c_str());
		}
};
/*
Squat class is the main class of the program which reads data and analyzes the data to find errors. Also contains all the required variables such as required instances of Point class.
*/
class SquatAnalyze{
	public:
		int numberOfSquats;
		int numberOfStands;
		int numberOfFull;
		int squatDataNumber[10];								/*datanumber at which the squat is performed first squat at squatDatabNumber[0], second squat at squatDatanumber[1], and so on*/
		int standDataNumber[10];								/*datanumber at the person is standing straight, same as squatDataNumeber*/
		string errorsstr;										/*errorstring which will save errors when they are found*/
		int timeOfVideo = 10;									/*time of the video analyzed in seconds*/
		const double frameRate = 30;							/*framerate of the video for analyzing exact frames*/
		const static int numberOfData = 40;						/*number of json files to be read*/
		const double timeInterval = 0.25;						/*time interval of reading points (every 0.25 seconds video will be analyzed)*/
		const int delay = 10;									/*delay for rechecking after counting a squat*/
		bool error = 0;											/*1 if something is wrong*/
		int errorCount = 0;										/*for keeping track of number of errors occured*/
		Point feet_l[numberOfData],feet_r[numberOfData];		/*point11,14*/
		Point hip_l[numberOfData],hip_r[numberOfData];			/*point9,12*/
		Point knee_l[numberOfData],knee_r[numberOfData];		/*point10,13*/
		Point shoulder_l[numberOfData],shoulder_r[numberOfData];/*point2,5*/
		Point toe_l[numberOfData];								/*point22*/
		Point neck[numberOfData];								/*point1*/
		/*
		This Function reads the data in the output folder and saves them in appropriate variables (feet_l[],feet_r[],...etc)
		*/
		void read_data(){
		    string s;
		    for(int k = 0; k < numberOfData; k++){
		    	int frame = timeInterval * k * frameRate;
				stringstream ss;
				ss << frame;
				string framestr = ss.str();
				string new_framestr = string(12 - framestr.length(), '0') + framestr;
				string fileName = "output/squat1_" + new_framestr + "_keypoints.json"; /*specify json file name here*/
		    	ifstream openfile(fileName.c_str());
		    	if(openfile.is_open()){
		            getline(openfile, s);
		            for (int i = 3; i < s.size(); i++) {
		            	/*reads and saves point1 data*/
		            	if((s[i]=='"' && (s[i-1]=='1') && (s[i-2]=='"'))){
   							for(int j = i + 3; s[j] != ']'; j++)
   								neck[k].data.push_back(s[j]);
   							neck[k].distribute_data();
						}
						/*reads and saves point22 data*/
						if((s[i]=='"' && (s[i-1]=='2') && (s[i-2]=='2') && (s[i-3]=='"'))){
   							for(int j = i + 3; s[j] != ']'; j++)
   								toe_l[k].data.push_back(s[j]);
   							toe_l[k].distribute_data();
						}
						/*reads and saves point2 data*/
   						if((s[i]=='"' && (s[i-1]=='2') && (s[i-2]=='"'))){
   							for(int j = i + 3; s[j] != ']'; j++)
   								shoulder_l[k].data.push_back(s[j]);
   							shoulder_l[k].distribute_data();
						}
						/*reads and saves point5 data*/
   						if((s[i]=='"' && (s[i-1]=='5') && (s[i-2]=='"'))){
   							for(int j = i + 3; s[j] != ']'; j++)
   								shoulder_r[k].data.push_back(s[j]);
   							shoulder_r[k].distribute_data();
						}
						/*reads and saves point9 data*/
   						if((s[i]=='"' && (s[i-1]=='9') && (s[i-2]=='"'))){
   							for(int j = i + 3; s[j] != ']'; j++)
   								hip_l[k].data.push_back(s[j]);
   							hip_l[k].distribute_data();
						}
						/*reads and saves point11 data*/
						if((s[i]=='"' && (s[i-1]=='1') && (s[i-2]=='1') && (s[i-3]=='"'))){
   							for(int j = i + 3; s[j] != ']'; j++)
   								feet_l[k].data.push_back(s[j]);
   							feet_l[k].distribute_data();
						}
						/*reads and saves point14 data*/
						if((s[i]=='"' && (s[i-1]=='4') && (s[i-2]=='1') && (s[i-3]=='"'))){
   							for(int j = i + 3; s[j] != ']'; j++)
   								feet_r[k].data.push_back(s[j]);
   							feet_r[k].distribute_data();
						}
						/*reads and saves point12 data*/
						if((s[i]=='"' && (s[i-1]=='2') && (s[i-2]=='1') && (s[i-3]=='"'))){
   							for(int j = i + 3; s[j] != ']'; j++)
   								hip_r[k].data.push_back(s[j]);
   							hip_r[k].distribute_data();
						}
						/*reads and saves point13 data*/
						if((s[i]=='"' && (s[i-1]=='3') && (s[i-2]=='1') && (s[i-3]=='"'))){
   							for(int j = i + 3; s[j] != ']'; j++)
   								knee_r[k].data.push_back(s[j]);
   							knee_r[k].distribute_data();
						}
						/*reads and saves point10 data*/
						if((s[i]=='"' && (s[i-1]=='0') && (s[i-2]=='1') && (s[i-3]=='"'))){
   							for(int j = i + 3; s[j] != ']'; j++)
   								knee_l[k].data.push_back(s[j]);
   							knee_l[k].distribute_data();
						}	
					}
		    	}		
			}
		}
		/*
		This Function counts the number of squats performed, it counts as a squat when hip and knee points come close (within 25mm)
		*/
		void countNumberOfSquats(){
			int count = 0;
			for(int i = 1; i < numberOfData; i++){
				if( abs( hip_l[i].y - knee_l[i].y) < 25 ){
					squatDataNumber[count] = i;
					count++;
					i = i + delay;		/*checks for the squat again after 2.5 seconds (0.25*10)*/
				} 
			}
			numberOfSquats = count; 
		}
		/*
		This Function counts the number of time person is standing straight
		*/
		void countNumberOfStands(){
			int count = 0;
			for(int i = 1; i < numberOfData; i++){
				if( abs( hip_l[i].y - knee_l[i].y) > 140 ){
					standDataNumber[count] = i;
					count++;
					i = i + delay;		/*checks for the squat again after 2.5 seconds (0.25*10)*/
				} 
			}
			numberOfStands = count; 
		}
		/*
		This Function counts the full squats
		*/
		void countFullsquats(){
			if(numberOfStands < numberOfSquats)
				numberOfFull = numberOfStands;
			else
				numberOfFull = numberOfSquats;
			
		}
		/*
		This Function analyzes the data obtained and finds errors according to the constraints of perfect squat.(front view)
		*/
		void frontError(){
			string temp;
			int dataAtSquat;						/*dataAtSquat is the datanumber at which the squat is performed*/
			int dataAtStand;						/*dataAtStand is the datanumber at which the person is standing straight*/
			/*
			For analyzing errors at squat and stand positions
			*/	
			for(int i = 0; i < numberOfFull; i++){

				dataAtSquat = squatDataNumber[i];
				dataAtStand = standDataNumber[i];	
				/*
				feet width difference between stand and squat should be less than 5 as the constraint, else error is saved.
				*/	
				if( abs(abs(feet_r[dataAtStand].x - feet_l[dataAtStand].x) - abs(feet_r[dataAtSquat].x - feet_l[dataAtSquat].x)) > 5 ){
					error = 1;
					errorCount++;
					errorsstr.append("Mistake " + to_string(errorCount) + " at time " + to_string(timeInterval*dataAtSquat) + "s\n");
					temp = "feets are moved too much in squat " + to_string(i + 1) + ", please keep your feet stationary (same position)\n\n"; 
					errorsstr.append(temp);
				}
				/*
				hip_l height and hip_r height difference at stand should be less than 5 as the constraint, else error is saved.
				*/
				if( abs(hip_l[dataAtStand].y - hip_r[dataAtStand].y) > 5 ){
					error = 1;
					errorCount++;
					errorsstr.append("Mistake " + to_string(errorCount) + " at time " + to_string(timeInterval*dataAtSquat) + "s\n");
					temp = "hips are not horizontal at the start of squat(stand position) in squat " + to_string(i + 1) + ", please keep your hips straight\n\n"; 
					errorsstr.append(temp);
				}
				/*
				hip_l height and hip_r height difference at squat should be less than 5 as the constraint, else error is saved.
				*/ 
				if( abs(hip_l[dataAtSquat].y - hip_r[dataAtSquat].y) > 5 ){
					error = 1;
					errorCount++;
					errorsstr.append("Mistake " + to_string(errorCount) + " at time " + to_string(timeInterval*dataAtSquat) + "s\n");
					temp = "hips are not horizontal at the squat(squat position) in squat " + to_string(i + 1) + ", please keep your hips straight\n\n"; 
					errorsstr.append(temp);
				}
				/*
				knee width difference between stand and squat should be less than 5 as the constraint, else error is saved.
				*/
				if( abs(abs(knee_l[dataAtStand].x - knee_r[dataAtStand].x) - abs(knee_l[dataAtSquat].x - knee_r[dataAtSquat].x)) > 60 ){
					error = 1;
					errorCount++;
					errorsstr.append("Mistake " + to_string(errorCount) + " at time " + to_string(timeInterval*dataAtSquat) + "s\n");
					temp = "knees move too much in squat " + to_string(i + 1) + ", please keep your knees around the same position\n\n"; 
					errorsstr.append(temp);
				}
				/*
				shoulder_l height and shoulder_r height difference at stand should be less than 5 as the constraint, else error is saved.
				*/
				if( abs(shoulder_l[dataAtStand].y - shoulder_r[dataAtStand].y) > 10 ){
					error = 1;
					errorCount++;
					errorsstr.append("Mistake " + to_string(errorCount) + " at time " + to_string(timeInterval*dataAtStand) + "s\n");
					temp = "shoulders are not straight at the start of squat(stand position) in " + to_string(i + 1) + " squat, please keep your shoulders straight\n\n"; 
					errorsstr.append(temp);
				}
				/*
				shouler_l height and shoulder_r height difference at squat should be less than 5 as the constraint, else error is saved.
				*/
				if( abs(shoulder_l[dataAtSquat].y - shoulder_r[dataAtSquat].y) > 10 ){
					error = 1;
					errorCount++;
					errorsstr.append("Mistake " + to_string(errorCount) + " at time " + to_string(timeInterval*dataAtSquat) + "s\n");
					temp = "shoulders are not straight at the squat(squat position) in " + to_string(i + 1) + " squat, please keep your shoulders straight\n\n"; 
					errorsstr.append(temp);
				}
			}
		}
		/*
		This Function analyzes the data obtained and finds errors according to the constraints of perfect squat.(side view)
		*/
		void sideError(){
			string temp;
			int dataAtSquat;						/*dataAtSquat is the datanumber at which the squat is performed*/
			int dataAtStand;						/*dataAtStand is the datanumber at which the person is standing straight*/
			/*
			For analyzing errors at squat and stand positions
			*/	
			for(int i = 0; i < numberOfFull; i++){

				dataAtSquat = squatDataNumber[i];
				dataAtStand = standDataNumber[i];	
				/*
				knee_l and toe_l x difference at squat should be less than 5 as the constraint, else error is saved.
				*/ 
				if( abs(knee_l[dataAtSquat].x - toe_l[dataAtSquat].x) > 5 ){
					error = 1;
					errorCount++;
					errorsstr.append("Mistake " + to_string(errorCount) + " at time " + to_string(timeInterval*dataAtSquat) + "s\n");
					temp = "toe and knee are not in line at squat(squat position) in " + to_string(i + 1) + " squat, try pushing your knees forward\n\n"; 
					errorsstr.append(temp);
				}
				/*
				angle difference of thigh and calf at squat should be less than 90degrees as the constraint, else error is saved.
				*/ 
				float thighAngle = atan(abs(hip_l[dataAtSquat].y - knee_l[dataAtSquat].y) / abs(hip_l[dataAtSquat].x - knee_l[dataAtSquat].x)) * 180 / PI;
				float calfAngle = atan(abs(feet_l[dataAtSquat].y - knee_l[dataAtSquat].y) / abs(feet_l[dataAtSquat].x - knee_l[dataAtSquat].x)) * 180 / PI;
				if( (thighAngle + calfAngle) > 90){
					error = 1;
					errorCount++;
					errorsstr.append("Mistake " + to_string(errorCount) + " at time " + to_string(timeInterval*dataAtSquat) + "s\n");
					temp = "squat intensity is low at squat(squat position) in  " + to_string(i + 1) + " squat, try to go lower\n\n"; 
					errorsstr.append(temp);
				}
				/*
				shoulder and neck width difference between stand and squat should be less than 5 as the constraint, else error is saved.
				*/ 
				if( abs(abs(neck[dataAtStand].x - shoulder_r[dataAtStand].x) - abs(neck[dataAtSquat].x - shoulder_r[dataAtSquat].x)) > 5 ){
					error = 1;
					errorCount++;
					errorsstr.append("Mistake " + to_string(errorCount) + " at time " + to_string(timeInterval*dataAtSquat) + "s\n");
					temp = "shoulders move too much in squat " + to_string(i + 1) + ", please keep your shoulders steady\n\n"; 
					errorsstr.append(temp);
				}
				else if( abs(abs(neck[dataAtStand].y - shoulder_r[dataAtStand].y) - abs(neck[dataAtSquat].y - shoulder_r[dataAtSquat].y)) > 5 ){
					error = 1;
					errorCount++;
					errorsstr.append("Mistake " + to_string(errorCount) + " at time " + to_string(timeInterval*dataAtSquat) + "s\n");
					temp = "shoulders move too much in squat " + to_string(i + 1) + ", please keep your shoulders steady\n\n"; 
					errorsstr.append(temp);
				}
			}
		}
		/*
		This Function displays the results of the analysis on the screen.
		*/
		void output(){
			cout << "=========================================" << endl;
			cout << "NUMBER OF FULL SQUATS PERFORMED "<< numberOfFull << endl;
			if(error == 0)
				cout << "No mistakes, well done" << endl;
			else{
				cout << "NUMBER OF MISTAKES "<< errorCount << endl;
				cout << "=========================================" << endl;
				cout << errorsstr;
			}		
		}
};

int main(int argc, char** argv) {
	/*
	code to run the program.
	*/
	SquatAnalyze squat1;
	squat1.read_data();
	squat1.countNumberOfSquats();
	squat1.countNumberOfStands();
	squat1.countFullsquats();
	squat1.frontError();
	squat1.output();
	return 0;
}
