#include <DS1302.h>                // RTC 라이브러리 포함한다.
#include <LiquidCrystal.h>        // LiquidCrystal.h 라이브러리를 포함한다.

// ******************************************* rtc 관련 **************************************************

DS1302 rtc(26, 24, 22); // CLK(D7), DAT(D6), RST(D5)
                         // DS1302모듈을 init해주는 역할(init: 시스템이 종료될 때까지 계속 실행되는 데몬 프로세스)
LiquidCrystal lcd(48, 46, 47, 45, 43, 41);       // LCD가 연결된 핀을 설정한다.

const int pushUpButton = 5;      // Up 버튼
const int modeButton = 6;        // 모드 버튼
const int pushDownButton = 7;    // Down 버튼

int modeState = 0;
int upState = 0;
int downState = 0;

int mode = 0;

// 디폴트 값 날짜, 시간
uint16_t years = 2017;        // 년  
uint8_t months = 12;          // 달
uint8_t days = 12;            // 일
uint8_t hours = 3;            // 시
uint8_t minutes =45;         // 분
uint8_t seconds = 0;          // 초

// 수정하는 날짜 시간
uint16_t set_years = 2017;    // 년  
uint8_t set_months = 1;       // 달  
uint8_t set_days = 1;         // 일  
uint8_t set_hours = 0;        // 시  
uint8_t set_minutes = 0;      // 분 
uint8_t set_seconds = 0;      // 초

// ********************************************* LED 관련 ***************************************************

int touchSensor = 9;                    // 터치 센서
int mainLight = 2;                      // 3W LED
int colorLight_red = 11;                // LED red
int colorLight_green = 12;              // LED green
int colorLight_blue = 13;               // LED blue

int prev_red = 0;                       // 이전 red 값
int prev_green = 0;                     // 이전 green 값
int prev_blue = 0;                      // 이전 blue 값
int prev_white = 0;                     // 이전 white 값

bool colorMode = false;                 // 모드 상태(true = 램프, false = 색상등)
int colorLight_ColorNum = 0;            // 색상등 색상 번호
String colorLight_Color = "red";        // 색상등 색상 문자열값

int pirPin = 4;                         // PIR 센서

unsigned long prev_time_lcd = 0;        // LCD 이전 시간(ms)
unsigned long prev_time_led = 0;        // LED 이전 시간(ms)
unsigned long curr_time = 0;            // 다음 시간(ms)
unsigned long curr_time_lcd = 0;        // LCD 다음 시간(ms)





void setup() {               // 초기설정
    Serial.begin(9600);
//********************************************** rtc 관련 ************************************************

    rtc.halt(false);         // 시간을 run-mode로 설정
    rtc.writeProtect(false); // 쓰는 것을 방지하는 비활성화 역할
                             // false로 설정해주어야 시간 설정을 자유롭게 할 수 있다. 00시 00분 00초가 되면 다시 reset되어 처음부터 카운팅
                             // 처음 시간과 날짜를 설정해주고 나서 업로드 하고선 다시 시간과 날짜 코드를 주석처리하고 업로드를 해야지 
                             // 아두이노에 초기 시간과 날짜가 설정되어 전원이 꺼져도 동작이 가능해진다 

    rtc.setTime(hours, minutes, seconds);       // 00시 05분 30초(시간)
    rtc.setDate(days, months, years);           // 2017년 12월 12일 (날짜)
    lcd.begin(16, 2);                           // LCD의 크기를 설정한다.
    
    pinMode(modeButton, INPUT);
    pinMode(pushUpButton, INPUT);
    pinMode(pushDownButton, INPUT);

//************************************************ LED관련 ***********************************************

    pinMode(touchSensor, INPUT);
    pinMode(pirPin, INPUT);
  
    pinMode(mainLight, OUTPUT);
    pinMode(colorLight_red, OUTPUT);
    pinMode(colorLight_green, OUTPUT);
    pinMode(colorLight_blue, OUTPUT);
    
}





