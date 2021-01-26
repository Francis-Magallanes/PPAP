#include <LiquidCrystal.h>
#include <DS3231.h>
#include <Wire.h>
#include <Servo.h>

//for the profile class feed
class Feed
{
    private: 
      String Name;
      int amount;
      int frequency;
      bool isFeedGiven; //this will monitor whether the food is given within the schedule
      
    public:
      
      Feed(){
        this->Name = "Custom";
        this->amount = 0;
        this->frequency = 0;
        this->isFeedGiven = false;
      }
      
      Feed(String Name, int amount, int frequency){
          this->Name = Name;
          this->amount = amount;
          this->frequency = frequency;
          this->isFeedGiven = false;
      }
  
      //getters
      int getAmount(){ return amount; }
      int getFrequency() { return frequency; }
      String getName() { return Name; }
  
      //setters
      void setAmount(int amount){ this->amount = amount; }
      void setFrequency(int frequency) { this->frequency = frequency; }

     //for the checking whether it is feed or not
     //based on the inputted time
     bool isFeedingTime(int hr, int minute){
      /*
       * The feeding time is from 7:00 to 19:00 . This means that there will will be
       * 12 hours window for the feeding. This window time for feeding will be divided according to
       * the frequency set. This will be computed and stored to the delta variable
       */
      int delta; //in minutes
      int startTime = 420; //this the 7:00 but in minutes
      int minuteInput =  (hr * 60) + minute; //minute value of the input
      int allowance = 2; //this is for the allowance just in case the delay of the program affects the schedule of feeding

      if(frequency >1){
        delta = 720 / (frequency - 1);
      }
      else delta = 0;

      
      //this will run through the different time schedule using the delta
      for(int i = 1; i <= frequency ; i++){

        int currentSched = startTime + ( delta * (i-1) );
        if(minuteInput >= currentSched && minuteInput <= (currentSched + allowance) && !isFeedGiven){
          isFeedGiven = true; //if the condition are met, the machine will feed the pet
          return true;
        }
      }

      //this will set the isFeedGiven to false if it is without the time period
       for(int i = 1; i <= frequency ; i++){

        int currentSched = startTime + ( delta * (i-1) );
        int nextSched = currentSched + delta;
        if( minuteInput > currentSched + allowance && minuteInput < nextSched){
          isFeedGiven = false;
        }
      }
      
      return false; 
     }
  
};

const int rs = 13, rw = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal LCD(rs, rw, en, d4, d5, d6, d7); 
//for the clock of the machine
DS3231 Clock;

//for the motor
Servo myservo;

const int ButtonMain = 2;
const int ButtonUp = 4;
const int ButtonDown = 3;
const int BackLightPin = A1;
const int thresholdInactivity = 15000;//in milliseconds

int display_address = 0;
//this variable is for monitoring whether the display address
//is changed to avoid display flickering
int previous_display_address = -1;

Feed Preset1("Small Dog", 1, 2);
Feed Preset2("Medium Dog", 2, 2);
Feed Preset3("Large Dog", 3, 2);
Feed CustomPreset;
Feed *CurrentFeedPreset; //this will monitor the current apply preset
  
  
//these variables for the temporary storage for the custom set option
int tempFrequency;
int tempAmount;
//these variables for the temporary storage for the change time option
int tempHour;
int tempMinute;

//this variable is for monitoring whether the potentiometer or 
//display address is changed to avoid display flickering
int previous_poten_value = 0;

//this is for the monitoring of the inactivity to shut down the display using millis
int previousMillisCheck;
bool isScreenOn;


void setup(){

  //for the buttons
  pinMode(ButtonMain, INPUT_PULLUP);
  pinMode(ButtonUp, INPUT_PULLUP);
  pinMode(ButtonDown, INPUT_PULLUP);
  pinMode(BackLightPin, OUTPUT);

  myservo.attach(6);
  
  //for the lcd
  LCD.begin(16,2);

  //for the ds3231 rtc
  Wire.begin();
  
  Serial.begin(9600);

  //this will start the display
  isScreenOn = true;
  digitalWrite(BackLightPin, isScreenOn);
  
  //Preset 1 is the default feed preset
   CurrentFeedPreset = &Preset1;
}

