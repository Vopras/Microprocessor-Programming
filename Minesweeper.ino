// Modified for SPFD5408 Library by Joao Lopes  
 #include <SPFD5408_Adafruit_GFX.h>  // Core graphics library  
 #include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library  
 #include <SPFD5408_TouchScreen.h>  
 #include <time.h>
 #include <avr/eeprom.h>
 // Pin assignments for the TFT touch screen  
 #define YP  A1  
 #define XM  A2  
 #define YM  7  
 #define XP  6  
 // Calibrated values for the TFT touch screen   
 #define TS_MINX  178  
 #define TS_MINY  75  
 #define TS_MAXX  931  
 #define TS_MAXY  895  
 // Instantiate the ts object  
 TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);  
 #define LCD_RD  A0  
 #define LCD_WR  A1  
 #define LCD_CD  A2  
 #define LCD_CS  A3  
 #define LCD_RESET A4  
 // Instantiate the tft object  
 Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);  
 // Assign human-readable names to some common 16-bit color values:  
 #define BLACK  0x0000  
 #define BLUE  0x001F  
 #define RED   0xF800  
 #define GREEN  0x07E0  
 #define CYAN  0x07FF  
 #define MAGENTA 0xF81F  
 #define YELLOW 0xFFE0  
 #define WHITE  0xFFFF  
 #define BOXSIZE   40  
 #define PENRADIUS  3  
 #define MINPRESSURE 10  
 #define MAXPRESSURE 1000  
 #define NR_MINES 20



