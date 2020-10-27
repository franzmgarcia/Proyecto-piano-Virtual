import processing.serial.*; //Se incluye la libreria para utilizar el serial
import ddf.minim.analysis.*;
import ddf.minim.*;
import ddf.minim.signals.*;
import ddf.minim.ugens.*;

Minim minim;
Minim soundengine;
AudioSample sonido1;
AudioOutput out;
AudioRecorder recorder;
AudioInput in;
FilePlayer player;
PImage img,img2,img3,img4,img5,img6,img7,img8,img9;
boolean recorded;

Serial puerto;  //Se crea el objeto de clase serial
IntList info;  //Arreglo que almacena los datos de la senial
int x1, x2, y1, y2;  //Puntos para pintar la senal
boolean sinc = false;  //Variable para determinar si el buffer se lee sincronizadamente
int state = 0;  //Variable para la maquina de estados
byte aux1, aux2, aux3, aux4, aux5, aux6, aux7;  //Variables auxiliares para pasar los datos necesarios a la funcion de conversion
int result1;  //Resultado final de la conversion del primer analogico
int result2;  //Resultado final de la conversion del segundo analogico
float volumen;
byte do1, re, mi, fa, sol, la, si, do2;
byte do1p=0x00;
byte rep=0x00;
byte mip=0x00;
byte fap=0x00;
byte solp=0x00;
byte lap=0x00;
byte sip=0x00;
byte do2p=0x00;;
byte b1=0x01; //do1
byte b2=0x02; //re
byte b3=0x04; //mi
byte b4=0x08; //fa
byte b5=0x10; //sol 
byte b6=0x20; //la
byte b7=0x40; //si
byte b8=(byte)0x80; //do
boolean tamborp = true;
boolean grabar = false;
boolean parar = false;
boolean guardar = false;


void setup()
{
  int d = day();
  int m = month();
  int y = year();
  int h = hour();
  int minutes = minute();
  int second = second();
  
  size(1000, 700, P3D); //Tamaño de la pantalla 1000X700
 
  minim = new Minim(this); //Nuevo tipo minim
  
  in = minim.getLineIn(Minim.STEREO, 2048); //Variable de entrada 
  out = minim.getLineOut(Minim.STEREO); //Variable de salida

  
  recorder =minim.createRecorder(in,"Cancion "+y+"-"+m+"-"+d+" "+h+"."+minutes+"."+second+".wav"); //Variable para guardar el archivo cancion.wav
  soundengine = new Minim(this);
  sonido1 = soundengine.loadSample("tambor.mp3",1024);
  puerto = new Serial(this, Serial.list()[0], 115200);
  puerto.buffer(6);  //Se almacenaran 3 bytes en el buffer
  //info = new IntList();

  img=loadImage("record.png");
  img2=loadImage("recordb.png"); //Se carga la imagen para detener la grabacion
  img3=loadImage("botonguardar.png"); //Se carga la imagen para guardar la grabacion
  img6=loadImage("botonguardar2.png"); //Se carga la imagen para guardar la grabacion
  img7=loadImage("tambor.jpg");
  img8=loadImage("nota1.jpg");
  img9=loadImage("nota2.png");
}

