#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// botoes
#define BTN_START 5
#define BTN_A     26
#define BTN_B     27
#define BTN_C     12
#define BTN_D     13

// leds
#define LED_VERDE    32
#define LED_VERMELHO 25

// buzzer
#define BUZZER 19

// LCD I2C
#define LCD_SDA 22
#define LCD_SCL 23

// estados do jogo
#define ESTADO_ESPERA 0
#define ESTADO_JOGO   1

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ═══════════════════════════════
// VARIAVEIS GLOBAIS
// ═══════════════════════════════
struct Pergunta {
  const char* texto;
  const char* opcA;
  const char* opcB;
  const char* opcC;
  const char* opcD;
  char resposta;
};

Pergunta perguntas[8] = {
  {"Capital de PT?",  "Lisboa",  "Porto",   "Funchal", "Faro",   'A'},
  {"Ossos corpo?",    "198",     "206",     "215",     "223",    'B'},
  {"Cor olhos?",      "Azul",    "Verde",   "Casta.",  "Cinza",  'C'},
  {"Quanto e 8x8?",   "56",      "64",      "72",      "48",     'B'},
  {"Capital FR?",     "Lyon",    "Paris",   "Marsei.", "Bord.",  'B'},
  {"Planeta maior?",  "Saturn",  "Neptun.", "Jupiter", "Urano",  'C'},
  {"Pintou Mona?",    "Picasso", "DaVinci", "Monet",   "Dali",   'B'},
  {"Ano 2a Guerra?",  "1940",    "1939",    "1941",    "1942",   'B'}
};

int  nivelAtual     = 0;
bool mostraAB       = true;
int  estadoJogo     = ESTADO_ESPERA;

int notasVitoria[]    = {523, 659, 784, 659, 784, 1047};
int duracoesVitoria[] = {150, 150, 150, 100, 100, 600};

int notasPerdeu[]    = {440, 392, 349, 294, 262};
int duracoesPerdeu[] = {200, 200, 200, 200, 600};

// ═══════════════════════════════
// Buzzer
// ═══════════════════════════════
void musicaVitoria() {
  int numNotas = sizeof(notasVitoria) / sizeof(notasVitoria[0]);
  for (int i = 0; i < numNotas; i++) {
    tone(BUZZER, notasVitoria[i], duracoesVitoria[i]);
    delay(duracoesVitoria[i] * 1.3);
    noTone(BUZZER);
  }
}
void musicaPerdeu() {
  int numNotas = sizeof(notasPerdeu) / sizeof(notasPerdeu[0]);
  for (int i = 0; i < numNotas; i++) {
    tone(BUZZER, notasPerdeu[i], duracoesPerdeu[i]);
    delay(duracoesPerdeu[i] * 1.3);
    noTone(BUZZER);
  }
}

void respostaCerta(){
  tone(BUZZER,1000,200);
  delay(250);
  noTone(BUZZER);
}

// ═══════════════════════════════
// TIMER
// ═══════════════════════════════
hw_timer_t *timer           = NULL;
volatile int  segundos      = 9;
volatile bool tempoEsgotado = false;

void IRAM_ATTR onTimer() {
  if (segundos > 0) {
    segundos--;
  } else {
    tempoEsgotado = true;
  }
}

void iniciaTimer() {
  segundos      = 9;
  tempoEsgotado = false;
  timer = timerBegin(1000000);        // 1MHz
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000000, true, 0); // 1 segundo
}

void paraTimer() {
  timerDetachInterrupt(timer);
  timerEnd(timer);
}

// ═══════════════════════════════
// LCD
// ═══════════════════════════════
void mostraEcraInicio() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  QUIZ ESP32  ");
  lcd.setCursor(0, 1);
  lcd.print("  CLICA START!");
}

