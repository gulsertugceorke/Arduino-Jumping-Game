#include <LiquidCrystal.h>
#include <Servo.h>

/* CARACTERES ESPECIALES */
byte botonNoPres1[] = {
  0x00,0x00,0x00,0x07,0x04,0x04,0x04,0x0F
};

byte botonNoPres2[] = {
  0x00,0x00,0x00,0x1C,0x04,0x04,0x04,0x1E
};

byte botonPres1[] = {
  0x00,0x02,0x12,0x08,0x00,0x07,0x04,0x0F
};

byte botonPres2[] = {
  0x00,0x08,0x09,0x02,0x00,0x1C,0x04,0x1E
};

/* VARIABLES PARA INDICAR PINES */
const int LED_AZUL = 2;
const int LED_AMARILLO = 3;
const int LED_ROJO = 4;
const int LED_VERDE = 5;

const int SERVO = 6;

const int PIN_BUZZER = 7;

const int RS = 8;
const int E = 9;
const int D4 = 10;
const int D5 = 11;
const int D6 = 12;
const int D7 = 13;

const int PULSADOR_VERDE = A0;
const int PULSADOR_ROJO = A1;
const int PULSADOR_AMARILLO = A2;
const int PULSADOR_AZUL = A3;

const int POTENCIOMETRO = A4;

const int PULSADOR_START = A5;

/* VARIABLES PARA INDICAR SONIDOS */
const int SONIDO_VERDE = 440;
const int SONIDO_ROJO = 392;
const int SONIDO_AMARILLO = 349;
const int SONIDO_AZUL = 329;
const int DURACION_SONIDO = 100;

/* VARIABLES PARA INDICAR RONDAS */
const int MAX_RONDAS = 20;
int ronda = 1;

/* VARIABLES PARA EL TEMPORIZADOR */
const int MIN_SERVO = 0;    /* valor minimo servo    */
const int MAX_SERVO = 180;  /* valor maximo servo    */
int posicionServo = MIN_SERVO; /* valor actual servo */

int cambioServo = 1;        /* movimiento del servo  */
int velocidad = 300;        /* VEL por defecto = 300 */

const int STAGE1 = (MAX_RONDAS/4)*1;  
const int STAGE2 = (MAX_RONDAS/4)*2;
const int STAGE3 = (MAX_RONDAS/4)*3;
const int STAGE4 = MAX_RONDAS;

const int CAMBIO1 = MAX_SERVO / STAGE1;
const int CAMBIO2 = MAX_SERVO / STAGE2;
const int CAMBIO3 = MAX_SERVO / STAGE3;
const int CAMBIO4 = MAX_SERVO / STAGE4;

/* VARIABLES PARA COMPROBAR RESULTADO */
int secuenciaOriginal[MAX_RONDAS];
int secuenciaUsuario[MAX_RONDAS];

int leerVerde = -1;         /* leemos botón verde    */
int leerRojo = -1;          /* leemos botón rojo     */
int leerAmarillo = -1;      /* leemos botón amarillo */
int leerAzul = -1;          /* leemos botón azul     */

int leerPotenc = -1;        /* leemos potenciómetro  */

const int APAGADO = -99;    /* ningun LED asignado   */
int botonPulsado = APAGADO; /* control LED asignado |*/
int colorPulsado = APAGADO;                      /* |*/
int leerEstadoBotonPulsado = APAGADO;            /* |*/
bool algunBotonPulsado = false;                  /* |*/

bool salir = false;
bool gameOver = false;

/* -------------------------------------------------- */

/* INICIAMOS EL LCD Y SERVO */
Servo temporizador;
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

