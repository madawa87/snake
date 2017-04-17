/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>

// Maximum number of particles
int max_particles = 80;
int tailp = 100;


typedef struct particle{
  float x[120];
  float y[120];
} Particle_t;

// normal food structure
typedef struct nFood{
  // size of the food
  float fsize;
  float x;
  float y;

  // whether its eaten or not
  int eaten;
} NFood_t;

// special food structure
typedef struct sFood{
  // size of the food
  float fsize;
  float x;
  float y;
  // whether its eaten or not
  int eaten;
  // time span of the food
  float lifeSpan;
  // spawn time 
  float spawnTime;
} SFood_t;

// snake structure
typedef struct snake{
  // length of the snake
  int len;
  // travelling direction
  int dir;
  // newDir =1 if changed direction and didn't move it yet
  int newDir;
  // x and y coords of snake's body
  float x[1000];
  float y[1000];
} Snake_t;

// the game structure
typedef struct game{

  NFood_t nFood;
  SFood_t sFood;
  Snake_t snake;
  float x[120];
  float y[120];

  // thickness of a block
  float dr;
  // timestep size
  float dt;
  // score
  int score;
  // number of tiles in each direcion
  int nTiles;
} Game_t;

Game_t *game;

Particle_t *pSet;


// delta T and T
float dt = 0.3;
float T = 0.0;

// region of particles are genarated
float x_1 = 0.2;
float y_1 = 0.6;
float x_2 = 0.4;
float y_2 = 0.3;

void key_w(){dt = dt*2;}
void key_s(){dt = dt/2;}

void keyPress( unsigned char key, int x, int y ){
  switch (key){
  case 'W':
  case 'w':
    key_w();
    break;
  case 'S':
  case 's':
    key_s();
    break;
  case 27:
    exit(1);
  }
}

void specialKey(int key, int x, int y){
  switch (key){
  case GLUT_KEY_UP:
    if (game->snake.dir != 3) {      
      game->snake.dir = 1;
      game->snake.newDir = 1;
    }
    break;
  case GLUT_KEY_DOWN:
    if (game->snake.dir != 1){
      game->snake.newDir = 1;
      game->snake.dir = 3;
    }
    break;
  case GLUT_KEY_RIGHT:
    if (game->snake.dir != 0) {
      game->snake.newDir = 1; 
      game->snake.dir = 2;
    }
    break;
  case GLUT_KEY_LEFT:
    if (game->snake.dir != 2) {
      game->snake.newDir = 1;
      game->snake.dir = 0;
    }
    break;
  }
}

float xVectorF(float x, float y){
  //return ((0.7-x)/(2+T));
  //return sin(2*T-0.5) +(0.3 - x);
  //return sin(2*T-0.5);
  return 2*(0.5 - y);
}

float yVectorF(float x, float y){
  //return (T*(T -y)/ 2);
  //return cos(7*T) + (0.5 - y);
  //return cos(7*T)-4*(pow((x-0.5), 2.0));
  return cos(T)*(x-0.5);
}

// creat a peice of food at a random location fo
//void createParticle(int i){
  void createParticle(int i){
  int j;
  float x = x_1 + (x_2-x_1)*((float)(rand()%100)) / 100.0;
  float y = y_2 + (y_1-y_2)*((float)(rand()%100)) / 100.0;
  for (j=0; j<tailp; j++){
    pSet[i].x[j] = x;
    pSet[i].y[j] = y;
  }
}


void initParticles(){
  int i, j;
  srand(time(NULL));
  pSet = (Particle_t *) malloc(max_particles*sizeof(Particle_t));
  for (i=0; i<max_particles; i++){
    createParticle(i);
  }
}

// wrap coordinates around periodic boundary
void wrapPB(float* coord) {
  if(*coord<0.0)
    *coord = 1.0 + *coord;

  if(*coord>=1.0)
    *coord = *coord - 1.0;
}

// move snake by one dr 
void moveSnake() {
  int i;
  Game_t* g = game;
  float vx = g->snake.x[1]-g->snake.x[0];
  float vy = g->snake.y[1]-g->snake.y[0];
  int dir = g->snake.dir;

  // update tiles of snake except his head (1st element in the position arrays)
  for(i=g->snake.len-1; i>0; i--) {
    g->snake.x[i] = g->snake.x[i-1];
    g->snake.y[i] = g->snake.y[i-1];
  }
  
  // if facing left
  if(dir == 0)
    g->snake.x[0] = g->snake.x[0] - g->dr;
  
  // if facing right
  if(dir == 2)
      g->snake.x[0] = g->snake.x[0] + g->dr;
  
  // if facing down
  if(dir == 3)
      g->snake.y[0] = g->snake.y[0] - g->dr;

  // if facing up
  if(dir == 1)
    g->snake.y[0] = g->snake.y[0] + g->dr;

  wrapPB(&g->snake.x[0]);
  wrapPB(&g->snake.y[0]);
  if((g->snake.x[0] == g->nFood.x) && (g->snake.y[0] == g->nFood.y)) {
    //g->snake.len++;
    g->nFood.eaten ==1;
    printf("came here\n");
  }
  printf ("snake :: x[0],y[0] = %f, %f\n", g->snake.x[0],g->snake.y[0]);
}