void loop() {
  
  modeState = digitalRead(modeButton);
  upState = digitalRead(pushUpButton);
  downState = digitalRead(pushDownButton);

  int touchValue = digitalRead(touchSensor);            // 터치 센서 입력
  int analogInput = analogRead(A15);                    // 가변 저항 입력
  int pirState = digitalRead(pirPin);                   // PIR 상태
  
  curr_time = millis();                                 // 현재 시간(ms) 기록

//************************************************ rtc관련 ***************************************************

  Serial.println(mode);
  if(modeState == HIGH){
      mode = (mode + 1) % 7;
    }
  if(mode == 0){ //&& curr_time - prev_time_lcd >= 1000) {        // RTC 정상 작동  
    lcd.setCursor(0, 0);           // 커서 위치 이동(0번째 칸, 0번째 줄)
    lcd.print(rtc.getDateStr());   // 날짜를 출력
    lcd.setCursor(0, 1);           // 커서 위치 이동(0번째 칸, 1번째 줄)
    lcd.print(rtc.getTimeStr());   // 시간을 출력
    delay(1000);
  }
  else {                           // 날짜 변경 모드
    print_s();
//    curr_time_lcd = millis();
    delay(500);
//    if(curr_time_lcd - prev_time_lcd >= 500) {
      if(mode == 1){               // 일 변경
        if(upState == HIGH){
          set_days++;
          
          if(set_months == 1 || set_months == 3 || set_months == 5 || set_months == 7 || set_months == 8 || set_months == 10 || set_months == 12){
            set_days = (set_days % 32);          // 1 ~ 31까지 숫자 (일)
            if(set_days == 0) set_days = 1;
          }
          else if(set_months == 4 || set_months == 6 || set_months == 9 || set_months == 11){
            set_days = (set_days % 31);          // 1 ~ 30까지 숫자 (일)
            if(set_days == 0) set_days = 1;
          }
          else{
            if( (set_years % 400 == 0) || ((set_years % 4 == 0) && (set_years % 100 != 0)) ){
              set_days = (set_days % 30);        // 1 ~ 29까지 숫자 (일)-윤년
              if(set_days == 0) set_days = 1;
            }
            else{
              set_days = (set_days % 29);        // 1 ~ 28까지 숫자 (일)
              if(set_days == 0) set_days = 1;
            }
          }
        }
        if(downState == HIGH){
          set_days--;
          Serial.println(set_days);
          if(set_months == 1 || set_months == 3 || set_months == 5 || set_months == 7 || set_months == 8 || set_months == 10 || set_months == 12){
            set_days = (set_days % 32);
            if(set_days == 0) set_days = 31;
          }
          else if(set_months == 4 || set_months == 6 || set_months == 9 || set_months == 11){
            set_days = (set_days % 31);
            if(set_days == 0) set_days = 30;
          }
          else{
            if( (set_years % 400 == 0) || ((set_years % 4 == 0) && (set_years % 100 != 0)) ){
              set_days = (set_days % 30);
              if(set_days == 0) set_days = 29;
            }
            else{
              set_days = (set_days % 29);
              if(set_days == 0) set_days = 28;
            }
          }
        }
      }
      if(mode == 2){                              // 월 변경
        Serial.println("Mode 2");
        if(upState == HIGH){
          Serial.println("set_months");
          set_months++;
          set_months = (set_months % 13);         // 1~ 12까지 숫자 (월)
          if(set_months >= 13) set_months = 1;
        }
        if(downState == HIGH){
          Serial.println("set_months");
          set_months--;
          if(set_months <= 0) set_months = 12;
        }
      }
      if(mode == 3){
        Serial.println("Mode 3");
        if(upState == HIGH){
          set_years++;
        }
        if(downState == HIGH){
          set_years--;
        }
      }
      if(mode == 4){
        Serial.println("Mode 4");
        if(upState == HIGH){
          set_hours++;
          set_hours = (set_hours % 24);       // 0 ~ 23까지 숫자 (시)
          if(set_hours >= 24) set_hours = 0;
        }
        if(downState == HIGH){
          set_hours--;
          if(set_hours <= -1) set_hours = 23;
          if(set_hours == 25) set_hours = 23;
        }
      }
      if(mode == 5){
        Serial.println("Mode 5");
        if(upState == HIGH){
          set_minutes++;
          set_minutes = (set_minutes % 60);   // 0 ~ 59까지 숫자 (분)
        }
        if(downState == HIGH){
          set_minutes--;
          if(set_minutes < 0) set_minutes = 59;
        }
      }
      if(mode == 6){                   // 마지막 세팅버튼 입력시
        Serial.println("Mode 6");
        rtc.setTime(set_hours, set_minutes, set_seconds);   // 수정된 시간 세팅
        delay(2);
        rtc.setDate(set_days, set_months, set_years);       // 수정된 날짜 세팅
        
        // 수정하는 날짜 시간 초기화
        set_years = 2017;     // 년
        set_months = 1;       // 달  
        set_days = 1;         // 일  
        set_hours = 0;        // 시  
        set_minutes = 0;      // 분 
        set_seconds = 0;      // 초
        mode = 0;             // 시간 정상적으로 출력
      }
//      prev_time_lcd = millis();
//    }
  }
  
//************************************************ LED관련 ***************************************************

  // PIR 센서 반응 시(마지막으로 PIR 센서가 반응한지 3초 후)
   if(pirState == HIGH && curr_time - prev_time_led >= 3000) {
     if(colorMode) {                                     // 색상등 모드일 경우
       switch(colorLight_ColorNum) {                     // 다음 색상 선택
         case 0: colorLight_Color = "red"; break;
         case 1: colorLight_Color = "green"; break;
         case 2: colorLight_Color = "blue"; break;
         case 3: colorLight_Color = "yellow"; break;
         case 4: colorLight_Color = "magenta"; break;
         case 5: colorLight_Color = "cyan"; break;
       }
       colorLight_ColorNum = (colorLight_ColorNum + 1) % 6;
     }
     prev_time_led = millis();                           // 이전 시간(ms) 기록
   }

  // 터치 센서 반응 시(마지막으로 터치 센서가 반응한지 1초 후)
  if(touchValue == HIGH && curr_time - prev_time_led >= 1000) {
    if(colorMode) {                                     // 색상등 모드일 경우
      setColor("off", 0, colorMode);                    // 색상등 소등
      colorMode = false;                                // 램프 모드로 변경
    }
    else {                                              // 램프 모드일 경우
      setColor("off", 1023, colorMode);                 // 램프 소등
      colorMode = true;                                 // 색상등 모드로 변경
    }
    prev_time_led = millis();                           // 이전 시간(ms) 기록
  }
  setColor(colorLight_Color, analogInput, colorMode);   // 색상 변경
}

