#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "lib_surface.h"
#include "lib_2d.h"
#include "lib_3d.h"
#include "lib_objet3d.h"


//#define T2D
//#define T3D
#define O3D

int main(int argc,char** argv)
{
  t_surface *surface=NULL;
  int cpt = 0;
  int timestart;
  char buf[50];

#ifdef T2D
  t_point2d *p1 = definirPoint2d(10,50), *p2 = definirPoint2d(100,240), *p3 = definirPoint2d(50,300);
  t_triangle2d *t1 = definirTriangle2d(p1, p2, p3);
#endif

#ifdef T3D
  t_point3d *p10 = definirPoint3d(0,0,0), *p20 = definirPoint3d(100,0,0), *p30 = definirPoint3d(100,100,0);
  //t_point3d *p11 = definirPoint3d(0,0,0), *p21 = definirPoint3d(30,30,0), *p31 = definirPoint3d(30,0,0);
  t_point3d *pt=definirVecteur3d(0,0,0);
  t_triangle3d *t10 = definirTriangle3d(p10, p20, p30);
  //t_triangle3d *t20 = definirTriangle3d(p11, p21, p31);
#endif

#ifdef O3D
  t_point3d *origine = definirPoint3d(0,0,0), *vecteur;
  t_objet3d *cube=parallelepipede(200,200,200);
  //rotationObjet3d(cube,origine,5,0,0);
  //t_objet3d *o10 = sphere_amiga(80, 8, 16);
  //rotationObjet3d(o10, origine, 90, 0,0);
#endif

  int i=0;

  surface=creerFenetre(RX,RY);
  timestart = SDL_GetTicks();


  while(i<25)
    {
      effacerFenetre(surface, 0);

#ifdef T2D
      remplirTriangle2d(surface, t1, echelle_de_couleur(30*i));
      SDL_Delay(500);
#endif
      
#ifdef T3D
      rotationTriangle3d(t10, pt, 0, 10, 0);      
      remplirTriangle3d(surface, t10, echelle_de_couleur(30*i));
      SDL_Delay(40);
#endif

#ifdef O3D
      //vecteur = definirPoint3d(5*sin(i*M_PI/180),5*cos(i*M_PI/180),0);
      vecteur = definirPoint3d(50,50,0);
      //translationObjet3d(o10, vecteur);
      //affi_chaine(cube->chaine);
      printf("\n\n");
      dessinerObjet3d(surface,cube);
      check(cube);
      rotationObjet3d(cube,origine,0,30,0);
      //translationObjet3d(cube,vecteur);
      free(vecteur);
      SDL_Delay(120);
#endif

      majEcran(surface);

      i += 1;

      cpt++;
      sprintf(buf,"%d FPS",(int)(cpt*1000.0/(SDL_GetTicks()-timestart)));
      SDL_WM_SetCaption(buf,NULL);
    }
  SDL_Quit();
  return 0;
}
