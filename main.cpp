#include <SFML/Graphics.hpp>  //Библиотека для отрисовки
#include <iostream>
#include "set.h"  //Подкючаем файл с настройками
#include <cmath>
#include <vector>
#include <chrono>

#define ANTI_ALIASING 8
#define PI 3.1415926535

using namespace std;
using namespace sf;
using namespace std::chrono;

RenderWindow window(VideoMode(WIDTH, HEIGHT), "Circles", Style::Default, ContextSettings(0,0,ANTI_ALIASING)); // грубо говоря совмещение двух строчек сверху 

vector < vector <float>> RED_DOT; // Положение красных точек [-1; 1] X:[0] Y:[1]
vector < vector <float>> SHADOW_DOT; // тень точки
vector < vector <float>> TANGENT_DOT; //Положение касательной X:[0] Y:[1] ANGLE:[2]

Font MAIN_FONT;//шрифт 

void Reset(){
	FLAG_SPEED = true;
	TANGENT_DOT.clear();	
    RED_DOT.clear(); 
	SHADOW_DOT.clear();
	BIG_ANGLE = 0;
}

void Update_const(){
	Reset();
	
	FLAG_SPEED = true;
	if(HEIGHT < WIDTH)
		BIG_RAD = HEIGHT/4;
	else
		BIG_RAD = WIDTH/4;
	BIG_X = BIG_RAD + WIDTH/8; BIG_Y = BIG_RAD + HEIGHT/15;
	MINI_X = BIG_X + BIG_RAD - MINI_RAD;
	MINI_Y = BIG_Y;
	MINI_ANGLE = 0;
	
	FIND_RAD_MARK = 4; // изначально совпадает с FIND_RAD
	FIND_RAD = 4;
}

void Update(){
	if(not FLAG_SPEED)	
		BIG_ANGLE += ANG_SPEED * DELTA_TIME * 0.001;
	
	TANGENT_X = cos(BIG_ANGLE); 
	TANGENT_Y = sin(BIG_ANGLE + PI); // пи для нормального представления графика на окруж
	
	MINI_X = BIG_X + (BIG_RAD - MINI_RAD) * TANGENT_X;
	MINI_Y = BIG_Y + (BIG_RAD - MINI_RAD) * TANGENT_Y;
	
	BIG_DLINA = BIG_RAD * BIG_ANGLE; // BIG_DLINA == mini_dlina
	MINI_ANGLE = BIG_DLINA / MINI_RAD; // высчитывание закреплённой точки
	
	if(not FLAG_SPEED){
		float x = MINI_X + MINI_RAD * cos(MINI_ANGLE), y = MINI_Y + MINI_RAD * sin(MINI_ANGLE);
		SHADOW_DOT.push_back(vector <float>());
		SHADOW_DOT[SHADOW_DOT.size()-1].push_back(x);
		SHADOW_DOT[SHADOW_DOT.size()-1].push_back(y);
		
		if(BIG_ANGLE < 4 * PI){
			TANGENT_DOT.push_back(vector <float>());
			TANGENT_DOT[TANGENT_DOT.size()-1].push_back(TANGENT_X);
			TANGENT_DOT[TANGENT_DOT.size()-1].push_back(TANGENT_Y);
			TANGENT_DOT[TANGENT_DOT.size()-1].push_back(BIG_ANGLE);
			
			RED_DOT.push_back(vector <float>());
			RED_DOT[RED_DOT.size()-1].push_back((y-BIG_Y)/BIG_RAD);
			RED_DOT[RED_DOT.size()-1].push_back((x-BIG_X)/BIG_RAD);
			//RED_DOT[RED_DOT.size()-1].push_back(sqrt((BIG_X - x)*(BIG_X - x) + (BIG_Y - y)*(BIG_Y - y))/BIG_RAD);
		}
	}
	//RED_DOT[RED_DOT.size()-1].push_back(sqrt()/BIG_RAD); 
	
	//cout<<x<<" "<<BIG_X<<endl;
}

