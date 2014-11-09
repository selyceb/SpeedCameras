#include<iostream>
#include<string>
#include<vector>
#include<fstream>
using namespace std;

float speedLimit;
int numCams;
int currentCam;
ifstream inFile;
bool metric;

struct Vehicle{
private:
	vector<float> times;
	string id;
public:
	Vehicle(string vehicleId, float initTime){
		id = vehicleId;
		addTime(initTime);
	}
	void addTime(float time){
		times.push_back(time);
	}
	string getId(){
		return id;
	}
};//end Vehicle

//Vector of vectors containing times for each vehicle and names
vector<Vehicle> vehicles;
vector<Vehicle>::iterator tempV;

void openFile(string fileName){
	inFile.open(fileName);
}

void closeFile(){
	inFile.close();
}

string nextLine(){
	string current;
	getline(inFile, current);
	return current;
}

void getLimit(){
	string current = nextLine();
	string limitStr = "";
	for(int i = 0; i < current.length(); i++){
		if(isdigit(current[i]) || current[i] == '.'){
			limitStr += current[i];
		}
		if(current[i] == ' ' && limitStr != ""){
			i = current.length();
		}
	}//end for loop
	speedLimit = stof(limitStr);
	if(current[current.length()-4] == 'm'){
		metric = false;
	}
	else
		metric = true;
}//end getLimit()

bool checkSpeedLine(string line){
	string temp = "";
	for(int i = 0; i < 5; i++){
		temp += toupper(line[i]);
	}
	if (temp == "SPEED"){
		return true;
	}
	else
		return false;
}//end checkSpeedLine()

// float extractSpeed(string rawString){
	// string temp = "";
	// int i = 15;
	// while(isdigit(rawString[i] || rawString[i] == '.')){
		// temp += rawString[i];
		// i++;
	// }
	// cout << temp;
	// return stof(temp);
// }

float extractTime(string rawString){
	int i;
	int j;
	string temp = "";
	string tempNum = "";
	float out = 0;
	for(i = 0; i < rawString.length(); i++){
		if(rawString[i] == 'a' && rawString[i+1] == 't'){
			for(j = i+3; j<(rawString.length()-1); j++){
				temp += rawString[j];
			}
			i = rawString.length();
		}//end if
	}//end for
	tempNum += temp[0];
	tempNum += temp[1];
	out += (stof(tempNum)*3600);
	tempNum = "";
	tempNum += temp[3];
	tempNum += temp[4];
	out += (stof(tempNum)*60);
	tempNum = "";
	tempNum += temp[6];
	tempNum += temp[7];
	out += stof(tempNum);
	return out;
}//end extractTime()

//ONLY USE WHEN ID HAS ALREADY BEEN ADDED!!!
void addTime(string vehicId, float time){
	for(tempV = vehicles.begin(); tempV != vehicles.end(); tempV++){
		if(tempV->getId() == vehicId){
			tempV->addTime(time);
		}
	}//end for loop
}//addTime()

bool checkStartLine(string line){
	string temp = "";
	for(int i = 0; i < 5; i++){
		temp += toupper(line[i]);
	}
	if (temp == "START"){
		return true;
	}
	else
		return false;
}//end checkStartLine()

bool checkVehicleLine(string line){
	string temp = "";
	for(int i = 0; i < 7; i++){
		temp += toupper(line[i]);
	}
	if (temp == "VEHICLE"){
		return true;
	}
	else
		return false;
}//end checkVehicleLine()

string extractId(string rawString){
	int i;
	string temp = "";
	string out = "";
	for(i = 8; rawString[i]!='p'; i++){
		temp += rawString[i];
	}
	for(i = 0; i < temp.length(); i++){
		if(temp[i] != ' '){
			out += temp[i];
		}
	}
	return out;
}//end extractId()

bool canAddVehic(string vehicId){
	if(!vehicles.empty()){
		for(tempV = vehicles.begin(); tempV != vehicles.end(); tempV++)
		{
			if(tempV->getId() == vehicId)
			{
				return false;
			}
		}//end for
		return true;
	}//end if
	else
		return true;
}//end canAddVehic();

void addVehicle(string vehicId, int time){
	Vehicle temp(vehicId, time);
	vehicles.push_back(temp);
}//end addVehicle()

void readLines(){
	string current;
	while(!inFile.eof()){
		current = nextLine();
		if(checkSpeedLine(current)){
			//speedLimit = extractSpeed(current);
		}
		else if(checkStartLine(current)){
			numCams += 1;
		}
		else if(checkVehicleLine(current)){
			if(canAddVehic(extractId(current))){
				addVehicle(extractId(current), extractTime(current));
			}
			else{
				addTime(extractId(current), extractTime(current));
			}
		}
	}//end while
}//end readLines()

int main(){
	openFile("test.txt");
	if(inFile.is_open()){
		getLimit();
		cout << speedLimit;
		readLines();
	}
	closeFile();
	return 0;
}//end main