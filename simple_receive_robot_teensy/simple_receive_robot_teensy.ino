const byte rows = 17;
const byte collums = 5;
byte id_array [rows][collums];
int pot_array [rows][collums];
const byte pattern_rows = 18;
const byte pattern_collums = 7;
int robot_drive_pattern [pattern_rows][pattern_collums];
int robot_drive_pattern_counter = 0;

static byte normal_speed = 190;
static byte minimum_speed = 100;
static byte maximum_speed = 255;
byte global_speed_robot = normal_speed;
byte local_speed_robot [2] = {
  0,0};

byte local_speed_set = normal_speed;

int collum_counter = 0;
int row_counter = 0;
boolean waiting_for_pot_values = false;
boolean execute_driving = false;
boolean in_execution = false;
byte execution_row_counter = 0;

const byte driving_forwards_backwards = 2;
const byte driving_left_rigt = 3;
const byte speed_function = 4;
const byte delay_function = 5;
const byte loop_a = 6;
const byte loop_b = 7;
byte loop_a_counter = 0;
byte loop_b_counter = 0;
boolean loop_a_running = false;
boolean loop_b_running = false;
byte start_loop_a = 0;
byte start_loop_b = 0;

boolean loop_all = false;

// constant names for driving pattern array
const byte function_id_block = 0;
const byte left_wheel_direction = 1;
const byte right_wheel_direction = 2;
const byte driving_time = 3;
const byte left_wheel_speed = 4;
const byte right_wheel_speed = 5;
const byte forwards = 1;
const byte backwards = 0;


// changing the time will influence the distance robot will drive
const int max_time = 3000;
const int max_time_turning = 700;
const int min_time_turning = 50;

const int min_time = 100;
const int middle_knob = 513;
const int end_knob = 1024;
const int begin_knob = 1;

boolean start_constructing_patern = false;


unsigned long drive_timeout = 0;
String incomming_id = "";
// 23,22,21,20
const int AIA = 23;  // (pwm) pin 9 connected to pin A-IA 
const int AIB = 20;  // (pwm) pin 5 connected to pin A-IB 
const int BIA = 22; // (pwm) pin 10 connected to pin B-IA  
const int BIB = 21;  // (pwm) pin 6 connected to pin B-IB 


void setup(){
  //Serial.begin(9600);
  Serial2.begin(9600);
  pinMode(AIA, OUTPUT); // set pins to output
  pinMode(AIB, OUTPUT);
  pinMode(BIA, OUTPUT);
  pinMode(BIB, OUTPUT);
  //Serial.println("Robot started");
}
void loop(){
//  if(Serial.available()> 0){
//    char k = Serial.read();
//    Serial.println(k);
//    if(k == 'R'){
//      waiting_for_pot_values = false;
//    }
//  }

  if (Serial2.available() >= 2){
    if(waiting_for_pot_values){
      handle_pot_message();
    }
    else{
      handle_id_message();
    }
  }
  else if(id_array[0][0] != 0 && start_constructing_patern){
    // we are ready constructing the driving pattern so the robot can start
    construct_driving_pattern();
  }

  if(execute_driving){
    // TODO: when loop block is pressed this should continue to runn
    execution_blocks();
  }

}



