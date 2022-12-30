#include <LiquidCrystal.h>
#include <math.h>
#include <stdint.h>

/* LCD Display */
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

/* keypad */
uint16_t last_btn = 1023;
uint64_t last_millis;
const uint16_t button_delay = 200;

/* serial */
String last_serial_data = "";

/* mode */
uint8_t mode = 0;
const uint8_t MAX_MODE = 23;
/*
 *  0: COM1  MHz |  1: COM1  KHz |  2: COM2  MHz |  3: COM2  KHz
 *  4: NAV1  MHz |  5: NAV1  KHz |  6: NAV2  MHz |  7: NAV2  KHz
 *  8: ADF   100 |  9: ADF   010 | 10: ADF   001 | 11: ADF   DEC
 * 12: DME  SWAP | 13: XPDR 1000 | 14: XPDR 0100 | 14: XPDR 0010
 * 16: XPDR 0001 | 17: AP ALT    | 18: AP VS     | 19: AP HDG
 * 20: NAV HLD   | 21: APR HLD   | 22: NAV MODE  | 23: MASTER
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

    /* AP */
    else if (mode >= 17 && mode <= 22) lcd_blink(4, 1);
    else if (mode == 23) lcd_blink(7, 1);
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
    else if (mode == 12) Serial.println("DME_TGL");

    /* XPDR */
    else if (mode == 13) Serial.println("XPDR_1000UP");
    else if (mode == 14) Serial.println("XPDR_0100UP");
    else if (mode == 15) Serial.println("XPDR_0010UP");
    else if (mode == 16) Serial.println("XPDR_0001UP");

    /* AP */
    else if (mode == 17) Serial.println("AP_ALT_UP");
    else if (mode == 18) Serial.println("AP_VS_UP");
    else if (mode == 19) Serial.println("AP_HDG_UP");
    else if (mode == 20) Serial.println("AP_NAV_HLD");
    else if (mode == 21) Serial.println("AP_APR_HLD");
    else if (mode == 22) Serial.println("AP_NAV_MOD");
    else if (mode == 23) Serial.println("AP_MASTER");
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
    else if (mode == 12) Serial.println("DME_TGL");

    /* XPDR */
    else if (mode == 13) Serial.println("XPDR_1000DN");
    else if (mode == 14) Serial.println("XPDR_0100DN");
    else if (mode == 15) Serial.println("XPDR_0010DN");
    else if (mode == 16) Serial.println("XPDR_0001DN");

    /* AP */
    else if (mode == 17) Serial.println("AP_ALT_DN");
    else if (mode == 18) Serial.println("AP_VS_DN");
    else if (mode == 19) Serial.println("AP_HDG_DN");
    else if (mode == 20) Serial.println("AP_NAV_HLD");
    else if (mode == 21) Serial.println("AP_APR_HLD");
    else if (mode == 22) Serial.println("AP_NAV_MOD");
    else if (mode == 23) Serial.println("AP_MASTER");
}

void btn_right()
{
    mode = mode == MAX_MODE ? 0 : mode + 1;
    Serial.println("MODE " + String(mode));
}

void btn_left()
{
    mode = mode == 0 ? MAX_MODE : mode - 1;
    Serial.println("MODE " + String(mode));
}

void btn_select()
{
    if      (mode <  2) Serial.println("COM1_SWP");
    else if (mode <  4) Serial.println("COM2_SWP");
    else if (mode <  6) Serial.println("NAV1_SWP");
    else if (mode <  8) Serial.println("NAV2_SWP");
    else if (mode < 12) Serial.println("MODE " + String(mode = 12));
    else if (mode < 13) Serial.println("DME_TGL");
    else if (mode < 17) Serial.println("MODE " + String(mode = 17));
    else if (mode < 18) Serial.println("AP_ALT_H");
    else if (mode < 19) /* Unused */;
    else if (mode < 20) Serial.println("AP_HDG_H");
    else if (mode < 21) Serial.println("AP_NAV_HLD");
    else if (mode < 22) Serial.println("AP_APR_HLD");
    else if (mode < 23) Serial.println("AP_NAV_MOD");
    else if (mode < 24) Serial.println("AP_MASTER");
}

