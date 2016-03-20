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
  t_point3d *p1 = definirPoint3d(100,100,100);
  t_point3d *p2 = definirPoint3d(-100,100,100);
  t_point3d *p3 = definirPoint3d(100,-50,100);

  t_point3d *p01 = definirPoint3d(100,100,100);
  t_point3d *p03 = definirPoint3d(100,-100,100);
  t_point3d *p05 = definirPoint3d(100,100,-100);

  t_point3d *origine = definirPoint3d(0,0,0);

  t_triangle3d *t1 = definirTriangle3d(p1, p2, p3);

  t_triangle3d *t3 = definirTriangle3d(p01, p03, p05);

  t_point3d *vecteur=definirVecteur3d(0,0,0);
  
#endif

#ifdef O3D
  t_point3d *origine = definirPoint3d(0,0,0), *vecteur;
  //t_objet3d *cube=parallelepipede(200,200,200);
  //rotationObjet3d(cube,origine,0,0,-10);

  /*
  t_objet3d *sph1=sphere(50,10,20);
  vecteur = definirPoint3d(0,-150,0);
  translationObjet3d(sph1,vecteur);

  t_objet3d *sph3=sphere(50,10,20);
  vecteur = definirPoint3d(0,150,0);
  translationObjet3d(sph3,vecteur);

  t_objet3d *sph2=sphere(100,10,20);
  composerObjet3d(sph2,sph1);
  composerObjet3d(sph2,sph3);

  t_objet3d *plan=damier(200,200,10,10);
  rotationObjet3d(plan,origine,-90,0,0);
  */

  t_objet3d *geo=geode(200);



  //rotationObjet3d(sph2,origine,-90,0,0);
  //rotationObjet3d(sph2,origine,0,-90,0);
  //t_objet3d *o10 = sphere_amiga(80, 8, 16);
  //rotationObjet3d(o10, origine, 90, 0,0);
#endif

  int i=0;

  surface=creerFenetre(RX,RY);
  timestart = SDL_GetTicks();


  while(i<25*5)
    {
      effacerFenetre(surface, 0);

#ifdef T2D
      remplirTriangle2d(surface, t1, echelle_de_couleur(30*i));
      SDL_Delay(500);
#endif
      
#ifdef T3D
      //translationTriangle3d(t1,vecteur);
      rotationTriangle3d(t1, origine,0,0,0);      
      remplirTriangle3d(surface, t1, ROUGEF);
      //remplirTriangle3d(surface, t3, VERTF);
      SDL_Delay(40);
#endif

#ifdef O3D
      //vecteur = definirPoint3d(5*sin(i*M_PI/180),5*cos(i*M_PI/180),0);
      //vecteur = definirPoint3d(0,0,-5);
      //translationObjet3d(o10, vecteur);

      //dessinerObjet3d(surface,sph1);
      //dessinerObjet3d(surface,plan);
      //dessinerObjet3d(surface,sph2);
   
      dessinerObjet3d(surface,geo);
      rotationObjet3d(geo,origine,0,5,0);

      //printf("%deme passe:\n",i);

      
      //rotationObjet3d(sph1,origine,0,-5,0);
      //rotationObjet3d(sph2,origine,5,5,2);
      //translationObjet3d(plan,vecteur);


      //free(vecteur);
      //printf("\n\n");
      SDL_Delay(50);
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
