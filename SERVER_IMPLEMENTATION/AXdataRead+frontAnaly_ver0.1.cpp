#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <iomanip>
#include <conio.h>
#include <bits/stdc++.h> 
#include <cmath>
using namespace std;
/*
Point class which has the x,y,c data required for calculations, functions of this class distribute() distributes the string data to x,y,c accordingly.
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
		int squatDataNumber[10];								/*datanumber at which the squat is performed first squat at squatDatabNumber[0], second squat at squatDatanumber[1], and so on*/
		string errorsstr;										/*errorstring which will save errors when they are found*/
		int timeOfVideo = 10;									/*time of the video analyzed in seconds*/
		double frameRate = 30;									/*framerate of the video for analyzing exact frames*/
		const static int numberOfData = 40;						/*number of json files to be read*/
		const static int timeInterval = 0.25;					/*time interval of reading points (every 0.25 seconds video will be analyzed)*/
		bool error = 0;											/*1 if something is wrong*/
		int errorCount = 0;										/*for keeping track of number of errors occured*/
		Point feet_l[numberOfData],feet_r[numberOfData];		/*point11,14*/
		Point hip_l[numberOfData],hip_r[numberOfData];			/*point9,12*/
		Point knee_l[numberOfData],knee_r[numberOfData];		/*point10,13*/
		Point shoulder_l[numberOfData],shoulder_r[numberOfData];/*point2,5*/
		Point toe_l[numberOfData];								/*point22*/
		Point neck[numberOfData];								/*point1*/
		/*
		This Function reads the data in the output folder and saves them in appropriate variables (feet_l[],feet_r,...etc)
		*/
		void read_data(){
		    string s;
		    for(int k = 0; k < numberOfData; k++){
		    	int frame = 0.25 * k * 30;
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
					i = i + 10;		/*checks for the squat again after 2.5 seconds (0.25*10)*/
				} 
			}
			numberOfSquats = count; 
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
			for(int i = 0; i < numberOfSquats; i++){

				dataAtSquat = squatDataNumber[i];
				if(i != 0)
					dataAtStand = (squatDataNumber[i] - squatDataNumber[i-1])/2;
				else
					dataAtStand = squatDataNumber[i]/2;	
				/*
				feet width difference between stand and squat should be less than 5 as the constraint else error is saved.
				*/	
				if( abs((feet_r[dataAtStand].x - feet_l[dataAtStand].x) - (feet_r[dataAtSquat].x - feet_l[dataAtSquat].x)) > 5 ){
					error = 1;
					errorCount++;
					errorsstr.append("Mistake " + to_string(errorCount) + "\n");
					temp = "feets are moved too much in squat " + to_string(i + 1) + ", please keep your feet stationary\n"; 
					errorsstr.append(temp);
				}
				/*
				hip_l height and hip_r height difference at stand should be less than 5 as the constraint else error is saved.
				*/
				if( abs(hip_l[dataAtStand].y - hip_r[dataAtStand].y) > 5 ){
					error = 1;
					errorCount++;
					errorsstr.append("Mistake " + to_string(errorCount) + "\n");
					temp = "hips are not straight at the start of squat(stand position) in squat " + to_string(i + 1) + ", please keep your hips straight\n"; 
					errorsstr.append(temp);
				}
				/*
				hip_l height and hip_r height difference at squat should be less than 5 as the constraint else error is saved.
				*/ 
				if( abs(hip_l[dataAtSquat].y - hip_r[dataAtSquat].y) > 5 ){
					error = 1;
					errorCount++;
					errorsstr.append("Mistake " + to_string(errorCount) + "\n");
					temp = "hips are not straight at the squat(squat position) in squat " + to_string(i + 1) + ", please keep your hips straight\n"; 
					errorsstr.append(temp);
				}
				/*
				knee width difference between stand and squat should be less than 5 as the constraint else error is saved.
				*/
				if( abs(abs(knee_l[dataAtStand].x - knee_r[dataAtStand].x) - abs(knee_l[dataAtSquat].x - knee_r[dataAtSquat].x)) > 5 ){
					error = 1;
					errorCount++;
					errorsstr.append("Mistake " + to_string(errorCount) + "\n");
					temp = "knees move too much in squat " + to_string(i + 1) + ", please keep your knees around the same position\n"; 
					errorsstr.append(temp);
				}
				/*
				shoulder_l height and shoulder_r height difference at stand should be less than 10 as the constraint else error is saved.
				*/
				if( abs(shoulder_l[dataAtStand].y - shoulder_r[dataAtStand].y) > 10 ){
					error = 1;
					errorCount++;
					errorsstr.append("Mistake " + to_string(errorCount) + "\n");
					temp = "shoulders are not straight at the start of squat(stand position) in " + to_string(i + 1) + " squat, please keep your shoulders straight\n"; 
					errorsstr.append(temp);
				}
				/*
				shouler_l height and shoulder_r height difference at squat should be less than 5 as the constraint else error is saved.
				*/
				if( abs(shoulder_l[dataAtSquat].y - shoulder_r[dataAtSquat].y) > 10 ){
					error = 1;
					errorCount++;
					errorsstr.append("Mistake " + to_string(errorCount) + "\n");
					temp = "shoulders are not straight at the squat(squat position) in " + to_string(i + 1) + " squat, please keep your shoulders straight\n"; 
					errorsstr.append(temp);
				}
			}
		}
		/*
		This Function analyzes the data obtained and finds errors according to the constraints of perfect squat.(front view)
		*/
		void sideError(){
			string temp;
			int data;
			/*
			For analyzing errors at squat and stand positions
			*/	
			for(int i = 0; i < numberOfSquats; i++){

				dataAtSquat = squatDataNumber[i];
				if(i != 0)
					dataAtStand = (squatDataNumber[i] - squatDataNumber[i-1])/2;
				else
					dataAtStand = squatDataNumber[i]/2;	
				/*
				feet width difference between stand and squat should be less than 5 as the constraint else error is saved.
				*/	
				if( abs((feet_r[dataAtStand].x - feet_l[dataAtStand].x) - (feet_r[dataAtSquat].x - feet_l[dataAtSquat].x)) > 5 ){
					error = 1;
					errorCount++;
					errorsstr.append("Mistake " + to_string(errorCount) + "\n");
					temp = "feets are moved too much in squat " + to_string(i + 1) + ", please keep your feet stationary\n"; 
					errorsstr.append(temp);
				}
				/*
				hip_l height and hip_r height difference at stand should be less than 5 as the constraint else error is saved.
				*/
				if( abs(hip_l[dataAtStand].y - hip_r[dataAtStand].y) > 5 ){
					error = 1;
					errorCount++;
					errorsstr.append("Mistake " + to_string(errorCount) + "\n");
					temp = "hips are not straight at the start of squat(stand position) in squat " + to_string(i + 1) + ", please keep your hips straight\n"; 
					errorsstr.append(temp);
				}
				/*
				hip_l height and hip_r height difference at squat should be less than 5 as the constraint else error is saved.
				*/ 
				if( abs(hip_l[dataAtSquat].y - hip_r[dataAtSquat].y) > 5 ){
					error = 1;
					errorCount++;
					errorsstr.append("Mistake " + to_string(errorCount) + "\n");
					temp = "hips are not straight at the squat(squat position) in squat " + to_string(i + 1) + ", please keep your hips straight\n"; 
					errorsstr.append(temp);
				}
				/*
				knee width difference between stand and squat should be less than 5 as the constraint else error is saved.
				*/
				if( abs(abs(knee_l[dataAtStand].x - knee_r[dataAtStand].x) - abs(knee_l[dataAtSquat].x - knee_r[dataAtSquat].x)) > 5 ){
					error = 1;
					errorCount++;
					errorsstr.append("Mistake " + to_string(errorCount) + "\n");
					temp = "knees move too much in squat " + to_string(i + 1) + ", please keep your knees around the same position\n"; 
					errorsstr.append(temp);
				}
				/*
				shoulder_l height and shoulder_r height difference at stand should be less than 10 as the constraint else error is saved.
				*/
				if( abs(shoulder_l[dataAtStand].y - shoulder_r[dataAtStand].y) > 10 ){
					error = 1;
					errorCount++;
					errorsstr.append("Mistake " + to_string(errorCount) + "\n");
					temp = "shoulders are not straight at the start of squat(stand position) in " + to_string(i + 1) + " squat, please keep your shoulders straight\n"; 
					errorsstr.append(temp);
				}
				/*
				shouler_l height and shoulder_r height difference at squat should be less than 5 as the constraint else error is saved.
				*/
				if( abs(shoulder_l[dataAtSquat].y - shoulder_r[dataAtSquat].y) > 10 ){
					error = 1;
					errorCount++;
					errorsstr.append("Mistake " + to_string(errorCount) + "\n");
					temp = "shoulders are not straight at the squat(squat position) in " + to_string(i + 1) + " squat, please keep your shoulders straight\n"; 
					errorsstr.append(temp);
				}
			}
		}
		/*
		This Function displays the results of the analysis on the screen.
		*/
		void output(){
			cout << "=========================================" << endl;
			cout << "NUMBER OF SQUATS PERFORMED "<< numberOfSquats << endl;
			if(error == 0)
				cout << "No mistakes, well done" << endl;
			else{
				cout << "NUMBER OF MISTAKES "<< error << endl;
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
	squat1.frontError();
	squat1.output();
	return 0;
}
