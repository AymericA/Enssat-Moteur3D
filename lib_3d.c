#include "lib_surface.h"
#include "lib_3d.h"
#include "lib_2d.h"
#include "lib_mat.h"
#include <math.h>

typedef struct
{
  double m[4][4];
} t_matrice3d;

t_point3d *definirPoint3d(double x, double y, double z)	// attention malloc
{
  t_point3d *p;
  p  = (t_point3d*) malloc(sizeof(t_point3d));
  if (p!=NULL){
    p->xyzt[0] = x;
    p->xyzt[1] = y;
    p->xyzt[2] = z;
    p->xyzt[3] = 1;
    p->copie=NULL;
  }
  return p;
}

t_point3d *definirVecteur3d(double x, double y, double z)
{
  t_point3d *p = NULL;
  p=(t_point3d*) malloc(sizeof(t_point3d));
  p->xyzt[0] = x;
  p->xyzt[1] = y;
  p->xyzt[2] = z;
  p->xyzt[3] = 0;
  p->copie=NULL;
  return p;
}

t_triangle3d *definirTriangle3d(t_point3d * a, t_point3d * b, t_point3d * c)
{
  t_triangle3d *t = NULL;
  t = (t_triangle3d*) malloc(sizeof(t_triangle3d));
  t->abc[0]=a;
  t->abc[1]=b;
  t->abc[2]=c;
  return t;
}

t_point3d *copierPoint3d(t_point3d *p)
{
  return definirPoint3d(p->xyzt[0],p->xyzt[1],p->xyzt[2]);
}

t_triangle3d *copierTriangle3d(t_triangle3d *t)
{
  t_triangle3d *n = NULL;
  t_point3d *a=definirPoint3d(t->abc[0]->xyzt[0],t->abc[0]->xyzt[1],t->abc[0]->xyzt[2]);
  t_point3d *b=definirPoint3d(t->abc[1]->xyzt[0],t->abc[1]->xyzt[1],t->abc[1]->xyzt[2]);
  t_point3d *c=definirPoint3d(t->abc[2]->xyzt[0],t->abc[2]->xyzt[1],t->abc[2]->xyzt[2]);
  n= definirTriangle3d(a,b,c);
  return n;
}

void libererTriangle3d(t_triangle3d *t)
{
  free(t->abc[0]);
  free(t->abc[1]);
  free(t->abc[2]);
  free(t);
}

// effectue une conversion de 2D en 3D
t_point2d *__conversion_2d_3d(t_point3d *p3d, double h)
{
  t_point2d *p2d;
  t_point3d *p3dtmp;
  p2d = NULL;
  p3dtmp = (t_point3d*)malloc(sizeof(t_point3d));
  if (p3dtmp!=NULL){
    double lala= h/p3d->xyzt[2];
    //printf("valeur de h : %lf\n",h);

    double tmp[4][4]={{lala, 0, 0, 0},		\
		      {0, lala, 0, 0},		\
		      {0, 0, lala, 0},		\
		      {0, 0, 0, 1}};   
    
    multiplicationVecteur3d(p3dtmp, tmp, p3d);
    p2d = definirPoint2d(p3dtmp->xyzt[0]+RX/2, p3dtmp->xyzt[1]+RY/2); // malloc implicite il faut faire un free plus tard... (dans une vingtaine de lignes)
  }
  free(p3dtmp);
  return p2d;
}

void remplirTriangle3d(t_surface * surface, t_triangle3d * triangle, Uint32 c,double h)
{
   t_point2d *p2da, *p2db, *p2dc;
  t_triangle2d *t2d;
  double A,B,C,D,acx,acy,acz,bcx,bcy,bcz;

  if(triangle->abc[0]->xyzt[2]<0 && triangle->abc[1]->xyzt[2]<0 && triangle->abc[2]->xyzt[2]<0) //vérif si triangle est dans le demi espace des z négatifs
    // (utile ?)
  {

  p2da = __conversion_2d_3d(triangle->abc[0],h);
  p2db = __conversion_2d_3d(triangle->abc[1],h);
  p2dc = __conversion_2d_3d(triangle->abc[2],h);
  t2d = definirTriangle2d(p2da, p2db, p2dc);

  //calcul d'un vecteur normal a t3d

  acx=triangle->abc[2]->xyzt[0]-triangle->abc[0]->xyzt[0];
  acy=triangle->abc[2]->xyzt[1]-triangle->abc[0]->xyzt[1];
  acz=triangle->abc[2]->xyzt[2]-triangle->abc[0]->xyzt[2];

  bcx=triangle->abc[2]->xyzt[0]-triangle->abc[1]->xyzt[0];
  bcy=triangle->abc[2]->xyzt[1]-triangle->abc[1]->xyzt[1];
  bcz=triangle->abc[2]->xyzt[2]-triangle->abc[1]->xyzt[2];

  A=acy*bcz-acz*bcy;
  B=acz*bcx-acx*bcz;
  C=acx*bcy-acy*bcx;

  //équation du t3d : Ax+By+Cz+D=0
  
  D=-(triangle->abc[0]->xyzt[0]*A+triangle->abc[0]->xyzt[1]*B+triangle->abc[0]->xyzt[2]*C);

  remplirTriangle2d(surface,t2d,A,B,C,D,h,c);
  free(t2d);
  free(p2da); // le free est fait ici :)
  free(p2db);
  free(p2dc);
    }
}

void translationTriangle3d(t_triangle3d *t, t_point3d *vecteur)
{
  double m[4][4]={{1, 0, 0,vecteur->xyzt[0]},\
		  {0, 1, 0,vecteur->xyzt[1]},\
		  {0, 0, 1,vecteur->xyzt[2]},\
		  {0, 0, 0, 1}};
  transformationTriangle3d(t,m);
}

void rotationTriangle3d(t_triangle3d *t, t_point3d *centre, float degreX, float degreY, float degreZ)
{
  float x,y,z;
  x=degreX*M_PI/180;
  y=degreY*M_PI/180;
  z=degreZ*M_PI/180;
  double transfo[4][4];
  double m[4][4]={{1, 0, 0,centre->xyzt[0]},\
		  {0, 1, 0,centre->xyzt[1]},\
		  {0, 0, 1,centre->xyzt[2]},\
		  {0, 0, 0, 1}};
  double mx[4][4]={{1, 0, 0, 0},\
		   {0, cos(x),-sin(x), 0},\
		   {0, sin(x), cos(x), 0},\
		   {0, 0, 0, 1}};
  double my[4][4]={{cos(y), 0, sin(y), 0},\
		   {0, 1, 0, 0},\
		   {-sin(y), 0, cos(y), 0},\
		   {0, 0, 0, 1}};
  double mz[4][4]={{cos(z),-sin(z), 0, 0},\
		   {sin(z), cos(z), 0, 0},\
		   {0, 0, 1, 0},\
		   {0, 0, 0, 1}};
  double minv[4][4]={{1, 0, 0,-centre->xyzt[0]},\
		     {0, 1, 0,-centre->xyzt[1]},\
		     {0, 0, 1,-centre->xyzt[2]},\
		     {0, 0, 0, 1}};
  multiplicationMatrice3d(transfo,m,mx);
  multiplicationMatrice3d(transfo,transfo,my);
  multiplicationMatrice3d(transfo,transfo,mz);
  multiplicationMatrice3d(transfo,transfo,minv);
  transformationTriangle3d(t,transfo);
}

void transformationTriangle3d(t_triangle3d *t, double mat[4][4])
{
  int i;
  for(i=0;i<3;i++){
    multiplicationVecteur3d(t->abc[i],mat,t->abc[i]);
  }
}

