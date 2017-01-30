/*
  ESP-WROVER-KIT RGB LED demo

  Based on Kris Winer's public domain ESP32 PWM demo:
  https://github.com/kriswiner/ESP32/tree/master/PWM

  As modified by Martin Falatic

  Runs through the full 255 color spectrum for an rgb led
  Demonstrate ledcWrite functionality for driving leds with PWM on ESP32
*/

// LED GPIO pins
uint8_t ledR = 0;
uint8_t ledG = 2;
uint8_t ledB = 4;

uint8_t ledArray[3] = {1, 2, 3}; // PWM led channels
const boolean invert = false; // set true if common anode, false if common cathode

uint32_t R, G, B;          // the Red Green and Blue color components
uint8_t brightness = 255;  // 255 is maximum brightness, but can be changed


void setup()
{
  // Assign RGB led pins to channels
  ledcAttachPin(ledR, 1);
  ledcAttachPin(ledG, 2);
  ledcAttachPin(ledB, 3);

  // Initialize channels
  // channels 0-15, resolution 1-16 bits, freq limits depend on resolution
  // ledcSetup(uint8_t channel, uint32_t freq, uint8_t resolution_bits);
  ledcSetup(1, 12000, 8); // 12 kHz PWM, 8-bit resolution
  ledcSetup(2, 12000, 8);
  ledcSetup(3, 12000, 8);

  for (uint8_t i = 0; i < 3; i++) {
    // ledcWrite(channel, dutycycle)
    // For 8-bit resolution duty cycle is 0 - 255
    ledcWrite(ledArray[i], 255);  // test high output of all leds in sequence
    delay(1000);
    ledcWrite(ledArray[i], 0);
  }
}


void loop()
{
  for (uint8_t hue = 0; hue < 255; hue++) { // Slew through the color spectrum
    hueToRGB(hue, brightness);  // call function to convert hue to RGB
    ledcWrite(1, R);
    ledcWrite(2, G);
    ledcWrite(3, B);
    delay(20); // About 5s per cycle
  }
}


void hueToRGB(uint8_t hue, uint8_t brightness)
{
  // Courtesy http://www.instructables.com/id/How-to-Use-an-RGB-LED/?ALLSTEPS
  // function to convert a color to its Red, Green, and Blue components.

  uint16_t scaledHue = (hue * 6);
  uint8_t segment = scaledHue / 256; // segment 0 to 5 around the
  // color wheel
  uint16_t segmentOffset =
    scaledHue - (segment * 256); // position within the segment

  uint8_t complement = 0;
  uint16_t prev = (brightness * ( 255 -  segmentOffset)) / 256;
  uint16_t next = (brightness *  segmentOffset) / 256;

  if (invert)
  {
    brightness = 255 - brightness;
    complement = 255;
    prev = 255 - prev;
    next = 255 - next;
  }

  switch (segment ) {
    case 0:      // red
      R = brightness;
      G = next;
      B = complement;
      break;
    case 1:     // yellow
      R = prev;
      G = brightness;
      B = complement;
      break;
    case 2:     // green
      R = complement;
      G = brightness;
      B = next;
      break;
    case 3:    // cyan
      R = complement;
      G = prev;
      B = brightness;
      break;
    case 4:    // blue
      R = next;
      G = complement;
      B = brightness;
      break;
    case 5:      // magenta
    default:
      R = brightness;
      G = complement;
      B = prev;
      break;
  }
}

