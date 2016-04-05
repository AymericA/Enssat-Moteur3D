#include <assert.h>
#include <math.h>
#include "lib_3d.h"
#include "lib_objet3d.h"
#include "lib_mat.h"
#include "lib_surface.h"


double moy_z(t_triangle3d *t)
{
  return ((t->abc[0]->xyzt[2])+(t->abc[1]->xyzt[2])+(t->abc[0]->xyzt[2]))/3;
}

void remplir_z_index(t_maillon *pt_maillon)
{
  if(pt_maillon!=NULL)
    {
      pt_maillon->z_index=moy_z(pt_maillon->face);
      remplir_z_index(pt_maillon->pt_suiv);
    }
}

void check(t_objet3d *pt_objet)
{
  t_maillon *tmp=pt_objet->tete;
  do{
    if(tmp->z_index>tmp->pt_suiv->z_index)
      {
	printf("WARNING\n");
      }
    tmp=tmp->pt_suiv;
  }
  while(tmp->pt_suiv!=NULL);
}

t_chaine *__cree_chaine(t_point3d *point)
{    
  t_chaine* pt_chaine=malloc(sizeof(t_chaine));
  pt_chaine->point=point;
  pt_chaine->pt_suiv=NULL;
  return pt_chaine;
}

void __insere_tete_chaine(t_objet3d *pt_objet, t_chaine *pt_chaine)
{
  pt_chaine->pt_suiv=pt_objet->chaine;
  pt_objet->chaine=pt_chaine;
}

void affi_chaine(t_chaine *pt_chaine)
{
  if(pt_chaine==NULL)
    printf("NULL\n");
  else{
    affPoint(pt_chaine->point);
    printf("-->");
    affi_chaine(pt_chaine->pt_suiv);
  }
}

t_point3d *customPoint3d(t_point3d *p,t_objet3d *pt_objet)
{
  if(p->copie==NULL){
    p->copie=copierPoint3d(p);
    __insere_tete_chaine(pt_objet,__cree_chaine(p->copie));
  }
  return p->copie;
}


t_maillon* __cree_maillon(t_triangle3d *face, Uint32 couleur)
{
  t_maillon* pt_maillon = malloc(sizeof(t_maillon));
  pt_maillon->face=face;
  pt_maillon->couleur=couleur;
  pt_maillon->pt_suiv=NULL;
  pt_maillon->z_index=moy_z(face);
  return pt_maillon;
}


void __insere_tete(t_objet3d *pt_objet, t_maillon *pt_maillon)
{
  pt_maillon->pt_suiv=pt_objet->tete;
  pt_objet->tete=pt_maillon;
}

t_objet3d *objet_vide()
{
  t_objet3d *pt_objet = malloc(sizeof(t_objet3d));
  pt_objet->est_trie=true;
  pt_objet->est_camera=false;
  pt_objet->tete=NULL;
  pt_objet->chaine=NULL;
  return pt_objet;
}

t_objet3d *camera()
{
  t_objet3d *pt_objet = malloc(sizeof(t_objet3d));
  pt_objet->est_trie=true;
  pt_objet->est_camera=true;
  pt_objet->tete=NULL;
  pt_objet->chaine=NULL;
  return pt_objet;
}

t_objet3d* parallelepipede(double lx, double ly, double lz)
{
  t_objet3d *pt_objet = NULL;
  pt_objet = objet_vide();
  double x=lx/2,y=ly/2,z=lz/2;

  t_point3d *p1 = definirPoint3d(x,y,z);
  t_point3d *p2 = definirPoint3d(-x,y,z);

  t_point3d *p3 = definirPoint3d(x,-y,z);
  t_point3d *p4 = definirPoint3d(-x,-y,z);

  t_point3d *p5 = definirPoint3d(x,y,-z);
  t_point3d *p6 = definirPoint3d(-x,y,-z);

  t_point3d *p7 = definirPoint3d(x,-y,-z);
  t_point3d *p8 = definirPoint3d(-x,-y,-z);

  __insere_tete_chaine(pt_objet,__cree_chaine(p1));
  __insere_tete_chaine(pt_objet,__cree_chaine(p2));
  __insere_tete_chaine(pt_objet,__cree_chaine(p3));
  __insere_tete_chaine(pt_objet,__cree_chaine(p4));
  __insere_tete_chaine(pt_objet,__cree_chaine(p5));
  __insere_tete_chaine(pt_objet,__cree_chaine(p6));
  __insere_tete_chaine(pt_objet,__cree_chaine(p7));
  __insere_tete_chaine(pt_objet,__cree_chaine(p8));

  t_triangle3d *t1 = definirTriangle3d(p1, p2, p3);
  t_triangle3d *t2 = definirTriangle3d(p2, p3, p4);

  t_triangle3d *t3 = definirTriangle3d(p1, p3, p5);
  t_triangle3d *t4 = definirTriangle3d(p3, p5, p7);

  t_triangle3d *t5 = definirTriangle3d(p1, p2, p5);
  t_triangle3d *t6 = definirTriangle3d(p2, p5, p6);

  t_triangle3d *t7 = definirTriangle3d(p4, p6, p8);
  t_triangle3d *t8 = definirTriangle3d(p2, p4, p6);

  t_triangle3d *t9 = definirTriangle3d(p6, p7, p8);
  t_triangle3d *t10 = definirTriangle3d(p5, p6, p7);

  t_triangle3d *t11 = definirTriangle3d(p4, p7, p8);
  t_triangle3d *t12 = definirTriangle3d(p3, p4, p7);

  __insere_tete(pt_objet, __cree_maillon(t1,ROUGEF));
  __insere_tete(pt_objet, __cree_maillon(t2,ROUGEC));

  __insere_tete(pt_objet, __cree_maillon(t3,VERTF));
  __insere_tete(pt_objet, __cree_maillon(t4,VERTC));

  __insere_tete(pt_objet, __cree_maillon(t5,BLEUF));
  __insere_tete(pt_objet, __cree_maillon(t6,BLEUC));

  __insere_tete(pt_objet, __cree_maillon(t7,JAUNEF));
  __insere_tete(pt_objet, __cree_maillon(t8,JAUNEC));

  __insere_tete(pt_objet, __cree_maillon(t9,ROSEF));
  __insere_tete(pt_objet, __cree_maillon(t10,ROSEC));

  __insere_tete(pt_objet, __cree_maillon(t11,GRISC));
  __insere_tete(pt_objet, __cree_maillon(t12,BLANC));

  pt_objet->est_trie=false;
  return pt_objet;
}

