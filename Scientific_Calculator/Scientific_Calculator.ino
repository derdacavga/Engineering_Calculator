#include <TFT_eSPI.h>
#include <SPI.h>
#include <math.h>
#include <FS.h>
#include <LittleFS.h>

TFT_eSPI tft = TFT_eSPI();

#define BUTTON_W 60
#define BUTTON_H 40
#define BUTTON_ROWS 6
#define BUTTON_COLS 4

const char *labels[BUTTON_ROWS][BUTTON_COLS] = {
  { "sin", "cos", "tan", "sqrt" },
  { "log", "exp", "C", "<" },
  { "7", "8", "9", "/" },
  { "4", "5", "6", "*" },
  { "1", "2", "3", "-" },
  { "0", ".", "=", "+" }
};

TFT_eSPI_Button buttons[BUTTON_ROWS][BUTTON_COLS];
String input = "";

uint16_t calData[5];

void drawButtons() {
  int x_offset = 0;
  int y_offset = 75;

  for (int r = 0; r < BUTTON_ROWS; r++) {
    for (int c = 0; c < BUTTON_COLS; c++) {
      int x = c * BUTTON_W + x_offset;
      int y = r * BUTTON_H + y_offset;
      buttons[r][c].initButton(&tft, x + BUTTON_W / 2, y + BUTTON_H / 2,
                               BUTTON_W - 4, BUTTON_H - 4,
                               TFT_WHITE, TFT_BLUE, TFT_WHITE,
                               (char *)labels[r][c], 1);
      buttons[r][c].drawButton();
    }
  }
}

void updateDisplay() {
  tft.fillRect(0, 0, 240, 50, TFT_GREEN);
  tft.setTextDatum(TR_DATUM);
  tft.setTextColor(TFT_BLACK, TFT_GREEN);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.drawString(input, 235, 30);
}

float evalExpression(String expr) {
  expr.trim();

  if (expr.startsWith("sin")) return sin(radians(expr.substring(3).toFloat()));
  if (expr.startsWith("cos")) return cos(radians(expr.substring(3).toFloat()));
  if (expr.startsWith("tan")) return tan(radians(expr.substring(3).toFloat()));
  if (expr.startsWith("sqrt")) return sqrt(expr.substring(4).toFloat());
  if (expr.startsWith("log")) return log10(expr.substring(3).toFloat());
  if (expr.startsWith("exp")) return exp(expr.substring(3).toFloat());

  char op = 0;
  int idx = -1;
  for (int i = 0; i < expr.length(); i++) {
    if (strchr("+-*/", expr[i])) {
      op = expr[i];
      idx = i;
      break;
    }
  }

  if (idx != -1 && idx > 0) {
    float a = expr.substring(0, idx).toFloat();
    float b = expr.substring(idx + 1).toFloat();
    switch (op) {
      case '+': return a + b;
      case '-': return a - b;
      case '*': return a * b;
      case '/': return b != 0 ? a / b : 0;
    }
  }
  return expr.toFloat();
}

void touchCalibration() {
  uint16_t calDataTemp[5];
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(10, 140);
  tft.setTextColor(TFT_WHITE);
  tft.println("Touch Corner to calibrate");
  tft.calibrateTouch(calDataTemp, TFT_GREEN, TFT_BLACK, 15);
  for (int i = 0; i < 5; i++) calData[i] = calDataTemp[i];
  tft.setTouch(calData);
}

void setup() {
  Serial.begin(115200);
  LittleFS.begin(true);
  tft.init();
  tft.setRotation(2);
  logo();
  delay(1500);
  touchCalibration();
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTouch(calData);
  tft.fillScreen(TFT_GREEN);
  updateDisplay();
  drawButtons();
}

void loop() {
  uint16_t x, y;
  if (tft.getTouch(&x, &y)) {
    for (int r = 0; r < BUTTON_ROWS; r++) {
      for (int c = 0; c < BUTTON_COLS; c++) {
        if (buttons[r][c].contains(x, y)) {
          String label = labels[r][c];
          if (label == "=") {
            float result = evalExpression(input);
            input = String(result);
          } else if (label == "C") {
            input = "";
          } else if (label == "<") {
            if (input.length() > 0) input.remove(input.length() - 1);
          } else {
            input += label;
          }
          updateDisplay();
          delay(200);
        }
      }
    }
  }
}

void logo() {
  tft.fillScreen(TFT_RED);
  int centerX = 120;
  int centerY = 160;
  int radius = 70;
  int thickness = 7;

  for (int r = radius - thickness / 2; r <= radius + thickness / 2; r++) {
    tft.drawCircle(centerX, centerY, r, TFT_BLACK);
  }

  tft.setTextColor(TFT_BLACK, TFT_RED);
  tft.setTextDatum(MC_DATUM);
  tft.setFreeFont(&FreeSansBold24pt7b);
  tft.drawString("DSN", centerX, centerY);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.drawString("Engineering Calculator", centerX, 300);
}
