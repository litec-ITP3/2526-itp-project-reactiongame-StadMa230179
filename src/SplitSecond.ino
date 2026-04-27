const int button1 = 6;
const int button2 = 8;
const int led1 = 5;
const int led2 = 7;

unsigned long startZeit;
unsigned long zeitP1 = 0;
unsigned long zeitP2 = 0;
bool spielLaeuft = false;

void setup() {
  Serial.begin(9600);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}

void loop() {
  if (!spielLaeuft) {
    if (digitalRead(button1) == LOW || digitalRead(button2) == LOW) {
      Serial.println("START");
      zeitP1 = 0;
      zeitP2 = 0;

      for (int i = 0; i < 3; i++) {
        digitalWrite(led1, HIGH); digitalWrite(led2, HIGH);
        delay(300);
        digitalWrite(led1, LOW); digitalWrite(led2, LOW);
        delay(300);
      }

      unsigned long warteDauer = random(2000, 5000);
      unsigned long warteStart = millis();
      
      while (millis() - warteStart < warteDauer) {
        if (digitalRead(button1) == LOW) {
          Serial.println("ERR:P1_FEHLSTART");
          blinkError(led1);
          return; 
        }
        if (digitalRead(button2) == LOW) {
          Serial.println("ERR:P2_FEHLSTART");
          blinkError(led2);
          return;
        }
      }

      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      startZeit = millis();
      spielLaeuft = true;
    }
  }

  if (spielLaeuft) {
    if (digitalRead(button1) == LOW && zeitP1 == 0) {
      zeitP1 = millis() - startZeit;
      Serial.print("P1:"); Serial.println(zeitP1);
      digitalWrite(led1, LOW);
    }
    if (digitalRead(button2) == LOW && zeitP2 == 0) {
      zeitP2 = millis() - startZeit;
      Serial.print("P2:"); Serial.println(zeitP2);
      digitalWrite(led2, LOW);
    }
    if ((zeitP1 > 0 && zeitP2 > 0) || (millis() - startZeit > 3000)) {
      spielLaeuft = false;
      Serial.println("ENDE");
      delay(2000);
    }
  }
}

void blinkError(int ledPin) {
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPin, HIGH); delay(100);
    digitalWrite(ledPin, LOW); delay(100);
  }
}