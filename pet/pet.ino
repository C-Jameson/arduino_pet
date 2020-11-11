#include <SSD1306Ascii.h>
#include <SSD1306AsciiAvrI2c.h>
//#include <SSD1306AsciiSoftSpi.h>
//#include <SSD1306AsciiSpi.h>
//#include <SSD1306AsciiWire.h>
//#include <SSD1306init.h>

#define SLEEPPIN 4
#define SWITCHPIN 8
#define FOODPIN 7
#define WATERPIN 6
#define ATTNPIN 5


#define I2C_ADDRESS 0x3c
SSD1306AsciiAvrI2c oled;

int sleepState = 0;
int switchState = 0;
int foodState = 0;
int waterState = 0;
int attnState = 0;

String NameOptions[] = {"Toaster  Age: ", "Frog     Age: ", "Johm     Age: ", "Jonh     Age: ", "Yeet III Age: ", "Himbo    Age: ", "asdlkjf  Age: "};
//each line is 15 characters long exactly
String petFaces[][4] = {{"     /\\ /\\     ","     |~ ~|     ","     \\   /     ", "       \\@/      "}, {"     /\\_/\\     ", "    (~   ~)    ", "     \\>*</     ", "      | |      "}};
String healthValues[] = {"  [----------]  ", "  [+---------]  ", "  [++--------]  ", "  [+++-------]  ", "  [++++------]  ", "  [+++++-----]  ", "  [++++++----]  ", "  [+++++++---]  ", "  [++++++++--]  ", "  [+++++++++-]  ", "  [++++++++++]  "};
//three of them together is 15 characters
String statValues[] = {"[---]", "[+--]", "[++-]", "[+++]"};

String displayName;
String nameBar;
String healthBar;
int petPictureIndex;
String statBar;
String statLabel = "Food Water Attn";

bool foodPress = false;
bool waterPress = false;
bool attnPress = false;

unsigned long lastLoop = 0;
unsigned long lastFood = 0;
unsigned long lastWater = 0;
unsigned long lastAttn = 0;

unsigned long foodDelay = 600000;
unsigned long waterDelay = 300000;
unsigned long attnDelay =900000;
unsigned long tickDelay = 6000;

bool asleep = false;

class Pet {
  public:
  bool alive = false;
  String petName;
  int faceIndex;

  int maxHealth;
  int health;

  int food;
  int water;
  int attn;

  int age;

  int hunger;
  int thirst;
  int neediness;

  bool hadBaby;

  void setUpPet(){
    alive = true;
    
    petName = NameOptions[random(0,7)];
    faceIndex = random(0,2);
  
    maxHealth = random(5000,15001);
    health = maxHealth;
    
    food = 30000;
    water = 30000;
    attn = 20000;
    
    age = 0;
  
    hunger = random(1,16);
    thirst = random(2,11);
    neediness = random(0,21);
  
    hadBaby = false;
  }

  void giveFood(){
    food += random(3000,5000);
  }
  void giveWater(){
    water += random(4000,5500);
  }
  void giveAttn(){
    attn += random(1000, 10000);
  }
  
  void damage(){
    health -= 1;
  }
};
Pet pet0, pet1, pet2, pet3, pet4, pet5, pet6, pet7, pet8, pet9;
Pet pets[] = {pet0, pet1, pet2, pet3, pet4, pet5, pet6, pet7, pet8, pet9};
int petIndex = 0;

void setup() {
  // put your setup code here, to run once:

  pets[petIndex].setUpPet();

  pinMode(SLEEPPIN, INPUT);
  pinMode(SWITCHPIN, INPUT);
  pinMode(FOODPIN, INPUT);
  pinMode(WATERPIN, INPUT);
  pinMode(ATTNPIN, INPUT);

  //setup oled
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(font8x8);
  oled.clear();
}

