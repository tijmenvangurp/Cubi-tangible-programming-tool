void request_timout_change_search_direction(){
  /*
       When request is not handled by block because there was no neighbor within a given time go and search into the next direction
   when the search direction was for a new line, and there are no more blocks down than construct the message for the robot
   */

  static int time_to_wait = 9000;// if there is no repons in 5000 microseconds search in next direction
  if (waiting_for_id == true && (micros() - waiting_for_id_timeout) > time_to_wait) {
    waiting_for_id = false;
    switch(slave_search_direction) {
    case right:
      // if we where searching to the right, we go and check if there is a block left
      slave_search_direction = left;
      received_id = id_array[row_counter][0];// Send request to direction needs to check on the current row, but the first block so first collum
      send_request_to_direction(); 
      // Serial.println(F("No more neighbors, search to left"));
      break;
    case left:
      // we now have looked left and right on this current row, so we must go to the new row/line
      slave_search_direction = new_line;
      received_id = id_array[row_counter][0];// Send request to direction needs to check on the current row, but the first block so first collum
      send_request_to_direction();
      //Serial.println(F("No more neighbors, search down"));
      break;
    case new_line:
      //ready to construct message to robot
      construct_message();
    }
  }

}

