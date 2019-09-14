
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
    ftp_file << "send result.txt\n";
    ftp_file << "delete tobeprocessed.mp4\n";
    ftp_file << "quit\n";
    ftp_file.close();
}

void batch_file_receive(){
	ofstream batch_file;
	batch_file.open("server.bat", ios::out);
	batch_file << "del /f tobeprocessed.mp4\n";
	batch_file << "ftp -s:ftp_receive.txt\n";
	batch_file.close();
}

void batch_file_process(){
	ofstream batch_file;
	batch_file.open("server.bat", ios::out);	
	string openpose_cmd = "bin\\OpenPoseDemo.exe --video tobeprocessed.mp4 --write_video processed.avi --part_candidates --write_json output/\n";
	batch_file << openpose_cmd;
	batch_file << "ffmpeg -i processed.avi processed.gif\n";
	batch_file << "AXdataRead+frontAnaly_ver0.1.exe";
	write_ftp_send();
	batch_file << "ftp -s:ftp_send.txt\n";
	batch_file << "del -f tobeprocessed.mp4\n";
	batch_file << "del -f processed.avi\n";
	batch_file << "del -f processed.gif\n";
	batch_file.close();
}

bool is_file_exist(const char *fileName){
    ifstream infile(fileName);
    return infile.good();
}

int main(int argc, char *argv[]){
	
	cout << "Starting Up The Server...\n";
    write_ftp_receive();
    batch_file_receive();
	while(!is_file_exist("tobeprocessed.mp4"))
    	system("server.bat");
    write_ftp_send();
    batch_file_process();
    system("server.bat");
}