void Line_XY(){
	RectangleShape line(Vector2f(10, 2.f));
	line.move(BIG_X + BIG_RAD + 40, BIG_Y + 2);  
	
 	line.rotate(150);
	line.setFillColor(Color::Black); 
	window.draw(line); 
	
	line.rotate(60);
	line.move(-1, 0);
	window.draw(line); 
	
	line.setPosition(BIG_X + 1, 11);
	line.rotate(-90);
	window.draw(line);
	
	line.rotate(-60);
	line.move(-1, -1);
	window.draw(line);
	
	//Font font; //шрифт 
	//font.loadFromFile("arkhip.ttf");
	
	Text text("x", MAIN_FONT, 20);
	text.setPosition(BIG_X + BIG_RAD + 40 - text.getGlobalBounds().width/2, BIG_Y); 
	window.draw(text);
	
	text.setString("y");
	text.setPosition(BIG_X + 15 - text.getGlobalBounds().width/2, 0); 
	window.draw(text);
}

void Connect_Dot(float x1, float y1, float x2, float y2, Color Cl){
	Vertex line_1[] =
	{
		Vertex(Vector2f(x1, y1)),  //Координата первой вершины
		Vertex(Vector2f(x2, y2))  //Координата второй вершины
	};
	
	line_1[0].color = Cl;
	line_1[1].color = Cl;
	
	window.draw(line_1, 2, Lines);  
}

void Connect_Dot_graf(float x1, float y1, float x2, float y2, Color Cl){
	Vertex line_1[] =
	{
		Vertex(Vector2f(x1, y1)),  
		Vertex(Vector2f(x2, y2)),
		Vertex(Vector2f(x2, y2+2)),  
		Vertex(Vector2f(x1, y1+2)),
	};
	
	line_1[0].color = Cl;
	line_1[1].color = Cl;
	line_1[2].color = Cl;
	line_1[3].color = Cl;
	
	window.draw(line_1, 4, Quads);  
}

void Render_axis(){
	int y_min = 5 * HEIGHT / 8, k_y = 3 * HEIGHT / 32, k_x = WIDTH / 4; 
	
	RectangleShape space(Vector2f(WIDTH, 3 * HEIGHT / 8));  // Создаем прямоугольник 		
	space.setPosition(0, 5 * HEIGHT / 8);  
	space.setFillColor(Color(170, 170, 170));  //Устанавливаем ему цвет
	window.draw(space);	
	
	RectangleShape graf_osi(Vector2f(WIDTH, 1));  // Создаем прямоугольник 		
	graf_osi.setPosition(0, y_min + k_y);  
	graf_osi.setFillColor(Color::Black);  //Устанавливаем ему цвет
	window.draw(graf_osi);
		
	for(int i=0; i<2; i++){
		graf_osi.move(0, k_y);
		window.draw(graf_osi);
	}
	
	RectangleShape vertical(Vector2f(1, 4 * k_y));  // Создаем прямоугольник 		
	vertical.setFillColor(Color::Black);  //Устанавливаем ему цвет
	
	vertical.setPosition(k_x, y_min);  
	window.draw(vertical);
	
	vertical.move(2 * k_x, 0);  
	window.draw(vertical);
	
	vertical.setScale(Vector2f(2, 2 * k_y));
	vertical.move(- k_x, 0);  
	window.draw(vertical);
	
	vertical.setPosition(WIDTH-1, y_min);  
	window.draw(vertical);
	
	vertical.setPosition(-1, y_min);  
	window.draw(vertical);
	
	Vertex line[] =
		{
			Vertex(Vector2f(WIDTH - 15, y_min + k_y - 5)),  //Координата первой вершины
			Vertex(Vector2f(WIDTH, y_min + k_y)),  //Координата второй вершины
		};
		
	line[0].color = Color::Black;
	line[1].color = Color::Black;
 
	window.draw(line, 2, Lines);
	
	
	line[0].position = Vector2f(WIDTH - 15, y_min + k_y + 5);
	window.draw(line, 2, Lines);
	
	line[0].position = Vector2f(WIDTH - 15, y_min + k_y*3 - 5);
	line[1].position = Vector2f(WIDTH, y_min + k_y * 3);
	window.draw(line, 2, Lines);
	
	line[0].position = Vector2f(WIDTH - 15, y_min + k_y*3 + 5);
	window.draw(line, 2, Lines);
	
	string txt[5];
	
	if(MAX_AXIS_X)	
		txt[0] = "0", txt[1] ="Pi", txt[2] = "2Pi", txt[3] = "3Pi", txt[4] = "4Pi";
	else
		txt[0] = "0", txt[1] ="Pi/2", txt[2] = "Pi", txt[3] = "3/2Pi", txt[4] = "2Pi";
		
		
	//Font font;//шрифт 
	//font.loadFromFile("arkhip.ttf");
	Text text(txt[0], MAIN_FONT, 20);
	
	//text.setColor(Color(255, 255, 255));
	
	text.setPosition(text.getGlobalBounds().width/2, y_min + 1.6 * k_y );
	window.draw(text);
	
	for(int i = 1; i < 4; i++){
		text.setString(txt[i]);
		text.move(k_x, 0);
		window.draw(text);
	}
	text.setString(txt[4]);
	text.move(k_x - 50, 0);
	window.draw(text);
	
	text.setString("Y");
	text.setPosition(WIDTH - 35, y_min + k_y - 25);
	window.draw(text);
	
	text.setString("X");
	text.setPosition(WIDTH - 35, y_min + 3 * k_y + 5);
	window.draw(text);
}