/* read buttons */
void read_buttons()
{
    uint16_t btn = analogRead(0);
    uint64_t current_millis = millis();

    /* check for button click (with error margin) */
    if (abs(btn - last_btn) <= 50) return;

    /* avoid debounce */
    if (current_millis - last_millis <= button_delay) return;
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
    if (Serial.available() < 1) return;                /* Nothing to read */
    String serial_data = Serial.readStringUntil('\n'); /* incoming text   */
    if (serial_data == last_serial_data) return;       /* Nothing new     */

    /* if something new was received, display it */

    /* COM 1 */
    if (mode < 2) {
        lcd_clear();
        lcd.setCursor(0, 0);
        lcd.print("COM1: ");
        lcd.print(serial_data.substring(2));
        lcd.setCursor(0, 1);
        lcd.print(" MHz  KHz");
        last_serial_data = serial_data;

    /* COM 2 */
    } else if (mode < 4) {
        lcd_clear();
        lcd.setCursor(0, 0);
        lcd.print("COM2: ");
        lcd.print(serial_data.substring(2));
        lcd.setCursor(0, 1);
        lcd.print(" MHz  KHz");
        last_serial_data = serial_data;

    /* NAV 1 */
    } else if (mode < 6) {
        lcd_clear();
        lcd.setCursor(0, 0);
        lcd.print("NAV1: ");
        lcd.print(serial_data.substring(2));
        lcd.setCursor(0, 1);
        lcd.print(" MHz  KHz");
        last_serial_data = serial_data;

    /* NAV 2 */
    } else if (mode < 8) {
        lcd_clear();
        lcd.setCursor(0, 0);
        lcd.print("NAV2: ");
        lcd.print(serial_data.substring(2));
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

    /* XPDR */
    } else if (mode < 17) {
        lcd_clear();
        lcd.setCursor(0, 0);
        lcd.print("XPDR: ");
        lcd.print(serial_data);
        last_serial_data = serial_data;

    /* AP ALT */
    } else if (mode < 18) {
        lcd_clear();
        lcd.setCursor(0, 0);
        lcd.print("AUTOPILOT");
        lcd.setCursor(0, 1);
        lcd.print("ALT: ");
        lcd.setCursor(5, 1);
        lcd.print(serial_data);
        last_serial_data = serial_data;

    /* AP VS */
    } else if (mode < 19) {
        lcd_clear();
        lcd.setCursor(0, 0);
        lcd.print("AUTOPILOT");
        lcd.setCursor(0, 1);
        lcd.print("VS:  ");
        lcd.setCursor(5, 1);
        lcd.print(serial_data);
        last_serial_data = serial_data;

    /* AP HDG */
    } else if (mode < 20) {
        lcd_clear();
        lcd.setCursor(0, 0);
        lcd.print("AUTOPILOT");
        lcd.setCursor(0, 1);
        lcd.print("HDG: ");
        lcd.setCursor(5, 1);
        lcd.print(serial_data);
        last_serial_data = serial_data;

    /* AP NAV */
    } else if (mode < 21) {
        lcd_clear();
        lcd.setCursor(0, 0);
        lcd.print("AUTOPILOT");
        lcd.setCursor(0, 1);
        lcd.print("NAV: ");
        lcd.setCursor(5, 1);
        lcd.print(serial_data.substring(1));
        last_serial_data = serial_data;

    /* AP APR */
    } else if (mode < 22) {
        lcd_clear();
        lcd.setCursor(0, 0);
        lcd.print("AUTOPILOT");
        lcd.setCursor(0, 1);
        lcd.print("APR: ");
        lcd.setCursor(5, 1);
        lcd.print(serial_data.substring(1));
        last_serial_data = serial_data;

    /* AP NAV MOD */
    } else if (mode < 23) {
        lcd_clear();
        lcd.setCursor(0, 0);
        lcd.print("AUTOPILOT");
        lcd.setCursor(0, 1);
        lcd.print("MOD: ");
        lcd.setCursor(5, 1);
        lcd.print(serial_data);
        last_serial_data = serial_data;

    /* AP MASTER */
    } else if (mode < 24) {
        lcd_clear();
        lcd.setCursor(0, 0);
        lcd.print("AUTOPILOT");
        lcd.setCursor(0, 1);
        lcd.print("MASTER: ");
        lcd.setCursor(8, 1);
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
