const int buttonPin = 4; // Sesuai D2 pada NodeMCU (GPIO 4)
const int ledPin = 5;    // Sesuai D1 pada NodeMCU (GPIO 5)

// Variabel untuk menyimpan status
int ledState = LOW;         // Status terakhir LED (nyala/mati)
int buttonState = 1;        // Status pembacaan tombol (karena PULLUP, defaultnya 1 / HIGH)
int lastButtonState = 1;    // Status pembacaan tombol sebelumnya

unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 50;    // Jeda waktu untuk mengatasi pantulan tombol (debounce)

void setup() {
  Serial.begin(115200);
  // Menggunakan INPUT_PULLUP agar tidak butuh resistor eksternal
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState); // Inisialisasi awal LED mati
}

void loop() {
  int reading = digitalRead(buttonPin);

  // Jika status tombol berubah
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Jika status tombol stabil dan berbeda dari sebelumnya
    if (reading != buttonState) {
      buttonState = reading;

      // Karena pakai INPUT_PULLUP, tombol ditekan saat bernilai LOW (0)
      if (buttonState == LOW) {
        // Balik status LED
        ledState = !ledState; 
        digitalWrite(ledPin, ledState);

        if (ledState == HIGH) {
          Serial.println("Tombol ditekan -> LED ON");
        } else {
          Serial.println("Tombol ditekan ulang -> LED OFF");
        }
      }
    }
  }

  lastButtonState = reading;
}