/**
* SETUP: INICIAMOS EL PROGRAMA
**/
void setup() {
  lcd.begin(16, 2);
  /* Creamos los caracteres especoales */
  lcd.createChar(0, botonNoPres1);
  lcd.createChar(1, botonNoPres2);
  lcd.createChar(2, botonPres1);
  lcd.createChar(3, botonPres2);
  
  /* Iniciamos temporizador */
  lcd.clear();
  lcd.print(" STARTING");
  temporizador.attach(SERVO);
  temporizador.write(0);
  
  Serial.begin(9600);
  
  /* Iniciamos pulsadores y LEDs */
  pinMode(PULSADOR_VERDE, INPUT);
  pinMode(PULSADOR_ROJO, INPUT);
  pinMode(PULSADOR_AMARILLO, INPUT);
  pinMode(PULSADOR_AZUL, INPUT);

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_AMARILLO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);

  /* Iniciamos todos los LED a 0 */
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_ROJO, LOW);
  digitalWrite(LED_AMARILLO, LOW);
  digitalWrite(LED_AZUL, LOW);
  
  /* Rafaga LEDs */
  lcd.print(".");
  for (int n = LED_VERDE; n >= LED_AZUL; n--) {
    digitalWrite(n, HIGH);
    delay(80);
    digitalWrite(n, LOW);
  }
  lcd.print(".");
  delay(400);
  lcd.print(".");
  for (int n = LED_AZUL; n <= LED_VERDE; n++) {
    digitalWrite(n, HIGH);
    delay(80);
    digitalWrite(n, LOW);
  }

  /* Imprimimos en el monitor serie el título */
  Serial.print("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  Serial.print("         ########################\n");
  Serial.print("         #      SIMON SAYS      #\n");
  Serial.print("         ########################\n\n");
  
  
  /* Esperamos botón de Start */
  lcd.clear();
  lcd.print(" # SIMON SAYS #");
  lcd.print(" PUSH START ");
  while(digitalRead(PULSADOR_START) == HIGH){
    lcd.setCursor(0, 1);
    lcd.print(" PUSH START  ");
    lcd.write((byte)0);
    lcd.write((byte)1);
    
    /* Leemos varias veces para no coger la misma semilla siempre */
    delay(75);
    if(digitalRead(PULSADOR_START) == LOW)
      break;
    delay(75);
    if(digitalRead(PULSADOR_START) == LOW)
      break;
    delay(75);
    if(digitalRead(PULSADOR_START) == LOW)
      break;
    delay(75);
    if(digitalRead(PULSADOR_START) == LOW)
      break;
    
    lcd.setCursor(0, 1);
    lcd.print(" PUSH START  ");
    lcd.write((byte)2);
    lcd.write((byte)3);
    
    /* Leemos varias veces para no coger la misma semilla siempre */
    delay(75);
    if(digitalRead(PULSADOR_START) == LOW)
      break;
    delay(75);
    if(digitalRead(PULSADOR_START) == LOW)
      break;
    delay(75);
    if(digitalRead(PULSADOR_START) == LOW)
      break;
    delay(75);
  }
  
  /* Generamos la secuencia aleatoria del juego */
  generarSecuencia();
}


/**
* LOOP: SE REPITE POR CADA RONDA
**/
void loop()
{ 
  /* Nueva Ronda: se prepara temporizador y pantalla */ 
  temporizador.write(MIN_SERVO);
  lcd.clear();
  lcd.print(" # SIMON SAYS #");
  lcd.setCursor(0, 1);
  lcd.print(" ROUND:" + String(ronda));
  delay(2000);
  
  /* Reproducir secuencia */
  mostrarSecuencia();
  
  botonPulsado = APAGADO;
  posicionServo = 0;
  
  /* Comprobamos la entrada por botones */
  while(!salir)
    comprobarSecuencia();
  
  /* Se ha PERDIDO */
  if(gameOver){
    finalizarJuegoPerder();
    while(1){
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print("RESET TO PLAY AG");
      delay(400);
      lcd.setCursor(0, 1);
      lcd.print("ESET TO PLAY AGA");
      delay(100);
      lcd.setCursor(0, 1);
      lcd.print("SET TO PLAY AGAI"); 
      delay(100);
      lcd.setCursor(0, 1);
      lcd.print("ET TO PLAY AGAIN");

      delay(1000);
      lcd.setCursor(0, 1); 
      lcd.print("                ");
    }
  }
  
  /* Juego completado: se ha GANADO */
  if(ronda == MAX_RONDAS) {
    lcd.clear();
    lcd.print("    YOU WIN!");
    Serial.print("\n\n         --- VICTORY! ---");
    
    /* Melodía victoria */
    delay(2000);
    tone(PIN_BUZZER,1318,125);
    delay(130);
    tone(PIN_BUZZER,1568,125);
    delay(130);
    tone(PIN_BUZZER,2637,125);
    delay(130);
    tone(PIN_BUZZER,2093,125);
    delay(130);
    tone(PIN_BUZZER,2350,125);
    delay(130);
    tone(PIN_BUZZER,3136,125);
    delay(125);
    noTone(PIN_BUZZER);
    
    while(1){
      finalizarJuegoGanar();
    }
  }
  
  /* Siguiente ronda */
  ronda++;
  salir = false;
}

/* -------------------------------------------------- */

