#include "lib_3d.h"
#include <stdlib.h>
#include "lib_mat.h"

void affPoint(t_point3d *p)
{
  if(p!=NULL)
    printf("(%d,%d,%d)",(int)p->xyzt[0],(int)p->xyzt[1],(int)p->xyzt[2]);
}

void affMatrice(double m[4][4])
{
  printf("matrice(%f,%f,%f,%f)\n",m[0][0],m[0][1],m[0][2],m[0][3]);
  printf("       (%f,%f,%f,%f)\n",m[1][0],m[1][1],m[1][2],m[1][3]);
  printf("       (%f,%f,%f,%f)\n",m[2][0],m[2][1],m[2][2],m[2][3]);
  printf("       (%f,%f,%f,%f)\n",m[3][0],m[3][1],m[3][2],m[3][3]);
  printf("\n");
}

void affTri(t_triangle3d *t)
{
  printf("triangle de point :\n");
  affPoint(t->abc[0]);
  affPoint(t->abc[1]);
  affPoint(t->abc[2]);
  printf("\n");
}

void multiplicationVecteur3d(t_point3d *v1, double m[4][4], t_point3d *v2) // v1 = m*v2
{
  int i, j;
  double tmp[4];
  for(i=0;i<4;i++){
    tmp[i]=0;
    for(j=0;j<4;j++){
      tmp[i]=tmp[i]+(m[i][j])*(v2->xyzt[j]);
    }     
  }
  for(i=0;i<4;i++){
    v1->xyzt[i]=tmp[i];
  }
}

void multiplicationMatrice3d(double m1[4][4], double m2[4][4], double m3[4][4]) // m1 = m2*m3
{
  int i, j, k;
  double tmp[4][4];
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      tmp[i][j]=0;
      for(k=0;k<4;k++){
	tmp[i][j]=tmp[i][j]+m2[i][k]*m3[k][j];
      }}}
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      m1[i][j]=tmp[i][j];
    }}
}

void copierMatrice3d(double m1[4][4], double m2[4][4]) // m1 = m2
{
  int i, j;
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      m1[i][j]=m2[i][j];
    }}
}


int randy(int a,int b)
{
  return rand()%(b-a)+a;
}
