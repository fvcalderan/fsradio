#include <LiquidCrystal.h>
#include <math.h>
#include <stdint.h>

/* LCD Display */
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

/* Keypad */
uint16_t last_btn = 1023;
uint64_t last_millis;

/* Serial */
String last_serial_data = "";

/* Mode */
uint8_t mode = 0;
const uint8_t MAX_MODE = 3;
/*
 * 0: COM1 MHz | 1: COM1 KHz
 * 2: COM2 MHz | 3: COM2 KHz
 */

/* clear the LCD screen */
void lcd_clear()
{
    uint8_t i;
    lcd.setCursor(0,0);
    for (i = 0; i < 16; ++i) lcd.write(' ');
    lcd.setCursor(0,1);
    for (i = 0; i < 16; ++i) lcd.write(' ');
}

/* blink LCD at position */
void lcd_blink(int x, int y)
{
    lcd.setCursor(x, y);
    lcd.blink();
}

/* blink screen on different spots depending on the mode */
void blink_mode()
{
    if      (mode == 0) lcd_blink(0, 1);
    else if (mode == 1) lcd_blink(5, 1);
    else if (mode == 2) lcd_blink(0, 1);
    else if (mode == 3) lcd_blink(5, 1);
}

/* buttons*/
void btn_up()
{
    if      (mode == 0) Serial.println("COM1_MUP");
    else if (mode == 1) Serial.println("COM1_KUP");
    else if (mode == 2) Serial.println("COM2_MUP");
    else if (mode == 3) Serial.println("COM2_KUP");
}

void btn_down()
{
    if      (mode == 0) Serial.println("COM1_MDN");
    else if (mode == 1) Serial.println("COM1_KDN");
    else if (mode == 2) Serial.println("COM2_MDN");
    else if (mode == 3) Serial.println("COM2_KDN");
}

void btn_right()
{
    Serial.println("MODE_UP");
    mode = min(MAX_MODE, mode + 1);
}

void btn_left()
{
    Serial.println("MODE_DN");
    mode = max(0, mode - 1);
}

void btn_select()
{
    if      (mode < 2) Serial.println("COM1_SWP");
    else if (mode < 4) Serial.println("COM2_SWP");
}

/* read buttons */
void read_buttons()
{
    uint16_t btn = analogRead(0);
    uint64_t current_millis = millis();

    /* check for button click (with error margin) */
    if (abs(btn - last_btn) > 50) {
        /* avoid debounce */
        if (current_millis - last_millis > 200) {
            if (btn < 90) {
                btn_right();
            } else if (btn < 200) {
                btn_up();
            } else if (btn < 400) {
                btn_down();
            } else if (btn < 600) {
                btn_left();
            } else if (btn < 800) {
                btn_select();
            }
            last_btn = btn;
            last_millis = current_millis;
        }
    }
}

/* read from serial */
void read_serial()
{
    if (Serial.available() > 0) {
        /* read the incoming text */
        String serial_data = Serial.readStringUntil('\n');

        /* if something new was received, display it */
        if (serial_data != last_serial_data) {
            if (mode < 2) {
                lcd_clear();
                lcd.setCursor(0, 0);
                lcd.print("COM1: ");
                lcd.print(serial_data);
                lcd.setCursor(0, 1);
                lcd.print(" MHz  KHz");
                last_serial_data = serial_data;
            } else if (mode < 4) {
                lcd_clear();
                lcd.setCursor(0, 0);
                lcd.print("COM2: ");
                lcd.print(serial_data);
                lcd.setCursor(0, 1);
                lcd.print(" MHz  KHz");
                last_serial_data = serial_data;
            }
        }
    }
}


/* setup function */
void setup()
{
    /* Write stuff to the LCD screen */
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("FVC FS2004 Radio");
    lcd.setCursor(0, 1);
    lcd.print(" V1.0 [13/11/22]");
    lcd_blink(0, 1);

    /* Setup Serial port with USB baud rate */
    Serial.begin(115200);
}

/* loop function */
void loop()
{
    read_buttons();
    read_serial();
    blink_mode();
}
