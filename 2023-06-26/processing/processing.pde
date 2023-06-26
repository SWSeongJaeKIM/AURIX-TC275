import processing.serial.*;
Serial port;
int board_late = 115200;
String comport_result = "COM16";
int flag = 0;
int test1 = 100;
int motor_speed = 0;
int motor_postion = 0;
void setup()
{
  
  
  size(500, 500);
  smooth();
  background(245,235,235);
  
   
 
  //println(Serial.list()); //Shows your connected serial ports
  //portName = (Serial.list());
  port = new Serial(this,"COM16",115200); 
  port.bufferUntil('\n');  //Shows your connected serial ports //Shows your connected serial ports
  
   
   
   
  //**************************** 초기 사각형
  fill(232,199,199);
  rect(50, 80, 120, 60);    //COMPORT 알려줄 함수
  rect(50, 200, 120, 60);   //BOARD RATE 알려줄 사각형
  rect(50, 320, 120, 60);   //모드 변경 사각형
  rect(220, 160, 120, 60);   //속도 변경 사각형
  rect(220, 240, 120, 60); //위치 변경 사각형
  rect(350, 160, 70, 60);   //속도 reset변경 사각형
    rect(350, 240, 70, 60);   //속도 reset변경 사각형
  //**************************** 초기 사각형
    
  //**************************** 문자열
  fill(0);
  text("COMPORT", 85, 95);
  text("COMPORT", 85, 215);
  text("Velocity : 0 Postion : 1", 55, 335);
  text("Velocity", 230, 175);
  text("Postion", 230, 255);


  textSize(28);
  text(comport_result, 70, 125);
  text(board_late, 70, 245);
  text(flag, 105, 365);
//**************************** 문자열
  
  
  
  
}



/*
void serialEvent(Serial port)
{
  
 byte[] in_buff = port.readBytesUntil('\n');  // 프로세싱을 사용하려 경우 TC275의 송신 프로토콜에 마지막에 '\n'을 반드시 추가해서 넣을 것
 int byte_length = 0;
 int[] input_io_data = new int[10];
 int i;
 
 if(in_buff != null)
  {
    byte_length = in_buff.length;
    //print("Data length :  ");
   // println(byte_length);
    
      for(i=0;i<byte_length;i++)
      {
          //print( (in_buff[i]);  // byte의 범위가 -127+256이므로
          //print("   ");
      }

  
    }
}
*/



void draw()
{
  port.write(motor_speed);
 // if(flag ==1) port.write(motor_speed);
  //else if(flag ==2) port.write(motor_postion);
  fill(0,0,0);
  textSize(28);
  text(motor_speed,230, 210);
  text(motor_postion,230, 290);
  
    if(flag>=2)
    {
      flag = 0;
    fill(232,199,199);
    rect(50, 320, 120, 60);
    textSize(12);
    fill(0,0,0);
    text("Velocity : 0 Postion : 1", 55, 335);
    textSize(28);
    text(flag, 105, 365);
    }
    
}


void keyPressed() {
  switch (flag) {
    case 0:
      if (keyCode == UP) {
        fill(232, 199, 199);
        rect(220, 160, 120, 60);
        fill(0);
        motor_speed += 5;  // 방향키 위를 누를 때마다 motor_speed 변수에 5를 더함
        textSize(15);
        text("Velocity", 230, 175);
      } else if (keyCode == DOWN) {
        fill(232, 199, 199);
        rect(220, 160, 120, 60);
        fill(0);
        motor_speed -= 5;  // 방향키 아래를 누를 때마다 motor_speed 변수에서 5를 뺌
        textSize(15);
        text("Velocity", 230, 175);
      }
      break;

    case 1:
      if (keyCode == UP) {
        fill(232, 199, 199);
        rect(220, 240, 120, 60);
        fill(0);
        motor_postion += 5;  // 방향키 위를 누를 때마다 motor_speed 변수에 5를 더함
        textSize(15);
        text("Position", 230, 255);
      } else if (keyCode == DOWN) {
        fill(232, 199, 199);
        rect(220, 240, 120, 60);
        fill(0);
        motor_postion -= 5;  // 방향키 아래를 누를 때마다 motor_speed 변수에서 5를 뺌
        textSize(15);
        text("Position", 230, 255);
      }
      break;
  }
}


void mouseClicked() {
  // 버튼 영역 안에서 마우스 클릭을 확인
  if (mouseX > 350 && mouseX < 420 && mouseY > 160 && mouseY < 220)
  {
     fill(232,199,199);
    rect(220, 160, 120, 60);
    fill(0,0,0);
    motor_speed = 0;
    textSize(15);
    text("Velocity", 230, 175);
    delay(10);
  }
  
  else if (mouseX > 350 && mouseX < 420 && mouseY > 240 && mouseY < 300)
  {
     fill(232,199,199);
    rect(220, 240, 120, 60);
    fill(0,0,0);
    motor_postion = 0;
    textSize(15);
    text("Position", 230, 255);
    delay(10);
  }
    else if (mouseX > 50 && mouseX < 170 && mouseY > 320 && mouseY < 380)
  {
    fill(232,199,199);
    rect(50, 320, 120, 60);
    textSize(12);
    fill(0,0,0);
    text("Velocity : 0 Postion : 1", 55, 335);
    textSize(28);
    flag++;
   text(flag, 105, 365);

    delay(10);
  }
  
}