t_objet3d* cylindre(double lh,double r, double nlong)
{
  t_objet3d *pt_objet = NULL;
  pt_objet = objet_vide();
  double h=lh/2;
  t_point3d *tp[(int)nlong][2];
  t_triangle3d *tmp;
  int i;
  t_point3d *p0 =definirPoint3d(0,-h,0);
  t_point3d *p1 =definirPoint3d(0,h,0);
  __insere_tete_chaine(pt_objet,__cree_chaine(p0));
  __insere_tete_chaine(pt_objet,__cree_chaine(p1));
  
  for(i=0;i<nlong;i++){
    tp[i][0]=definirPoint3d(
			    r*cos(i*2*M_PI/nlong),
			    -h,
			    r*sin(i*2*M_PI/nlong));
    tp[i][1]=definirPoint3d(
			    r*cos(i*2*M_PI/nlong),
			    h,
			    r*sin(i*2*M_PI/nlong));
    __insere_tete_chaine(pt_objet,__cree_chaine(tp[i][0]));
    __insere_tete_chaine(pt_objet,__cree_chaine(tp[i][1]));
  }
  
  for(i=0;i<nlong-1;i++){
    tmp = definirTriangle3d(p0,tp[i][0],tp[i+1][0]);
    __insere_tete(pt_objet, __cree_maillon(tmp,GRISC));
    tmp = definirTriangle3d(tp[i][1],tp[i+1][1],p1);
    __insere_tete(pt_objet, __cree_maillon(tmp,GRISC));
    
    tmp = definirTriangle3d(tp[i][0],tp[i+1][0],tp[i+1][1]);
    __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEC));
    tmp = definirTriangle3d(tp[i][0],tp[i][1],tp[i+1][1]);
    __insere_tete(pt_objet, __cree_maillon(tmp,BLANC));    
    
  }

  tmp = definirTriangle3d(p0,tp[(int)nlong-1][0],tp[0][0]);
  __insere_tete(pt_objet, __cree_maillon(tmp,GRISC));
  tmp = definirTriangle3d(tp[(int)nlong-1][1],tp[0][1],p1);
  __insere_tete(pt_objet, __cree_maillon(tmp,GRISC));
  
  tmp = definirTriangle3d(tp[(int)nlong-1][0],tp[0][0],tp[0][1]);
  __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEC));
  tmp = definirTriangle3d(tp[(int)nlong-1][0],tp[(int)nlong-1][1],tp[0][1]);
  __insere_tete(pt_objet, __cree_maillon(tmp,BLANC));    
  
  pt_objet->est_trie=false;
  return pt_objet;
}


t_objet3d* cone(double lh,double r, double nlat, double nlong)
{
  t_objet3d *pt_objet = NULL;
  pt_objet = objet_vide();
  double h=lh/2;
  t_point3d *tp[(int)nlong][(int)nlat];
  t_triangle3d *tmp;
  int i,j;
  t_point3d *p0 =definirPoint3d(0,-h,0);
  t_point3d *p1 =definirPoint3d(0,h,0);
  __insere_tete_chaine(pt_objet,__cree_chaine(p0));
  __insere_tete_chaine(pt_objet,__cree_chaine(p1));
  
  for(i=0;i<nlong;i++){
    for(j=0;j<nlat;j++){
      tp[i][j]=definirPoint3d(r*(j+1)*cos(i*2*M_PI/nlong)/nlat,
			      -h+(j+1)*lh/nlat,
			      r*(j+1)*sin(i*2*M_PI/nlong)/nlat);
      __insere_tete_chaine(pt_objet,__cree_chaine(tp[i][j]));
    }}

  for(i=0;i<nlong-1;i++){
    tmp = definirTriangle3d(p0,tp[i][0],tp[i+1][0]);
    __insere_tete(pt_objet, __cree_maillon(tmp,GRISC));
    tmp = definirTriangle3d(p1,tp[i][(int)nlat-1],tp[i+1][(int)nlat-1]);
    __insere_tete(pt_objet, __cree_maillon(tmp,GRISC));
    for(j=0;j<nlat-1;j++){    
      tmp = definirTriangle3d(tp[i][j],tp[i+1][j],tp[i+1][j+1]);
      __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEC));
      tmp = definirTriangle3d(tp[i][j],tp[i][j+1],tp[i+1][j+1]);
      __insere_tete(pt_objet, __cree_maillon(tmp,BLANC));
    }
  }
  for(j=0;j<nlat-1;j++){    
    tmp = definirTriangle3d(tp[(int)nlong-1][j],tp[0][j],tp[0][j+1]);
    __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEC));
    tmp = definirTriangle3d(tp[(int)nlong-1][j],tp[(int)nlong-1][j+1],tp[0][j+1]);
    __insere_tete(pt_objet, __cree_maillon(tmp,BLANC));
  }
  tmp = definirTriangle3d(p0,tp[(int)nlong-1][0],tp[0][0]);
  __insere_tete(pt_objet, __cree_maillon(tmp,GRISC));
  tmp = definirTriangle3d(p1,tp[(int)nlong-1][(int)nlat-1],tp[0][(int)nlat-1]);
  __insere_tete(pt_objet, __cree_maillon(tmp,GRISC));

  pt_objet->est_trie=false;
  return pt_objet;
}



