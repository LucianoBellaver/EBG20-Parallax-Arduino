/*
  EBG-20 + Parallax Continuous Rotation Servo
  Controle pelo Monitor Serial (9600 baud)

  Projeto mecanico original: ToolBoxRobotics
  https://www.thingiverse.com/thing:6460227

  A = fechar | D = abrir | C = demonstracao
  P = parar  | Z = confirmar posicao inicial aberta

  ATENCAO: controle por tempo; sem sensor de posicao ou fim de curso.
  Ajustes do exemplo sao especificos de uma montagem. Ajuste antes de usar.
*/

#include <Servo.h>

Servo garra;

const int PINO = 9;

// Parallax 360
const int PARADO = 1500;

// Velocidade normal
const int FECHAR = 1450;
const int ABRIR = 1550;

// Velocidade lenta
const int FECHAR_LENTO = 1470;
const int ABRIR_LENTO = 1530;

// Tempos iniciais em milissegundos
const int TEMPO_FECHAR = 1100;
const int TEMPO_ABRIR = 1200;

const int TEMPO_FECHAR_LENTO = 1500;
const int TEMPO_ABRIR_LENTO = 1650;

// 0 = aberta, 1 = fechada
int posicao = 0;
bool posicaoConhecida = true;

// Movimento continuo
bool mover(int pulso, int tempo) {
  garra.writeMicroseconds(pulso);

  unsigned long inicio = millis();

  while (millis() - inicio < (unsigned long)tempo) {
    if (Serial.available()) {
      char comando = Serial.read();

      if (comando == 'P' || comando == 'p') {
        garra.writeMicroseconds(PARADO);
        posicaoConhecida = false;

        Serial.println("PARADA!");
        Serial.println("Posicao desconhecida.");
        return false;
      }
    }
    delay(2);
  }

  garra.writeMicroseconds(PARADO);
  return true;
}

// Fechar completamente
bool fechar(bool lento = false) {
  if (posicao == 1) return true;

  Serial.println("Fechando...");

  bool ok = lento
    ? mover(FECHAR_LENTO, TEMPO_FECHAR_LENTO)
    : mover(FECHAR, TEMPO_FECHAR);

  if (ok) posicao = 1;
  return ok;
}

// Abrir completamente
bool abrir(bool lento = false) {
  if (posicao == 0) return true;

  Serial.println("Abrindo...");

  bool ok = lento
    ? mover(ABRIR_LENTO, TEMPO_ABRIR_LENTO)
    : mover(ABRIR, TEMPO_ABRIR);

  if (ok) posicao = 0;
  return ok;
}

// Demonstracao
void demonstracao() {
  Serial.println("=== DEMONSTRACAO ===");

  Serial.println("1. Fechando rapido");
  if (!fechar()) return;
  delay(500);

  Serial.println("2. Abrindo rapido");
  if (!abrir()) return;
  delay(500);

  Serial.println("3. Fechando lento");
  if (!fechar(true)) return;
  delay(500);

  Serial.println("4. Abrindo lento");
  if (!abrir(true)) return;

  Serial.println("Demonstracao finalizada!");
}

void setup() {
  Serial.begin(9600);

  garra.writeMicroseconds(PARADO);
  garra.attach(PINO);

  Serial.println("=== GARRA EBG-20 ===");
  Serial.println("A = Fechar");
  Serial.println("D = Abrir");
  Serial.println("C = Demonstracao");
  Serial.println("P = Parar");
  Serial.println("Z = Confirmar aberta");
}

void loop() {
  if (!Serial.available()) return;

  char comando = Serial.read();

  if (comando == 'Z' || comando == 'z') {
    garra.writeMicroseconds(PARADO);
    posicao = 0;
    posicaoConhecida = true;
    Serial.println("Posicao zero confirmada.");
    return;
  }

  if (!posicaoConhecida) {
    Serial.println("Posicao desconhecida!");
    Serial.println("Reposicione a garra aberta e envie Z.");
    return;
  }

  switch (comando) {
    case 'A':
    case 'a':
      fechar();
      break;

    case 'D':
    case 'd':
      abrir();
      break;

    case 'C':
    case 'c':
      demonstracao();
      break;

    case 'P':
    case 'p':
      garra.writeMicroseconds(PARADO);
      break;
  }
}
