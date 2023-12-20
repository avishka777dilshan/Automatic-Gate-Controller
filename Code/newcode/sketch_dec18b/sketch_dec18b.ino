// Define register addresses for ATmega328P (Arduino Uno's microcontroller)
#define PORTB_REG_ADDR 0x25   // PORTB register address
#define DDRB_REG_ADDR 0x24    // DDRB register address
#define PINB_REG_ADDR 0x23    // PINB register address

// Define constants for trigPin and echoPin
#define TRIG_PIN_NUM 1       // Pin 9 corresponds to bit 1 of PORTB
#define ECHO_PIN_NUM 2       // Pin 10 corresponds to bit 2 of PORTB
//#define Relay 
// Function to initialize the setup
void mySetup() {

  // Set bit 5 of DDRB to 1 to configure pin 13 (LED_BUILTIN) as an output
  volatile uint8_t *ddrBRegL = (volatile uint8_t *)DDRB_REG_ADDR;
  *ddrBRegL |= (1 << DDB0);
  
  // Set bit 1 of DDRB to 1 to configure pin 9 (trigPin) as an output
  volatile uint8_t *ddrBReg = (volatile uint8_t *)DDRB_REG_ADDR;
  *ddrBReg |= (1 << TRIG_PIN_NUM);
  
  // Set bit 2 of DDRB to 0 to configure pin 10 (echoPin) as an input
  *ddrBReg &= ~(1 << ECHO_PIN_NUM);
  
  // Enable pull-up resistor for echoPin
  volatile uint8_t *portBReg = (volatile uint8_t *)PORTB_REG_ADDR;
  *portBReg |= (1 << ECHO_PIN_NUM);
  
  // Initialize Serial communication
  // Assuming 9600 baud rate
  // UBRRn = (F_CPU / (16 * baud)) - 1
  UBRR0H = 0;
  UBRR0L = 103;
  UCSR0B |= (1 << TXEN0) | (1 << RXEN0);  // Enable transmitter and receiver
  UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // Set frame format: 8 data bits, 1 stop bit
}

// Function to implement the main logic
void myLoop() {
  // Trigger pulse
  volatile uint8_t *portBReg = (volatile uint8_t *)PORTB_REG_ADDR;
  *portBReg &= ~(1 << TRIG_PIN_NUM); // Set trigPin low
  _delay_us(2);
  *portBReg |= (1 << TRIG_PIN_NUM);  // Set trigPin high
  _delay_us(10);
  *portBReg &= ~(1 << TRIG_PIN_NUM); // Set trigPin low

  // Measure duration of pulse on echoPin
  volatile uint8_t *pinBReg = (volatile uint8_t *)PINB_REG_ADDR;
  uint32_t pulseDuration = 0;
  while (!(*pinBReg & (1 << ECHO_PIN_NUM))) {
    _delay_us(1);
  }
  while (*pinBReg & (1 << ECHO_PIN_NUM)) {
    pulseDuration++;
    _delay_us(1);
  }

  // Convert duration to distance
  float distance = (pulseDuration * 0.0343) / 2;

  // Print distance to Serial
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'D';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = 'i';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = 's';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = 't';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = 'a';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = 'n';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = 'c';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = 'e';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = ':';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = ' ';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = (int)(distance / 10) + '0';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = (int)(distance) % 10 + '0';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = '\n';

if (distance < 5.0) {
   // Distance is 5 or more, turn on the built-in LED
    volatile uint8_t *portBRegL = (volatile uint8_t *)PORTB_REG_ADDR;
    *portBRegL |= (1 << PORTB0);  // Assuming PORTB5 corresponds to the built-in LED
  } else {
    // Distance is less than 5, turn off the built-in LED
    volatile uint8_t *portBRegL = (volatile uint8_t *)PORTB_REG_ADDR;
    *portBRegL &= ~(1 << PORTB0);  // Assuming PORTB5 corresponds to the built-in LED
  }


  // Delay before next measurement
  _delay_ms(100);
}

// Main program
int main() {
  mySetup();  // Call the setup function

  // Run the main loop indefinitely
  while (1) {
    myLoop();  // Call the loop function
  }

  return 0;
}