double n64_x(int i,double x,double c){
  switch(i){
  case 0:return -x;
    break;
  case 1: return (-x+x/c);
    break;
  case 2: return (x-x/c);
    break;
  case 3: return x;
    break;
  }
}
    
double n64_z(int k,double x,double c){
  switch(k){
  case 0:return -x;
    break;
  case 1: return (-x+x/c);
    break;
  case 2: return (x-x/c);
    break;
  case 3: return x;
    break; 
  }
}

double n64_y(int j,double x,double c){
  switch(j){
  case 0:return -x;
    break;
  case 1: return (-x+2*x/c);
    break;
  case 2: return (x-2*x/c);
    break;
  case 3: return x;
    break; 
  }
}

t_objet3d* n64(double lx)
{
  t_objet3d *pt_objet = NULL;
  pt_objet = objet_vide();
  double x=lx/2;
  const double c=570/165;
  t_point3d *tp[4][4][4];
  t_triangle3d *tmp;
  int i,j,k;

  for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      for(k=0;k<4;k++){
	tp[i][j][k]=definirPoint3d(n64_x(i,x,c),n64_y(k,x,c),n64_z(j,x,c));
	__insere_tete_chaine(pt_objet,__cree_chaine(tp[i][j][k]));
      }}}

  //carré supérieur et inférieur jaune

  for(i=0;i<2;i++){
    for(j=0;j<2;j++){
      for(k=0;k<2;k++){

	tmp = definirTriangle3d(tp[2*i][2*j][3*k],tp[i*2][2*j+1][3*k],tp[i*2+1][2*j+1][3*k]);
	__insere_tete(pt_objet, __cree_maillon(tmp,JAUNEC));
	tmp = definirTriangle3d(tp[2*i][2*j][3*k],tp[i*2+1][2*j][3*k],tp[i*2+1][2*j+1][3*k]);
	__insere_tete(pt_objet, __cree_maillon(tmp,JAUNEC));
      }}}

  //face avant et arriere des "N" bleu&vert
  i=1;
  k=2;
  
  for(j=0;j<2;j++){
    //barre transversal arrière       
    tmp = definirTriangle3d(tp[i][j][k],tp[i][j][k+1],tp[i+1][j][k-1]);
    __insere_tete(pt_objet, __cree_maillon(tmp,BLEUF));
    tmp = definirTriangle3d(tp[i][j][k],tp[i+1][j][k-2],tp[i+1][j][k-1]);
    __insere_tete(pt_objet, __cree_maillon(tmp,BLEUF));
    

    tmp = definirTriangle3d(tp[j+2][i][k],tp[j+2][i][k+1],tp[j+2][i+1][k-1]);
    __insere_tete(pt_objet, __cree_maillon(tmp,VERTF));
    tmp = definirTriangle3d(tp[j+2][i][k],tp[j+2][i+1][k-2],tp[j+2][i+1][k-1]);
    __insere_tete(pt_objet, __cree_maillon(tmp,VERTF));

    //barre transversal avant j+2
    tmp = definirTriangle3d(tp[i][j+2][k-1],tp[i][j+2][k-2],tp[i+1][j+2][k]);
    __insere_tete(pt_objet, __cree_maillon(tmp,BLEUF));
    tmp = definirTriangle3d(tp[i][j+2][k-1],tp[i+1][j+2][k+1],tp[i+1][j+2][k]);
    __insere_tete(pt_objet, __cree_maillon(tmp,BLEUF));

    tmp = definirTriangle3d(tp[j][i][k-1],tp[j][i][k-2],tp[j][i+1][k]);
    __insere_tete(pt_objet, __cree_maillon(tmp,VERTF));
    tmp = definirTriangle3d(tp[j][i][k-1],tp[j][i+1][k+1],tp[j][i+1][k]);
    __insere_tete(pt_objet, __cree_maillon(tmp,VERTF));
  }

  i=0;
  k=0;
  for(j=0;j<2;j++){
    //barre verticale gauche
    tmp = definirTriangle3d(tp[i][3*j][k],tp[i][3*j][k+3],tp[i+1][3*j][k+3]);
    __insere_tete(pt_objet, __cree_maillon(tmp,BLEUF));
    tmp = definirTriangle3d(tp[i][3*j][k],tp[i+1][3*j][k],tp[i+1][3*j][k+3]);
    __insere_tete(pt_objet, __cree_maillon(tmp,BLEUF));

    tmp = definirTriangle3d(tp[3*j][i][k],tp[3*j][i][k+3],tp[3*j][i+1][k+3]);
    __insere_tete(pt_objet, __cree_maillon(tmp,VERTF));
    tmp = definirTriangle3d(tp[3*j][i][k],tp[3*j][i+1][k],tp[3*j][i+1][k+3]);
    __insere_tete(pt_objet, __cree_maillon(tmp,VERTF));

    //barre verticale droite
    tmp = definirTriangle3d(tp[i+2][3*j][k],tp[i+2][3*j][k+3],tp[i+3][3*j][k+3]);
    __insere_tete(pt_objet, __cree_maillon(tmp,BLEUF));
    tmp = definirTriangle3d(tp[i+2][3*j][k],tp[i+3][3*j][k],tp[i+3][3*j][k+3]);
    __insere_tete(pt_objet, __cree_maillon(tmp,BLEUF));

    tmp = definirTriangle3d(tp[3*j][i+2][k],tp[3*j][i+2][k+3],tp[3*j][i+3][k+3]);
    __insere_tete(pt_objet, __cree_maillon(tmp,VERTF));
    tmp = definirTriangle3d(tp[3*j][i+2][k],tp[3*j][i+3][k],tp[3*j][i+3][k+3]);
    __insere_tete(pt_objet, __cree_maillon(tmp,VERTF));

  }

  
  //face supérieur #hardcode
  tmp = definirTriangle3d(tp[1][0][2],tp[1][1][2],tp[2][1][0]);
  __insere_tete(pt_objet, __cree_maillon(tmp,VERTF));
  tmp = definirTriangle3d(tp[1][0][2],tp[2][0][0],tp[2][1][0]);
  __insere_tete(pt_objet, __cree_maillon(tmp,VERTF));

  tmp = definirTriangle3d(tp[1][2][0],tp[1][3][0],tp[2][3][2]);
  __insere_tete(pt_objet, __cree_maillon(tmp,VERTF));
  tmp = definirTriangle3d(tp[1][2][0],tp[2][2][2],tp[2][3][2]);
  __insere_tete(pt_objet, __cree_maillon(tmp,VERTF));

  tmp = definirTriangle3d(tp[0][1][1],tp[0][2][3],tp[1][2][3]);
  __insere_tete(pt_objet, __cree_maillon(tmp,BLEUF));
  tmp = definirTriangle3d(tp[0][1][1],tp[1][1][1],tp[1][2][3]);
  __insere_tete(pt_objet, __cree_maillon(tmp,BLEUF));

  tmp = definirTriangle3d(tp[2][2][1],tp[2][1][3],tp[3][1][3]);
  __insere_tete(pt_objet, __cree_maillon(tmp,BLEUF));
  tmp = definirTriangle3d(tp[2][2][1],tp[3][2][1],tp[3][1][3]);
  __insere_tete(pt_objet, __cree_maillon(tmp,BLEUF));
 
  tmp = definirTriangle3d(tp[1][0][3],tp[1][1][3],tp[2][1][1]);
  __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEF));
  tmp = definirTriangle3d(tp[1][0][3],tp[2][0][1],tp[2][1][1]);
  __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEF));

  tmp = definirTriangle3d(tp[1][2][1],tp[1][3][1],tp[2][3][3]);
  __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEF));
  tmp = definirTriangle3d(tp[1][2][1],tp[2][2][3],tp[2][3][3]);
  __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEF));

  tmp = definirTriangle3d(tp[0][1][0],tp[0][2][2],tp[1][2][2]);
  __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEF));
  tmp = definirTriangle3d(tp[0][1][0],tp[1][1][0],tp[1][2][2]);
  __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEF));

  tmp = definirTriangle3d(tp[2][1][2],tp[2][2][0],tp[3][2][0]);
  __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEF));
  tmp = definirTriangle3d(tp[2][1][2],tp[3][1][2],tp[3][2][0]);
  __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEF));

  //petite face ch***** #hardcode

  tmp = definirTriangle3d(tp[0][1][1],tp[1][1][1],tp[1][1][3]);
  __insere_tete(pt_objet, __cree_maillon(tmp,BLEUF));
  tmp = definirTriangle3d(tp[0][1][1],tp[0][1][3],tp[1][1][3]);
  __insere_tete(pt_objet, __cree_maillon(tmp,BLEUF));

  tmp = definirTriangle3d(tp[2][1][0],tp[2][1][2],tp[3][1][2]);
  __insere_tete(pt_objet, __cree_maillon(tmp,BLEUF));
  tmp = definirTriangle3d(tp[2][1][0],tp[3][1][0],tp[3][1][2]);
  __insere_tete(pt_objet, __cree_maillon(tmp,BLEUF));

  tmp = definirTriangle3d(tp[0][2][0],tp[0][2][2],tp[1][2][2]);
  __insere_tete(pt_objet, __cree_maillon(tmp,BLEUF));
  tmp = definirTriangle3d(tp[0][2][0],tp[1][2][0],tp[1][2][2]);
  __insere_tete(pt_objet, __cree_maillon(tmp,BLEUF));

  tmp = definirTriangle3d(tp[2][2][1],tp[2][2][3],tp[3][2][3]);
  __insere_tete(pt_objet, __cree_maillon(tmp,BLEUF));
  tmp = definirTriangle3d(tp[2][2][1],tp[3][2][1],tp[3][2][3]);
  __insere_tete(pt_objet, __cree_maillon(tmp,BLEUF));

  tmp = definirTriangle3d(tp[1][0][0],tp[1][0][2],tp[1][1][2]);
  __insere_tete(pt_objet, __cree_maillon(tmp,VERTF));
  tmp = definirTriangle3d(tp[1][0][0],tp[1][1][0],tp[1][1][2]);
  __insere_tete(pt_objet, __cree_maillon(tmp,VERTF));

  tmp = definirTriangle3d(tp[1][2][1],tp[1][2][3],tp[1][3][3]);
  __insere_tete(pt_objet, __cree_maillon(tmp,VERTF));
  tmp = definirTriangle3d(tp[1][2][1],tp[1][3][1],tp[1][3][3]);
  __insere_tete(pt_objet, __cree_maillon(tmp,VERTF));

  tmp = definirTriangle3d(tp[2][0][1],tp[2][0][3],tp[2][1][3]);
  __insere_tete(pt_objet, __cree_maillon(tmp,VERTF));
  tmp = definirTriangle3d(tp[2][0][1],tp[2][1][1],tp[2][1][3]);
  __insere_tete(pt_objet, __cree_maillon(tmp,VERTF));

  tmp = definirTriangle3d(tp[2][2][0],tp[2][2][2],tp[2][3][2]);
  __insere_tete(pt_objet, __cree_maillon(tmp,VERTF));
  tmp = definirTriangle3d(tp[2][2][0],tp[2][3][0],tp[2][3][2]);
  __insere_tete(pt_objet, __cree_maillon(tmp,VERTF));


  pt_objet->est_trie=false;
  return pt_objet;
}

