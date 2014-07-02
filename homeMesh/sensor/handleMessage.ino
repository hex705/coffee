void handleMessage( String s ){
  
  cut.update(s);

  String target = cut.getString(0); // local OR NI
  String action = cut.getString(1);  // read, write, mode
  String type   = cut.getString(2);
  int    pin    = cut.getInt(3);
  int    value  = cut.getInt(4);
  

    
        if (action == "read") {
      
            if ( type == "digital" ) {
                 value = digitalRead(pin);
            }
            
            if ( type == "analog" ){
                value =  analogRead(pin);
            }
            
            // call back here ? 
            
        }
      
          if (action == "write") {
            
             if ( type == "digital" ) {
                   digitalWrite(pin,value);
             }
             
             if (type == "analog"){
                   analogWrite(pin,value);
             }
          
          }
         
          if (action == " read") {
          
              if ( type == "input") {
                  pinMode(pin, INPUT);
              } 
              
              if (type =="output") {
                  pinMode(pin, OUTPUT);
              }
      
          }

    
    
    
}