void draw() {
  background(255); // Color blanco de fondo
  image(img7,375,75,200,200);
  fill(255); //Pintamos las notas blancas del teclado
  rect(100,380,100,300,0,0,10,10); //do Cambie todo a 380 en el segundo parametro
  rect(200,380,100,300,0,0,10,10); //re
  rect(300,380,100,300,0,0,10,10); //mi
  rect(400,380,100,300,0,0,10,10); //fa
  rect(500,380,100,300,0,0,10,10); //sol
  rect(600,380,100,300,0,0,10,10); //la
  rect(700,380,100,300,0,0,10,10); //si
  rect(800,380,100,300,0,0,10,10); //do
  
  fill(0); //Pintamos las notas negras del teclado
  rect(170,380,60,200,0); //do#
  rect(270,380,60,200,0); //re#
  rect(470,380,60,200,0); //fa#
  rect(570,380,60,200,0); //sol#
  rect(670,380,60,200,0); //la#  

/////
  fill(33); //156
  rect(0,680,1000,20,0,0,10,10);
  rect(0,380,100,300,0);
  rect(900,380,100,300,0);
  
  rect(0,250,1000,130,10,10,0,0);
//////

  fill(33);
  rect(0,0,950,50);
  
  fill(255,131,34);
  rect(950,0,50,50);

  if(grabar){
    image(img2,100,120,100,100);  //Indicamos la posicion de las imagenes para dar inicio, pausa y guardado de las grabaciones
  }
  
  else{
    image(img,100,120,100,100);
  }
  
  image(img3,800,120,75,75);
  
  if(do1>0 && result1>0){
    fill(255,0,0);
    rect(125,600,50,50,10);
  }
  
  if(re>0 && result1>0){
    fill(255,0,0);
    rect(225,600,50,50,10);
  }
  
  if(mi>0 && result1>0){
    fill(255,0,0);
    rect(325,600,50,50,10);
  }
  
  if(fa>0 && result1>0){
    fill(255,0,0);
    rect(425,600,50,50,10);
  }
  
  if(sol>0 && result1>0){
    fill(255,0,0);
    rect(525,600,50,50,10);
  }
  
  if(la>0 && result1>0){
    fill(255,0,0);
    rect(625,600,50,50,10);
  }
  
  if(si>0 && result1>0){
    fill(255,0,0);
    rect(725,600,50,50,10);
  }
  
  if(do2<0 && result1>0){
    fill(255,0,0);
    rect(825,600,50,50,10);
  }
  
  if(!tamborp){
    image(img8,575,75,40,40);
    image(img9,335,75,40,40);
  }

}  
///////
void serialEvent(Serial puerto) { 
  byte[] dato = new byte[6];  //Se crea una varibale tipo byte para guardar el dato del buffer
  dato = puerto.readBytes();  //Se lee y se guardan los datos
  for (int i = 0; i<6; i++) {  //Bucle para leer los datos que vienen del buffer
    if (dato[i] == -13) {  //Si se lee la cabecera (etiqueta)
      sinc = true;  //Entonces se esta sincronizado
    }
    if (sinc) {
      switch(state) {
      case 0:
        state = 1;
        break;
      case 1:
        aux1 = dato[i];  //Se guarda el segundo byte recibido
        state = 2;
        break;
      case 2:
        aux2 = dato[i];  //Se guarda el tercer byte recibido
        state = 3;
        break;
      case 3:
        aux3 = dato[i];  //Se guarda el cuarto byte recibido
        state = 4;
        break;
      case 4:
        aux4 = dato[i];  //Se guarda el quinto byte recibido
        state = 5;
        break;
      case 5:
        aux5 = dato[i];  //Se guarda el sexto byte recibido
        state = 6;
        break;
      case 6:
        result1 = analogconvertion(aux1, aux2);  //Se convierte
        result2 = analogconvertion(aux3, aux4);

        state = 0;     
        sinc = false;

        SineWave mySine;
        MyNote newNote;
      
        float pitch = 0; //Frecuencia inicial
        
        volumen = map(result1,0,4095,0,0.5);
          
        do1 = byte(aux5 & b1);
        if(do1>0 && do1p==0x00){
         pitch= 262;
         newNote = new MyNote(pitch, volumen);
        }
        do1p = do1;
        
        re=  byte(aux5 & b2);
        if(re>0 && rep==0){
         pitch=294;
         newNote = new MyNote(pitch, volumen);
        }
        rep = re;
        
        mi= byte(aux5 & b3);
        if(mi>0 && mip==0){
         pitch=330;
         newNote = new MyNote(pitch, volumen);
        }
        mip = mi;
        
        fa= byte(aux5 & b4);
        if(fa>0 && fap==0){
         pitch = 349;  
         newNote = new MyNote(pitch, volumen);
        } 
        fap = fa;
        
        sol= byte(aux5 & b5);
        if(sol>0 && solp==0){
         pitch=392;
         newNote = new MyNote(pitch, volumen);
        }
        solp =sol;
        
        la= byte(aux5 & b6);
        if(la>0 && lap==0){
         pitch=440;
         newNote = new MyNote(pitch, volumen);
        }
        lap = la;
        
        si= byte(aux5 & b7);
        if(si>0 && sip==0){
         pitch=494;
         newNote = new MyNote(pitch, volumen);
        }
        sip = si;
        
        do2= byte(aux5 & b8);
        if(do2<0 && do2p==0){
          pitch=523;
          newNote = new MyNote(pitch, volumen);
        }
        do2p = do2;
        
        if(result2 > 1000 && tamborp && result1 > 0){
          sonido1.trigger();
          tamborp=false;
        }
        else if(result2 <= 7 && !tamborp){
          tamborp=true;
        }
        
        break;
      }
    }
  }
}


