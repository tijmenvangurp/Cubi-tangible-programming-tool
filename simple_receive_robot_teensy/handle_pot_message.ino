void handle_pot_message (){

  int received_byte = receive_bytes();

  switch (received_byte) {
  case 2000:
    Serial.println("new Pot message");
    memset(pot_array, 0, sizeof(pot_array[0][0]) * rows * collums); // clear out id array so we can fill it again
    collum_counter = 0;
    row_counter = 0;

    break;
  case 4000:
  //  Serial.println("new pot line");
    row_counter++;
    break;
  case 3000:
  //  Serial.println("new pot collum");
    collum_counter++;
    break;
  case 5000:
    Serial.println("End pot message");
   // Serial2.print('P');
    waiting_for_pot_values = false;
    start_constructing_patern = true;
    break;
  default:
    Serial.println(received_byte);
    pot_array [row_counter][collum_counter] = received_byte;
    break;
  }


}

int receive_bytes (){
  int byte_to_read;
  byte_to_read = Serial2.read() << 8 ; // read the least significant byte
  byte_to_read |= Serial2.read(); // add the most significant byte
  return byte_to_read;
}
