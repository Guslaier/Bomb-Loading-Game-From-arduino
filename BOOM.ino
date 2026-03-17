#include <Keypad_I2C.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define I2CADDR 0x20
#define LED_AWlestimeWorng 7
#define led_uL1 12
#define led_uL2 13
#define led_1 8
#define led_2 9
#define led_3 10
#define led_4 11
#define spaker 6
#define port1 2
#define port2 3
#define port3 4
#define port4 5
LiquidCrystal_I2C LCD(0x27, 16, 2);
String passSh;
String pass;
int passLength = 5;
byte portType;
long TimeSt;
long letTime;
const byte ROWS = 4;
const byte COLS = 4;
String dotset[14] = { "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", ".-", "-...", "-.-.", "-.." };
byte portSet[8] = { 0b1100, 0b0011, 0b1101, 0b1011, 0b1010, 0b0101, 0b0010, 0b0100 };
byte rowPins[ROWS] = { 7, 6, 5, 4 };
byte colPins[COLS] = { 3, 2, 1, 0 };
char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
Keypad_I2C keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS, I2CADDR);

double run;
String inPass;
String text_end = "     :#ToPss";
bool lockLavel_1, lockLavel_2;
bool stutGameRun;

void setup() {
  keypad.begin();
  Serial.begin(9600);
  Wire.begin();
  LCD.init();
  pinMode(LED_AWlestimeWorng, OUTPUT);
  pinMode(spaker, OUTPUT);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(led_4, OUTPUT);
  pinMode(led_uL1, OUTPUT);
  pinMode(led_uL2, OUTPUT);
  pinMode(port1, INPUT_PULLUP);
  pinMode(port2, INPUT_PULLUP);
  pinMode(port3, INPUT_PULLUP);
  pinMode(port4, INPUT_PULLUP);
  LCD.backlight();
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("    Starting   ");
  delay(500);
  setUpGame();
}

void setUpGame() {
  run = 0;
  inPass = "";
  lockLavel_1 = 0, lockLavel_2 = 0;
  stutGameRun = 1;
  passSh = "<";
  pass = "";
  letTime = 80;
  LCD.clear();
  randomSeed(analogRead(0));
  portType = portSet[random(8)];
  for (int i = 1; i <= passLength; i++) {
    int r = random(14);
    passSh = passSh + "|" + dotset[r];
    if (r == 10) {
      pass = pass + 'A';
    } else if (r == 11) {
      pass = pass + 'B';
    } else if (r == 12) {
      pass = pass + 'C';
    } else if (r == 13) {
      pass = pass + 'D';
    } else {
      pass = pass + r;
    }
  }
  passSh = passSh + "|> ";
  Serial.println("passS: " + passSh);
  Serial.println("pass: " + pass);
  LCD.setCursor(0, 0);
  S2P(portType);
  LCD.print("   Game Start");
  delay(1000);
  TimeSt = millis() / 1000;
  LCD.clear();
}
char key;
void loop() {
  if (stutGameRun == 1) {
    if (runGame() == 0) {
      stutGameRun = 0;
      digitalWrite(LED_AWlestimeWorng, LOW);
    }
  }else {
    LCD.setCursor(0, 1);
    LCD.print(" # for new Game");
    key = keypad.getKey();
    if(key == '#'){
      passKeySoun();
      setUpGame();
    }
  }

}


bool runGame() {
  if ((letTime - (millis() / 1000 - TimeSt)) <= 10) {
    digitalWrite(LED_AWlestimeWorng, HIGH);
  }
  if ((letTime - (millis() / 1000 - TimeSt)) <= 0 || (letTime - (millis() / 1000 - TimeSt)) > 4000) {
    LCD.clear();
    LCD.setCursor(0, 0);
    digitalWrite(LED_AWlestimeWorng, LOW);
    LCD.print("    BOOM!!!!  ");
    delay(100);
    analogWrite(spaker, 255);
    delay(3000);
    analogWrite(spaker, 0);
    return 0;
  } else if (lockLavel_1 == 0) {
    if (Chackconnet() == 1) {
      lockLavel_1 = 1;
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print(" Unlock lavel 1");
      delay(1000);
      digitalWrite(led_uL1, HIGH);
      LCD.clear();
    }
  } else if (lockLavel_2 == 0) {

    if (ChackPassWord() == 1) {
      lockLavel_2 = 1;
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print("  --  WIN  --  ");
      delay(1000);
      digitalWrite(led_uL2, HIGH);
      return 0;
    }
  }
  return 1;
}


bool Chackconnet() {
  LCD.setCursor(0, 0);
  LCD.print(" CHANGE Connect");
  LCD.setCursor(0, 1);
  LCD.print(String("Time left :") + (letTime - (millis() / 1000 - TimeSt)) + " s ");

  if (((portType & 1) - digitalRead(port1) != 0)
      && ((portType & 2) - digitalRead(port2) != 1)
      && ((portType & 4) - digitalRead(port3) != 3)
      && ((portType & 8) - digitalRead(port4) != 7)) {
    S2P(0b0000);
    return 1;
  }

  if ((portType & 1) - digitalRead(port1) != 0) {
    digitalWrite(led_1, HIGH);
  } else {
    digitalWrite(led_1, LOW);
  }
  if ((portType & 2) - digitalRead(port2) != 1) {
    digitalWrite(led_2, HIGH);
  } else {
    digitalWrite(led_2, LOW);
  }
  if ((portType & 4) - digitalRead(port3) != 3) {
    digitalWrite(led_3, HIGH);
  } else {
    digitalWrite(led_3, LOW);
  }
  if ((portType & 8) - digitalRead(port4) != 7) {
    digitalWrite(led_4, HIGH);
  } else {
    digitalWrite(led_4, LOW);
  }

  delay(500);
  return 0;
}

bool ChackPassWord() {
  LCD.setCursor(0, 0);
  LCD.print(passSh.substring(run));
  if (run > passSh.length() - 16) {
    run = 0;
  } else {
    run = run + 0.15;
  }
  LCD.setCursor(0, 1);
  key = keypad.getKey();
  if (key != NO_KEY) {
    passKeySoun();
    if (key == '*') {
      Serial.println(key);
      inPass = "";
    } else if (key == '#') {
      Serial.println(key);
      if (inPass != pass) {
        inPass = "";
        LCD.print("     False!!    ");
        letTime = letTime - 20;
        delay(800);
      } else if (inPass == pass) {
        return 1;
      }
    } else if (inPass.length() == passLength) {
      Serial.println(key);
      inPass = "";
    } else {
      Serial.println(key);
      inPass = inPass + key;
    }
  };
  LCD.print(inPass + text_end.substring(inPass.length()) + (letTime - (millis() / 1000 - TimeSt)) + "s ");
  return 0;
}

void S2P(byte LED) {
  if (LED & 1) {
    digitalWrite(led_1, LOW);
  } else {
    digitalWrite(led_1, HIGH);
  }
  if (LED & 2) {
    digitalWrite(led_2, LOW);
  } else {
    digitalWrite(led_2, HIGH);
  }
  if (LED & 4) {
    digitalWrite(led_3, LOW);
  } else {
    digitalWrite(led_3, HIGH);
  }
  if (LED & 8) {
    digitalWrite(led_4, LOW);
  } else {
    digitalWrite(led_4, HIGH);
  }
}


void passKeySoun() {
  analogWrite(spaker, 50);
  delay(100);
  analogWrite(spaker, 0);
}
