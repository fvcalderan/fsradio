#include <LiquidCrystal.h>
#include <math.h>
#include <stdint.h>

/* LCD Display */
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

/* keypad */
uint16_t last_btn = 1023;
uint64_t last_millis;

/* serial */
String last_serial_data = "";

/* mode */
uint8_t mode = 0;
const uint8_t MAX_MODE = 16;
/*
 *  0: COM1  MHz |  1: COM1  KHz |  2: COM2  MHz |  3: COM2  KHz
 *  4: NAV1  MHz |  5: NAV1  KHz |  6: NAV2  MHz |  7: NAV2  KHz
 *  8: ADF   100 |  9: ADF   010 | 10: ADF   001 | 11: ADF   DEC
 * 12: DME  SWAP | 13: XPDR 1000 | 14: XPDR 0100 | 14: XPDR 0010
 * 16: XPDR 0001 |
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
    /* COM 1 */
    if      (mode == 0) lcd_blink(0, 1);
    else if (mode == 1) lcd_blink(5, 1);

    /* COM 2 */
    else if (mode == 2) lcd_blink(0, 1);
    else if (mode == 3) lcd_blink(5, 1);

    /* NAV 1 */
    if      (mode == 4) lcd_blink(0, 1);
    else if (mode == 5) lcd_blink(5, 1);

    /* NAV 2 */
    else if (mode == 6) lcd_blink(0, 1);
    else if (mode == 7) lcd_blink(5, 1);

    /* ADF */
    else if (mode ==  8) lcd_blink( 6, 1);
    else if (mode ==  9) lcd_blink( 7, 1);
    else if (mode == 10) lcd_blink( 8, 1);
    else if (mode == 11) lcd_blink(10, 1);

    /* DME */
    else if (mode == 12) lcd_blink(5, 0);

    /* XPDR */
    else if (mode == 13) lcd_blink(6, 1);
    else if (mode == 14) lcd_blink(7, 1);
    else if (mode == 15) lcd_blink(8, 1);
    else if (mode == 16) lcd_blink(9, 1);

}

/* buttons*/
void btn_up()
{
    /* COM 1 */
    if      (mode == 0) Serial.println("COM1_MUP");
    else if (mode == 1) Serial.println("COM1_KUP");

    /* COM 2 */
    else if (mode == 2) Serial.println("COM2_MUP");
    else if (mode == 3) Serial.println("COM2_KUP");

    /* NAV 1 */
    else if (mode == 4) Serial.println("NAV1_MUP");
    else if (mode == 5) Serial.println("NAV1_KUP");

    /* NAV 2 */
    else if (mode == 6) Serial.println("NAV2_MUP");
    else if (mode == 7) Serial.println("NAV2_KUP");

    /* ADF */
    else if (mode ==  8) Serial.println("ADF_100UP");
    else if (mode ==  9) Serial.println("ADF_010UP");
    else if (mode == 10) Serial.println("ADF_001UP");
    else if (mode == 11) Serial.println("ADF_DECUP");

    /* DME */
    else if (mode == 12) /* DME selected -> pass */;

    /* XPDR */
    else if (mode == 13) Serial.println("XPDR_1000UP");
    else if (mode == 14) Serial.println("XPDR_0100UP");
    else if (mode == 15) Serial.println("XPDR_0010UP");
    else if (mode == 16) Serial.println("XPDR_0001UP");
}

void btn_down()
{
    /* COM 1 */
    if      (mode == 0) Serial.println("COM1_MDN");
    else if (mode == 1) Serial.println("COM1_KDN");

    /* COM 2 */
    else if (mode == 2) Serial.println("COM2_MDN");
    else if (mode == 3) Serial.println("COM2_KDN");

    /* NAV 1 */
    if      (mode == 4) Serial.println("NAV1_MDN");
    else if (mode == 5) Serial.println("NAV1_KDN");

    /* NAV 2 */
    if      (mode == 6) Serial.println("NAV2_MDN");
    else if (mode == 7) Serial.println("NAV2_KDN");

    /* ADF */
    else if (mode ==  8) Serial.println("ADF_100DN");
    else if (mode ==  9) Serial.println("ADF_010DN");
    else if (mode == 10) Serial.println("ADF_001DN");
    else if (mode == 11) Serial.println("ADF_DECDN");

    /* DME */
    else if (mode == 12) /* DME selected -> pass */;

    /* XPDR */
    else if (mode == 13) Serial.println("XPDR_1000DN");
    else if (mode == 14) Serial.println("XPDR_0100DN");
    else if (mode == 15) Serial.println("XPDR_0010DN");
    else if (mode == 16) Serial.println("XPDR_0001DN");
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
    if      (mode <  2) Serial.println("COM1_SWP");
    else if (mode <  4) Serial.println("COM2_SWP");
    else if (mode <  6) Serial.println("NAV1_SWP");
    else if (mode <  8) Serial.println("NAV2_SWP");
    else if (mode < 12) /* ADF selected -> pass */;
    else if (mode < 13) Serial.println("DME_TGL");
    else if (mode < 17) /* XPDR selected -> pass */;
}

