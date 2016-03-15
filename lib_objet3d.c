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

void remplir_z_index(t_maillon *pt_maillon){
  if(pt_maillon!=NULL){
    pt_maillon->z_index=moy_z(pt_maillon->face);
  }
}

t_maillon* __cree_maillon(t_triangle3d *face, Uint32 couleur)
{
  t_maillon* pt_maillon = malloc(sizeof(t_maillon));
  pt_maillon->face=face;
  pt_maillon->couleur=couleur;
  pt_maillon->pt_suiv=NULL;
  //printf("valeur avant : %f\n",moy_z(face));
  pt_maillon->z_index=moy_z(face);
  //printf("valeur insérer : %f\n",(pt_maillon->z_index));
  return pt_maillon;
}


void __insere_tete(t_objet3d *pt_objet, t_maillon *pt_maillon)
{
  pt_maillon->pt_suiv=pt_objet->tete;
  pt_objet->tete=pt_maillon;
  (pt_objet->nb_face)++;
}

t_objet3d *objet_vide()
{
  t_objet3d *pt_objet = malloc(sizeof(t_objet3d));
  pt_objet->est_trie=true;
  pt_objet->est_camera=false;
  pt_objet->tete=NULL;
  pt_objet->nb_face=0;
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

t_objet3d* objtest(double x,double y,double z){

  t_objet3d *pt_objet = NULL;
  pt_objet = objet_vide();
  t_point3d *p11 = definirPoint3d(x,x,x);
  t_point3d *p12 = definirPoint3d(y,y,y);
  t_point3d *p13 = definirPoint3d(z,z,z);

  t_triangle3d *t1 = definirTriangle3d(p11, p12, p13);

  t_point3d *p21 = definirPoint3d(x,x,x);
  t_point3d *p22 = definirPoint3d(x,x,z);
  t_point3d *p23 = definirPoint3d(x,z,z);

  t_triangle3d *t2 = definirTriangle3d(p21, p22, p23);

  t_point3d *p31 = definirPoint3d(x,x,x);
  t_point3d *p32 = definirPoint3d(x,x,y);
  t_point3d *p33 = definirPoint3d(x,y,y);

  t_triangle3d *t3 = definirTriangle3d(p31, p32, p33);

  __insere_tete(pt_objet, __cree_maillon(t1,ROUGEF));
  __insere_tete(pt_objet, __cree_maillon(t2,ROUGEC));
  __insere_tete(pt_objet, __cree_maillon(t3,VERTF));

  //remplir_z_index(pt_objet->tete);
  pt_objet->est_trie=false;
  return pt_objet;
}


t_objet3d* parallelepipede(double lx, double ly, double lz)
{
  t_objet3d *pt_objet = NULL;
  pt_objet = objet_vide();
  double x=lx/2,y=ly/2,z=lz/2;

  t_point3d *p11 = definirPoint3d(x,y,z);
  t_point3d *p12 = definirPoint3d(-x,y,z);
  t_point3d *p13 = definirPoint3d(-x,-y,z);

  t_triangle3d *t1 = definirTriangle3d(p11, p12, p13);

  t_point3d *p21 = definirPoint3d(x,y,z);
  t_point3d *p22 = definirPoint3d(x,-y,z);
  t_point3d *p23 = definirPoint3d(-x,-y,z);

  t_triangle3d *t2 = definirTriangle3d(p21, p22, p23);

  t_point3d *p31 = definirPoint3d(x,y,z);
  t_point3d *p32 = definirPoint3d(x,-y,z);
  t_point3d *p33 = definirPoint3d(x,-y,-z);

  t_triangle3d *t3 = definirTriangle3d(p31, p32, p33);

  t_point3d *p41 = definirPoint3d(x,y,z);
  t_point3d *p42 = definirPoint3d(x,y,-z);
  t_point3d *p43 = definirPoint3d(x,-y,-z);

  t_triangle3d *t4 = definirTriangle3d(p41, p42, p43);

  t_point3d *p51 = definirPoint3d(x,y,z);
  t_point3d *p52 = definirPoint3d(x,y,-z);
  t_point3d *p53 = definirPoint3d(-x,y,-z);

  t_triangle3d *t5 = definirTriangle3d(p51, p52, p53);

  t_point3d *p61 = definirPoint3d(x,y,z);
  t_point3d *p62 = definirPoint3d(-x,y,z);
  t_point3d *p63 = definirPoint3d(-x,y,-z);

  t_triangle3d *t6 = definirTriangle3d(p61, p62, p63);

  t_point3d *p71 = definirPoint3d(-x,y,z);
  t_point3d *p72 = definirPoint3d(-x,-y,z);
  t_point3d *p73 = definirPoint3d(-x,-y,-z);

  t_triangle3d *t7 = definirTriangle3d(p71, p72, p73);

  t_point3d *p81 = definirPoint3d(-x,y,z);
  t_point3d *p82 = definirPoint3d(-x,y,-z);
  t_point3d *p83 = definirPoint3d(-x,-y,-z);

  t_triangle3d *t8 = definirTriangle3d(p81, p82, p83);

  t_point3d *p91 = definirPoint3d(x,y,-z);
  t_point3d *p92 = definirPoint3d(-x,y,z);
  t_point3d *p93 = definirPoint3d(-x,-y,-z);

  t_triangle3d *t9 = definirTriangle3d(p91, p92, p93);

  t_point3d *p101 = definirPoint3d(x,y,-z);
  t_point3d *p102 = definirPoint3d(x,-y,-z);
  t_point3d *p103 = definirPoint3d(-x,-y,-z);

  t_triangle3d *t10 = definirTriangle3d(p101, p102, p103);

  t_point3d *p111 = definirPoint3d(x,-y,z);
  t_point3d *p112 = definirPoint3d(x,-y,-z);
  t_point3d *p113 = definirPoint3d(-x,-y,-z);

  t_triangle3d *t11 = definirTriangle3d(p111, p112, p113);

  t_point3d *p121 = definirPoint3d(x,-y,z);
  t_point3d *p122 = definirPoint3d(-x,-y,z);
  t_point3d *p123 = definirPoint3d(-x,-y,-z);

  t_triangle3d *t12 = definirTriangle3d(p121, p122, p123);


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

t_objet3d* sphere(double r, double nlat, double nlong)
{
  t_objet3d *pt_objet = NULL;

  t_point3D tab[nlat][nlong];
  int i,j;
  
  for(i=0;i<nlat;i++){
    for(j=0;j<nlong;j++){
      tab[i][j]=definirPoint3d(-r*cos(i*pi/nlat),r*sin(i*pi/nlong)*sin(j*2*pi/nlong),r*sin(i*pi/nlat)*cos(j*2*pi/nlong));
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

void split(t_maillon *liste,int pos,t_maillon *tete,t_maillon *queue)
{
  if(pos!=0&&pos!=1){
    int i=pos;
    t_maillon *tmp=liste;
    while(i>1){
      tmp=tmp->pt_suiv;
      i--;
    }
    queue=tmp->pt_suiv;
    tmp->pt_suiv=NULL;
    tete=liste;
  }
}

void __fus(t_maillon *res,t_maillon *chaine1,t_maillon *chaine2);

void __tri_fus(t_maillon *pt_maillon,int nb_face)
{
  if(nb_face!=0&&nb_face!=1){
    t_maillon *tete,*queue;
    split(pt_maillon,nb_face/2+nb_face%2,tete,queue);
    __tri_fus(tete,nb_face/2+nb_face%2);
    __tri_fus(queue,nb_face/2);
    __fus(pt_maillon,tete,queue);
  }
}

void __insersion_q(t_maillon *l1, t_maillon *l2)
{
  if(l1==NULL){
    l1=l2;
  }
  else{
    __insersion_q(l1->pt_suiv,l2);
  }
}

void __insersion_t(t_maillon *l1, t_maillon *l2)
{
  t_maillon *tmp;
  if(l2!=NULL){
    if(l1==NULL){
      l1=l2;
    }
    else{
      tmp=l2->pt_suiv;
      l2->pt_suiv=l1;
      l2=tmp;
    }
  }
}

void __fus(t_maillon *res,t_maillon *chaine1,t_maillon *chaine2)
{
  if(chaine1==NULL){
    __insersion_q(res,chaine2);
  }
  if(chaine2==NULL){
    __insersion_q(res,chaine1);
  }
  else{
    if(chaine1->z_index<chaine2->z_index){
      __insersion_t(res,chaine1);
      __fus(res,chaine1,chaine2);
    }
    else{
      __insersion_t(res,chaine2);
      __fus(res,chaine1,chaine2);
    }
  }
}

//effectue un tri des faces de l'objet dans l'ordre des z decroissants => cf algorithme du peintre
void __trier_objet(t_objet3d *pt_objet)
{
  if(!(pt_objet->est_trie)){
    t_maillon *tmp = pt_objet->tete;
    __tri_fus(tmp,pt_objet->nb_face);
  }
  pt_objet->est_trie=true;
}

void dessinerObjet3d(t_surface *surface, t_objet3d* pt_objet)
{
  //__trier_objet(pt_objet);
  t_maillon *tmp=pt_objet->tete;
  do{
    remplirTriangle3d(surface, tmp->face,tmp->couleur);
    tmp=tmp->pt_suiv;
  }
  while(tmp!=NULL);

}

void translationObjet3d(t_objet3d* pt_objet, t_point3d *vecteur)
{
  // TODO

}

void rotationObjet3d(t_objet3d* pt_objet, t_point3d *centre, float degreX, float degreY, float degreZ)
{
  t_maillon *tmp=pt_objet->tete;
  do{
    rotationTriangle3d(tmp->face,centre,degreX,degreY,degreZ);
    tmp->z_index=moy_z(tmp->face);
    tmp=tmp->pt_suiv;
  }
  while(tmp!=NULL);
  pt_objet->est_trie=false;
}

void transformationObjet3d(t_objet3d* pt_objet, double mat[4][4])
{
  // TODO

}