void Graph(){
	Render_axis();

	if(SHADOW_DOT.size() == 0)
		return;	
		
	//int j = 0;
	//if(3000 < SHADOW_DOT.size()) 
	//	j = SHADOW_DOT.size() - 3000;
	
	float k_x = WIDTH/((MAX_AXIS_X + 1) * 2 * PI), k_y = 3 * HEIGHT / 32;
	float last_sha_dot_x = SHADOW_DOT[0][0], last_sha_dot_y = SHADOW_DOT[0][1];

	float last_angel = TANGENT_DOT[0][2] * k_x - 1;
	float last_tan_dot_x = 29 * HEIGHT / 32 - k_y * TANGENT_DOT[0][0] - 1, last_tan_dot_y = 23 * HEIGHT / 32 + k_y * TANGENT_DOT[0][1] - 1;
	float last_red_dot_x = 23 * HEIGHT / 32 - k_y * RED_DOT[0][0] - 1, last_red_dot_y = 29 * HEIGHT / 32 - k_y * RED_DOT[0][1] - 1; 

	
	for(int i=1; i + 1 < SHADOW_DOT.size(); i++){ //
		//if(i + 3000 > SHADOW_DOT.size()){
		Connect_Dot(SHADOW_DOT[i][0], SHADOW_DOT[i][1], last_sha_dot_x, last_sha_dot_y, Color::Red);
		last_sha_dot_x = SHADOW_DOT[i][0];		last_sha_dot_y = SHADOW_DOT[i][1];
		//}
			
		if(i + 1 < TANGENT_DOT.size() and TANGENT_DOT[i][2] < (MAX_AXIS_X + 1)* 2 * PI){
			Connect_Dot_graf(TANGENT_DOT[i][2] * k_x - 1, 29 * HEIGHT / 32 - k_y * TANGENT_DOT[i][0] - 1, last_angel, last_tan_dot_x, Color::Blue);
			Connect_Dot_graf(TANGENT_DOT[i][2] * k_x - 1, 23 * HEIGHT / 32 + k_y * TANGENT_DOT[i][1] - 1, last_angel, last_tan_dot_y, Color::Green);
			Connect_Dot_graf(TANGENT_DOT[i][2] * k_x - 1, 23 * HEIGHT / 32 + k_y * RED_DOT[i][0] - 1, last_angel, last_red_dot_x, Color::Red);
			Connect_Dot_graf(TANGENT_DOT[i][2] * k_x - 1, 29 * HEIGHT / 32 - k_y * RED_DOT[i][1] - 1, last_angel, last_red_dot_y, Color(255, 190, 128));	
			
			//Connect_Dot_graf(TANGENT_DOT[i][2] * k_x - 1, 23 * HEIGHT / 32 - k_y * RED_DOT[i][1] - 1, last_angel, last_red_dot_y, Color(255, 190, 128));
			//Connect_Dot_graf(TANGENT_DOT[i][2] * k_x - 1, 29 * HEIGHT / 32 + k_y * RED_DOT[i][0] - 1, last_angel, last_red_dot_x, Color::Red);			
			
			last_tan_dot_x = 29 * HEIGHT / 32 - k_y * TANGENT_DOT[i][0] - 1;
			last_tan_dot_y = 23 * HEIGHT / 32 + k_y * TANGENT_DOT[i][1] - 1;
			last_red_dot_x = 23 * HEIGHT / 32 + k_y * RED_DOT[i][0] - 1; 
			last_red_dot_y = 29 * HEIGHT / 32 - k_y * RED_DOT[i][1] - 1;
			//last_red_dot_y = 23 * HEIGHT / 32 - k_y * RED_DOT[i][1] - 1; //замена x и y
			//last_red_dot_x = 29 * HEIGHT / 32 + k_y * RED_DOT[i][0] - 1;
			
			last_angel = TANGENT_DOT[i][2] * k_x - 1; 
			
			/*circle_green.setPosition(); 
			window.draw(circle_green);
			circle_black.setPosition(); 
			window.draw(circle_black); 
			circle_red.setPosition(TANGENT_DOT[i][2] * k_x - 1, 23 * HEIGHT / 32 - k_y * RED_DOT[i][0] - 1); 
			window.draw(circle_red); // Color::Red */
		} 
	}
}
string Int_to_string(int x){
	string text = "";
	
	if(x < 0)
		text = "-";
	
	int a, b;
	a = x;
	x = x - a;
	a = abs(a);

	if(a == 0)
		text = "0";
		
	while(a != 0){
		b = a % 10;
		char ch(48 + b);
		text = ch + text;
		a /= 10;
	}
	return text;
}

