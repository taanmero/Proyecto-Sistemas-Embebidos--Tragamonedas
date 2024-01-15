#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
 #include <Wire.h>

#define PIN_LED 5 // Pin al que está conectada la tira de LEDs
#define NUM_LEDS 30 // Número total de LEDs en la tira

#define PIN_X 34 // Pin analógico para la coordenada X del joystick
#define PIN_Y 35 // Pin analógico para la coordenada Y del joystick
#define PIN_BUTTON 25 // Pin para el botón del joystick

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN_LED, NEO_GRB + NEO_KHZ800);

int selectedPositions[4]; // Arreglo para almacenar las posiciones seleccionadas por el usuario
int numSelections = 0; // Contador de selecciones del usuario

void setup();
void loop();
void animateGuessingGame();
void colorWipe(uint32_t color, int wait);

void setup() {
  Serial.begin(115200); // Inicia la comunicación serial
  strip.begin(); // Inicia la tira de LEDs
  strip.show(); // Inicia todos los LEDs apagados
  pinMode(PIN_BUTTON, INPUT_PULLUP); // Configura el pin del botón como entrada con pull-up
}

void loop() {
  int xValue = analogRead(PIN_X);
  int yValue = analogRead(PIN_Y);

  int mappedX = map(xValue, 0, 4095, 0, NUM_LEDS - 1);
  int mappedY = map(yValue, 0, 4095, 0, NUM_LEDS - 1);

  colorWipe(strip.Color(0, 0, 0), 0);
  strip.setPixelColor(mappedX, strip.Color(255, 165, 0));
  strip.setPixelColor(mappedY + 10, strip.Color(255, 165, 0));
  strip.setPixelColor(mappedY + 20, strip.Color(255, 165, 0));
  strip.show();

  if (digitalRead(PIN_BUTTON) == LOW) {
    selectedPositions[numSelections] = mappedX;
    numSelections++;

    if (numSelections == 4) {
      animateGuessingGame();
    }

    while (digitalRead(PIN_BUTTON) == LOW) {
      delay(10);
    }
  }

  delay(100);
}

void animateGuessingGame() {
  for (int i = 0; i < NUM_LEDS; i++) {
    colorWipe(strip.Color(0, 0, 0), 0);
    strip.setPixelColor(i, strip.Color(255, 165, 0));
    strip.show();
    delay(50);
  }

  int correctGuesses = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (selectedPositions[i] == (j * 10) + j) {
        correctGuesses++;
        break;
      }
    }
  }

  Serial.print("Aciertos: ");
  Serial.println(correctGuesses);

  if (correctGuesses > 0) {
    Serial.println("¡Has acertado al menos una posición! Puedes seguir jugando.");
    numSelections = 0;
  } else {
    Serial.println("No has acertado ninguna posición. Fin del juego.");
    while (true) {
      // Puedes agregar aquí alguna acción adicional al final del juego
    }
  }
}

void colorWipe(uint32_t color, int wait) {
  for(int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
  delay(wait);
}
