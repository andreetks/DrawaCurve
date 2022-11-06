/*
Nombre: Gonzalo Andree Garcia Mugaburu
Fecha: 19/01/2022
Hora: 8:40pm
Tema: Practica Calificada 3
*/

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

GLfloat a=1.0,b=0.0,c=0.0;// variables que se usan para el color de la curva
int modelo=0;
/*  modelo 0: vacio
    modelo 1: Graficar Puntos
    modelo 2: Graficar curva
*/
int modo=0;
/*  modo 0: No hacer nada
    modo 1: Ingresar los puntos
    modo 2: Mover los puntos ingresados
*/
//booleanos para mostrar o no (curva/punto)
bool show_curve =false;
bool show_point=true;
float grosor_curva=1;

struct point{
    float x;
    float y;
};//creando una estructura para almacenar los puntos
vector<point> points;//creo un vector de puntos llamado points


int findNearPointNum(float x, float y) {
	if (points.size() == 0) {//si no hay ningun punto retorna sin ejecutar calculo
		return -1;
	}
	int near = 0;//usado para retornar la posicion del vector mas cercano
	double min = -1;
	for (unsigned int i = 0; i < points.size(); i++) {//recorrido del vector
        //calculo de la distancia
		double distence = (points[i].x - x)*(points[i].x - x) + (points[i].y - y)*(points[i].y - y);
		if (min == -1) {
			min = distence;//si es la primera iteracion se almacena la primera distancia conocida
		}
		else if (min > distence) { //comparando las distancia minima almacenada con la nueva calculada
			min = distence;
			near = i;
		}
	}
	return near;//retorna la posicion del punto mas cercano
}


void points_paint(){//imprimir los puntos de control

    glClearColor (1.0, 1.0, 1.0, 0.0);
    glColor3f(0,0,1);
    glPointSize(4.0);
    glBegin(GL_POINTS);
    for(unsigned int i=0;i<points.size();i++){//recorrido por el vector
        glVertex3f(points[i].x,points[i].y,0);//crea un vertice en cada coordenada dada por el vector
    }
    glEnd();

}

void curve_paint(float x,float y){//imprimir los puntos pertenecientes a la curva

    glPointSize(grosor_curva);
    glBegin(GL_POINTS);
    glVertex3f(x,y,0);
    glEnd();

}


void points_move(int draggedNum, float x, float y){
    //recibe la posicion del vector y las nuevas coordenadas para actualizarlo
    points[draggedNum].x = x;
    points[draggedNum].y = y;
}

void Spline(){
    if(points.size()<=3){//verificar si existen mas de 3 puntos para graficar la curva
        cout<<"No existen suficientes puntos"<<endl;
        return;
    }
    int nroite = points.size()-3;
    //nroite almacena cuantas veces el bucle tiene que calcular los puntos de la curva
    //ej si el vector tiene 4 puntos nroite = 1, entonces el bucle solo calculara la curva para p0,p1,p2,p3
    //si el vector tiene 5, nroite=2, entonces el bucle funciona dos veces, para p0,p1,p2,p3 y para p1,p2,p3,p4

    for(int i=0;i<nroite;i++){
        for(float t=0;t<=1;t+=0.001){
            float p0=points[i].x,p1=points[i+1].x,p2=points[i+2].x,p3=points[i+3].x;
            float y0=points[i].y, y1=points[i+1].y, y2=points[i+2].y, y3=points[i+3].y;

            //calcular los ejes para cada 4 puntos de control
            float x_axis = 0.5*((2*p1)+ ((-p0 + p2)*t)+
                            ((2*p0 - 5*p1 + 4*p2 - p3)*t*t)+
                            ((-p0 + 3*p1 - 3*p2+ p3)*t*t*t));
            float y_axis = 0.5*((2*y1)+ ((-y0 + y2)*t)+
                            ((2*y0 - 5*y1 + 4*y2 - y3)*t*t)+
                            ((-y0 + 3*y1 - 3*y2+ y3)*t*t*t));
            //pintar la curva
            curve_paint(x_axis,y_axis);

    }
    }

}



void mouse(int button,int state, int x, int y){
    //regularizar x y para su correspondencia en la pantalla
    float x_reg=float(x-300)/100,y_reg = float(300-y)/100;
    cout << fixed;
    cout << setprecision(2);
    cout << x_reg<<" "<<y_reg << endl;//muestra las coordenadas capturadas

    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && modo == 1){//ingresar puntos
        point t{x_reg,y_reg};//creo una estructura de tipo punto para agregarla al vector
        points.push_back(t);//funcion de vectores para agregar un elemento al final del vector
    }
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && modo == 2){//modificar puntos
        int draggedNum = findNearPointNum(x_reg, y_reg);//almacena la posicion en el vector del punto mas cercano
        points_move(draggedNum,x_reg,y_reg);

    }

}


void iniciar(void)
{
 glClearColor (1.0, 1.0, 1.0, 0.0);
 glColor3f (1.0, 0.0, 0.0);
 glLineWidth(2.0);
 glShadeModel (GL_FLAT);
}

