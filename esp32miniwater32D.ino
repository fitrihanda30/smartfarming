#include "library.h"
#include "secret.h"

char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASS;

// Sensor DHT11
#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// BH1750
BH1750 lightMeter;

// MQ135
MQ135 GasSensor = MQ135(34);

// Soil Sensor
#define SoilSensorPin 35

// Mini Water Pump
#define RelayWater 25
#define RelayFertilizer 26

// Ultrasonic
#define EchoFertilizer 32
#define TriggerFertilizer 33

#define EchoWater 16
#define TriggerWater 17

// Soil Temperature
#define TempPin 27
OneWire oneWire(TempPin);
DallasTemperature DS18B20(&oneWire);

// LCD Button
#define ButtonPin 4
LiquidCrystal_I2C lcd(0x27, 16,2);

// Buzzer
#define BuzzerPin 15

// Thinger
#define THINGER_SERIAL_DEBUG

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

#include "data.h"
#include "function.h"
#include "main.h"

void setup()
{
  Serial.begin(9600);

  lcd.begin();
  lcd.backlight();
  lcd.print("Setup..");

  Serial.println("============================================");
  Serial.println("Smart Farming - Setup");
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  Serial.println("============================================");

  thing.add_wifi(ssid, pass);
  lcd.setCursor(0,1);
  lcd.print("Wifi: ");
  lcd.print(ssid);

  //Sensor DHT
  dht.begin();      

  //BH1750
  Wire.begin();
  lightMeter.begin();
  Serial.println(F("BH1750 Test begin"));
  
  //Mini Water Pump
  pinMode(RelayWater, OUTPUT);
  pinMode(RelayFertilizer, OUTPUT);

  //Ultrasonic
  pinMode(EchoFertilizer, INPUT);
  pinMode(TriggerFertilizer, OUTPUT);

  pinMode(EchoWater, INPUT);
  pinMode(TriggerWater, OUTPUT);

  //Button
  pinMode(ButtonPin, INPUT_PULLUP);

  //Buzzer
  pinMode(BuzzerPin, OUTPUT);

  //Soil Temperature
  DS18B20.begin();

  thing["Data"] >> [] (pson & out)
  {
    out ["AirHumidity"] = humidity;
    out ["AirTemperature"] = temperature;
    out ["Light"] = lux;
    out ["GasQuality"] = gasSensorPPM;
    out ["SoilHumidity"] = soilSensorValue;
    out ["SoilTemperature"] = soilTemperatureC;
    out ["AvailableWater"] = availableWater;
    out ["AvailableFertilizer"] = availableFertilizer;
  };

  thing.handle();

  getThingerProperty(false);
  setThingerProperty();

  lcd.clear();
  lcd.print("Setting Done!");
}

void loop()
{
  thing.handle();
  getThingerProperty();
  
  mainProgram();
}
