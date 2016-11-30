
// these constants describe the pins. They won't change:
const int xpin = A3;                  // x-axis of the accelerometer
const int ypin = A2;                  // y-axis
const int zpin = A1;                  // z-axis (only on 3-axis models)
const int curtinopenpin = 2;
const int curtinclosepin = 3;
const int blindactionpin = 4;
const int closepin = 5;
const int openpin = 6;
const int motorlockpin = 11;
int sensorPin = A0;
int x,y,z;
int xp,yp,zp;
int dx,dy,dz;
int knockfound; // this will have a 0 if nothing, and a 1 if we found a knock
int threshold = 20;
int sensorValue = 0;
int lastsensorValue = 0;

void setup()
{
  // initialize the serial communications:
  Serial.begin(9600);
  pinMode(curtinopenpin, OUTPUT);
  pinMode(curtinclosepin, OUTPUT);
  pinMode(blindactionpin, OUTPUT);
  pinMode(closepin, INPUT);
  pinMode(openpin, INPUT);
  pinMode(motorlockpin, OUTPUT);
  pinMode(sensorPin, INPUT);

  // run twice to burn initial knock detect, should prevent running at programming
  detectKnock();
  detectKnock();
}

void opencurtins (int count)
{
  digitalWrite(curtinopenpin, HIGH);
  int i = 0;
  while( i < count )
  {
    digitalWrite(motorlockpin, HIGH);
    digitalWrite(curtinclosepin, HIGH);
    delay(1);
    digitalWrite(curtinclosepin, LOW);
    delay(1);
    i++;
    digitalWrite(motorlockpin, LOW);
  }
}

void closecurtins (int count)
{ 
  digitalWrite(curtinclosepin, HIGH);
  int i = 0;
  while ( i < count )
  {
    digitalWrite(motorlockpin, HIGH);
    digitalWrite(curtinopenpin, HIGH);
    delay(1);
    digitalWrite(curtinopenpin, LOW);
    delay(1);
    i++;
    digitalWrite(motorlockpin, LOW);
  }
}
// int x,y,z;
/// int xp,yp,zp;
// int dx,dy,dz;

// int threshold = 10;
void detectKnock ()
{
  // pull fresh sensor values
  x = analogRead(xpin);
  y = analogRead(ypin); 
  z = analogRead(zpin);

  // calculate difference from previous sensor values
  dx = x - xp;
  dy = y - yp;
  dz = z - zp;
  // Note: at this point all calculations are correct

  if (sensorValue != lastsensorValue || abs(dx) > threshold || abs(dy) > threshold || abs(dz) > threshold ) 
  {
    Serial.print("detect  ");
    knockfound = 1;
  }
  else
  {
    knockfound = 0;
  }
  //   Serial.print(dx);
  //   Serial.print a tab between values;
  //   Serial.print("\t");
  //   Serial.print(dy);
  //   Serial.print a tab between values;
  //   Serial.print("\t");
  //   Serial.print(dz);
  //   Serial.println();
  Serial.print(sensorPin);
  // move what was fresh into the previous, (discarding what was previous)
  // Note: dont do math after these 3 lines
  xp = x;
  yp = y;
  zp = z;
}
//if (sensorValue != lastsensorValue)
//{
 //Serial.print("detect  ");
//knockfound = 1;
//}
//else
//{
 //knockfound = 0;
//}

void loop()
{
  sensorValue = analogRead(sensorPin); 

  float voltage= sensorValue * (3.3 / 1023.0);

  {

    // jump and run this code.  this function will set the variable knockfound;
    // "this is looking for the value"
    detectKnock();

    // "knockfound actually makes it do something"
    if (digitalRead (closepin) == LOW && knockfound)
    {
      while(digitalRead(openpin) == HIGH)
      {
        opencurtins(15); // was 5680
      }

      // at this poing we always know there will be another detection
      // burn value
      detectKnock();
      detectKnock();
      detectKnock();
      detectKnock();
    }
    // delay(200);
    if (digitalRead (openpin) == LOW && knockfound)
    {
      while(digitalRead(closepin) == HIGH)
      {
        closecurtins(15); // was 5680
      }


    }
    //while (closepin == HIGH);
    // at this poing we always know there will be another detection
    // burn value
    detectKnock();
    detectKnock();
    detectKnock();
    detectKnock();

    if (digitalRead(openpin) == HIGH && digitalRead(closepin) == HIGH && knockfound)
    {
      while(digitalRead(closepin) == HIGH)
      {
        closecurtins(15); // was 5680
      }
      detectKnock();
      detectKnock();

      //closecurtins(1000); // was 5690
      // delay before next reading:
      //delay(100);
    }
  }
}




