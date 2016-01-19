// -----------------------------------
// Controlling Blinds over the Internet
// -----------------------------------

// First, let's create our "shorthand" for the pins

int up = D0;
int stop = D1;
int down = D2;
int left = D3;
int right = D4;

// Universal Delay for how long we want to press the desired button

int BUTTON_DELAY = 500;

// Current position of the blinds init to 0

int pos = 0;

// Desired position of the blinds init to 0

int newPos = 0;


bool isRunning = false;

// Current direction of the blinds init to 0

int direction = 0;

// New direction of the blinds init to 0

int newDirection =0;


// Log message to cloud, message is a printf-formatted string
void debug(String message, int value) {
    char msg [50];
    sprintf(msg, message.c_str(), value);
    Spark.publish("DEBUG", msg);
}





// Register our Spark function and set pins to off to start

void setup()
{

   // Here's the pin configuration
   pinMode(up, OUTPUT);
   pinMode(down, OUTPUT);
   pinMode(stop, OUTPUT);
   pinMode(left, OUTPUT);
   pinMode(right, OUTPUT);

   // We are also going to declare a Spark.function so that we can controll the blinds from the cloud.
   Spark.function("blinds",blinds);

   digitalWrite(D0, LOW);
   digitalWrite(D1, LOW);
   digitalWrite(D2, LOW);
   digitalWrite(D3, LOW);
   digitalWrite(D4, LOW);
}


//Controller logic for the blinds
//Checks to see if a new position is desired and moves the blind if needed.
//Has ability to change direction if position changes while the blinds are moving

void loop()
{
  //Determine if we need to move the blinds by pressing a button on the remote
   if (pos != newPos){
       
       if ((pos-newPos) < 0){
           newDirection = 1;
           if (!isRunning || direction != newDirection){
              digitalWrite(up,HIGH);
              delay(BUTTON_DELAY);
              digitalWrite(up,LOW);
              isRunning = true;
              direction = 1;
           }
       }
       else{
           newDirection = -1;
          if (!isRunning || direction != newDirection){
              digitalWrite(down,HIGH);
              delay(BUTTON_DELAY);
              digitalWrite(down,LOW);
              isRunning = true;
              direction = -1;
          }
       }
       //Hack delay because the position are based on seconds the blinds move
       delay(1000);
       pos += direction;
   }else{
   //Determine if we need to stop
    if (isRunning){
        if (direction == -1 && pos == 0){
            delay(1000);
        }
        
        if (direction == 1 && pos >= 60){
            delay(1000);
        }
        
        digitalWrite(stop,HIGH);
        delay(BUTTON_DELAY);
        digitalWrite(stop,LOW);
        isRunning = false;
    }
   }
   debug("Position %d: ", pos);
}

// We're going to have a super cool function now that gets called when a matching API request is sent
// This is the blinds function we registered to the "blinds" Spark.function earlier.
// used to set new desired position for blinds


int blinds(String i) {
    //TODO change input to percent instead of position so it can be controlled through Echo
    int temp = atoi(i.c_str());
    if (temp > 60){
        newPos = 60;
    }else{
        newPos = temp;
    }
    
    debug("New Position %d: ", newPos);
    return 1;
}

