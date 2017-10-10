int LED[4][3] = { { 22, 24, 26 }, { 23, 25, 27 }, { 30, 32, 34 }, { 31, 33, 35 } };
int button = 7;
volatile bool state = true; //현재 상태

void setup() 
{
  //LED의 출력 모드 설정
  for(int i=0; i<4; i++)
  {
    for(int j=0; j<3; j++)
    {
      pinMode(LED[i][j],OUTPUT);
    }
  }
  //스위치의 입력 모드 설정
  //그냥 INPUT을 할 경우 스위치가 계속해서 눌러지는 상태
  //INPUT_PULLUP을 해야 한 번만 눌러도 동작이 가능
  pinMode(button,INPUT_PULLUP); 
}

void loop() 
{
  //모든 LED의 빨간 신호가 켜짐
  digitalWrite(LED[0][0],HIGH);
  digitalWrite(LED[1][0],HIGH);
  digitalWrite(LED[2][0],HIGH);
  digitalWrite(LED[3][0],HIGH);
  
  //기본 루프
  for(int i=0; i<4; i++)
  {
    //신호등 빨간신호 꺼짐(처음 시작하는 신호등 LED는 빨간 신호가 아닌 초록 신호이고 나머지 LED는 빨간 신호이므로)
    digitalWrite(LED[i][0],LOW);
    
    //신호등 초록신호 켜짐(10초 유지)
    digitalWrite(LED[i][2],HIGH);
    //초록신호가 켜진 상태에서 아래 함수를 이용하여 스위치 동작 확인
    delaySeconds(10);
    
    //신호등 초록신호 꺼짐
    digitalWrite(LED[i][2],LOW);
    
    //신호등 노랑신호 켜짐(시간에 대한 for문으로 한 번 반복할 때마다 0.5초 켜졌다 꺼지므로 총 1초가 깜빡, 따라서 총 3초 깜빡임을 유지)  
    for(int j=0; j<6; j++)
    {
      digitalWrite(LED[i][1],HIGH);
      delay(500);
      digitalWrite(LED[i][1],LOW);
      delay(500);
    }
    
    //신호등 빨간신호 켜짐(그 다음 LED로 넘어감)
    digitalWrite(LED[i][0],HIGH);
  }
}

//LED가 초록불인지 확인하는 함수
//스위치를 누르는 조건이 초록불일 때이므로
int Check() 
{
  for(int i = 0; i < 4; i++)
  {
    if(digitalRead(LED[i][2]) == HIGH)
        return i;
  }
}

//초 단위로 스위치가 눌렸는지 확인하는 함수
//polling 방법을 이용하므로 계속해서 확인을 해주어야함
//만약 스위치를 눌렀다는 것을 확인하면 동작하고 있는 신호등의 초록불은 꺼지고 노란불이 깜빡거리며 빨간불로 바뀜
void delaySeconds(int t) 
{
  int a; //네 개의 신호등의 인덱스 변수
  for(int i = 0; i < t; i++)
  {
    if(tTof() == false)
    {
      a = Check();
      delay(3000);
      digitalWrite(LED[a][2],LOW); //초록불 꺼짐
      digitalWrite(LED[a][1],HIGH); //노란불 켜짐
      delay(3000);
      digitalWrite(LED[a][1],LOW); //노란불 꺼짐
      digitalWrite(LED[a][0],HIGH); //빨간불 켜짐
      delay(5000);
      digitalWrite(LED[a][0],LOW); //빨간불 꺼짐
      digitalWrite(LED[a][2],HIGH); //초록불 켜짐
      state = true;
    }
    delay(1000);
  }
}

//스위치를 누르면 false값을 반환하는 함수(단순히 스위치를 눌렀을 때 동작할 수 있는 조건(스위치를 누르면 false가 반환)을 반환하기 위한 함수)
bool tTof() 
{
  if(digitalRead(button) == LOW){ //스위치를 눌렀을 때를 LOW로 가정하여 상태를 false로 만들어줌
    state = false;
  }
   return state;
}

