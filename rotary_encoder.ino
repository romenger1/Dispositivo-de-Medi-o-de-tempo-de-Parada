#include <Arduino.h>
#include <ArduinoJson.h>

#define ENCODER_PIN_A 18
#define ENCODER_PIN_B 19
#define ENCODER_SWITCH_PIN 17
#define ZERO_BUTTON_PIN 16

volatile int encoderValue = 0;
volatile bool aLastState;
volatile bool bLastState;

bool medindo = false;
unsigned long tempoInicio = 0;

void IRAM_ATTR readEncoder() {
  bool aState = digitalRead(ENCODER_PIN_A);
  bool bState = digitalRead(ENCODER_PIN_B);

  if (aState != aLastState) {
    if (bState != aState) {
      encoderValue++;
    } else {
      encoderValue--;
    }
  }
  aLastState = aState;
  bLastState = bState;
}

void setup() {
  Serial.begin(115200);
  
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);
  pinMode(ENCODER_SWITCH_PIN, INPUT_PULLUP);
  pinMode(ZERO_BUTTON_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), readEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), readEncoder, CHANGE);

  aLastState = digitalRead(ENCODER_PIN_A);
  bLastState = digitalRead(ENCODER_PIN_B);

  Serial.println("Pressione o botão de zeramento para zerar a leitura...");
  while (digitalRead(ZERO_BUTTON_PIN) == HIGH) {
    delay(10);
  }
  encoderValue = 0;
  Serial.println("Leitura do encoder zerada.");
}

void loop() {
  if (digitalRead(ZERO_BUTTON_PIN) == LOW) {
    encoderValue = 0;
    Serial.println("Leitura do encoder zerada.");
    delay(200);
  }

  if (digitalRead(ENCODER_SWITCH_PIN) == LOW && !medindo) {
    Serial.println("Medição iniciada...");
    medindo = true;
    encoderValue = 0;
    tempoInicio = millis();  
    delay(200);
  }

  if (medindo) {
    unsigned long tempoAtual = millis() - tempoInicio;
    float deslocamento = (encoderValue / 512.0) * 100.0;
    float velocidade = (deslocamento / (tempoAtual / 1000.0));

    StaticJsonDocument<128> jsonDoc;
    jsonDoc["tempo_ms"] = tempoAtual;
    jsonDoc["posicao"] = encoderValue;
    jsonDoc["deslocamento_mm"] = deslocamento;
    jsonDoc["velocidade_mm_s"] = velocidade;

    String jsonString;
    serializeJson(jsonDoc, jsonString);
    Serial.println(jsonString);
  }

  delay(10);
}