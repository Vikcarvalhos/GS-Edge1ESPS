#include<LiquidCrystal.h>

// Define os pinos para os LEDs, sensores e buzzer.
#define REDLED 12
#define GREENLED 13
#define TEMPERATURE A0
#define HEARTRATE A1
#define ARRHYTHMIA A2
#define buzzerAlarm 11


// Cria um objeto lcd para controlar o display LCD.
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// Variáveis para controlar o timing do buzzer e verificar condições de saúde.
long tempoAnteriorBuzzer = 0;
long tempoAnteriorDisplay = 0;
long verifyCondition = 0;

void setup(){
  // Inicia o display LCD com 16 colunas e 2 linhas.
  lcd.begin(16, 2);
  // Configura os pinos como entrada ou saída.
  pinMode(GREENLED, OUTPUT);
  pinMode(REDLED, OUTPUT);
  pinMode(TEMPERATURE, INPUT);
  pinMode(HEARTRATE, INPUT);
  pinMode(ARRHYTHMIA, INPUT);
  pinMode(buzzerAlarm, OUTPUT);
  pinMode(A2, INPUT);
  // Inicia a comunicação serial.
  Serial.begin(9600);
}

// Loop principal, executa repetidamente após o setup.
void loop(){
  // Lê e processa os valores dos sensores.
  float tempReading = (analogRead(TEMPERATURE) * 5.0) / 1024;
  int temperatureC = round((tempReading - 0.5) * 100);

  int heartRateValue = analogRead(HEARTRATE);
  int heartRate = map(heartRateValue, 0, 1023, 0, 200);
  
  int arrhythmiaValue = analogRead(ARRHYTHMIA);
  int beatInterval = map(arrhythmiaValue, 0, 1023, 100, 200);
  
  // Avalia as condições de saúde e aciona LEDs e buzzer conforme necessário.
  if (heartRate > 50 && heartRate < 100 && temperatureC >= 35 && temperatureC <= 37 && beatInterval < 160){
    digitalWrite(GREENLED, HIGH);
    digitalWrite(REDLED, LOW);
    noTone(buzzerAlarm);
    verifyCondition = 0;
  }else{
    digitalWrite(REDLED, HIGH);
    digitalWrite(GREENLED, LOW);
    verifyCondition = 1;
  }
  
  // Lógica para acionar o buzzer em intervalos regulares se a condição for anormal.
  // O buzzer é acionado a cada 3 segundos se a condição de saúde estiver anormal.
  if (millis() >= tempoAnteriorBuzzer + 3000 && verifyCondition == 1){
      tone(buzzerAlarm, 261);
      digitalWrite(buzzerAlarm, HIGH);
      delay(3000);
      noTone(buzzerAlarm);
      digitalWrite(buzzerAlarm, LOW);
      delay(3000);
      tempoAnteriorBuzzer = millis();
   }
  
  // Exibe informações no display LCD.
  // O display mostra a temperatura corporal e o status da frequência cardíaca.
  if (millis() >= tempoAnteriorDisplay + 5000){
    if(temperatureC >= 35 && temperatureC <= 37){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
      lcd.print(temperatureC);
      lcd.print("C");
      lcd.setCursor(0, 1);
      lcd.print("Normal");
    }else if(temperatureC < 35){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
      lcd.print(temperatureC);
      lcd.print(" C");
      lcd.setCursor(0, 1);
      lcd.print("Hipotermia");
    }else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
      lcd.print(temperatureC);
      lcd.print(" C");
      lcd.setCursor(0, 1);
      lcd.print("Febril");
    }
    tempoAnteriorDisplay = millis();
  }
	
  // Continuação da exibição de informações no LCD.
  if (millis() >= tempoAnteriorDisplay + 5000){
    if(heartRate > 100){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("FC: ");
      lcd.print(heartRate);
      lcd.print(" BPM");
      lcd.setCursor(0, 1);
      lcd.print("Taquicardia");
    } else if (heartRate < 50){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("FC: ");
      lcd.print(heartRate);
      lcd.print(" BPM");
      lcd.setCursor(0, 1);
      lcd.print("Bradicardia");
    } else if (beatInterval > 160){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("FC: ");
      lcd.print(heartRate);
      lcd.print(" BPM");
      lcd.setCursor(0, 1);
      lcd.print("Arritmia");
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("FC: ");
      lcd.print(heartRate);
      lcd.print(" BPM");
      lcd.setCursor(0, 1);
      lcd.print("Normal");
    }
    tempoAnteriorDisplay = millis();
  }
  
  // Envia dados de temperatura e frequência cardíaca para a porta serial.
  // Monitoramento e depuração.
  Serial.print("Temperatura: ");
  Serial.print(temperatureC);
  Serial.println("C");
  Serial.print("Frequencia Cardiaca: ");
  Serial.print(heartRate);
  Serial.println(" BPM");
  Serial.print("Tempo Buzzer: ");
  Serial.println(tempoAnteriorBuzzer);
  Serial.print("Tempo Display: ");
  Serial.println(tempoAnteriorDisplay);
  }
