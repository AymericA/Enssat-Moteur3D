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
  t_point3d *origine = definirPoint3d(0,0,0), *vecteur,*centre=definirPoint3d(0,0,0);
  double h = -500;

  /*
  
  t_objet3d *cube=parallelepipede(400,400,400);
  //rotationObjet3d(cube,origine,0,0,-10);

  vecteur = definirPoint3d(0,0,-1400);
  translationObjet3d(cube, vecteur);
  centre=definirPoint3d(0,0,-1400);
  */
  /*
  t_objet3d *sph1=sphere(400,10,20);

 vecteur = definirPoint3d(0,0,-1400);
  translationObjet3d(sph1, vecteur);
  centre=definirPoint3d(0,0,-1400);
  */
  /*
  t_objet3d *sph3=sphere(50,10,20);
  vecteur = definirPoint3d(0,150,0);
  translationObjet3d(sph3,vecteur);

  t_objet3d *sph12=sphere(50,10,20);
  vecteur = definirPoint3d(150,0,0);
  translationObjet3d(sph12,vecteur);

  t_objet3d *sph32=sphere(50,10,20);
  vecteur = definirPoint3d(-150,0,0);
  translationObjet3d(sph32,vecteur);


  t_objet3d *sph2=sphere_amiga(100,10,20);
  composerObjet3d(sph2,sph1);
  composerObjet3d(sph2,sph3);
  composerObjet3d(sph2,sph12);
  composerObjet3d(sph2,sph32);
  */

  t_objet3d *plan=damier(200,200,3,3);
  rotationObjet3d(plan,origine,90,0,0);
  vecteur = definirPoint3d(0,0,-1000);
  translationObjet3d(plan, vecteur);
  centre=definirPoint3d(0,0,-1000);

  t_objet3d *n=n64(200);
  vecteur = definirPoint3d(0,0,-1000);
  translationObjet3d(n, vecteur);
  centre=definirPoint3d(0,0,-1000);


  t_objet3d *to=tore(200,50,5,10);
  t_objet3d *cyli=cylindre(200,100,3);
  //t_objet3d *bou=copierObjet3d(to);
  //composerObjet3d(to,cyli);
  t_objet3d *ami = sphere_amiga(80, 7, 16);
  //rotationObjet3d(ami, origine,-90, 0,0);

  //rotationObjet3d(sph2,origine,-90,0,0);
  //rotationObjet3d(sph2,origine,0,-90,0);
#endif

  int i=0;

  surface=creerFenetre(RX,RY);
  timestart = SDL_GetTicks();


  while(i<25*10)
    {
      //printf("%deme passe:\n",i);
      init();
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
      vecteur = definirPoint3d(0,0,100*sin(5*i*M_PI/180));
      //vecteur = definirPoint3d(0,0,-10);
      //translationObjet3d(o10, vecteur);

      //dessinerObjet3d(surface,sph1);
      //dessinerObjet3d(surface,plan);
      //dessinerObjet3d(surface,sph2);
      //rotationObjet3d(sph2,origine,0,5,0);
      //translationObjet3d(sph2,vecteur);
      //dessinerObjet3d(surface,ami);
      //rotationObjet3d(ami,origine,0,5,0);
      

      dessinerObjet3d(surface,n,h,i);
      rotationObjet3d(n,centre,0,5,0);
      

      //translationObjet3d(plan, vecteur);

     //centre=definirPoint3d(0,0,(i+1)*-1);

      //printf("coor en z %f\n",cube->tete->face->abc[0]->xyzt[2]);

      //dessinerObjet3d(surface,n);
      //rotationObjet3d(n,origine,0,5,0);

      //dessinerObjet3d(surface,geo);
      //rotationObjet3d(geo,origine,0,5,0);

      //printf("%deme passe:\n",i);

      
      //rotationObjet3d(sph1,origine,0,-5,0);
      //rotationObjet3d(sph2,origine,0,5,0);
      //translationObjet3d(plan,vecteur);


      //free(vecteur);
      //printf("\n\n");
      
#endif
      

      screen[10][10].couleur=BLANC;
      afficherFenetre(surface,screen);
      majEcran(surface);
      SDL_Delay(40);
      i += 1;

      cpt++;
      sprintf(buf,"%d FPS",(int)(cpt*1000.0/(SDL_GetTicks()-timestart)));
      SDL_WM_SetCaption(buf,NULL);
    }
  SDL_Quit();
  return 0;
}