void loop(){
  DisplayMenu(display_address);
  display_address = ButtonEvent(display_address);
  FeedingEvent();
  delay(10);
}


//for the display and the menu
//da - short for display address
void DisplayMenu(int da){

    if(da != previous_display_address){
       switch(da){

       case 0:
         LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("Current Time: ");
       break;
         
       case 1: 
        //Display of Feeding Settings
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("Main Menu");
        LCD.setCursor(0, 1);
        LCD.print("Change Feed Set.");
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
      case 15: //Back to the main menu
          LCD.clear();
          LCD.setCursor(0, 0);
          LCD.print("Select Option");
          LCD.setCursor(0, 1);
          LCD.print("Back");
        break;
         
     //for the options under the custom set
     // note the that value will be printed later in the code
    case 141: //for the frequency
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print("Custom Setting");
      LCD.setCursor(0, 1);
      LCD.print("Frequency: ");
      break;
         
    case 142://for the amount
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print("Custom Setting");
      LCD.setCursor(0, 1);
      LCD.print("Amount: ");
      break;
         
     case 143://for the confirm
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print("Custom Setting");
      LCD.setCursor(0, 1);
      LCD.print("Confirm");
      break;
         
     case 144://for the cancel
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print("Custom Setting");
      LCD.setCursor(0, 1);
      LCD.print("Cancel");
      break;
      
    //for the options under the change time settings
      case 2: 
        //Display of Clock Change Option
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("Main Menu");
        LCD.setCursor(0, 1);
        LCD.print("Change Time");
        break;

      case 21: //for the hour
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("Change Time");
        LCD.setCursor(0, 1);
        LCD.print("Hour: ");
      break;
         
    case 22://for the minute
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("Change Time");
        LCD.setCursor(0, 1);
        LCD.print("Minute: ");
      break;
         
     case 23://for the confirm 
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("Change Time");
        LCD.setCursor(0, 1);
        LCD.print("Confirm");
      break;
         
     case 24://for the cancel
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("Change Time");
        LCD.setCursor(0, 1);
        LCD.print("Cancel");
      break;

       case 3:
        //Display of info about the preset and food left 
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("Main Menu");
        LCD.setCursor(0, 1);
        LCD.print("System Info");
       break;
        
      case 31://display the food left
         LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("Food Left: ");
        LCD.print(GetFoodAmountLeft());
        LCD.setCursor(0, 1);
        LCD.print("Set: ");
        LCD.print(CurrentFeedPreset->getName());
       break;
         
      case 32://display the frequency
         LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("Frequency: ");
        LCD.print(CurrentFeedPreset->getFrequency());
        LCD.setCursor(0, 1);
        LCD.print("Amount: ");
        LCD.print(CurrentFeedPreset->getAmount());
        LCD.print(" Cups");
       break;
         
      case 4: 
        //Display of Back option
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("Main Menu");
        LCD.setCursor(0, 1);
        LCD.print("Back");
       break;

       default:

       break;
        
      }

     previous_display_address = da; // this is important to avoid flickering
    }
    
     if(da == 0){
      //this is for the displaying of the clock
      DisplayClock();
     }
    //this will display the values of the potentiometer for specific display addresses
      if(da == 141){
        //for the custom set frequency
        tempFrequency = PotentiometerEvent(display_address);
        if(previous_poten_value != tempFrequency){
            LCD.setCursor(10, 1);//set the cursor for overwriting
            LCD.print(tempFrequency);
          previous_poten_value = tempFrequency;
        }         
      }
      else if( da == 142){
        //for the custom set amount
        tempAmount = PotentiometerEvent(display_address);
        
        if(previous_poten_value != tempAmount){
          LCD.setCursor(8, 1);//set the cursor for overwriting
          LCD.print(tempAmount);
          LCD.print(" Cups");
                    
          previous_poten_value = tempAmount;
        }
          
      }
      else if(da == 21){
              //for the hour
              tempHour = PotentiometerEvent(display_address);
              if(previous_poten_value != tempHour){
                  LCD.setCursor(5, 1);//set the cursor for overwriting
                  LCD.print(tempHour);
                if(tempHour < 24) LCD.print("  ");
                previous_poten_value = tempHour;
              }         
            }
      else if(da == 22){
              //for the minute
              tempMinute = PotentiometerEvent(display_address);
              if(previous_poten_value != tempMinute){
                  LCD.setCursor(7, 1);//set the cursor for overwriting
                  LCD.print(tempMinute);
                if(tempMinute < 59) LCD.print("  ");
                previous_poten_value = tempMinute;
              }         
     }

 }

