// Drop Diverter Arduino code. Aryan A. 02/23/2023
int startPin = 4;
int pumpPin = 7;
int sewerPin = 10;
int tankPin = 8;
int tankLedPin = 11;
int sewerLedPin = 12;
int buzzer = 9;

int soakingBathTime = 3000;
int washingBathTime = 3000;
int comfortBathTime = 3000;

int flowCount = 0;
int flowCheckInterval = 200;
unsigned long currTime, loopTime;
boolean startGame = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(startPin, INPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(sewerPin, OUTPUT);
  pinMode(sewerLedPin, OUTPUT);
  pinMode(tankPin, OUTPUT);
  pinMode(tankLedPin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  attachInterrupt(0, countFlow, RISING);
  Serial.begin(9600);
  currTime = millis();
  loopTime = currTime;
}

void loop() {
  int buttonState = digitalRead(startPin);
  if (buttonState == 1) {
    // Start button pressed
    startWaterPump();
    buttonState = 0;
    startGame = true;
    delay(2500);
    flowCount = 0;
    shutWater();
  }

  if (startGame) {
    if (isFlowing()) {
      // Water to tank
      waterToTank();
      delay(soakingBathTime);
      // Water to sewer.
      waterToSewer();
      delay(washingBathTime);
      // Water to tank.
      waterToTank();
      delay(comfortBathTime);
      // Stop pump.
      stopWaterPump();
      // Shut both valves.
      shutWater();
      flowCount = 0;
      startGame = false;
      delay(1000);
    }
  }
}

void startWaterPump() {
  digitalWrite(pumpPin, HIGH);
}

void stopWaterPump() {
  digitalWrite(pumpPin, LOW);
}

void waterToSewer() {
  //close tank valve, open sewer valve
  delay(200);
  digitalWrite(sewerPin, HIGH);
  digitalWrite(tankPin, LOW);
  digitalWrite(sewerLedPin, HIGH);
  digitalWrite(tankLedPin, LOW);
  buzz();
}

void waterToTank() {
  //open tank valve, close sewer valve
  delay(200);
  digitalWrite(tankPin, HIGH);
  digitalWrite(sewerPin, LOW);
  digitalWrite(tankLedPin, HIGH);
  digitalWrite(sewerLedPin, LOW);
  buzz2();
}

void buzz() { // single buzzer
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
}
void buzz2() { // pattern buzzer
  digitalWrite(buzzer, HIGH);
  delay(80);
  digitalWrite(buzzer, LOW);
  delay(50);
  digitalWrite(buzzer, HIGH);
  delay(80);
  digitalWrite(buzzer, LOW);
  delay(50);
  digitalWrite(buzzer, HIGH);
  delay(80);
  digitalWrite(buzzer, LOW);
}

void shutWater() {
  // close both valves
  digitalWrite(tankPin, LOW);
  digitalWrite(sewerPin, LOW);
  digitalWrite(tankLedPin, LOW);
  digitalWrite(sewerLedPin, LOW);
}

boolean isFlowing() {
  currTime = millis();
  if (currTime >= (loopTime + flowCheckInterval)) {
    if (flowCount > 20) {
      loopTime = currTime;
      flowCount = 0;
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

void countFlow() {
  flowCount = flowCount + 1;
}