#ifndef _LIB_SCENE3D_H_
#define _LIB_SCENE3D_H_

#include "lib_3d.h"
#include "lib_objet3d.h"
#include "lib_mat.h"
#include "lib_surface.h"



typedef struct __noeud t_scene3d;
typedef struct __m_noeud m_noeud;

typedef struct
{
  double mat[4][4];
} matrice;


struct __m_noeud
{
  t_scene3d * fils;
  struct __m_noeud * pt_suiv;
};

struct __noeud
{
  matrice *mat;
  matrice *inv;
  t_objet3d * objet3d;
  int nom;
  m_noeud * lifils;
};

matrice * creerMatrice(double m[4][4]);
matrice * matTranslation(t_point3d *vecteur);
matrice * matTranslationinv(t_point3d *vecteur);
matrice * matRotation(t_point3d *centre,float degreX, float degreY, float degreZ);
matrice * matRotationinv(t_point3d *centre,float degreX, float degreY, float degreZ);



void translationScene3d(t_scene3d* pt_scene3d,t_point3d *vecteur);
void rotationScene3d(t_scene3d* pt_scene3d,t_point3d *centre, float degreX, float degreY, float degreZ);

void transformationScene3d(t_scene3d *pt_scene3d,double mat[4][4]);
void transformationFils(double mat[4][4],m_noeud * lifils);


t_scene3d * creerScene3d(t_objet3d * pt_objet3d,matrice * m1,matrice * m2,int nom);
m_noeud * __creerMNoeud(t_scene3d * elem);

void ajoutFilsNoeud(t_scene3d *pt_scene3d,t_scene3d*elem,int pos);

t_scene3d * rechercheScene3d(int val,t_scene3d* pt_scene3d);
t_scene3d * rechercheFils(int val,m_noeud * lifils);

t_scene3d * rechercheScene3dv2(int val,t_scene3d* pt_scene3d,double mat[4][4]);
t_scene3d * rechercheFilsv2(int val,m_noeud * lifils,double mat[4][4]);

void dessinerScene3d(t_surface *surface,t_scene3d * pt_scene3d,double h);
void dessinerFils(t_surface * surface,m_noeud * lifils,double h);



#endif