string Float_to_string(float x){

	string text = "", text2 = "";
	bool minus = false;
	
	if(x < -0.0001)
		minus = true;

	int a, b;

	a = abs(floor(x));
	if(x > -0.0001 and x < 0.0001)
		a = 0;
	x = abs(x) - a;
	if(x > 0.95)
		x = 0, a++;

	if(a == 0)
		text = "0";
		
	while(a != 0){
		b = a % 10;
		char ch(48 + b);
		text = ch + text;
		a /= 10;
	}
	text2 = "";
	
	a = round(abs(x * 100));
		
	bool flag = false;
	if(a < 10 and a != 0)
		flag = true;
	
	while(a != 0){
		b = a % 10;
		char ch(48 + b);
		text2 = ch + text2;
		a /= 10;
	}

	if(flag)
		text2 = "0" + text2;
	
	if(text2 != "")
		text += "." + text2;
	
	while(text[text.size()-1] == '0' and text.find(".") != -1)
		text = text.erase(text.size()-1, 1);
	
	if(minus)
		text = "-" + text;
	
	return text;
}

void Arrow_but(int x, int y, int a){
	RectangleShape rect(Vector2f(20, a));
	rect.setPosition(x-15, y-a);
	rect.setOutlineThickness(1);
	rect.setOutlineColor(Color::Black);
	rect.setFillColor(Color(190, 190, 190));
	window.draw(rect);
	
	rect.move(0, a);
	window.draw(rect);
	
	Vertex vert[] =
	{
	    Vertex(Vector2f( x - 12,   y - 2), Color::Black),
	    Vertex(Vector2f( x - 5, y - a + 2), Color::Black),
	    Vertex(Vector2f( x + 2, y - 2), Color::Black)
	};
	
	window.draw(vert, 3, Triangles);
	
	vert[0].position = Vector2f( x - 12,   y + 2);
	vert[1].position = Vector2f( x - 5,  y + a - 2);
	vert[2].position = Vector2f( x + 2,   y + 2);
	
	window.draw(vert, 3, Triangles);
}

