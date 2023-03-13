#include "address_map_arm.h"    // Define hardware
volatile int * LED_ptr = (int *)JP1_BASE ;          // LED address
volatile int * const ADC_ptr = (int *)ADC_BASE;     // ADC address
volatile int * SW_ptr = (int *)SW_BASE;             //switch address

// Function to display LED
void displayLED(int i) {
    // Lookup Table that controls over the LEDs
    volatile int lookUpTable[10];
    // There are 11 LEDs and each of them represents one bit
	lookUpTable[0] = 0x0000; // 0b0001 
	lookUpTable[1] = 0x0001; // 0b0011
	lookUpTable[2] = 0x0003; // 0b0111
	lookUpTable[3] = 0x0007; // 0b1111
	lookUpTable[4] = 0x000F; // 0b0001 1111
	lookUpTable[5] = 0x001F; // 0b0011 1111
	lookUpTable[6] = 0x003F; // 0b0111 1111
	lookUpTable[7] = 0x007F; // 0b1111 1111
	lookUpTable[8] = 0x0FF; // 0b0001 1111 1111
	lookUpTable[9] = 0x01FF; // 0b0011 1111 1111
	lookUpTable[10] = 0x03FF; // 0b0011 1111 1111

    // If i < 10, then search for the value inside table
    if (i < 10) {
       *(LED_ptr) = lookUpTable[i];
    }
    // If i > 10, then reset LED back to 0
    else {
       *(LED_ptr) = lookUpTable[10];
    }
}

// Read switch value 
int readSwitch(void){
    // Store the value stored in the switch
    volatile int switchState = *SW_ptr; 
	// Check switch state and clear 
    switchState = switchState & 1;
    return switchState;
}

int main(void) {
    // Set delay length to 100hz
    volatile int DELAY_LENGTH;
    DELAY_LENGTH = 100;                      
    while(1) {
        // Detect which switch is using
        // If state equals 1, read from first potentiometer, update all channels
        if (readSwitch() == 1)                        
        {   
            // Write 1 to channel 0
            ADC_ptr[0] = 0x0001;
            volatile int value = ADC_ptr[0];
            // Remember that the ADC data is only 12 bits
            // And bit 15 for each channel is set when the channel has completed a measurement.
            // Set all 10 LEDs to 1
            *(LED_ptr+1) = 0b1111111111;               
            value = value & 0xFFF;
            // Display on LED pins, 2^12 / 10 = 4096 / 10 = 409, one block is 409 bytes                       
            displayLED(value/409);                   
        }
        // If state is 0, read from the second potentiometer 
        else if(readSwitch() == 0)                   
        {
            // Refresh channel, set to start
            // Write 1 to channel 1
            ADC_ptr[4] = 0x0001;                     
            volatile int value = ADC_ptr[1];
            // Check if its 15 bits
            int bit_mask = 1 << 15;  
            // If so set value to 15                
            value = value & 0xFFF;                          
            displayLED(value/409);                  
        }
        // Set delay such that LED does not flash too quick
        int delay;                         
        for(delay = DELAY_LENGTH; delay != 0; --delay)
        {
        };
    }
}