import ddf.minim.analysis.*;
import ddf.minim.*;
import ddf.minim.signals.*;
import ddf.minim.ugens.*;
 
Minim minim;
Minim soundengine;  //
AudioSample sonido1; // 
AudioOutput out;
AudioRecorder recorder;
AudioInput in;
FilePlayer player;




color azul = color(0,0,255);
PImage img,img2,img3,img4,img5,img6;
boolean notas [];
boolean recorded;
boolean pintardo = false;

void setup()
{
  size(1000, 700, P3D); //Tamaño de la pantalla 1000X700
 
  minim = new Minim(this); //Nuevo tipo minim
  
  in = minim.getLineIn(Minim.STEREO, 2048); //Variable de entrada 
  out = minim.getLineOut(Minim.STEREO); //Variable de salida

  //highPass = new HighPass(this);

  recorder =minim.createRecorder(in, "cancion.wav"); //Variable para guardar el archivo cancion.wav
  //guardar = minim.createRecorder(out,"cancion.wav",true);
  soundengine = new Minim(this);
  sonido1 = soundengine.loadSample("tambor.mp3", 1024);
  //a = textFont(createFont("Arial",32)); //Fuente de texto 
  //fill(255,131,34);
  //text("Play",);

  background(255); // Color blanco de fondo
  
  img=loadImage("botonplay.jpg"); //Se carga la imagen para dar inicio a la grabacion
  img2=loadImage("botonstop.jpg"); //Se carga la imagen para detener la grabacion
  img3=loadImage("botonguardar.png"); //Se carga la imagen para guardar la grabacion
  img4=loadImage("botonplay2.jpg"); //Se carga la imagen para dar inicio a la grabacion
  img5=loadImage("botonstop2.jpg"); //Se carga la imagen para detener la grabacion
  img6=loadImage("botonguardar2.png"); //Se carga la imagen para guardar la grabacion
    
  
  
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

  image(img,100,280,75,75);  //Indicamos la posicion de las imagenes para dar inicio, pausa y guardado de las grabaciones
  image(img2,200,280,75,75);
  image(img3,800,280,75,75);
}

void draw()
{
  background(255); // Color blanco de fondo
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

  image(img,100,150,75,75);  //Indicamos la posicion de las imagenes para dar inicio, pausa y guardado de las grabaciones
  image(img2,200,150,75,75);
  image(img3,800,150,75,75);
  
  if(pintardo){
    fill(255,0,0);
    rect(125,600,50,50,10);
  }
  
}

void mousePressed(){ //Programamos las interacciones con el mouse


if(!recorded && (mouseX >100 && mouseX<175)&&(mouseY>100 && mouseY<175)) //Si se da click en el boton para iniciar la grabacion
{
  image(img4,100,100,75,75);
  if( recorder.isRecording() ) //Si ya se encuentra realizando una grabacion
  {
    
    recorder.endRecord(); //Finaliza la grabacion
    recorded = true; 
  }  
  else
  {
    recorder.beginRecord(); // Si no se encuentra grabando da inicio a la grabacion
  }  
}

if ( recorded && (mouseX >200 && mouseX<275)&&(mouseY>100 && mouseY<175)) //Si se da click al boton de pausa
{
  image(img5,200,100,75,75);
  recorder.endRecord(); //Finaliza la grabacion
  recorded = true;
}

if ( recorded && (mouseX >800 && mouseX<875)&&(mouseY>100 && mouseY<175)) // Si se da click al boton de guardar
{
  image(img6,800,100,75,75);
  if (player != null)
  {
        player.unpatch( out ); 
        player.close();
    }
    player = new FilePlayer(recorder.save()); // Se guarda la grabacion
    player.patch( out ); 
    player.play(); //Permite reproducir la grabacion
}  
}

void keyPressed()
{
  SineWave mySine;
  MyNote newNote;

  float pitch = 0; //Frecuencia inicial

  switch(key) {
    case 'z': pitch = 262;
              pintardo = true;
              break; //Frecuencia do
    case 's': pitch = 277; break; //Frecuencia do#
    case 'x': pitch = 294; break; //Frecuencia re
    case 'd': pitch = 311; break; //Frecuencia re#
    case 'c': pitch = 330; break; //Frecuencia mi
    case 'v': pitch = 349; break; //Frecuencia fa
    case 'g': pitch = 370; break; //Frecuencia fa#
    case 'b': pitch = 392; break; //Frecuencia sol
    case 'h': pitch = 415; break; //Frecuencia sol#
    case 'n': pitch = 440; break; //Frecuencia la
    case 'j': pitch = 466; break; //Frecuencia la#
    case 'm': pitch = 494; break; //Frecuencia si
    case ',': pitch = 523; break; //Frecuencia do
    case 't': sonido1.trigger(); break; 
  }
  println(pitch); //imprime la frecuencia que se esta tocando
  println(key); //Imprime la tecla que se esta tocando
   if (pitch > 0) {
      newNote = new MyNote(pitch, 0.5);
   }
  
  
   
}


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
