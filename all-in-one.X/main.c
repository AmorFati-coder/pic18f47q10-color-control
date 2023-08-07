/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F47Q10
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include "TCS3471.h"
#include "RGB4x4Click.h"
#include <xc.h>



uint32_t combine_low_eight_bits(uint16_t val1, uint16_t val2, uint16_t val3) {
    uint32_t result = 0;

    result |= (uint32_t)(val1 & 0xFF) << 16;
    result |= (uint32_t)(val2 & 0xFF) << 8;
    result |= (uint32_t)(val3 & 0xFF);

    return result;
}

void __interrupt() INTERRUPT_InterruptManager (void)
{
    // interrupt handler
    if(PIE0bits.INT0IE == 1 && PIR0bits.INT0IF == 1)
    {
        if (TCS3471_RgbcValid())
        {
            TCS3471_ClearInterrupt();
            uint16_t clearVal = TCS3471_ReadCData();
            uint16_t redVal   = TCS3471_ReadRData();
            uint16_t greenVal = TCS3471_ReadGData();
            uint16_t blueVal  = TCS3471_ReadBData();
            printf("Light is %x\n", clearVal);
            printf("red is %x\n", redVal);
            printf("green is %x\n", greenVal);
            printf("blue is %x\n\n", blueVal);
            uint32_t re = combine_low_eight_bits(redVal, greenVal, blueVal);
            RGBLed_SetDiode(10, re , DiodeArray);

        }
        INT0_ISR();
    }
    else
    {
        //Unhandled Interrupt
    }
}



void FillScreen(uint32_t AColor) {
  uint8_t n;
  for (n=1; n<=16; n++) {
    RGBLed_SetDiode(n, AColor, DiodeArray);
  }
}


/*
                         Main application
 */
void main(void)
{
    
    //???
    SYSTEM_Initialize();
    //????
    INTERRUPT_GlobalInterruptEnable();
    //??????
    INTERRUPT_PeripheralInterruptEnable();
    _detected = false;
    //?????
    _i2cAddress = 0;
    
    if(TCS3471_Detect()){
        printf("TCS3471 Detect\n");
        //TCS3471???
        TCS3471_Init();
    }else{
        printf("TCS3471 does not exist\n");
    }
    FillScreen(0x0);
    
//    ANSELAbits.ANSELA2 = 0;
//    TRISAbits.TRISA2 = 0;
//    LATAbits.LATA2 = 1;
//    ANSELAbits.ANSELA5 = 0;
//    TRISAbits.TRISA5 = 0;
//    LATAbits.LATA5 = 1;
//    ANSELDbits.ANSELD7 = 0;
//    TRISDbits.TRISD7 = 0;
//    LATDbits.LATD7 = 1;
    

    
    while (1)
    {
        
        if(!_detected){
            for(int z = 0; z<=16; z++){
                RGBLed_SetDiode(z, 0x111111 , DiodeArray);
                __delay_ms(1000);
                RGBLed_SetDiode(z, 0x0 , DiodeArray);
                __delay_ms(1000); 
            }
            
        }
    }
}
/**
 End of File
*/