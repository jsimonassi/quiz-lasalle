#define LEDS_COUNT 4

#define DEBUGGER_MODE 1
#define QUIZ_MODE 2
#define GENIUS_MODE 3

//Genius Mode
#define NOTE_D4  294
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_A5  880

int NOTES[4] = { NOTE_A5, NOTE_A4, NOTE_G4, NOTE_D4 };
int sequence[100] = {};
int sequence_step = 0;
int current_round = 0;
int button_pressed = 0;
int end_of_game = false;

int execution_mode = DEBUGGER_MODE;

//General config
int NEGATIVE_LEDS[4] = {0, 2, 4, 6}; 
int POSITIVE_BLUE_LED = A0;
int POSITIVE_GREEN_LED = 3;
int POSITIVE_RED_LED = 5;
int POSITIVE_YELLOW_LED = 7;

int GREEN_BUTTON = 8;
int RED_BUTTON = 9;
int BLUE_BUTTON = 10;
int YELLOW_BUTTON = 11;
int MASTER_BUTTON = 13;
int BUZZER = 12;

int BUTTONS[4] = {BLUE_BUTTON, GREEN_BUTTON, RED_BUTTON, YELLOW_BUTTON};
int LEDS[LEDS_COUNT] = {POSITIVE_BLUE_LED, POSITIVE_GREEN_LED, POSITIVE_RED_LED, POSITIVE_YELLOW_LED};

//QuizMode
int quiz_finished = false;

void setup() {

  for (int i = 0; i < LEDS_COUNT; i++){
    pinMode(NEGATIVE_LEDS[i], OUTPUT);
  }

  pinMode(BUZZER, OUTPUT);
  for(int i = 0; i < LEDS_COUNT; i++){
      pinMode(LEDS[i], OUTPUT);
  }

  for(int i = 0; i < LEDS_COUNT; i++){
      pinMode(BUTTONS[i], INPUT_PULLUP);
  }

  pinMode(MASTER_BUTTON, INPUT_PULLUP);
  randomSeed(analogRead(A5));
  Serial.begin(9600);
}


void loop() {

  // Define as portas lógicas digitais e analógicas como LOW para usar como GND
  for(int i = 0; i < LEDS_COUNT; i++){
    digitalWrite(NEGATIVE_LEDS[i], LOW);
  }
  
  switch(execution_mode) {
    case DEBUGGER_MODE:
      debugMode();
    break;

    case QUIZ_MODE:
      quizMode();
    break;

    default:
      geniusMode();
    break;
  }

  if(digitalRead(MASTER_BUTTON) == LOW){
    playFeedbackSound(); 
    execution_mode++;
    Serial.println(execution_mode);
    if(execution_mode > GENIUS_MODE){
      execution_mode = DEBUGGER_MODE;
    } 

    if(execution_mode == QUIZ_MODE){
      quiz_finished = false;
    }

    if(execution_mode == GENIUS_MODE){
        resetGeniusGame();
    }
  }
}

/**
* Em modo debug, todos os leds devem acender e os 
* botões são lidos e escritos no monitor serial.
*/
void debugMode() {
  for(int i = 0; i < LEDS_COUNT; i++){
    __turnLedOn(LEDS[i]);
  }
  
  for(int i = 0; i < LEDS_COUNT; i++){
    Serial.print(digitalRead(BUTTONS[i]));
  }
  Serial.print(digitalRead(MASTER_BUTTON));
  Serial.println("-----");
}

/**
* Em modo quiz, ganha quem apertar o botão primeiro. 
*/
void quizMode(){
  if(!quiz_finished){
    for(int i = 0; i < LEDS_COUNT; i++){
       __turnLedOff(LEDS[i]);
    }

    for(int i = 0; i < LEDS_COUNT; i++){
      if(digitalRead(BUTTONS[i]) == LOW){
        quiz_finished = true;
        __turnLedOn(LEDS[i]);
        playLongFeedbackkSound();
      }
    }
  }
} 

void geniusMode() {

  if (end_of_game) {
    resetGeniusGame();
  }
 
  if (current_round == 0) {
    playStartSound();
    delay(500);
  }

  nextRoud();
  playGameSequenceSound();
  waitPlayer();
}

void resetGeniusGame(){
    int sequence[100] = {};
    current_round = 0;
    sequence_step = 0;
    end_of_game = false;
}

void nextRoud(){
    int sorted_number = random(0, 4);
    sequence[current_round++] = sorted_number;
}