int select[10][10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                      
int mines[10][10] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
                                        

int numbers[10][10] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

int i = 0;
int j = 0;
int mines_nr = 0;
long randomNumber;

int cell;




 void setup() {  
  Serial.begin(9600);  
//  srand(generateRandomSeed());
  tft.reset();  
  tft.begin(0x9341);  
  tft.setRotation(1);
  tft.fillScreen(BLACK);  
  layMines();
  getNumbers();
  randomNumber = rand();
//  uint32_t rando = generateRandomSeed();
  Serial.print("Rand =");
  Serial.print("\n");
  Serial.print(randomNumber);
  Serial.print("\n");
 

  for (i = 0; i < 240; i += 24) {
    for (j = 0; j < 240; j += 24) {
      tft.fillRect(i, j, 22, 22, BLUE);
    }
  }
 }

uint32_t generateRandomSeed()
{
  uint8_t  seedBitValue  = 0;
  uint8_t  seedByteValue = 0;
  uint32_t seedWordValue = 0;

  for (uint8_t wordShift = 0; wordShift < 4; wordShift++)     // 4 bytes in a 32 bit word
  {
    for (uint8_t byteShift = 0; byteShift < 8; byteShift++)   // 8 bits in a byte
    {
      for (uint8_t bitSum = 0; bitSum <= 8; bitSum++)         // 8 samples of analog pin
      {
        seedBitValue = seedBitValue + (analogRead(11) & 0x01);                // Flip the coin eight times, adding the results together
      }
      delay(1);                                                                    // Delay a single millisecond to allow the pin to fluctuate
      seedByteValue = seedByteValue | ((seedBitValue & 0x01) << byteShift);        // Build a stack of eight flipped coins
      seedBitValue = 0;                                                            // Clear out the previous coin value
    }
    seedWordValue = seedWordValue | (uint32_t)seedByteValue << (8 * wordShift);    // Build a stack of four sets of 8 coins (shifting right creates a larger number so cast to 32bit)
    seedByteValue = 0;                                                             // Clear out the previous stack value
  }
  return (seedWordValue);

}

void layMines() {
  do {
    do {
      i = rand()%10;
      j = rand()%10;
   }
    while(mines[i][j] != 0);
    mines[i][j] = 1;
    mines_nr++;
   }
   while(mines_nr < NR_MINES+1);

   Serial.print("Mines");
   Serial.print("\n");
   for (i = 0; i < 10; i++) {
    for (j = 0; j < 10; j++) {
      Serial.print(mines[i][j]);
    }
    Serial.print("\n");
   }
}

void setNonCorner(int x, int y) {
  if(!mines[x-1][y-1])
    numbers[x-1][y-1]++;
  if(!mines[x][y-1])
    numbers[x][y-1]++;
  if(!mines[x+1][y-1])
    numbers[x+1][y-1]++;
  if(!mines[x-1][y])
    numbers[x-1][y]++;
  if(!mines[x+1][y])
    numbers[x+1][y]++;
  if(!mines[x-1][y+1])
    numbers[x-1][y+1]++;
  if(!mines[x][y+1])
    numbers[x][y+1]++;
  if(!mines[x+1][y+1])
    numbers[x+1][y+1]++;
}

//void setNonCorner(int x, int y) {
////  if(!mines[x-1][y-1])
//    numbers[x-1][y-1]++;
////  if(!mines[x][y-1])
//    numbers[x][y-1]++;
////  if(!mines[x+1][y-1])
//    numbers[x+1][y-1]++;
////  if(!mines[x-1][y])
//    numbers[x-1][y]++;
////  if(!mines[x+1][y])
//    numbers[x+1][y]++;
////  if(!mines[x-1][y+1])
//    numbers[x-1][y+1]++;
////  if(!mines[x][y+1])
//    numbers[x][y+1]++;
////  if(!mines[x+1][y+1])
//    numbers[x+1][y+1]++;
//}

void setTop(int x) {
  if(!mines[x-1][0])
    numbers[x-1][0]++;
  if(!mines[x+1][0])
    numbers[x+1][0]++;
  if(!mines[x-1][1])
    numbers[x-1][1]++;
  if(!mines[x][1])
    numbers[x][1]++;
  if(!mines[x+1][1])
    numbers[x+1][1]++;
}

//void setTop(int x) {
////  if(!mines[x-1][0])
//    numbers[x-1][0]++;
////  if(!mines[x+1][0])
//    numbers[x+1][0]++;
////  if(!mines[x-1][1])
//    numbers[x-1][1]++;
////  if(!mines[x][1])
//    numbers[x][1]++;
////  if(!mines[x+1][1])
//    numbers[x+1][1]++;
//}

void setBot(int x) {
  if(!mines[x-1][9])
    numbers[x-1][9]++;
  if(!mines[x+1][9])
    numbers[x+1][9]++;
  if(!mines[x-1][8])
    numbers[x-1][8]++;
  if(!mines[x][8])
    numbers[x][8]++;
  if(!mines[x+1][8])
    numbers[x+1][8]++;
}

//void setBot(int x) {
////  if(!mines[x-1][9])
//    numbers[x-1][9]++;
////  if(!mines[x+1][9])
//    numbers[x+1][9]++;
////  if(!mines[x-1][8])
//    numbers[x-1][8]++;
////  if(!mines[x][8])
//    numbers[x][8]++;
////  if(!mines[x+1][8])
//    numbers[x+1][8]++;
//}

void setLeft(int y) {
  if(!mines[0][y-1])
    numbers[0][y-1]++;
  if(!mines[1][y-1])
    numbers[1][y-1]++;
  if(!mines[1][y])
    numbers[1][y]++;
  if(!mines[0][y+1])
    numbers[0][y+1]++;
  if(!mines[1][y+1])
    numbers[1][y+1]++;
}

//void setLeft(int y) {
////  if(!mines[0][y-1])
//    numbers[0][y-1]++;
////  if(!mines[1][y-1])
//    numbers[1][y-1]++;
////  if(!mines[1][y])
//    numbers[1][y]++;
////  if(!mines[0][y+1])
//    numbers[0][y+1]++;
////  if(!mines[1][y+1])
//    numbers[1][y+1]++;
//}

void setRight(int y) {
  if(!mines[9][y-1])
    numbers[9][y-1]++;
  if(!mines[8][y-1])
    numbers[8][y-1]++;
  if(!mines[8][y])
    numbers[8][y]++;
  if(!mines[8][y-1])
    numbers[8][y-1]++;
  if(!mines[9][y-1])
    numbers[9][y-1]++;
}

//void setRight(int y) {
////  if(!mines[9][y-1])
//    numbers[9][y-1]++;
////  if(!mines[8][y-1])
//    numbers[8][y-1]++;
////  if(!mines[8][y])
//    numbers[8][y]++;
////  if(!mines[8][y-1])
//    numbers[8][y-1]++;
////  if(!mines[9][y-1])
//    numbers[9][y-1]++;
//}


void getNumbers() {
  for (i = 1; i < 9; i++) {
    for (j = 1; j < 9; j++) {
      if (mines[i][j])
        setNonCorner(i, j);
    }
  }
  
  for (i = 1; i < 9; i++) {
    if (mines[i][0]) {
      setTop(i);
    }
    if (mines[i][9]) {
      setBot(i);
    }
  }
  
  for (j = 1; j < 9; j++) {
    if (mines[0][j]) {
      setLeft(j);
    }
    if (mines[9][j]) {
      setRight(j);
    }
  }

  if (mines[0][0]) {
    numbers[0][1]++;
    numbers[1][0]++;
    numbers[1][1]++;
  }
  if (mines[0][9]) {
    numbers[1][9]++;
    numbers[1][8]++;
    numbers[0][8]++;
  }
  if (mines[9][0]) {
    numbers[8][0]++;
    numbers[8][1]++;
    numbers[9][1]++;
  }
  if (mines[9][9]) {
    numbers[8][9]++;
    numbers[8][8]++;
    numbers[9][8]++;
  }
  Serial.print("Numbers");
  Serial.print("\n");
  for (i = 0; i < 10; i++) {
    for (j = 0; j < 10; j++) {
      Serial.print(numbers[i][j]);
    }
    Serial.print("\n");
  }
}

int getSquare(int x, int y) {
  if ((x >= 0 && x <=240) && (y >=0 && y <=240)) {
    int row, column;
    row = x / 24;
    column = y / 24;
    Serial.print("row = ");
    Serial.println(row);
    Serial.print("column = ");
    Serial.println(column);
    Serial.print("x = ");
    Serial.println(x);
    Serial.print("y = ");
    Serial.println(y);
    return row*10 + column;
  }
  else return -1;
}

void selectCell(int cell) {
  int cellX = getX(cell);
  int cellY = getY(cell);

  if (select[cellX][cellY] == 0)
    select[cellX][cellY] = 1;

  for (i = 0; i < 240; i += 24) {
    for (j = 0; j < 240; j += 24) {
      if (select[cellX][cellY] == 0)
        tft.fillRect(i, j, 22, 22, BLUE);
    }
  }

  for (i = 0; i < 10; i++) {
    for (j = 0; j < 10; j++) {
      if (select[i][j] == 1) {
        if (mines[i][j] == 1) { 
          tft.fillRect(i * 24, j * 24, 22, 22, RED);
          }
          else {
            tft.fillRect(i * 24, j * 24, 22, 22, GREEN);
    //        tft.setRotation(0);
            tft.setTextColor(BLACK);
            tft.setTextSize(2);
            tft.setCursor(i * 24+5, j * 24+5);
            tft.println(numbers[i][j]);
    //        tft.setRotation(1);
          }
      }
    }
  }

  
}

int getX(int n) {
  return n/10;
}
int getY(int n) {
  return n%10;
}

 void drawButtons() {
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.setCursor(240, 240);
  tft.println("reset");
 }

 
 void loop() {  
//  tft.setRotation(1);
  // Get the touch points  
  TSPoint p = ts.getPoint();  
  // Restore the mode  
  pinMode(XM, OUTPUT);  
  pinMode(YP, OUTPUT);  
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {  
   p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());  
   p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());  

  
  cell = getSquare(p.x, p.y);
  if (cell == -1) {
    Serial.print("x = ");
    Serial.println(p.x);
    Serial.print("y = ");
    Serial.println(p.y);
    if (p.x > 260)
      Serial.print("reset");
  }
  
  selectCell(cell);

  Serial.println(cell);
  cell = 0;

  delay(300);
  }
  drawButtons();
 }