/**
* Método para generar la secuencia aleatoria 
**/
void generarSecuencia() {
  randomSeed(millis());

  for (int i = 0; i < MAX_RONDAS; i++){
    secuenciaOriginal[i] = random(LED_AZUL, LED_VERDE+1);
  }

  /* Imprimimos la inicial correspondiente a cada color */
  Serial.print("Secuencia completa: \n[");
  for (int i = 0; i < MAX_RONDAS-1; i++){
    if(secuenciaOriginal[i] == LED_VERDE)
      Serial.print("G ");
    else if(secuenciaOriginal[i] == LED_ROJO)
      Serial.print("R ");
    else if(secuenciaOriginal[i] == LED_AMARILLO)
      Serial.print("Y ");
    else if(secuenciaOriginal[i] == LED_AZUL)
      Serial.print("B ");
  }
  
  if(secuenciaOriginal[MAX_RONDAS-1] == LED_VERDE)
    Serial.print("G]\n\n");
  else if(secuenciaOriginal[MAX_RONDAS-1] == LED_ROJO)
    Serial.print("R]\n\n");
  else if(secuenciaOriginal[MAX_RONDAS-1] == LED_AMARILLO)
    Serial.print("Y]\n\n");
  else if(secuenciaOriginal[MAX_RONDAS-1] == LED_AZUL)
    Serial.print("B]\n\n");
}

/**
* Método para mostrar la secuencia aleatoria. Solo
* reproduce los colores necesarios para cada ronda
**/
void mostrarSecuencia() {
  leerPotenc = analogRead(POTENCIOMETRO);
  if(ronda < 9)
    lcd.print(" ");
  lcd.print(" VEL:");
  velocidad = calcularVelocidad(leerPotenc);
  
  for (int i = 0; i < ronda; i++){
    /* Reproducimos el sonido correspondiente al color */
    if(secuenciaOriginal[i] == LED_VERDE)
      tone(PIN_BUZZER, SONIDO_VERDE, DURACION_SONIDO);
    else if(secuenciaOriginal[i] == LED_ROJO)
      tone(PIN_BUZZER, SONIDO_ROJO, DURACION_SONIDO);
    else if(secuenciaOriginal[i] == LED_AMARILLO)
      tone(PIN_BUZZER, SONIDO_AMARILLO, DURACION_SONIDO);
    else if(secuenciaOriginal[i] == LED_AZUL)
      tone(PIN_BUZZER, SONIDO_AZUL, DURACION_SONIDO);
    
    /* Iluminamos el LED correspondiente al color*/
    digitalWrite(secuenciaOriginal[i], HIGH);
    delay(velocidad);
    digitalWrite(secuenciaOriginal[i], LOW);
    delay(velocidad);
  }
}

/**
* Método para seleccionar la velocidad del juego
**/
int calcularVelocidad (int valorP) {
  valorP = map(valorP, 0, 1023, 0, 3);
  if (valorP >= 2) {
  lcd.print("1");
    delay(2000);
    return 300;
  } else if (valorP == 1) {
  lcd.print("2");
    delay(2000);
    return 200;
  } else if (valorP == 0) {
  lcd.print("3");
    delay(2000);
    return 100;
  }
  
  /* En caso de error, retorna una velocidad genérica */
  return 300;
}