void playGameSequenceSound() {
  for (int i = 0; i < current_round; i++) {

    tone(BUZZER, NOTES[sequence[i]]);
    //TODO: Acender o led aqui
    //digitalWrite(pinosLeds[sequencia[i]], HIGH);
    delay(500);
    noTone(BUZZER);
    //TODO: Apagar o led aqui
    //digitalWrite(pinosLeds[sequencia[i]], LOW);
    delay(100);
  }
  noTone(BUZZER);
}

void waitPlayer() {
  for (int i = 0; i < current_round; i++) {
    __waitPlay();
    __checkPlay();
    if (end_of_game) {
      break;
    }
    sequence_step++;
  }
  sequence_step = 0;
}

void __waitPlay(){
  boolean user_played = false;

  while (!user_played) {
    for (int i = 0; i < LEDS_COUNT; i++) {
      //if (digitalRead(pinosBotoes[i]) == HIGH) {
        // Dizendo qual foi o botao pressionado.

        button_pressed = i;

        tone(BUZZER, NOTES[i]);
        //TODO: acender led
        //digitalWrite(pinosLeds[i], HIGH);
        delay(300);
        //TODO: acender led
        //digitalWrite(pinosLeds[i], LOW);
        noTone(BUZZER);

        user_played = true;
      //}
    }
    delay(10);
  }
}

void __checkPlay(){
 if (sequence[sequence_step] != button_pressed) {
    // GAME OVER.

    for (int i = 0; i < LEDS_COUNT; i++) {
      tone(BUZZER, NOTES[i]);
      //digitalWrite(pinosLeds[i], HIGH);
      delay(200);
      //digitalWrite(pinosLeds[i], LOW);
      noTone(BUZZER);
    }

    tone(BUZZER, NOTES[3]);

    for (int i = 0; i < LEDS_COUNT; i++) {

    //   digitalWrite(pinosLeds[0], HIGH);
    //   digitalWrite(pinosLeds[1], HIGH);
    //   digitalWrite(pinosLeds[2], HIGH);
    //   digitalWrite(pinosLeds[3], HIGH);
    //   delay(100);

    //   digitalWrite(pinosLeds[0], LOW);
    //   digitalWrite(pinosLeds[1], LOW);
    //   digitalWrite(pinosLeds[2], LOW);
    //   digitalWrite(pinosLeds[3], LOW);
    //   delay(100);
    }

    noTone(BUZZER);
    end_of_game = true;
  }
}

void playStartSound(){
   tone(BUZZER, NOTES[0]);

//   digitalWrite(pinosLeds[0], HIGH);
//   digitalWrite(pinosLeds[1], HIGH);
//   digitalWrite(pinosLeds[2], HIGH);
//   digitalWrite(pinosLeds[3], HIGH);

  delay(500);

//   digitalWrite(pinosLeds[0], LOW);
//   digitalWrite(pinosLeds[1], LOW);
//   digitalWrite(pinosLeds[2], LOW);
//   digitalWrite(pinosLeds[3], LOW);
  delay(500);
  noTone(BUZZER);
}

void playFeedbackSound() {
  tone(BUZZER, NOTE_A5);
  delay(50);
  noTone(BUZZER);
  delay(50);
  tone(BUZZER, NOTE_A5);
  delay(50);
  noTone(BUZZER);
  delay(50); 
  tone(BUZZER, NOTE_A5);
  delay(50);
  noTone(BUZZER);
  delay(50);  
  tone(BUZZER, NOTE_A5);
  delay(50);
  noTone(BUZZER);
  delay(50);  
}

void playLongFeedbackkSound() {
  tone(BUZZER, NOTE_D4);
  delay(2000);
  noTone(BUZZER);
}

/**
* Como o led foi adicionado na porta analógica, essa função faz a diferenciação para tornar a escrita
* mais simples e abstrair o tipo de porta usada.
*/
void __turnLedOn(int led){
    if(led == POSITIVE_BLUE_LED){
      analogWrite(led, 255);
      return;
    }
    digitalWrite(led, HIGH);
}

/**
* Como o led foi adicionado na porta analógica, essa função faz a diferenciação para tornar a escrita
* mais simples e abstrair o tipo de porta usada.
*/
void __turnLedOff(int led){
    if(led == POSITIVE_BLUE_LED){
      analogWrite(led, 0);
      return;
    }
    digitalWrite(led, LOW);
}
