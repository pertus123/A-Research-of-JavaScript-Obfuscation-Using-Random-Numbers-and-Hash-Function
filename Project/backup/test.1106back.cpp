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
//#include <direct.h>
#include "sha256.h"
using namespace std;

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
// 예약어 개수 
void KeywordRead() {
	maxWord = 0;
	string KeywordArry = "/root/test/testdir/JsKeywords.txt";
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
					HJInit(item->d_name);
					JsRead(item->d_name, file_path2);
				}

				else if (file_name == "tml") {
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
	string line, compareC;
	a = "";	// 난독화 값을 같는 전역 변수. 초기화.
	addr = addr + readFileName; // 주소 + 파일명
	ifstream inFile(addr.c_str()); // 파일 읽기
	int tk = 0, cnt = 0;

	while (getline(inFile, line)) {
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

				if (compareC.substr(0, 7) == "script " || compareC.substr(0, 7) == "script>") {
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
					a+=k;
				}
				if ((65 <= line[i] && line[i] <= 90) || (97 <= line[i] && line[i] <= 122)) { // 만약 알파벳이라면,
					for (int j = i; j < line.length(); j++) { //알파벳 위치부터 , 알파벳 끝나는 부분까지 compareC에 단어만듬
						if (!((65 <= line[j] && line[j] <= 90) || (97 <= line[j] && line[j] <= 122) || (48 <= line[j] && line[j] <= 57) || (line[j] == '$')|| (line[j] == '_'))) { 								break;
						}
						compareC += line[j];
					}
				}
				else {
					a = a + line[i];
					continue;
				}

				if (ExTable(compareC.c_str()) == true) { // 만들어진 단어, 예약어 검사
					i += compareC.length() - 1; // 단어 길이만큼 +
					a += compareC;
					cout << compareC << endl;
					continue; // 그리고 밑에 것 무시 맨위로
				}

				k += salt[0] + sha256(compareC+salt).substr(0,40);
				i += compareC.length() - 1;
				a = a + k;
			}
			else a = a + k + line[i];
		}
		cnt++;
		if ((cnt % 100) == 0) {
			cnt = 0;
			HJWrite(readFileName);
			a = "";
		}
	}
	HJWrite(readFileName); // 저장
	inFile.close();
}

void JsRead(string readFileName, string addr) { // 파일 읽기. 파일명
	string line, compareC;
	a = "";	// 난독화 값을 같는 전역 변수. 초기화.
	addr = addr + readFileName; // 주소 + 파일명
	ifstream inFile(addr.c_str()); // 파일 읽기
	int cnt = 0;
	while (getline(inFile, line)) {
		k = "";
		for (int i = 0; i < line.length(); i++) { //전수 조사
			if (line[i] == '/' && line[i + 1] == '*') { //tk 부분 다시 전체적 다시
				int tk = 0;
				for (int j = i + 2; j <= line.length(); j++) {
					if (line[j] == '*' && line[j + 1] == '/') {
						tk = 1;
						break;
					}
				}
				if (tk == 0) continue;
			}
			if (line[i] == '\'' || line[i] == '"') i = Qutoes(i, line);
			if (line[i] == '/' && line[i + 1] == '/') break; // 주석 제거 합시다. 마지막 먹히는 것까지
			if (line[i] == '	') continue; // 이거슨 텝
			compareC = "";
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
				
				k += salt[0] + sha256(compareC+salt).substr(0,40);
				i += compareC.length() - 1;
				continue;
			}
			k = k + line[i];
		}

		a = a + k + '\n';
		cnt++;
		if ((cnt % 100) == 0) {
			cnt = 0;
			HJWrite(readFileName);
			a = "";
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
	char file_path[PATH_MAX] = "/root/test/testdir/read";
	char file_copyDir[PATH_MAX] = "/testhtml2/httpd";

	srand((unsigned int)time(NULL));
	saltNumber = (rand() % 5)+5;

	ChSaltCreate();
	KeywordRead();
	FileSearch(file_path, file_copyDir);

	return 0;
}
