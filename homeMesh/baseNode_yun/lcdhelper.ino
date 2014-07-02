
// lcd clear line utility function

void clearLine ( int line ) {
  clearLine(line, 16); // deafult width to clear
}

void  clearLine (int line, int width){
    lcd.setCursor(0,line);
    for (int i =0 ; i < width; i ++ ) {
        lcd.print(" ");
    }
    
    lcd.setCursor(0,line);
} // end clear line


// message utility function
void  lcdMessage(String m) {
  lcdMessage(m,1); // default write to second line
}

void  lcdMessage(String m, int l) {
   // LDC messages
    clearLine(l);
    lcd.setCursor(0,l); // start of second (1) line 
    lcd.print(m);
  
}

// message utility function
void  lcdMessage(char c) {
  lcdMessage(c,1); // default write to second line
}

void  lcdMessage(char c, int l) {
   // LDC messages
    lcd.print(c);
  
}

// used ??

void  lcdPrint( String s ){
	lcd.print(s);
}

void  lcdPrintln(String s) {
	lcd.println(s);

}
