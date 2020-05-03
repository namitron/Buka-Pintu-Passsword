#include <DFPlayer_Mini_Mp3.h>
#include <SoftwareSerial.h>
#include <PCM.h>
#include <Keypad.h>
#include <Password.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#define TouchSensor 10 // Pin Di gunakan untuk capactitive touch sensor

 
#define RELAY_ON 0
#define RELAY_OFF 1
#define RELAY_1  12   // pin yang digunakan yaitu pin 12

byte currentLength = 0;

boolean currentState = LOW;
boolean lastState = LOW;
boolean RelayState = LOW;

Password password = Password("3131");//Password bisa di ubah ubah
LiquidCrystal_I2C  lcd(0x27,2,1,0,4,5,6,7);;// pin lcd yang di gunakan

const byte ROWS= 4; //baris pada keypad
const byte COLS= 4; //Kolom pada keypad

//inisialisasi led dan relay sebagai output bila password betul maka akan menyala relay,
//dan bila password salah akan menyala led merah

const int red = 13;//pin yang digunakan

/*keymap mendefinisikan tombol ditekan sesuai
dengan baris dan kolom seperti muncul pada keypad*/
char keys[ROWS][COLS] =
{
{'1', '2', '3', 'A'},
{'4', '5', '6', 'B'},
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};


byte rowPins[ROWS]= {9,8,7,6}; 
byte colPins[COLS]= {5,4,3,2}; 


Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );




void setup()
{
Serial.begin(9600);
mp3_set_serial (Serial);
mp3_set_volume (30); //volume 0-30
pinMode(13, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(RELAY_1, OUTPUT);
  pinMode(TouchSensor, INPUT);
  digitalWrite(RELAY_1, RELAY_OFF);

  //sumber diambil pada examples di library password.
  keypad.addEventListener(keypadEvent);
 lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.begin(16, 2);
  lcd.setCursor(4, 0);
  lcd.print("Password");
  
  lcd.setCursor(0, 1);
  lcd.print("NAMITRON");
  delay(1000);
 
}

// Jika tombol ditekan, tombol ini disimpan dalam 'keypressed' variabel
// Jika kunci tidak sama dengan 'NO_KEY', maka tombol ini dicetak
// Jika jumlah = 17, maka penghitungan reset kembali ke 0 (ini berarti tidak ada tombol yang ditekan selama proses pemindaian seluruh keypad

void loop()
{
  lcd.setBacklight(HIGH);
  lcd.setCursor(0, 0);
  lcd.print("Masukan Password");
  lcd.setCursor(0,1);
  lcd.print("Pass: ");
  keypad.getKey();;

//code touch sensor
  currentState = digitalRead(TouchSensor);
  if (currentState == HIGH && lastState == LOW) {
    Serial.println("Berhasil dibuka 5 detik");
   

    if (RelayState == HIGH) {
      digitalWrite(RELAY_1, RELAY_OFF);
      RelayState = LOW;
      
    } 
    else {
      lcd.setBacklight(HIGH);
      digitalWrite(RELAY_1, RELAY_ON);
      RelayState = HIGH;
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Berhasil Buka dalam");
      mp3_play(1);
      lcd.setCursor(3,1);
      lcd.print("dari Dalam");
     
      lcd.clear();
      password.reset();
      digitalWrite(RELAY_1, RELAY_OFF);
    }
  }
  lastState = currentState;
  
}


void checkPassword(){
   if(password.evaluate()){        
      lcd.setBacklight(HIGH);
      digitalWrite(RELAY_1, RELAY_ON); 
      mp3_play();
       lcd.clear();
      lcd.print("Berhasil");
      Serial.println("Berhasil dibuka 5 detik"); 
      delay(5000);
      
     
     
     
      
      password.reset();
      digitalWrite(RELAY_1, RELAY_OFF);
      
   }
   else {
    mp3_play(3);
      digitalWrite(13, HIGH);
      digitalWrite(13, LOW);

      lcd.setBacklight(HIGH);
      digitalWrite(red, HIGH);
      lcd.clear();
      lcd.print("Salah coba lagi");
      Serial.println("Salah coba lagi");
      delay(1000);//lama led on 
      password.reset();
            digitalWrite(13, HIGH);
      digitalWrite(13, LOW);
      digitalWrite(red, LOW);  
   }
}


void keypadEvent(KeypadEvent eKey){
  
  switch (keypad.getState()){  
    case PRESSED:
  lcd.setCursor(0,1);
    lcd.print(eKey);
    Serial.println(eKey);
  switch (eKey){
    case '*': checkPassword(); 
    lcd.clear(); currentLength=0;break;
    case '#': password.reset();
    lcd.clear();currentLength=0; break;
    default:// password.append(eKey);
          password << eKey;
          currentLength++;
     
        //Print some feedback.
        lcd.setCursor(0,1);
        lcd.print("Pass: ");
        for (byte i=0; i<currentLength; i++){
            lcd.print('*');
        }
           
   }
  }  
}  