//************************************************ rtc관련 ***************************************************

void print_s() {
      if(set_days < 10) {
        lcd.setCursor(0, 0);
        lcd.print("0");
        lcd.setCursor(1, 0);
        lcd.print(set_days);     
      } else if(set_days >= 10) {
        lcd.setCursor(0, 0);
        lcd.print(set_days);
      }

      lcd.setCursor(2, 0);
      lcd.print(".");
      
      if(set_months < 10) {
        lcd.setCursor(3, 0);
        lcd.print("0");
        lcd.setCursor(4, 0);
        lcd.print(set_months);
      } else if(set_months >= 10) {
        lcd.setCursor(3, 0);
        lcd.print(set_months);
      }
      
      lcd.setCursor(5, 0);
      lcd.print(".");

      lcd.setCursor(6, 0);
      lcd.print(set_years);
      
      if(set_hours < 10) {
        lcd.setCursor(0, 1);
        lcd.print("0");
        lcd.setCursor(1, 1);
        lcd.print(set_hours);
      } else if(set_hours >= 10) {
        lcd.setCursor(0, 1);
        lcd.print(set_hours); 
      }
      
      lcd.setCursor(2, 1);
      lcd.print(".");

      if(set_minutes < 10) {
        lcd.setCursor(3, 1);
        lcd.print("0");
        lcd.setCursor(4, 1);
        lcd.print(set_minutes);
      } else if(set_minutes >= 10) {
        lcd.setCursor(3, 1);
        lcd.print(set_minutes);
      }
      
      lcd.setCursor(5, 1);
      lcd.print(".");

      if(set_seconds < 10) {
        lcd.setCursor(6, 1);
        lcd.print("0");
        lcd.setCursor(7, 1);
        lcd.print(set_seconds);
      } else if(set_seconds >= 10) {
        lcd.setCursor(6, 1);
        lcd.print(set_seconds);
      }
}

