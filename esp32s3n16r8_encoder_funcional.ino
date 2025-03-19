//#include <Arduino.h>

#define ENCODER_PIN_A 4   // Pino para canal A do encoder
//leve en consideração a tensão de alimentação do encoder o minimo é 5v pode não ser estavel
#define ENCODER_PIN_B 5   // Pino para canal B do encoder

volatile int encoderValue = 0;  // Variável para armazenar o valor do encoder
volatile bool aLastState;       // Variável para armazenar o último estado do pino A

// Função de interrupção para ler o encoder
void IRAM_ATTR readEncoder() {
  bool aState = digitalRead(ENCODER_PIN_A);  // Lê o estado atual do pino A
  bool bState = digitalRead(ENCODER_PIN_B);  // Lê o estado atual do pino B

  // Verifica se o estado do pino A mudou
  if (aState != aLastState) {
    // Se o estado do pino B for diferente do pino A, incrementa o valor do encoder
    if (bState != aState) {
      encoderValue++;
    } else {
      // Caso contrário, decrementa o valor do encoder
      encoderValue--;
    }
  }
  // Atualiza o último estado do pino A
  aLastState = aState;
}

void setup() {
  Serial.begin(115200);  // Inicia a comunicação serial

  // Configura os pinos do encoder como entradas com pull-up interno
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);

  // Configura a interrupção no pino A do encoder
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), readEncoder, CHANGE);

  // Lê o estado inicial do pino A
  aLastState = digitalRead(ENCODER_PIN_A);

  Serial.println("Encoder pronto para uso.");
}

void loop() {
  // Exibe o valor do encoder no monitor serial
  Serial.print("Posição do encoder: ");
  Serial.println(encoderValue);
  delay(100);  // Pequeno delay para evitar sobrecarregar o monitor serial
}