void Switcher(int x, int y, int wid, int hig){
	RectangleShape rect(Vector2f(wid, hig));
	rect.setPosition(x, y);
	rect.setOutlineThickness(1);
	rect.setOutlineColor(Color::Black);
	rect.setFillColor(Color(190, 190, 190));
	window.draw(rect);
}

void Clicked_button(int x, int y){
	int devide_y =  HEIGHT / 12, devide_x = WIDTH * 3 / 4;
	
	if((devide_y * 2 - 13 < y and y <devide_y * 2 + 13) and (devide_x + 95 <= x and x <= devide_x + 110)){
		if (y <devide_y * 2){
			if(abs(MINI_RAD) - abs(int(MINI_RAD)) == 0)
						MINI_RAD++;
					else
						MINI_RAD = ceil(MINI_RAD);
			Reset();
		}
		else{
			if(abs(MINI_RAD) - abs(int(MINI_RAD)) == 0)
						MINI_RAD--;
					else
						MINI_RAD = floor(MINI_RAD);
			Reset();
		}
		return;
	}
	
	if((devide_y * 3 - 13 < y and y <devide_y * 3 + 13) and (devide_x + 95 <= x and x <= devide_x + 110)){
		if (y <devide_y * 3)
			FIND_RAD++;
		else
			if(FIND_RAD != 2)
				FIND_RAD--;
		return;
	}
	if((devide_y * 3 - 12 < y and y <devide_y * 3 + 12) and (devide_x + 113 <= x and x <= devide_x + 127) and FIND_RAD_MARK != FIND_RAD){
		FIND_RAD_MARK = FIND_RAD;
		MINI_RAD = BIG_RAD;
		MINI_RAD = MINI_RAD/(FIND_RAD-1);
		Reset();
	}
		
	if((devide_y * 4 - 13 < y and y <devide_y * 4 + 13) and (devide_x + 95 <= x and x <= devide_x + 110)){
		if (y <devide_y * 4)
			ANG_SPEED += 0.05;
		else
			if(ANG_SPEED > 0.02)
				ANG_SPEED -= 0.05;
		return;
	}
	if((devide_y * 5 - 13 < y and y <devide_y * 5 + 13) and (devide_x - 105 <= x and x <= devide_x + 110))
		FLAG_SPEED = not FLAG_SPEED;
		
	if((devide_y * 6 - 13 < y and y <devide_y * 6 + 13) and (devide_x - 105 <= x and x <= devide_x + 110))
		Reset();
			
	if((devide_y * 7 < y and y <devide_y * 7 + 18) and (devide_x - 14 <= x and x <= devide_x + 16))
		MAX_AXIS_X = not MAX_AXIS_X;
	//cout<<devide_y * 2 - 13<<" "<<y<<" "<<devide_y * 2<<endl;
	//cout<<devide_x + 95<<" "<<x<<" "<<devide_x + 110<<endl;
}

