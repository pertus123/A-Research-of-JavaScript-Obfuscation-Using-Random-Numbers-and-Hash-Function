#include <stdio.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <iostream>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include "sha256.h"
#include <sstream>

using namespace std;

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

//// 임시 키워드 
string tempKeywordPath = "/root/Project/tempkeyword.txt";
void Temp_Preprocessing();
int tempMaxWord;
string tempWordArr[10000];
bool TempExTable(string compareC);
////

//https://stackoverflow.com/questions/12975341/to-string-is-not-a-member-of-std-says-g-mingw tostring 사용sddff한글english
using namespace patch;

void JsRead(string readFileName, string addr);// 읽고, 처리
bool ExTable(string compareC);	// 예외처리 검사
void FileCheck(char file_pt[], char file_cd[]);							// 파일 검색 file cd 지울수도
void HtmlRead(string readFileName, string addr);		// html에서 script검사 
void HJWrite(string readFileName);
void KeywordRead();
int Qutoes(int pi, string line);
void ChSaltCreate();
char fd_final[PATH_MAX]; // 저장 경로
string a, k;
string wordArr[1000];
int maxWord;
void HJInit(string readFileName);
string salt;
int saltNumber;
//
int Goto_index(int x);  // gotoindex(무작위 숫자가 들어가는 배열)에 무작위로 숫자를 넣어주는 메소드
string  Discrimination(string compare);  // 제어문 판독하는 메소드
void Read();  //읽어들인 파일을 일반문장과 제어문을 따로 분리하여 배열에 저장하기
void TransGoto();  //배열에 제어문인걸 확인 후 goto제어문으로 바꿔준다.
string TransGotoFor(string contant);  // for 제어문을 goto 문으로 변경 해주슨 메소드
string TransGotoIf(string contant);  // if 제어문을 goto 문으로 변경 해주슨 메소드
string TransGotoWhile(string contant);  // while 제어문을 goto 문으로 변경 해주슨 메소드
string TransGotoDowhile(string contant);  // dowhile 제어문을 goto 문으로 변경 해주슨 메소드
string replace_all(const string& message, const string& pattern, const string& replace);  // 제어문 continue, break 변경 메소드
string CPST;  // 파일의 내용을 저장하는 string
string contralkinds[100];//읽어온 파일을 줄단위로 평뮨, 제어문을 판멸하기위한 배열
string allcontent[100];//읽어온 파일을 줄단위로 저장하는 배열
int allcontentindex = 0; // allcontent의스택 포인트
int gotoindex[51] = { 0, }; // 무작위 숫자가 들어가는 배열
int TransGotoForIndex = 0; // for문을 발견한 누적숫자
int TransGotoIfIndex = 0; // if문을 발견한 누적숫자
int TransGotoWhileIndex = 0;// while문을 발견한 누적숫자
int TransGotoDowhileIndex = 0;// dowhile문을 발견한 누적숫자
//			
void Temp_Preprocessing(string readFileName, string addr) {
	tempMaxWord = 0;
	string pullNamePath = addr + readFileName;
	ifstream inFile(pullNamePath.c_str());
	string line, stringStorage;
	int startNum;
	ofstream os;
	os.open(tempKeywordPath.c_str());
	while (getline(inFile, line)) {
		startNum = line.find("eval");
		if (startNum != -1) {
			cout << startNum << endl;
			for (int i = startNum + 4; i < line.length(); i++) {
				if (line[i] == '"') {
					for (int j = i + 1; j < line.length(); j++) {
						if (line[j] == '"') {
							i = j;
							goto EXIT;
						}
						stringStorage += line[j];
					}
				}
			}
		EXIT: {
			tempWordArr[tempMaxWord] = stringStorage;
			tempMaxWord++;
			}
		}
	}

	inFile.close();
	os.close();
}
bool TempExTable(string compareC) { // 다 다르면 false 맞으면 true;
	int togle = 0;
	for (int i = 0; i < tempMaxWord; i++) {
		if (tempWordArr[i].length() <= compareC.length()) {
			togle = 0;
			for (int t = 0; t < tempWordArr[i].length(); t++) {
				if (tempWordArr[i][t] != compareC[t]) {

					togle = 1;
					break;
				}
			}
			if (togle == 0) {
				return true;
			}
		}
	}
	return false;
}

// 예약어 개수 
void KeywordRead() {
	maxWord = 0;
	string KeywordArry = "./JsKeywords.txt";
	ifstream inFile(KeywordArry.c_str()); // 파일 읽기
	string line;

	while (getline(inFile, line)) {
		wordArr[maxWord] = line.substr(0, line.length()-1);
		maxWord++;
	}
}

int Qutoes(int pi, string line) {
	k = k + line[pi];
	for (int i = pi + 1; i < line.length(); i++) {
		k = k + line[i];
		if (line[i] == line[pi]) return i + 1;
	}
	return pi;
}

