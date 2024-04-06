#include <Keypad.h>

// Define la disposición del teclado
#define ROWS 4
#define COLS 4
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Define los pines de fila y columna
byte rowPins[ROWS] = {9, 8, 7, 6};  // Conecta las filas a estos pines
byte colPins[COLS] = {5, 4, 3, 2};  // Conecta las columnas a estos pines

// Define los pines de salida para la representación binaria
int outputPins[4] = {10, 11, 12, 13};  // Conecta estos pines al decodificador

// Inicializa el teclado
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
    Serial.begin(9600);

    // Configura los modos de los pines
    for (byte i = 0; i < COLS; i++) {
        pinMode(colPins[i], INPUT_PULLUP);
    }
    for (byte i = 0; i < ROWS; i++) {
        pinMode(rowPins[i], OUTPUT);
        digitalWrite(rowPins[i], HIGH);
    }
    for (int i = 0; i < 4; i++) {
        pinMode(outputPins[i], OUTPUT);
    }
}

void loop() {
    char key = keypad.getKey();
    if (key != NO_KEY) {
        // Convierte la tecla a su valor decimal
        int decimalValue = key - '0';
        showBinary(decimalValue);
        Serial.println(decimalValue, BIN);
        delay(500);  // Para evitar lecturas múltiples
    }
}

// Función para mostrar la representación binaria
void showBinary(int decimalValue) {
    for (int i = 0; i < 4; i++) {
        digitalWrite(outputPins[i], decimalValue & (1 << (3 - i)) ? HIGH : LOW);
    }
}
