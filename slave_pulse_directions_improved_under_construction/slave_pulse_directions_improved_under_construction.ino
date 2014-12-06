#include <Wire.h>
const byte master_address = 10;
const byte led_pin = 13;

// BLOCK ID
const byte block_id = 41;

boolean message_recieved = false;

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
// led variables
boolean start_green = true;
unsigned long error_blink_timout = 0;
long previous_blink_timout = 0;
byte let_state = LOW;
boolean done_with_green = false;

boolean error = false;

int pulse_counter = 0;

const byte sensor_pin = A0;
const byte r = 3;
const byte g = 10;
const byte b = 11;

byte amount_red = 80;
byte amount_green = 255;
byte amount_blue = 255;

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
  if(error){
    //Serial.println("I should blink here");
    analogWrite(r,200);
    analogWrite(g,0);
    analogWrite(b,0);
  }





  if((millis() - switch_pulse_direction_timout) > 3000){
    handshake_listening_direction = pulse_line_above;
    send_pulses = false;
    listen_for_pulses = true;
    rgb_off();
    start_green = true;
    done_with_green = false;
    digitalWrite(led_pin, LOW);
  }
  if(message_recieved){
    message_recieved = false;
    error = false;
    //    Serial.print(message[0]);
    //    Serial.println(message[1]);
    //--------------------------------- Check if message was for changing the INPUT direction
    if(message[0]== 'I'){

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
    }
    //--------------------------------- Check if message was for changing the OUTPUT direction
    else{
      switch (message[0]){
      case 'R':
        handshake_sending_direction = pulse_line_right;
        send_pulses = true;
        listen_for_pulses = false;
        digitalWrite(led_pin, HIGH);
        break;
      case 'D':
        handshake_sending_direction = pulse_line_down;
        send_pulses = true;
        listen_for_pulses = false;
        digitalWrite(led_pin, HIGH);
        break;
      case 'L':
        handshake_sending_direction = pulse_line_left;
        send_pulses = true;
        listen_for_pulses = false;
        digitalWrite(led_pin, HIGH);
        break;
      case 'B':
        error = true; 
        // Serial.println("WRONG SO BLINK DUDE");
        break;
      case 'G':
      if(amount_red > 0){
        amount_red = 0;
        amount_green = 255;
        amount_blue = 0;
      }else{
        amount_red = 80;
        amount_green = 255;
        amount_blue = 255;
            }
     
        break;
      }
    }
  }
  else{
    if(send_pulses){
      send_pulses_to_direction();
    }
    else if(listen_for_pulses){
      listen_for_pulses_from_direction();
    }
   
   if(!done_with_green){
      if(start_green){
        error_blink_timout = millis();
        start_green = false;
      }
      else if( millis()- error_blink_timout < 1500){
        amount_red = 0;
        amount_green = 255;
        amount_blue = 255;
      }
      else{
        done_with_green = true;
        amount_red = 80;
        amount_green = 255;
        amount_blue = 255;
      }
    }
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

    // if(done_with_green){
    rgb_on();
    //  }

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
  analogWrite(r,amount_red);
  analogWrite(g,amount_green);
  analogWrite(b,amount_red);
}
void rgb_off(){
  analogWrite(r,0);
  analogWrite(g,0);
  analogWrite(b,0);
}


























