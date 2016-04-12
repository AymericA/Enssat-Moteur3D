#ifndef _LIB_SCENE3D_H_
#define _LIB_SCENE3D_H_

#include "lib_3d.h"
#include "lib_objet3d.h"
#include "lib_mat.h"
#include "lib_surface.h"


typedef struct __noeud t_scene3d;
typedef struct __m_noeud m_noeud;

struct __m_noeud
{
  t_scene3d * fils;
  struct __m_noeud * pt_suiv;
};

struct __noeud
{
  double m[4][4];
  t_objet3d * objet3d;
  int nom;
  m_noeud * lifils;
};

t_scene3d * creerVide();
void ajoutNoeud(t_scene3d *elem,t_scene3d *noeud,int pos);
m_noeud * __creerMNoeud(t_scene3d * elem);
m_noeud * ajoutFilsNoeud(t_scene3d * elem,m_noeud *pm_noeud,int pos);
t_scene3d * rechercheScene3d(int val,t_scene3d* pt_scene3d);
t_scene3d * rechercheFils(int val,m_noeud * lifils);
void dessinerScene3d(t_surface *surface,t_scene3d * pt_scene3d,double h);
void dessinerFils(t_surface * surface,m_noeud * lifils,double h);



#endif
