
#define MOTOR_A1 5
#define MOTOR_A2 6
#define MOTOR_B1 10
#define MOTOR_B2 11

void pararMotores() {
  analogWrite(MOTOR_A1, 0);
  analogWrite(MOTOR_A2, 0);
  analogWrite(MOTOR_B1, 0);
  analogWrite(MOTOR_B2, 0);
}

void setup() {
  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);
  pinMode(MOTOR_B1, OUTPUT);
  pinMode(MOTOR_B2, OUTPUT);
}
bool adversarioYN = false;
void loop() {
  busca(adversarioYN);
  ultrasomLet();
}
// função de busca de adversarios
void busca(bool adversario) {
  if (adversario == false) {
    // Os dois lados para frente
    analogWrite(MOTOR_A1, 0);
    analogWrite(MOTOR_A2, 130);
    analogWrite(MOTOR_B1, 130);
    analogWrite(MOTOR_B2, 0);
    delay(2000);

    pararMotores();
    delay(1000);

    // Os dois lados para trás
    analogWrite(MOTOR_A1, 130);
    analogWrite(MOTOR_A2, 0);
    analogWrite(MOTOR_B1, 0);
    analogWrite(MOTOR_B2, 130);
    delay(2000);

    pararMotores();
    delay(1000);
  } else {
    atacar();
  }
}
// velocidade maxima caso encontre adversario
void atacar() {
  analogWrite(MOTOR_A1, 255);
  analogWrite(MOTOR_A2, 0);
  analogWrite(MOTOR_B1, 255);
  analogWrite(MOTOR_B2, 0);
}
// pegar dados sensor ultrasom e comparar com distancias
void ultrasomLet() {
  long medirDistancia() {
    digitalWrite(TRIGGER, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER, LOW);

    long duracao = pulseIn(ECHO, HIGH, 30000); // Timeout de 30ms (~5m max)
    if (duracao == 0) return 400; // Se não houver eco, considera livre

    return duracao * 0.034 / 2; // Converte para centímetros
  }
  long distancia = medirDistancia();

    // Posição do Servo
    if (distancia <= 5) {
      moverServo(90);
    }
    else if (distancia > 7) {
      moverServo(0);
    }

    // Lógica dos Motores
    if (distancia < 10) {
      // Menos de 10 cm: Ataque em Velocidade Máxima
      moverFrente(255);
    }
    else if (distancia <= 30) {
      // Entre 10 cm e 30 cm: Aproximação em Velocidade Reduzida
      moverFrente(180);
    }
}
