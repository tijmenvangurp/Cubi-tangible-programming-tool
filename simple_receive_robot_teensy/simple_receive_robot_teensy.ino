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
          int combined_id_nummerator = robot_drive_pattern[level][0] + robot_drive_pattern[level+1][0];

          if{}


          // in case one is forward/backwards and the otherone left/right, make a forwards turn, or backwards turn
          switch (combined_id_nummerator){
          case 4:
            {
              if(robot_drive_pattern[level][1] == robot_drive_pattern[level][1]){
                // in case both are driving forward or both are driving backwards: add the time driving forward/backwards
                robot_drive_pattern[level][3] += robot_drive_pattern[level+1][3];
              }
              else{
                // one is forward the otherone is backward
                int result_combined_forward_backward = 0;
                if(robot_drive_pattern[level][1] == 1){
                  // level block = forwards en level+1 = backwards
                  result_combined_forward_backward = robot_drive_pattern[level][3] - robot_drive_pattern[level+1][3];
                  if(result_combined_forward_backward >= 0){
                    // result is driving forward
                    robot_drive_pattern[level][1] = 1;
                    robot_drive_pattern[level][2] = 1;
                    robot_drive_pattern[level][3] = result_combined_forward_backward;
                  }
                  else{
                    // result is driving backwards
                    robot_drive_pattern[level][1] = 0;
                    robot_drive_pattern[level][2] = 0;
                    robot_drive_pattern[level][3] = (result_combined_forward_backward *-1);
                  }
                }
                else{
                  // level block = backwards en level+1 = forwards
                  result_combined_forward_backward = robot_drive_pattern[level+1][3] - robot_drive_pattern[level][3];
                  if(result_combined_forward_backward >= 0){
                    // result is driving forward
                    robot_drive_pattern[level][1] = 1;
                    robot_drive_pattern[level][2] = 1;
                    robot_drive_pattern[level][3] = result_combined_forward_backward;
                  }
                  else{
                    // result is driving backwards
                    robot_drive_pattern[level][1] = 0;
                    robot_drive_pattern[level][2] = 0;
                    robot_drive_pattern[level][3] = (result_combined_forward_backward *-1);
                  }

                }

              }

            }

          case 5:
            {
              // one is forward/backwards the otherone is turning
              if(robot_drive_pattern[level][0] = 2){
                // level block was forward/backwards


                robot_drive_pattern[level+1][3] = time_to_drive_forward;// time to turn makes the ammount of speed difference between the motors
                robot_drive_pattern[level][4] = speed_robot;// left motor
                robot_drive_pattern[level][5] = speed_robot;// right motor

              }
              else{
                // level block was forwards/backwards

              }

            }

          }



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