t_objet3d* sphere(double r, double nlat, double nlong)
{
  t_objet3d *pt_objet = NULL;
  t_point3d *tp[(int)nlat][(int)nlong];
  int i,j;
  t_triangle3d *tmp;
  pt_objet = objet_vide();
  for(i=1;i<nlat+1;i++){
    for(j=0;j<nlong;j++){
      tp[i-1][j]=definirPoint3d(r*sin(i*M_PI/(nlat+1))*sin(j*2*M_PI/nlong),-r*cos(i*M_PI/(nlat+1)),r*sin(i*M_PI/(nlat+1))*cos(j*2*M_PI/nlong));
      __insere_tete_chaine(pt_objet,__cree_chaine(tp[i-1][j]));
    }
  }
  t_point3d *p0 =definirPoint3d(0,-r,0);
  t_point3d *p1 =definirPoint3d(0,r,0);
  __insere_tete_chaine(pt_objet,__cree_chaine(p0));
  __insere_tete_chaine(pt_objet,__cree_chaine(p1));
  for(i=0;i<nlat-1;i++){
    for(j=0;j<nlong-1;j++){

      tmp = definirTriangle3d(tp[i][j],tp[i][j+1],tp[i+1][j+1]);
      __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEC));

      tmp = definirTriangle3d(tp[i][j],tp[i+1][j],tp[i+1][j+1]);
      __insere_tete(pt_objet, __cree_maillon(tmp,BLANC));
    }

    tmp = definirTriangle3d(tp[i][(int)nlong-1],tp[i][0],tp[i+1][0]);
    __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEC));

    tmp = definirTriangle3d(tp[i][(int)nlong-1],tp[i+1][(int)nlong-1],tp[i+1][0]);
    __insere_tete(pt_objet, __cree_maillon(tmp,BLANC));
    
  }
  
  for(j=0;j<nlong-1;j++){

    tmp = definirTriangle3d(p0,tp[0][j],tp[0][j+1]);
    __insere_tete(pt_objet, __cree_maillon(tmp,GRISC));

    tmp = definirTriangle3d(tp[(int)nlat-1][j],tp[(int)nlat-1][j+1],p1);
    __insere_tete(pt_objet, __cree_maillon(tmp,GRISF));
    
  }

  tmp = definirTriangle3d(p0,tp[0][(int)nlong-1],tp[0][0]);
  __insere_tete(pt_objet, __cree_maillon(tmp,GRISC));

  tmp = definirTriangle3d(tp[(int)nlat-1][(int)nlong-1],tp[(int)nlat-1][0],p1);
  __insere_tete(pt_objet, __cree_maillon(tmp,GRISF));
  
  pt_objet->est_trie=false;
  return pt_objet;
}

