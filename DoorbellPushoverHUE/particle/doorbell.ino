// This #include statement was automatically added by the Particle IDE.
#include <SparkTime.h>

/**
 * Created by Roy Oltmans
 * Date 2016-01-25
 * 
 * First version was based on the setup of Jurjen de Vries
 * 
 * Release 0.2 Uses HTTP post API call
 * Release 0.1 Uses JSON webhook trough particle.io (unreliable)
 * 
 **/

#define LIB_DOMAIN "pushover PHP page ea pushover.somesite.com/pushoverhue"
#define TOKEN "246809864334680" // change this token

TCPClient client;

int pinDoorbutton = DAC;
int TimerPushMsgEpoc = 0;
int timeNow = 0;
int vanTijd = 2000;
int totTijd = 600;
int millDelay = 5000;
int ringTime = 500;
int relay_0 = D0;
int relay_1 = D1;
int relay_2 = D2;

void setup() {
    pinMode(pinDoorbutton, INPUT_PULLUP);
	pinMode(relay_0, OUTPUT);
    pinMode(relay_1, OUTPUT);
    pinMode(relay_2, OUTPUT);
    digitalWrite(relay_0, LOW);
    digitalWrite(relay_1, LOW);
    digitalWrite(relay_2, LOW);
    RGB.control(true);
    Time.zone(1);
    TimerPushMsgEpoc = Time.now();
}
 
void loop() {
    if(digitalRead(pinDoorbutton)==LOW) {
        dingdong();
        //while (digitalRead(pinDoorbutton) == LOW);
    }
    else {
        RGB.color(255, 0, 0);
    }
}

void pushover() {
    client.connect(LIB_DOMAIN, 80);
    client.println("POST / HTTP/1.0");
    client.println("Host: " LIB_DOMAIN);
    client.println("User-Agent: Photon");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(strlen(TOKEN)+4);
    client.println();
    client.print("key=");
    client.print(TOKEN);
    delay(100);
    client.flush();
    delay(100);
    client.stop();
    delay(250);
}
 
void dingdong() {
    timeNow = Time.format(Time.now(),"%H%M").toInt();
    if (timeNow <= vanTijd && timeNow >= totTijd) {
        digitalWrite(relay_0, HIGH);
    };
    if ((Time.now() - TimerPushMsgEpoc) >= 15) {
        pushover();
        //Spark.publish("Doorbell", "Deurbel Ring Ring - " + String(Time.format(Time.now(), "aangebeld om: %H:%M ")), 60, PRIVATE);
        TimerPushMsgEpoc = Time.now();
    }
    RGB.color(0, 255, 0);
    delay(ringTime); //Time that the bell rings
    digitalWrite(relay_0, LOW);
    RGB.color(255, 0, 0);
    delay(millDelay);
}