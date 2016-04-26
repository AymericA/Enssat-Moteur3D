#ifndef _LIB_SCENE3D_H_
#define _LIB_SCENE3D_H_

#include "lib_3d.h"
#include "lib_objet3d.h"
#include "lib_surface.h"

typedef struct _noeud t_scene3d;

struct _noeud
{
  double mat[4][4];
  double inv[4][4];
  t_objet3d * objet3d;
  t_scene3d * pere;
  t_scene3d * fils;
  t_scene3d * frere;
};

//fonction de traduction paramètre -> matrice de transformation
void matTranslation(t_point3d*vecteur,double mat[4][4]);
void matTranslationinv(t_point3d*vecteur,double mat[4][4]);
void matRotation(t_point3d*centre,float degreX,float degreY,float degreZ,double mat[4][4]);
void matRotationinv(t_point3d*centre,float degreX,float degreY,float degreZ,double mat[4][4]);

void translationScene3d(t_scene3d*pt_scene3d,t_point3d*vecteur);
void rotationScene3d(t_scene3d*pt_scene3d,t_point3d*centre,float degreX,float degreY,float degreZ);

void transformationScene3d(t_scene3d*pt_scene3d,double mat[4][4],double inv[4][4]);

t_scene3d*creerScene3d(t_objet3d*pt_objet3d);

void ajoutObjet3d(t_scene3d*pt_scene3d,t_objet3d*pt_objet3d);

void dessinerScene3d(t_surface*surface,t_scene3d*pt_scene3d,double h);
void dessinerScene3d_rec(t_surface*surface,t_scene3d*pt_scene3d,double h,double mat[4][4],double inv[4][4]);

#endif
