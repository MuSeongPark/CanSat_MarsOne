# CanSat Competition Korea (2022)
## <img width="388" alt="ACT로고" src="https://user-images.githubusercontent.com/62331567/183722588-31290fee-752e-4306-a7c6-c957eacba6d3.png"> 
> 안녕하세요! 2022 캔위성 경연대회에 참가한 ACT 팀의 깃허브입니다!<br> <a href="https://instagram.com/act_cansat?igshid=YmMyMTA2M2Y=">Click here to VISIT OUR INSTAGRAM</a>
***
### 사용한 센서 및 라이브러리
*캔 위성 임무 구현을 위해 사용한 센서들의 이름과 사용 목적, 그리고 사용 라이브러리에 대한 설명입니다.*
+ **CCS811**
+ **MPU6050**
+ **NEO8M**
+ **BMP280**
+ 그 외 지상관제시스템과의 통신을 위한 **Xbee**<br>

|센서명|라이브러리|사용목적|
|:---:|:---:|:---:| 
|CCS811|Adafruit CCS811|실시간 이산화탄소 농도 분석에 필요한 데이터 수집을 위한 센서|
|MPU6050|I2Cdevlib MPU6050|z축 방향만 사용, 캔 위성이 안정된 상태에서 씨앗 사출 조건을 만족시키기 위한 센서|
|NEO8M|TinyGPSPlus|GPS데이터(위도,경도)를 받아와 지상관제시스템에 캔위성의 위치를 실시간으로 표시하기 위한 센서|
|BMP280|Adafruit BMP280|가속도 센서와 함께 사용, 지정한 고도에서 씨앗을 사출시키기 위한 센서|
***
### 설계 구상도와 실제 캔위성 모습
*좌측 사진 : 설계 , 우측 사진 : 실제 캔위성*<br><br>
<img src = "https://user-images.githubusercontent.com/62331567/183831049-e2455e1b-dc8a-4233-96be-8383e80155fe.jpg" width="350" height="500">   <img src = "https://user-images.githubusercontent.com/62331567/183831116-f6372207-cf4d-4de2-af05-9fea780ccd0e.jpg" width="350" height="500">
***