/**
* Método para comprobar la secuencia introducida con
* la secuencia generada. Si coinciden, continua la 
* ejecución. Si no coinciden, se indica error para
* finalizar el programa con 'gameOver'.
**/
void comprobarSecuencia() {
  Serial.print("\nRONDA " + String(ronda) + ":");
  
  for(int i = 0; i < ronda;){
    algunBotonPulsado = false;
  
    leerVerde = digitalRead(PULSADOR_VERDE);
    leerRojo = digitalRead(PULSADOR_ROJO);
    leerAmarillo = digitalRead(PULSADOR_AMARILLO);
    leerAzul = digitalRead(PULSADOR_AZUL);
    
  
    /* Verde pulsado */
    if(leerVerde == LOW) {
    tone(PIN_BUZZER, SONIDO_VERDE, DURACION_SONIDO);
      leerEstadoBotonPulsado = leerVerde;
      botonPulsado = PULSADOR_VERDE;
      colorPulsado = LED_VERDE;
      digitalWrite(LED_VERDE, HIGH);
      algunBotonPulsado = true;
    }
    /* Rojo pulsado */
    else if(leerRojo == LOW) {
    tone(PIN_BUZZER, SONIDO_ROJO, DURACION_SONIDO);
      leerEstadoBotonPulsado = leerRojo;
      botonPulsado = PULSADOR_ROJO;
      colorPulsado = LED_ROJO;
      digitalWrite(LED_ROJO, HIGH);
      algunBotonPulsado = true;
    }
    /* Amarillo pulsado */
    else if (leerAmarillo == LOW) {
    tone(PIN_BUZZER, SONIDO_AMARILLO, DURACION_SONIDO);
      leerEstadoBotonPulsado = leerAmarillo;
      botonPulsado = PULSADOR_AMARILLO;
      colorPulsado = LED_AMARILLO;
      digitalWrite(LED_AMARILLO, HIGH);
      algunBotonPulsado = true;
    }
    /* Azul pulsado */
    else if (leerAzul == LOW) {
    tone(PIN_BUZZER, SONIDO_AZUL, DURACION_SONIDO);
      leerEstadoBotonPulsado = leerAzul;
      botonPulsado = PULSADOR_AZUL;
      colorPulsado = LED_AZUL;
      digitalWrite(LED_AZUL, HIGH);
      algunBotonPulsado = true;
    }
    
    /* Temporizador (Servo) */
    if(ronda > 0 && ronda <= STAGE1)
      cambioServo = CAMBIO1;
    else if(ronda > STAGE1 && ronda <= STAGE2)
      cambioServo = CAMBIO2;
    else if(ronda > STAGE2 && ronda <= STAGE3)
      cambioServo = CAMBIO3;
    else if(ronda > STAGE3 && ronda <= STAGE4)
      cambioServo = CAMBIO4;
    
    if((millis()%1000) == 0) {
      posicionServo += cambioServo;
      temporizador.write(posicionServo);
      delay(50);
    }
    
    /* Si se ACABA el TIEMPO */
    if(posicionServo >= MAX_SERVO) {
      salir = true;
      gameOver = true;
      Serial.print(" -> [X] Tiempo acabado! [X]");
      Serial.print("\n\n        --- GAME OVER ---");
      lcd.clear();
      delay(100);
      lcd.print("   TIME'S UP!");
      delay(500);
      lcd.clear();
      delay(100);
      lcd.print("   TIME'S UP!");
      delay(500);
      lcd.clear();
      delay(100);
    }
    
    /* Si se ha PULSADO ALGÚN BOTÓN se entra aquí*/
    else if(algunBotonPulsado) {
      secuenciaUsuario[i] = colorPulsado;
      Serial.print("\n - Pulsado: ");
      if(colorPulsado == LED_VERDE)
      Serial.print("G");
      else if(colorPulsado == LED_ROJO)
        Serial.print("R");
      else if(colorPulsado == LED_AMARILLO)
        Serial.print("Y");
      else if(colorPulsado == LED_AZUL)
        Serial.print("B");
     
      /* Esperamos a que se suelte el botón y apagamos el LED */
      while(leerEstadoBotonPulsado == LOW){
        leerEstadoBotonPulsado = digitalRead(botonPulsado);
      }
      digitalWrite(colorPulsado, LOW);
  
      /* ERROR*/
      if(secuenciaOriginal[i] != secuenciaUsuario[i]) {
        salir = true;
        gameOver = true;
        Serial.print(" -> [X] NO CORRECTO! [X]");
        Serial.print("\n\n        --- GAME OVER ---");
      }
      i++;
    }
    if(gameOver) {
      break;
    }
  }
  salir = true;
}

/**
* Método para juego finalizado PERDIDO
**/
void finalizarJuegoPerder() {
  /* Mensaje LCD */
  lcd.clear();
  lcd.print("   GAME OVER!");
  
  /* Parpadeo LEDs para indicar al usuario ERROR */
  for(int parpadeo = 0; parpadeo < 3; parpadeo++) {
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_ROJO, HIGH);
    digitalWrite(LED_AMARILLO, HIGH);
    digitalWrite(LED_AZUL, HIGH);
    delay(100);
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_ROJO, LOW);
    digitalWrite(LED_AMARILLO, LOW);
    digitalWrite(LED_AZUL, LOW);
    delay(100);
  }

  /* Melodía derrota */
  tone(PIN_BUZZER, 466, 400);
  delay(400);
  tone(PIN_BUZZER, 440, 400);
  delay(400);
  tone(PIN_BUZZER, 415, 400);
  delay(400);
  tone(PIN_BUZZER, 392, 1000);
  delay(1000);
}

/**
* Método para juego finalizado GANADO
**/
void finalizarJuegoGanar() {
  /* Rafaga mensaje LCD */
  for(int n = 0; n < 3; n++){
    delay(500);
    lcd.clear();
    delay(100);
    lcd.print("    YOU WIN!");
  }
  
  /* Rafaga LEDs */
  for (int n = LED_VERDE; n >= LED_AZUL; n--) {
    digitalWrite(n, HIGH);
    delay(80);
    digitalWrite(n, LOW);
  }
  delay(400);
  for (int n = LED_AZUL; n <= LED_VERDE; n++) {
    digitalWrite(n, HIGH);
    delay(80);
    digitalWrite(n, LOW);
  }
}
