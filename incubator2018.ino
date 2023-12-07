#include <EEPROM.h>
#include <dht11.h>
#include <LiquidCrystal.h>
#include <DS1302RTC.h>
#include <TimeLib.h>
#include<Servo.h>



Servo sg90;
DS1302RTC RTC(5, 6, 7);
# define role 4
int aci = 90;
int yumurta_gun = 1;
int servo = 0;
float sicaklik = 0;
const int Lm35 = A5;

int sifirla = 0;
int gelen_veri = 0;
float voltaj_deger = 0;

float ortalama = 0;
int say =0;
int address = 50;                                    // eepromdaki değişken adres
int kayit = 0;                                  // eepromdan okunarak tutulan data

int servo_konum=0;

//            lcd(RS,  E, d4, d5, d6, d7)
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
dht11 DHT11;
int saat = 0;
int dakika = 0;
int saniye = 0;
int yil = 0;
int ay = 0;
int gun = 0;
int alt_seviye=0;
int ust_seviye=0;


int layer[4] = {A0, A1, A2, A3};
int chk = 0;
#define DHT11_PIN A3 // Sensor is connected to Arduino pin 2
void setup()
{
  // Setup LCD to 16x2 characters

  //  DHT11.attach(2);
  Serial.begin(9600);
  lcd.begin(16, 2);

 

  for (int i = 0; i < 4; i++)
  {
    pinMode(layer[i], OUTPUT);
  }
  pinMode(role, OUTPUT);

  lcd.clear();
  lcd.setCursor (0, 0);
  lcd.println("KULUCKA MAKINASI");
  lcd.setCursor (0, 0);
  lcd.println("  BASLATILIYOR  ");
  delay(2000);
  lcd.clear();

  lcd.setCursor (0, 3);
  lcd.println("HAZIRLAYAN");
  lcd.setCursor (0, 2);
  lcd.println("AhmetBahadir");
  delay(2000);
  lcd.clear();



  sg90.attach(3);
  delay(1000);

  // Check clock oscillation
  lcd.clear();
  if (RTC.haltRTC())
    lcd.print("Saat durdu!");
  else
    lcd.print("Saat aktif");

  // Check write-protection
  lcd.setCursor(0, 1);
  if (RTC.writeEN())
    lcd.print("Yazmaya izin ver.");
  else
    lcd.print("Yazma korumali.");

  delay (200);

  // Setup Time library
  lcd.clear();
  lcd.print("RTC Senk.");
  setSyncProvider(RTC.get); // the function to get the time from the RTC
  if (timeStatus() == timeSet)
    lcd.print(" Tamam!");
  else
    lcd.print(" HATALI!");

  delay (200);



  lcd.clear();
  
  yumurta_gun = EEPROM.read(address);                  // adresten yumurta_guni oku
}