void ChSaltCreate(){
	srand((unsigned int)time(NULL));
	for (int j = 0; j < saltNumber; j++) {
		int b = rand() % 52;
		if (b > 25) salt = salt + (char)('a' + b - 26);
		else salt = salt + (char)('A' + b);
	}
}

bool ExTable(string compareC) {
	for (int i = 0; i < maxWord; i++){
		if (!wordArr[i].compare(compareC)){ 
			return true;
		}
	}
	return false;
}

int isFileOrDir(int bnum) {
	if (bnum == 4) return 0; // 디렉터리
	return 1; // 파일
}

void FileSearch(char file_path[], char file_copyPath[]) { // 읽고 쓰고 
	int check, tk = 0;
	string a;
	struct dirent *item;
	DIR *dp;
	char file_path2[PATH_MAX], file_copyPath2[PATH_MAX];

	strcat(file_path, "/");
	strcpy(file_path2, file_path);

	strcat(file_copyPath, "/");
	dp = opendir(file_path); // 읽을 곳 복사본

	if (dp != NULL){
		for (;;){
			item = readdir(dp);
			if (item == NULL) break;

			char file_pt[PATH_MAX]; // read
			char file_cd[PATH_MAX]; // write

			strcpy(file_pt, file_path2);
			strcat(file_pt, item->d_name);
			strcpy(file_cd, file_copyPath);
			check = isFileOrDir(item->d_type); // 0 디렉터리 1 파일

			if (check == 0 && item->d_name[0] != '.') { // 디렉터리
				printf("%s\n", item->d_name);
				strcat(file_cd, item->d_name);
				mkdir(file_cd,0755);
				FileSearch( file_pt , file_cd);    //하위 디렉토리 검색 재귀함수
			}
			else if (check == 1 && item->d_name[0] != '.' && item->d_reclen != 0 && tk == 0) {			 //tk는 dir 
				string file_name(file_pt);
				file_name = file_name.substr(file_name.length() - 3, file_name.length());

				strcpy(fd_final,file_cd);
				if (file_name == ".js") {
					Temp_Preprocessing(item->d_name, file_path2);
					HJInit(item->d_name);
					JsRead(item->d_name, file_path2);
				}

				else if (file_name == "tml") {
					Temp_Preprocessing(item->d_name, file_path2);
					HJInit(item->d_name);
					HtmlRead(item->d_name, file_path2);
				}

				else {
					strcat(file_cd, item->d_name);
					printf("%s %d\n", file_cd, strlen(file_pt));
					ifstream  src(file_pt, ios::binary);
					ofstream  dst(file_cd, ios::binary);
					dst << src.rdbuf();
					src.close();
					dst.close();
				}
			}
		}
		closedir(dp);
	}

}

