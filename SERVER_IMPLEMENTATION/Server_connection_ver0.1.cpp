
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

void write_ftp_receive(){
	ofstream ftp_file;
    ftp_file.open("ftp_receive.txt", ios::out);
    ftp_file << "open 3.19.139.163\n";
    ftp_file << "ilona\n";
    ftp_file << "Nar340536!\n";
    ftp_file << "get tobeprocessed.mp4\n";
    ftp_file << "get result.txt\n";
    ftp_file << "quit\n";
    ftp_file.close();
}

void write_ftp_send(){
	ofstream ftp_file;
    ftp_file.open("ftp_send.txt", ios::out);
    ftp_file << "open 3.19.139.163\n";
    ftp_file << "ilona\n";
    ftp_file << "Nar340536!\n";
    
    ftp_file << "send processed.gif\n";
    ftp_file << "send processed2.gif\n";
    
    ftp_file << "send result.txt\n";
    
    ftp_file << "send stand0.jpg\n";
    ftp_file << "send stand1.jpg\n";
    ftp_file << "send stand2.jpg\n";
    ftp_file << "send stand3.jpg\n";
	
    ftp_file << "send squat0.jpg\n";
    ftp_file << "send squat1.jpg\n";
    ftp_file << "send squat2.jpg\n";
    ftp_file << "send squat3.jpg\n";
	
	ftp_file << "send error1.jpg\n";
    ftp_file << "send error2.jpg\n";
	ftp_file << "send error3.jpg\n";
    ftp_file << "send error4.jpg\n";
 	ftp_file << "send error5.jpg\n";
    ftp_file << "send error6.jpg\n";
	ftp_file << "send error7.jpg\n";
    ftp_file << "send error8.jpg\n";
	
    ftp_file << "delete tobeprocessed.mp4\n";
    ftp_file << "quit\n";
    ftp_file.close();
}

void batch_file_receive(){
	ofstream batch_file;
	batch_file.open("server.bat", ios::out);
	batch_file << "del /f tobeprocessed.mp4\n";
	//batch_file << "del /f result.txt\n";
	batch_file << "ftp -s:ftp_receive.txt\n";
	batch_file.close();
}

void batch_file_process(){
	ofstream batch_file;
	batch_file.open("server.bat", ios::out);	
	string openpose_cmd = "bin\\OpenPoseDemo.exe --video tobeprocessed.mp4 --net_resolution \"512x288\" --write_video processed.avi --part_candidates --write_json output/\n";
	batch_file << openpose_cmd;
	batch_file << "bin\\OpenPoseDemo.exe --video tobeprocessed.mp4 --net_resolution \"512x288\" --disable_blending --write_video processed2.avi\n";
	batch_file << "ffmpeg -i processed.avi frame%%04d.jpg -hide_banner\n";
	batch_file << "ffmpeg -y -i processed.avi -pix_fmt rgb24 -r 20 -s 512x288 processed.gif\n";
	batch_file << "ffmpeg -y -i processed2.avi -pix_fmt rgb24 -r 20 -s 512x288 processed2.gif\n";
	batch_file << "AXdataRead+frontAnaly_ver0.1.exe\n";
	write_ftp_send();
	batch_file << "ftp -s:ftp_send.txt\n";
	batch_file << "del -f tobeprocessed.mp4\n";
	batch_file << "del -f processed.avi\n";
	batch_file << "del -f processed2.avi\n";
	batch_file << "del -f processed.gif\n";
	batch_file << "del -f processed2.gif\n";
	batch_file << "del error1.jpg\n";
    batch_file << "del error2.jpg\n";
	batch_file << "del error3.jpg\n";
    batch_file << "del error4.jpg\n";
	batch_file << "del error5.jpg\n";
    batch_file << "del error6.jpg\n";
	batch_file << "del error7.jpg\n";
    batch_file << "del error8.jpg\n";
	batch_file.close();
}

bool is_file_exist(const char *fileName){
    ifstream infile(fileName);
    return infile.good();
}

int main(int argc, char *argv[]){
	
	cout << "Starting Up The Server...\n";
		while(1){
			write_ftp_receive();
		    batch_file_receive();
			while(!is_file_exist("tobeprocessed.mp4"))
		    	system("server.bat");
		    write_ftp_send();
		    batch_file_process();
		    system("server.bat");
		}
    
}
