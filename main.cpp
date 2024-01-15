#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

#define LCD_I2C_ADDR 0x27
#define SDA_PIN 21
#define SCL_PIN 22
#define PIN_LED 5    // Pin al que está conectada la tira de LEDs
#define NUM_LEDS 30  // Número total de LEDs en la tira
#define PIN_X 34     // Pin analógico para la coordenada X del joystick
#define PIN_Y 35     // Pin analógico para la coordenada Y del joystick
#define PIN_BUTTON 25 // Pin para el botón del joystick

LiquidCrystal_I2C lcd(LCD_I2C_ADDR, 20, 4);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN_LED, NEO_GRB + NEO_KHZ800);

const int buttonUpPin = 2;    // Pin para el botón de desplazamiento hacia arriba
const int buttonDownPin = 4;  // Pin para el botón de desplazamiento hacia abajo
const int buttonSelectPin = 12; // Pin para el botón de selección

int selectedOption = 1;  // Inicialmente selecciona la primera opción
bool buttonSelectPressed = false;  // Estado del botón de selección

int selectedPositions[4];  // Arreglo para almacenar las posiciones seleccionadas por el usuario
int numSelections = 0;     // Contador de selecciones del usuario
bool gameStarted = false;  // Variable para verificar si el juego se ha iniciado

void configure();
void Loop();
void readButtons();
void displayMenu();
void selectOption();
void startGame();
void animateGuessingGame();
void colorWipe(uint32_t color, int wait);

void setup() {
  configure();
  gameStarted = false;  // Asegura que el juego no se inicie automáticamente
}

void loop() {

  readButtons();
  displayMenu();

  // Si el juego ha comenzado, ejecuta la lógica del juego
  if (gameStarted) {
    animateGuessingGame();
  }
}

void configure() {
  Serial.begin(9600);
  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.begin(20, 4);
  lcd.init();
  lcd.backlight();
  pinMode(buttonUpPin, INPUT_PULLUP);
  pinMode(buttonDownPin, INPUT_PULLUP);
  pinMode(buttonSelectPin, INPUT_PULLUP);
  strip.begin();                     // Inicia la tira de LEDs
  strip.show();                      // Inicia todos los LEDs apagados
  pinMode(PIN_BUTTON, INPUT_PULLUP);  // Configura el pin del botón como entrada con pull-up
}


void readButtons() {
  if (digitalRead(buttonUpPin) == LOW) {
    if (selectedOption < 2) {
      selectedOption++;
      delay(800);
    }
  }

  if (digitalRead(buttonDownPin) == LOW) {
    if (selectedOption > 1) {
      selectedOption--;
      delay(300);
    }
  }

  if (digitalRead(buttonSelectPin) == LOW && !buttonSelectPressed) {
    buttonSelectPressed = true;
    selectOption();
  } else if (digitalRead(buttonSelectPin) == HIGH) {
    buttonSelectPressed = false;
  }
}

void displayMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Seleccione una opcion:");

  for (int i = 1; i <= 2; i++) {
    lcd.setCursor(0, i);
    lcd.print(selectedOption == i ? "> " : "  ");
    lcd.print(i == 1 ? "Jugar" : "Consultar Saldo");
  }
}

void selectOption() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Opcion seleccionada:");

  lcd.setCursor(0, 1);
  lcd.print(selectedOption == 1 ? "Jugar" : "Consultar Saldo");

  if (selectedOption == 1) {
    startGame(); // Llama a la función para iniciar el juego
  } else if (selectedOption == 2) {
    // Lógica para la opción "Consultar Saldo"
    // ...
    delay(2000);
  }
}

void startGame() {
  gameStarted = false; // Establece el estado del juego como no iniciado
  numSelections = 0;
}

void animateGuessingGame() {
  // Aquí va la lógica del juego cuando está iniciado
  int xValue, yValue, mappedX, mappedY;

  // Realiza la animación de recorrer un LED naranja
  for (int i = 0; i < NUM_LEDS; i++) {
    colorWipe(strip.Color(0, 0, 0), 0); // Apaga todos los LEDs
    strip.setPixelColor(i, strip.Color(255, 165, 0)); // Ilumina el LED actual
    strip.show();
    delay(50);
  }

  // Verifica las respuestas del usuario
  int correctGuesses = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (selectedPositions[i] == (j * 10) + j) {
        correctGuesses++;
        break;
      }
    }
  }

  // Imprime el resultado en el puerto serie
  Serial.print("Aciertos: ");
  Serial.println(correctGuesses);

  // Reinicia el juego si el usuario acertó al menos una posición
  if (correctGuesses > 0) {
    Serial.println("¡Has acertado al menos una posición! Puedes seguir jugando.");
    numSelections = 0; // Reinicia el contador de selecciones
  } else {
    Serial.println("No has acertado ninguna posición. Fin del juego.");
    while (true) {
      // Puedes agregar aquí alguna acción adicional al final del juego
    }
  }

  gameStarted = false;
  numSelections = 0;
}

void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
  delay(wait);
}