void loop() {
  // put your main code here, to run repeatedly:

  sleepState = digitalRead(SLEEPPIN);
  if(sleepState == HIGH){
    asleep = !asleep;
    delay(30000);
  }
  if(!asleep){
    switchState = digitalRead(SWITCHPIN);
    foodState = digitalRead(FOODPIN);
    waterState = digitalRead(WATERPIN);
    attnState = digitalRead(ATTNPIN);
  
    if(switchState == HIGH){
      
    }
    else if(foodState == HIGH){
      if((millis()-lastFood)> foodDelay){
        foodPress = true;
      }
    }
    else if(waterState == HIGH){
      if((millis()-lastWater)> waterDelay){
        waterPress = true;
      }
    }
    else if(attnState == HIGH){
      if((millis()-lastAttn)> attnDelay){
        attnPress = true;
      }
    }
    
    //print displayName
    oled.setCursor(1,1);
    oled.print(nameBar);
    //print healthBar
    oled.setCursor(1,2);
    oled.print(healthBar);
    //print petFaces[petPictureIndex]
    for(int i = 0; i < 4; i++){
      oled.setCursor(3,i+3);
      oled.print(petFaces[petPictureIndex][i]);
    }
    //print statBar
    oled.setCursor(3,7);
    oled.print(statBar);
    //print statLabel
    oled.setCursor(1,8);
    oled.print(statLabel);
    //change pet buttons
  
    //food water and attn buttons
  
    //sleep button
  
    if((millis() - lastLoop) >= tickDelay){
      for( int i = 0; i < 8; i++){
        if(pets[i].alive){
          tick(i);
          if(foodPress){
            pets[i].giveFood();
            lastFood = millis();
          }
          if(waterPress){
            pets[i].giveWater();
            lastWater = millis();
          }
          if(attnPress){
            pets[i].giveAttn();
            lastAttn = millis();
          }
        }
      }
    }
    updateNameBar(petIndex);
    updateHealthBar(petIndex);
    updateStatBar(petIndex);
    lastLoop = millis();
  }
}

void buildStatBar(int food, int water, int attention){
  statBar = statValues[food] + statValues[water] + statValues[attention];
}

void changePet(int change){
  while(true){
    petIndex = petIndex + change;
    if(petIndex < 0){
      petIndex = 7;
    }
    else if(petIndex > 7){
      petIndex = 0;
    }
    if(!pets[petIndex].alive){
      continue;
    }
    else{
      break;
    }
  }

  displayName = pets[petIndex].petName;
  petPictureIndex = pets[petIndex].faceIndex;
}

void updateNameBar(int index){
  Pet currentPet = pets[index];
  if(round(currentPet.age/10000) > 99){
    nameBar = displayName + "99";
  }
  else{
    String age = String(round(currentPet.age/10000));
    nameBar = displayName + age.substring(0,2);
  }
}

void updateHealthBar(int index){
  Pet currentPet = pets[index];
  int maxHealth = currentPet.maxHealth;
  int currentHealth = currentPet.health;
  int healthIndex = round((currentHealth/maxHealth)*10);
  healthBar = healthValues[healthIndex];
}

void updateStatBar(int index) {
  Pet currentPet = pets[index];
  int food = round((currentPet.food/30000)*3);
  int water = round((currentPet.water/30000)*3);
  int attn = round((currentPet.attn/30000)*3);

  buildStatBar(food, water, attn); 
}
void tick(int index){
  Pet currentPet = pets[index];

  currentPet.food -= currentPet.hunger;
  currentPet.water -= currentPet.thirst;
  currentPet.attn -= currentPet.neediness;

  currentPet.age += 1;

  if(currentPet.food < 10000){
    pets[index].damage();
  }
  if(currentPet.water < 10000){
    pets[index].damage();
  }
  if(currentPet.attn < 10000){
    currentPet.food -= currentPet.hunger;
    currentPet.water -= currentPet.thirst;
  }
  if(!currentPet.hadBaby){
    if(currentPet.age > 7000){
      for( int i = 0; i < 8; i++){
        if(!pets[i].alive){
          pets[i].setUpPet();
          currentPet.hadBaby = true;
          break;
        }
      }
    }
  }
  if(currentPet.health <= 0){
    currentPet.alive = false;
  }
}

void sleep(){
  for( int i = 0; i < 8; i++){
    if(pets[i].alive){
      if(pets[i].food > 20000 && pets[i].water > 20000){
        pets[i].health += random(2500,5000);
      }
    }
  }
  asleep = true;
}
