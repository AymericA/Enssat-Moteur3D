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

#endif

#ifdef T3D
 
#endif

#ifdef O3D
 
#endif
  
#ifdef S3D
  t_bool is_button_down=false;
  double x,y;
  t_point3d *origine = definirPoint3d(0,0,0);
  t_point3d *vecteur,*centre;
  t_point3d *unitaire = definirPoint3d(1,1,1);
  t_point3d *translation;
  double Tx=0,Ty=0,Tz=0;
  double Rx=0,Ry=0,Rz=0;

  int dx=0,dy=0,dz=0;
  int drx=0,dry=0,drz=0;

  double mat[4][4];
  double inv[4][4];

  double tmpmat[4][4];
  double tmpinv[4][4];

  double h = -500;
  
  t_objet3d*c1=camera();
  t_objet3d*c2=camera();
  
  t_objet3d*p1=damier(800,800,16,16);
  t_objet3d*s1=sphere(100,10,20);
  t_objet3d*t1=tore(100,10,10,50);
  t_objet3d*n1=n64(100);

  t_scene3d*scene;

  t_scene3d*sc1=creerScene3d(c1);

  ajoutObjet3d(sc1,p1);
  t_scene3d*sp1=sc1->fils;

  ajoutObjet3d(sp1,s1);
  t_scene3d*ss1=sp1->fils;
  
  ajoutObjet3d(ss1,t1);
  t_scene3d*st1=ss1->fils;

  ajoutObjet3d(sp1,c2);
  t_scene3d*sc2=sp1->fils;

  ajoutObjet3d(sp1,n1);
  t_scene3d*sn1=sp1->fils;

  scene=sc1;

  vecteur=definirPoint3d(0,150,-500);
  translationScene3d(sp1,vecteur);
  free(vecteur);

  vecteur=definirPoint3d(0,-150,0);
  translationScene3d(ss1,vecteur);
  free(vecteur);

  vecteur=definirPoint3d(0,100,0);
  translationScene3d(st1,vecteur);
  free(vecteur);

  vecteur=definirPoint3d(500,-150,0);
  translationScene3d(sc2,vecteur);
  free(vecteur);

  vecteur=definirPoint3d(200,-50,0);
  translationScene3d(sn1,vecteur);
  free(vecteur);

  vecteur=GetcentreR(sc2);
  rotationScene3d(sc2,vecteur,0,90,0);
  free(vecteur);

#endif

  int i=0;

  surface=creerFenetre(RX,RY);
  timestart = SDL_GetTicks();

  while(i<10000 && fin)
    {
      //printf("\n%deme passe:\n",i);
      init();
      effacerFenetre(surface,0);

#ifdef T2D

#endif
  
#ifdef T3D

#endif

#ifdef O3D

#endif

#ifdef S3D
      
      dessinerScene3d(surface,scene,h);
      
      SDL_PollEvent(&event);
      switch(event.type){
	/*
      case SDL_MOUSEMOTION:
	if(is_button_down)
	  {
	    int xd,yd;
	    t_point3d *centre = definirPoint3d(0, 0, 0);
	    
	    xd = event.motion.x;
	    yd = event.motion.y;
	
	    //rotation de la caméra active
	    // definition : 100px = rotation de 90° sur un axe
	    rotationScene3d(scene, centre, 90.0*(yd-y)/100.0, 0, 0);
	    rotationScene3d(scene, centre, 0, -90.0*(xd-x)/100.0, 0);

	    x = xd;
	    y = yd;

	    free(centre);
	  }
	break;
      case SDL_MOUSEBUTTONDOWN:
        is_button_down = true;
        x = event.motion.x;
        y = event.motion.y;
	break;
      case SDL_MOUSEBUTTONUP:
        is_button_down = false;
	break;
	*/
      case SDL_KEYDOWN:
	switch(event.key.keysym.sym){
	case SDLK_m:
	  Racine(sc2);
	  scene=sc2;
	  break;
	case SDLK_l:
	  Racine(sc1);
	  scene=sc1;
	  break;
	case SDLK_KP9:
	  dy++;
	  break;
	case SDLK_KP3:
	  dy--;
	  break;
	case SDLK_KP6:
	  dx--;
	  break;
	case SDLK_KP4:
	  dx++;
	  break;
	case SDLK_KP8:
	  dz++;
	  break;
	case SDLK_KP2:
	  dz--;
	  break;
	case SDLK_a:
	  drz++;
	  break;
	case SDLK_e:
	  drz--;
	  break;
	case SDLK_q:
	  dry--;
	  break;
	case SDLK_d:
	  dry++;
	  break;
	case SDLK_z:
	  drx++;
	  break;
	case SDLK_s:
	  drx--;
	  break;
	case SDLK_SPACE:
	  dx=0;
	  dy=0;
	  dz=0;
	  drx=0;
	  dry=0;
	  drz=0;
	  break;
	case SDLK_ESCAPE:
	  fin=false;
	  break;
	}
	break;
      }    
      
      translation=definirPoint3d(dx,dy,dz);
      translationScene3d(scene->fils,translation);      
      free(translation);

    if(drx<0)
	{	
	  rotationScene3d(scene->fils,origine,-1,0,0);
	}
      if(dry<0)
	{
	  rotationScene3d(scene->fils,origine,0,-1,0);
	}
      if(drz<0)
	{
	  rotationScene3d(scene->fils,origine,0,0,-1);
	}
      if(drx>0)
	{
	  rotationScene3d(scene->fils,origine,1,0,0);
	}
      if(dry>0)
	{
	  rotationScene3d(scene->fils,origine,0,1,0);
	}
      if(drz>0)
	{
	  rotationScene3d(scene->fils,origine,0,0,1);
	}

      
      centre=GetcentreR(ss1);
      rotationScene3d(ss1,centre,0,8,0);
      free(centre);
      /*
      centre=GetcentreR(st1);
      rotationScene3d(st1,centre,5,0,0);
      free(centre);
      */
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
