const int cvPin = 9;
const int bpmControlPin = A0;

const int audioPin = 3;
const int audioClickDuration = 60;
const int audioClickFreq = 913;
const int audioClickHBFreq = 813;

int audio_idx = 0;
int tempo = 4;
int clickCounter = 0;
int cvSignalValue = 0;

bool incUnpressed = true;
bool decUnpressed = true;
int buttonState = 0;      // Variable to store the button state
int bpm = 50;
const int waitDurationMillis = 2;
bool bpmChanged = true;
unsigned long previousMillis = 0;
unsigned long millisPerBeat = 0;
bool clickPlayed = false;

unsigned long calculateMillisPerBeat(int bpm){
  return 60000 / bpm;
}

void evaluateBpmChanges(int bpmValue){
  if (bpm != bpmValue){
    bpm = bpmValue;
    bpmChanged = false;
    millisPerBeat = calculateMillisPerBeat(bpm);
  }
}



void playAudioClick(bool highBeat){
  for (int i = 0; i < audioClickDuration; i++) {
        digitalWrite(audioPin, HIGH);
        delayMicroseconds(!highBeat ? audioClickFreq : audioClickHBFreq);
        digitalWrite(audioPin, LOW);
        delayMicroseconds(!highBeat ? audioClickFreq : audioClickHBFreq);
  }
}

void playCVSignal() {
    // Calculate the duration of the pulse in microseconds
    unsigned long pulseDuration = 3000; // Duration of the pulse in microsecond
    // Set the PWM pin HIGH for the pulse duration
    digitalWrite(cvPin, HIGH);
    delayMicroseconds(pulseDuration); // Wait for the pulse duration
    digitalWrite(cvPin, LOW); // Set the PWM pin LOW
}

void produceOutput(){  
  unsigned long currentMillis = millis();

  if ( (currentMillis - previousMillis) > waitDurationMillis ){
  }

  if ( (currentMillis - previousMillis) > millisPerBeat ){
    previousMillis = currentMillis;
    playAudioClick(clickCounter == 0);
    playCVSignal();
    clickCounter = (clickCounter + 1) % tempo;
  }
}

int getBpmValue(){
  return map(analogRead(bpmControlPin),0,1023,1,300);
}

void setup() {
    pinMode(audioPin,OUTPUT);
    pinMode(cvPin,OUTPUT);
    previousMillis = millis();
}



void loop() {
    // Read the state of the button
    int inc = 0;
    int dec = 0;
    int bpmValue = getBpmValue();
    evaluateBpmChanges(bpmValue);
    produceOutput();
}