void DisplayClock(){
  bool h12;
  bool PM;
  int hour = Clock.getHour(h12,PM);
  int minute = Clock.getMinute();
  int second = Clock.getSecond();
  
  LCD.setCursor(0, 1);
  if(hour < 10) LCD.print("0");
  LCD.print(hour);
  LCD.print(":");
  if(minute < 10) LCD.print("0");
  LCD.print(minute);
  LCD.print(":");
  if(second < 10) LCD.print("0");
  LCD.print(second);
  
}

int ChangeClock(){
  Clock.setHour(tempHour);
  Clock.setMinute(tempMinute);
  Clock.setSecond(0);
  return 0;
}

//for the button events
int ButtonEvent(int da){

    int btnMain = DebounceButton(ButtonMain);
    int btnUp = DebounceButton(ButtonUp);
    int btnDown = DebounceButton(ButtonDown);
  
  //temporary variable
    int tempAddress = da;
  
  //case to case basis and the screen should be on
  if(da <= 0 && isScreenOn) {
    //for the display of clock
    if(btnMain == LOW){
      tempAddress = 1;
    }
  }
  else if(isScreenOn){
    
    //when the main button is pressed
    if(btnMain == LOW){
      
      
      if(da == 4 || da ==  15 ){
        //for the back option
        tempAddress = tempAddress / 10;
      }
      else if (da == 24 || da == 144){
        //for the canceel option
        tempAddress = 0;
      }
      else if (da == 23){
        //for the clock
        tempAddress = ChangeClock();
      }
      else if(da == 31 || da == 32){
        tempAddress = 0; //this will return to the clock
      }
      //for the confirmation of setting of the preset
      else if(da == 11 || da == 12 || da==23 || da==13 || da == 143){
        tempAddress = FeedSettingSelection(da);
      }

      // the condition is for the scenario where the user press the main button
      //during the changing of the time or setting the custom setting
      else if(da != 141 && da != 142 && da != 21 && da != 22) tempAddress = (tempAddress * 10) +1 ;//for the sub-menu selection
     
    }
  
    //when the button down is pressed
    else if(btnDown ==LOW){
    
      //for the looping of the menu
      if(da == 4) tempAddress = 1;
      else if (da == 15) tempAddress = 11;
      else if (da == 144) tempAddress = 141;
      else if( da == 32) tempAddress = 31;
      else if( da == 24) tempAddress = 21;
      else tempAddress ++;

    }
    
    //when the button up is pressed
    else if(btnUp == LOW){
    
      //for the looping of menu
      if(da == 1) tempAddress = 5;
      else if(da == 11) tempAddress = 15;
      else if (da == 141) tempAddress = 144;
      else if( da == 31) tempAddress = 32;
      else if( da == 21) tempAddress = 24;
      else tempAddress --;

    }
    
  }


  //this will monitor whether there is inactivity using the buttons 
  if(btnMain == HIGH && btnUp == HIGH && btnDown == HIGH){

    int inactiveTime = millis() - previousMillisCheck;

    if(inactiveTime > thresholdInactivity){
      isScreenOn = false;
      digitalWrite(BackLightPin, LOW);
      LCD.noDisplay();      
    }
    
  }
  else{
    
    previousMillisCheck = millis();

     //when the screen off, needs to be turned on
    if(!isScreenOn){
       isScreenOn = true;
      digitalWrite(BackLightPin, HIGH);
      LCD.display(); 
      tempAddress = 0;//this will return to the clock after a button pressed
    }
   
  }

  
  return tempAddress;
}

