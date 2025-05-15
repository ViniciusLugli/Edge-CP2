  #include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// === LCD ===
LiquidCrystal_I2C lcd(0x27, 16, 2);

// === Pinos ===
#define led1 3
#define led2 4
#define led3 5
#define buzzer 6
#define LDR A0
#define DHTPIN 2
#define DHTTYPE DHT11

// === Objetos e variáveis ===
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

unsigned long previousDHTMillis = 0;
unsigned long previousBuzzerMillis = 0;
unsigned long previousLCDMillis = 0;

bool buzzerAtivo = false;
int lcdPagina = 0;

float temperatura = 0;
float umidade = 0;
int luz = 0;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(LDR, INPUT);

  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sistema Iniciado");
  delay(1000);
  lcd.clear();
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();

  // Leitura do LDR e inversão para % correto (0 escuro, 100 claro)
  int ldr = analogRead(LDR);
  luz = map(ldr, 0, 1023, 0, 100);
  Serial.println(luz);

  // Controle LEDs e buzzer (igual antes)
  if (luz <= 20) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(buzzer, LOW);
    buzzerAtivo = false;
  } else if (luz <= 50) {
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
    digitalWrite(buzzer, LOW);
    buzzerAtivo = false;
  } else if (luz <= 65) {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, HIGH);
    digitalWrite(buzzer, LOW);
    buzzerAtivo = false;
  } else {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, HIGH);

    if (!buzzerAtivo) {
      digitalWrite(buzzer, HIGH);
      previousBuzzerMillis = currentMillis;
      buzzerAtivo = true;
    }
  }
  if (buzzerAtivo && (currentMillis - previousBuzzerMillis >= 3000)) {
    digitalWrite(buzzer, LOW);
    buzzerAtivo = false;
  }

  // Leitura do sensor DHT22 com intervalo
  if (currentMillis - previousDHTMillis >= delayMS) {
    previousDHTMillis = currentMillis;

    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (!isnan(event.temperature)) {
      temperatura = event.temperature;
    }
    dht.humidity().getEvent(&event);
    if (!isnan(event.relative_humidity)) {
      umidade = event.relative_humidity;
    }
  }

  // Controle do LCD para mostrar as páginas com mensagens customizadas
  if (currentMillis - previousLCDMillis >= 2000) {
    previousLCDMillis = currentMillis;
    lcd.clear();

    if (lcdPagina == 0) {
      // Temperatura
      lcd.setCursor(0, 0);
      if (temperatura <= 10) {
        lcd.print("Temp. BAIXA");
      } else if (temperatura <= 15) {
        lcd.print("Temperatura OK");
      } else {
        lcd.print("Temp. ALTA");
      }

      lcd.setCursor(0, 1);
      lcd.print("Temp. = ");
      lcd.print(temperatura, 1);
      lcd.print((char)223);
      lcd.print("C");

      lcdPagina = 1;

    } else if (lcdPagina == 1) {
      // Umidade
      lcd.setCursor(0, 0);
      if (umidade < 45) {
        lcd.print("Umidade BAIXA");
      } else if (umidade <= 70) {
        lcd.print("Umidade OK");
      } else {
        lcd.print("Umidade ALTA");
      }

      lcd.setCursor(0, 1);
      lcd.print("Umid. = ");
      lcd.print(umidade, 0);
      lcd.print("%");

      lcdPagina = 2;

    } else {
      // Luz
      lcd.setCursor(0, 0);
      if (luz <= 30) {
        lcd.print("Luz BAIXA");
      } else if (luz <= 50) {
        lcd.print("Luz OK");
      } else {
        lcd.print("Luz ALTA");
      }

      lcd.setCursor(0, 1);
      lcd.print("Luz = ");
      lcd.print(luz);
      lcd.print("%");

      lcdPagina = 0;
    }
  }
}