void drawNFood() {
  int i, j;
  float dr = game->dr;
  glColor3f(0.0, 0.940, 0.30);
  
  float x,y;
  if(game->nFood.eaten) {
    game->nFood.x = ((float)(rand()%game->nTiles)) * game->dr;
    game->nFood.y = ((float)(rand()%game->nTiles)) * game->dr;
  }
  
  x = game->nFood.x;
  y = game->nFood.y;
  printf("x = %f y=%f\n",x,y);
  
  glBegin(GL_POLYGON);
  glVertex3f(x, y, 0.0);
  glVertex3f(x+dr, y, 0.0);
  glVertex3f(x+dr, y+dr, 0.0);
  glVertex3f(x, y+dr, 0.0);
  glEnd();
}

void drawSFood() {
  int i, j;
  float dr = game->dr;
  int size = 2;
  glColor3f(0.0, 0.940, 0.30);
  
  float x = game->sFood.x = ((float)(rand()%game->nTiles)) * game->dr;
  float y = game->sFood.y = ((float)(rand()%game->nTiles)) * game->dr;

  glBegin(GL_POLYGON);
  glVertex3f(x, y, 0.0);
  glVertex3f(x+dr*size, y, 0.0);
  glVertex3f(x+dr*size, y+dr*size, 0.0);
  glVertex3f(x, y+dr*size, 0.0);
  glEnd();
}

void drawSnake() {
  int i, j;
  Game_t* g = game;
  float dr = g->dr;
  
  glColor3f(0.780, 0.0, 0.530);
  for(i=0;i<g->snake.len;i++) {
    glBegin(GL_POLYGON);
    glVertex3f(g->snake.x[i], g->snake.y[i], 0.0);
    glVertex3f(g->snake.x[i]+dr, g->snake.y[i], 0.0);
    glVertex3f(g->snake.x[i]+dr, g->snake.y[i]+dr, 0.0);
    glVertex3f(g->snake.x[i], g->snake.y[i]+dr, 0.0);
    glEnd();
  }

}

void drawGrid(){

  int i,j, gridsize = 15;
  float veclen = 0.02f;

  glColor3f(0.10, 0.40, 0.60);
  glBegin(GL_LINES);
  for (i=1; i<gridsize; i++){
    float x = ((float)(i))/gridsize;
    for (j=1; j<gridsize; j++){
      float y = ((float)(j))/gridsize;
      glVertex3f(x, y, 0.0);
      float xv = xVectorF(x, y);
      float yv = yVectorF(x, y);
      float d = sqrt(pow(xv, 2.0f) + pow(yv, 2.0f));
      float xx = x + veclen*xv;
      float yy = y + veclen*yv;
      glVertex3f(xx, yy, 0.0);
    }
  }
  glEnd();

  glColor3f(1.0, 0.1, 0.1);
  glBegin(GL_POINTS);
  for (i=1; i<gridsize; i++){
    float x = ((float)(i))/gridsize;
    for (j=1; j<gridsize; j++){
      float y = ((float)(j))/gridsize;
      glVertex3f(x, y, 0.0);
    }
  }
  glEnd();

}

void Draw() {
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0.888, 0.999, 0.909);
  glBegin(GL_POLYGON);
  glVertex3f(x_1, y_1, 0.0);
  glVertex3f(x_2, y_1, 0.0);
  glVertex3f(x_2, y_2, 0.0);
  glVertex3f(x_1, y_2, 0.0);
  glEnd();

  //drawGrid();
  drawNFood();
  drawSnake();
  glutSwapBuffers();

  // update gamestep
  moveSnake();
  
  T = T + dt;
  printf("time :: %f\n", T);
}

void Timer(){
  glutPostRedisplay();
  glutTimerFunc(300, Timer, 0);
}

void Initialize() {
  glClearColor(1.0, 1.0, 1.0, 0.0);
  //glClearColor(0.678, 0.679, 0.684, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
  glPointSize(3);
  glLineWidth(2);
}

// initialize game. 
int initGame() {
  int i;
  
  game = (Game_t*) malloc(sizeof(Game_t));

  // set no. of game tiles and tile-size 
  game->nTiles = 50;
  game->dr = 1.0/((float) game->nTiles);

  // set values for normal food
  game->nFood.fsize = 0.02;
  game->nFood.x = ((float)(rand()%game->nTiles)) * game->dr;
  game->nFood.y = ((float)(rand()%game->nTiles)) * game->dr;
  game->nFood.eaten = 0;

  // set values for normal food
  game->sFood.fsize = 0.04;
  game->sFood.eaten = 0;
  game->sFood.lifeSpan = 10.0;

  // set snake's properties and position
  game->snake.len = 10.0;
  game->snake.dir = 0;
  game->snake.x[0] = ((float)(rand()%game->nTiles)) * game->dr;
  game->snake.y[0] = ((float)(rand()%game->nTiles)) * game->dr;

  for (i=1;i<game->snake.len;i++) {
    game->snake.x[i] = game->snake.x[i-1] +game->dr;
    game->snake.y[i] = game->snake.y[i-1];
    printf ("snake :: x[%d],y[%d] = %f, %f\n",i,i, game->snake.x[i],game->snake.y[i]);
  }

  game->dt = 1.0;
  game->score = 0;
}

int main(int argc, char** argv) {

  initParticles();
  //initialize game
  initGame();
    

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(640, 640);
  glutInitWindowPosition(200, 200);
  glutCreateWindow("The Snake...");
  Initialize();
  // Set up callback functions for key presses
  glutKeyboardFunc( keyPress );
  glutSpecialFunc(specialKey);

  glutDisplayFunc(Draw);
  Timer();
  glutMainLoop();
  return 0;
}
