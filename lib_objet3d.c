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

t_chaine* __cree_chaine(t_point3d *point)
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
  else
    {
      affPoint(pt_chaine->point);
      printf("-->");
      affi_chaine(pt_chaine->pt_suiv);
    }
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
  return pt_objet;
}

t_objet3d *camera()
{
  t_objet3d *pt_objet = malloc(sizeof(t_objet3d));
  pt_objet->est_trie=true;
  pt_objet->est_camera=true;
  pt_objet->tete=NULL;
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
  //t_point3d *p4 = definirPoint3d(-x,-y,z);

  t_point3d *p5 = definirPoint3d(x,y,-z);
  //t_point3d *p6 = definirPoint3d(-x,y,-z);

  //t_point3d *p7 = definirPoint3d(x,-y,-z);
  //t_point3d *p8 = definirPoint3d(-x,-y,-z);

  __insere_tete_chaine(pt_objet,__cree_chaine(p1));
  __insere_tete_chaine(pt_objet,__cree_chaine(p2));
  __insere_tete_chaine(pt_objet,__cree_chaine(p3));
  //__insere_tete_chaine(pt_objet,__cree_chaine(p4));
  //__insere_tete_chaine(pt_objet,__cree_chaine(p5));
  //__insere_tete_chaine(pt_objet,__cree_chaine(p6));
  //__insere_tete_chaine(pt_objet,__cree_chaine(p7));
  //__insere_tete_chaine(pt_objet,__cree_chaine(p8));

  t_triangle3d *t1 = definirTriangle3d(p1, p2, p3);
  //t_triangle3d *t2 = definirTriangle3d(p2, p3, p4);

  t_triangle3d *t3 = definirTriangle3d(p1, p3, p5);
  //t_triangle3d *t4 = definirTriangle3d(p3, p5, p7);

  //t_triangle3d *t5 = definirTriangle3d(p1, p2, p5);
  //t_triangle3d *t6 = definirTriangle3d(p2, p5, p6);

  //t_triangle3d *t7 = definirTriangle3d(p4, p6, p8);
  //t_triangle3d *t8 = definirTriangle3d(p2, p4, p6);

  //t_triangle3d *t9 = definirTriangle3d(p6, p7, p8);
  //t_triangle3d *t10 = definirTriangle3d(p5, p6, p7);

  //t_triangle3d *t11 = definirTriangle3d(p4, p7, p8);
  //t_triangle3d *t12 = definirTriangle3d(p3, p4, p7);

  __insere_tete(pt_objet, __cree_maillon(t1,ROUGEF));
  //__insere_tete(pt_objet, __cree_maillon(t2,ROUGEC));

  __insere_tete(pt_objet, __cree_maillon(t3,VERTF));
  //__insere_tete(pt_objet, __cree_maillon(t4,VERTC));

  //__insere_tete(pt_objet, __cree_maillon(t5,BLEUF));
  //__insere_tete(pt_objet, __cree_maillon(t6,BLEUC));

  //__insere_tete(pt_objet, __cree_maillon(t7,JAUNEF));
  //__insere_tete(pt_objet, __cree_maillon(t8,JAUNEC));

  //__insere_tete(pt_objet, __cree_maillon(t9,ROSEF));
  //__insere_tete(pt_objet, __cree_maillon(t10,ROSEC));

  //__insere_tete(pt_objet, __cree_maillon(t11,GRISC));
  //__insere_tete(pt_objet, __cree_maillon(t12,BLANC));

  pt_objet->est_trie=false;
  return pt_objet;
}

t_objet3d* sphere(double r, double nlat, double nlong)
{
  t_objet3d *pt_objet = NULL;

  t_point3d tab[(int)nlat][(int)nlong];
  int i,j;
  
  for(i=0;i<nlat;i++){
    for(j=0;j<nlong;j++){
      //tab[i][j]=definirPoint3d(-r*cos(i*M_PI/nlat),r*sin(i*M_PI/nlong)*sin(j*2*M_PI/nlong),r*sin(i*M_PI/nlat)*cos(j*2*M_PI/nlong));
    }
  }
  pt_objet = objet_vide();

  // TODO

  return pt_objet;
}

t_objet3d* sphere_amiga(double r, double nlat, double nlong)
{
  t_objet3d *pt_objet = NULL;

  pt_objet = objet_vide();

  // TODO


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

  // TODO

  return pt_objet;

}

t_objet3d *copierObjet3d(t_objet3d *o) // attention, effectue une copie mirroir
{
  t_objet3d *n = objet_vide();

  // TODO

  return n;
}

void composerObjet3d(t_objet3d* o, t_objet3d* o2) /* o = o+o2, o2 ne signifiera plus rien */
{

  // TODO

}

void libererObjet3d(t_objet3d *o)
{
  // TODO

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
    affi_maillon(pt_objet->tete);
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
  do{
    multiplicationVecteur3d(tmp->point,m,tmp->point);
    tmp=tmp->pt_suiv;
  }
  while(tmp!=NULL);
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
  do{
    multiplicationVecteur3d(tmp->point,mat,tmp->point);
    tmp=tmp->pt_suiv;
  }
  while(tmp!=NULL);
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

