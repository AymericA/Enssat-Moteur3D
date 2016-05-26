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

t_point3d*Getcentre(t_scene3d*pt_scene3d);
void Getcentre_rec(double mat[4][4],t_scene3d*pt_scene3d);
t_point3d*GetcentreR(t_scene3d*pt_scene3d);

void translationScene3d(t_scene3d*pt_scene3d,t_point3d*vecteur);
void rotationScene3d(t_scene3d*pt_scene3d,t_point3d*centre,float degreX,float degreY,float degreZ);

void transformationScene3d(t_scene3d*pt_scene3d,double mat[4][4],double inv[4][4]);

t_scene3d*creerScene3d(t_objet3d*pt_objet3d);

void ajoutObjet3d(t_scene3d*pt_scene3d,t_objet3d*pt_objet3d);
void ajoutfils(t_scene3d*pere,t_scene3d*fils);

void dessinerScene3d(t_surface*surface,t_scene3d*pt_scene3d,double h);
void dessinerScene3d_rec(t_surface*surface,t_scene3d*pt_scene3d,double h,double mat[4][4],double inv[4][4]);

void affscene(t_scene3d*scene);

void Racine(t_scene3d*scene);
void extract(t_scene3d*pt_scene3d);

t_scene3d*dragon(t_scene3d** tab[5],Uint32*tabdc[5]);
void mer_init(int nx,int nz,Uint32 tabc[nx][nz]);
t_scene3d*mer(double lx,double ly,int nx,int nz,Uint32 tabc[nx][nz],t_scene3d*tab[nx][nz]);
Uint32 next(int lon,Uint32 val,const Uint32 tab[lon]);
void Umer(int nx,int nz,int freqx,int freqz,t_scene3d*tab[nx][nz],int cycle,Uint32 tabc[nx][nz]);

void kraken_init(int nb,int sec,Uint32 tabc[nb][sec]);
t_scene3d*tentacle(int nb,int sec,Uint32 tabc[nb][sec],t_scene3d*tab[nb][sec],double r,double rapr,double lh,double raph);
void Ukraken(int nb,int sec,t_scene3d*tab[nb][sec],int kinfo[nb][sec],double lh,double raph,int cycle);
void kapp(t_scene3d*baset,t_bool btabk[3],int ckapp);
 
#endif
