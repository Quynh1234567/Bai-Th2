#include <WiFi.h>
#include <esp_now.h>

// Địa chỉ MAC của ESP nhận
uint8_t receiverMac[] = {0x14, 0x33, 0x5C, 0x30, 0x19, 0xF8};

// Hàm callback khi gửi xong
void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Trạng thái gửi: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Thành công" : "Thất bại");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);  // ESP-NOW cần ở chế độ STA

  if (esp_now_init() != ESP_OK) {
    Serial.println("Lỗi khởi tạo ESP-NOW!");
    return;
  }

  // Đăng ký callback khi gửi xong
  esp_now_register_send_cb(onSent);

  // Thêm peer (ESP nhận)
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Thêm peer thất bại!");
    return;
  }

  Serial.println("Bắt đầu gửi dữ liệu...");
}

void loop() {
  const char *msg = "Hello World";
  esp_err_t result = esp_now_send(receiverMac, (const uint8_t *)msg, strlen(msg) + 1);

  if (result == ESP_OK) {
    Serial.println("Đã gửi: Hello World");
  } else {
    Serial.println("Gửi lỗi!");
  }

  delay(2000);
}
