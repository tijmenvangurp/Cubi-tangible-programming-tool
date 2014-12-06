#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#define PIN            10
#define NUMPIXELS      8
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


const byte master_address = 10;
const byte led_pin = 3;
const byte pulse_line_down = 7;

const byte rows = 17;
const byte collums = 5;
byte id_array [rows][collums];
int pot_array [rows][collums];

int received_id = 0;
boolean first_id_received = false;
boolean message_recieved = false;
boolean line_ready = false;
boolean waiting_to_send_pot_values = false;

int collum_counter = 0;
int row_counter = 0;
String robot_message = "";
String slave_summary = "";
byte potmeter_values_to_send [64];
int potmeter_array_counter = 0;

enum search_direction {
  none, 
  new_line,
  left,
  right
};

//initialize a new instance of search direction
search_direction slave_search_direction = none;
unsigned long waiting_for_id_timeout = 0;
unsigned long no_pulse_response_timeout = 0;
unsigned long i2c_communications_delay = 0; 
boolean waiting_for_id = false ;


//button to start
const int  button_start = 12;
const int  button_loop = 11;
int button_state_start = 0;         // current state of the button
int lastButtonState_start = 0;     // previous state of the button

int button_state_loop = 0;         // current state of the button
int lastButtonState_loop = 0;     // previous state of the button



void setup()
{
  // initialize I2C
  Wire.begin(master_address);
  Wire.onReceive(receiveEvent);
  // initialize pins
  pinMode(led_pin,OUTPUT);
  pinMode(button_start,OUTPUT);
  pinMode(button_loop,OUTPUT);
  // initialize led strip master block
  pixels.begin();
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(100,100,100)); 
    pixels.show(); // This sends the updated pixel color to the hardware.
  }
  // initialize serial connection xbee
  Serial.begin(9600);

  // Serial.println("Master started");// can be removed
}

void loop(){

  if(waiting_to_send_pot_values && Serial.available() > 0 ){
    char robot_message = Serial.read();
    // robot sends a go once it has received all id values so we can send potvalues
    if(robot_message == 'G' ){
      waiting_to_send_pot_values = false;
      for(int i = 0; i < sizeof(potmeter_values_to_send); i++ ){
        Serial.write(potmeter_values_to_send[i]);
        /*
        //remember to change i++ to i+=2
         int test_print = potmeter_values_to_send[i] << 8;
         test_print |= potmeter_values_to_send[i+1];
         Serial.println(test_print);
         */
        // semd all the values from potmeter
        if(potmeter_values_to_send[i] == lowByte(5000)){
          break;
          // if we have the last byte we can stop sending, the array of bytes is filled with 0 after 5000
        }
      }
    }
  }
  else if(Serial.available() > 1){
    String id_to_light_up = "";
    char robot_message = Serial.read();
    id_to_light_up += robot_message;
    robot_message = Serial.read();
    id_to_light_up += robot_message;
    int active_block = id_to_light_up.toInt();
    error_message(active_block,'G');
    Serial.print("make block nr ");
    Serial.print(active_block);
    Serial.println(" green");
  }
  check_buttons();
  if(millis() - i2c_communications_delay > 500){// once we have finished all the important jobs ge give 500 miliseconds the time wherein the line can be broken, 
    //the larger the number the lower the chance i2c gets disturbed by a disconnection but the slower the discovery protocol works
    init_i2c();
    save_id();
    request_timout_change_search_direction();
    send_request_to_id_or_reset();
  }

}

void send_pulses_down() {
  // Serial.println(F("Sending pulses down"));
  digitalWrite(led_pin, HIGH);
  pinMode(pulse_line_down, OUTPUT);
  digitalWrite(pulse_line_down, LOW);
  delayMicroseconds(30);
  digitalWrite(pulse_line_down, HIGH);
  delayMicroseconds(30);
  digitalWrite(pulse_line_down, LOW);
}

