void HtmlRead(string readFileName, string addr) {

	string line, compareC, line2;
	a = "";	// 난독화 값을 같는 전역 변수. 초기화.
	addr = addr + readFileName; // 주소 + 파일명
	ifstream inFile(addr.c_str()); // 파일 읽기
	int tk = 0;
	int cnt = 0;
	int cntyh = 0;
	CPST = "";

		while (!inFile.eof()) {
		getline(inFile, line2);

		CPST += line2;
		cntyh++;

		if (inFile.eof()) {
			cout << "wpw" << endl;
		}

		if (cntyh == 5 || inFile.eof()) {

			Read();
			TransGoto();
			for (int i = 0; i <= allcontentindex; i++) {
				line += allcontent[i];
			}
			for (int i = 0; i < line.length(); i++) {
				k = "";
				compareC = "";

				if (line[i] == '<') {
					for (int j = i + 1; j < line.length(); j++) {
						if (line[j] == '>') {
							compareC += '>';
							break;
						}
						compareC += line[j];
					}

					if (compareC.substr(0, 7) == "script " || compareC.substr(0, 7) == "script>") {		//script 부분 검색
						i += compareC.length();
						k += '<' + compareC;
						tk = 1;
						a = a + k;
						continue;
					}
					else if (compareC.substr(0, 7) == "/script") {
						i += compareC.length();
						k += '<' + compareC;
						tk = 0;
						a = a + k;
						continue;
					}
				}

				if (tk == 1) {
					if (line[i] == '\'' || line[i] == '"') {
						i = Qutoes(i, line);

						a += k; //
					}

					if ((65 <= line[i] && line[i] <= 90) || (97 <= line[i] && line[i] <= 122)) {	// 만약 알파벳이라면,
						for (int j = i; j < line.length(); j++) {				//알파벳 위치부터 , 알파벳 끝나는 부분까지 compareC에 단어만듬
							if (!((65 <= line[j] && line[j] <= 90) || (97 <= line[j] && line[j] <= 122) || (48 <= line[j] && line[j] <= 57) || (line[j] == '$') || (line[j] == '_'))) {  //추가사항 : 변수에 사용가능한 특수문자, 숫자 
								break;
							}
							compareC += line[j];
						}
					}
					//
					else {
						a = a + line[i];
						continue;
					}

					if (ExTable(compareC)) { // 만들어진 단어, 예약어 검사
						i += compareC.length() - 1; // 단어 길이만큼 +
						a += compareC;
						continue;
					}

/////////////////
					if (compareC == "setTimeout") { // 여기부터 
						a += compareC+'(';
						i += compareC.length() - 1; // 단어 길이만큼 +
						string timeoutString;
						int togle = 0;
						for (i = i + 1; i < line.length(); i++) {
							if (line[i] == '"') {
								a += '"';
								togle++;
								for (i = i + 1; i < line.length(); i++) {
									if (line[i] != '(') {
										timeoutString += line[i];
									}
									else {
										togle = 1;
										break;
									}
								}
								if (togle == 1) break;
							}			
						}
					cout << timeoutString;
						//a+= 
						a += salt[0] + sha256(timeoutString + salt).substr(0, 40) +"()\"";
						i = i + 2;
						cout << "1";
						continue;
					}
					//
					if (ExTable(compareC)) { // 만들어진 단어, 예약어 검사

												//


						//

						i += compareC.length() - 1; // 단어 길이만큼 +
						//cout << compareC << endl;
						a += compareC;
						//a = a + k;
						//
						if (compareC == "document") {
							//printf("1111111111111111111111111111111");
							for (i= i+1; i < line.length(); i++) {
								a += line[i];
								if ((line[i] == ';') || (line[i] == 32) || (line[i] == '(') || (line[i] == '=')) { // 32 스페이스(공백)
									break;
								}
							}

						}

						continue;
					}
//////////////////

					//ChTable(line[i]);
					k += salt[0] + sha256(compareC + salt).substr(0, 40);
					i += compareC.length() - 1;
					a = a + k;
				}
				else {
					a = a + k + line[i];
				}
			}
			// 추가
			cnt++;
			if ((cnt % 5) == 0) {
				//cout << a << "\n";
				cnt = 0;
				HJWrite(readFileName);
				a = "";
			}

			cntyh = 0;
			CPST = "";
			fill(allcontent, allcontent + 100, "");
			fill(contralkinds, contralkinds + 100, "");
			allcontentindex = 0;
			line = "";
		}
	}

	HJWrite(readFileName); // 저장
	inFile.close();
}
// this step
void JsRead(string readFileName, string addr) { // 파일 읽기. 파일명
	string line, compareC, line2;
	a = "";	// 난독화 값을 같는 전역 변수. 초기화.
	addr = addr + readFileName; // 주소 + 파일명
	ifstream inFile(addr.c_str()); // 파일 읽기
	int tk = 0;
	int cnt = 0;
	int cntyh = 0;
	CPST = "";

	while (!inFile.eof()) {
		getline(inFile, line2);

		CPST += line2;
		cntyh++;

		if (inFile.eof()) {
			cout << "wpw" << endl;
		}

		if (cntyh == 5 || inFile.eof()) { // 여기 확인

			Read();
			TransGoto();
			for (int i = 0; i <= allcontentindex; i++) {
				line += allcontent[i];
			}
			k = "";

			for (int i = 0; i < line.length(); i++) { // 전체에서

				if (line[i] == '/' && line[i + 1] == '*') { // 주석 /*
					int tk = 0;
					for (int j = i + 2; j <= line.length(); j++) {
						if (line[j] == '*' && line[j + 1] == '/') {
							tk = 1;
							break;
						}
					}
					if (tk == 0) {
						continue;
					}
				}
				if (line[i] == '\'' || line[i] == '"') i = Qutoes(i, line); // 인용구 검사.
				if (line[i] == '/' && line[i + 1] == '/') break; // 주석 제거 합시다. 마지막 먹히는 것까지 //
				if (line[i] == '	') continue; // 이것은 텝, 텝 제거 명령
				compareC = ""; // 각 문자열 단어 만드는 것 띄어쓰기
				// 검사하고, a,b,c 문자열들 검사
				if ((65 <= line[i] && line[i] <= 90) || (97 <= line[i] && line[i] <= 122)) { // 만약 알파벳이라면,
					for (int j = i; j < line.length(); j++) { //알파벳 위치부터 , 알파벳 끝나는 부분까지 compareC에 단어만듬
						if (!((65 <= line[j] && line[j] <= 90) || (97 <= line[j] && line[j] <= 122))) {
							break;
						}
						compareC += line[j];
					}
					if (ExTable(compareC)) { // 만들어진 단어, 예약어 검사
						i += compareC.length() - 1; // 단어 길이만큼 +
						k += compareC;
						continue; // 그리고 밑에 것 무시 맨위로
					}
					k += salt[0] + sha256(compareC + salt).substr(0, 40);
					i += compareC.length() - 1;
					continue;
				}
				k = k + line[i];
			} // line.length
			//추가


			a = a + k + '\n';
			cnt++;
			if ((cnt%100) == 0) {
				//cout << "test1" << "\n";
				cnt = 0;
				HJWrite(readFileName);
				a = "";
			}
			cntyh = 0;
			CPST = "";
			fill(allcontent, allcontent + 100, "");
			fill(contralkinds, contralkinds + 100, "");
			allcontentindex = 0;
			line = "";
		}
	}
	HJWrite(readFileName);
	inFile.close();
}

