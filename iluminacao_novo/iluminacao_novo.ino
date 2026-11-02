#include "BluetoothSerial.h"
#include <esp_spp_api.h> 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

#define OLED_SDA 21  // Pino SDA
#define OLED_SCL 22  // Pino SCL

// Configurações do OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

char valorRecebido = 'A';
const int LedRed = 2;
const int LedBlue = 15;

const char* nomeDaPlaca = "Iluminacao 10";
bool dispositivoConectado = false;

// Função que desenha todas as informações na tela
void atualizaTela() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  // 1ª Linha: Nome da Placa
  display.setCursor(0, 0);
  display.print("Placa: ");
  display.println(nomeDaPlaca);
  
  // 2ª Linha: Status da Conexão
  display.setCursor(0, 15);
  if (dispositivoConectado) {
    display.println("Status: CONECTADO!");
  } else {
    display.println("Status: Aguardando...");
  }
  display.setCursor(0, 35);
  display.print("Led Branco:  ");
  if (digitalRead(LedRed) == HIGH) {
    display.println("ACESO");
  } else {
    display.println("APAGADO");
  }
  
  // 4ª Linha: Status do Led Blue
  display.setCursor(0, 45);
  display.print("Led Azul: ");
  if (digitalRead(LedBlue) == HIGH) {
    display.println("ACESO");
  } else {
    display.println("APAGADO");
  }
  
  display.display(); // Envia os dados para o OLED
}

// Função de eventos de conexão
void btCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    // Quando um dispositivo CONECTAR
    dispositivoConectado = true;
    atualizaTela();
    
  } else if (event == ESP_SPP_CLOSE_EVT) {
    // Quando um dispositivo DESCONECTAR
    dispositivoConectado = false;
    atualizaTela();
  }
}

void setup()
{
  Serial.begin(115200);
  
  Wire.begin(OLED_SDA, OLED_SCL);

  // INICIA O DISPLAY OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("Falha ao iniciar SSD1306"));
    for(;;);
  }
  
  pinMode(LedRed, OUTPUT);
  pinMode(LedBlue, OUTPUT);
  digitalWrite(LedRed, LOW);
  digitalWrite(LedBlue, LOW);

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.println("Iniciando Bluetooth...");
  display.display();

  SerialBT.register_callback(btCallback);         
  SerialBT.begin(nomeDaPlaca); 
  
  atualizaTela();
}

void AtualizaST()
{
  if (SerialBT.available()) 
  {
    valorRecebido = (char)SerialBT.read();
    bool mudouLed = false; 
    
    if(valorRecebido == 'R') {
      digitalWrite(LedRed, HIGH);
      mudouLed = true;
      
      SerialBT.print('2');
      
    }
    else if(valorRecebido == 'r') {
      digitalWrite(LedRed, LOW);
      mudouLed = true;
      
      SerialBT.print('1');
      
    }
    else if(valorRecebido == 'B') {
      digitalWrite(LedBlue, HIGH);
      mudouLed = true;
      delay(1);
      SerialBT.println('4');
    }
    else if(valorRecebido == 'b') {
      digitalWrite(LedBlue, LOW);
      mudouLed = true;
      delay(1);
      SerialBT.println('3');
    } 
    if (mudouLed) {
      atualizaTela();
    }
  }
}

void loop()
{
    AtualizaST();
    //delay(50); 
}