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
const int ButtonUp = 4;
const int ButtonDown = 3;

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
         
       //for the options under the feeding settings
      case 11: 
        //showing the Preset1
      LCD.clear();
      LCD.setCursor(0, 0);
        LCD.print("Select Option");
        LCD.setCursor(0, 1);
        LCD.print(Preset1.getName());
      break;
    case 12: //Preset2
      LCD.clear();
      LCD.setCursor(0, 0);
        LCD.print("Select Option");
        LCD.setCursor(0, 1);
        LCD.print(Preset2.getName());
      break;
    case 13: //Preset3
      LCD.clear();
      LCD.setCursor(0, 0);
        LCD.print("Select Option");
        LCD.setCursor(0, 1);
        LCD.print(Preset3.getName());
      break;
    case 14: //Custom
      LCD.clear();
      LCD.setCursor(0, 0);
        LCD.print("Select Option");
        LCD.setCursor(0, 1);
        LCD.print("Custom Set");
      break;     
    case 15: //Back
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print("Select Option");
      LCD.setCursor(0, 1);
      LCD.print("Back");
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
        //Display of Food Left Inside option
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("Main Menu");
        LCD.setCursor(0, 1);
        LCD.print("Food Left");
        break;
        
      case 4: 
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

    int btnMain = DebounceButton(ButtonMain);
    int btnUp = DebounceButton(ButtonUp);
    int btnDown = DebounceButton(ButtonDown);
  
  //temporary variable
    int tempAddress = da;
  
  //case to case basis 
  if(da <= 0) {
    //for the display of clock
    if(btnMain == LOW){
      tempAddress = 1;
    }
  }
  else{
    
    if(btnMain == LOW){
      if(da == 4 || da ==  15) tempAddress = tempAddress / 10;
      else tempAddress = (tempAddress * 10) +1 ;
      delay(10);
    }

    else if(btnDown ==LOW){

      if(da == 4) tempAddress = 1;
      else if (da == 15) tempAddress = 11;
      else tempAddress ++;

    }
    else if(btnUp == LOW){

      if(da == 1) tempAddress = 5;
      else if(da == 11) tempAddress = 15;
      else tempAddress --;

    }
    
  }
 
  
  return tempAddress;
}

int PotentiometerEvent(int){
  //this will return the value of the pontentiometer according to the current settings
  
}

//this for the debouncing of the button results
//dpi stands for digital pin input
int DebounceButton(int dpi){
  int results = digitalRead(dpi);
  
  if(results == LOW){
    
    delay(100);//adjust if there are still bouncing
    if(results == digitalRead(dpi)) return LOW;
    else return HIGH;
  }
  
  return HIGH;
}

void setup(){
  
  pinMode(ButtonMain, INPUT_PULLUP);
  pinMode(ButtonUp, INPUT_PULLUP);
  pinMode(ButtonDown, INPUT_PULLUP);
  LCD.begin(16,2);
  Serial.begin(9600);
  
}

void loop(){
  DisplayMenu(display_address);
  display_address = ButtonEvent(display_address);
  delay(10);
} 
