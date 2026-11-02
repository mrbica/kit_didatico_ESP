#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "ssd1306.h"





#define DEBUG 1


// Mapeamento dos Pinos GPIO
#define LED_1   GPIO_NUM_15
#define LED_2  GPIO_NUM_2
#define botao1 GPIO_NUM_16
#define botao2 GPIO_NUM_17
#define Rele GPIO_NUM_12
#define SDA_PIN 21
#define SCL_PIN 22
#define TAG "OLED"
#define Q1 GPIO_NUM_14


SSD1306_t dev;

void SSD()
{
  ESP_LOGI(TAG, "Inicializando interface I2C");
    i2c_master_init(&dev, SDA_PIN, SCL_PIN, -1);

    ESP_LOGI(TAG, "Inicializando display SSD1306 (128x64)");
    ssd1306_init(&dev, 128, 64);
 
    // Limpa a tela
    ssd1306_clear_screen(&dev, false);
    ssd1306_contrast(&dev, 0xff);

    // Escreve texto no display
   // ssd1306_display_text(dev, pagina, texto, tamanho_texto, invertido)
    ssd1306_display_text(&dev, 0, "Placa Didatica ", 16, false);
    ssd1306_display_text(&dev, 2, "Prof. Bica", 10, false);
    ssd1306_display_text(&dev, 4, "ESP32 S e C3", 12, true); // Texto invertido

    ESP_LOGI(TAG, "Display configurado com sucesso!");

    if(gpio_get_level(LED_2) == 0)
    {
        ssd1306_clear_line(&dev, 2, false);
        ssd1306_display_text(&dev, 2, "Botao 1: ON ", 12, false);
    }
    else
              
    {
          ssd1306_clear_screen(&dev, false);
          //ssd1306_clear_line(&dev, 2, false);
       // ssd1306_display_text(&dev, 2, "Botao 1: OFF", 12, false);
    }

}



// Handles das tarefas
//static TaskHandle_t task_botao = NULL;

// Protótipos das funções
void task_botao1(void *pvParameters);
void task_botao2(void *pvParameters);



// --- CONTROLE DE TAREFAS (FreeRTOS) ---
void task_botao1(void *pvParameters) {
    	
    if( DEBUG )
      ESP_LOGI( TAG, "task_botao1 run...\r\n" );

    esp_rom_gpio_pad_select_gpio(LED_1); 
    gpio_set_direction( LED_1, GPIO_MODE_OUTPUT );
    esp_rom_gpio_pad_select_gpio(LED_2); 
    gpio_set_direction( LED_2, GPIO_MODE_OUTPUT );
    esp_rom_gpio_pad_select_gpio(botao1);
    gpio_set_direction( botao1, GPIO_MODE_INPUT );
    esp_rom_gpio_pad_select_gpio(botao2);
    gpio_set_direction( botao2, GPIO_MODE_INPUT );
    


   // for(;;) 
   // {
		
  		if( ( gpio_get_level( botao1 ) == 0 ) ) 
      
        {      
                gpio_set_level(LED_2, 1);
                gpio_set_level(LED_1, 0);
  		
             ssd1306_clear_screen(&dev, false);
        ssd1306_clear_line(&dev, 2, false);
        ssd1306_display_text(&dev, 2, "Botao 1: ON ", 12, false);
   
         ssd1306_clear_line(&dev, 4, false);
        ssd1306_display_text(&dev, 4, "Botao 2: OFF", 12, false);
    }

  		vTaskDelay( 1500/portTICK_PERIOD_MS );
  //  }
}

void task_botao2(void *pvParameters) {
    	
    if( DEBUG )
      ESP_LOGI( TAG, "task_botao2 run...\r\n" );

    esp_rom_gpio_pad_select_gpio(LED_1); 
    gpio_set_direction( LED_1, GPIO_MODE_OUTPUT );
    esp_rom_gpio_pad_select_gpio(LED_2); 
    gpio_set_direction( LED_2, GPIO_MODE_OUTPUT );
    esp_rom_gpio_pad_select_gpio(botao1);
    gpio_set_direction( botao1, GPIO_MODE_INPUT );
    esp_rom_gpio_pad_select_gpio(botao2);
    gpio_set_direction( botao2, GPIO_MODE_INPUT );
    
   // for(;;) 
    //{
		
  		if( ( gpio_get_level( botao2 ) == 0 ) ) 
      
        {      
                gpio_set_level(LED_2, 0);
                gpio_set_level(LED_1, 1);
  		
             ssd1306_clear_screen(&dev, false);
        ssd1306_clear_line(&dev, 2, false);
        ssd1306_display_text(&dev, 2, "Botao 2: ON ", 12, false);
   
         ssd1306_clear_line(&dev, 4, false);
        ssd1306_display_text(&dev, 4, "Botao 1: OFF", 12, false);
    }

  		vTaskDelay( 1500/portTICK_PERIOD_MS );
   // }
}


void app_main(void) 
{
   esp_rom_gpio_pad_select_gpio(LED_1); 
    gpio_set_direction( LED_1, GPIO_MODE_INPUT );
    esp_rom_gpio_pad_select_gpio(LED_2); 
    gpio_set_direction( LED_2, GPIO_MODE_INPUT_OUTPUT );
    esp_rom_gpio_pad_select_gpio(Rele);
    gpio_set_direction( Rele, GPIO_MODE_OUTPUT );
     esp_rom_gpio_pad_select_gpio(Q1);
    gpio_set_direction( Q1, GPIO_MODE_OUTPUT );
    gpio_set_level(Rele, 0);
    gpio_set_level(LED_2, 0);
    gpio_set_level(LED_1, 0);
    gpio_set_level(Q1, 0);

 // SSD1306_t dev;
SSD();
   /* ESP_LOGI(TAG, "Inicializando interface I2C");
    i2c_master_init(&dev, SDA_PIN, SCL_PIN, -1);

    ESP_LOGI(TAG, "Inicializando display SSD1306 (128x64)");
    ssd1306_init(&dev, 128, 64);
 
    // Limpa a tela
    ssd1306_clear_screen(&dev, false);
    ssd1306_contrast(&dev, 0xff);

    // Escreve texto no display
    // ssd1306_display_text(dev, pagina, texto, tamanho_texto, invertido)
   // ssd1306_display_text(&dev, 0, "Placa Didatica ", 16, false);
  //  ssd1306_display_text(&dev, 2, "Prof. Bica", 10, false);
  //  ssd1306_display_text(&dev, 4, "ESP32 S e C3", 12, true); // Texto invertido

    ESP_LOGI(TAG, "Display configurado com sucesso!");*/
    
  /*  while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
      
    }*/
      if( (xTaskCreate( task_botao1, "task_botao1", 2048, NULL, 1, NULL )) != pdTRUE )
        {
      if( DEBUG )
        ESP_LOGI( TAG, "error - Nao foi possivel alocar task_botao1.\r\n" );  
      return;   
     
    }
      if( (xTaskCreate( task_botao2, "task_botao2", 2048, NULL, 1, NULL )) != pdTRUE )
        {
      if( DEBUG )
        ESP_LOGI( TAG, "error - Nao foi possivel alocar task_botao2.\r\n" );  
      return;   
     
    }

}