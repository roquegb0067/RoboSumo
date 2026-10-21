#define MOTOR_A1 5
#define MOTOR_A2 6
#define MOTOR_B1 10
#define MOTOR_B2 11

const int pinoTrig = 9;
const int pinoEcho = 3;

// Variáveis globais de controle
bool adversarioYN = false;
bool fugaInimigo = false;
bool detec = false;
float distanciaAnterior = 0; // Guardar a leitura anterior para comparar

void setup() {
  Serial.begin(9600); // Serve para mostrar a distância no monitor do PC
  
  pinMode(pinoTrig, OUTPUT);
  pinMode(pinoEcho, INPUT);
  
  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);
  pinMode(MOTOR_B1, OUTPUT);
  pinMode(MOTOR_B2, OUTPUT);
}

void loop() {
  busca(adversarioYN);
  ultrasomVal();
  atividadeOponente(); // Chamando a função de estratégia
}

void pararMotores() {
  analogWrite(MOTOR_A1, 0);
  analogWrite(MOTOR_A2, 0);
  analogWrite(MOTOR_B1, 0);
  analogWrite(MOTOR_B2, 0);
}

void busca(bool adversario) {
  if (adversario == false) {
    // Direção de busca 1
    analogWrite(MOTOR_A1, 0);
    analogWrite(MOTOR_A2, 130);
    analogWrite(MOTOR_B1, 130);
    analogWrite(MOTOR_B2, 0);
    delay(2000);

    pararMotores();
    delay(1000);

    // Direção de busca 2
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

void atacar() {
  analogWrite(MOTOR_A1, 255);
  analogWrite(MOTOR_A2, 0);
  analogWrite(MOTOR_B1, 255);
  analogWrite(MOTOR_B2, 0);
}

float ultrasomVal() {
  digitalWrite(pinoTrig, LOW);
  delayMicroseconds(2);

  digitalWrite(pinoTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinoTrig, LOW);

  long duracao = pulseIn(pinoEcho, HIGH);
  float distancia = duracao * 0.034 / 2;

  // Atualiza a variável global adversarioYN com base no sensor
  Berserker(distancia);

  // Exibe no Monitor Serial
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  delay(100); 
  return distancia;
}

void Berserker(float distancia){
  if (distancia <= 5) {
      // moverServo(90); // Descomente se for usar o servo
    }
    else if (distancia > 7) {
      // moverServo(0);
    }
    
    if (distancia < 10) {
      adversarioYN = true;
      detec = true;
    }
    else if (distancia <= 30 && distancia >= 10) {
      andar(180);
    }
}

void andar(int vel){
  analogWrite(MOTOR_A1, vel);
  analogWrite(MOTOR_A2, 0);
  analogWrite(MOTOR_B1, vel);
  analogWrite(MOTOR_B2, 0);
}

// Função de armadilha: Detecta se chegou perto e agora está recuando
void atividadeOponente(){
  float distanciaAtual = ultrasomValLeituraApenas(); // Função auxiliar para pegar a distância limpa
  
  // Se o inimigo estava muito perto (ex: menos de 15cm) e agora a distância começou a aumentar (ele está voltando/recuando)
  if (distanciaAtual < 15) {
    fugaInimigo = true; // Marcamos que ele esteve colado
  }

  // Se ele esteve perto e a distância agora subiu (foi para mais de 20cm, por exemplo), dá o bote!
  if (fugaInimigo == true && distanciaAtual > 20 && distanciaAtual > distanciaAnterior) {
    atacar();
    delay(500); // Dá um gás no ataque
    fugaInimigo = false; // Reseta a armadilha
  }

  distanciaAnterior = distanciaAtual; // Salva para o próximo ciclo
}

// Função auxiliar para evitar disparar o ultrassom duplicado no loop
float ultrasomValLeituraApenas() {
  digitalWrite(pinoTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinoTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinoTrig, LOW);
  long duracao = pulseIn(pinoEcho, HIGH, 30000); // Timeout para travar menos
  if (duracao == 0) return 50; // Se não achar nada, retorna longe
  return duracao * 0.034 / 2;
}
