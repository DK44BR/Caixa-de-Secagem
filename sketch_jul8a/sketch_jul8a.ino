#include <DHT.h>

#define SENSOR_DE_TEMP_ATIVO 0

#define INTERVALO_VENTILADORES_DESATIVADO 30 * 1000 * 60 // 30 minutos
#define INTERVALO_VENTILADORES_ATIVADO    15 * 1000      // 15 segundos

#define PIN_SENSOR_DE_TEMPERATURA 33
#define TIPO_DHT DHT11

enum EstadoVentilador { VENTILADOR_LIGADO, VENTILADOR_DESLIGADO };

typedef struct ControleVentilador {
  EstadoVentilador estado;
  int tempo_ate_ativar;
  int tempo_ate_desativar;
} ControleVentilador;

ControleVentilador controle_ventilador;

DHT dht(PIN_SENSOR_DE_TEMPERATURA, TIPO_DHT);

void setup() {
  Serial.begin(115200);

  Serial.println("======= Caixa de Secagem - Firmware v1.0 =======");

  Serial.println("[INFO] Inicializando DHT.");
  dht.begin();
  delay(2000); // dá tempo para o sensor inicializar
}

void loop() {
  if(SENSOR_DE_TEMP_ATIVO) {
    Serial.println("[DEBUG] Lendo temperatura do sensor DHT.");
    float temperatura = dht.readTemperature();
  
    if (isnan(temperatura)) {
      Serial.println("[ERROR] Erro ao ler o sensor de temperatura. Verifique se tem mal contato.");
    } else {
      Serial.print("[Info] Leitura feita com sucesso! Temperatura: ");
      Serial.print(temperatura);
      Serial.println(" °C");
    }
  }

  if(controle_ventilador.estado == VENTILADOR_LIGADO) {
    Serial.print("[INFO] Ventilador ativado. Tempo ate desativar: ");
    Serial.print(controle_ventilador.tempo_ate_desativar/1000);
    Serial.print("/");
    Serial.print(INTERVALO_VENTILADORES_ATIVADO/1000);
    Serial.println(" segundos.");

    if(controle_ventilador.tempo_ate_desativar >= INTERVALO_VENTILADORES_ATIVADO) {
      controle_ventilador.tempo_ate_desativar = 0;
      controle_ventilador.tempo_ate_ativar = 0;
      controle_ventilador.estado = VENTILADOR_DESLIGADO;

      Serial.println("[INFO] Ventiladores desligados.");
    } else {
      controle_ventilador.tempo_ate_desativar += 1000;
    }
    
  }

  if(controle_ventilador.estado == VENTILADOR_DESLIGADO) {
    Serial.print("[INFO] Ventilador desativado. Tempo ateé ativar: ");
    Serial.print(controle_ventilador.tempo_ate_ativar/1000);
    Serial.print("/");
    Serial.print(INTERVALO_VENTILADORES_DESATIVADO/1000);
    Serial.println(" segundos.");

    if(controle_ventilador.tempo_ate_ativar >= INTERVALO_VENTILADORES_DESATIVADO) {
      controle_ventilador.tempo_ate_desativar = 0;
      controle_ventilador.tempo_ate_ativar = 0;
      controle_ventilador.estado = VENTILADOR_LIGADO;

      Serial.println("[INFO] Ventiladores ligados.");
    } else {
      controle_ventilador.tempo_ate_ativar += 1000;
    }
  }

  delay(1000); // mais seguro para o DHT11
}