void HJWrite(string readFileName) { // 완료 후, 저장
	ofstream os;
	string addr(fd_final);
	addr = addr + readFileName;
	os.open(addr.c_str(), ios_base::app);
	os << a;
	os.close();
}
void HJInit(string readFileName) {
	ofstream os;
	string addr(fd_final);
	addr = addr + readFileName;
	os.open(addr.c_str());
	os.close();
}

int main()
{
	char file_path[PATH_MAX] = "./target/read";
	char file_copyDir[PATH_MAX] = "/testhtml2/httpd"; // 경로 수정 부분

	srand((unsigned int)time(NULL));
	saltNumber = (rand() % 5)+5;

	ChSaltCreate();
	KeywordRead();
	FileSearch(file_path, file_copyDir);

	return 0;
}

string replace_all(const string& message, const string& pattern, const string& replace) {
	string result = message;
	string::size_type pos = 0;
	string::size_type offset = 0;
	while ((pos = result.find(pattern, offset)) != std::string::npos) {
		result.replace(result.begin() + pos, result.begin() + pos + pattern.size(), replace);
		offset = pos + replace.size();
	}
	return result;
}

//index를 무작위로 섞기
int Goto_index(int x) {
	bool check[50] = { false, };
	if (gotoindex[x] == 0) {
		for (int i = 0; i < 50;) {
			int num = rand() % (49 + 1);
			if (check[num] == false) {  //num -1 한거는 num은 1 ~ 100 까지기 때문에
				check[num] = true;
				gotoindex[i] = num;
				i++; //while 문 처럼 쓰려고 여기서 ++
			}
		}
	}
	return gotoindex[x];
}

// 파일 읽어들이기
void first() {
	ifstream read("hello.txt");
	string a, b;
	if (read.good()) {
		while (!read.eof()) {
			getline(read, a);
			int length = a.length();
			for (int i = 0; i < length; i++) {
				b += a[i];
			}
		}
	}
	CPST = b;
}

//제어문 판독하기
string  Discrimination(string compare) {
	string check[] = { "for(" , "if(", "while(", "do{" };
	int intcheck = sizeof(check) / sizeof(string);
	string ans;
	for (int i = 0; i < intcheck; i++) {
		if (check[i] == compare) {
			ans = check[i];
			break;
		}
	}
	return ans;
}



