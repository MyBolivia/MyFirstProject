
/************************************************   RadioPingRole()  ( Ping Out Role ) ***************************/
#ifdef RADIO_IN_USE
void RadioPingRole() {      // Sendig data Role, expecting answer from Listener
  radio.stopListening();                                    // First, stop listening so we can talk.
#ifdef IN_LOOP_DEBUG
  Serial.println(F("Now sending"));
#endif
  unsigned long start_time = micros();                             // Take the time, and send it.  This will block until complete
  // *********    Sending data to Listener    ***************
  if (!radio.write( &start_time, sizeof(unsigned long) )) {
    Serial.println(F("failed"));
  }

  // *********    waiting the Feedback from Listener    ***************
  radio.startListening();                                    // Now, continue listening
  unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
  boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
  while ( ! radio.available() ) {                            // While nothing is received
    if (micros() - started_waiting_at > 200000 ) {           // If waited longer than 200ms, indicate timeout and exit while loop
      timeout = true;
      break;
    }
  }
#ifdef IN_LOOP_DEBUG
  if ( timeout ) {                                            // Describe the results
    Serial.println(F("Failed, response timed out."));
  } else {
    unsigned long got_time;                                 // Grab the response, compare, and send to debugging spew
    radio.read( &got_time, sizeof(unsigned long) );
    unsigned long end_time = micros();
    // Spew it
    Serial.print(F("Sent "));
    Serial.print(start_time);
    Serial.print(F(", Got response "));
    Serial.print(got_time);
    Serial.print(F(", Round-trip delay "));
    Serial.print(end_time - start_time);
    Serial.println(F(" microseconds"));
  }
#endif
  // Try again 1s later
  //delay(1000);
}
#endif


/****************************************************   RadioPingRole()    ( Pong Back Role ) ***************************/
#ifdef RADIO_IN_USE
void RadioPongRole() {            // Listening from data, and sending back the answer
  //  *******  receiving data from sender *******
  if ( radio.available()) {
    // Variable for the received timestamp
    while (radio.available()) {                                   // While there is data ready
      radio.read( &got_time, sizeof(int16_t) );             // Get the payload
      TextDisplay(String(got_time));                // Send the payload to OLED and LCD display
    }
    radio.stopListening();                                        // First, stop listening so we can talk
    //  *******  sending back data to sender *******
    radio.write( &got_time, sizeof(int16_t) );              // Send the final one back.
    radio.startListening();                                       // Now, resume listening so we catch the next packets.
#ifdef IN_LOOP_DEBUG
    Serial.print(F("Sent response "));
    Serial.println(got_time);
#endif
  }
}
#endif
