#include <DHT.h>

#define PIN_SENSOR_DE_TEMPERATURA 33
#define TIPO_DHT DHT11

DHT dht(PIN_SENSOR_DE_TEMPERATURA, TIPO_DHT);

void setup() {
  Serial.begin(115200);
  dht.begin();
  delay(2000); // dá tempo para o sensor inicializar
}

void loop() {
  float temperatura = dht.readTemperature();

  if (isnan(temperatura)) {
    Serial.println("Erro ao ler o sensor de temperatura.");
  } else {
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" °C");
  }

  delay(2000); // mais seguro para o DHT11
}
