// I2C device class (I2Cdev) demonstration Processing sketch for MPU6050 DMP output
// 6/20/2012 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     2012-06-20 - initial release

/* ============================================
 I2Cdev device library code is placed under the MIT license
 Copyright (c) 2012 Jeff Rowberg
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ===============================================
 */

import processing.serial.*;
import processing.opengl.*;
import toxi.geom.*;
import toxi.processing.*;

// NOTE: requires ToxicLibs to be installed in order to run properly.
// 1. Download from http://toxiclibs.org/downloads
// 2. Extract into [userdir]/Processing/libraries
//    (location may be different on Mac/Linux)
// 3. Run and bask in awesomeness

ToxiclibsSupport gfx;

Serial port;                         // The serial port
char[] teapotPacket = new char[14];  // InvenSense Teapot packet
char[] compassPacket = new char[14];  // InvenSense Teapot packet

int serialCount = 0;                 // current packet byte position
int synced = 0;
int interval = 0;

int compassSerialCount = 0;                 // current packet byte position
int compasInterval = 0;
float[] cq = new float[4];

PFont b;
int lf = 10;    // Linefeed in ASCII
String myString = null;
float myAngle = 0;
float myCompAngle = 0;

float[] q = new float[4];
Quaternion quat = new Quaternion(1, 0, 0, 0);

float[] gravity = new float[3];
float[] euler = new float[3];
float[] ypr = new float[3];

/********************************************************************************************************************/

void setup() {
  // 300px square viewport using OpenGL rendering
  size(900, 900, P3D);
  gfx = new ToxiclibsSupport(this);
  b = createFont("Arial-BoldMT-48.vlw", 48);
  // setup lights and antialiasing
  lights();
  smooth();

  // display serial port list for debugging/clarity
  println(Serial.list());

  // get the first available port (use EITHER this OR the specific port code below)
  //String portName = Serial.list()[0];

  // get a specific serial port (use EITHER this OR the first-available code above)
  String portName = "COM57";

  // open the serial port
  //port = new Serial(this, portName, 115200);
  port = new Serial(this, portName, 74880);

  // send single character to trigger DMP init/start
  // (expected by Arduino sketch)
  port.write('r');
}

void draw() {
  
  if (millis() - interval > 2000) {
    // resend single character to trigger DMP init/start
    // in case the MPU is halted/reset while applet is running
    port.write('r');
    interval = millis();
    println (" **** Sending 'r' **** ");
  }

  // black background
  background(0);
  //background(255);
  gymbalDraw (0,100);
  compassDraw (myAngle,-200,0,"Raw Hdg : ");
  compassDraw (myCompAngle,200,0,"Comp. Hdg : ");
}

/********************************************************************************************************************/

void gymbalDraw (int OffsetX, int OffsetY) {
  // translate everything to the middle of the viewport
  pushMatrix();
  translate(width / 2, height / 2 );
  strokeWeight(1);  // Default
  stroke(0) ;
  // 3-step rotation from yaw/pitch/roll angles (gimbal lock!)
  // ...and other weirdness I haven't figured out yet
  //rotateY(-ypr[0]);
  //rotateZ(-ypr[1]);
  //rotateX(-ypr[2]);

  // toxiclibs direct angle/axis rotation from quaternion (NO gimbal lock!)
  // (axis order [1, 3, 2] and inversion [-1, +1, +1] is a consequence of
  // different coordinate system orientation assumptions between Processing
  // and InvenSense DMP)
  float[] axis = quat.toAxisAngle();
  //rotate(axis[0], -axis[1], axis[3], axis[2]);
rotate(axis[0], -axis[1], axis[3], axis[2]);
  // draw main body in red
  fill(255, 0, 0, 200);
  box(10, 10, 200);

  // draw front-facing tip in blue
  fill(0, 0, 255, 200);
  pushMatrix();
  translate(0, 0, -120);
  rotateX(PI/2);
  drawCylinder(0, 20, 20, 8);
  popMatrix();

  // draw wings and tail fin in green
  fill(0, 255, 0, 200);
  beginShape(TRIANGLES);
  vertex(-100, 2, 30); 
  vertex(0, 2, -80); 
  vertex(100, 2, 30);  // wing top layer
  vertex(-100, -2, 30); 
  vertex(0, -2, -80); 
  vertex(100, -2, 30);  // wing bottom layer
  vertex(-2, 0, 98); 
  vertex(-2, -30, 98); 
  vertex(-2, 0, 70);  // tail left layer
  vertex( 2, 0, 98); 
  vertex( 2, -30, 98); 
  vertex( 2, 0, 70);  // tail right layer
  endShape();
  beginShape(QUADS);
  vertex(-100, 2, 30); 
  vertex(-100, -2, 30); 
  vertex(  0, -2, -80); 
  vertex(  0, 2, -80);
  vertex( 100, 2, 30); 
  vertex( 100, -2, 30); 
  vertex(  0, -2, -80); 
  vertex(  0, 2, -80);
  vertex(-100, 2, 30); 
  vertex(-100, -2, 30); 
  vertex(100, -2, 30); 
  vertex(100, 2, 30);
  vertex(-2, 0, 98); 
  vertex(2, 0, 98); 
  vertex(2, -30, 98); 
  vertex(-2, -30, 98);
  vertex(-2, 0, 98); 
  vertex(2, 0, 98); 
  vertex(2, 0, 70); 
  vertex(-2, 0, 70);
  vertex(-2, -30, 98); 
  vertex(2, -30, 98); 
  vertex(2, 0, 70); 
  vertex(-2, 0, 70);
  endShape();

  popMatrix();
}

