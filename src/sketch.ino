#include <pt.h>   // include protothread library

static struct pt pt1, pt2; // each protothread needs one of these
boolean ledstate1 = false;
boolean ledstate2 = false;

void toggleLED1() {
    ledstate1 = !ledstate1;   // toggle LED state using xor
    if(ledstate1)
        Serial.println("led1 switched on");
    else
        Serial.println("led1 switched off");
}

void toggleLED2() {
    ledstate2 = !ledstate2;   // toggle LED state using xor
    if(ledstate2)
        Serial.println("led2 switched on");
    else
        Serial.println("led2 switched off");
}

/* This function toggles the LED after 'interval' ms passed */
static int protothread1(struct pt *pt, int interval) {
    static unsigned long timestamp = 0;
    PT_BEGIN(pt);
    while(1) { // never stop 
        /* each time the function is called the second boolean
        *  argument "millis() - timestamp > interval" is re-evaluated
        *  and if false the function exits after that. */
        PT_WAIT_UNTIL(pt, millis() - timestamp > interval);
        timestamp = millis(); // take a new timestamp
        toggleLED1();
    }
    PT_END(pt);
}

/* exactly the same as the protothread1 function */
static int protothread2(struct pt *pt, int interval)
{
    static unsigned long timestamp = 0;
    PT_BEGIN(pt);
    while(1) {
        PT_WAIT_UNTIL(pt, millis() - timestamp > interval );
        timestamp = millis();
        toggleLED2();
    }
    PT_END(pt);
}

void setup()
{
    Serial.begin(9600); 
    PT_INIT(&pt1);  // initialise the two
    PT_INIT(&pt2);  // protothread variables
}

void loop()
{
    protothread1(&pt1, 500); // schedule the two protothreads
    protothread2(&pt2, 1000); // by calling them infinitely
}
