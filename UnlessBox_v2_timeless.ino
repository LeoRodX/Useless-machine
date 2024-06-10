  /*
  Created 2024  by RodX
  RodX Labs Inc.
  
  Useless machine (timeless)
  БЕСПОЛЕЗНАЯ МАШИНА
  Бесполезная машина это развлечение, 
  которое должно быть постоянно готово удивить гостя, 
  готовое месяцами стоять на полке 
  ждать когда гость обратит на него внимание, 
  после чего сразу начать развлекать гостя без предварительных манипуляций,
  например, включения тумблера питания или зарядки 
  ... и простым для повторения.
  Поэтому для корпуса устройтсва подойдет любая шкатулка из магазина для творчества, 
  испольуются доступные модули, крепится все на термоклей,
  рука(толкатель тумблера) из медной проволоки с пайкой для жесткости,    
  есть таймер энергосбережения для полного отключения питания, 
  единственный тумблер включает питание и функционал устройства.  
 */

#include <Servo.h>   //используем библиотеку для работы с сервоприводом
Servo servoPaw, servoLid;     //объявляем переменную servo для лапы и крышки

int script = 0; ////9 сценарий выключения тумбера
int ajarLid = 155;  //150 угол приоткрытой крышки
int closedLid = 115;  //угол закрытой крышки
int forwardPaw = 40;  //45 угол выдвижения вперед лапы
int backwardPaw = 180;  //угол спрятанной лапы
int iFirstTime = 1; //первый сценарий при включнии питания
int powered = 0;  //pin питание включено
int toggleState = 0;  //положение тумблера
///int pressing_time_Paw = 500;  //время нажатия лапы на тумблер

const int togglePin = 2;  //pin тумблера
////const int buttonPin = 2;  // the number of the pushbutton pin   ////
const int relayPin = 3;  //был 4 the number of the relay pin
const int eyesPin = 8;  //pin глаз
const long inaction = 60000;  //бездействие перед отключением

unsigned long currentMillis = 0; 
unsigned long StartMillisInaction = 0; 
unsigned long endTime = 0;
unsigned long difTime = 0;

void setup() {
  pinMode(togglePin, INPUT); //Тумблер
  pinMode(relayPin, OUTPUT); //реле
  digitalWrite(relayPin, HIGH); //включаем реле при подаче питания
  delay(300);  
  servoPaw.attach(6);  //серво лапы pin 5  
  servoLid.attach(7);  //серво крышки pin 7 
  pinMode(eyesPin, OUTPUT); //Глаза  
  servoPaw.write(backwardPaw); //Назад лапу
  delay(500);//500  
  servoLid.write(closedLid);   
  randomSeed(analogRead(0));  //установка случайного алгоритма функции random
}

void ajar_lid() {
  // мигнем глазами 
  servoLid.write(ajarLid);
  delay(100);////
  digitalWrite(eyesPin, HIGH);
  delay(150);
  digitalWrite(eyesPin, LOW);
  delay(150);
  digitalWrite(eyesPin, HIGH);
  delay(150);
  digitalWrite(eyesPin, LOW);
}

void closed_lid() {
  servoLid.write(closedLid);
  delay(500); ///500 
}

  void switch_off() {
  //Выключает
  ajar_lid ();
  servoPaw.write(forwardPaw);
  delay(600);
  servoPaw.write(backwardPaw);
  delay(500);
  closed_lid();
}

void watching() {
  //Наблюдает
  ajar_lid ();
  servoPaw.write(90);
  delay(2000);
  servoPaw.write(backwardPaw);
  delay(500);
  closed_lid();      
  delay(1700);
}

void slowly(int initial_angle, int time) {
    //медленно убираем
    for (int i = initial_angle; i <= backwardPaw; i = i + 1){
        servoPaw.write(i); 
        delay(30);            
      } 
    delay(time);  
  }  

  void look() {
    //Смотрит
    ajar_lid ();
    delay(1700);
    closed_lid();      
    delay(200);    
  }  

void loop() { 

  //гибернация
  powered = digitalRead(togglePin);
  currentMillis = millis();

  //проверим положение тумблера
  toggleState = digitalRead(togglePin);
  
  if (toggleState == HIGH) {
    //если тумблер включен:      
    difTime=currentMillis-endTime;
    //если тумблер не щелкали более 10 сек или первое включение устройство 
    if (difTime > 10000 || iFirstTime == 1) {
       script = 1;
    } else {
       script = random(1, 12);
    }

    switch (script) {
      case 1:
        //Выключает
        switch_off();
        script = 0;
        break;
      case 2:
        //Наблюдает, прячется, выключает
        watching();
        switch_off();
        script = 0;
        break;
      case 3:
        //Выключает, прячется, наблюдает, прячется
        switch_off();
        watching(); 
        script = 0;
        break;
      case 4:
        //Выключает, держит, убирает лапу
        ajar_lid ();
        servoPaw.write(forwardPaw);
        delay(500);
        //Удерживает
        servoPaw.write(forwardPaw + 7);
        delay(4000);
        servoPaw.write(backwardPaw);
        delay(500);
        closed_lid();
        script = 0;
        break;
      case 5:
        //Выключает, держит, медленно убирает лапу
        ajar_lid ();
        servoPaw.write(forwardPaw);
        delay(500);
        //Держит
        servoPaw.write(forwardPaw + 7);
        delay(3000);
        slowly(forwardPaw + 7, 1000);        
        closed_lid();
        script = 0;
        break;
      case 6:
        //Коснется, держит, медленно убирает лапу, быстро выключает
        ajar_lid ();
        //Держит
        servoPaw.write(forwardPaw + 18);
        delay(3000);
        slowly(forwardPaw + 18, 1000);        
        delay(3000);
        closed_lid();       
        delay(1700);
        switch_off();        
        script = 0;
        break;      
      case 7:
        //Включает, косается, косается, косается, косается, убирает лапу.
        ajar_lid ();
        servoPaw.write(forwardPaw);
        delay(500);
        servoPaw.write(forwardPaw + 7);
        delay(1000);
        for (int i=1; i<=4; i++) {
          servoPaw.write(backwardPaw - 80);
          delay(300);
          servoPaw.write(forwardPaw + 7);
          delay(1000);
        }      
        servoPaw.write(backwardPaw);
        delay(500);  
        closed_lid();       
        script = 0;
        break;
      case 8:
        //Открывает/закрывает четырежды, выключает.
        for (int i=1; i<=4; i++) {
          ajar_lid ();
          delay(700);
          closed_lid(); 
        }  
        delay(500);     
        switch_off(); 
        script = 0;
        break;
      case 9:
        //Выключает
        switch_off();
        script = 0;
        break;
      case 10:
        //Наблюдает, прячется, выключает
        watching();
        switch_off();
        script = 0;
        break;
      case 11:
        //Выключает, прячется, наблюдает, прячется
        switch_off();
        watching(); 
        script = 0;
        break;
      //case 12:
        //Выключает
        //switch_off();
        //script = 0;
        //break;
      }

      endTime = currentMillis;
      iFirstTime = 0;
      StartMillisInaction = currentMillis; //гибер
  }

//отключение питания
  if (powered == HIGH) {
    //Включение реле питания
      digitalWrite(relayPin, HIGH);
      StartMillisInaction = currentMillis;    
  } else {
    if (currentMillis - StartMillisInaction > inaction) {
      //Отключие реле питания
      digitalWrite(relayPin, LOW);
      delay(300); //задержка необходимая реле для отключения
      //Serial.print("Time: "); 
      //Serial.println(currentMillis); // prints time since program started
    }  
  }  


 }
