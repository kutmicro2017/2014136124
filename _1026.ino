// 시간 관련 상수
#define GREEN_DELAY           10                // 초록불 지속 시간(s)
#define BLINK_COUNT           3                 // 노란불 점멸 횟수
#define GREEN_TO_YELLOW       1                 // 버튼을 누르고 노란불이 켜질 때까지의 시간(s)
#define RED_DELAY             5                 // 보행자 버튼 지속 시간(s)

// 차량 신호등
// 상하 -> 좌우 방향으로 반복
// 1차 배열: { 상(i = 0), 좌(i = 1), 하(i = 0), 우(i = 0) }
// 2차 배열: { 빨간불, 노란불, 초록불 }
int LED[4][3] = { { 22, 24, 26 }, { 23, 25, 27 }, { 28, 30, 32 }, { 29, 31, 33 } };
int buttonUD = 9;                   // 보행자 버튼(상하)
int buttonLR = 8;                   // 보행자 버튼(좌우)
volatile bool stateUD = true;       // 보행자 버튼(상하) 상태(true : 버튼을 누르지 않은 상태)
volatile bool stateLR = true;       // 보행자 버튼(좌우) 상태(true : 버튼을 누르지 않은 상태)

// 보행자 신호등
// 8x8 도트 매트릭스 핀 번호 정의
int uCol[] = { 0, 1, 2, 3, 4, 5, 6, 7 };           // 열(정지 신호)
int uRow[] = { 21, 20, 19, 18, 17, 16, 15, 14 };   // 행(정지 신호)
int dCol[] = { 34, 36, 38, 40, 42, 44, 46, 48};    // 열(통행 신호)
int dRow[] = { 35, 37, 39, 41, 43, 45, 47, 49 };   // 행(통행 신호)

// 잔여 시간 타이머
int remainingTime[10][8][4] = {
  { // 0
    { 1, 1, 1, 0 },
    { 1, 0, 1, 0 },
    { 1, 0, 1, 0 },
    { 1, 0, 1, 0 },
    { 1, 0, 1, 0 },
    { 1, 0, 1, 0 },
    { 1, 1, 1, 0 },
    { 0, 0, 0, 0 }
  },
  { // 1
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 0 }
  },
  { // 2
    { 1, 1, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0},
    { 1, 1, 1, 0 },
    { 1, 0, 0, 0 },
    { 1, 0, 0, 0 },
    { 1, 1, 1, 0 },
    { 0, 0, 0, 0 }
  },
  { // 3
    { 1, 1, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 1, 1, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 1, 1, 1, 0 },
    { 0, 0, 0, 0 }
  },
  { // 4
    { 1, 0, 1, 0 },
    { 1, 0, 1, 0 },
    { 1, 0, 1, 0 },
    { 1, 1, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 0 }
  },
  { // 5
    { 1, 1, 1, 0 },
    { 1, 0, 0, 0 },
    { 1, 0, 0, 0 },
    { 1, 1, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 1, 1, 1, 0 },
    { 0, 0, 0, 0 }
  },
  { // 6
    { 1, 1, 1, 0 },
    { 1, 0, 0, 0 },
    { 1, 0, 0, 0 },
    { 1, 1, 1, 0 },
    { 1, 0, 1, 0 },
    { 1, 0, 1, 0 },
    { 1, 1, 1, 0 },
    { 0, 0, 0, 0 }
  },
  { // 7
    { 1, 1, 1, 0 },
    { 1, 0, 1, 0 },
    { 1, 0, 1, 0 },
    { 1, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 0 }
  },
  { // 8
    { 1, 1, 1, 0 },
    { 1, 0, 1, 0 },
    { 1, 0, 1, 0 },
    { 1, 1, 1, 0 },
    { 1, 0, 1, 0 },
    { 1, 0, 1, 0 },
    { 1, 1, 1, 0 },
    { 0, 0, 0, 0 }
  },
  { // 9
    { 1, 1, 1, 0 },
    { 1, 0, 1, 0 },
    { 1, 0, 1, 0 },
    { 1, 1, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 1, 1, 1, 0 },
    { 0, 0, 0, 0 }
  }
};

// 보행자 신호 패턴
int pattern[2][8][8] = {
  { // STOP
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 1, 1, 0, 0, 1},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0} 
  },
  { // PASS
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {1, 0, 0, 0, 1, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 1, 0, 0, 1},
    {0, 0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 0, 1, 1, 0} 
  }
};



// 사전 설정
void setup() {
  // LED의 출력 모드 설정
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 3; j++)
      pinMode(LED[i][j], OUTPUT);
  }
  // 스위치의 입력 모드 설정
  // 그냥 INPUT을 사용할 경우 스위치가 계속해서 눌러지는 상태가 되므로 한 번만 눌러도 동작이 가능한 INPUT_PULLUP을 사용
  pinMode(buttonUD, INPUT_PULLUP);
  pinMode(buttonLR, INPUT_PULLUP);

  // 도트 매트릭스 출력 모드 설정
  for(int i = 0; i < 8; i++) {
    pinMode(uRow[i], OUTPUT);
    pinMode(uCol[i], OUTPUT);
    pinMode(dRow[i], OUTPUT);
    pinMode(dCol[i], OUTPUT);
  }
  
  // 모든 LED의 빨간신호가 켜짐
  for(int i = 0; i < 4; i++)
    digitalWrite(LED[i][0], HIGH);
}



