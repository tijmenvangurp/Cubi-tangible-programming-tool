#include <Wire.h>
const byte master_address = 10;
const byte led_pin = 13;

// BLOCK ID
const byte block_id = 60;

boolean message_recieved = false;

boolean blink_led = false;
int led_state = LOW;
unsigned long time_stamp_led_change = 0;        // will store last time LED was updated

char message[10];


boolean send_pulses = false;
boolean listen_for_pulses = true;
const byte pulse_line_above = 9;
const byte pulse_line_down = 7;
const byte pulse_line_right = 8;
const byte pulse_line_left = 6;

int handshake_listening_direction = pulse_line_above;
int handshake_sending_direction;

unsigned long duration_pulse_in;
unsigned long duration_between_pulses = 0;
unsigned long switch_pulse_direction_timout = 0;

int pulse_counter = 0;

const byte sensor_pin = A0;
const byte r = 3;
const byte g = 10;
const byte b = 11;

void setup()
{
  Wire.begin(block_id);
  TWAR = (block_id << 1) | 1;  // enable broadcasts to be received
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  pinMode(sensor_pin, INPUT);
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(13, OUTPUT);

  switch_pulse_direction_timout = millis();

  Serial.begin(9600);
  Serial.println(block_id);
  Serial.println(TWAR, BIN);
}

void loop(){
  // Serial.println(millis() - switch_pulse_direction_timout);

  if((millis() - switch_pulse_direction_timout) > 3000){
    handshake_listening_direction = pulse_line_above;
    send_pulses = false;
    listen_for_pulses = true;
    rgb_off();
    digitalWrite(led_pin, LOW);
  }
  if(message_recieved){
    message_recieved = false;
    //    Serial.print(message[0]);
    //    Serial.println(message[1]);

    char identifyer_character = message[0];
    switch (identifyer_character){
    case  'I':
      //--------------------------------- Check if message was for changing the INPUT direction
      {
        switch (message[1]){
        case 'U':
          handshake_listening_direction = pulse_line_above;
          // Serial.println("message recieved listen to block up");
          break;
        case 'R':
          handshake_listening_direction = pulse_line_right;
          break;
        case 'L':
          handshake_listening_direction = pulse_line_left;
          break;
        }
        send_pulses = false;
        listen_for_pulses = true;
        digitalWrite(led_pin, LOW);
        break;
      }
      case 'B'{
        //--------------------------------- There was an error: let this block blink
        blink_led = true;
        break;
      }

    default:
      //--------------------------------- Check if message was for changing the OUTPUT direction
      {
        switch (message[0]){
        case 'R':
          handshake_sending_direction = pulse_line_right;
          break;
        case 'D':
          handshake_sending_direction = pulse_line_down;
          break;
        case 'L':
          handshake_sending_direction = pulse_line_left;
          break;
        }
        send_pulses = true;
        listen_for_pulses = false;
        digitalWrite(led_pin, HIGH);
        break;
      }
    }
  }
  if(blink_led){
    unsigned long current_millis = millis();
    if(current_millis - time_stamp_led_change > 250){
      time_stamp_led_change = current_millis;
      if(led_state == LOW){
        analogWrite(r,150);
        led_state = HIGH;
      }
      else{
        rgb_off();
        led_state = LOW;     
      }
    }
  }

  if(send_pulses){
    send_pulses_to_direction();
  }
  if(listen_for_pulses){
    listen_for_pulses_from_direction();
  }
}


void receiveEvent(int howMany)
{
  for(int i = 0; i < howMany; i++){
    char c = Wire.read();
    message[i] = c;
  }
  message_recieved = true;
}

void requestEvent()
{
  int val = analogRead(sensor_pin);
  byte buf [2];
  buf [0] = val >> 8;
  buf [1] = val & 0xFF;
  Wire.write (buf, 2);
}

void listen_for_pulses_from_direction()
{

  if(pulse_counter > 3 || pulse_counter < 0){
    //Serial.print(pulse_counter);s
    //  Serial.println(" :pulse counter reset");
    pulse_counter = 0;
  }
  if(pulse_counter == 0){
    duration_between_pulses = micros();
  }

  if((micros()- duration_between_pulses)>2000){
    pulse_counter = 0;
    //pulses by accidentally wire touching will be removed because we expect 3 pulses in time period of 1500 microseconds
  }
  pinMode(handshake_listening_direction, INPUT);

  duration_pulse_in = pulseIn(handshake_listening_direction, HIGH, 100); // listen for a pulse for max 100 microseconds

  if (pulse_counter < 3 && duration_pulse_in > 30 && duration_pulse_in < 40){
    //    duration_between_pulses = micros();
    // Serial.println(duration_pulse_in);
    pulse_counter++;
    // Serial.println(pulse_counter);
  }
  else if(pulse_counter >= 3 && (micros()- duration_between_pulses) < 2000){
    pulse_counter = 0;
    //  Serial.println("duration for 3 pulses ");
    //   Serial.println(micros()- duration_between_pulses);
    // And send my ID to the master
    switch_pulse_direction_timout = millis();
    if(!blink_led){
      rgb_on();
    }
    Wire.beginTransmission(master_address);
    Wire.write(block_id);              // sends its adress (ID)
    Wire.endTransmission();            // stop transmitting
    listen_for_pulses = false; // stop listening for pulses, my neigbour could still be sending wait for instructions from the master.
  }
}


void send_pulses_to_direction() {
  // Serial.println(F("Sending pulses down"));

  pinMode(handshake_sending_direction, OUTPUT);
  digitalWrite(handshake_sending_direction, LOW);
  delayMicroseconds(30);
  digitalWrite(handshake_sending_direction, HIGH);
  delayMicroseconds(30);
  digitalWrite(handshake_sending_direction, LOW);
}



void rgb_on(){
  analogWrite(r,80);
  analogWrite(g,225);
  analogWrite(b,225);
}
void rgb_off(){
  analogWrite(r,0);
  analogWrite(g,0);
  analogWrite(b,0);
}




