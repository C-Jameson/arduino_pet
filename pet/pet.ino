#include <SSD1306Ascii.h>
#include <SSD1306AsciiAvrI2c.h>
#include <SSD1306AsciiSoftSpi.h>
#include <SSD1306AsciiSpi.h>
#include <SSD1306AsciiWire.h>
#include <SSD1306init.h>

String petNames[] = {"Toaster", "Frog", "Johm", "Jonh", "Yeeticus III", "Himbo"};
String petName = petNames[random(0,6)];
String pets[] = {"^ ^\n(><)"}
String healthBar{"[----------]", "[+---------]", "[++--------]", "[+++-------]", "[++++------]", "[+++++-----]", "[++++++----]", "[+++++++---]", "[++++++++--]", "[+++++++++-]", "[++++++++++]"};
String petWords{" doesn't feel so good.", " looks hungry.", " looks thirsty.", " adores the pets!", " loves the food!", " looks refreshed."};
String speechBubble;
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

void petTalk(int talkIndex) {
  speechBubble = petName + petWords[talkIndex];
}
