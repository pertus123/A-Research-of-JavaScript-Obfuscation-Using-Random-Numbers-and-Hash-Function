A Research of JavaScript Obfuscation Using Random Numbers and Hash Function 
=============


주의사항
-------------

* virtualbox을 통해 공유 파일에 첨부된 7GB ova파일을 
  실행하면 17GB 가상머신이 생성됩니다.
* 현재 crontab과 apache 서버가 꺼져있는 상태입니다.
  * crontab 활성화	service crond start
  * 아파치 활성화	./root/shellscript.sh
* ECMAscript 6 버전과 SHA256 해시 함수를 사용했습니다.
* ECMAscript 버전에 따라 달라지는 키워드나 사용자 예외 키워드는 
  JsKeyword.txt에 수동입력할 수 있습니다. 
* 로직 변경은 제어문은 for, while, do-while 입니다.
* 예외적인 함수 사용 시, 프로그램이 정상 실행이 안될 수 있습니다.
* 원본 파일이나 서비스 실행 파일 변경(이름, 위치) 등 상황에 따라 
  make 파일, shellscript, 메인코드, crontab, httpd.conf 파일변경을 해야합니다.

<br>
<br>

시스템 실행 환경
-------------

* CentOS7RealTestReal	LAST EDIT 2019.12.14
* 사용자 이름 : root 	암호 : centos
* CentOS Linux release 7.5.1804 (xore) Version.
* Server version, Apache 2.4.35 (Unix)
* PHP version, PHP 5.6.38 (cli)
* Mysql version, 15.1 Distrib 5.5.64-MariaDB, for Linux(x86_64) using read line 5.1
* GCC Version, (GCC) 4.8.5 20150623 (Read Hat 4.8.5-28)

<br>
<br>

파일 위치
-------------

* 난독화 시킬 원본 파일
  > /root/Project/target/read/

<br>

* 난독화가 수행된 파일
  > /testhtml1/httpd/
  > 
  > /testhtml2/httpd/

<br>

* 소스 파일 및 JsKeyword.txt, make 파일
  > /root/Project/

<br>

* 쉘 스크립트
  > /root/shellscript.sh

<br>
<br>

shell script & crontab
-------------
- 위치  /root/shellscript.sh
  > if a -nt b => a 파일이 b 파일보다 최신이면 참.
* * *

```
#!/bin/bash
if [ /testhtml1/httpd/index.html -nt /testhtml2/httpd/index.html ];then
        killall httpd
        /usr/local/apache/bin/httpd -f /usr/local/apache/conf/httpd.conf
        /usr/local/apache/bin/apachectl start
        /root/Project/exetesthtml2
else
        killall httpd
        /usr/local/apache/bin/httpd -f /usr/local/apache/conf/httpd2.conf
        /usr/local/apache/bin/apachectl start
        /root/Project/exetesthtml1
fi
```
***
- crontab
  > 30 * * * * /root/shellscript.sh
- crontab 명령
  > service crond [start, stop, restart]

<br>
<br>

소스 파일 구성
-------------
- 위치 : root/Project/
***

[test.cpp]
- 메인 파일.

[sha256.h], [sha256.cpp]
- 해시 알고리즘을 사용하기 위한 라이브러리

[JsKeyword.txt]
- ECMAscript의 표준에 의한 키워드 및 예약어, 없는 키워드는 수동으로 입력

[makefile 구성]
```
exetesthtml2: test.o sha256.o				// 실행될 파일명 exetesthtml1, 2로
	g++ -o exetesthtml2 test.o sha256.o		// 변경하면서 수행
test.o: test.cpp
	g++ -c test.cpp
sha256.o: sha256.cpp
	g++ -c sha256.cpp
clean:
	rm *.o
```
<br>
<br>

원본 및 서비스 파일 위치 변경
-------------

[test.cpp]
- 492 line : 난독화가 될 원본 파일 위치
- 493 line : 난독화 수행된 파일의 위치(서비스 실행할 위치)

<br>

