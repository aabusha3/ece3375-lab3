volatile int* LED_ptr = (int*)0xFF200000;  //lights
volatile int*  JP1_BASE = (int*) 0xFF200060 ; //gpio lights
volatile int*  switch_ptr = (int*) 0xFF200040; //switch pointer
volatile int* ADC_BASE = (int*) 0xFF204000; 
volatile int* ADC_ch1 = (int*) 0xFF204004;//where am I reading the 2 adcs from

void displayValue(int adcInput){
    const unsigned char ledTable[11] = {
        0x0, 0x1, 0x3, 0x7,
        0xF, 0x1F, 0x3F, 0x7F,
        0xFF, 0x1FF, 0x3FF
    };

    adcInput = adcInput *10 /4095;   
    if (adcInput < 10){
        *(JP1_BASE) = ledTable[adcInput];
    }else{
        *(JP1_BASE) = ledTable[10];
    }
}

int readSwitches(void) {
    volatile unsigned int switchState = *switch_ptr;
    switchState = switchState & 0x1; //bit mask with 1, determine if switch 1 is on
    return switchState;
}

int b15_status;
int adcData;
int delay;
int mask = 1<<15;   //mask to check status bit 0x8000
int main(){
    *(ADC_ch1) = 0x01;//autoupdate
    *(JP1_BASE+0x04) = 0x3FF;//set 0-9 led pins to output
    while(1){
        b15_status = *(ADC_BASE+0x08) & mask;
        if(b15_status == 1){
            adcData = *(ADC_BASE+0x08) & 0xFFF;
            displayValue(adcData);
        }

        for (delay = 100; delay > 0; delay--){}
    }

    return 1;
}