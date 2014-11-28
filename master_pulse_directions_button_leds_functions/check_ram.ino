int freeRam (String place) {
  extern int __heap_start, *__brkval; 
  int v; 
  Serial.print(place);
  Serial.println( (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval)); 
}
