#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <FastLED.h>
#include <StreamUtils.h>

#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#define TxD 13
#define RxD 12
const int READ_TIME = 500;
String input = "";

#define NUM_LEDS    16
CRGB leds[NUM_LEDS];
SoftwareSerial BTSerial(TxD, RxD);

DynamicJsonDocument doc(2048);
JsonVariant variant_doc;
JsonVariant variant_doc2;
//JsonVariant variant_doc3;
// Json variant olusturmayi dene
//DynamicJsonDocument doc_recursive(1024);
//String Json_bir_string = "";



void setup()
{
  BTSerial.begin(9600);
  Serial.begin(9600);
}

void loop()
{
  recvOneChar();

}

void recvOneChar() {

  // while incoming signal is available.
  if (BTSerial.available()) {
    unsigned long readStart = millis();
    while (BTSerial.available() || millis() - readStart < READ_TIME) {
      if (BTSerial.available()) {
        input = BTSerial.readStringUntil('\n');

        delay(1);

      }
    }
    Serial.println(F("input:"));
    Serial.println(input);
    to_Json(input);
  }
}

void to_Json(String Json_BT_Data)
{
  //doc1.clear();

  Serial.println(F("gelen inputu json formatina donusturuyoruz:"));
  DeserializationError error = deserializeJson(doc, Json_BT_Data);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  } // alıp Json formatına dönüştürüyor


  //Serial.println("S nin tipi");
  //  Serial.println(doc["s"].is<JsonArray>()); // "s" nin tipini kontrol ediyoruz. JsonArray
  //  Serial.println(doc["t"].is<String>());    //"t"  nin tipini kontrol ediyoruz. String
  if (doc["t"] == "r")
  {
    String Json_string = doc["s"].as<String>(); //Yeni input olarak 

    execute(Json_string);
  }


}

void execute(String Json_input) {
  Serial.println("recursive...");
  //doc_recursive.clear();
  doc.clear();

  deserializeJson(doc, Json_input);
  variant_doc = doc.as<JsonVariant>();

  //  Serial.println(variant_doc.size());
  //  Serial.println("variant veri: ");
  //  Serial.println(variant_doc.as<String>());



  for (int i = 0; i <= variant_doc.size(); i++) {

    if ((variant_doc[i]["t"]) == "start")
    {
      // Serial.println("t=start durumu");
      continue;
    }
    if ((variant_doc[i]["t"]) == "fl")
    {
      // Serial.println(F("t=fl durumu"));
      variant_doc2 = variant_doc[i]["s"].as<JsonVariant>();
      // String String_var2 = variant_doc2.as<String>();
      // Serial.println(F("var s value"));
      // Serial.println(String_var2);
      forever_loop();
    }
    if ((variant_doc[i]["t"]) == "end")
    {
      // Serial.println(F("t=end durumu"));
    }
  }
}

void forever_loop() {
  Serial.println(F("forever function..."));

  // Serial.println(variant_doc2.size());
  for (int i = 0; i < variant_doc2.size(); i++) {
    if ((variant_doc2[i]["t"]) == "b")
    {
      Serial.println(F("t=brunch durumu"));
      Serial.print(i);
      Serial.println(F(". branch"));
      Serial.println(variant_doc2[i].as<String>());
      conditions_function(i);
      if_functions(i);
      else_functions(i);
    }
  }

  // variant_doc2 = variant_doc2[i]["s"].as<JsonVariant>();
  // String String_var2 = variant_doc2.as<String>();
  // Serial.println(F("var s value"));
  // Serial.println(String_var2);



}

void conditions_function(int condition_number) {
  Serial.println(F("c fonksiyonu"));
  Serial.println(variant_doc2[condition_number]["c"].as<String>());
  if(variant_doc2[condition_number]["c"]["t"] == "ds")
  {
    Serial.println(F("distance sensor"));
    distance_sensor(variant_doc2[condition_number]["c"]["o"].as<String>(), variant_doc2[condition_number]["c"]["v"].as<String>(), variant_doc2[condition_number]["c"]["p"].as<String>());
  }
  
  //Serial.println (doc_recursive.size());
  //Serial.println F(("conditions gelen veri: "));
  //Serial.println (doc_recursive.as<String>());
}


void if_functions(int if_number) {
  Serial.println(F("f fonksiyonu"));
  Serial.println(variant_doc2[if_number]["f"].as<String>());
}


void else_functions(int else_number) {
  Serial.println(F("e fonksiyonu"));
  Serial.println(variant_doc2[else_number]["e"].as<String>());
}

void distance_sensor(String operation, String Port, String value){
Serial.println(F("distance..."));
Serial.println(operation);
Serial.println(Port);
Serial.println(value);

}