/********************************************************************************************************************/



void compassDraw (float angle , int OffsetX , int OffsetY,String Title) {
//println(angle);

  pushMatrix();
  translate(width / 2 + OffsetX, 50 + OffsetY);
  //textFont(b,40);
  textSize(32);
  textAlign(CENTER);
  text(Title, -100, 20);
  text((angle), 60, 20);
  //println(angle);
  popMatrix();
  /*
  float delta = abs(angle - prevAngle);
   if (delta > 30) {
     print(angle);
     print("  ");
     print(prevAngle);
          print("  ");
     println(delta);
 //delay (2000);
 }
 prevAngle = angle;
*/
  
  pushMatrix();
  translate(width / 2 - 150 + OffsetX, height / 4 - 150 + OffsetY);

  // draw the compass background - external
  ellipseMode(CENTER);
  fill(50);
  stroke(10);
  strokeWeight(2);
  ellipse(150, 150, 300, 300);

  // draw the lines and dots
  translate(150, 150);  // translate the lines and dots to the middle of the compass
  float CompassX = -angle;
  rotate(radians(CompassX));
  noStroke();
  fill(51, 255, 51);

  int radius = 120;

  for ( int degC = 5; degC < 360; degC += 10) //Compass dots
  {
    float angleC = radians(degC);
    float xC = 0 + (cos(angleC)* radius);
    float yC = 0 + (sin(angleC)* radius);
    ellipse(xC, yC, 3, 3);
  }

  for ( int degL = 10; degL < 370; degL += 10) //Compass lines
  {
    float angleL = radians(degL);
    float x = 0 + (cos(angleL)* 145);
    float y = 0 + (sin(angleL)* 145);

    if ( degL==90 || degL==180 || degL==270 || degL==360) {
      stroke(51, 255, 51);
      strokeWeight(4);
    } else {
      stroke(234, 144, 7);
      strokeWeight(2);
    }
    line(0, 0, x, y);
  }

  fill(102, 102, 102);
  noStroke();
  translate(0, 0, 1);
  ellipseMode(CENTER);
  ellipse(0, 0, 228, 228); //draw a filled circle to hide the lines in the middle

  b = loadFont("Arial-BoldMT-48.vlw");
  textAlign(CENTER);

  // Draw the letters
  fill(250);
  //textFont(b);
  textSize(32);
  text("N", 1, -90);
  rotate(radians(90));
  text("E", 0, -90);
  rotate(radians(90));
  text("S", 0, -90);
  rotate(radians(90));
  text("W", 0, -90);
  rotate(radians(90));

  //draw the needle
  translate(0, 0, 1);
  rotate(radians(-CompassX)); //make it stationary
  stroke(234, 144, 7);
  strokeWeight(3);

  triangle(-10, 0, 10, 0, 0, -85);
  fill(234, 144, 7);
  triangle(-10, 0, 10, 0, 0, 60);
  popMatrix();
}

/********************************************************************************************************************/

