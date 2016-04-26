#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "lib_surface.h"
#include "lib_2d.h"
#include "lib_3d.h"
#include "lib_objet3d.h"
#include "lib_scene3d.h"

//#define T2D
//#define T3D
//#define O3D
#define S3D

int main(int argc,char** argv)
{
  t_surface *surface=NULL;
  int cpt = 0;
  int timestart;
  char buf[50];
  SDL_Event event;

  t_bool fin=true;
  int turnlr=0,turnhb=0;

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
  double h = -800;

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
  rotationObjet3d(plan,origine,90,0,45);
  vecteur = definirPoint3d(0,0,-1000);
  translationObjet3d(plan, vecteur);
  centre=definirPoint3d(0,0,-1000);

  t_objet3d *n=n64(400);
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

  
#ifdef S3D
  t_point3d *origine = definirPoint3d(0,0,0);
  t_point3d *vecteur;
  t_point3d *unitaire = definirPoint3d(1,1,1);
  t_point3d *translation;
  int Tx=0,Ty=0,Tz=0;
  int Rx=0,Ry=0,Rz=0;

  double mat[4][4];
  double inv[4][4];

  double tmpmat[4][4];
  double tmpinv[4][4];

  double h = -500;
  
  t_objet3d*c1=camera();

  t_objet3d*p1=damier(500,500,10,10);
  t_objet3d*s1=sphere(100,10,20);
  t_objet3d*t1=tore(100,10,10,50);

  t_scene3d*scene=creerScene3d(c1);

  ajoutObjet3d(scene,p1);
  t_scene3d*sp1=scene->fils;

  ajoutObjet3d(sp1,s1);
  t_scene3d*ss1=sp1->fils;
  
  ajoutObjet3d(ss1,t1);
  t_scene3d*st1=ss1->fils;

  t_point3d *centre = definirPoint3d(0,0,-500);

  vecteur=definirPoint3d(0,150,-500);
  translationScene3d(sp1,vecteur);
  free(vecteur);

  vecteur=definirPoint3d(0,-150,0);
  translationScene3d(ss1,vecteur);
  free(vecteur);

  vecteur=definirPoint3d(0,100,0);
  translationScene3d(st1,vecteur);
  free(vecteur);
 
#endif

  int i=0;

  surface=creerFenetre(RX,RY);
  timestart = SDL_GetTicks();

  while(i<10000 && fin)
    {
      printf("%deme passe:\n",i);
      init();
      effacerFenetre(surface,0);

#ifdef T2D
      remplirTriangle2d(surface, t1, echelle_de_couleur(30*i));
#endif
      
#ifdef T3D
      rotationTriangle3d(t1, origine,0,0,0);      
      remplirTriangle3d(surface, t1, ROUGEF);
#endif

#ifdef O3D
      vecteur = definirPoint3d(0,0,7*sin(i*M_PI/180));   
      dessinerObjet3d(surface,plan,h);
      translationObjet3d(plan,vecteur);
      free(vecteur);
#endif

#ifdef S3D
      
      dessinerScene3d(surface,scene,h);
      
      SDL_PollEvent(&event);
      switch(event.type){
	case SDL_KEYDOWN:
	  switch(event.key.keysym.sym){
	  case SDLK_KP9:
	    Ty++;
	    break;
	  case SDLK_KP3:
	    Ty--;
	    break;
	  case SDLK_KP6:
	    Tx--;
	    break;
	  case SDLK_KP4:
	    Tx++;
	    break;
	  case SDLK_KP8:
	    Tz++;
	    break;
	  case SDLK_KP2:
	    Tz--;
	    break;
	  case SDLK_a:
	    Rz++;
	    break;
	  case SDLK_e:
	    Rz--;
	    break;
	  case SDLK_q:
	    Ry++;
	    break;
	  case SDLK_d:
	    Ry--;
	    break;
	  case SDLK_z:
	    Rx--;
	    break;
	  case SDLK_s:
	    Rx++;
	    break;
	  case SDLK_SPACE:
	    Rx=0;
	    Ry=0;
	    Rz=0;
	    Tx=0;
	    Ty=0;
	    Tz=0;
	    break;
	  case SDLK_ESCAPE:
	    fin=false;
	    break;
	  }
	  break;
      }
      /*
      matTranslation(unitaire,mat);
      matTranslation(unitaire,inv);
      matTranslation(unitaire,tmpmat);
      matTranslation(unitaire,tmpinv);
      */

      //vecteur=definirPoint3d(0,0,-500);

      rotationScene3d(ss1,centre,0,1,0);

      //free(vecteur);

      translation=definirPoint3d(Tx,Ty,Tz);
      translationScene3d(scene,translation);
      
      
      /*
      matRotation(origine,Rx,Ry,Rz,tmpmat);
      matRotationinv(origine,Rx,Ry,Rz,tmpinv);
      
      multiplicationMatrice3d(mat,tmpmat,mat);
      multiplicationMatrice3d(inv,tmpinv,inv);
      */
      
      /*
      if(Rx<0)
	rotationScene3d(scene,origine,-1,0,0);
      if(Ry<0)
	rotationScene3d(scene,origine,0,-1,0);
      if(Rz<0)
	rotationScene3d(scene,origine,0,0,-1);
      if(Rx>0)
	rotationScene3d(scene,origine,1,0,0);
      if(Ry>0)
	rotationScene3d(scene,origine,0,1,0);
      if(Rz>0)
	rotationScene3d(scene,origine,0,0,1);
      */
      free(translation);
      
   
#endif

      afficherFenetre(surface,screen);
      majEcran(surface);
      SDL_Delay(20);
      i++;

      cpt++;
      sprintf(buf,"%d FPS",(int)(cpt*1000.0/(SDL_GetTicks()-timestart)));
      SDL_WM_SetCaption(buf,NULL);
    }
  SDL_Quit();
  return 0;
}
