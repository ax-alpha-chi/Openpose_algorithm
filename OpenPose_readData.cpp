#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <iomanip>
#include <conio.h>
#include <bits/stdc++.h> 

using namespace std;

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
   				for(; data[i] != ','; i++)
					sy.push_back(data[i]);
				for(; i > data.size(); i++)
					sc.push_back(data[i]);
			x = atof(sx.c_str());
			y = atof(sy.c_str());
			c = atof(sc.c_str());
		}
};

class FrontView{
	public:
		int numberOfSquats;
		const static int timeOfVideo = 10;						//seconds
		const static double frameRate = 30;						//fps
		const static int numberOfData = 40;						//json files read
		const static int timeInterval = 0.25;					//time interval of reading points
		bool error;												//1 if something is wrong
		Point feet_l[numberOfData],feet_r[numberOfData];		//point11,14
		Point hip_l[numberOfData],hip_r[numberOfData];			//point9,12
		Point knee_l[numberOfData],knee_r[numberOfData];		//point10,13
		Point shoulder_l[numberOfData],shoulder_r[numberOfData];//point2,5
		void read_data(){
		    string s;
		    for(int k = 0; k < numberOfData; k++){
		    	int frame = 0.25 * k * 30;
				stringstream ss;
				ss << frame;
				string framestr = ss.str();
				string new_framestr = string(12 - framestr.length(), '0') + framestr;
				string fileName = "output/squat1_" + new_framestr + "_keypoints.json";
		    	ifstream openfile(fileName.c_str());
		    	if(openfile.is_open()){
		            getline(openfile, s);
		            for (int i = 3; i < s.size(); i++) {
   						if((s[i]=='"' && (s[i-1]=='2') && (s[i-2]=='"'))){
   							for(int j = i + 3; s[j] != ']'; j++)
   								shoulder_l[k].data.push_back(s[j]);
   							shoulder_l[k].distribute_data();
						}
   						if((s[i]=='"' && (s[i-1]=='5') && (s[i-2]=='"'))){
   							for(int j = i + 3; s[j] != ']'; j++)
   								shoulder_r[k].data.push_back(s[j]);
   							shoulder_r[k].distribute_data();
						}
   						if((s[i]=='"' && (s[i-1]=='9') && (s[i-2]=='"'))){
   							for(int j = i + 3; s[j] != ']'; j++)
   								hip_l[k].data.push_back(s[j]);
   							hip_l[k].distribute_data();
						}
						if((s[i]=='"' && (s[i-1]=='1') && (s[i-2]=='1') && (s[i-3]=='"'))){
   							for(int j = i + 3; s[j] != ']'; j++)
   								feet_l[k].data.push_back(s[j]);
   							feet_l[k].distribute_data();
						}
						if((s[i]=='"' && (s[i-1]=='4') && (s[i-2]=='1') && (s[i-3]=='"'))){
   							for(int j = i + 3; s[j] != ']'; j++)
   								feet_r[k].data.push_back(s[j]);
   							feet_r[k].distribute_data();
						}
						if((s[i]=='"' && (s[i-1]=='2') && (s[i-2]=='1') && (s[i-3]=='"'))){
   							for(int j = i + 3; s[j] != ']'; j++)
   								hip_r[k].data.push_back(s[j]);
   							hip_r[k].distribute_data();
						}
						if((s[i]=='"' && (s[i-1]=='3') && (s[i-2]=='1') && (s[i-3]=='"'))){
   							for(int j = i + 3; s[j] != ']'; j++)
   								knee_r[k].data.push_back(s[j]);
   							knee_r[k].distribute_data();
						}
						if((s[i]=='"' && (s[i-1]=='0') && (s[i-2]=='1') && (s[i-3]=='"'))){
   							for(int j = i + 3; s[j] != ']'; j++)
   								knee_l[k].data.push_back(s[j]);
   							knee_l[k].distribute_data();
						}	
					}
		    	}		
			}
		}
		void countNumberOfSquats(){
			int count = 0;
			for(int i = 0; i < numberOfData; i++){
				if( (knee_l[i].y - hip_l[i].y) < 15 ){
					count++;
					i = i + 15;		//checks again after 3.75 seconds
				} 
			}
			numberOfSquats = count; 
		}
};

class SideView{
};
int main(int argc, char** argv) {
	FrontView squat1;
	squat1.read_data();
	squat1.countNumberOfSquats();
	cout << squat1.numberOfSquats;
	return 0;
}
