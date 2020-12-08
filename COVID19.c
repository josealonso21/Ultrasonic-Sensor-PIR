#pragma config PLLDIV = 1
#pragma config CPUDIV = OSC1_PLL2
#pragma config FOSC = XTPLL_XT
#pragma config PWRT = ON
#pragma config BOR = OFF
#pragma config WDT = OFF
#pragma config CCP2MX = ON
#pragma config PBADEN = OFF
#pragma config MCLRE = ON
#pragma config LVP = OFF

#include <XC.h>
#include "LCD.h"
#define _XTAL_FREQ 48000000UL

unsigned int distancia=0;
unsigned int distancia_1=0;
unsigned int distancia_2=0;
unsigned int distancia_3=0;
unsigned int total1=0;
unsigned int total2=0;
unsigned int total3=0;
unsigned int total4=0;

unsigned int centena = 0;
unsigned int decena= 0;
unsigned int unidad = 0;

void lcd_init(void) {
    TRISD = 0x00;
    LCD_CONFIG();
    __delay_ms(15);
    BORRAR_LCD();
    CURSOR_HOME();
    CURSOR_ONOFF(OFF);
}

void convierte(unsigned int numero){
    centena = (numero % 1000) /100;
    decena = (numero % 100) /10;
    unidad = numero % 10;
}

void Config(void){
    TRISB = 0xAA;
    TRISCbits.RC0 = 0; 
    TRISCbits.RC1 = 1;
    TRISCbits.RC2 = 1; 
    
    LATCbits.LC0 = 0; 
    LATBbits.LB0 = 0; 
    LATBbits.LB2 = 0;
    LATBbits.LB4 = 0; 
    LATBbits.LB6 = 0;
}

unsigned int distancia1(void){
    distancia = 0; 
    LATBbits.LB0 = 1; 
    __delay_us(10);
    LATBbits.LB0 = 0;
    //Se activa el sensor
    while(PORTBbits.RB1==0);
    while(PORTBbits.RB1==1){
        T0CON = 0xC4;
        TMR0L=234;
        while(INTCONbits.TMR0IF == 0);
        distancia = distancia +1;
        INTCONbits.TMR0IF = 0; 
    }
    return(distancia);
}

unsigned int distancia2(void){
    distancia_1 = 0; 
    LATBbits.LB2 = 1; 
    __delay_us(10);
    LATBbits.LB2 = 0;
    //Se activa el sensor
    while(PORTBbits.RB3==0);
    while(PORTBbits.RB3==1){
        T0CON = 0xC4;
        TMR0L=234;
        while(INTCONbits.TMR0IF == 0);
        distancia_1 = distancia_1 +1;
        INTCONbits.TMR0IF = 0; 
    }
    return(distancia_1);
}

unsigned int distancia3(void){
    distancia_2 = 0; 
    LATBbits.LB4 = 1; 
    __delay_us(10);
    LATBbits.LB4 = 0;
    //Se activa el sensor
    while(PORTBbits.RB5==0);
    while(PORTBbits.RB5==1){
        T0CON = 0xC4;
        TMR0L=234;
        while(INTCONbits.TMR0IF == 0);
        distancia_2 = distancia_2 +1;
        INTCONbits.TMR0IF = 0; 
    }
    return(distancia_2);
}

unsigned int distancia4(void){
    distancia_3 = 0; 
    LATBbits.LB6 = 1; 
    __delay_us(10);
    LATBbits.LB6 = 0;
    //Se activa el sensor
    while(PORTBbits.RB7==0);
    while(PORTBbits.RB7==1){
        T0CON = 0xC4;
        TMR0L=234;
        while(INTCONbits.TMR0IF == 0);
        distancia_3 = distancia_3 +1;
        INTCONbits.TMR0IF = 0; 
    }
    return(distancia_3);
}

void ESCRIBE(void){
    ENVIA_CHAR(centena+0x30);
    ENVIA_CHAR(decena+0x30);
    ENVIA_CHAR(unidad+0x30);
    __delay_us(10);
}

void CONDICION (void){
    if(PORTCbits.RC1 == 1 || PORTCbits.RC2 ==1){
        LATCbits.LC0 = 1;
    }
    else{
        LATCbits.LC0 = 0;
    }
}

void main(void){
    lcd_init();
    Config();
    while(1){
        total1 = distancia1();    
        total2 = distancia2();
        total3 = distancia3();    
        total4 = distancia4();
        if(total1 <=150){
            CONDICION();
        }
        else if (total2 <=150){
            CONDICION();
        }
        else if (total3 <=150){
            CONDICION();
        }
        else if (total4 <=150){
            CONDICION();
        }
        else{
            LATCbits.LC0 = 0; 
        }
        
        convierte(total1);
        POS_CURSOR(1,0);
        ESCRIBE_MENSAJE("D1: ", 4);
        ESCRIBE();
        
        convierte(total2);
        POS_CURSOR(1,11);
        ESCRIBE_MENSAJE("D2: ", 4);
        ESCRIBE();
        
        convierte(total3);
        POS_CURSOR(2,0);
        ESCRIBE_MENSAJE("D3: ", 4);
        ESCRIBE();
        
        convierte(total4);
        POS_CURSOR(2,11);
        ESCRIBE_MENSAJE("D4: ", 4);
        ESCRIBE();
    }
}