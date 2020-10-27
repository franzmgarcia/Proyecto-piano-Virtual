import processing.serial.*; //Se incluye la libreria para utilizar el serial

Serial puerto;  //Se crea el objeto de clase serial
IntList info;  //Arreglo que almacena los datos de la senial
int x1,x2,y1,y2;  //Puntos para pintar la senal
boolean sinc = false;  //Variable para determinar si el buffer se lee sincronizadamente
int state = 0;  //Variable para la maquina de estados
byte aux1, aux2;
int oldresult = 0;
int result;

void setup(){
  size(840,620);  //Se ajusta el tamano de la pantalla
  background(255);  //Se pinta la pantalla de blanco
  puerto = new Serial(this, Serial.list()[0], 115200);
  puerto.buffer(3);  //Se almacenaran 3 bytes en el buffer
  info = new IntList();
  x1 = 0;  //Se inicializan las coordenadas
  x2 = 1;
  y1 = 620;
}

void draw(){
  if(info.size() > 40){  //Si ya se tienen los datos suficientes
  paint(info.remove(0));  //Se pintan
  }
}

void paint(int punto){
  y2 = punto;  //Se pinta por lineas
  if(x1 == 0){
    stroke(255);
  }
  else{
    stroke(0);
  }
  line(x1,y1,x2,y2);
  x1 = x1 +1;
  x2 = x2 +1;
  y1 = y2;
  if(x2 > 840){  //Si se supera el tamano de la pantalla
    background(255);  //Se pinta de blanco
    x1 = 0;  //Se empezara a pintar desde el principio de la pantalla otra vez
    x2 = 1;
    info.clear();
  }
}

void serialEvent(Serial puerto){
  byte[] dato = new byte[3];  //Se crea una varibale tipo byte para guardar el dato del buffer
  dato = puerto.readBytes();  //Se lee y se guardan los datos
  for(int i = 0; i<3; i++){
    if(dato[i] == -15){
      sinc = true;
    }
    if(sinc){
      switch(state){
        case 0:
          state = 1;
          break;
        case 1:
          aux1 = dato[i];
          state = 2;
          break;
         case 2:
           aux2 = dato[i];
           result = convertion(aux1, aux2);
           result = (int) map(result, 0, 4095, 620, 0);
           //oldresult = (int) map(oldresult, 0, 4095, 620, 0);
           //if((result > (oldresult + 50)) | (result < (oldresult - 50))){
             //info.append(result);  //Se almacenan los datos en el arreglo
           //}
           //else{
             //result = oldresult;
             info.append(result);  //Se almacenan los datos en el arreglo
           //}
          // oldresult = result;
           state = 0;           
           break;
      }
    }
  }
  if(info.size() == 20){
    println(info);
  }
}

int convertion(byte aux1, byte aux2){
  int r;
  int b,c;
  int aux3, aux4;
  b = (aux1 & 0x1F); //Elimino los tres primeros bits, que corresponden al cero y los dos digitales
  c = (aux2 << 1);  //Elimino el cero del principio y queda al final
  aux3 = (b << 8);  //Para concatenar, se shiftea 8 veces a la izquierda, y quedan ocho ceros a la derecha
  aux4 = c & 0x00FF; //Paso de 11111111c a 00000000c para concatenar 
  
  aux4 = (aux3 | aux4);  //Se concatenan
  r = (aux4 >> 1);  //Se elimina el ultimo cero
  
  return r;
}