//읽어들인 파일을 일반문장과 제어문을 따로 분리하여 배열에 저장하기
void Read() {
	string gototemp;
	string Disc, cont; // 제어문을 판별하기 위한 문자열
	int length = CPST.length();
	int writeindex = 0; //제어문 외 글씨 그대로 출력하기위한 정수(그 위치를 알기위한)
	int repeat = 0; // 제어문 중 중괄호 중첩을 스택포인터로 사용하기 위한 정수 
	
	for (int i = 0; i < length; i++) {
		Disc = "";
		////////////////////////////////////////////////
		if (length - i > 6) {
			for (int k = 0; k < 8; k++) {  // 현재 for문만 체크함
				Disc += CPST[i + k];  // 현재 글자에서 6글자 안에 제어문이 있는지 검사
				cont = Discrimination(Disc);
				if (cont != "") {
					break;
				}
			}
		}
		////////////////////////////////////////////////
		if (cont != "") {
			//cout << "test" << endl;
			if (i - writeindex > 4) {
				for (int k = writeindex; k < i; k++) {
					allcontent[allcontentindex] += CPST[k];
				}
				allcontentindex++;
				contralkinds[allcontentindex] = cont;
			}

			if (cont != "do{" && cont != "if(") {

				for (int j = i; j < length; j++) {
					gototemp += CPST[j];

					if (j == length - 1 && repeat >= 1) {

						allcontent[allcontentindex] = gototemp;
						contralkinds[allcontentindex] = "";
						allcontentindex++;
						i = j;
						writeindex = j + 1;
						gototemp = "";
						break;

					}

					if (CPST[j] == 123)  repeat++;
					if (CPST[j] == 125) {
						repeat--;

						if (repeat == 0) {

							allcontent[allcontentindex] = gototemp;
							contralkinds[allcontentindex] = cont;
							allcontentindex++;
							i = j;
							writeindex = j + 1;
							gototemp = "";
							break;
						}
					}
				}
			}
			else if (cont == "do{") {

				for (int j = i; j < length; j++) {

					gototemp += CPST[j];

					if (j == length - 1 && repeat >= 1) {

						allcontent[allcontentindex] = gototemp;
						contralkinds[allcontentindex] = "";
						allcontentindex++;
						i = j;
						writeindex = j + 1;
						gototemp = "";
						break;

					}

					if (CPST[j] == 123)  repeat++;
					if (CPST[j] == 125) {
						repeat--;

						if (repeat == 0) {

							for (int k = j + 1; k < length; k++) {
								gototemp += CPST[k];

								if (CPST[k] == 41) {

									j = k;
									break;
								}
							}

							allcontent[allcontentindex] = gototemp;
							contralkinds[allcontentindex] = cont;
							allcontentindex++;
							i = j;
							writeindex = j + 1;
							gototemp = "";
							break;
						}
					}
				}
			}
			else if (cont == "if(") {
				for (int j = i; j < length; j++) {

					gototemp += CPST[j];

					if (j == length - 1 && repeat >= 1) {

						allcontent[allcontentindex] = gototemp;
						contralkinds[allcontentindex] = "";
						allcontentindex++;
						i = j;
						writeindex = j + 1;
						gototemp = "";
						break;

					}

					if (CPST[j] == 123)  repeat++;
					if (CPST[j] == 125) {
						repeat--;

						if (repeat == 0) {
							string ifrept = "";
							for (int k = j + 1; k <= j + 9; k++) {
								ifrept += CPST[k];

								if (ifrept == "else{" || ifrept == "else if(") {
									break;
								}

							}
							if (ifrept == "else{" || ifrept == "else if(") continue;

							allcontent[allcontentindex] = gototemp;
							contralkinds[allcontentindex] = cont;
							//cout << "test" << endl;
							allcontentindex++;
							i = j;
							writeindex = j + 1;
							gototemp = "";
							break;
						}
					}
				}
			}
		}
		// 제어문이 끝나고 내용들 혹은 제어문이 없을때 그냥 나타내기 
		if (i + 1 == length) {

			for (int k = writeindex; k <= i; k++) {
				allcontent[allcontentindex] += CPST[k];
			}

		}

	}
}
//
//배열에 제어문인걸 제어문에 따라 바꿔준다.
void TransGoto() {
	for (int i = 0; i < allcontentindex; i++) {

		if (contralkinds[i] == "for(") {
			allcontent[i] = TransGotoFor(allcontent[i]);
		}
		else if (contralkinds[i] == "if(") {
			allcontent[i] = TransGotoIf(allcontent[i]);

		}
		else if (contralkinds[i] == "while(") {
			allcontent[i] = TransGotoWhile(allcontent[i]);
		}
		else if (contralkinds[i] == "do{") {
			allcontent[i] = TransGotoDowhile(allcontent[i]);
		}
	}
}
string TransGotoFor(string contant) {

	int gotoindex[51] = { 0, };

	int index = 36;

	int intend = contant.length();
	string fortransgoto[4] = { "", "","","" };
	int transgotoindex = 0;
	string fora;

	string result;
	bool usedblock = false;

	int repeat = 100;
	int plusint = 0;
	int overlap = 0;
	int lastgotoindex = 0;
	bool first1 = false;
	bool first2 = false;
	bool first3 = false;


	int countfor = TransGotoForIndex;
	string countforstring = "";
	while (true) {
		if (countfor >= 1000) {
			countforstring += "d";
			countfor -= 1000;
		}
		else if (countfor >= 100) {
			countforstring += "c";
			countfor -= 100;
		}
		else if (countfor >= 10) {
			countforstring += "b";
			countfor -= 10;
		}
		else if (countfor >= 1) {
			countforstring += "a";
			countfor -= 1;
		}
		if (countfor == 0) break;
	}

	for (int i = 0; i < intend; i++) {

		if (contant[i] == 40 && first1 == false) {
			//cout << "선언문" << endl;

			for (int first = i + 1; first < intend; first++) {
				fortransgoto[transgotoindex] += contant[first];
				if (contant[first] == 59) { i = first - 1; break; }
			}
			fora = to_string(Goto_index(index)) + ":";
			fortransgoto[transgotoindex] += countforstring + "ae" + to_string(Goto_index(index + 1)) + "();";
			index++;
			transgotoindex++;
			first1 = true;

		}
		else if (contant[i] == 59 && repeat == 100 && first2 == false) {
			//cout << "조건문" << endl;

			repeat = Goto_index(index);
			plusint = Goto_index(repeat - 1);

			fora = "function " + countforstring + "ae" + to_string(Goto_index(index)) + "(){if(";
			fortransgoto[transgotoindex] = fora;

			for (int first = i + 1; first < intend; first++) {
				if (contant[first] == 59) { i = first - 1; break; }
				fortransgoto[transgotoindex] += contant[first];

			}

			fora = ")return(" + countforstring + "ae" + to_string(Goto_index(index + 1)) + "());}";
			fortransgoto[transgotoindex] += fora;
			index++;
			transgotoindex++;
			first2 = true;
		}
		else if (contant[i] == 59 && usedblock == false && first3 == false) {
			//cout << "증감 and for문역할을 위한 돌아가기" << endl;
			string plus, cont = "";


			for (int first = i + 1; first < intend; first++) {
				if (contant[first] == 41) { i = first; break; }
				plus += contant[first];
			}

			for (int first = i; first < intend; first++) {
				if (contant[first] == 123) {
					overlap++;
					for (int second = first + 1; second < intend; second++) {

						if (contant[second] == 123) overlap++;
						if (contant[second] == 125) {
							overlap--;
							if (overlap == 0) {
								i = second;
								break;
							}
						}
						cont += contant[second];

					}

					string temp[3] = { "" , "", "" };
					int tempint = 0;
					int contlength = cont.length();
					int checkifint = 0;

					for (int qq = 0; qq < contlength; qq++) {

						//cout << cont[qq];


						if (cont[qq] == 123) {
							int kk = 0;
							bool checkif = false;
							string tempci;
							for (int ci = qq; ci >= checkifint; ci--) {
								tempci = "";
								if (ci >= 2) {
									for (int j = ci - 2; j <= ci; j++) {
										tempci += cont[j];
									}
								}
								if (tempci == "if(") {
									checkif = true;

									checkifint = ci + 1;
									break;
								}
								kk++;
							}

							if (checkif == false)      tempint++;


						}
						//else {                     tempint++;            
						if (tempint == 0) {
							temp[tempint] += cont[qq];
						}
						else if (tempint == 1) {
							if (cont[qq] == 123) {
								for (int over = qq; over < contlength; over++) {
									temp[tempint] += cont[over];
									if (cont[over] == 123) overlap++;
									if (cont[over] == 125) {
										overlap--;
										if (overlap == 0) {
											qq = over;
											break;
										}
									}



								}
								tempint++;
							}
						}
						else if (tempint == 2) {
							temp[tempint] += cont[qq];
						}
					}

					cont = "";

					for (int i = 0; i < 3; i++) {

						if (i == 0 || i == 2) {
							temp[i] = replace_all(temp[i], "break;", "return 0;");
							temp[i] = replace_all(temp[i], "continue;", "return(" + countforstring + "ae" + to_string(plusint) + "());");
						}

						cont += temp[i];
					}

					break;
				}
			}


			fora = "function " + countforstring + "ae" + to_string(Goto_index(index)) + "(){";
			string foraa = "return(" + countforstring + "ae" + to_string(plusint) + "());}";
			fortransgoto[transgotoindex] += fora + cont + foraa;


			fora = "function " + countforstring + "ae" + to_string(plusint) + "(){";
			fortransgoto[transgotoindex] += fora + plus + ";";
			fora = "return(" + countforstring + "ae" + to_string(repeat) + "());}";
			fortransgoto[transgotoindex] += fora;

			index++;
			transgotoindex++;
			usedblock = true;
			first1 = true;
		}
	}

	for (int i = 0; i < 4; i++) {
		result += fortransgoto[i];
	}

	TransGotoForIndex++;

	return result;
}
//

