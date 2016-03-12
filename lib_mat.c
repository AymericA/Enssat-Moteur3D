#include "lib_3d.h"
#include "lib_mat.h"

void multiplicationVecteur3d(t_point3d *v1, double m[4][4], t_point3d *v2) // v1 = m*v2
{
  int i, j;
  double tmp;
  //printf("bula4.8.1.0\n");
  for(i=0;i<4;i++){
    //printf("bula4.8.1.%d\n",i);
    tmp=0;
    for(j=0;j<4;j++){
      //printf("bula4.8.1.%d.%d\n",i,j);

      tmp=tmp+(m[i][j])*(v2->xyzt[j]);
    }
    v1->xyzt[i]=tmp;       
  }
}

void multiplicationMatrice3d(double m1[4][4], double m2[4][4], double m3[4][4]) // m1 = m2*m3
{
  int i, j, k;
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      m1[i][j]=0;
      for(k=0;k<4;k++){
	m1[i][j]=m1[i][j]+m2[i][k]*m3[k][j];
      }
    }
  }
}

void copierMatrice3d(double m1[4][4], double m2[4][4]) // m1 = m2
{
  int i, j;
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      m1[i][j]=m2[i][j];
    }
  }
}
