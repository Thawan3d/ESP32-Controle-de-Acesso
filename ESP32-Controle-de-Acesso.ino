
#include <LiquidCrystal.h>

// LCD: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(22, 23, 18, 19, 21, 17);

// LEDs
const int ledPorta = 32;
const int ledVermelho = 27;

// Botoes da senha: 1, 2, 3 e 4
const int botoes[] = {13, 14, 25, 26};

// Botao 5: cartao de liberacao
const int botaoCartao = 33;

// Senha correta: 3 -> 2 -> 4 -> 1
const int senha[] = {3, 2, 4, 1};
const int tamanhoSenha = 4;

// Configuracoes
const int maxErros = 3;
const int maxBloqueios = 6;

const unsigned long tempoBloqueio = 5000;
const unsigned long tempoPorta = 5000;
const unsigned long tempoMensagem = 2000;
const unsigned long debounce = 180;
const unsigned long intervaloPisca = 250;

// Controle da senha
int digitados[4];
int posicao = 0;
int erros = 0;
int bloqueios = 0;

// Controle dos botoes
const int totalBotoes = 5;
int pinosBotoes[] = {13, 14, 25, 26, 33};

bool estadoAnterior[5] = {
  HIGH, HIGH, HIGH, HIGH, HIGH
};

unsigned long ultimoClique[5] = {0, 0, 0, 0, 0};

// Estado da porta
bool portaAberta = false;
unsigned long inicioPorta = 0;

// Bloqueio temporario
bool bloqueado = false;
unsigned long inicioBloqueio = 0;

// Mensagens do display
bool mensagemAtiva = false;
unsigned long inicioMensagem = 0;

// Bloqueio permanente
bool travaPermanente = false;
bool estadoLedVermelho = false;
unsigned long ultimaPiscada = 0;

// Exibe o estado inicial
void mostrarTrancada() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PORTA TRANCADA");
  lcd.setCursor(0, 1);
  lcd.print("Senha: ____");
}

// Ativa o bloqueio permanente
void iniciarBloqueioPermanente(unsigned long agora) {
  travaPermanente = true;

  digitalWrite(ledPorta, LOW);

  estadoLedVermelho = HIGH;
  digitalWrite(ledVermelho, HIGH);
  ultimaPiscada = agora;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TRAVA PERMANENTE");
  lcd.setCursor(0, 1);
  lcd.print("Aproxime cartao");
}

// Ativa bloqueio temporario ou permanente
void iniciarBloqueio(unsigned long agora) {
  erros = 0;
  posicao = 0;
  bloqueios++;

  if (bloqueios >= maxBloqueios) {
    iniciarBloqueioPermanente(agora);
    return;
  }

  bloqueado = true;
  inicioBloqueio = agora;

  digitalWrite(ledPorta, LOW);
  digitalWrite(ledVermelho, HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SISTEMA BLOQUEADO");
  lcd.setCursor(0, 1);
  lcd.print("Aguarde 5 seg");
}

// Verifica a senha somente apos 4 digitos
void verificarSenha(unsigned long agora) {
  bool correta = true;

  for (int j = 0; j < tamanhoSenha; j++) {
    if (digitados[j] != senha[j]) {
      correta = false;
    }
  }

  posicao = 0;
  lcd.clear();

  if (correta) {
    erros = 0;

    lcd.setCursor(0, 0);
    lcd.print("SENHA CORRETA!");
    lcd.setCursor(0, 1);
    lcd.print("PORTA ABERTA");

    digitalWrite(ledPorta, HIGH);
    portaAberta = true;
    inicioPorta = agora;

  } else {
    erros++;

    if (erros >= maxErros) {
      iniciarBloqueio(agora);

    } else {
      lcd.setCursor(0, 0);
      lcd.print("SENHA INCORRETA");
      lcd.setCursor(0, 1);
      lcd.print("Erros: ");
      lcd.print(erros);
      lcd.print("/3");

      digitalWrite(ledPorta, LOW);

      mensagemAtiva = true;
      inicioMensagem = agora;
    }
  }
}

void setup() {
  pinMode(ledPorta, OUTPUT);
  pinMode(ledVermelho, OUTPUT);

  digitalWrite(ledPorta, LOW);
  digitalWrite(ledVermelho, LOW);

  for (int i = 0; i < totalBotoes; i++) {
    pinMode(pinosBotoes[i], INPUT_PULLUP);
  }

  lcd.begin(16, 2);
  mostrarTrancada();
}

void loop() {
  unsigned long agora = millis();
  bool pressionado[5];

  // Detecta cada pressionamento dos botoes
  for (int i = 0; i < totalBotoes; i++) {
    bool leitura = digitalRead(pinosBotoes[i]);

    pressionado[i] =
      leitura == LOW &&
      estadoAnterior[i] == HIGH &&
      agora - ultimoClique[i] >= debounce;

    if (pressionado[i]) {
      ultimoClique[i] = agora;
    }

    estadoAnterior[i] = leitura;
  }

  // BLOQUEIO PERMANENTE
  // LED vermelho pisca continuamente ate o cartao
  if (travaPermanente) {
    if (agora - ultimaPiscada >= intervaloPisca) {
      ultimaPiscada = agora;
      estadoLedVermelho = !estadoLedVermelho;
      digitalWrite(ledVermelho, estadoLedVermelho);
    }

    // Botao 5 simula o cartao autorizado
    if (pressionado[4]) {
      travaPermanente = false;
      bloqueios = 0;
      erros = 0;
      posicao = 0;

      digitalWrite(ledVermelho, LOW);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("CARTAO ACEITO");
      lcd.setCursor(0, 1);
      lcd.print("Acesso liberado");

      mensagemAtiva = true;
      inicioMensagem = agora;
    }

    return;
  }

  // BLOQUEIO TEMPORARIO
  if (bloqueado) {
    if (agora - inicioBloqueio >= tempoBloqueio) {
      bloqueado = false;
      digitalWrite(ledVermelho, LOW);
      mostrarTrancada();
    } else {
      return;
    }
  }

  // FECHAMENTO AUTOMATICO DA PORTA
  if (portaAberta) {
    if (agora - inicioPorta >= tempoPorta) {
      portaAberta = false;
      digitalWrite(ledPorta, LOW);
      mostrarTrancada();
    } else {
      return;
    }
  }

  // FINALIZA MENSAGENS
  if (mensagemAtiva) {
    if (agora - inicioMensagem >= tempoMensagem) {
      mensagemAtiva = false;
      mostrarTrancada();
    } else {
      return;
    }
  }

  // LEITURA DOS QUATRO BOTOES DA SENHA
  for (int i = 0; i < 4; i++) {
    if (pressionado[i]) {
      digitados[posicao] = i + 1;

      // Mostra asteriscos, sem revelar a senha
      lcd.setCursor(7 + posicao, 1);
      lcd.print("*");

      posicao++;

      if (posicao == tamanhoSenha) {
        verificarSenha(agora);
        return;
      }
    }
  }
}