// 루프
void loop() {
  for(int i = 0; i < 2; i++) {
    // 신호등 빨간신호 꺼짐
    digitalWrite(LED[i][0], LOW);
    digitalWrite(LED[i + 2][0], LOW);
    
    // 신호등 초록신호 켜짐
    digitalWrite(LED[i][2], HIGH);
    digitalWrite(LED[i + 2][2], HIGH);
    
    // 초록신호가 켜진 상태에서 아래 함수를 이용하여 스위치 동작 확인
    buttonInterrrupt(GREEN_DELAY);
    
    // 신호등 초록신호 꺼짐
    digitalWrite(LED[i][2], LOW);
    digitalWrite(LED[i + 2][2], LOW);
    
    // 신호등 노랑신호 켜짐
    yellowBlink(i);
    
    // 신호등 빨간신호 켜짐(그 다음 LED로 넘어감)
    digitalWrite(LED[i][0], HIGH);
    digitalWrite(LED[i + 2][0], HIGH);
  }
}



// 신호등 노랑신호 점멸 함수(0.5초 켜졌다 0.5초 꺼짐)
void yellowBlink(int a) {
  for(int i = BLINK_COUNT; i > 0 ; i--) {
    digitalWrite(LED[a][1], HIGH);        // 노란불 켜짐
    digitalWrite(LED[a + 2][1], HIGH);
    for(int j = 0; j < 25; j++) {         // =delay(500) (각 함수가 10ms씩 걸리므로 20 * 25 = 500)
      displaySignal(a);                   // 보행자 신호 패턴 표시
      displayTime(a, i);                  // 잔여 시간 표시
    }
    digitalWrite(LED[a][1], LOW);         // 노란불 꺼짐
    digitalWrite(LED[a + 2][1], LOW);
    for(int j = 0; j < 25; j++) {         // 위와 동일
      displaySignal(a);
      displayTime(a, i);
    }
  }
}

// LED가 초록불인지 확인 후 해당 인덱스를 반환하는 함수
int check() {
  for(int i = 0; i < 2; i++) {
    if(digitalRead(LED[i][2]) == HIGH && digitalRead(LED[i + 2][2]) == HIGH)
      return i;
  }
}

// ms 단위로 스위치가 눌렸는지 확인하는 함수
// polling 방법을 이용하므로 계속해서 확인을 해주어야함
// 만약 스위치를 눌렀다는 것을 확인하면 동작하고 있는 신호등의 초록불은 꺼지고 노란불이 깜빡거리며 빨간불로 바뀜
void buttonInterrrupt(int t) {
  for(int i = t * 50; i > 0; i--) {
    if(isPushUD() == false) {                    // 보행자 버튼(상하) 누른 경우
      if(check() == 1) changeLight(check());     // 좌우 신호등이 초록불일 때 상하 신호등과 좌우 신호등의 신호 바꾸기
      i = t * 50;                                // 상하 신호등 초록불 지속시간 초기화
      stateUD = true;
    }
    if(isPushLR() == false) {                    // 보행자 버튼(좌우) 누른 경우
      if(check() == 0) changeLight(check());     // 상하 신호등이 초록불일 때 좌우 신호등과 상하 신호등의 신호 바꾸기
      i = t * 50;                                // 좌우 신호등 초록불 지속시간 초기화
      stateLR = true;
    }
    displaySignal(check());                             // 보행자 신호 패턴 표시
    displayTime(check(), (i / 50) + BLINK_COUNT + 1);   // 잔여 시간 표시(초록불 지속 시간 + 노란불 지속 시간 + 1초(0초가 되자마자 체인지하기 위해))
  }
}

