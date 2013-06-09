#include <Wire.h>
#include <utility/twi.h>

class twi
{
public:
  virtual ~twi() {};
  virtual byte read(uint8_t address, uint8_t* buffer, uint8_t length) = 0;
  virtual byte write(uint8_t address, uint8_t* buffer, uint8_t length) = 0;
};


class twi_impl : public twi
{
public:
  byte read(uint8_t address, uint8_t* buffer, uint8_t length)
  {
    byte nbytes = twi_readFrom(address, buffer, length, (uint8_t)true);
    return nbytes;
  }
  
  byte write(uint8_t address, uint8_t* buffer, uint8_t length)
  {
    byte nbytes = twi_writeTo(address, buffer, length, (uint8_t)true, (uint8_t)true);
    return nbytes;
  }
};

class twi_wire_impl : public twi
{
  byte read(uint8_t address, uint8_t* buffer, uint8_t length)
  {
    Wire.requestFrom(address, length);
    byte nbytes = 0;
    while(nbytes < length && Wire.available())
      buffer[nbytes++] = Wire.read();
    return nbytes;
  }
  
  byte write(uint8_t address, uint8_t* buffer, uint8_t length)
  {
    Wire.beginTransmission(address);
    byte nbytes = 0;
    while( nbytes<length )
      Wire.write(buffer[nbytes++]);
    Wire.endTransmission();
    return nbytes;
  }
};

class wii2st
{
public:
  wii2st(twi& bus)
  : m_bus(bus)
  {
          /*
Pin	Mouse/Joystick 0	Joystick 1
1	XB/Up	Up
2	XA/Down	Down
3	YA/Left	Left
4	YB/Right	Right
5	*Middle Button/joystick 1 up	Reserved
6	Left Button/Fire	 Fire
7	+5V DC	+5V DC
8	Ground	Ground
9	Right Button/Joystick 1 Fire	N/C
  */

    m_joy_masks[0] = JOY_UP;
    m_joy_masks[1] = JOY_DOWN;
    m_joy_masks[2] = JOY_LEFT;
    m_joy_masks[3] = JOY_RIGHT;
    m_joy_masks[4] = 0;
    m_joy_masks[5] = JOY_Y | JOY_B;
    m_joy_masks[6] = JOY_X | JOY_A;
  }

  int begin()
  {
    /// Setup Atari pins as output
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
    for(int pin=0; pin<ATARI_NBPINS; ++pin) {
      pinMode(ATARI_PIN1+pin, OUTPUT);
      digitalWrite(ATARI_PIN1+pin, HIGH);
    }
  
    if(Serial)
      Serial.println(F("Starting Wii to Atari-ST converter"));
    
    byte buffer[2];
    /// Initialize device in non-crypted mode
    buffer[0] = 0xF0;
    buffer[1] = 0x55;
    m_bus.write(WIIEXT_ADDR, buffer, 2);

    delay(I2C_DELAY);
    buffer[0] = 0xFB;
    buffer[1] = 0x00;
    m_bus.write(WIIEXT_ADDR, buffer, 2);
    
    
    /// Ask for the device ID
    delay(I2C_DELAY*8);
    buffer[0] = 0xFA;
    m_bus.write(WIIEXT_ADDR, buffer, 1);
    delay(13);
    m_bus.read(WIIEXT_ADDR, m_dev_id, sizeof(m_dev_id));
    
    if(Serial) {
      Serial.print(F("Device detected: "));
      for(int i=0; i<sizeof(m_dev_id); ++i) {
        if(i)
          Serial.print(F("-"));
        Serial.print((int)m_dev_id[i], HEX);
      }
      Serial.println();
    }
    /// No errors
    return 1;
  }
  
  int read_wii(void)
  {
    delay(I2C_DELAY);

    byte buffer[6] = {0, 0, 0, 0, 0, 0};
    byte length = sizeof(buffer);
    for(byte nbytes=0; nbytes < length; )
      nbytes += m_bus.read(WIIEXT_ADDR, buffer+nbytes, length-nbytes);
    
    m_changed = 0;
    for(int i=0; i<length; ++i) {
      if(buffer[i] != m_data[i])
        m_changed = 1;
      m_data[i] = buffer[i];
    }

    if(m_changed && Serial) {
      Serial.print(F("Data: "));
      for(int i=0; i<length; ++i)
        Serial.println((int)(0xFF & ~buffer[i]), HEX);
      Serial.println();
    }

    delay(I2C_DELAY);
    buffer[0] = 0x00;
    if(! m_bus.write(WIIEXT_ADDR, buffer, 1))
      return 0;
    
    /// No errors
    return 1;
  }
  
  int write_st(void)
  {
    if(! m_changed)
      return 1;
    
    short flags = ~(m_data[4] << 8 | m_data[5]);
    for(int pin=0; pin<ATARI_NBPINS; ++pin) {
      bool level = (flags & m_joy_masks[pin]);
      digitalWrite(ATARI_PIN1+pin, level ? LOW : HIGH);
    }

    /// No errors
    return 1;
  }
  
  void handle_serial()
  {
    if(! m_changed) {
      return;
    }
      
    short flags = ~(m_data[4] << 8 | m_data[5]);
    for(int pin=0; pin<ATARI_NBPINS; ++pin) {
      bool level = (flags & m_joy_masks[pin]);
      if(level)
        Serial.print(F(" X "));
      else
        Serial.print(F(" - "));
    }
    Serial.println();
  }
  
private:  
  static int const WIIEXT_ADDR = 0x52;
  
  static short const JOY_RIGHT = 0x8000;
  static short const JOY_DOWN  = 0x4000;
  static short const JOY_L     = 0x2000;
  static short const JOY_MINUS = 0x1000;
  static short const JOY_HOME  = 0x0800;
  static short const JOY_START = 0x0400;
  static short const JOY_R     = 0x0200;
  static short const JOY_ZL    = 0x0080;
  static short const JOY_B     = 0x0040;
  static short const JOY_Y     = 0x0020;
  static short const JOY_A     = 0x0010;
  static short const JOY_X     = 0x0008;
  static short const JOY_ZR    = 0x0004;
  static short const JOY_LEFT  = 0x0002;
  static short const JOY_UP    = 0x0001;

  
  static int const ATARI_PIN1   = 2;
  static int const ATARI_NBPINS = 7;
  short m_joy_masks[ATARI_NBPINS];

  static int const LED_PIN = 13;  

  static int const I2C_DELAY = 13;

  /// Holds an I2C bus interface
  twi& m_bus;
  
  /// Holds the detected device id
  byte m_dev_id[6];
  
  /// Holds previous data buffer
  byte m_data[6];
  
  /// Holds a flag indicating if we have to update
  /// the Atari-ST pins
  int m_changed;
};


//twi_impl i2c_bus;
twi_wire_impl i2c_bus;
wii2st   app(i2c_bus);

/// This is called once at startup
void setup()
{
  /// Initialize Serial for debug
  Serial.begin(19200);

  /// Initialize TWI in master mode
  //twi_init();
  Wire.begin();
  
  /// Initialize the app
  app.begin();
}


/// This is called repeatedly by the arduino main()
void loop()
{
  /// Read from wii and write to st
  if(app.read_wii())
    app.write_st();
  app.handle_serial();
}
