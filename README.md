# stepmtrPJT



## 리니어 스테이지 모터 연구

### 길이 정보
실제 이동 가능한 레일 길이 = 100cm

### 각도 정보
32분주 이므로 1펄스당 0.05625도 회전.(360/6400)
6400번 펄스 주면 한 바퀴 회전. 

현재 0.001초당 1펄스 주는 중. 즉, 1초당 56.25도 회전하는 것... 
6.4초당 한 바퀴 간다. 

한 바퀴 돌면 몇 센치 가나요? 이걸 알아야돼.


5센치 가는데 9바퀴 하고도 6091 펄스가 들었습니다...
6091펄스 : 342.61875도
6090펄스 인 경우 : 342.5625도 이걸로 채택
9바퀴 : 360*9 = 3240도

5mm / 12490 펄스 계산하면,
1펄스당 : 0.0040032025620496mm 이동함

1미터는 1000mm 이므로, 1미터를 이동하기 위해 총 249,800.0000000025 펄스가 필요함. 일단 249800펄스로 맞춘다. 


IP Address: 192.168.0.94
ESP32 MAC Address: A0:DD:6C:10:64:5C


모터 프레임 끝 단에서 멈출 수 있는 센서
금속센서 추정

max : 25(추정)
1000 / (speed + 1)
맥시멈 속도 = 1000 / 26
미니멈 속도 = 1000 / 1
1000 선정 기준 : 예제 코드에 1000으로 기본 세팅 되어있고, 매우 느려서 모터에 부하 안 가는 속도.

속도 단위 = cm/sec
![figure](speedfigure.png)
as the input an the actual speed shows a legitimate linear trend, we can get a formula of the actual speed.

**0.0615 * input + 0.08 = speed**
However, the number is kind of messy, so we may just use them for reference.

Also, setting a high speed as initial input can cause a noise and potential damage. 
So if we do not need a manual speed control function, we can set the initial/stop move speed gradually.


https://docs.google.com/document/d/1bkvPQNkkFRb2QJgUPw2rLMsRN7aS1cx5Md6RTBFhhAI/edit



###



고정 속도
실제 이동 거리(속도 - 거리 실제)



파이썬으로 시리얼통신.