int PotentiometerEvent(int da){
  //this will return the mapped value of the pontentiometer
  //according to the display address
  
  int mappedReading = 0;
  int reading = analogRead(A0); 

  Serial.println(reading);
  int maxPotenReading = 885;

  if(da == 141){
    //for the frequency of feeding
    mappedReading = map(reading,0,maxPotenReading,1,6);
  }
  else if(da == 142){
    //for the amount of feeding in cups
    mappedReading = map(reading,0,maxPotenReading,1,9);
  }
  else if(da == 21){
    //for the changing of hour
    mappedReading = map(reading,0,maxPotenReading,00,23);
  }
  else if(da == 22){
    //for the changing of minute
    mappedReading = map(reading,0,maxPotenReading,00,59);
  }

  
  return mappedReading;
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

//da refers to the display address variable
//this function will excute the selection
//displaying a affirmation window
int FeedSettingSelection(int da){
  
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("Using ");
  
  if(da == 11){
    //for the selection of the first preset
    CurrentFeedPreset = &Preset1;
    LCD.print(CurrentFeedPreset->getName());
  }
  else if(da == 12){
    //for the selection of the second preset
    CurrentFeedPreset = &Preset2;
    LCD.print(CurrentFeedPreset->getName());
    
  }
  else if(da == 13){
    //for the selection of the third preset
    CurrentFeedPreset = &Preset3;
    LCD.print(CurrentFeedPreset->getName());
    
  }
  else if(da == 143){
    //for the selection of the custom preset
    CustomPreset.setFrequency(tempFrequency);
    CustomPreset.setAmount(tempAmount);
    CurrentFeedPreset = &CustomPreset;
    LCD.print(CurrentFeedPreset->getName());
    
  }
  LCD.setCursor(0, 1);
  LCD.print("Returning...");
  delay(2000);//2 second delay before returning to the time
  
  return 0; //this will make the display go back to the time
}

//this function will handle the feeding process
void FeedingEvent(){
  bool h12;
  bool PM;
  int currentHour = Clock.getHour(h12,PM); //get the current time
  int currentMinute = Clock.getMinute();
  
  //check first whether it is feeding time
  if( CurrentFeedPreset -> isFeedingTime(currentHour, currentMinute) ){
    //for the message
    LCD.clear();
    LCD.setCursor(0, 0);
    LCD.print("Feeding your dog");
    LCD.setCursor(0, 1);
    LCD.print("UwU ...........");

    //for the feeding proper and this will dispense the food
      DispenseFood(4);
      
    //checking whether there is enough food left
    if(GetFoodAmountLeft() <= 300){
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print("Food left less");
      LCD.setCursor(0, 1);
      LCD.print("than 300 grams");

      //sound for the notification
      
    }
    
  }
  
}

void DispenseFood(int cups){
  //this will calculate the delay for which the "valve" is open
  //this is done through linear regression
  
  float  gramsFeed = 97 * cups; //based on the sample got
  float timeDelay = (gramsFeed - 58.5)/63.5; //this is in seconds

  myservo.write(0); // this will open the "valve"
  delay(timeDelay * 1000);
  myservo.write(90); //this will close the "valve"
}

//this function will get the food amount left from the load cell
int GetFoodAmountLeft(){
  //to be modified
  return 1000;
}