void serialEvent(Serial port) {
  interval = millis();
  while (port.available() > 0) {
    int ch = port.read();
    if (synced == 0 && ((ch != '$') &&  (ch != '£'))) 
    //if (synced == 0 && ch != '$')
    {
      //println("initial synchronization for MPU");
      //println("*** RESINC ***");
      return;   // initial synchronization - also used to resync/realign if needed
    }
   
    synced = 1;
    // print ((char)ch);   // print every char from Serial 


    if ((serialCount == 1 && ch != 2)
      || (serialCount == 12 && ch != '\r')
      || (serialCount == 13 && ch != '\n')) {
      serialCount = 0;
      synced = 0;
      //println(" ** END **");
      return;
    }

    if (serialCount > 0 || ch == '$') {
      teapotPacket[serialCount++] = (char)ch;
      //print ("|");
      if (serialCount == 14) {
        serialCount = 0; // restart packet byte position

        // get quaternion from data packet
        q[0] = ((teapotPacket[2] << 8) | teapotPacket[3]) / 16384.0f;
        q[1] = ((teapotPacket[4] << 8) | teapotPacket[5]) / 16384.0f;
        q[2] = ((teapotPacket[6] << 8) | teapotPacket[7]) / 16384.0f;
        q[3] = ((teapotPacket[8] << 8) | teapotPacket[9]) / 16384.0f;
        for (int i = 0; i < 4; i++) if (q[i] >= 2) q[i] = -4 + q[i];

        // set our toxilibs quaternion to new data
        quat.set(q[0], q[1], q[2], q[3]);

        /*
                // below calculations unnecessary for orientation only using toxilibs
         
         // calculate gravity vector
         gravity[0] = 2 * (q[1]*q[3] - q[0]*q[2]);
         gravity[1] = 2 * (q[0]*q[1] + q[2]*q[3]);
         gravity[2] = q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3];
         
         // calculate Euler angles
         euler[0] = atan2(2*q[1]*q[2] - 2*q[0]*q[3], 2*q[0]*q[0] + 2*q[1]*q[1] - 1);
         euler[1] = -asin(2*q[1]*q[3] + 2*q[0]*q[2]);
         euler[2] = atan2(2*q[2]*q[3] - 2*q[0]*q[1], 2*q[0]*q[0] + 2*q[3]*q[3] - 1);
         
         // calculate yaw/pitch/roll angles
         ypr[0] = atan2(2*q[1]*q[2] - 2*q[0]*q[3], 2*q[0]*q[0] + 2*q[1]*q[1] - 1);
         ypr[1] = atan(gravity[0] / sqrt(gravity[1]*gravity[1] + gravity[2]*gravity[2]));
         ypr[2] = atan(gravity[1] / sqrt(gravity[0]*gravity[0] + gravity[2]*gravity[2]));
         
         */
        // output various components for debugging
        //println("q:\t" + round(q[0]*100.0f)/100.0f + "\t" + round(q[1]*100.0f)/100.0f + "\t" + round(q[2]*100.0f)/100.0f + "\t" + round(q[3]*100.0f)/100.0f);
        //println("euler:\t" + euler[0]*180.0f/PI + "\t" + euler[1]*180.0f/PI + "\t" + euler[2]*180.0f/PI);
        //println("ypr:\t" + ypr[0]*180.0f/PI + "\t" + ypr[1]*180.0f/PI + "\t" + ypr[2]*180.0f/PI);
      }
    } else if (compassSerialCount > 0 || ch == '£') 
    //} else if ((compassSerialCount > 0) ^ (ch == '£')) 
    {

      compassPacket[compassSerialCount++] = (char)ch;
      //print ("|");
      if (compassSerialCount == 14) {
        compassSerialCount = 0; // restart packet byte position

        byte[] MycompassPacket = new byte[0];
        MycompassPacket = byte(compassPacket);
        if (MycompassPacket[10] != 0) return;
     
        myAngle = get4bytesFloat(MycompassPacket, 2);
        myCompAngle = get4bytesFloat(MycompassPacket, 6);
        /*
        for (int index = 0; index < 14 ; index++) {
        print (MycompassPacket[index]); print("\t");
        }
        print("==>") ; print(myAngle); print("  ");println(myCompAngle);
        */
    }
    }
    else {
      print ((char)ch);
    }
  }
}

/********************************************************************************************************************/

float get4bytesFloat(byte[] data, int offset) {
  String hexint=hex(data[offset+3])+hex(data[offset+2])+hex(data[offset+1])+hex(data[offset]);
  return Float.intBitsToFloat(unhex(hexint));
} 

/********************************************************************************************************************/

void drawCylinder(float topRadius, float bottomRadius, float tall, int sides) {
  float angle = 0;
  float angleIncrement = TWO_PI / sides;
  beginShape(QUAD_STRIP);
  for (int i = 0; i < sides + 1; ++i) {
    vertex(topRadius*cos(angle), 0, topRadius*sin(angle));
    vertex(bottomRadius*cos(angle), tall, bottomRadius*sin(angle));
    angle += angleIncrement;
  }
  endShape();

  // If it is not a cone, draw the circular top cap
  if (topRadius != 0) {
    angle = 0;
    beginShape(TRIANGLE_FAN);

    // Center point
    vertex(0, 0, 0);
    for (int i = 0; i < sides + 1; i++) {
      vertex(topRadius * cos(angle), 0, topRadius * sin(angle));
      angle += angleIncrement;
    }
    endShape();
  }

  // If it is not a cone, draw the circular bottom cap
  if (bottomRadius != 0) {
    angle = 0;
    beginShape(TRIANGLE_FAN);

    // Center point
    vertex(0, tall, 0);
    for (int i = 0; i < sides + 1; i++) {
      vertex(bottomRadius * cos(angle), tall, bottomRadius * sin(angle));
      angle += angleIncrement;
    }
    endShape();
  }
}