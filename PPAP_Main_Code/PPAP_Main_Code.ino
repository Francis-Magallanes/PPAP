#include <LiquidCrystal.h>

//for the time class definition
class Time{
   private:
      int hr;
      int minute;
      int sec;
      long timesec; //this is the combined time in secs.
      //this function will convert the hr, min, and secs into one time in seconds
      
      void ConvertToTimesec(){
        //the reason for the transfer because the range of int will be
         //exceed once calculations is done.
        long h = hr;
        timesec = (3600 * h) + (60 * minute)+ sec;
       }
       
     public:
     
       Time(){
         hr = 0;
         minute = 0;
         sec = 0;
         timesec = 0;
       }
       
       void setHour(int hour){
         hr = hour % 24; //this will ensure that the hour is less than or equal to 24
         ConvertToTimesec(); //this update the timesecs in secs
       }
       int getHour(){
        return hr;
       }
       void setMinute(int minute){
         minute = minute % 60; //this will ensure that the minute is less than or equal to 60
         ConvertToTimesec(); //this update the timesecs in secs
       }
       int getMinute(){
         return minute;
       }
       void setSecond(int Second){
         sec = Second % 60; //this will ensure that the second is less than or equal to 60
         ConvertToTimesec(); //this update the timesecs in secs
       }
       
       int getSecond(){
         return sec;
       }
       
       void IncreaseOneSec (){
       timesec = (timesec + 1) % 86400; //this will ensure that it will go circular once it hits 24hrs
       //this will convert the time secs into its respective time format.
       //also, this is for update the variables
         hr = timesec / 3600;
         minute = (timesec%3600) / 60;
         sec = (timesec%3600) % 60;
       }

};

//for the profile class feed
class Feed
{
  
    private: 
      String Name;
      int weight;
      int frequency;
      
    public:
      
      Feed(){
        this->Name = "Custom";
        this->weight = 0;
        this->frequency = 0;
      }
      
      Feed(String Name, int weight, int frequency){
          this->Name = Name;
          this->weight = weight;
          this->frequency = frequency;
      }
  
      //getters
      int getWeight(){ return weight; }
      int getFrequency() { return frequency; }
      String getName() { return Name; }
  
      //setters
      void setWeight(int weight){ this->weight = weight; }
      void setFrequency() { this->frequency = frequency; }
  
};

const int rs = 13, rw = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal LCD(rs, rw, en, d4, d5, d6, d7); 

const int ButtonMain = 2;
const int ButtonUp = 3;
const int ButtonDown = 4;

int display_address = 0;
int previous_display_address = -1;

Feed Preset1("XDogs", 1, 6);
Feed Preset2("YDogs", 1, 6);
Feed Preset3("ZDogs", 1, 6);
Feed Customized();

//for the display
//da - short for display address
void DisplayMenu(int da){

    if(da != previous_display_address){
       switch(da){
  
       case 0:
        //Display of Clock
        DisplayClock();  
        break;
        
       case 1: 
        //Display of Feeding Settings
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("Main Menu");
        LCD.setCursor(0, 1);
        LCD.print("Feeding Settings");
        break;
        
      case 2: 
        //Display of Clock Change Option
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("Main Menu");
        LCD.setCursor(0, 1);
        LCD.print("Change Time");
        break;
        
      case 3:
       //Display of reset option
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("Main Menu");
        LCD.setCursor(0, 1);
        LCD.print("Reset");
        break;
  
      case 4:
        //Display of Food Left Inside option
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("Main Menu");
        LCD.setCursor(0, 1);
        LCD.print("Food Left");
        break;
        
      case 5: 
        //Display of Back option
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("Main Menu");
        LCD.setCursor(0, 1);
        LCD.print("Back");
        break;
        
      }

      previous_display_address = da; // this is important to avoid flickering
    }
     
}

void DisplayClock(){
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("Current Time: ");
  LCD.setCursor(0, 1);
  
}

//for the button events
int ButtonEvent(int da){

    int btnMain = digitalRead(ButtonMain);
    int btnUp = digitalRead(ButtonUp);
    int btnDown = digitalRead(ButtonDown);

    int tempAddress = da;
    
  //case to case basis 
  
  if(btnMain == LOW){
    
    if(da == 0)  tempAddress = 1;
    else if(da == 5) tempAddress = 0;
    delay(10);
  }
  else if(btnDown ==LOW){
    
    if( da >= 1 || da <= 5){
      tempAddress = (tempAddress % 5) + 1;
    }
    
  }
  else if(btnUp == LOW){
    
    if(da < 2) tempAddress = 5;
    else tempAddress --;
    
    
  }
  
  return tempAddress;
}

int PotentiometerEvent(int){
  //this will return the value of the pontentiometer according to the current settings
  
}


void setup(){
  
  pinMode(ButtonMain, INPUT_PULLUP);
  pinMode(ButtonUp, INPUT_PULLUP);
  pinMode(ButtonDown, INPUT_PULLUP);
  LCD.begin(16,2);
  Serial.begin(9600);
  
}

void loop(){
  delay(10);
  DisplayMenu(display_address);
  delay(10);
  display_address = ButtonEvent(display_address);
  delay(20);
}
