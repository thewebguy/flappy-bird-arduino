#include <ezLCDLib.h>

ezLCD3 lcd;
volatile boolean ezLCDInt = false;

unsigned long timer = millis();
int draw = 1;
int centered = 0;
int right = 1;
int left = 2;
int bottom = 3;
int top = 4;

float gravity = 0.25;


// Canvas / Sky
int canvasColor = BLUE;
int canvasX = 0;
int canvasY = 0;
int canvasW = 240;
int canvasH = 240;

// Grass
int grassColor = GREEN;
int grassX = 0;
int grassY = 240;
int grassW = 240;
int grassH = 80;

// Bird
int birdColor = WHITE;
int birdW = 30;
int birdH = 25;
int birdX = (canvasW - birdW) / 2;
int birdY = 30;
float birdVelocityY = -5;
int birdTerminalVelocityY = 5;

// Buttons
int jumpButton = 1;
int jumpButtonRadius = 5;
int jumpButtonW = 80;
int jumpButtonH = 40;
int jumpButtonX = grassX + (grassW - jumpButtonW) / 2;
int jumpButtonY = grassY + (grassH - jumpButtonH) / 2;

// Strings
int jumpString = 1;

// Themes
int jumpTheme = 1;

// Pipes
float pipeVelocityX = -1;
int pipeColor = DARKGREEN;
int pipeW = 40;
int pipeH = (canvasH / 2) - (2 * birdH);
int pipeX[] = {0 - pipeW};


void setup() {
	lcd.begin(EZM_BAUD_RATE);
	
	lcd.cls(BLACK, grassColor);
	lcd.rect(grassY, grassX, grassH, grassW, true);
	
	lcd.string(jumpString, "<");
	lcd.theme(jumpTheme, 5, 20, 3, 3, 3, 4, 4, 4, 2, 1);
	lcd.button(jumpButton, jumpButtonY, jumpButtonX, jumpButtonH, jumpButtonW, draw, centered, jumpButtonRadius, jumpTheme, jumpString);
	attachInterrupt(0, ezLCDevent, LOW);  
}

void loop() {
	if (ezLCDInt) {
		ezLCDInt = false;
    lcd.wstack(0);
		if (lcd.currentWidget == jumpButton) {
			if (birdY + birdVelocityY > 0 - (birdH / 2)) {
				birdVelocityY = -4;
			}
		}
	}
	
	if (millis() - timer >= 16) {
		timer = millis();
		
		if (birdVelocityY < birdTerminalVelocityY) birdVelocityY += gravity;
		if (birdY + birdVelocityY + birdH > canvasH + canvasY) {
			birdY = canvasH + canvasY - birdH;
		} else {
			birdY += birdVelocityY;
		}
		
		if (pipeX[0] - pipeVelocityX > canvasW) {
			pipeX[0] = 0 - pipeW;
		} else {
			pipeX[0] -= pipeVelocityX;
		}
		
		lcd.color(canvasColor);
		lcd.rect(canvasY, canvasX, canvasH, canvasW, true);
		
		lcd.color(pipeColor);
		lcd.rect(0, pipeX[0], pipeH, pipeW, true);
		lcd.rect(canvasH - pipeH, pipeX[0], pipeH, pipeW, true);

		lcd.color(birdColor);
		lcd.rect(birdY, birdX, birdH, birdW, true);
	}
}


void ezLCDevent( void ) {
	ezLCDInt = true;
}``