string TransGotoIf(string contant) {
	int index = 27;
	int intend = contant.length();

	string iftransgoto[100];
	string iftransgotoarray = to_string(Goto_index(index)) + "(){";

	int transgotoindex = 0;
	string ifcontentarray[100];  // 조건문 내용이 들어가는 곳
	int ifcontentarrayindex = 0;
	string ifarray[100]; // 조건이 들어가는 곳
	int ifarrayindex = 0;
	string ifelseif[100]; // 조건 이후 else인지 else if가 들어가는 곳
	int ifelseifindex = 0;

	int countif = TransGotoIfIndex;
	string countifstring = "";
	while (true) {
		if (countif >= 1000) {
			countifstring += "d";
			countif -= 1000;
		}
		else if (countif >= 100) {
			countifstring += "c";
			countif -= 100;
		}
		else if (countif >= 10) {
			countifstring += "b";
			countif -= 10;
		}
		else if (countif >= 1) {
			countifstring += "a";
			countif -= 1;
		}
		if (countif == 0) break;
	}

	int overlap = 0;
	string k = "";
	bool firstfindif = false;

	for (int i = 0; i < intend; i++) {


		if (firstfindif == false) {
			k += contant[i];

			if (k == "if") {
				ifelseif[ifelseifindex] = "if";
				ifelseifindex++;
				firstfindif = true;
			}
		}

		if (contant[i] == 40) {

			for (int first = i; first < intend; first++) {
				ifarray[ifarrayindex] += contant[first];

				if (contant[first] == 40) overlap++;
				if (contant[first] == 41) {
					overlap--;
					if (overlap == 0)
						i = first;
					break;
				}

			}
			if (ifarray[ifarrayindex] != "") ifarrayindex++;
		}
		if (contant[i] == 123) {
			overlap++;
			for (int first = i + 1; first < intend; first++) {
				if (contant[first] == 123) overlap++;
				if (contant[first] == 125) {
					overlap--;
					if (overlap == 0) {
						i = first;
						k = "";

						for (int second = i + 1; second <= intend; second++) {

							k += contant[second];

							if (k == "else{") {

								ifelseif[ifelseifindex] = "else";
								ifelseifindex++;
								i = second - 1;
								break;
							}
							if (k == "else if(") {
								ifelseif[ifelseifindex] = "else if";
								ifelseifindex++;
								i = second - 1;
								break;
							}
						}
					}
					break;
				}
				ifcontentarray[ifcontentarrayindex] += contant[first];
			}
			if (ifcontentarray[ifcontentarrayindex] != "") ifcontentarrayindex++;

		}
	}








	for (int i = 0; i < 100; i++) {

		if (ifelseif[i] == "") { iftransgotoarray = iftransgotoarray + "}"; break; }

		if (ifelseif[i] == "else") {
			iftransgotoarray = iftransgotoarray + ifelseif[i] + " return( " + countifstring + "er" + to_string(Goto_index(index + i + 1)) + "());";
		}
		else if (ifelseif[i] == "else if") {
			iftransgotoarray = iftransgotoarray + ifelseif[i] + ifarray[i] + " return( " + countifstring + "er" + to_string(Goto_index(index + i + 1)) + "());";
		}
		else if (ifelseif[i] == "if") {
			iftransgotoarray = countifstring + "er" + to_string(Goto_index(index)) + "(); " + "function " + countifstring + "er" + iftransgotoarray + ifelseif[i] + ifarray[i] + " return(" + countifstring + "er" + to_string(Goto_index(index + i + 1)) + "());";
		}

		iftransgoto[i] = "function " + countifstring + "er" + to_string(Goto_index(index + i + 1)) + "(){" + ifcontentarray[i] + "return( " + countifstring + "er" + to_string(Goto_index(index - 1)) + "());}"; // index 39로 빠져나감
	}


	string result = iftransgotoarray;

	for (int i = 0; i < 100; i++) {
		if (iftransgoto[i] == "") {
			result += "function " + countifstring + "er" + to_string(Goto_index(index - 1)) + "(){return 0;}";
			break;
		}
		result += iftransgoto[i];
	}

	TransGotoIfIndex++;
	return result;
}
//