492 line 원본파일 위치 수정 시,
- Vi MakeFile에서 각각 위치에 해당하는 실행 파일 생성, make를 통한 생성 및 실행
- 서비스 시작.

493 line 서비스파일 위치 수정 시, 
- Vi MakeFile에서 각각 위치에 해당하는 실행 파일 생성, make를 통한 생성 및 실행
- 아파치 서비스 실행을 하기 위해 /usr/local/apache/conf/httpd.conf, httpd2.conf에서
231, 232, 239 line 서비스 디렉터리 경로 설정
- 서비스 시작.

***

<br>
<br>

1.Introduce
=============
* ‘안랩, 2019 년 3 분기 사이버 공격 동향 발표’에 따르면 웹 기반 공격이 44%로 가장 높은 비중을 차지하고 있음.
  
* 웹 서비스의 중요성은 커지고 있는 반면, 웹 서비스에는 다양한 취약점이 존재하며 정보 유출과 취약점 파악에 관한 보안 관리는 허술.
  
* 클라이언트 측에서 노출된 코드를 분석할 경우, 서버 측 코드와 로직을 예측 및 분석하거나 직접적인 공격 또한 가능.
  
* 이러한 문제점을 보완할 수 있는 클라이언트 측 보안을 강화하는 방법으로 난독화(obfuscation) 기법을 제시

<br>

![Alt text](/img/website_security.JPG)

<br>

![Alt text](/img/javascript_img.JPG)

<br>
<br>




2.JavaScript obfuscation technique
=============

프로그램 설계
-------------

* 내부 디렉터리에서 HTML과 JAVASCRIPT를 탐색하고 난독화를 수행하는 방법으로 DFS 탐색. 동영상, 사진 등 다른 파일은 복사를 수행.
* 예외적인 함수 키워드를 찾기 위한 1 - PASS 전처리 작업 후 난독화 수행
* 2개의 난독화 파일을 생성하여, 웹 서비스는 일정 시간마다 디렉터리 포인트를 변경하며 성공적으로 수행된다면 다음 디렉터리의 난독화를 수행
  
![Alt text](/img/TheProposedObfuscationProcess.JPG)

<br>

Obfuscation Using Hash Function
-------------

* 난독화를 진행하는 방안으로 충돌이 발견되지 않은 SHA-256 해시 함수 사용
* 입력 값의 아주 작은 변화로도 결과 값이 전혀 다르게 도출되는 효과
*  Salt 값을 사용함으로써, 해시함수 결과의 충돌가능성을 낮추고, 난독화의 결과값을 정기적으로 바꿈으로써, 유추 가능성 또한 낮출 수 있음.
*  출력 값이 64 byte로 고정되어있기 때문에 파일이 커지고, 저장 공간 낭비와 파일을 읽을 때 시간 증가 문제점이 발생. 이를 해결하기 위해 난독화 슬라이드 수행.

![Alt text](/img/hash_function.JPG)

<br>

로직 변경
-------------

* 해시 함수 난독화와 함께 로직 변경 진행.
* 변경되는 제어문은 for 문, while 문, do-while 문 등의 3 종이며, 각 제어문의 기능은 그대로 유지한 채로 함수형으로 변경.

![Alt text](/img/logic_change_for.JPG)

![Alt text](/img/logic_change_for2.JPG)

<br>
<br>

***

참고 문헌
-------------

[1] 김영호, 최동식, 여상수 (2019). 난수 및 해시함수를 이용한 자바스크립트 난독화 방안 연구. JOURNALOF PLATFORM TECHNOLOGY, 7(4), 49-56

[2] Young-Ho Kim, Dong-Sik Choi, Sang-Soo Yeo, “A Research of JavaScript Obfuscation Using Random Numbers,” 2019 Summer Conference of ICT Platform Society, pp. 111-113, 2019.

[3] Young-Ho Kim, Dong-Sik Choi, Sang-Soo Yeo, “An Obfuscation Method on JavaScript using Logic Flow Change,” 2019 Summer Conference of ICT Platform Society, pp. 108-110, 2019.