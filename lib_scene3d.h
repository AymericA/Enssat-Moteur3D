#ifndef _LIB_SCENE3D_H_
#define _LIB_SCENE3D_H_

#include "lib_3d.h"
#include "lib_objet3d.h"
#include "lib_mat.h"
#include "lib_surface.h"


typedef struct __noeud t_scene3d;

struct __noeud
{
  double m[4][4];
  t_objet3d objet3d;
  struct __noeud* lifils[10];
};

t_scene3d creerVide;
void ajoutNoeud(t_scene3d elem,t_scene3d noeud,int pos);




#endif