string TransGotoWhile(string contant) {
	int index = 10;
	int intend = contant.length();
	string whiletransgoto[4];
	int transgotoindex = 0;
	string ifa;

	int repeat = 100;
	int overlap = 0;
	string plus;
	string result;

	int countwhile = TransGotoWhileIndex;
	string countwhilestring = "";
	while (true) {
		if (countwhile >= 1000) {
			countwhilestring += "d";
			countwhile -= 1000;
		}
		else if (countwhile >= 100) {
			countwhilestring += "c";
			countwhile -= 100;
		}
		else if (countwhile >= 10) {
			countwhilestring += "b";
			countwhile -= 10;
		}
		else if (countwhile >= 1) {
			countwhilestring += "a";
			countwhile -= 1;
		}
		if (countwhile == 0) break;
	}

	for (int i = 0; i < intend; i++) {
		if (contant[i] == 40) {
			overlap++;

			ifa = countwhilestring + "wh" + to_string(Goto_index(index)) + "(); " + "function " + countwhilestring + "wh" + to_string(Goto_index(index)) + "(){";
			whiletransgoto[transgotoindex] += ifa + "if(";

			for (int first = i + 1; first < intend; first++) {
				whiletransgoto[transgotoindex] += contant[first];
				if (contant[first] == 40) overlap++;
				if (contant[first] == 41) {
					overlap--;
					if (overlap == 0) {
						i = first;
						break;
					}
				}
			}

			ifa = "return(" + countwhilestring + "wh" + to_string(Goto_index(index + 1)) + "());}";
			whiletransgoto[transgotoindex] += ifa;

			index++;
			transgotoindex++;
		}
		else if (contant[i] == 123) {

			overlap++;

			ifa = "function " + countwhilestring + "wh" + to_string(Goto_index(index)) + "(){";
			whiletransgoto[transgotoindex] += ifa;

			for (int first = i + 1; first < intend; first++) {

				if (contant[first] == 123) overlap++;
				if (contant[first] == 125) {
					if (contant[first] == 125) {
						overlap--;
						if (overlap == 0) {
							i = first;
							break;

						}
					}
				}
				plus += contant[first];


			}

			string temp[3] = { "" , "", "" };
			int tempint = 0;
			int contlength = plus.length();
			int checkifint = 0;

			for (int qq = 0; qq < contlength; qq++) {

				//cout << cont[qq];


				if (plus[qq] == 123) {
					int kk = 0;
					bool checkif = false;
					string tempci;
					for (int ci = qq; ci >= checkifint; ci--) {
						tempci = "";
						if (ci >= 2) {
							for (int j = ci - 2; j <= ci; j++) {
								tempci += plus[j];
							}
						}
						if (tempci == "if(") {
							checkif = true;

							checkifint = ci + 1;
							break;
						}
						kk++;
					}

					if (checkif == false)      tempint++;


				}
				if (tempint == 0) {
					temp[tempint] += plus[qq];
				}
				else if (tempint == 1) {
					if (plus[qq] == 123) {
						for (int over = qq; over < contlength; over++) {
							if (plus[over] == 123) overlap++;
							if (plus[over] == 125) {
								overlap--;
								if (overlap == 0) {
									qq = over;
									break;
								}
							}

							temp[tempint] += plus[over];

						}
						tempint++;

					}
				}
				else if (tempint == 2) {
					temp[tempint] += plus[qq];
				}
			}

			plus = "";

			for (int i = 0; i < 3; i++) {

				if (i == 0 || i == 2) {
					temp[i] = replace_all(temp[i], "break;", "return 0;");
					temp[i] = replace_all(temp[i], "continue;", "return(" + countwhilestring + "wh" + to_string(Goto_index(index - 1)) + "());");
				}

				plus += temp[i];
			}

			ifa = "return(" + countwhilestring + "wh" + to_string(Goto_index(index - 1)) + "());}";
			whiletransgoto[transgotoindex] += plus + ifa;

			index++;
			transgotoindex++;


		}

	}
	for (int i = 0; i < 4; i++) {
		result += whiletransgoto[i];
	}

	TransGotoWhileIndex++;
	return result;
}


