/*
  Teste de movimento curto para descobrir o sentido e contar o curso.
  Monitor Serial: 9600 baud
  A = pulso de 100 ms para fechar (1450 us na montagem testada)
  D = pulso de 100 ms para abrir  (1550 us na montagem testada)
  P = parar

  Cada comando e um pulso, NAO uma medida precisa de angulo ou dente.
  Pare antes de tocar os batentes mecanicos!
*/

#include <Servo.h>

Servo servo;

const int PINO_SERVO = 9;
const int PARADO = 1500;
const int SENTIDO_1 = 1450;
const int SENTIDO_2 = 1550;
const int TEMPO_MOVIMENTO = 100;

void mover(int sentido) {
  servo.writeMicroseconds(sentido);
  delay(TEMPO_MOVIMENTO);
  servo.writeMicroseconds(PARADO);
  delay(200);
}

void setup() {
  Serial.begin(9600);
  servo.attach(PINO_SERVO);
  servo.writeMicroseconds(PARADO);

  Serial.println("=== TESTE DE PASSOS EBG-20 ===");
  Serial.println("A = sentido 1 (fechar na montagem testada)");
  Serial.println("D = sentido 2 (abrir na montagem testada)");
  Serial.println("P = parar");
}

void loop() {
  if (!Serial.available()) return;

  char comando = Serial.read();

  switch (comando) {
    case 'A': case 'a':
      mover(SENTIDO_1);
      Serial.println("Passo sentido 1");
      break;

    case 'D': case 'd':
      mover(SENTIDO_2);
      Serial.println("Passo sentido 2");
      break;

    case 'P': case 'p':
      servo.writeMicroseconds(PARADO);
      Serial.println("Parado");
      break;
  }
}
