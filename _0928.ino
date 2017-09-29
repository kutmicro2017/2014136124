int LED[4][3] = { { 22, 24, 26 }, { 23, 25, 27 }, { 30, 32, 34 }, { 31, 33, 35 } };
int button = 7;
volatile bool state = true; //현재 상태

void setup() {
  Serial.begin(9600);
  for(int i=0; i<4; i++)
  {
    for(int j=0; j<3; j++)
    {
      pinMode(LED[i][j],OUTPUT);
      Serial.println(LED[i][j]);
    }
  }
  pinMode(button,INPUT); //누르는 스위치 입력 모드
  Serial.println(button);
}

void loop() {
  //모든 빨간 신호가 켜짐
  digitalWrite(LED[0][0],HIGH);
  digitalWrite(LED[1][0],HIGH);
  digitalWrite(LED[2][0],HIGH);
  digitalWrite(LED[3][0],HIGH);
  
  
  for(int i=0; i<4; i++)
  {
    Serial.println(LED[i][0]);
    Serial.println(LED[i][1]);
    Serial.println(LED[i][2]);
    //신호등 빨간신호 꺼짐
    digitalWrite(LED[i][0],LOW);
    //신호등 초록신호 켜짐
    digitalWrite(LED[i][2],HIGH);
    delaySeconds(10);
    //신호등 초록신호 꺼짐
    digitalWrite(LED[i][2],LOW);
    //신호등 노랑신호 켜짐
    for(int j=0; j<6; j++)
    {
      digitalWrite(LED[i][1],HIGH);
      delay(500);
      digitalWrite(LED[i][1],LOW);
      delay(500);
    }
    //신호등 빨간신호 켜짐
    digitalWrite(LED[i][0],HIGH);
  }
}

int Check() //초록불인지 확인하는 함수
{
  for(int i = 0; i < 4; i++)
  {
    if(digitalRead(LED[i][2]) == HIGH)
        return i;
  }
}

void delaySeconds(int t) //초 단위로 스위치가 눌렸는지 확인하는 함수
{
  int a;
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

bool tTof() //스위치를 누르면 false값을 반환하는 함수
{
  if(digitalRead(button) == HIGH){
    Serial.println(button);
    state = false;
  }
   return state; 
}

