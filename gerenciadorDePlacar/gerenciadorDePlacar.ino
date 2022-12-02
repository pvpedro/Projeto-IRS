/*
// Programa: Gerenciar Placar com Display LCD 16x2
// Autor: PEDRO VICTOR R. D. SANTOS
// Data: 30/11/2022
// Local: Rio de Janeiro
// Colégio: IRS
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "notas.h"
#include "musicas.h"
#include "pinosArduino.h"

LiquidCrystal_I2C lcd(endereco, colunas, linhas);

int player;
int maquina;

byte outPinos[] = {ledRed, ledGreen, ledBlue, buzzer};
byte inpPinos[] = {botao1P, botao2P, botao1M, botao2M, botaoR};

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();

  for (int i = 0; i <= 3; i++) {
    pinMode(outPinos[i], OUTPUT); 
  } 
  for (int i = 0; i <= 4; i++) {
    pinMode(inpPinos[i], INPUT_PULLUP); 
  } 

  resetScoreboard();
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()>0) {
    char c = Serial.read();
    if (c == 'A') {
      adicionar1P();
      delay(200);
    } else if (c == 'B') {
      adicionar1M();
      delay(200);
    } 
  }
  
  if (digitalRead(botao1P) == LOW) {
    adicionar1P();
    for (int i = 0; i <= 1; i++) {
      buzzerToque(); 
      delay(50);
    } 

    while(digitalRead(botao1P) == LOW) {}
    delay(200);
  } else if (digitalRead(botao2P) == LOW) {
    remover1P();
    buzzerToque();
    
    while(digitalRead(botao2P) == LOW) {}
    delay(200);
  } else if (digitalRead(botao1M) == LOW) {
    adicionar1M();

    for (int i = 0; i <= 1; i++) {
      buzzerToque(); 
      delay(50);
    } 
    
    while(digitalRead(botao1M) == LOW) {}
    delay(200);
  } else if (digitalRead(botao2M) == LOW) {
    remover1M();
    buzzerToque();
    
    while(digitalRead(botao2M) == LOW) {}
    delay(200);
  } else if (digitalRead(botaoR) == LOW) {
    resetScoreboard();

    for (int i = 0; i <= 2; i++) {
      buzzerToque(); 
      delay(50);
    } 
    
    while(digitalRead(botaoR) == LOW) {}
    delay(200);
  }

  if (player == 7 && maquina == 0) {
    toca(starTrek, sizeof(starTrek) / sizeof(int) / 2);
    resetScoreboard();
    delay(200);
  }
}

void updateScoreboard() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Player  ");
  lcd.print(player);
  lcd.setCursor(0, 1);
  lcd.print("Maquina ");
  lcd.print(maquina); 
  delay(200);
}

void resetScoreboard() {
  player = 0;
  maquina = 0;
  updateScoreboard();
  digitalWrite(ledRed, HIGH);
  digitalWrite(ledGreen, HIGH);
  digitalWrite(ledBlue, HIGH); 
  delay(50);
}

void buzzerToque() {
  tone(buzzer, NOTE_F6);
  delay(100);
  noTone(buzzer);
  return;
}

void toca(int melodia[], int notas) {
  int notaInteira = (60000 * 4) / tempo,
      divisor = 0, duracaoNota = 0, notaAtual;

  for (notaAtual = 0; notaAtual < notas * 2; notaAtual += 2) {
    // Calcula a duração de cada nota
    divisor = melodia[notaAtual + 1];
    if (divisor > 0) {
      duracaoNota = (notaInteira) / divisor;
    } else if (divisor < 0) {
      // dotted notes are represented with negative durations!!
      duracaoNota = (notaInteira) / abs(divisor);
      duracaoNota *= 1.5; // increases the duration in half for dotted notes
    }

    // Toca apenas 90% da duração da nota
    tone(buzzer, melodia[notaAtual], duracaoNota * 0.9);
    delay(duracaoNota);
    noTone(buzzer);
  }
}

void adicionar1P() {
    player = player +1;
    updateScoreboard();

    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledBlue, LOW);
    delay(50);
}

void remover1P() {
    player = player - 1;
    updateScoreboard();

    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledBlue, HIGH);
    delay(50);
}

void adicionar1M() {
    maquina = maquina +1;
    updateScoreboard();

    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledBlue, LOW);
    delay(50);
}

void remover1M() {
    maquina = maquina - 1;
    updateScoreboard();

    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledBlue, HIGH);
    delay(50);
}