string TransGotoDowhile(string contant) {
	int index = 46;
	int intend = contant.length();
	string dowhiletransgoto[4];
	int transgotoindex = 0;
	string ifa;

	int repeat = 100;
	int overlap = 0;

	string plus;
	string result;

	int countdowhile = TransGotoDowhileIndex;
	string countdowhilestring = "";
	while (true) {
		if (countdowhile >= 1000) {
			countdowhilestring += "d";
			countdowhile -= 1000;
		}
		else if (countdowhile >= 100) {
			countdowhilestring += "c";
			countdowhile -= 100;
		}
		else if (countdowhile >= 10) {
			countdowhilestring += "b";
			countdowhile -= 10;
		}
		else if (countdowhile >= 1) {
			countdowhilestring += "a";
			countdowhile -= 1;
		}
		if (countdowhile == 0) break;
	}

	for (int i = 0; i < intend; i++) {

		if (contant[i] == 123) {

			overlap++;

			ifa = countdowhilestring + "dw" + to_string(Goto_index(index)) + "(); " + "function " + countdowhilestring + "dw" + to_string(Goto_index(index)) + "(){";
			dowhiletransgoto[transgotoindex] += ifa;

			for (int first = i + 1; first < intend; first++) {

				if (contant[first] == 123) overlap++;
				if (contant[first] == 125) {
					if (contant[first] == 125) {
						overlap--;
						if (overlap == 0) {
							i = first; break;
						}
					}
				}
				plus += contant[first];

			}

			string temp[3] = { "" , "", "" };
			int tempint = 0;
			int contlength = plus.length();
			int checkifint = 0;

			for (int qq = 0; qq < contlength; qq++) {

				//cout << cont[qq];


				if (plus[qq] == 123) {
					int kk = 0;
					bool checkif = false;
					string tempci;
					for (int ci = qq; ci >= checkifint; ci--) {
						tempci = "";
						if (ci >= 2) {
							for (int j = ci - 2; j <= ci; j++) {
								tempci += plus[j];
							}
						}
						if (tempci == "if(") {
							checkif = true;

							checkifint = ci + 1;
							break;
						}
						kk++;
					}

					if (checkif == false)      tempint++;


				}
				if (tempint == 0) {
					temp[tempint] += plus[qq];
				}
				else if (tempint == 1) {
					if (plus[qq] == 123) {
						for (int over = qq; over < contlength; over++) {
							if (plus[over] == 123) overlap++;
							if (plus[over] == 125) {
								overlap--;
								if (overlap == 0) {
									qq = over;
									break;
								}
							}

							temp[tempint] += plus[over];

						}
						tempint++;

					}
				}
				else if (tempint == 2) {
					temp[tempint] += plus[qq];
				}
			}
			plus = "";

			for (int i = 0; i < 3; i++) {

				if (i == 0 || i == 2) {
					temp[i] = replace_all(temp[i], "break;", "return 0;");
					temp[i] = replace_all(temp[i], "continue;", "return(" + countdowhilestring + "dw" + to_string(Goto_index(index + 1)) + "());");
				}

				plus += temp[i];
			}

			ifa = "return(" + countdowhilestring + "dw" + to_string(Goto_index(index + 1)) + "());";
			dowhiletransgoto[transgotoindex] += plus + ifa;
			dowhiletransgoto[transgotoindex] += "}";

			index++;
			transgotoindex++;

		}
		else if (contant[i] == 40) {
			overlap++;

			ifa = "function " + countdowhilestring + "dw" + to_string(Goto_index(index)) + "(){";
			dowhiletransgoto[transgotoindex] += ifa + "if(";

			for (int first = i + 1; first < intend; first++) {
				dowhiletransgoto[transgotoindex] += contant[first];
				if (contant[first] == 40) overlap++;
				if (contant[first] == 41) {
					overlap--;
					if (overlap == 0)
						i = first; break;
				}
			}

			ifa = "return(" + countdowhilestring + "dw" + to_string(Goto_index(index - 1)) + "());}";
			dowhiletransgoto[transgotoindex] += ifa + ";";

			index++;
			transgotoindex++;
		}
	}

	for (int i = 0; i < 4; i++) {
		result += dowhiletransgoto[i];
	}

	TransGotoDowhileIndex++;
	return result;
}