int analogconvertion(byte auxi1, byte auxi2) {
  int r;
  int b, c;
  int auxi3, auxi4;
  b = (auxi1 & 0x1F); //Elimino los tres primeros bits, que corresponden al cero y los dos digitales
  c = (auxi2 << 1);  //Elimino el cero del principio y queda al final
  auxi3 = (b << 8);  //Para concatenar, se shiftea 8 veces a la izquierda, y quedan ocho ceros a la derecha
  auxi4 = c & 0x00FF; //Paso de 11111111c a 00000000c para concatenar 

  auxi4 = (auxi3 | auxi4);  //Se concatenan
  r = (auxi4 >> 1);  //Se elimina el ultimo cero

  return r;
}

///////

void mousePressed(){ //Programamos las interacciones con el mouse


if(!recorded && (mouseX >100 && mouseX<175)&&(mouseY>120 && mouseY<195)) //Si se da click en el boton para iniciar la grabacion
{
  grabar = true;
  int d = day();
  int m = month();
  int y = year();
  int h = hour();
  int minutes = minute();
  int second = second();
  
  if( recorder.isRecording() ) //Si ya se encuentra realizando una grabacion
  {
    grabar = false;
    recorder.endRecord(); //Finaliza la grabacion
    recorded = true;  
}  
  else{
    recorder =minim.createRecorder(in,"Cancion "+y+"-"+m+"-"+d+" "+h+"."+minutes+"."+second+".wav");
    recorder.beginRecord(); // Si no se encuentra grabando da inicio a la grabacion
       
}  
}


if ( recorded && (mouseX >800 && mouseX<875)&&(mouseY>120 && mouseY<195)) // Si se da click al boton de guardar
{
  image(img6,800,120,75,75);
  if (player != null)
  {
        player.unpatch( out ); 
    //    player.close();
    }
    player = new FilePlayer( recorder.save() ); // Se guarda la grabacion
    player.patch( out ); 
    player.play(); //Permite reproducir la grabacion
    
    recorder.save();
    recorded = false;

}  
  
}
////////


void stop()
{
  out.close(); //Para la reproduccion de sonido
  minim.stop();
 
  super.stop();
}

class MyNote implements AudioSignal
{
     private float freq; // Variable para la frecuencia 
     private float level; //Variable para el volumen
     private float alph; //Variable para la ganancia
     private SineWave sine;
     
     MyNote(float pitch, float amplitude)
     {
         freq = pitch;
         level = amplitude;
         sine = new SineWave(freq, level, out.sampleRate());
         alph = 0.9;
         out.addSignal(this); //reproduce la señal con esos niveles de frecuencia y amplitud
     }

     void updateLevel()
     {
         level = level * alph;
         sine.setAmp(level);
         
         if (level < 0.05) {
             out.removeSignal(this); //Se crea un filtro digital para las señales de ruido
         }
     }
     
     void generate(float [] samp)
     {
         sine.generate(samp);
         updateLevel();
     }
     
    void generate(float [] sampL, float [] sampR)
    {
        sine.generate(sampL, sampR);
        updateLevel();
    }

}
 


/////////
