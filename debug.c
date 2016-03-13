#include "debug.h"


void affPoint(t_point3d *p)
{
  printf("point(%f,%f,%f,%f)\n",p->xyzt[0],p->xyzt[1],p->xyzt[2],p->xyzt[3]);
}

void affTri(t_triangle3d *t)
{
  printf("triangle de point :\n");
  affPoint(t->abc[0]);
  affPoint(t->abc[1]);
  affPoint(t->abc[2]);
  printf("\n");
}

void affMatrice(double m[4][4])
{
  printf("matrice((%f,%f,%f,%f)\n",m[0][0],m[0][1],m[0][2],m[0][3]);
  printf("        (%f,%f,%f,%f)\n",m[1][0],m[1][1],m[1][2],m[1][3]);
  printf("        (%f,%f,%f,%f)\n",m[2][0],m[2][1],m[2][2],m[2][3]);
  printf("        (%f,%f,%f,%f))\n",m[3][0],m[3][1],m[3][2],m[3][3]);
  printf("\n");
}

double moy_z(t_triangle3d *t)
{
  return ((t->abc[0]->xyzt[2])+(t->abc[1]->xyzt[2])+(t->abc[0]->xyzt[2]))/3;
}