t_objet3d* geode(double r)
{
  t_objet3d *pt_objet = NULL;
  int i;
  t_point3d *tp[2][5];
  t_triangle3d *tmp;
  pt_objet = objet_vide();
  
  double phi=1.61803398874;
  double a2=r/sqrt(2+phi);
  double r1=a2/sin(M_PI/5);
  double y=sqrt(a2*a2*4-r1*r1);

  //printf("valeur de r : %f\nvaleur de a : %f\nvaleur de r1 : %f\nvaleur de y : %f\n",r,a2*2,r1,y);
  
  t_point3d *p0 =definirPoint3d(0,-r,0);
  t_point3d *p1 =definirPoint3d(0,r,0);
  __insere_tete_chaine(pt_objet,__cree_chaine(p0));
  __insere_tete_chaine(pt_objet,__cree_chaine(p1));
  

  
  for(i=0;i<5;i++){
    tp[0][i]=definirPoint3d(r1*sin(i*0.4*M_PI),y-r,r1*cos(i*0.4*M_PI));
    tp[1][i]=definirPoint3d(r1*sin((i*2+1)*M_PI/5),r-y,r1*cos((i*2+1)*M_PI/5));
    __insere_tete_chaine(pt_objet,__cree_chaine(tp[0][i]));
    __insere_tete_chaine(pt_objet,__cree_chaine(tp[1][i]));
  }

  
  for(i=0;i<4;i++){
    
    tmp = definirTriangle3d(p0,tp[0][i],tp[0][i+1]);
    __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEC));
    
    tmp = definirTriangle3d(p1,tp[1][i],tp[1][i+1]);
    __insere_tete(pt_objet, __cree_maillon(tmp,GRISC));

    
    tmp = definirTriangle3d(tp[0][i],tp[0][i+1],tp[1][i]);
    __insere_tete(pt_objet, __cree_maillon(tmp,GRISF));

    tmp = definirTriangle3d(tp[1][i],tp[1][i+1],tp[0][i+1]);
    __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEF));
    
  }

  
  tmp = definirTriangle3d(p0,tp[0][4],tp[0][0]);
  __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEF));
  
  tmp = definirTriangle3d(p1,tp[1][4],tp[1][0]);
  __insere_tete(pt_objet, __cree_maillon(tmp,GRISF));
    
  
  tmp = definirTriangle3d(tp[0][4],tp[0][0],tp[1][4]);
  __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEF));
  
  tmp = definirTriangle3d(tp[1][4],tp[1][0],tp[0][0]);
  __insere_tete(pt_objet, __cree_maillon(tmp,GRISF));
  
  pt_objet->est_trie=false;
  return pt_objet;
}