void Button(){
	int area_y = HEIGHT / 2, devide_y =  HEIGHT / 12, devide_x = WIDTH * 3 / 4; // y = 105 (общая длина примерно 210) 
	
	RectangleShape space(Vector2f(210, area_y - devide_y + 13));
	space.setPosition(devide_x - 105, devide_y - 13);
	space.setFillColor(Color(0, 137, 207));
	window.draw(space);	
	
	RectangleShape field(Vector2f(65, 26));  // Создаем прямоугольник + 5 т.к. кнопки вверх вниз
	field.setPosition(devide_x + 45, -13);   
	
	for(int i = 0; i < 4; i++){
		field.move(0, devide_y);
		window.draw(field);	
	}
	
	field.setSize(Vector2f(210, 26));
	field.setPosition(devide_x - 105, devide_y * 5 - 13);
	window.draw(field);	
	
	field.move(0, devide_y);
	window.draw(field);
		
	string txt_static[4] = {"Big Radius", "Small Radius", "Find radius", "Speed angle"}; //
	string txt_dynamic[4] = {Int_to_string(BIG_RAD), Float_to_string(MINI_RAD), Int_to_string(FIND_RAD), Float_to_string(ANG_SPEED)}; //
	
	Text text_static(txt_static[0], MAIN_FONT, 16);
	Text text_dynamic(txt_dynamic[0], MAIN_FONT, 16);
	
	text_static.setPosition(devide_x - 105, -10);
	text_dynamic.setPosition(devide_x + 50, -10);
	text_dynamic.setFillColor(Color::Black);
	
	for(int i = 0; i < 4; i++){
		text_static.setString(txt_static[i]);
		text_static.move(0, devide_y);
		window.draw(text_static);
		
		text_dynamic.setString(txt_dynamic[i]);
		text_dynamic.move(0, devide_y);
		window.draw(text_dynamic);
		
		if(i != 0)
			Arrow_but(devide_x + 105, (i+1) * devide_y, 13);
		
	}	
	
	if(FLAG_SPEED)
		text_dynamic.setString("Start"), text_dynamic.setPosition(devide_x - text_dynamic.getGlobalBounds().width/2 - 0.5, devide_y * 5 - 10);
	else
		text_dynamic.setString("Pause"), text_dynamic.setPosition(devide_x - text_dynamic.getGlobalBounds().width/2, devide_y * 5 - 10);
		
	window.draw(text_dynamic);
	
	text_dynamic.setString("Reset");
		
	text_dynamic.setPosition(devide_x - text_dynamic.getGlobalBounds().width/2, devide_y * 6 - 10);	
	window.draw(text_dynamic);
	
	text_static.setString("Max");
	text_static.setPosition(devide_x - 105, devide_y * 7);
	window.draw(text_static);
	
	text_static.setString("2Pi");
	text_static.move(55, 0);
	window.draw(text_static);
	
	text_static.setString("4Pi");
	text_static.move(70, 0);
	window.draw(text_static);
	
	Switcher(devide_x - 14, devide_y * 7, 30, 18);
	
	CircleShape circle(2.f);  
	circle.setFillColor(Color::White); 
	
	if(MAX_AXIS_X)
		circle.setPosition(devide_x + 9, devide_y * 7 + 8);
	else
		circle.setPosition(devide_x - 11, devide_y * 7 + 8);
		
	window.draw(circle);  //Отрисовка круга
	
		
	if(FIND_RAD != FIND_RAD_MARK){
		text_static.setString("V");
		text_static.setPosition(devide_x + 115, devide_y * 3 - 10);
		window.draw(text_static);
	}
		
	text_static.setString("(for n-angel shape)");
	text_static.setPosition(devide_x - 105, devide_y * 3.5 - 10);
	text_static.setFillColor(Color(190, 190, 190));
	window.draw(text_static);
}

void Otris(){
	// DELTA_TIME * 0.001
	Update();
	
	//XY
	RectangleShape line(Vector2f(BIG_RAD * 2 + 80, 2.f));
	//line_with_thickness.rotate(); 
	line.move(WIDTH/8 - 40, BIG_Y);  
 
	line.setFillColor(Color::Black); 
	window.draw(line); 
	
	line.rotate(90); 
	line.setPosition(BIG_X + 1, 10);
	window.draw(line); 
	
	Line_XY();
	
	// большой круг
	CircleShape circle(BIG_RAD, 60);  //круг радиусом 300, кол-во углов 60
	circle.setFillColor(Color(255, 255, 255, 32));  
		
	circle.setOutlineThickness(2.f);  //Устанавливаем толщину контура круга
	circle.setOutlineColor(Color::Black);  
			
	circle.move(WIDTH/8, HEIGHT/15); 
		
	window.draw(circle);  
	
	// Маленький круг
	circle.setRadius(MINI_RAD);
	circle.setPosition(MINI_X - MINI_RAD, MINI_Y - MINI_RAD); 
	window.draw(circle); 
	
	//Касательная	
	Vertex line_1[] =
		{
			Vertex(Vector2f(BIG_X, BIG_Y)),  //Координата первой вершины
			Vertex(Vector2f(MINI_X, MINI_Y))  //Координата второй вершины
			//Vertex(Vector2f(BIG_X + BIG_RAD * TANGENT_X, BIG_Y + BIG_RAD * TANGENT_Y))  //Координата второй вершины
		};
	
	window.draw(line_1, 2, Lines);  
	
	Vertex line_2[] =
		{
			Vertex(Vector2f(MINI_X, MINI_Y)),  //Координата первой вершины
			Vertex(Vector2f(MINI_X + MINI_RAD * cos(MINI_ANGLE), MINI_Y + MINI_RAD * sin(MINI_ANGLE)))  //Координата второй вершины
		};
	
	line_2[0].color = Color::White;
	line_2[1].color = Color::Red;
	
	window.draw(line_2, 2, Lines); 
	Graph();
	
	Button();
}

