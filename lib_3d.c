#include "lib_surface.h"
#include "lib_3d.h"
#include "lib_2d.h"
#include "lib_mat.h"
#include <math.h>
#define pi (3.141592653589793)
typedef struct
{
  double m[4][4];
} t_matrice3d;

t_point3d *definirPoint3d(double x, double y, double z)	// attention malloc
{
  t_point3d *p;

  p  = (t_point3d*) malloc(sizeof(t_point3d));
  if (p!=NULL)
    {
      p->xyzt[0] = x;
      p->xyzt[1] = y;
      p->xyzt[2] = z;
      p->xyzt[3] = 1;
    }

  return p;
}

t_point3d *definirVecteur3d(double x, double y, double z)	// attention malloc
{
  t_point3d *p = NULL;

  p=  (t_point3d*) malloc(sizeof(t_point3d));
  p->xyzt[0] = x;
  p->xyzt[1] = y;
  p->xyzt[2] = z;
  p->xyzt[3] = 0;
  return p;
}

t_triangle3d *definirTriangle3d(t_point3d * a, t_point3d * b, t_point3d * c)	// attention malloc
{
  t_triangle3d *t = NULL;
  t = (t_triangle3d*) malloc(sizeof(t_triangle3d));
  t->abc[0]=a;
  t->abc[1]=b;
  t->abc[2]=c;
  
  return t;
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
t_point2d *__conversion_2d_3d(t_point3d *p3d)
{
  t_point2d *p2d;
  t_point3d *p3dtmp;
  double matrice_projection[4][4]={{1, 0, 0, 0},\
				   {0, 1, 0, 0},\
				   {0, 0, 1, 0},\
				   {0, 0, 0, 1}};


  p2d = NULL;
  p3dtmp = (t_point3d*)malloc(sizeof(t_point3d));
  if (p3dtmp!=NULL)
    {
      multiplicationVecteur3d(p3dtmp, matrice_projection, p3d);

      p2d = definirPoint2d(p3dtmp->xyzt[0]+RX/2, p3dtmp->xyzt[1]+RY/2); // malloc implicite il faut faire un free plus tard... (dans une vingtaine de lignes)
    }

  free(p3dtmp);
  return p2d;
}

void remplirTriangle3d(t_surface * surface, t_triangle3d * triangle, Uint32 c)
{
  t_point2d *p2da, *p2db, *p2dc;
  t_triangle2d *t2d;
  p2da = __conversion_2d_3d(triangle->abc[0]);
  p2db = __conversion_2d_3d(triangle->abc[1]);
  p2dc = __conversion_2d_3d(triangle->abc[2]);

  t2d = definirTriangle2d(p2da, p2db, p2dc);

  remplirTriangle2d(surface, t2d, c);

  free(t2d);
  free(p2da); // le free est fait ici :)
  free(p2db);
  free(p2dc);

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
  //printf("bula4.1\n");
  float x,y,z;
  x=degreX*2*pi/360;
  y=degreY*2*pi/360;
  z=degreZ*2*pi/360;
  //printf("bula4.2\n");
  double mx[4][4]={{1, 0, 0, 0},		\
		   {0, cos(x),-sin(x), 0},\
		   {0, sin(x), cos(x), 0},\
		   {0, 0, 0, 1}};
  //printf("bula4.3\n");
  double my[4][4]={{cos(y), 0, sin(y), 0},\
		   {0, 1, 0, 0},\
		   {-sin(y), 0, cos(y), 0},\
		   {0, 0, 0, 1}};
  //printf("bula4.4\n");
  double mz[4][4]={{cos(z),-sin(z), 0, 0},\
		   {sin(z), cos(z), 0, 0},\
		   {0, 0, 1, 0},\
		   {0, 0, 0, 1}};
  //printf("bula4.5\n");
  transformationTriangle3d(t,mx);
  //printf("bula4.6\n");
  transformationTriangle3d(t,my);
  //printf("bula4.7\n");
  transformationTriangle3d(t,mz);
  //printf("bula4.8\n");
}

void transformationTriangle3d(t_triangle3d *t, double mat[4][4])
{
  int i;
  //printf("bula4.8.1\n");
  for(i=0;i<3;i++){
    //printf("bula4.8.1.%d\n",i);
    multiplicationVecteur3d(t->abc[i],mat,t->abc[i]);
  }
}