/* read buttons */
void read_buttons()
{
    uint16_t btn = analogRead(0);
    uint64_t current_millis = millis();

    /* check for button click (with error margin) */
    if (abs(btn - last_btn) <= 50) return;

    /* avoid debounce */
    if (current_millis - last_millis <= 200) return;
    if      (btn <  90) btn_right();
    else if (btn < 200) btn_up();
    else if (btn < 400) btn_down();
    else if (btn < 600) btn_left();
    else if (btn < 800) btn_select();
    last_btn = btn;
    last_millis = current_millis;
}

/* read from serial */
void read_serial()
{
    if (Serial.available() < 1) return; /* Nothing to read */
    String serial_data = Serial.readStringUntil('\n'); /* incoming text */
    if (serial_data == last_serial_data) return; /* Nothing new received */

    /* if something new was received, display it */

    /* COM 1 */
    if (mode < 2) {
        lcd_clear();
        lcd.setCursor(0, 0);
        lcd.print("COM1: ");
        lcd.print(serial_data);
        lcd.setCursor(0, 1);
        lcd.print(" MHz  KHz");
        last_serial_data = serial_data;

    /* COM 2 */
    } else if (mode < 4) {
        lcd_clear();
        lcd.setCursor(0, 0);
        lcd.print("COM2: ");
        lcd.print(serial_data);
        lcd.setCursor(0, 1);
        lcd.print(" MHz  KHz");
        last_serial_data = serial_data;

    /* NAV 1 */
    } else if (mode < 6) {
        lcd_clear();
        lcd.setCursor(0, 0);
        lcd.print("NAV1: ");
        lcd.print(serial_data);
        lcd.setCursor(0, 1);
        lcd.print(" MHz  KHz");
        last_serial_data = serial_data;

    /* NAV 2 */
    } else if (mode < 8) {
        lcd_clear();
        lcd.setCursor(0, 0);
        lcd.print("NAV2: ");
        lcd.print(serial_data);
        lcd.setCursor(0, 1);
        lcd.print(" MHz  KHz");
        last_serial_data = serial_data;

    /* ADF */
    } else if (mode < 12) {
        lcd_clear();
        lcd.setCursor(0, 0);
        lcd.print("ADF: ");
        lcd.print(serial_data);
        last_serial_data = serial_data;

    /* DME */
    } else if (mode < 13) {
        lcd_clear();
        lcd.setCursor(0, 0);
        lcd.print("DME:  #");
        lcd.print(serial_data.substring(0, 1));
        lcd.setCursor(0, 1);
        lcd.print(serial_data.substring(1, 5));
        lcd.print(" nm ");
        lcd.print(serial_data.substring(5));
        lcd.print(" KT");
        last_serial_data = serial_data;
        /* FIX CURSOR POSITION BUG ON MODE CHANGE */

    /* XPDR */
    } else if (mode < 17) {
        lcd_clear();
        lcd.setCursor(0, 0);
        lcd.print("XPDR: ");
        lcd.print(serial_data);
        last_serial_data = serial_data;
    }
}


/* setup function */
void setup()
{
    /* write stuff to the LCD screen */
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("FVC FS2004 Radio");
    lcd.setCursor(0, 1);
    lcd.print(" V1.0 [13/11/22]");
    lcd_blink(0, 1);

    /* setup Serial port with USB baud rate */
    Serial.begin(115200);
}

/* loop function */
void loop()
{
    read_buttons();
    read_serial();
    blink_mode();
}
