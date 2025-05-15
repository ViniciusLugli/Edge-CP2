# Sistema de Monitoramento Ambiental com Arduino

Este projeto monitora a **luminosidade**, **temperatura** e **umidade** do ambiente usando Arduino, sensores LDR e DHT22, e exibe as informações em um display LCD 16x2 com interface I2C. LEDs indicam o nível de luminosidade, e um buzzer é acionado em caso de luminosidade alta.

## Links
### Tinkercad: 
https://wokwi.com/projects/430982419716998145

### Vídeo de Explicação:
https://youtu.be/6_8JmXO6j2Q

## Integrantes do Ambiente Guard:
- Enzo Dourado - RM 565585
- Matheus Victorio - RM 566447
- Vinícius Lugli - RM 564694
- Hugo Copatti - RM 561968
- Lucas Villani - RM 562200



---

## Componentes Utilizados

- Arduino Uno (ou compatível)
- Sensor de luminosidade LDR com divisor de tensão
- Sensor DHT11 (temperatura e umidade)
- Display LCD 16x2 com módulo I2C (endereço 0x27)
- 3 LEDs (verde, amarelo e vermelho)
- Buzzer
- Resistores e cabos para conexões

---

## Funcionamento do Código

### Inicialização

- Configura os pinos de LEDs, buzzer e sensor LDR.
- Inicializa a comunicação serial (removida na versão final).
- Inicializa o sensor DHT11 e lê suas características para definir o intervalo mínimo entre leituras.
- Inicializa o display LCD e exibe uma mensagem de "Sistema Iniciado".

### Leitura e Controle da Luminosidade

- O sensor LDR é lido pela entrada analógica A0.
- O valor bruto do LDR (0 a 1023) é invertido e mapeado para uma escala percentual (0 a 100%) para facilitar a interpretação (menor valor do LDR significa mais luz).
- LEDs são acionados conforme o nível de luz:
  - Luz baixa (≤20%): LED verde aceso.
  - Luz média (21% a 50%): LED amarelo aceso.
  - Luz alta (>65%): LED vermelho aceso e buzzer acionado por 3 segundos.

### Leitura da Temperatura e Umidade

- As leituras do sensor DHT11 são feitas com intervalo mínimo baseado no tempo recomendado pelo sensor.
- Temperatura e umidade são armazenadas em variáveis para serem exibidas no LCD.
- Tratamento para leitura inválida (NaN) está incluído.

### Exibição no LCD

- A cada 2 segundos, o LCD alterna entre mostrar a luminosidade, a temperatura e a umidade.
- Para cada parâmetro, o LCD mostra:
  - Um status textual (BAIXA, OK, ALTA) conforme os limites definidos.
  - O valor numérico correspondente.
- Exemplos:
  - Temperatura OK
    Temp. : 18.5C
  - Umidade ALTA
    Umidade = 85%
  - Luz BAIXA
    Luz = 25%

### Controle do Buzzer

- Quando a luz está em nível ALTO, o buzzer é ligado e fica ativo por 3 segundos sem travar o código (usando `millis()` para temporização não bloqueante).

---

## Diagrama de Conexão

| Componente      | Pino Arduino      |
|-----------------|-------------------|
| LDR (A0 do módulo) | A0                |
| DHT11 Data      | 2                 |
| LED Verde       | 3                 |
| LED Amarelo     | 4                 |
| LED Vermelho    | 5                 |
| Buzzer          | 6                 |
| LCD I2C SDA     | A4 (ou SDA)       |
| LCD I2C SCL     | A5 (ou SCL)       |

> **Nota:** O endereço I2C do LCD pode variar, certifique-se com um scanner I2C se necessário.

---

## Bibliotecas Necessárias

- LiquidCrystal_I2C
- DHT sensor library (Adafruit)
- Adafruit Unified Sensor

---

Este sistema é eficiente para monitoramento básico, sem travar o Arduino, graças ao uso de `millis()` para temporização não bloqueante e à alternância inteligente das mensagens no LCD.

