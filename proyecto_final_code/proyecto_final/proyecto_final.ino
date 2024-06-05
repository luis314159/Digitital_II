#include <BluetoothSerial.h> 
BluetoothSerial BT;    // Definimos los pines RX y TX del Arduino conectados al Bluetooth
 
int motora1 = 23;
int motora2 = 22;
int motorb1 = 1;
int motorb2 = 3; 
int apwm = 21;
int bpwm = 19;
int sensor = 36;
int sen_in;
int a = 5;
int inf = 494-a;
int sup = 494+a;
float mapeo1;
float mapeo2;
int x;
int y;
string data;

void setup() {
  // put your setup code here, to run once:
    BT.begin("carro");
    pinMode(motora1, OUTPUT);
    pinMode(motora2, OUTPUT);
    pinMode(motorb1, OUTPUT);
    pinMode(motorb2, OUTPUT);
    pinMode(apwm, OUTPUT);
    pinMode(bpwm, OUTPUT);
    pinMode (sensor, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
    if(BT.available())    // Si llega un dato por el puerto BT se envía al monitor serial
  {
    data = BT.read();
    //separar datos TODO
   splitData(data, x,y);
    fun_h(x,y);
  }
  sen_in = readUltrasonic();
}

void fun_h(int x, int y){
  //Respuestá del punto H en función en la entrada de joystick
  //funcion para puente h
    if (y>=inf && y<=sup){
        if(inf>x){
            //GIRO ESTATICO IZQUIERDA
            mapeo1 = map(y, 0,1023 , 0,512);
            digitalWrite(motora1, HIGH);
            digitalWrite(motora2, LOW);
            digitalWrite(motorb1, LOW);
            digitalWrite(motorb2, HIGH);
            analogWrite(apwm, mapeo1 );
            analogWrite(bpwm, mapeo1 );
        }
      if(sup<x){
        //GIRO ESTATICO DERECHA
            mapeo1 = map(y, 0,1023 , 0,512);
            digitalWrite(motora1, LOW);
            digitalWrite(motora2, HIGH);
            digitalWrite(motorb1, HIGH);
            digitalWrite(motorb2, LOW);
            analogWrite(apwm, mapeo1 );
            analogWrite(bpwm, mapeo1 );
      }else{
        //CARRO ESTATICO
            digitalWrite(motora1, LOW);
            digitalWrite(motora2, LOW);
            digitalWrite(motorb1, LOW);
            digitalWrite(motorb2, LOW);
      }
    }
    if(y>sup){
        if(inf>x){
          //AVANZA IZQUIERDA
            mapeo1 = map(y, 0,1023 , 0,512);
            mapeo2 = map(x, 0, 1023, 0, 1);
            digitalWrite(motora1, HIGH);
            digitalWrite(motora2, LOW);
            digitalWrite(motorb1, HIGH);
            digitalWrite(motorb2, LOW);
            analogWrite(apwm, mapeo1 );
            analogWrite(bpwm, mapeo1*mapeo2 );
        }
      if(sup<x){
        //AVANZA DERECHA
            mapeo1 = map(y, 0,1023 , 0,512);
            mapeo2 = map(x, 0, 1023, 0, 1);
            digitalWrite(motora1, HIGH);
            digitalWrite(motora2, LOW);
            digitalWrite(motorb1, HIGH);
            digitalWrite(motorb2, LOW);
            analogWrite(apwm, mapeo1*mapeo2 );
            analogWrite(bpwm, mapeo2 );
      }else{
        //AVANZA RECTO 
            mapeo1 = map(y, 0,1023 , 0,512);
            digitalWrite(motora1, HIGH);
            digitalWrite(motora2, LOW);
            digitalWrite(motorb1, HIGH);
            digitalWrite(motorb2, LOW);
            analogWrite(apwm, mapeo1);
      }
    }
    if(y<inf){
        if(inf>x){
          
            mapeo1 = map(y, 0,1023 , 0,512);
            mapeo2 = map(x, 0, 1023, 0, 1);
            digitalWrite(motora1, LOW);
            digitalWrite(motora2, HIGH);
            digitalWrite(motorb1, LOW);
            digitalWrite(motorb2, HIGH);
            analogWrite(apwm, mapeo1*mapeo2 );
            analogWrite(bpwm, mapeo2 );
        }
      if(sup<x){
        //
            mapeo1 = map(y, 0,1023 , 0,512);
            mapeo2 = map(x, 0, 1023, 0, 1);
            digitalWrite(motora1, LOW);
            digitalWrite(motora2, HIGH);
            digitalWrite(motorb1, LOW);
            digitalWrite(motorb2, HIGH);
            analogWrite(apwm, mapeo1 );
            analogWrite(bpwm, mapeo2*mapeo1 );
      }else{
        // 
            mapeo1 = map(y, 0,1023 , 0,512);
            digitalWrite(motora1, LOW);
            digitalWrite(motora2, HIGH);
            digitalWrite(motorb1, LOW);
            digitalWrite(motorb2, HIGH);
            analogWrite(apwm, mapeo1);
      }
    }
}

void splitData(String data, int& x, int& y) {
  // Encontrar la posición de la coma
  int commaIndex = data.indexOf(',');

  // Extraer las subcadenas antes y después de la coma
  String xStr = data.substring(0, commaIndex);
  String yStr = data.substring(commaIndex + 1);

  // Convertir las subcadenas en enteros
  x = xStr.toInt();
  y = yStr.toInt();
}

float medirDistancia(int triggerPin, int echoPin) {
  digitalWrite(triggerPin, HIGH); // Genera el pulso a enviar
  delayMicroseconds(10); // Espera 10 microsegundos
  digitalWrite(triggerPin, LOW); // Termina el pulso

  long duracion = pulseIn(echoPin, HIGH); // Espera el pulso alto en Echo
  float distancia = duracion / 58.2; // Calcula la distancia en centímetros
  
  return distancia; // Devuelve la distancia medida
}