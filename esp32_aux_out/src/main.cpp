#include <Arduino.h>
#include <WiFi.h>
#include "AudioTools.h"
#include "BluetoothA2DPSink.h"

BluetoothA2DPSink a2dp_sink;

void setup() {
  Serial.begin(115200);

  // Temporary: Force Wi-Fi off to free up the radio antenna
  WiFi.mode(WIFI_OFF);

  // Custom I2S configuration with enlarged DMA buffers
  i2s_config_t custom_i2s_config = {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
      .sample_rate = 44100, // Auto adjusts as BT stream changes
      .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
      .communication_format = I2S_COMM_FORMAT_STAND_I2S,
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
      .dma_buf_count = 16,   // Increased from default (6)
      .dma_buf_len = 1024,   // Increased from default (64)
      .use_apll = true,
      .tx_desc_auto_clear = true // Keeps signal silent when no audio is being routed
  };

  // Define hardware pins
  i2s_pin_config_t my_pin_config = {
      .bck_io_num = 26,
      .ws_io_num = 25,
      .data_out_num = 19,
      .data_in_num = I2S_PIN_NO_CHANGE
  };

  // Apply configurations
  a2dp_sink.set_i2s_config(custom_i2s_config);
  a2dp_sink.set_pin_config(my_pin_config);
  a2dp_sink.set_volume(70);

  // Start the service
  a2dp_sink.start("ESP32_Audio_Receiver"); 
}

void loop() {
  delay(100); 
}
