/*
  Calibracao de neutro do servo Parallax de rotacao continua.
  Desacople o servo da garra, carregue o programa e ajuste o
  pequeno potenciometro ate o eixo ficar completamente parado.
  Nao force o potenciometro ate o batente.
*/

#include <Servo.h>

Servo servo;

void setup() {
  servo.attach(9);
  servo.writeMicroseconds(1500);
}

void loop() {
}