//************************************************ LED관련 *************************************************

// 색상 변경 함수(색상 이름, 가변 저항 입력값, 현재 모드)
void setColor(String color, int analog, bool mode) {
  if(color == "white") setColor(255, 255, 255, analog, mode);         // 흰색
  else if(color == "red") setColor(255, 0, 0, analog, mode);          // 적색
  else if(color == "green") setColor(0, 255, 0, analog, mode);        // 녹색
  else if(color == "blue") setColor(0, 0, 255, analog, mode);         // 청색
  else if(color == "yellow") setColor(255, 255, 0, analog, mode);     // 노란색
  else if(color == "magenta") setColor(255, 0, 255, analog, mode);    // 자주색
  else if(color == "cyan") setColor(0, 255, 255, analog, mode);       // 청록색
  else if(color == "off") setColor(0, 0, 0, analog, mode);            // 꺼짐
  else setColor(0, 0, 0, analog, mode);                               // 그 외(=꺼짐)
}

// 색상 변경 함수(R, G, B, 가변 저항 입력값, 현재 모드)
void setColor(int red, int green, int blue, int analog, bool mode) {
  int white;                    // 3W LED 밝기
  if(mode)                      // 색상등 모드일 경우 3W LED 밝기 최소
    white = 0;
  else                          // 램프 모드일 경우 3W LED 밝기 최대
    white = 255;
  
  if(mode && prev_white != 255) {       // 색상등 모드이면서 3W LED가 꺼져있는 경우
    for(int i = 0; i <= 255; i++) {
      if(i >= prev_red - red && prev_red < red)             // red 값 증가
        analogWrite(colorLight_red, (prev_red + i) * (255 - (analog/4)) / 255);
      if(i >= prev_green - green && prev_green < green)     // green 값 증가
        analogWrite(colorLight_green, (prev_green + i) * (255 - (analog/4)) / 255);
      if(i >= prev_blue - blue && prev_blue < blue)         // blue 값 증가
        analogWrite(colorLight_blue, (prev_blue + i) * (255 - (analog/4)) / 255);
      if(i >= red - prev_red && prev_red > red)             // red 값 감소
        analogWrite(colorLight_red, (prev_red - i) * (255 - (analog/4)) / 255);
      if(i >= green - prev_green && prev_green > green)     // green 값 감소
        analogWrite(colorLight_green, (prev_green - i) * (255 - (analog/4)) / 255);
      if(i >= blue - prev_blue && prev_blue > blue)         // blue 값 감소
        analogWrite(colorLight_blue, (prev_blue - i) * (255 - (analog/4)) / 255);
      
      if(prev_red != red || prev_green != green || prev_blue != blue)       // RGB 값이 바뀔 때 딜레이
        delay(1);
    }
    delay(3);

    // 최종적으로 RGB 값 고정
    analogWrite(colorLight_red, red * (255 - (analog/4)) / 255);
    analogWrite(colorLight_green, green * (255 - (analog/4)) / 255);
    analogWrite(colorLight_blue, blue * (255 - (analog/4)) / 255);

    // 이전 RGB 값에 현재 RGB 값 저장
    prev_red = red;
    prev_green = green;
    prev_blue = blue;
  }
  else {
    for(int i = 0; i <= 255; i++) {     // 램프 모드이거나 3W LED가 켜져있는 경우
      if(i >= prev_white - white && prev_white < white)     // white 값 증가
        analogWrite(mainLight, (prev_white + i) * (255 - (analog/4)) / 255);
      if(i >= white - prev_white && prev_white > white)     // white 값 감소
        analogWrite(mainLight, (prev_white - i) * (255 - (analog/4)) / 255);
      
      if(prev_white != white)                               // white 값이 이전 white 값과 다르면 딜레이
        delay(1);
    }
    delay(3);

    // 최종적으로 white 값 고정
    analogWrite(mainLight, white * (255 - (analog/4)) / 255);

    // 이전 white 값에 현재 white 값 저장
    prev_white = white;
  }
}
