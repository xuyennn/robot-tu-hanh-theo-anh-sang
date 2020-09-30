
#include <Servo.h>
//Thiết lập chân điều khiển động cơ 
const int MotorA = 7;
const int MotorB = 4; 
//Thiết lập chân điều khiển tốc độ động cơ A 
const int EnMotorA = 6;
const int EnMotorB = 5;
//Thiết lập chân đọc cường độ ánh sáng
#define light A3

int leftscanval, centerscanval, rightscanval, ldiagonalscanval, rdiagonalscanval; //biến lưu giá trị ánh sáng tại 5 điểm
int inten_limit = 150;  //cường độ ánh sáng giới hạn
char choice;            //biến lựa chọn hướng đi

int inten;              //biến lưu cường độ ánh sáng môi trường
char turndirection;     //Biến lưu hướng quay robot
const int turntime = 400; //Thời gian quay robot
const int turntimeh = 2200;

Servo head;   //đặt tên servo

void setup()
{
  head.attach(A0);// Thiết lập chân cho động cơ servo
  head.write(80);
   pinMode(MotorA, OUTPUT); 
  pinMode(MotorB, OUTPUT); 
  analogWrite(EnMotorA, 0);
  analogWrite(EnMotorB, 0);
  Serial.begin(9600);   //dùng cho debug
}

void go(){ //đi thẳng
  analogWrite(EnMotorA, 50);
  analogWrite(EnMotorB, 50);
  digitalWrite (MotorA, LOW);                              
  digitalWrite (MotorB, HIGH); 
}

void backwards(){ //lùi
  analogWrite(EnMotorA, 50);
  analogWrite(EnMotorB, 50);
  digitalWrite (MotorA, HIGH);                              
  digitalWrite (MotorB, LOW); 
}

int watch(){    //đọc ánh sáng
  long light_inten;
  light_inten = analogRead(light);
  return round(light_inten);
}

void turnleft(int t){   //rẽ trái
  analogWrite(EnMotorA, 50);
  analogWrite(EnMotorB, 50);
  digitalWrite (MotorA, HIGH);                              
  digitalWrite (MotorB, HIGH); 
  delay(t);
}

void turnright(int t){    //rẽ phải
  analogWrite(EnMotorA, 50);
  analogWrite(EnMotorB, 50);
  digitalWrite (MotorA, LOW);                              
  digitalWrite (MotorB, LOW); 
  delay(t);
}  

void stopmove(){  //dừng robot
  analogWrite(EnMotorA, 0);
  analogWrite(EnMotorB, 0);
}  

void watchsurrounding() //quan sát xung quanh
 {  //đọc cường độ ánh sáng tại 5 điểm
  head.write(80); 
  delay(500);
  centerscanval = watch();
  Serial.print("centerscanval = ");
  Serial.println(centerscanval);
  
  head.write(120);
  delay(500);
  ldiagonalscanval = watch();
  Serial.print("ldiagonalscanval = ");
  Serial.println(ldiagonalscanval);
  delay(50);
  
  head.write(160); 
  delay(500);
  leftscanval = watch();
  Serial.print("leftscanval = ");
  Serial.println(leftscanval);
  delay(50);
    
  head.write(40);
  delay(500);
  rdiagonalscanval = watch();
  Serial.print("rdiagonalscanval = ");
  Serial.println(rdiagonalscanval);
  delay(50);
  
  head.write(0);
  delay(500);
  rightscanval = watch();
  Serial.print("rightscanval = ");
  Serial.println(rightscanval);
  delay(50);

  head.write(80); 
  delay(300);
}

char decide()   //quyết định hướng quay của robot
{
  watchsurrounding();
  if (ldiagonalscanval<rightscanval && ldiagonalscanval<centerscanval && leftscanval>ldiagonalscanval){
    choice = 'd';   //Quay trái 50%
  }
  else if (leftscanval<rightscanval && leftscanval<centerscanval && leftscanval<ldiagonalscanval){
    choice = 'l';   //Quay trái
  }
  else if (rightscanval<leftscanval && rightscanval<centerscanval && rightscanval<rdiagonalscanval){
    choice = 'r';   //Quay phải
  }
  else if (rdiagonalscanval<leftscanval && rdiagonalscanval<centerscanval && rightscanval>rdiagonalscanval){
    choice = 'h';   //Quay phải 50%
  }
  else{
    choice = 'f';   //Đi thẳng
  }
  return choice;
}

void loop()
{
  inten = watch();
  Serial.print("inten = ");
  Serial.println(inten);
  if(inten > inten_limit){ 
    stopmove();
    turndirection = decide();
    Serial.print("directiton = ");
    Serial.println(turndirection);
    switch (turndirection){
      case 'l':
      turnleft(turntime);
      delay(20);
      break;

      case 'd':
      turnleft(turntimeh);
      delay(20);
      break;
      
      case 'r':
      turnright(turntime);
      delay(20);
      break;

      case 'h':
      turnright(turntimeh);
      delay(20);
      break;
      
      case 'f':
      break;
    } 
    go();
    head.write(80);
    }
   else { go();}
  }