void mostraOpcoes() {
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);

  if (mostraAB) {
    lcd.print("A:");
    lcd.print(perguntas[nivelAtual].opcA);
    lcd.print(" B:");
    lcd.print(perguntas[nivelAtual].opcB);
  } else {
    lcd.print("C:");
    lcd.print(perguntas[nivelAtual].opcC);
    lcd.print(" D:");
    lcd.print(perguntas[nivelAtual].opcD);
  }
}

void mostraPergunta() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(perguntas[nivelAtual].texto);
  lcd.setCursor(15, 0);
  lcd.print(segundos);
  mostraAB = true;
  mostraOpcoes();
  iniciaTimer();
}

// ═══════════════════════════════
// JOGO
// ═══════════════════════════════
void gameOver() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  GAME OVER!  ");
  lcd.setCursor(0, 1);
  lcd.print(" Tenta de novo");
  digitalWrite(LED_VERMELHO, HIGH);
  musicaPerdeu();                  
  digitalWrite(LED_VERMELHO, LOW); 
  nivelAtual = 0;
  estadoJogo = ESTADO_ESPERA;
  mostraEcraInicio();
}

void vitoria() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  PARABENS!!  ");
  lcd.setCursor(0, 1);
  lcd.print(" Ganhaste! :) ");
  digitalWrite(LED_VERDE, HIGH);
  musicaVitoria();                 
  digitalWrite(LED_VERDE, LOW);    
  delay(7000);                     
  nivelAtual = 0;
  estadoJogo = ESTADO_ESPERA;
  mostraEcraInicio();
}

void verificaResposta(char respostaDada) {
  if (respostaDada == perguntas[nivelAtual].resposta) {
    respostaCerta();
    digitalWrite(LED_VERDE, HIGH);
    delay(1000);
    digitalWrite(LED_VERDE, LOW);
    nivelAtual++;
    if (nivelAtual >= 8) {
      vitoria();
    } else {
      mostraPergunta();
    }
  } else {
    digitalWrite(LED_VERMELHO, HIGH);
    delay(1000);
    digitalWrite(LED_VERMELHO, LOW);
    gameOver();
  }
}

// ═══════════════════════════════
// SETUP
// ═══════════════════════════════
void setup() {
  Wire.begin(LCD_SDA, LCD_SCL);
  Wire.setClock(10000);

  lcd.init();
  lcd.backlight();
  mostraEcraInicio();

  pinMode(BTN_START, INPUT_PULLUP);
  pinMode(BTN_A,     INPUT_PULLUP);
  pinMode(BTN_B,     INPUT_PULLUP);
  pinMode(BTN_C,     INPUT_PULLUP);
  pinMode(BTN_D,     INPUT_PULLUP);

  pinMode(LED_VERDE,    OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BUZZER,       OUTPUT);
}

// ═══════════════════════════════
// LOOP
// ═══════════════════════════════
void loop() {

  if (estadoJogo == ESTADO_ESPERA) {
    if (digitalRead(BTN_START) == LOW) {
      delay(200);
      estadoJogo = ESTADO_JOGO;
      nivelAtual = 0;
      mostraPergunta();
    }
  }

  else if (estadoJogo == ESTADO_JOGO) {
    lcd.setCursor(15, 0);
    lcd.print(segundos);
    lcd.print(" ");

    if (tempoEsgotado) {
      paraTimer();
      gameOver();
      return;
    }

    if (digitalRead(BTN_START) == LOW) {
      delay(200);
      mostraAB = !mostraAB;
      mostraOpcoes();
    }

    if (digitalRead(BTN_A) == LOW) { delay(200); paraTimer(); verificaResposta('A'); }
    if (digitalRead(BTN_B) == LOW) { delay(200); paraTimer(); verificaResposta('B'); }
    if (digitalRead(BTN_C) == LOW) { delay(200); paraTimer(); verificaResposta('C'); }
    if (digitalRead(BTN_D) == LOW) { delay(200); paraTimer(); verificaResposta('D'); }
  }
}