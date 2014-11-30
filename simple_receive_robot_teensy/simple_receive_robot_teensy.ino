const byte rows = 17;
const byte collums = 5;
byte id_array [rows][collums];
int pot_array [rows][collums];

const byte pattern_rows = 18;
unsigned long drive_timeout = 0;
int robot_drive_pattern [pattern_rows][6];
byte speed_robot = 255;

int collum_counter = 0;
int row_counter = 0;
boolean waiting_for_pot_values = false;
boolean execute_driving = false;
boolean in_execution = false;

byte execution_row_counter = 0; 

String incomming_id = "";
void setup(){
  Serial.begin(9600);
  Serial2.begin(9600);
}
void loop(){

  if (Serial2.available() >= 2){
    if(waiting_for_pot_values){
      handle_pot_message();
    }
    else{
      handle_id_message();
    }
  }  
  else if(id_array[0][0] =! 0){
    // when we have blocks in the id array, make the driving array for what the robot should do
    for (int level = 0; level < rows; level++) {
      if(id_array [level][0] =! 0){
        // check what function this block has
        get_function_block_by_id(id_array [level][0], pot_array[level][0], level, true);
        // todo: if it is a speed block, go to the next don't save it
        
        if(id_array [level][1] =! 0){
          // get function block id save it temporarely to the next line of the 2D array so we can compare and combine it
          get_function_block_by_id(id_array [level+1][1], pot_array[level+1][1],level,false);
          // todo: combine functions
          robot_drive_pattern[level][0];

        }
      }
      else{
        // no more id's in the message 
        execute_driving = true;
        break; // not necesarry to look for more id's if ther are no more so break the for loop
      }
    }
    // we are ready constructing the driving pattern so the robot can start
  }

  if(execute_driving){
    execution_blocks();
  }
}


















































