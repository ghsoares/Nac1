const int pinButtonAdd = 2;
const int pinButtonDec = 3;
const int pinButtonEnter = 4;
const int pinBomba = 9;
const int pinAquecedor = 10;
const int pinBuzzer = 11;
const int pinLed = 12;

const float timerStepAddDec = 5;
const int buzzerFrequency = 261;

bool settingTime = true;
bool timerRunning = false;
bool buzzing = false;
float setTime = 0; // Minutos
float currentTime = 0;
float prevTime = 0;
float deltaTime = 0;

String prevTimerOutput;

// fiz essa função para converter em hh:mm
String convertTime(float mins) {
  int minsInt = (int)ceil(mins);
  int hours = (minsInt / 60);
  int minutes = (minsInt - hours * 60);
  String str = "";
  str = str + hours + ":" + minutes;
  return str;
}

void setTimerLoop() {
  bool add = digitalRead(pinButtonAdd) == LOW;
  bool dec = digitalRead(pinButtonDec)== LOW;
  bool enter = digitalRead(pinButtonEnter) == LOW;
  
  if (add) {
    setTime += deltaTime * timerStepAddDec;
    setTime = min(setTime, 60*3); // Limita entre 0 Minutos - 3 horas
    Serial.println("Tempo setado: " + convertTime(setTime));
  }
  
  if (dec) {
    setTime -= deltaTime * timerStepAddDec;
    setTime = max(setTime, 0); // Limita entre 0 Minutos - 3 horas
    Serial.println("Tempo setado: " + convertTime(setTime));
  }
  
  if (enter) {
    settingTime = false;
    timerRunning = true;
    currentTime = setTime;

    digitalWrite(pinBomba, HIGH);
    digitalWrite(pinAquecedor, HIGH);
  }
}

// nn sei pq q continua a girar kkkkk

void timerLoop() {
  deltaTime *= 100.0; // Acelera essa parte da simulação para fins de teste
  
  currentTime -= deltaTime / 60.0;
  
  String currentTimeStr = convertTime(currentTime);
  String setTimeStr = convertTime(setTime);

  String currTimerOutput = "Tempo: " + setTimeStr + "|" + currentTimeStr;

  if (currTimerOutput != prevTimerOutput) {
    Serial.println(currTimerOutput);
    prevTimerOutput = currTimerOutput;
  }
}

void buzzLoop() {
  digitalWrite(pinLed, HIGH);
  delay(250);
  digitalWrite(pinLed, LOW);
  delay(250);
}

void setup() {
  Serial.begin(9600);
  pinMode(pinButtonAdd, INPUT);
  pinMode(pinButtonDec, INPUT);
  pinMode(pinButtonEnter, INPUT);
  pinMode(pinBomba, OUTPUT);
  pinMode(pinAquecedor, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinLed, OUTPUT);
}

void loop() {
  deltaTime = (millis() - prevTime) / 1000.0;
  prevTime = millis();

  if (settingTime) {
    setTimerLoop();
  }
  if (timerRunning) {
    timerLoop();
    if (currentTime <= 0.0) {
      timerRunning = false;
      buzzing = true;
      tone(pinBuzzer, buzzerFrequency);
      digitalWrite(pinBomba, LOW);
      digitalWrite(pinAquecedor, LOW);
    }
  }
  if (buzzing) {
    buzzLoop();
  }
}