t_objet3d* sphere_amiga(double r, double nlat, double nlong)
{
  t_objet3d *pt_objet = NULL;
  t_point3d *tp[(int)nlat][(int)nlong];
  int i,j;
  t_triangle3d *tmp;
  pt_objet = objet_vide();
  for(i=1;i<nlat+1;i++){
    for(j=0;j<nlong;j++){
      tp[i-1][j]=definirPoint3d(r*sin(i*M_PI/(nlat+1))*sin(j*2*M_PI/nlong),-r*cos(i*M_PI/(nlat+1)),r*sin(i*M_PI/(nlat+1))*cos(j*2*M_PI/nlong));
      __insere_tete_chaine(pt_objet,__cree_chaine(tp[i-1][j]));
    }
  }
  t_point3d *p0 =definirPoint3d(0,-r,0);
  t_point3d *p1 =definirPoint3d(0,r,0);
  __insere_tete_chaine(pt_objet,__cree_chaine(p0));
  __insere_tete_chaine(pt_objet,__cree_chaine(p1));
  for(i=0;i<nlat-1;i++){
    for(j=0;j<nlong-1;j++){

      if((i+j)%2){
	tmp = definirTriangle3d(tp[i][j],tp[i][j+1],tp[i+1][j+1]);
	__insere_tete(pt_objet, __cree_maillon(tmp,ROUGEC));
	
	tmp = definirTriangle3d(tp[i][j],tp[i+1][j],tp[i+1][j+1]);
	__insere_tete(pt_objet, __cree_maillon(tmp,ROUGEF));
      }
      else{
	tmp = definirTriangle3d(tp[i][j],tp[i][j+1],tp[i+1][j+1]);
	__insere_tete(pt_objet, __cree_maillon(tmp,BLANC));
	
	tmp = definirTriangle3d(tp[i][j],tp[i+1][j],tp[i+1][j+1]);
	__insere_tete(pt_objet, __cree_maillon(tmp,BLANC));
      }
    }
    if((i+1)%2){
      tmp = definirTriangle3d(tp[i][(int)nlong-1],tp[i][0],tp[i+1][0]);
      __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEC));

      tmp = definirTriangle3d(tp[i][(int)nlong-1],tp[i+1][(int)nlong-1],tp[i+1][0]);
      __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEF));
    }
    else{

      tmp = definirTriangle3d(tp[i][(int)nlong-1],tp[i][0],tp[i+1][0]);
      __insere_tete(pt_objet, __cree_maillon(tmp,BLANC));

      tmp = definirTriangle3d(tp[i][(int)nlong-1],tp[i+1][(int)nlong-1],tp[i+1][0]);
      __insere_tete(pt_objet, __cree_maillon(tmp,BLANC));

    }
  }
  
  for(j=0;j<nlong-1;j++){
    if((j+1)%2){
      tmp = definirTriangle3d(p0,tp[0][j],tp[0][j+1]);
      __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEC));
    }
    else{
      tmp = definirTriangle3d(p0,tp[0][j],tp[0][j+1]);
      __insere_tete(pt_objet, __cree_maillon(tmp,BLANC));
    }

    if((j+(int)nlat)%2){
      tmp = definirTriangle3d(tp[(int)nlat-1][j],tp[(int)nlat-1][j+1],p1);
      __insere_tete(pt_objet, __cree_maillon(tmp,BLANC));
    }
    else{
      tmp = definirTriangle3d(tp[(int)nlat-1][j],tp[(int)nlat-1][j+1],p1);
      __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEC));
    }

  }
  if(((int)nlong)%2){
    tmp = definirTriangle3d(p0,tp[0][(int)nlong-1],tp[0][0]);
    __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEC));
  }
  else{
    tmp = definirTriangle3d(p0,tp[0][(int)nlong-1],tp[0][0]);
    __insere_tete(pt_objet, __cree_maillon(tmp,BLANC));
  }

  if(((int)nlong+(int)nlat)%2){
    tmp = definirTriangle3d(tp[(int)nlat-1][(int)nlong-1],tp[(int)nlat-1][0],p1);
    __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEC));
  }
  else{
    tmp = definirTriangle3d(tp[(int)nlat-1][(int)nlong-1],tp[(int)nlat-1][0],p1);
    __insere_tete(pt_objet, __cree_maillon(tmp,BLANC));
  }
  
  pt_objet->est_trie=false;
  return pt_objet;
}



