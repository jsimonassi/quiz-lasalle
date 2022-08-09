#define LEDS_COUNT 4

#define DEBUGGER_MODE 1
#define QUIZ_MODE 2
#define GENIUS_MODE 3

int execution_mode = DEBUGGER_MODE;

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

int quizFinished = false;

void setup() {

  for (int i = 0; i < LEDS_COUNT; i++){
    pinMode(NEGATIVE_LEDS[i], OUTPUT);
  }

  pinMode(BUZZER, OUTPUT);
  pinMode(POSITIVE_BLUE_LED, OUTPUT);
  pinMode(POSITIVE_GREEN_LED, OUTPUT);
  pinMode(POSITIVE_RED_LED, OUTPUT);
  pinMode(POSITIVE_YELLOW_LED, OUTPUT);

  pinMode(GREEN_BUTTON, INPUT_PULLUP);
  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(BLUE_BUTTON, INPUT_PULLUP);
  pinMode(YELLOW_BUTTON, INPUT_PULLUP);
  pinMode(MASTER_BUTTON, INPUT_PULLUP);

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
      quizFinished = false;
    }
  }
}

/**
* Em modo debug, todos os leds devem acender e os 
* botões são lidos e escritos no monitor serial.
*/
void debugMode() {
  analogWrite(POSITIVE_BLUE_LED, 255);
  digitalWrite(POSITIVE_GREEN_LED, HIGH);
  digitalWrite(POSITIVE_RED_LED, HIGH);
  digitalWrite(POSITIVE_YELLOW_LED, HIGH);

  Serial.print(digitalRead(GREEN_BUTTON));
  Serial.print(digitalRead(RED_BUTTON));
  Serial.print(digitalRead(BLUE_BUTTON));
  Serial.print(digitalRead(YELLOW_BUTTON));
  Serial.print(digitalRead(MASTER_BUTTON));
  Serial.println("-----");
}

/**
 * Ativa o modo quiz, em que cada botão é testado uma única vez
 * Acende o led ganhador e chama o efeito de feedback sonoro associado.
 */
void quizMode(){
  if(!quizFinished){
    analogWrite(POSITIVE_BLUE_LED, 0);
    digitalWrite(POSITIVE_GREEN_LED, LOW);
    digitalWrite(POSITIVE_RED_LED, LOW);
    digitalWrite(POSITIVE_YELLOW_LED, LOW);

    if(digitalRead(BLUE_BUTTON) == LOW){
      quizFinished = true;
      analogWrite(POSITIVE_BLUE_LED, 255);
      playLongFeedbackkSound();

    }else if(digitalRead(GREEN_BUTTON) == LOW){
      quizFinished = true;
      digitalWrite(POSITIVE_GREEN_LED, HIGH);
      playLongFeedbackkSound();

    }else if(digitalRead(RED_BUTTON) == LOW){
      quizFinished = true;
      digitalWrite(POSITIVE_RED_LED, HIGH);
      playLongFeedbackkSound();

    }else if(digitalRead(YELLOW_BUTTON) == LOW){
      quizFinished = true;
      digitalWrite(POSITIVE_YELLOW_LED, HIGH);
      playLongFeedbackkSound();
      
    }
  }
} 

void geniusMode() {

}

void playFeedbackSound() {
  tone(BUZZER, 1500);
  delay(50);
  noTone(BUZZER);
  delay(50);
  tone(BUZZER, 1500);
  delay(50);
  noTone(BUZZER);
  delay(50); 
  tone(BUZZER, 1500);
  delay(50);
  noTone(BUZZER);
  delay(50);  
  tone(BUZZER, 1500);
  delay(50);
  noTone(BUZZER);
  delay(50);  
}

void playLongFeedbackkSound() {
  tone(BUZZER, 1500);
  delay(2000);
  noTone(BUZZER);
}
