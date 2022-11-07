int WIDTH = 800, HEIGHT = 600;  //Длинна, ширина

// буллы
bool CLICK = false;
bool FLAG_SPEED = true;

// константы при данным рарешением экрана
int BIG_RAD = HEIGHT/4; // 150
int BIG_X = BIG_RAD + WIDTH/8, BIG_Y = BIG_RAD + HEIGHT/15;

// динамические переменные
float ANG_SPEED = 1;//1.15

float MINI_RAD = 50; // 3:75 4:50 5:38.5 6:30 7:25 8:22.5 9:18.75 10:16.6 11:15 16:10
float DELTA_TIME = 0;
float BIG_ANGLE = 0, MINI_ANGLE = 0;
float TANGENT_X = 1, TANGENT_Y = 0; // X, Y = [-1;1]
float MINI_X = BIG_X + BIG_RAD - MINI_RAD, MINI_Y = BIG_Y;
float BIG_DLINA = 0; //длина пройденная окружностью
bool MAX_AXIS_X = true; // 0: 2pi; 1: 4pi
int FIND_RAD_MARK = 4; // изначально совпадает с FIND_RAD
int FIND_RAD = 4;

// константы
float TO_GRAD = 180/3.1415926535;
float TO_RAD = 3.1415926535/180;