t_objet3d* tore(double r1, double r2,double nlat,double nlong)
{
  t_objet3d *pt_objet = NULL;
  t_point3d *tp[(int)nlat][(int)nlong];
  pt_objet = objet_vide();
  int i,j;
  t_triangle3d *tmp;

  for(i=0;i<nlat;i++){
    for(j=0;j<nlong;j++){
      tp[i][j]=definirPoint3d(
			      (r1+r2*cos(i*2*M_PI/nlat))*sin(j*2*M_PI/nlong),
			      -r2*sin(i*2*M_PI/nlat),
			      (r1+r2*cos(i*2*M_PI/nlat))*cos(j*2*M_PI/nlong));
      __insere_tete_chaine(pt_objet,__cree_chaine(tp[i][j]));
    }
  }
  for(i=0;i<nlat-1;i++){
    for(j=0;j<nlong-1;j++){
      tmp = definirTriangle3d(tp[i][j],tp[i][j+1],tp[i+1][j+1]);
      __insere_tete(pt_objet, __cree_maillon(tmp,ROUGEC));
      tmp = definirTriangle3d(tp[i][j],tp[i+1][j],tp[i+1][j+1]);
      __insere_tete(pt_objet, __cree_maillon(tmp,BLANC));
    }
    tmp = definirTriangle3d(tp[i][(int)nlong-1],tp[i][0],tp[i+1][0]);
    __insere_tete(pt_objet, __cree_maillon(tmp,GRISC));
    tmp = definirTriangle3d(tp[i][(int)nlong-1],tp[i+1][(int)nlong-1],tp[i+1][0]);
    __insere_tete(pt_objet, __cree_maillon(tmp,GRISF));  
  }
  for(j=0;j<nlong-1;j++){
    tmp = definirTriangle3d(tp[(int)nlat-1][j],tp[0][j],tp[0][j+1]);
    __insere_tete(pt_objet, __cree_maillon(tmp,GRISC));
    tmp = definirTriangle3d(tp[(int)nlat-1][j],tp[(int)nlat-1][j+1],tp[0][j+1]);
    __insere_tete(pt_objet, __cree_maillon(tmp,GRISF)); 
  }
  tmp = definirTriangle3d(tp[(int)nlat-1][(int)nlong-1],tp[(int)nlat-1][0],tp[0][0]);
  __insere_tete(pt_objet, __cree_maillon(tmp,GRISC));
  tmp = definirTriangle3d(tp[(int)nlat-1][(int)nlong-1],tp[0][(int)nlong-1],tp[0][0]);
  __insere_tete(pt_objet, __cree_maillon(tmp,GRISF));
  
  return pt_objet;
}



t_objet3d* arbre(double lx, double ly, double lz)
{
  t_objet3d *pt_objet = NULL;

  pt_objet = objet_vide();
  // TODO

  return pt_objet;
}

t_objet3d* damier(double lx, double lz, double nx, double nz)
{
  t_objet3d *pt_objet = NULL;
  pt_objet = objet_vide();
  t_point3d *tp[(int)nz+1][(int)nx+1];
  int i,j;
  t_triangle3d *tmp1,*tmp2;
  for(i=0;i<nz+1;i++){
    for(j=0;j<nx+1;j++){
      tp[i][j]=definirPoint3d(j*lx/(int)nx-lx/2,0,i*lz/(int)nz-lz/2);
      __insere_tete_chaine(pt_objet,__cree_chaine(tp[i][j]));
    }
  }
  for(i=0;i<nz;i++){
    for(j=0;j<nx;j++){
      tmp1 = definirTriangle3d(tp[i][j],tp[i][j+1],tp[i+1][j+1]);
      tmp2 = definirTriangle3d(tp[i][j],tp[i+1][j],tp[i+1][j+1]);
      if((i+j)%2){
	__insere_tete(pt_objet, __cree_maillon(tmp1,BLANC));
	__insere_tete(pt_objet, __cree_maillon(tmp2,BLANC));
      }
      else{
	__insere_tete(pt_objet, __cree_maillon(tmp1,GRISF));
	__insere_tete(pt_objet, __cree_maillon(tmp2,GRISF));
      }
    }
  }
  pt_objet->est_trie=false;
  return pt_objet;
}

t_maillon *copierMaillon(t_maillon *pt_maillon,t_objet3d *pt_objet)
{
  t_maillon *res=NULL;
  if(pt_maillon!=NULL)
    {
      res=__cree_maillon(
			 definirTriangle3d(
					   customPoint3d(pt_maillon->face->abc[0],pt_objet),
					   customPoint3d(pt_maillon->face->abc[1],pt_objet),
					   customPoint3d(pt_maillon->face->abc[2],pt_objet)
					   ),
			 pt_maillon->couleur);
      res->pt_suiv=copierMaillon(pt_maillon->pt_suiv,pt_objet);
    }
  return res;
}