void display(void)
{

 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 glLoadIdentity();
 gluLookAt(0,0.0,5.21,0,0.0,0,0,1,0);
 glClearColor (1.0, 1.0, 1.0, 0.0);
 glColor3f (1.0, 0.0, 0.0);

 glColor3f(a,b,c);
 switch(modelo){

 case 1: if(show_point){
            points_paint();
        };
        break;
 case 2: if(show_curve){Spline();if(show_point){points_paint();};}break;

 }
 glFlush();
 glutSwapBuffers();
}



void myReshape(int w, int h)
{
 glViewport (0, 0, (GLsizei) w, (GLsizei) h);
 glMatrixMode (GL_PROJECTION);
 glLoadIdentity ();
 gluPerspective(60.0, (GLfloat) w/(GLfloat) h,0.1, 30.0);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
}

//funciones menus

void menu_insertar(int opcion){
    switch(opcion){
    case 1:modo=1;modelo=1;break;
    case 2:modo=0;modelo=0;break;
    }
}

void menu_graficar_punto(int opcion){
    switch(opcion){
    case 1: show_point=true; modelo=1;break;
    case 2: show_point=false; break;
    }
}

void menu_graficar_curva(int opcion){
    switch(opcion){
    case 1: show_curve=true;modelo =2; break;
    case 2:show_curve=false; break;
    }
}


void menu_color(int opcion){
    switch(opcion){
    case 1: a=1,b=0,c=0; break;
    case 2: a=0,b=1,c=0; break;
    case 3: a=0,b=0,c=1; break;
    case 4: {cout << (float(rand())/float((RAND_MAX)) * 1.0) << endl;
            a=(float(rand())/float((RAND_MAX)) * 1.0),//elige numeros decimales aleatorios del 0.0 hasta el 1.0
            b=(float(rand())/float((RAND_MAX)) * 1.0),
            c=(float(rand())/float((RAND_MAX)) * 1.0);
            break;}
    case 5: {cout<<"Ingrese el valor para rojo (0-1)"<<endl;cin>>a;
            cout<<"Ingrese el valor para verde (0-1)"<<endl;cin>>b;
            cout<<"Ingrese el valor para azul (0-1)"<<endl;cin>>c;
            break;}
    }
}

void menu_mod_puntos(int opcion){
    switch(opcion){
    case 1: modo=2; break;
    case 2: modo=0; break;
    }
}

void menu_grosor(int opcion){
    switch (opcion){//cambia el grosor de las lineas
    case 1: grosor_curva=1; break;
    case 2: grosor_curva=2; break;
    case 3: grosor_curva=3; break;
    }
}


void menu_principal(int opcion){
    switch(opcion){
    case 7: exit( -1 );
    }
}

int main(int argc, char **argv)
{
 glutInit(&argc, argv);
 iniciar();
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
 glutInitWindowSize(600,600);
 glutCreateWindow("Spline | Catmull-Rom");

 int submenu_insertar=glutCreateMenu(menu_insertar);
 glutAddMenuEntry("Activado",1);
 glutAddMenuEntry("Desactivado",2);

 int submenu_graficar_punto=glutCreateMenu(menu_graficar_punto);
 glutAddMenuEntry("Activado",1);
 glutAddMenuEntry("Desactivado",2);

int submenu_graficar_curva=glutCreateMenu(menu_graficar_curva);
 glutAddMenuEntry("Activado",1);
 glutAddMenuEntry("Desactivado",2);

 int submenu_color=glutCreateMenu(menu_color);
 glutAddMenuEntry("Rojo",1);
 glutAddMenuEntry("Verde",2);
 glutAddMenuEntry("Azul",3);
 glutAddMenuEntry("Aleatorio",4);
 glutAddMenuEntry("Personalizado",5);

 int submenu_mod_puntos=glutCreateMenu(menu_mod_puntos);
 glutAddMenuEntry("Activado",1);
 glutAddMenuEntry("Desactivado",2);

 int submenu_grosor=glutCreateMenu(menu_grosor);
 glutAddMenuEntry("grosor 1",1);
 glutAddMenuEntry("grosor 2",2);
 glutAddMenuEntry("grosor 3",3);

 glutCreateMenu(menu_principal);
 glutAddSubMenu("Insertar Puntos",submenu_insertar);
 glutAddSubMenu("Graficar Puntos",submenu_graficar_punto);
 glutAddSubMenu("Graficar Curva",submenu_graficar_curva);
 glutAddSubMenu("Color",submenu_color);
 glutAddSubMenu("Cambiar posicion",submenu_mod_puntos);
 glutAddSubMenu("Grosor",submenu_grosor);
 glutAddMenuEntry("Esc",7);

 // usaremos el boton derecho del mouse
 glutAttachMenu(GLUT_RIGHT_BUTTON);

 glutReshapeFunc(myReshape);
 glutDisplayFunc(display);
 glutIdleFunc(display);
 //funcion para capturar el mouse
 glutMouseFunc(mouse);
 glEnable(GL_DEPTH_TEST);
 glutMainLoop();

 return 0;
}
