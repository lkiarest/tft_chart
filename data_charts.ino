#include <TFT_eSPI.h>
#include "santi.h"

#define LABEL_COLOR TFT_WHITE
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 160
#define CHART_HEIGHT 100
#define CHART_PADDING 10

void showPic();
void drawCharts(int *datas);

// For 1.44" and 1.8" TFT with ST7735 use:
TFT_eSPI tft =TFT_eSPI();

int LIST_SIZE = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print(F("initializing ..."));

  tft.init();
  tft.setRotation(6);

  showPic();

  delay(2000);

  Serial.println(F("Initialized"));
}

void showPic() {
  tft.pushImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, gImage_santi);
}

int getMaxData(float *arr, int count) {
  int maxData = 0;
  for (int i = 0; i < count; i++) {
    if (maxData < arr[i]) {
      maxData = arr[i];
    }
  }
  return maxData;
}

int getAvgData(float *arr, int count) {
  int totalData = 0;
  for (int i = 0; i < count; i++) {
    totalData += arr[i];
  }
  return totalData / count;
}

void drawSummary(float maxVal, float avgVal) {
  tft.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - CHART_HEIGHT, TFT_BLACK);
  // labels
  int labelTop = 16;
  int valueTop = 40;
  int maxLeft = 32;
  int avgLeft = 96;
  int sepTop = labelTop - 4;
  int sepBottom = labelTop + 8;

  tft.setTextSize(1);
  tft.setTextDatum(CC_DATUM);
  tft.setTextColor(LABEL_COLOR);
  // - seperate
  tft.drawFastVLine(64, sepTop, 12, TFT_WHITE);
  tft.drawFastHLine(0, sepBottom, 128, TFT_WHITE);
  // - max
  tft.drawString("MAX", maxLeft, labelTop);
  // - avg
  tft.drawString("AVG", avgLeft, labelTop);

  // values
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  // - max
  tft.drawFloat(maxVal, 1, maxLeft, valueTop);
  // - avg
  tft.setTextColor(TFT_GREEN);
  tft.drawFloat(avgVal, 1, avgLeft, valueTop);
}

void drawLineChart(float *datas, int count, float maxVal) {
  int span = (SCREEN_WIDTH - CHART_PADDING * 2) / (count - 1);

  tft.fillRect(0, SCREEN_HEIGHT - CHART_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, TFT_WHITE);

  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(0);
  tft.setTextDatum(TC_DATUM);

  for (int i = 0; i < count - 1; i++) {
    int startX = CHART_PADDING + span * i;
    int endX = CHART_PADDING + span * (i + 1);
    Serial.print("startX");
    Serial.println(startX);
    int startY = SCREEN_HEIGHT - map(datas[i], 0, maxVal, 0, CHART_HEIGHT - CHART_PADDING * 2);
    int endY = SCREEN_HEIGHT - map(datas[i + 1], 0, maxVal, 0, CHART_HEIGHT- CHART_PADDING * 2);
    tft.drawLine(startX, startY, endX, endY, TFT_RED);

    // data label
    tft.drawFloat(datas[i], 1, startX, startY - CHART_PADDING);
    tft.drawFloat(datas[i + 1], 1, endX, endY - CHART_PADDING);

    delay(500);
  }
}

void drawCharts(float *datas, int count) {
  tft.fillScreen(TFT_WHITE);

  int maxVal = getMaxData(datas, count);
  int avgVal = getAvgData(datas, count);

  drawSummary(maxVal, avgVal);
  delay(100);

  drawLineChart(datas, count, maxVal);
}

void loop() {
  // draw fake data
  float datas[] = {2.0, 3.1, 4.8, 6.0, 5.8, 4.1, 8.0};
  drawCharts(datas, 7);
  delay(5000);
}
