/****************************************************************************
 * This code is for the Brain in a Jar project.
 * It uses sample code, mostly from the LCD template written by Mark Bramwell
 * and includes samples from the Fast LED library to control the LED strip,
 * the Relay test code, and The Button Debounce code, all written by other 
 * authors.
 * 
 * This Code is rather simple and my contsain mistakes, redundancies, etc.
 * You are free to use, modify, and distribute this code as long as you 
 * include this portion of the code in your distrubution as well as comply 
 * with the restrictions listed in the code authored by others, who's 
 * samples were used in the creation of this code.
 * 
 ************** Operation **************
 * The LCD displays a Home menu
 * Pushing buttons on the LCD Keypad will change colors on the LED Strip and 
 * switch the relay on and off (which operated the fish tank pump.
 * 
 *****************************************************************************/

 
/*************  Program Log *************
 * 10-13-18
 * Tested sample program, written by Mark Bramwell, with modifications
 * successful...
 * Changed scripts on LCD
 * successful...
 * 
 * 10-15-18
 * Changed name to "Brain_In_A_Jar"
 * added debounce variables to stop button repeat
 * successful...
 * 
 * 10-17-18
 * Added RGB_LED Strip to program
 * successful...
 * 
 * 10-18-18
 * Added Relay
 * successful...
 * Changed LCD Display
 * 
 * 10-20-18
 * Final Version Complete
 * Cleaned up code
*/

// LCD
#include <LiquidCrystal.h>

// LED Strip 
#include <FastLED.h>

#define LED_PIN    3           // LED Strip Pin
#define NUM_LEDS    23
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

/* There are several different palettes of colors demonstrated here.

 FastLED provides several 'preset' palettes: 
 RainbowColors_p, 
 RainbowStripeColors_p,
 OceanColors_p, 
 CloudColors_p, 
 LavaColors_p, 
 ForestColors_p, and 
 PartyColors_p.

 Additionally, you can manually define your own color palettes, or you can write
 code that creates color palettes on the fly.  All are shown here.
*/

//  ***** Debounce Variables  *****
int buttonPin;    // the number of the pushbutton pin

//  Button Debouncing
int butval = LOW;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 150;    // the debounce time; increase if the output flickers


// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);      // Backlight is pin 10

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons
int read_LCD_buttons()

{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  

 // For V1.0 comment the other threshold and use the one below:
/*
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   
*/

 return btnNONE;  // when all others fail, return this...
}

void setup()
{
 // Relay
 pinMode(11, OUTPUT);// connected to S terminal of Relay
  
 // LCD Section 
 lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
 lcd.print(" Happy Halloween");  // Greeting message
 lcd.setCursor(0,1);            // move to the begining of the second line
 lcd.print(" Trick or Treat");
 delay( 1000 );
 lcd.setCursor(0,0);            // move to the begining of the firstd line
 
 
// RGB Strip Section
 delay( 2000 ); // power-up safety delay
 FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
 FastLED.setBrightness(  BRIGHTNESS );
    
 currentPalette = ForestColors_p;
 currentBlending = LINEARBLEND; 
}
 
void loop()
{

 lcd_key = read_LCD_buttons();  // read the buttons
 
//LED Strip
    
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    
    FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
    
 switch (lcd_key)               // depending on which button was pushed, we perform an action
 
 {
 
   case btnRIGHT:
     {
     debounce(btnRIGHT);
     Angry();
     break;
     }     
  
   case btnLEFT:
     {
     debounce(btnLEFT);
     Excited();
     break;
     }
     
   case btnUP:
     {
     debounce(btnUP);
     Happy();
     break;
     }
     
   case btnDOWN:
     {
     debounce(btnDOWN);
     Sad();
     break;
     }
     
   case btnSELECT:
     {
     debounce(btnSELECT);     
     Calm();
     break;
     }

     
//     case btnNONE:
//     {
//     lcd.print("NONE  ");
//     break;
//     }

 }
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

//  --------------------------- Moods ---------------------------
void  Sad(){
 lcd.clear();                   // Clears LCD
 lcd.setCursor(0,0);            // move to the begining of the first line
 lcd.print("  My Mood is:");
 lcd.setCursor(5,1);            // move to the 5th char of the second line
 lcd.print("Sad    ");
  currentPalette = OceanColors_p;
  currentBlending = LINEARBLEND;              
} 
void  Happy(){
 lcd.clear();                   // Clears LCD
 lcd.setCursor(0,0);            // move to the begining of the first line
 lcd.print("  My Mood is:");
 lcd.setCursor(5,1);            // move to the 5th char of the second line
 lcd.print("Happy    ");
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
 digitalWrite(10,HIGH);// turn relay ON
 delay(1000);// keep it ON for 1 seconds
 digitalWrite(10, LOW);// turn relay OFF
 delay(1000);// keep it OFF for 1 seconds               
}

void  Calm(){
 lcd.clear();                   // Clears LCD
 lcd.setCursor(0,0);            // move to the begining of the first line
 lcd.print("  My Mood is:");
 lcd.setCursor(5,1);            // move to the 5th char of the second line
 lcd.print("Calm    ");   
  currentPalette = CloudColors_p;
  currentBlending = LINEARBLEND; 
 digitalWrite(10,HIGH);// turn relay ON
 delay(500);// keep it ON for .5 seconds
 digitalWrite(10, LOW);// turn relay OFF
 delay(2000);// keep it OFF for 2 seconds            
}

void  Angry(){ 
 lcd.clear();                   // Clears LCD
 lcd.setCursor(0,0);            // move to the begining of the first line
 lcd.print("  My Mood is:");
 lcd.setCursor(5,1);            // move to the 5th char of the second line
 lcd.print("Angry    ");  
  currentPalette = LavaColors_p;
  currentBlending = LINEARBLEND; 
 digitalWrite(10,HIGH);// turn relay ON          
}

void  Excited(){  
 lcd.clear();                   // Clears LCD
 lcd.setCursor(0,0);            // move to the begining of the first line
 lcd.print("  My Mood is:");
 lcd.setCursor(5,1);            // move to the 5th char of the second line
 lcd.print("Excited    ");   
  currentPalette = PartyColors_p;
  currentBlending = LINEARBLEND;  
 digitalWrite(10,HIGH);// turn relay ON
 delay(2000);// keep it ON for 2 seconds
 digitalWrite(10, LOW);// turn relay OFF
 delay(500);// keep it OFF for .5 seconds          
}



//  --------------------------- Button DeBounce ---------------------------
void debounce(int but) {
  // read the state of the switch into a local variable:
  int reading = digitalRead(but);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        butval = !butval;
      }
    }
  }

  // set the Button
  digitalWrite(but, butval);

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}

