// Define register addresses for ATmega328P
#define PORTB_REG_ADDR 0x25   // PORTB register address
#define DDRB_REG_ADDR 0x24    // DDRB register address
#define PINB_REG_ADDR 0x23    // PINB register address

// Define constants for trigPin and echoPin
#define TRIG_PIN_NUM1 1       // Pin 9 corresponds to bit 1 of PORTB
#define ECHO_PIN_NUM1 2       // Pin 10 corresponds to bit 2 of PORTB

#define TRIG_PIN_NUM2 4       // Pin 9 corresponds to bit 1 of PORTB
#define ECHO_PIN_NUM2 5       // Pin 10 corresponds to bit 2 of PORTB
//#define Relay 
// Function to initialize the setup
void mySetup() {

  // Set bit 5 of DDRB to 1 to configure pin 13 as an output
  volatile uint8_t *Relay1 = (volatile uint8_t *)DDRB_REG_ADDR;
  *Relay1 |= (1 << DDB0);

  volatile uint8_t *Relay2 = (volatile uint8_t *)DDRB_REG_ADDR;
  *Relay2 |= (1 << DDB3);
  
  // Set bit 1 of DDRB to 1 to configure pin 9 (trigPin) as an output
  volatile uint8_t *trig1 = (volatile uint8_t *)DDRB_REG_ADDR;
  *trig1 |= (1 << TRIG_PIN_NUM1);
  
  // Set bit 2 of DDRB to 0 to configure pin 10 (echoPin) as an input
  volatile uint8_t *eco1 = (volatile uint8_t *)DDRB_REG_ADDR;
  *eco1 &= ~(1 << ECHO_PIN_NUM1);
  
  // Enable pull-up resistor for echoPin
  volatile uint8_t *ecop1 = (volatile uint8_t *)PORTB_REG_ADDR;
  *ecop1 |= (1 << ECHO_PIN_NUM1);

  // Set bit 1 of DDRB to 1 to configure pin 9 (trigPin) as an output
  volatile uint8_t *trig2 = (volatile uint8_t *)DDRB_REG_ADDR;
  *trig2 |= (1 << TRIG_PIN_NUM2);
  
  // Set bit 2 of DDRB to 0 to configure pin 10 (echoPin) as an input
  volatile uint8_t *eco2 = (volatile uint8_t *)DDRB_REG_ADDR;
  *eco2 &= ~(1 << ECHO_PIN_NUM2);
  
  // Enable pull-up resistor for echoPin
  volatile uint8_t *ecop2 = (volatile uint8_t *)PORTB_REG_ADDR;
  *ecop2 |= (1 << ECHO_PIN_NUM2);
  
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
  // Trigger pulse 1
  volatile uint8_t *trig1 = (volatile uint8_t *)PORTB_REG_ADDR;
  *trig1 &= ~(1 << TRIG_PIN_NUM1); // Set trigPin low
  _delay_us(2);
  *trig1 |= (1 << TRIG_PIN_NUM1);  // Set trigPin high
  _delay_us(10);
  *trig1 &= ~(1 << TRIG_PIN_NUM1); // Set trigPin low

  // Measure duration of pulse on echoPin
  volatile uint8_t *eco1 = (volatile uint8_t *)PINB_REG_ADDR;
  uint32_t pulseDuration1 = 0;
  while (!(*eco1 & (1 << ECHO_PIN_NUM1))) {
    _delay_us(1);
  }
  while (*eco1 & (1 << ECHO_PIN_NUM1)) {
    pulseDuration1++;
    _delay_us(1);
  }

  // Convert duration to distance1
  float distance1 = (pulseDuration1 * 0.0343) / 2;
 // _delay_ms(1000);
  
// Trigger pulse 2
  volatile uint8_t *trig2 = (volatile uint8_t *)PORTB_REG_ADDR;
  *trig2 &= ~(1 << TRIG_PIN_NUM2); // Set trigPin low
  _delay_us(2);
  *trig2 |= (1 << TRIG_PIN_NUM2);  // Set trigPin high
  _delay_us(10);
  *trig2 &= ~(1 << TRIG_PIN_NUM2); // Set trigPin low

  // Measure duration of pulse on echoPin
  volatile uint8_t *eco2 = (volatile uint8_t *)PINB_REG_ADDR;
  uint32_t pulseDuration2 = 0;
  while (!(*eco2 & (1 << ECHO_PIN_NUM2))) {
    _delay_us(1);
  }
  while (*eco2 & (1 << ECHO_PIN_NUM2)) {
    pulseDuration2++;
    _delay_us(1);
  }

  // Convert duration to distance1
  float distance2 = (pulseDuration2 * 0.0343) / 2;
  distance2=distance2/5;
 // _delay_ms(1000);
  
// distance 1 print
if (distance1 <1.0){
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'N';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'A';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = '\n';
  }

if (distance1 >50){
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'O';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'U';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'T';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = '\n';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = ' ';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = ',';
  }
  
  
else{
  // Print distance1 to Serial
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'D';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
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
  UDR0 = '1';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = ':';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = ' ';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = (int)(distance1 / 10) + '0';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = (int)(distance1) % 10 + '0';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = ',';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = '\n';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = ' ';
  
  
  }

//distance 2 print

if (distance2 <1.0){
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'N';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'A';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = '\n';
  }

if (distance2 >50){
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'O';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'U';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'T';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = '\n';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = ' ';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = ',';
  }
  
  
else{
  // Print distance1 to Serial
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
  UDR0 = '2';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = ':';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = ' ';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = (int)(distance2 / 10) + '0';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = (int)(distance2) % 10 + '0';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = ',';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = '\n';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = ' ';
  
  
  }
  
//motor controll
if (distance1 < 5.0 || distance2 < 5.0) {
   // distance1 is 5 or more, turn on the built-in LED
    volatile uint8_t *Relay1 = (volatile uint8_t *)PORTB_REG_ADDR;
    *Relay1 |= (1 << PORTB0);  // Assuming PORTB5 corresponds to the built-in LED

  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'O';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'P';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'E';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'N';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'I';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'N';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'G';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = '\n';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = ' ';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = ',';
  _delay_ms(2000);

  // distance1 is less than 5, turn off the built-in LED
   // volatile uint8_t *Relay1 = (volatile uint8_t *)PORTB_REG_ADDR;
    *Relay1 &= ~(1 << PORTB0);  // Assuming PORTB5 corresponds to the built-in LED

  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'H';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'O';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'L';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'D';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'I';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'N';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'G';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = ' ';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = ',';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = '\n';

  _delay_ms(4000);


   volatile uint8_t *Relay2 = (volatile uint8_t *)PORTB_REG_ADDR;
    *Relay2 |= (1 << PORTB3);  // Assuming PORTB5 corresponds to the built-in LED

  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'C';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'L';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'O';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'S';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'E';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'I';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'N';
  while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
  UDR0 = 'G';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = ' ';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = ',';
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = '\n';
  
  _delay_ms(2000);
  
  *Relay2 &= ~(1 << PORTB3);
    
  }
    
    
   else {
    // distance1 is less than 5, turn off the built-in LED
    volatile uint8_t *Relay1 = (volatile uint8_t *)PORTB_REG_ADDR;
    *Relay1 &= ~(1 << PORTB0);  // Assuming PORTB5 corresponds to the built-in LED
    volatile uint8_t *Relay2 = (volatile uint8_t *)PORTB_REG_ADDR;
    *Relay2 &= ~(1 << PORTB3);  // Assuming PORTB5 corresponds to the built-in LED

  
  }


  // Delay before next measurement
  _delay_ms(2000);
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
