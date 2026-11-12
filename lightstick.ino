
const int buttonPin = 2;
const int RledPin = 9;
const int GledPin = 10;
const int BledPin = 11;

int buttonState = LOW;
int lastButtonReading = LOW;

bool buttonPressed = false;
bool longPressTriggered = false;

unsigned long pressingTime = 0;
const unsigned long longPressInterval = 2000;

unsigned long debounceTimer = 0;
const unsigned long debounceInterval = 30;

// MODES
int currentMode = 0;

// BLINK
unsigned long blinkTimer = 0;
const unsigned long blinkInterval = 500;
bool blinkOn = true;

// COLORS
int lightNum = 0;
int previousLightNum = 0;

int RLightColor = 0;
int GLightColor = 0;
int BLightColor = 0;

int RCurrentColor = 0;
int GCurrentColor = 0;
int BCurrentColor = 0;

// BREATHING
const int fadeAmount = 2;
int fadeDirection = 1;
int fadeBrightness = 0;

unsigned long fadeTimer = 0;
const unsigned long fadeInterval = 10;

void setup() {
  pinMode(buttonPin, INPUT);

  pinMode(RledPin, OUTPUT);
  pinMode(GledPin, OUTPUT);
  pinMode(BledPin, OUTPUT);
}

void loop() {
  checkButton();
  updateLEDColor();

  setRGBLEDColor(
    RCurrentColor,
    GCurrentColor,
    BCurrentColor
  );
}

// BUTTON
void checkButton() {
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonReading) {
    debounceTimer = millis();
  }

  if (millis() - debounceTimer >= debounceInterval) {

    if (reading != buttonState) {
      buttonState = reading;

      // BUTTON PRESSED
      if (buttonState == HIGH) {
        pressingTime = millis();
        buttonPressed = true;
        longPressTriggered = false;

        // Change color immediately
        previousLightNum = lightNum;
        changeLEDColor();
      }

      // BUTTON RELEASED
      else {
        buttonPressed = false;
      }
    }
  }

  lastButtonReading = reading;

  // LONG PRESS
  if (buttonPressed && !longPressTriggered) {
    if (millis() - pressingTime >= longPressInterval) {

      // Restore previous color
      lightNum = previousLightNum;
      setSelectedColor();

      // Switch mode
      changeMode();

      longPressTriggered = true;
    }
  }
}

// CHANGE COLOR
void changeLEDColor() {
  lightNum++;

  if (lightNum >= 5) {
    lightNum = 0;
  }

  setSelectedColor();
}

void setSelectedColor() {
  if (lightNum == 0) {
    RLightColor = 0;
    GLightColor = 0;
    BLightColor = 0;
  }
  else if (lightNum == 1) {
    RLightColor = 0;
    GLightColor = 255;
    BLightColor = 255;
  }
  else if (lightNum == 2) {
    RLightColor = 255;
    GLightColor = 0;
    BLightColor = 255;
  }
  else if (lightNum == 3) {
    RLightColor = 255;
    GLightColor = 255;
    BLightColor = 0;
  }
  else if (lightNum == 4) {
    RLightColor = 0;
    GLightColor = 0;
    BLightColor = 255;
  }
}

// CHANGE MODE
void changeMode() {
  currentMode++;

  if (currentMode >= 3) {
    currentMode = 0;
  }

  if (currentMode == 1) {
    blinkTimer = millis();
    blinkOn = true;
  }

  if (currentMode == 2) {
    fadeBrightness = 0;
    fadeDirection = 1;
    fadeTimer = millis();
  }
}

// UPDATE LED
void updateLEDColor() {

  // MODE 0: CONSTANT
  if (currentMode == 0) {
    RCurrentColor = RLightColor;
    GCurrentColor = GLightColor;
    BCurrentColor = BLightColor;
  }

  // MODE 1: BLINK
  else if (currentMode == 1) {
    unsigned long currentTime = millis();

    if (currentTime - blinkTimer >= blinkInterval) {
      blinkOn = !blinkOn;
      blinkTimer = currentTime;
    }

    if (blinkOn) {
      RCurrentColor = RLightColor;
      GCurrentColor = GLightColor;
      BCurrentColor = BLightColor;
    }
    else {
      RCurrentColor = 0;
      GCurrentColor = 0;
      BCurrentColor = 0;
    }
  }

  // MODE 2: BREATHING
  else if (currentMode == 2) {
    unsigned long currentTime = millis();

    if (currentTime - fadeTimer >= fadeInterval) {
      fadeTimer = currentTime;

      fadeBrightness += fadeDirection * fadeAmount;

      if (fadeBrightness >= 255) {
        fadeBrightness = 255;
        fadeDirection = -1;
      }

      if (fadeBrightness <= 0) {
        fadeBrightness = 0;
        fadeDirection = 1;
      }
    }

    RCurrentColor = RLightColor * fadeBrightness / 255;
    GCurrentColor = GLightColor * fadeBrightness / 255;
    BCurrentColor = BLightColor * fadeBrightness / 255;
  }
}

// RGB OUTPUT
void setRGBLEDColor(int R, int G, int B) {
  analogWrite(RledPin, R);
  analogWrite(GledPin, G);
  analogWrite(BledPin, B);
}
