#include <SoftwareSerial.h>

  int bufferPosition; // 버퍼에 데이타를 저장할 때 기록할 위치
  const int flexPin5= A4;  // 검지 손가락 Flex센서와 연결된 Pin
  const int flexPin4 = A3; // 중지 손가락 Flex센서와 연결된 Pin
  const int flexPin3 = A2;
  const int flexPin1 = A0;
  const int flexPin2 = A1;

void setup(){
  Serial.begin(9600); 
}
void loop(){
  int flexVal1 = analogRead(flexPin1);
  int flexVal2 = analogRead(flexPin2);
  int flexVal3 = analogRead(flexPin3);
  int flexVal4 = analogRead(flexPin4);
  int flexVal5 = analogRead(flexPin5);
    int f1=500,f2=500,f3=500,f4=500,f5=500;

  if(abs(flexVal1-f1)>100 || abs(flexVal2-f2)>100 ||abs(flexVal3-f3)>100 || abs(flexVal4-f4)>100 || abs(flexVal5-f5)>100){
  Serial.print(flexVal1);
  Serial.print(",");
  Serial.print(flexVal2);
  Serial.print(",");
  Serial.print(flexVal3);
  Serial.print(",");
  Serial.print(flexVal4);
  Serial.print(",");
  Serial.print(flexVal5);
  Serial.print("\n");
  }    
  delay(1000);
  
}