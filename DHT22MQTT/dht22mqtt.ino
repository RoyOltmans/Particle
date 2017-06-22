#include "Adafruit_DHT/Adafruit_DHT.h"
//DHT module loading
#define DHTPIN 1     // what pin we're connected to
// Uncomment whatever type you're using!
#define DHTTYPE DHT22		// DHT 22 
DHT dht(DHTPIN, DHTTYPE);

#include "MQTT/MQTT.h"
void callback(char* topic, byte* payload, unsigned int length);
/**
 * if want to use IP address,
 * 
 * byte server[] = { XXX,XXX,XXX,XXX };
 * MQTT client(server, 1883, callback);
 * want to use domain name,
 * MQTT client("www.sample.com", 1883, callback);
 **/
byte server[] = { 192,168,2,1 };
MQTT client(server, 1883, callback);

// Logging level
#define DEBUGLOG true
#define INFOLOG true

//Declare variables and settings
int led4 = 4;
int led0 = 0;
int intDelayMeasurements = 300000;
int intCycle = 0;
int intRetryHTTP = 10;
int intRetrySensor = 5;
float fltData = 0.000000;
String strPubRoot = "home/particle/";
String strPubDevice = "sparkcore_1/";

void setup() {
    Serial.begin(9600);
    pinMode(led4, OUTPUT);
    pinMode(led0, OUTPUT);
    loadDHTmodule();
    resolveDNS("www.google.com");
}

void resolveDNS(char *hostname) {
    Particle.publish("DNS qeury ", "Initializing trying to resolve hostname " + String(hostname), 60, PRIVATE); // See https://console.particle.io/logs 
    HAL_IPAddress ip_addr;
    network_interface_t t;
    unsigned long tic = millis();
    inet_gethostbyname(hostname, strlen(hostname), &ip_addr, t, &t);
    unsigned long toc = millis();
    IPAddress resolvedIP(ip_addr);
    Particle.publish("DNS qeury ", String(hostname) + " resolved in ip " + String(resolvedIP), 60, PRIVATE); // See https://console.particle.io/logs
    Particle.publish("DNS qeury ", "resolve timing "+ String(tic) + "-" + String(toc), 60, PRIVATE); // See https://console.particle.io/logs
}

void mqttPublish(const char *chrTopic, const char *chrMessage, int intQos) {
    // MQTT PUB/SUB
    if (!client.isConnected()) {
        client.connect("sparkclient"); // connect to the server for MQTT
    }
    if (client.isConnected()) {
        Particle.publish("MQTT ", "Host Connection: Established", 60, PRIVATE); // See https://console.particle.io/logs
        client.publish(String(chrTopic),String(chrMessage));
        Particle.publish("MQTT ", "Message published " + String(chrMessage), 60, PRIVATE); // See https://console.particle.io/logs
        // client.subscribe("inTopic/message");
        client.disconnect();
        Particle.publish("MQTT ", "Host Connection: Disconnected", 60, PRIVATE); // See https://console.particle.io/logs
    } 
    else {
        Particle.publish("MQTT ", "Host Connection: Failed", 60, PRIVATE); // See https://console.particle.io/logs
    }
    
}

void loadDHTmodule() {
    Particle.publish("Startup", "DHT Loading", 60, PRIVATE); // See https://console.particle.io/logs
    dht.begin();
    delay(25000);
    digitalWrite(led0, HIGH);   // turn the LED on (HIGH is the voltage level)
    Particle.publish("Startup", "DHT Loaded", 60, PRIVATE); // See https://console.particle.io/logs
}

int getSensor(int intType) {
    float fltValue = 0.000000;
    intCycle = 0;
    while(fltValue==0.000000&&intCycle!=intRetrySensor) {
	    intCycle++;
        if(intType==0){
        	fltValue = dht.getTempCelcius(); //retry value 0.0
    	    Particle.publish("Reading Sensor: ", "Temperature", 60, PRIVATE); // See https://console.particle.io/logs
        } else if(intType==1) {
            fltValue = dht.getHumidity(); //retry value 0.0
            Particle.publish("Reading Sensor: ", "Humidity", 60, PRIVATE); // See https://console.particle.io/logs
        };
        delay(2000);
        // Reading temperature or humidity takes about 250 milliseconds!
        // Sensor readings may also be up to 2 seconds 'old' (its a 
        // very slow sensor)
        // Read temperature as Celsius
    };
    Particle.publish("Sensor value: ", String(fltValue), 60, PRIVATE); // See https://console.particle.io/logs
    return fltValue;
}

void loop() {
    digitalWrite(led4, HIGH);   // turn the LED on (HIGH is the voltage level)
    fltData = getSensor(0); // Get sensor data from temp DHT22
    if(fltData!=0.0) {
	   mqttPublish(strPubRoot + strPubDevice + "temperature", String(fltData), 0);
    };
	fltData = getSensor(1); // Get sensor data from humidity DHT22
    if(fltData!=0.0) {
       mqttPublish(strPubRoot + strPubDevice + "humidity", String(fltData), 0);
    };
    digitalWrite(led4, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(intDelayMeasurements); //X minute timeout
}