int main()
{	
	//cout<<"Руслан - лучший друг"<<endl;
	
	MAIN_FONT.loadFromFile("arkhip.ttf");
	//window.setFramerateLimit(120);
	//window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(true); //-------------------------против спама---------------
	
	
	Texture logo_texture;//создаем объект Texture (текстура)
	logo_texture.loadFromFile("logo.png");
	
	Sprite logo_sprite;
	logo_sprite.setTexture(logo_texture); 
	logo_sprite.setPosition(5, HEIGHT*5/8 - 105);
	
	high_resolution_clock::time_point LAST_TIME = high_resolution_clock::now();
	high_resolution_clock::time_point NOW_TIME;
	
	while (window.isOpen())  //Главный цикл приложения. Выполняется, пока открыто окно
	{	
		NOW_TIME = high_resolution_clock::now();
		//ContextSettings settings;
		Event event;
		while (window.pollEvent(event))  //Если произошло событие
		{
			if (event.type == Event::Closed)  //Если событие - закртыие окна
				window.close();  //Закрываем окно
			if (event.type == Event::Resized)
			{
	            WIDTH = event.size.width;
	            HEIGHT = event.size.height;    
				
	            window.setView(
	                View(
	                   Vector2f(WIDTH / 2, HEIGHT / 2), 
	                   Vector2f(WIDTH, HEIGHT)
	                )
	            );   
				Update_const(); 
				logo_sprite.setPosition(5, HEIGHT*5/8 - 105);
			}
			if (event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::R)
                	Reset();
				if(event.key.code == sf::Keyboard::E)	
					FLAG_SPEED = not FLAG_SPEED;
				if(event.key.code == sf::Keyboard::S and ANG_SPEED > 0.02)
					ANG_SPEED -= 0.05;
				if(event.key.code == sf::Keyboard::W)
					ANG_SPEED += 0.05;
				if(event.key.code == sf::Keyboard::Z){
					if(abs(MINI_RAD) - abs(int(MINI_RAD)) == 0)
						MINI_RAD++;
					else
						MINI_RAD = ceil(MINI_RAD);
					Reset();
				}
				if(event.key.code == sf::Keyboard::X){
					if(abs(MINI_RAD) - abs(int(MINI_RAD)) == 0)
						MINI_RAD--;
					else
						MINI_RAD = floor(MINI_RAD);
					Reset();
				}
            }
			//if (event.type == sf::Event::MouseMoved)FLAG_SPEED
				
			
			if (event.mouseButton.button == sf::Mouse::Left)
			{
			    int x, y;	
			    x = event.mouseButton.x;
				y = event.mouseButton.y;
				
				if(0 < x and x < WIDTH)
					CLICK = not CLICK;
				else
					CLICK = false;
				
				if(CLICK)
					Clicked_button(x, y);
			}
		}

		window.clear(Color(0, 137, 207)); 
		
		window.draw(logo_sprite);
		
		Otris();
		
		window.display();  //Отрисовка окна	
		
		duration<double, milli> time_span = NOW_TIME - LAST_TIME; // время между кадрами
		DELTA_TIME = time_span.count();
		
		if(DELTA_TIME > 500)
			DELTA_TIME = 500;
			
		LAST_TIME = NOW_TIME;
	}
	return 0;
}