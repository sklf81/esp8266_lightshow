#ifndef SEQ_LENGTH
#define SEQ_LENGTH 256
#endif

typedef struct COLOR{
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  unsigned char EOA;
}COLOR;

long unsigned int lastOn[3] = {millis(), millis(), millis()};

void PWM(unsigned char pin_red, unsigned char pin_green, unsigned char pin_blue, COLOR color);
void setColorSequenze(String colormode, COLOR gradient[], unsigned int gradient_length, COLOR colorSequenz[]);
unsigned int toInteger(String str);
int hexToDec(String str);
void convertRGB(COLOR* color, String cw);

void PWM(unsigned char pin_red, unsigned char pin_green, unsigned char pin_blue, COLOR color){
  unsigned char time_on[3];
  unsigned char pins[3];
  time_on[0] = color.red; pins[0] = pin_red;
  time_on[1] = color.green; pins[1] = pin_green;
  time_on[2] = color.blue; pins[2] = pin_blue;
  
  for(int i = 0; i < 3; i++)
    if(time_on[i] != 255 && time_on[i] != 0)
      time_on[i] = (unsigned char)map(time_on[i], 0, 255, 0, 20);
      
      
  for(int i = 0; i < 3; i++){
      if(time_on[i] == 0){
        digitalWrite(pins[i], LOW);
      }
      if(time_on[i] == 255){
        lastOn[i] = millis();
        digitalWrite(pins[i], HIGH);
      }
      if(time_on[i] != 255 && time_on[i] != 0){
        if((millis() - lastOn[i] > time_on[i]))
          digitalWrite(pins[i], LOW);
        if((millis() - lastOn[i] >= 20 - time_on[i])){
          lastOn[i] = millis();
          digitalWrite(pins[i], HIGH);
        }
      }
    }
}

