const byte rows = 17;
const byte collums = 5;
byte id_array [rows][collums];
int pot_array [rows][collums];
const byte pattern_rows = 18;
const byte pattern_collums = 6;
int robot_drive_pattern [pattern_rows][pattern_collums];
int robot_drive_pattern_counter = 0;

static byte normal_speed = 255;
static byte minimum_speed = 0;
static byte maximum_speed = 255;
byte global_speed_robot = 255;
byte local_speed_robot [2] = {
  0,0};

byte local_speed_set = 255;

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
const int max_time = 5000;
const int min_time = 100;
const int middle_knob = 513;
const int end_knob = 1025;
const int begin_knob = 1;


unsigned long drive_timeout = 0;
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
    // robot execution array is filled, important TODO clear out the array here
    global_speed_robot = normal_speed;
    robot_drive_pattern_counter = 0; // is increased in the get function block by id function
    // when we have blocks in the id array, make the driving array for what the robot should do
    for (int level = 0; level < rows; level++) {

      if(id_array [level][0] =! 0){
        // there is a block in this level ( if it is a speed block the function get function block id will handle the rest )
        get_function_block_by_id(id_array [level][0], pot_array[level][0]);
        local_speed_set = global_speed_robot; // if local speed is not the same as global speed local speed is set
        if(id_array [level][1] =! 0){
          robot_drive_pattern_counter--; // we handle it as a collum we dont want 2 the same entries in the array so we need to set the counter one back
          // there are at least 2 blocks in this level so start combining or combine speed blocks or both omg...
          // but first handle speed blocks
          int current_number_of_collums = 0;
          local_speed_robot[0] = 0;
          local_speed_robot[1] = 0;
          int nr_of_speed_blocks = 0;
          // there are certainly more collums so start for loop to combine blocks set speed vallue local
          for(int current_collum = 0; current_collum < collums ; current_collum++){
            // check if there are more cullums
            int current_function_id = round(floor((id_array [level][current_collum]/10)));
            if(current_function_id =! 0){
              current_number_of_collums++;
              if( current_function_id == speed_function){
                // save value of speed blocks
                local_speed_robot[nr_of_speed_blocks] = round(map(pot_array[level][current_collum], begin_knob, end_knob, minimum_speed, maximum_speed));
                // TODO also set a number execution array otherwise we have a empty place
                nr_of_speed_blocks++;
              }
            }
            else{
              // there are no more blocks this collum
              break;
            }
          }// finished checking nr of speed blocks

          if(nr_of_speed_blocks > 0 ){
            if(current_number_of_collums == nr_of_speed_blocks ){
              // there are at least 2 blocks, and nr of blocks is the same as number of speed blocks so combine speed and set global
              global_speed_robot = round(((local_speed_robot[0]+local_speed_robot[1])/2));
            }
            else if(nr_of_speed_blocks == 2){
              // there are 2 speed blocs but more than 2 blocks in this collum so combine speed values and set local speed
              local_speed_set = round(((local_speed_robot[0]+local_speed_robot[1])/2));
            }
            else{
              // there is only one speed block, but in total 2 blocks at this collum set this local
              local_speed_set = local_speed_robot[0];
            }
          }
          // nr of blocks to combine is current_number_of_collums - nr_of_speed_blocks
          if(current_number_of_collums - nr_of_speed_blocks > 0){
            // there are non speed blocks to process
            boolean combine_functions = false;
            for(int current_collum = 0 ; current_collum < current_number_of_collums ; current_collum++ ){
              if(id_array [level][current_collum]!= 0 ){
                int id_function = round(floor((id_array [level][current_collum]/10)));
                if(id_function =! speed_function){
                  // skipp all blocks that have a speed setting
                  if(!combine_functions){
                    // just save normaly
                    get_function_block_by_id(id_array [level][current_collum], pot_array[level][current_collum]);
                  }
                  else{
                    // we already have a setting saved so combine current reading with previous and decrease counter
                    get_function_block_by_id(id_array [level][current_collum], pot_array[level][current_collum]); // saves it on new line
                    // combine current block with previous
                    robot_drive_pattern_counter--; // make sure the next block overwrites the current values by decreasing counter
                    combine_drive_function_blocks();
                  }
                  combine_functions = true;
                }
              }
              else{
                // no more blocks in this collum
                break;
              }
            }

          }
          else{
            // we are done this level go to next level
          }
        }
        else{
          // there was just one block in this level go to next level
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