// 버튼이 눌렸을 경우 불빛이 바뀌는 절차를 구현한 함수
void changeLight(int a) {
  for(int i = GREEN_TO_YELLOW * 50; i > 0; i--) {       // =delay(GREEN_TO_YELLOW * 1000)
    displaySignal(a);
    displayTime(a, i / 50 + 4);
  }
  digitalWrite(LED[a][2], LOW);       // 초록불 꺼짐
  digitalWrite(LED[a + 2][2], LOW);
  yellowBlink(a);                     // 노란불 점멸

  if(a == 1) a = -1;                  // 좌우 신호등이 초록불일 때 반대쪽 신호등의 빨간불이 꺼지도록 매개변수 값 임시 설정
  digitalWrite(LED[a + 1][0], LOW);   // 반대쪽 신호등 빨간불 꺼짐
  digitalWrite(LED[a + 3][0], LOW);
  digitalWrite(LED[a + 1][2], HIGH);  // 반대쪽 신호등 초록불 켜짐
  digitalWrite(LED[a + 3][2], HIGH);
  if(a == -1) a = 1;                  // 매개변수 값 복귀
  digitalWrite(LED[a][0], HIGH);      // 빨간불 켜짐
  digitalWrite(LED[a + 2][0], HIGH);

  for(int i = RED_DELAY * 50; i > 0; i--) {             // =delay(RED_DELAY * 1000)
    displaySignal(!a);                                  // 보행자 신호등의 패턴과 잔여 시간을 반대로 표시(0 <-> 1)
    displayTime(!a, i / 50 + BLINK_COUNT + 1);
  }
  if(a == 1) a = -1;                  // 좌우 신호등이 초록불일 때 반대쪽 신호등의 빨간불이 꺼지도록 매개변수 값 임시 설정
  digitalWrite(LED[a + 1][2], LOW);   // 반대쪽 신호등 초록불 꺼짐
  digitalWrite(LED[a + 3][2], LOW);
  yellowBlink(a + 1);                 // 노란불 점멸
  digitalWrite(LED[a + 1][0], HIGH);  // 반대쪽 신호등 빨간불 켜짐
  digitalWrite(LED[a + 3][0], HIGH);
  if(a == -1) a = 1;                  // 매개변수 값 복귀
  digitalWrite(LED[a][0], LOW);       // 빨간불 꺼짐
  digitalWrite(LED[a + 2][0], LOW);
  digitalWrite(LED[a][2], HIGH);      // 초록불 켜짐
  digitalWrite(LED[a + 2][2], HIGH);
}

// 단순히 스위치를 눌렀을 때 동작할 수 있는 조건(스위치를 누르면 false가 반환)을 반환하기 위한 함수
// 스위치를 눌렀을 때를 LOW로 가정하여 상태를 false로 만들어줌
bool isPushUD() {                          // 보행자 버튼(상하) 상태
  if(digitalRead(buttonUD) == LOW)
    stateUD = false;
  return stateUD;
}
bool isPushLR() {                          // 보행자 버튼(좌우) 상태
  if(digitalRead(buttonLR) == LOW)
    stateLR = false;
  return stateLR;
}



// 정지 신호 도트 매트릭스 초기화
void clearStopSign() {
  for(int k = 0; k < 8; k++) {
    digitalWrite(uRow[k], LOW);
    digitalWrite(uCol[k], HIGH);
  }
}

// 통행 신호 도트 매트릭스 초기화
void clearPassSign() {
  for(int k = 0; k < 8; k++) {
    digitalWrite(dRow[k], LOW);
    digitalWrite(dCol[k], HIGH);
  }
}

// 보행자 정지/통행 신호 표시(10ms) (매개변수 i = 상하(0) or 좌우(1))
void displaySignal(int i) {
  for(int r = 0; r < 8; r++) {
    if(i == 1) {                    // 좌우 신호등이 초록불일 때
      clearStopSign();              // 보행자 정지 신호등에 정지 패턴 출력
      digitalWrite(uRow[r], HIGH);
      for(int c = 0; c < 8; c++)
        if(pattern[0][r][c]) digitalWrite(uCol[c], LOW);
    }
    else {                          // 상하 신호등이 초록불일 때
      clearPassSign();              // 보행자 통행 신호등에 통행 패턴 출력
      digitalWrite(dRow[r], HIGH);
      for(int c = 0; c < 8; c++)
        if(pattern[1][r][c]) digitalWrite(dCol[c], LOW);
    }
    delay(1);                       // 1초 딜레이를 유지함으로써 출력이 제대로 되도록 함
  }
  delay(2);
  clearStopSign();
  clearPassSign();
}

// 잔여 시간 표시(10ms) (매개변수 i = 상하(0) or 좌우(1), t = 초)
void displayTime(int i, int t) {
  for(int r = 0; r < 8; r++) {
    if(i == 0) {                    // 상하 신호등이 초록불일 때
      clearStopSign();              // 보행자 정지 신호등에 잔여 시간 표시
      digitalWrite(uRow[r], HIGH);
      for(int c = 0; c < 4; c++) {
        if(remainingTime[t / 10][r][c]) digitalWrite(uCol[c], LOW);       // 십의 자리 출력
        if(remainingTime[t % 10][r][c]) digitalWrite(uCol[c + 4], LOW);   // 일의 자리 출력
      }
    }
    else {                          // 좌우 신호등이 초록불일 때
      clearPassSign();              // 보행자 통행 신호등에 잔여 시간 표시
      digitalWrite(dRow[r], HIGH);
      for(int c = 0; c < 4; c++) {
        if(remainingTime[t / 10][r][c]) digitalWrite(dCol[c], LOW);       // 십의 자리 출력
        if(remainingTime[t % 10][r][c]) digitalWrite(dCol[c + 4], LOW);   // 일의 자리 출력
      }
    }
    delay(1);
  }
  delay(2);
  clearStopSign();
  clearPassSign();
}