void loop()
{
  
EEPROM.write(address,(yumurta_gun));                 // eeprom adrese değeri yaz

    sifirla = analogRead(A4);
    if (sifirla > 500)
    {
      yumurta_gun = 1;
    }
  saniye = second();
  saat = hour();
  dakika = minute();
  gun = day();
  ay = month();
  yil = year();
  lcd.setCursor (0, 1);
  lcd.setCursor (0, 1);
  if (ay < 10)
  {
    lcd.print("0");
  }
  else
  {
    lcd.print("-");
  }
  lcd.print(ay);
  if (gun < 10)
  {
    lcd.print("-0");
  }
  else
  {
    lcd.print("-");
  }
  lcd.print(gun);
  lcd.setCursor (8, 1);
  if (saat < 10)
  {
    lcd.print("0");
  }
  lcd.print(saat);
  if (dakika < 10)
  {
    lcd.print("-0");
  }
  else
  {
    lcd.print("-");
  }
  lcd.print(dakika);
  if (saniye < 10)
  {
    lcd.print("-0");
  }
  else
  {
    lcd.print("-");
  }
  lcd.print(saniye);

  ortalama = 0;
  for (int a = 0; a < 10; a++)
  {

    gelen_veri = analogRead(Lm35);
    voltaj_deger = ((gelen_veri / 1023.0) *6000);
    sicaklik = voltaj_deger / 10.0;
    ortalama = ortalama + sicaklik;
    delay(100);
  }
  float ort = ortalama / 10;

  chk = DHT11.read(DHT11_PIN);
  lcd.setCursor (0, 0);
  lcd.print("%");
  lcd.print((float)DHT11.humidity, DEC);
  lcd.setCursor (3, 0);
  //  sicaklik = (float)DHT11.temperature;
  lcd.print(" ");
  lcd.print(ort);
  lcd.print("C");

     lcd.setCursor (10, 0);
    
  lcd.print(" ");
  lcd.print(yumurta_gun);
  lcd.print(".GUN ");

  digitalWrite(role, HIGH); // ısıtıcı açık
  digitalWrite(layer[1], 1);//kırmızı





    if (saat == 12 && dakika == 0 && saniye < 3 )
  {
    yumurta_gun++;
    if (yumurta_gun == 22)
    {
      yumurta_gun =1;
    }

     lcd.setCursor (10, 0);
    
  lcd.print(" ");
  lcd.print(yumurta_gun);
  lcd.print(".GUN ");
    delay(3000);
  }

  if (yumurta_gun <= 18)
  {
    alt_seviye = 36.9;
    ust_seviye = 37.7;
  }

    if (yumurta_gun > 18)
  {
    alt_seviye = 36;
    ust_seviye = 36.7;
    servo_konum = 1;
  }

  if (ort > 38.1)

  {
    digitalWrite(role, LOW);// ısıtıcı kapalı
    lcd.setCursor (0, 0);
    lcd.print("%");
    lcd.print((float)DHT11.humidity, DEC);
    lcd.setCursor (3, 0);
    //  sicaklik = (float)DHT11.temperature;
    lcd.print(" C:");
    lcd.print(ort);
    lcd.setCursor (10, 0);
    
  lcd.print(" ");
  lcd.print(yumurta_gun);
  lcd.print(".GUN ");
    digitalWrite(layer[0], 1); //buzzer
    digitalWrite(layer[1], 1);//kırmızı
    delay(100);
    digitalWrite(layer[0], 0); //buzzer
    digitalWrite(layer[1], 0);//kırmızı
    delay(50);
  }

  if (ort >= ust_seviye)
  {

    
    while (say == 0)
    {
      ortalama = 0;
      for (int a = 0; a < 10; a++)
      {

        gelen_veri = analogRead(Lm35);
        voltaj_deger = ((gelen_veri / 1023.0) * 6000);
        sicaklik = voltaj_deger / 10.0;
        ortalama = ortalama + sicaklik;
        delay(100);
      }
      float ort = ortalama / 10;
      if (ort < alt_seviye)
      {
        say=1;
      }

          sifirla = analogRead(A4);
    if (sifirla > 500)
    {
      yumurta_gun = 1;
    }
      digitalWrite(role, LOW); // ısıtıcı kapalı
      digitalWrite(layer[1], 0);//kırmızı


  saniye = second();
  saat = hour();
  dakika = minute();
  gun = day();
  ay = month();
  yil = year();
  lcd.setCursor (0, 1);
  if (ay < 10)
  {
    lcd.print("0");
  }
  else
  {
    lcd.print("-");
  }
  lcd.print(ay);
  if (gun < 10)
  {
    lcd.print("-0");
  }
  else
  {
    lcd.print("-");
  }
  lcd.print(gun);
  lcd.setCursor (8, 1);
  if (saat < 10)
  {
    lcd.print("0");
  }
  lcd.print(saat);
  if (dakika < 10)
  {
    lcd.print("-0");
  }
  else
  {
    lcd.print("-");
  }
  lcd.print(dakika);
  if (saniye < 10)
  {
    lcd.print("-0");
  }
  else
  {
    lcd.print("-");
  }
  lcd.print(saniye);



      
      lcd.setCursor (0, 0);
      lcd.print("%");
      lcd.print((float)DHT11.humidity, DEC);
      lcd.setCursor (3, 0);
      //  sicaklik = (float)DHT11.temperature;
      lcd.print(" ");
      lcd.print(ort);
      lcd.print("C");
        lcd.setCursor (10, 0);
    
  lcd.print(" ");
  lcd.print(yumurta_gun);
  lcd.print(".GUN ");
    }
  }
       lcd.setCursor (10, 0); 
  lcd.print(" ");
  lcd.print(yumurta_gun);
  lcd.print(".GUN ");

  digitalWrite(role, HIGH); // ısıtıcı açık
  digitalWrite(layer[1], 1);//kırmızı

say=0;

if (servo_konum == 0)
{
  if (saat == 0 || saat == 1 || saat == 4 || saat == 5 || saat == 8 || saat == 9 || saat == 12 || saat == 13 || saat == 16 || saat == 17 || saat == 20 || saat == 21 )
  {
    for (int a = aci; a > 40; a--)
    {
      digitalWrite(layer[2], 0);//sarı
      digitalWrite(layer[3], 1);//mavi
      sg90.attach(3);
      sg90.write(a);
      aci = a;
      delay(100);
    }
  }
  if (saat == 2 || saat == 3 || saat == 6 || saat == 7 || saat == 10 || saat == 11 || saat == 14 || saat == 15 || saat == 18 || saat == 19 || saat == 22 || saat == 23)

  {
    for (int a = aci; a < 140; a++)
    {
      digitalWrite(layer[2], 1);//sarı
      digitalWrite(layer[3], 0);//mavi
      sg90.attach(3);
      sg90.write(a);
      aci = a;
      delay(100);
    }
  }

}

}