void clearCopie(t_chaine* pt_chaine){
  if(pt_chaine!=NULL){
    pt_chaine->point->copie=NULL;
    clearCopie(pt_chaine->pt_suiv);
  }
}


t_objet3d *copierObjet3d(t_objet3d *o) // attention, effectue une copie mirroir
{
  t_objet3d *n = objet_vide();
  n->est_trie=o->est_trie;
  n->est_camera=o->est_camera;
  n->tete=copierMaillon(o->tete,n);
  clearCopie(o->chaine);
  return n;
}

void composerObjet3d(t_objet3d* o, t_objet3d* o2) /* o = o+o2, o2 ne signifiera plus rien */
{
  t_maillon *tmp=o->tete;
  while(tmp->pt_suiv!=NULL){
    tmp=tmp->pt_suiv;
  }
  tmp->pt_suiv=o2->tete;

  t_chaine *tmp2=o->chaine;
  while(tmp2->pt_suiv!=NULL){
    tmp2=tmp2->pt_suiv;
  }
  tmp2->pt_suiv=o2->chaine;
  free(o2);
}

void libererMaillon(t_maillon *pt_maillon)
{
  if(pt_maillon!=NULL){ 
    libererMaillon(pt_maillon->pt_suiv);
    free(pt_maillon->face);
    free(pt_maillon);
  }
}

void libererChaine(t_chaine *pt_chaine)
{
  if(pt_chaine!=NULL){
    libererChaine(pt_chaine->pt_suiv);
    free(pt_chaine->point);
    free(pt_chaine);
  }
}

void libererObjet3d(t_objet3d *o)
{
  libererMaillon(o->tete);
  libererChaine(o->chaine);
  free(o);
}

void affi_maillon(t_maillon *pt_maillon)
{
  if(pt_maillon==NULL)
    printf("NULL\n");
  else
    {
      printf("(%f)->",pt_maillon->z_index);
      affi_maillon(pt_maillon->pt_suiv);
    }
}

t_maillon* tri_comp(t_maillon *pt_maillon,int *bool)
{
  if(pt_maillon!=NULL&&pt_maillon->pt_suiv!=NULL)
    {
      if(pt_maillon->z_index>pt_maillon->pt_suiv->z_index)
	{
	  t_maillon *tmp=pt_maillon->pt_suiv;
	  pt_maillon->pt_suiv=tmp->pt_suiv;
	  tmp->pt_suiv=pt_maillon;
	  pt_maillon=tmp;
	  *bool=*bool+1;
	}
      pt_maillon->pt_suiv=tri_comp(pt_maillon->pt_suiv,bool);
    }
  return pt_maillon;
}


//effectue un tri des faces de l'objet dans l'ordre des z decroissants => cf algorithme du peintre
void __trier_objet(t_objet3d *pt_objet)
{
  int bool;
  if(!(pt_objet->est_trie)){
    remplir_z_index(pt_objet->tete);
    do{
      bool=0;
      pt_objet->tete=tri_comp(pt_objet->tete,&bool);
    }
    while(bool!=0);
    //affi_maillon(pt_objet->tete);
    pt_objet->est_trie=true;
  }
}

void dessinerObjet3d(t_surface *surface, t_objet3d* pt_objet)
{
  __trier_objet(pt_objet);
  t_maillon *tmp=pt_objet->tete;
  while(tmp!=NULL)
    {
      remplirTriangle3d(surface, tmp->face,tmp->couleur);
      tmp=tmp->pt_suiv;
    }
}

void translationObjet3d(t_objet3d* pt_objet, t_point3d *vecteur)
{
  t_chaine *tmp=pt_objet->chaine;
  double m[4][4]={{1, 0, 0,vecteur->xyzt[0]},\
		  {0, 1, 0,vecteur->xyzt[1]},\
		  {0, 0, 1,vecteur->xyzt[2]},\
		  {0, 0, 0, 1}};
  transformationObjet3d(pt_objet,m); 
}

void rotationObjet3d(t_objet3d* pt_objet, t_point3d *centre, float degreX, float degreY, float degreZ)
{
  t_chaine *tmp=pt_objet->chaine;
  float x,y,z;
  x=degreX*M_PI/180;
  y=degreY*M_PI/180;
  z=degreZ*M_PI/180;
  double mat[4][4];
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

  multiplicationMatrice3d(mat,m,mx);
  multiplicationMatrice3d(mat,mat,my);
  multiplicationMatrice3d(mat,mat,mz);
  multiplicationMatrice3d(mat,mat,minv);
  //printf("avant transfo :\n");
  //affi_chaine(tmp);
  //printf("-------------------\n");
  transformationObjet3d(pt_objet,mat); 
  pt_objet->est_trie=false;
}

void transformationObjet3d(t_objet3d* pt_objet, double mat[4][4])
{
  t_chaine *tmp=pt_objet->chaine;
  do{
    multiplicationVecteur3d(tmp->point,mat,tmp->point);
    tmp=tmp->pt_suiv;
  }
  while(tmp!=NULL);
  pt_objet->est_trie=false;
}

