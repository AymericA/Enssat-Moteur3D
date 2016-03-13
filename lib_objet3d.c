#include <assert.h>
#include <math.h>
#include "lib_3d.h"
#include "lib_objet3d.h"
#include "lib_mat.h"
#include "lib_surface.h"


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

  return pt_objet;
}

t_objet3d* sphere(double r, double nlat, double nlong)
{
  t_objet3d *pt_objet = NULL;

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


//insere un maillon dans une liste en fonction des z_index
void __insertion_z_index(t_maillon *pt_maillon,t_maillon *liste)
{
  if(liste==NULL){
    pt_maillon->pt_suiv=NULL;
    liste=pt_maillon;
  }
  else{
    if(pt_maillon->z_index<liste->z_index){
      pt_maillon->pt_suiv=liste;
      liste=pt_maillon;
    }
    else{
      __insertion_z_index(pt_maillon,liste->pt_suiv);
    }
  }
}


//trie la chaine de maillon en fonction du z_index
void __trier_maillon(t_maillon *pt_maillon)
{
  if(pt_maillon!=NULL)
    __insertion_z_index(pt_maillon,pt_maillon->pt_suiv);
}


//effectue un tri des faces de l'objet dans l'ordre des z decroissants => cf algorithme du peintre
void __trier_objet(t_objet3d *pt_objet)
{
  if(!(pt_objet->est_trie)){
    __trier_maillon(pt_objet->tete);
  }
  pt_objet->est_trie=true;
}

void dessinerObjet3d(t_surface *surface, t_objet3d* pt_objet)
{
  __trier_objet(pt_objet);
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

