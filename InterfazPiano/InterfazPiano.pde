import ddf.minim.analysis.*;
import ddf.minim.*;
import ddf.minim.signals.*;
import ddf.minim.ugens.*;
 
Minim minim;
AudioOutput out;
AudioRecorder recorder;
AudioInput in;
FilePlayer player;


color azul = color(0,0,255);
PImage img,img2,img3;
boolean notas [];
boolean recorded;

void setup()
{
  size(1000, 700, P3D); //Tamaño de la pantalla 1000X700
 
  minim = new Minim(this); //Nuevo tipo minim
  
  in = minim.getLineIn(Minim.STEREO, 2048); //Variable de entrada 
  out = minim.getLineOut(Minim.STEREO); //Variable de salida


  recorder =minim.createRecorder(in, "cancion.wav"); //Variable para guardar el archivo cancion.wav
  //guardar = minim.createRecorder(out,"cancion.wav",true);

  textFont(createFont("Arial",12)); //Fuente de texto

  background(255); // Color blanco de fondo
  
  img=loadImage("botonplay.jpg"); //Se carga la imagen para dar inicio a la grabacion
  img2=loadImage("botonstop.jpg"); //Se carga la imagen para detener la grabacion
  img3=loadImage("botonguardar.png"); //Se carga la imagen para guardar la grabacion
  
  fill(255); //Pintamos las notas blancas del teclado
  rect(100,400,100,300,10); //do
  rect(200,400,100,300,10); //re
  rect(300,400,100,300,10); //mi
  rect(400,400,100,300,10); //fa
  rect(500,400,100,300,10); //sol
  rect(600,400,100,300,10); //la
  rect(700,400,100,300,10); //si
  rect(800,400,100,300,10); //do
  
  
  fill(0); //Pintamos las notas negras del teclado
  rect(170,400,60,200,10); //do#
  rect(270,400,60,200,10); //re#
  rect(470,400,60,200,10); //fa#
  rect(570,400,60,200,10); //sol#
  rect(670,400,60,200,10); //la#  

  image(img,100,10,75,75);  //Indicamos la posicion de las imagenes para dar inicio, pausa y guardado de las grabaciones
  image(img2,200,10,75,75);
  image(img3,800,10,75,75);
}

void draw()
{
int i;

}

void mousePressed(){ //Programamos las interacciones con el mouse

if(!recorded && (mouseX >100 && mouseX<175)&&(mouseY>10 && mouseY<85)) //Si se da click en el boton para iniciar la grabacion
{
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
if ( recorded && (mouseX >200 && mouseX<275)&&(mouseY>10 && mouseY<85)) //Si se da click al boton de pausa
{
  recorder.endRecord(); //Finaliza la grabacion
  recorded = true;
}

if ( recorded && (mouseX >800 && mouseX<875)&&(mouseY>10 && mouseY<85)) // Si se da click al boton de guardar
{
  if (player != null)
  {
        player.unpatch( out ); 
        player.close();
    }
    player = new FilePlayer( recorder.save() ); // Se guarda la grabacion
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
    case 'z': pitch = 262; break; //Frecuencia do
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