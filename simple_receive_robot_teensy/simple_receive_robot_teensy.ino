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
boolean block_in_wrong_position = false;
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
      if(id_array [level][0] =! 0 && block_in_wrong_position == false){
        // check what function this block has
        get_function_block_by_id(id_array [level][0], pot_array[level][0], level, true);

        // if it is a speed block, go to the next don't save it

        if(id_array [level][1] =! 0){
          // get function block id save it temporarely to the next line of the 2D array so we can compare and combine it
          get_function_block_by_id(id_array [level+1][1], pot_array[level+1][1],level,false);

        }
      }
      else if(block_in_wrong_position){
        // the robot should not work if the blocks are not in the right order
        execute_driving = false;
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


void drive( boolean direction_left, boolean direction_rigt, int speed_left, int speed_right){

}


void get_function_block_by_id(byte id, int pot_value,byte level, boolean global){
  /*
  motor_assignment[0]= indication what variables there are
   2 = driving forward/backwards
   3 = turning left/right
   4 = delay
   5 = speed
   6 = loop a
   7 = loop b
   motor_assignment[1]= direction left motor (1 = forward, 0 = backwards)  ||  the speed we should drive this direction || the ammount of time to repeat
   motor_assignment[2]= direction right motor (1 = forward, 0 = backwards)
   motor_assignment[3]= the ammount of time to drive this direction
   motor_assignment[4]= global speed value:  the speed for this block <-- we need this here because we change the speed setting globaly
   
   */
  // changing the time will influence the distance robot will drive
  static int max_time = 5000; 
  static int min_time = 100;
  static int middle_knob = 513;
  static int end_knob = 1025;
  static int begin_knob = 1;

  robot_drive_pattern[level][0] = round(floor((id/10)));

  switch  (round(floor((id/10)))){
  case 2:
    {
      //drive forward/backward
      if(pot_value <= middle_knob){
        //drive forward
        int time_to_drive_forward = round(map(pot_value,begin_knob,middle_knob,min_time,max_time));
        // both motors should go forward
        robot_drive_pattern[level][1] = 1;
        robot_drive_pattern[level][2] = 1;
        robot_drive_pattern[level][3] = time_to_drive_forward;
      }
      else{
        //drive backward
        // both motors should go forward
        int time_to_drive_backward = round(map(pot_value,middle_knob+1,end_knob,min_time,max_time));
        robot_drive_pattern[level][0] = 0;
        robot_drive_pattern[level][1] = 0;
        robot_drive_pattern[level][2] = time_to_drive_backward;
        robot_drive_pattern[level][3] = speed_robot;
      }
      break;
    }
  case 3:
    {
      //drive left/right
      break;
    }
  case 4:
    {
      // delay
      break;
    }
  case 5:
    {
      //speed
      if(global){
        speed_robot = round(map(pot_value,begin_knob,end_knob,0,255));
        robot_drive_pattern[level][0] = 3;
        robot_drive_pattern[level][3] = speed_robot;
      }
      else{
        robot_drive_pattern[level][0] = 3;
        robot_drive_pattern[level][3] = round(map(pot_value,begin_knob,end_knob,0,255));
      }
      break;
    }
  case 6:
    {
      // loop A:
      boolean save_potvalue = false;
      if (pot_value == 0){
        // this means we should save the potmeter value of the equevilant of potmeter 6 
        save_potvalue = true;      
      }
      unsigned int function_blocks_in_loop_counter = 0; 
      for(int i  = level; i < rows; i++ ){
        int current_id_section = round(floor((id_array [i][0]/10)));
        if( current_id_section == 7 ){
          // we have problem: there is an loop block too soon
          // send error message back to master block with loop block that is in the wrong position
          // break all the code
        }
        if( current_id_section == 6 && function_blocks_in_loop_counter > 0 ){
          // succes we have found the next loop section and we have had blocks to repeat in between

        }
        else if(current_id_section == 6){
          // the loop blocks are next to eachother, nothing to loop maybe break here?
          // or send error command back to master block
          if(save_potvalue){
            // because we want only the first block to have the repeat value we switch them around here
            pot_value = pot_array [i][0];
            pot_array [i][0] = 0; 
          }
        }
        if(current_id_section < 6){
          // we have a block inside the loop
          function_blocks_in_loop_counter++;
        }
      }
      int repeat_x_times = round(map(pot_value,begin_knob,end_knob,0,9));
      robot_drive_pattern[level][0] = 6;
      robot_drive_pattern[level][1] = repeat_x_times;
      break;
    }
  case 7:
    { // loop A:
      boolean save_potvalue = false;
      if (pot_value == 0){
        // this means we should save the potmeter value of the equevilant of potmeter 6 
        save_potvalue = true;      
      }
      unsigned int function_blocks_in_loop_counter = 0; 
      for(int i  = level; i < rows; i++ ){
        int current_id_section = round(floor((id_array [i][0]/10)));
        if( current_id_section == 6 ){

          // we have problem: there is an loop block too soon
          // send error message back to master block with loop block that is in the wrong position
          String error_message = "R";
          error_message += (String) id_array [i][0];
          Serial2.print(error_message);
          // break all the code
          block_in_wrong_position = true; 
          break;
        }
        if( current_id_section == 7 && function_blocks_in_loop_counter > 0 ){
          // succes we have found the next loop section and we have had blocks to repeat in between

        }
        else if(current_id_section == 7){
          // the loop blocks are next to eachother, nothing to loop maybe break here?
          // or send error command back to master block
          if(save_potvalue){
            // because we want only the first block to have the repeat value we switch them around here
            pot_value = pot_array [i][0];
            pot_array [i][0] = 0; 
          }
        }
        if(current_id_section < 6){
          // we have a block inside the loop that we can loop
          function_blocks_in_loop_counter++;
        }
      }
      int repeat_x_times = round(map(pot_value,begin_knob,end_knob,0,9));
      robot_drive_pattern[level][0] = 7;
      robot_drive_pattern[level][1] = repeat_x_times;
      break;
    }
  default:
    break;
  }


}












































