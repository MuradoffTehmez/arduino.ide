#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

// Yetkilendirilmiş kartların UID'lerini tanımlayın
String UID1 = "FA 70 EA AE";
String UID2 = "3C 68 CF 33";
String UID3 = "1C 1C CF 33";

// LED'lerin pinlerini tanımlayın
#define RED_LED_PIN 7
#define GREEN_LED_PIN 8

byte lock = 0;  // Kilidin durumunu göstermek için sınırlı bir değişken

LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);

  // LCD ekranını başlat
  lcd.init();
  lcd.backlight();

  // SPI ve RFID modülünü başlat
  SPI.begin();
  rfid.PCD_Init();

  // LED pinlerini çıkış olarak ayarla
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  // Başlangıçta kırmızı LED'i kapat
  digitalWrite(RED_LED_PIN, LOW);

  // Başlangıçta yeşil LED'i kapat
  digitalWrite(GREEN_LED_PIN, LOW);
}

void loop() {
  // LCD ekranına hoş geldiniz mesajı yazdır
  lcd.setCursor(4, 0);
  lcd.print("Xos geldiniz!");
  lcd.setCursor(2, 2);
  lcd.print("Kartinizi qoyun");

  // Yeni bir kartın varlığını kontrol et
  if (!rfid.PICC_IsNewCardPresent())
    return;

  // Kart okunmasını sağla
  if (!rfid.PICC_ReadCardSerial())
    return;

  // Kart okunurken ekranda skan ediliyor mesajı göster
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Skan edilir");

  // Kartın UID'sini oku
  String ID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    lcd.print(".");
    ID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    ID.concat(String(rfid.uid.uidByte[i], HEX));
    delay(300);
  }
  ID.toUpperCase();

  // Kartın yetkilendirilip açılması
  if ((ID.substring(1) == UID1 || ID.substring(1) == UID2 || ID.substring(1) == UID3) && lock == 0) {
    // Yeşil LED'i yak
    digitalWrite(GREEN_LED_PIN, HIGH);
    // Kırmızı LED'i söndür
    digitalWrite(RED_LED_PIN, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qapi baglidir");
    delay(1500);
    lcd.clear();
    lock = 1;  // Kilidi kapat
  }
  // Kartın yetkilendirilip kapatılması
  else if ((ID.substring(1) == UID1 || ID.substring(1) == UID2 || ID.substring(1) == UID3) && lock == 1) {
    // Yeşil LED'i söndür
    digitalWrite(GREEN_LED_PIN, LOW);
    // Kırmızı LED'i yak
    digitalWrite(RED_LED_PIN, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qapi aciqdir");
    delay(1500);
    lcd.clear();
    lock = 0;  // Kilidi aç
  }
  // Yanlış kart okunması durumu
  else {
    // Yeşil LED'i söndür
    digitalWrite(GREEN_LED_PIN, LOW);
    // Kırmızı LED'i yak
    digitalWrite(RED_LED_PIN, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Yanlis kart!");
    delay(1500);
    lcd.clear();
    // Kart yanlış okunduğunda kilidi açma
    lock = 1;
  }
}
















// #include <LiquidCrystal_I2C.h>
// #include <SPI.h>
// #include <MFRC522.h>

// #define SS_PIN 10
// #define RST_PIN 9

// String UID = "FA 70 EA AE"; // Yetkiləndirilmiş kartın UID-ni tanımlayın
// byte lock = 0; // Kilidin durumunu göstərmək üçün məhdudiyyət dəyişkəni

// LiquidCrystal_I2C lcd(0x27, 16, 2);
// MFRC522 rfid(SS_PIN, RST_PIN);

// // LED'lerin pinlerini tanımlayın
// #define RED_LED_PIN 7
// #define GREEN_LED_PIN 8

// void setup() {
//   Serial.begin(9600);

//   // LCD ekranını başlat
//   lcd.init();
//   lcd.backlight();

//   // SPI ve RFID modülünü başlat
//   SPI.begin();
//   rfid.PCD_Init();

//   // LED pinlerini çıkış olarak ayarla
//   pinMode(RED_LED_PIN, OUTPUT);
//   pinMode(GREEN_LED_PIN, OUTPUT);

//   // Başlangıçta kırmızı LED'i kapat
//   digitalWrite(RED_LED_PIN, LOW);

//   // Başlangıçta yeşil LED'i kapat
//   digitalWrite(GREEN_LED_PIN, LOW);
// }

// void loop() {
//   // LCD ekranına hoş geldiniz mesajı yazdır
//   lcd.setCursor(4, 0);
//   lcd.print("Xos geldiniz!");
//   lcd.setCursor(2, 2);
//   lcd.print("Kartinizi qoyun");

//   // Yeni bir kartın varlığını kontrol et
//   if (!rfid.PICC_IsNewCardPresent())
//     return;

//   // Kart okunmasını sağla
//   if (!rfid.PICC_ReadCardSerial())
//     return;

//   // Kart okunurken ekranda skan ediliyor mesajı göster
//   lcd.clear();
//   lcd.setCursor(0, 0);
//   lcd.print("Skan edilir");

//   // Kartın UID'sini oku
//   String ID = "";
//   for (byte i = 0; i < rfid.uid.size; i++) {
//     lcd.print(".");
//     ID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
//     ID.concat(String(rfid.uid.uidByte[i], HEX));
//     delay(300);
//   }
//   ID.toUpperCase();

//   // Kartın yetkilendirilip açılması
//   if (ID.substring(1) == UID && lock == 0) {
//     // Yeşil LED'i yak
//     digitalWrite(GREEN_LED_PIN, HIGH);
//     // Kırmızı LED'i söndür
//     digitalWrite(RED_LED_PIN, LOW);
//     lcd.clear();
//     lcd.setCursor(0, 0);
//     lcd.print("Qapi baglidir");
//     delay(1500);
//     lcd.clear();
//     lock = 1; // Kilidi kapat
//   }
//   // Kartın yetkilendirilip kapatılması
//   else if (ID.substring(1) == UID && lock == 1) {
//     // Yeşil LED'i söndür
//     digitalWrite(GREEN_LED_PIN, LOW);
//     // Kırmızı LED'i yak
//     digitalWrite(RED_LED_PIN, HIGH);
//     lcd.clear();
//     lcd.setCursor(0, 0);
//     lcd.print("Qapi aciqdir");
//     delay(1500);
//     lcd.clear();
//     lock = 0; // Kilidi aç
//   }
//   // Yanlış kart okunması durumu
//   else {
//     // Yeşil LED'i söndür
//     digitalWrite(GREEN_LED_PIN, LOW);
//     // Kırmızı LED'i yak
//     digitalWrite(RED_LED_PIN, HIGH);
//     lcd.clear();
//     lcd.setCursor(0, 0);
//     lcd.print("Yanlis kart!");
//     delay(1500);
//     lcd.clear();
//   }
// }











// #include <Servo.h>
// #include <LiquidCrystal_I2C.h>
// #include <SPI.h>
// #include <MFRC522.h>

// #define SS_PIN 10
// #define RST_PIN 9

// // Yetkilendirilmiş kartların UID'lerini tanımlayın
// String UID1 = "FA 70 EA AE";
// String UID2 = "3C 68 CF 33";
// String UID3 = "1C 1C CF 33";

// byte lock = 0; // Kilidin durumunu göstermek için sınırlı bir değişken

// Servo servo;
// LiquidCrystal_I2C lcd(0x27, 16, 2);
// MFRC522 rfid(SS_PIN, RST_PIN);

// void setup() {
//   Serial.begin(9600);

//   // Servo motorunu başlangıç pozisyonuna getir
//   servo.write(70);

//   // LCD ekranını başlat
//   lcd.init();
//   lcd.backlight();

//   // Servo motorunu bağla
//   servo.attach(3);

//   // SPI ve RFID modülünü başlat
//   SPI.begin();
//   rfid.PCD_Init();
// }

// void loop() {
//   // LCD ekranına hoş geldiniz mesajı yazdır
//   lcd.setCursor(4, 0);
//   lcd.print("Xos geldiniz!");
//   lcd.setCursor(2, 2);
//   lcd.print("Kartinizi qoyun");

//   // Yeni bir kartın varlığını kontrol et
//   if (!rfid.PICC_IsNewCardPresent())
//     return;

//   // Kart okunmasını sağla
//   if (!rfid.PICC_ReadCardSerial())
//     return;

//   // Kart okunurken ekranda skan ediliyor mesajı göster
//   lcd.clear();
//   lcd.setCursor(0, 0);
//   lcd.print("Skan edilir");

//   // Kartın UID'sini oku
//   String ID = "";
//   for (byte i = 0; i < rfid.uid.size; i++) {
//     lcd.print(".");
//     ID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
//     ID.concat(String(rfid.uid.uidByte[i], HEX));
//     delay(300);
//   }
//   ID.toUpperCase();

//   // Kartın yetkilendirilip açılması
//   if ((ID.substring(1) == UID1 || ID.substring(1) == UID2 || ID.substring(1) == UID3) && lock == 0) {
//     servo.write(70); // Servo motorunu belirli bir açıya getir
//     lcd.clear();
//     lcd.setCursor(0, 0);
//     lcd.print("Qapi baglidir");
//     delay(1500);
//     lcd.clear();
//     lock = 1; // Kilidi kapat
//   }
//   // Kartın yetkilendirilip kapatılması
//   else if ((ID.substring(1) == UID1 || ID.substring(1) == UID2 || ID.substring(1) == UID3) && lock == 1) {
//     servo.write(160); // Servo motorunu belirli bir açıya getir
//     lcd.clear();
//     lcd.setCursor(0, 0);
//     lcd.print("Qapi aciqdir");
//     delay(1500);
//     lcd.clear();
//     lock = 0; // Kilidi aç
//   }
//   // Yanlış kart okunması durumu
//   else {
//     lcd.clear();
//     lcd.setCursor(0, 0);
//     lcd.print("Yanlis kart!");
//     delay(1500);
//     lcd.clear();
//   }
// }





























// #include <Servo.h>

// Servo servo;  // Servo motor nesnesi oluşturulur

// void setup() {
//   servo.attach(8);  // Servo motorun bağlı olduğu pin belirtilir (örneğin, 3. pin)
// }

// void loop() {
//   // Servo motoru 0 ile 180 derece arasında döndürerek test eder
//   for (int angle = 0; angle <= 180; angle += 10) {
//     servo.write(angle);  // Servo motoru belirtilen açıya döndürür
//     delay(500);  // 500 milisaniye bekler
//   }

//   // Servo motoru 180 ile 0 derece arasında döndürerek test eder
//   for (int angle = 180; angle >= 0; angle -= 10) {
//     servo.write(angle);  // Servo motoru belirtilen açıya döndürür
//     delay(500);  // 500 milisaniye bekler
//   }
// }










// #include <I2C_16Bit.h>
// #include <I2C_32Bit.h>
// #include <I2C_8Bit.h>

// #include <LiquidCrystal_I2C.h>
// #include <SPI.h>
// #include <MFRC522.h>

// #define RST_PIN 9
// #define SS_PIN  10
// byte readCard[4];
// byte a = 0;

// LiquidCrystal_I2C lcd(0x27, 16, 2);
// MFRC522 mfrc522(SS_PIN, RST_PIN);

// void setup() {
//   Serial.begin(9600);
//   lcd.init();
//   lcd.backlight();
//   while (!Serial);
//   SPI.begin();
//   mfrc522.PCD_Init();
//   delay(4);
//   mfrc522.PCD_DumpVersionToSerial();
//   lcd.setCursor(2, 0);
//   lcd.print("Put your card");
// }

// void loop() {
//   if ( ! mfrc522.PICC_IsNewCardPresent()) {
//     return 0;
//   }
//   if ( ! mfrc522.PICC_ReadCardSerial()) {
//     return 0;
//   }

//   lcd.clear();
//   lcd.setCursor(0, 0);
//   lcd.print("Scanned UID");
//   a = 0;
//   Serial.println(F("Scanned PICC's UID:"));
//   for ( uint8_t i = 0; i < 4; i++) {  //
//     readCard[i] = mfrc522.uid.uidByte[i];
//     Serial.print(readCard[i], HEX);
//     Serial.print(" ");
//     lcd.setCursor(a, 1);
//     lcd.print(readCard[i], HEX);
//     lcd.print(" ");
//     delay(500);
//     a += 3;
//   }
//   Serial.println("");
//   mfrc522.PICC_HaltA();
//   return 1;
// }
// #include <SPI.h>      // SPI kütüphanesi
// #include <MFRC522.h>  // MFRC522 kütüphanesi
// #include <Servo.h>    // Servo kütüphanesi
// #include <EEPROM.h>

// int RST_PIN = 9;       // RC522 modülü reset pini
// int SS_PIN = 10;       // RC522 modülü chip select pini
// int servoPin = 8;      // Servo motor pini

// Servo motor;                     // Servo motor değişkeni
// MFRC522 rfid(SS_PIN, RST_PIN);   // RC522 modülü
// byte ID[4] = {0xFF, 0xFF, 0xFF, 0xFF};  // Yetkili kart ID'si (örneğin)

// // EEPROM adresleri
// #define EEPROM_START_ADDR 0
// #define ID_LENGTH 4

// // Servo motorun tam açma ve kapanma değerleri
// int openPosition = 180;
// int closedPosition = 0;

// // EEPROM'ye kart ID'lerini yazma
// void writeIDToEEPROM(byte *ID) {
//   for (int i = 0; i < ID_LENGTH; i++) {
//     EEPROM.write(EEPROM_START_ADDR + i, ID[i]);
//   }
// }

// // EEPROM'dan kart ID'lerini okuma
// void readIDFromEEPROM(byte *ID) {
//   for (int i = 0; i < ID_LENGTH; i++) {
//     ID[i] = EEPROM.read(EEPROM_START_ADDR + i);
//   }
// }

// // Kart yetkilendirme işlemi
// void checkAuthorization(byte *ID) {
//   if (rfid.uid.uidByte[0] == ID[0] && rfid.uid.uidByte[1] == ID[1] && rfid.uid.uidByte[2] == ID[2] && rfid.uid.uidByte[3] == ID[3]) {
//     Serial.println("Yetkili kart algılandı. Kapı açılıyor...");
//     motor.write(openPosition);  // Servo motoru açık konuma getirme
//     delay(3000);
//     motor.write(closedPosition);  // Servo motoru kapalı konuma getirme
//     delay(1000);
//   } else {
//     Serial.println("HATA: Yetkisiz kart algılandı!");
//   }
// }

// void setup() {
//   motor.attach(servoPin);    // Servo motor pinini motor değişkeni ile ilişkilendirme
//   Serial.begin(9600);        // Seri haberleşmeyi başlatma
//   SPI.begin();               // SPI iletişimini başlatma
//   rfid.PCD_Init();           // RC522 modülünü başlatma
//   // EEPROM'dan kart ID'lerini okuma
//   readIDFromEEPROM(ID);
// }

// void loop() {
//   // Yeni kartın varlığını kontrol etme
//   if (!rfid.PICC_IsNewCardPresent())
//     return;

//   // Kartı okuma işlemi
//   if (!rfid.PICC_ReadCardSerial())
//     return;

//   // Kartın yetkilendirilip yetkilendirilmediğini kontrol etme
//   checkAuthorization(ID);

//   // Kart işlemleri sonlandırma
//   rfid.PICC_HaltA();
// }



// #include <SPI.h>      //SPI kütüphanemizi tanımlıyoruz.
// #include <MFRC522.h>  //MFRC522 kütüphanemizi tanımlıyoruz.
// #include <Servo.h>    //Servo kütüphanemizi tanımlıyoruz.

// int RST_PIN = 9;   //RC522 modülü reset pinini tanımlıyoruz.
// int SS_PIN = 10;   //RC522 modülü chip select pinini tanımlıyoruz.
// int servoPin = 3;  //Servo motor pinini tanımlıyoruz.

// Servo motor;                     //Servo motor için değişken oluşturuyoruz.
// MFRC522 rfid(SS_PIN, RST_PIN);   //RC522 modülü ayarlarını yapıyoruz.
// byte ID[4] = {0x250, 0x112, 0x234, 0x174};  //Yetkili kart ID'sini tanımlıyoruz.

// void setup() {
//   motor.attach(servoPin);  //Servo motor pinini motor değişkeni ile ilişkilendiriyoruz.
//   Serial.begin(9600);      //Seri haberleşmeyi başlatıyoruz.
//   SPI.begin();             //SPI iletişimini başlatıyoruz.
//   rfid.PCD_Init();         //RC522 modülünü başlatıyoruz.
// }

// void loop() {

//   if (!rfid.PICC_IsNewCardPresent())  //Yeni kartın okunmasını bekliyoruz.
//     return;

//   if (!rfid.PICC_ReadCardSerial())  //Kart okunmadığı zaman bekliyoruz.
//     return;

//   if (rfid.uid.uidByte[0] == ID[0] &&  //Okunan kart ID'si ile ID değişkenini karşılaştırıyoruz.
//       rfid.uid.uidByte[1] == ID[1] && rfid.uid.uidByte[2] == ID[2] && rfid.uid.uidByte[3] == ID[3]) {
//     Serial.println("Kapi acildi");
//     ekranaYazdir();
//     motor.write(180);  //Servo motoru 180 dereceye getiriyoruz.
//     delay(3000);
//     motor.write(0);  //Servo motoru 0 dereceye getiriyoruz.
//     delay(1000);
//   } else {  //Yetkisiz girişte içerideki komutlar çalıştırılır.
//     Serial.println("Yetkisiz Kart");
//     ekranaYazdir();
//   }
//   rfid.PICC_HaltA();
// }
// void ekranaYazdir() {
//   Serial.print("ID Numarasi: ");
//   for (int sayac = 0; sayac < 4; sayac++) {
//     Serial.print(rfid.uid.uidByte[sayac]);
//     Serial.print(" ");
//   }
//   Serial.println("");
// }
