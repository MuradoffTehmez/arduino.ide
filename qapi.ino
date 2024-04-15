#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

// Yetkiləndirilmiş kartların UID'lerini və adlarını təyin edin
String UID1 = "FA 70 EA AE";
String ad1 = "Kart 1, Tahmaz";

String UID2 = "3C 68 CF 33";
String ad2 = "Kart 2, Atilla";

String UID3 = "1C 1C CF 33";
String ad3 = "Kart 3, Asena";

// LED'ların pinlərini təyin edin
#define RED_LED_PIN 7
#define GREEN_LED_PIN 8

byte kilidDurumu = 1;  // Kilidin durumunu göstərmək üçün bir dəyişən (Başlangıçda kilitli olmalıdır)

LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);

  // LCD ekranını başlat
  lcd.init();
  lcd.backlight();

  // SPI və RFID modulunu başlat
  SPI.begin();
  rfid.PCD_Init();

  // LED pinlərini çıxış olaraq təyin edin
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  // Başlangıcdakı qırmızı LED-i söndür
  digitalWrite(RED_LED_PIN, LOW);

  // Başlangıcdakı yaşıl LED-i yandır
  digitalWrite(GREEN_LED_PIN, HIGH);
}

void loop() {
  // LCD ekranına xoş gəldiniz mesajını yazın
  lcd.setCursor(3, 0);
  lcd.print("Giriş sistem");

  // Kart oxuyarkən animasiyanı göstərin
  for (int i = 0; i < 3; i++) {
    lcd.setCursor(i, 1);
    lcd.print(".");
    delay(500);
  }
  lcd.clear();

  // Yeni bir kartın varlığını yoxlayın
  if (!rfid.PICC_IsNewCardPresent())
    return;

  // Kart oxunmasını təmin edin
  if (!rfid.PICC_ReadCardSerial())
    return;

  // Kartın UID'sini oxuyun
  String ID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    ID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    ID.concat(String(rfid.uid.uidByte[i], HEX));
  }
  ID.toUpperCase();

  // Kartın yetkiləndirilərək açılması
  if ((ID.substring(1) == UID1 || ID.substring(1) == UID2 || ID.substring(1) == UID3) && kilidDurumu == 0) {
    // LED'ları uyğun vəziyyətə gətir
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(RED_LED_PIN, LOW);

    // LCD ekranında məlumatı göstər
    lcd.setCursor(0, 0);
    if (ID.substring(1) == UID1) {
      lcd.print(ad1 + " Acildi");
    } else if (ID.substring(1) == UID2) {
      lcd.print(ad2 + " Acildi");
    } else if (ID.substring(1) == UID3) {
      lcd.print(ad3 + " Acildi");
    }
    delay(1500);
    lcd.clear();
    kilidDurumu = 1;  // Kilidi bağla
  }
  // Kartın yetkiləndirilərək bağlanılması və ya Yanlis kart oxunması
  else {
    // LED'ları uyğun vəziyyətə gətir
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);

    // LCD ekranında məlumatı göstər
    lcd.setCursor(0, 0);
    if (ID.substring(1) == UID1) {
      lcd.print(ad1 + " Yanlis kart");
    } else if (ID.substring(1) == UID2) {
      lcd.print(ad2 + " Yanlis kart");
    } else if (ID.substring(1) == UID3) {
      lcd.print(ad3 + " Yanlis kart");
    }
    delay(1500);
    lcd.clear();

    // Yanlis kart oxunsa da kilidi açma
    kilidDurumu = 1;
  }
}

