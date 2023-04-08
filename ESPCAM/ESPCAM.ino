
#include "esp_camera.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>

// https://github.com/botabotlab/ESP32CAM-MQTT/blob/master/ESP32_Cam_MQTT/ESP32_Cam_MQTT.ino
// https://randomnerdtutorials.com/esp32-cam-ov2640-camera-settings/

// CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

const char* ssid = "vodafoneAAFAVK";
const char* password = "KfPTnPFpTkgL6qPF";
const char* mqtt_server = "192.168.0.6";
const char* HostName = "ESP32CAM";
const char* topic_UP = "Picture";
const char* topic_PHOTO = "TakeAPicture";


WiFiClient espClient;
PubSubClient client(espClient);

void callback(String topic, byte* message, unsigned int length) {
  String messageTemp;
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
  if (topic == topic_PHOTO) {
    Serial.println("PING");
    take_picture();
  }
}

void camera_init() {
  // Inicializa la cámara ESP32-CAM
  static camera_config_t camera_config = {
    .pin_pwdn = PWDN_GPIO_NUM,
    .pin_reset = RESET_GPIO_NUM,
    .pin_xclk = XCLK_GPIO_NUM,
    .pin_sscb_sda = SIOD_GPIO_NUM,
    .pin_sscb_scl = SIOC_GPIO_NUM,
    .pin_d7 = Y9_GPIO_NUM,
    .pin_d6 = Y8_GPIO_NUM,
    .pin_d5 = Y7_GPIO_NUM,
    .pin_d4 = Y6_GPIO_NUM,
    .pin_d3 = Y5_GPIO_NUM,
    .pin_d2 = Y4_GPIO_NUM,
    .pin_d1 = Y3_GPIO_NUM,
    .pin_d0 = Y2_GPIO_NUM,
    .pin_vsync = VSYNC_GPIO_NUM,
    .pin_href = HREF_GPIO_NUM,
    .pin_pclk = PCLK_GPIO_NUM,
    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,
    .pixel_format = PIXFORMAT_JPEG,  // 1BPP/GRAYSCALE,
    .frame_size = FRAMESIZE_VGA,
    .jpeg_quality = 10,
    .fb_count = 1
  };
  esp_err_t err = esp_camera_init(&camera_config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}

void take_picture() {
  for (size_t i = 0; i < 2; i++) {
    // Captura la imagen con la cámara
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Error al capturar la imagen");
      return;
    }
    // Convierte la imagen en formato JPEG a un array de bytes
    uint8_t* jpg_buf = fb->buf;
    size_t jpg_buf_len = fb->len;
    if (i == 1) {
      client.beginPublish(topic_UP, jpg_buf_len, false);
      client.write(jpg_buf, jpg_buf_len);
      client.endPublish();
    }
    // Libera el buffer de la imagen capturada
    esp_camera_fb_return(fb);
  }
}
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(HostName);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(HostName)) {
      Serial.println("connected");
      client.subscribe(topic_PHOTO);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void setup() {
  Serial.begin(115200);
  camera_init();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}