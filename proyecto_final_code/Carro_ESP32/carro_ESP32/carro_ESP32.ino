#include <BluetoothSerial.h> 

BluetoothSerial BT;

int motora1 = 23;
int motora2 = 22;
int motorb1 = 1;
int motorb2 = 3; 
int apwm = 21;
int bpwm = 19;
int sensor = 36;
float sen_in;
int a = 5;
int inf = 494-a;
int sup = 494+a;
float mapeo1;
float mapeo2;
int x;
int y;
String data;

int triggerPin = 10; // Cambiar valor
int echoPin = 11;    // Cambiar valor

void setup() {
    BT.begin("carro");
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(motora1, OUTPUT);
    pinMode(motora2, OUTPUT);
    pinMode(motorb1, OUTPUT);
    pinMode(motorb2, OUTPUT);
    pinMode(apwm, OUTPUT);
    pinMode(bpwm, OUTPUT);
    pinMode(sensor, INPUT);
}

void loop() {
    if (BT.available()) { // Si llega un dato por el puerto BT se envía al monitor serial
        data = BT.readString();
        splitData(data, x, y);
        fun_h(x, y);
    }
    sen_in = readUltrasonic();
    BT.println(sen_in);
}

void fun_h(int x, int y) {
    // Respuesta del punto H en función de la entrada de joystick
    if (y >= inf && y <= sup) {
        if (inf > x) {
            // GIRO ESTATICO IZQUIERDA
            mapeo1 = map(y, 0, 1023, 0, 512);
            digitalWrite(motora1, HIGH);
            digitalWrite(motora2, LOW);
            digitalWrite(motorb1, LOW);
            digitalWrite(motorb2, HIGH);
            analogWrite(apwm, mapeo1);
            analogWrite(bpwm, mapeo1);
        } else if (sup < x) {
            // GIRO ESTATICO DERECHA
            mapeo1 = map(y, 0, 1023, 0, 512);
            digitalWrite(motora1, LOW);
            digitalWrite(motora2, HIGH);
            digitalWrite(motorb1, HIGH);
            digitalWrite(motorb2, LOW);
            analogWrite(apwm, mapeo1);
            analogWrite(bpwm, mapeo1);
        } else {
            // CARRO ESTATICO
            digitalWrite(motora1, LOW);
            digitalWrite(motora2, LOW);
            digitalWrite(motorb1, LOW);
            digitalWrite(motorb2, LOW);
        }
    } else if (y > sup) {
        if (inf > x) {
            // AVANZA IZQUIERDA
            mapeo1 = map(y, 0, 1023, 0, 512);
            mapeo2 = map(x, 0, 1023, 0, 1);
            digitalWrite(motora1, HIGH);
            digitalWrite(motora2, LOW);
            digitalWrite(motorb1, HIGH);
            digitalWrite(motorb2, LOW);
            analogWrite(apwm, mapeo1);
            analogWrite(bpwm, mapeo1 * mapeo2);
        } else if (sup < x) {
            // AVANZA DERECHA
            mapeo1 = map(y, 0, 1023, 0, 512);
            mapeo2 = map(x, 0, 1023, 0, 1);
            digitalWrite(motora1, HIGH);
            digitalWrite(motora2, LOW);
            digitalWrite(motorb1, HIGH);
            digitalWrite(motorb2, LOW);
            analogWrite(apwm, mapeo1 * mapeo2);
            analogWrite(bpwm, mapeo2);
        } else {
            // AVANZA RECTO
            mapeo1 = map(y, 0, 1023, 0, 512);
            digitalWrite(motora1, HIGH);
            digitalWrite(motora2, LOW);
            digitalWrite(motorb1, HIGH);
            digitalWrite(motorb2, LOW);
            analogWrite(apwm, mapeo1);
        }
    } else if (y < inf) {
        if (inf > x) {
            // RETROCEDE IZQUIERDA
            mapeo1 = map(y, 0, 1023, 0, 512);
            mapeo2 = map(x, 0, 1023, 0, 1);
            digitalWrite(motora1, LOW);
            digitalWrite(motora2, HIGH);
            digitalWrite(motorb1, LOW);
            digitalWrite(motorb2, HIGH);
            analogWrite(apwm, mapeo1 * mapeo2);
            analogWrite(bpwm, mapeo2);
        } else if (sup < x) {
            // RETROCEDE DERECHA
            mapeo1 = map(y, 0, 1023, 0, 512);
            mapeo2 = map(x, 0, 1023, 0, 1);
            digitalWrite(motora1, LOW);
            digitalWrite(motora2, HIGH);
            digitalWrite(motorb1, LOW);
            digitalWrite(motorb2, HIGH);
            analogWrite(apwm, mapeo1);
            analogWrite(bpwm, mapeo2 * mapeo1);
        } else {
            // RETROCEDE RECTO
            mapeo1 = map(y, 0, 1023, 0, 512);
            digitalWrite(motora1, LOW);
            digitalWrite(motora2, HIGH);
            digitalWrite(motorb1, LOW);
            digitalWrite(motorb2, HIGH);
            analogWrite(apwm, mapeo1);
        }
    }
}

void splitData(String data, int &x, int &y) {
    int commaIndex = data.indexOf(',');
    String xStr = data.substring(0, commaIndex);
    String yStr = data.substring(commaIndex + 1);
    x = xStr.toInt();
    y = yStr.toInt();
}

float readUltrasonic() {
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    float distance = duration / 58.2;

    return distance;
}