void setColorSequenze(String colormode, COLOR gradient[], unsigned int gradient_length, COLOR colorSeq[]){
  for(int i = 0; i < SEQ_LENGTH; i++){
    colorSeq[i].red = 0;
    colorSeq[i].green = 0;
    colorSeq[i].blue = 0;
    colorSeq[i].EOA = 0;
  }
  colorSeq[SEQ_LENGTH].EOA = 1;

  if(colormode == "stroke"){
    for(int i = 0; i < gradient_length; i++){    
      colorSeq[i] = gradient[i];
    }
    colorSeq[gradient_length].EOA = 1;
  }
  if(colormode == "smoothloop"){
    unsigned char steps = 5;
    unsigned short counter_total = 0;
    
    for(int i = 0; i < gradient_length - 1; i++){
  		short stepsize_red = (gradient[i + 1].red - gradient[i].red) / steps;
  		short stepsize_green = (gradient[i + 1].green - gradient[i].green) / steps;
  		short stepsize_blue = (gradient[i + 1].blue - gradient[i].blue) / steps;
  		for (int j = 0; j < steps; j++){
  			colorSeq[counter_total].red = gradient[i].red + stepsize_red * j;
  			colorSeq[counter_total].green = gradient[i].green + stepsize_green * j;
  			colorSeq[counter_total].blue = gradient[i].blue + stepsize_blue * j;
        counter_total++;
  		}
    }
    colorSeq[counter_total].red = gradient[gradient_length - 1].red;
    colorSeq[counter_total].green = gradient[gradient_length - 1].green;
    colorSeq[counter_total].blue = gradient[gradient_length - 1].blue;
    for(int i = counter_total; i <= 2 * counter_total; i++){
      colorSeq[i].red = colorSeq[2 * counter_total - i].red;
      colorSeq[i].green = colorSeq[2 * counter_total - i].green;
      colorSeq[i].blue = colorSeq[2 * counter_total - i].blue;
    }
    colorSeq[counter_total * 2 + 1].EOA = 1;
  }
  if(colormode == "smooth"){
    unsigned char steps = 10;
    unsigned short counter_total = 0;
    
    for(int i = 0; i < gradient_length - 1; i++){
      short stepsize_red = (gradient[i + 1].red - gradient[i].red) / steps;
      short stepsize_green = (gradient[i + 1].green - gradient[i].green) / steps;
      short stepsize_blue = (gradient[i + 1].blue - gradient[i].blue) / steps;
      for (int j = 0; j < steps; j++){
        colorSeq[counter_total].red = gradient[i].red + stepsize_red * j;
        colorSeq[counter_total].green = gradient[i].green + stepsize_green * j;
        colorSeq[counter_total].blue = gradient[i].blue + stepsize_blue * j;
        counter_total++;
      }
    }
    colorSeq[counter_total].red = gradient[gradient_length - 1].red;
    colorSeq[counter_total].green = gradient[gradient_length - 1].green;
    colorSeq[counter_total].blue = gradient[gradient_length - 1].blue;
    colorSeq[counter_total].EOA = 1;
  }
  if(colormode == "fade"){
    unsigned char steps = 10;
    unsigned short counter_total = 0;
    gradient_length = 4;
    gradient[0].red = 255; gradient[0].green = 0; gradient[0].blue = 0;
    gradient[1].red = 0; gradient[1].green = 0; gradient[1].blue = 255;
    gradient[2].red = 0; gradient[2].green = 255; gradient[2].blue = 0;
    gradient[3].red = 255; gradient[3].green = 0; gradient[3].blue = 0;
    
    for(int i = 0; i < gradient_length - 1; i++){
      short stepsize_red = (gradient[i + 1].red - gradient[i].red) / steps;
      short stepsize_green = (gradient[i + 1].green - gradient[i].green) / steps;
      short stepsize_blue = (gradient[i + 1].blue - gradient[i].blue) / steps;
      for (int j = 0; j < steps; j++){
        colorSeq[counter_total].red = gradient[i].red + stepsize_red * j;
        colorSeq[counter_total].green = gradient[i].green + stepsize_green * j;
        colorSeq[counter_total].blue = gradient[i].blue + stepsize_blue * j;
        counter_total++;
      }
    }
    colorSeq[counter_total].red = gradient[gradient_length - 1].red;
    colorSeq[counter_total].green = gradient[gradient_length - 1].green;
    colorSeq[counter_total].blue = gradient[gradient_length - 1].blue;
    colorSeq[counter_total].EOA = 1;
  }
  if(colormode == "off"){
    colorSeq[0].red = 0;
    colorSeq[0].green = 0;
    colorSeq[0].blue = 0;
    colorSeq[1].EOA = 1;
  }
  else
    return;
}

void convertRGB(COLOR* color, String cw){
  String tr = "00", tg = "00", tb ="00";
  for(int i = 0; i < 2; i++){
    tr[i] = cw[i+1];
    tg[i] = cw[i+3];
    tb[i] = cw[i+5];
  }
  color->red = hexToDec(tr);
  color->green = hexToDec(tg);
  color->blue = hexToDec(tb);
}

int hexToDec(String str){
  int num = 0, multi = 1;
  for (int i = 0; i < str.length(); i++){
    multi = 1;
    for(int j = str.length()-1; j > i; j--)
      multi *= 16;
    switch(str[i]){
      case 'a':
        num += 10 * multi;
      break;
      case 'b':
        num += 11 * multi;
      break;
      case 'c':
        num += 12 * multi;
      break;
      case 'd':
        num += 13 * multi;
      break;
      case 'e':
        num += 14 * multi;
      break;
      case 'f':
        num += 15 * multi;
      break;
      default:
        num += (int)(str[i] - 48) * multi;
    }
  }
  return num;
}

unsigned int toInteger(String str){
  int num = 0, multi = 1, digits = str.length();
  for (int i = 0; i < digits; i++){
    multi = 1;
    for(int j = digits-1; j > i; j--)
      multi *= 10;
    num += ((int)str[i]-48)* multi;
  }
  return num;
}
