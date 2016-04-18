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

  
  t_point3d *origine = definirPoint3d(0,0,0), *vecteur,*vecteur2,*centre1,*centre2;
  double h = -500;
  
  t_objet3d*cam1=camera();
  t_objet3d *plan=damier(200,200,3,3);
  t_objet3d *cube=parallelepipede(200,200,200);

  t_objet3d *cube2=tore(100,10,10,50);
  
  double m1[4][4]={{1,0,0,0},			\
		   {0,1,0,0},			\
		   {0,0,1,0},			\
		   {0,0,0,1}};

  double m2[4][4]={{1,0,0,0},			\
		   {0,1,0,0},			\
		   {0,0,1,0},			\
		   {0,0,0,1}};
  
  
  t_scene3d * scene=creerScene3d(cam1,creerMatrice(m1),creerMatrice(m2),0);
    
  vecteur = definirPoint3d(0,0,-500);

  t_scene3d * scube=creerScene3d(cube,matTranslation(vecteur),matTranslationinv(vecteur),1);

  vecteur = definirPoint3d(0,150,0);

  t_scene3d * scube2=creerScene3d(cube2,matTranslation(vecteur),matTranslationinv(vecteur),2);


  ajoutFilsNoeud(scene,scube,0);

  ajoutFilsNoeud(scene,scube2,1);

  centre1=definirPoint3d(0,0,-500);
  centre2=definirPoint3d(0,150,-500);

  vecteur = definirPoint3d(0,0,0);
  vecteur2=definirVecteur3d(5,0,0);
 

#endif


  int i=0;

  surface=creerFenetre(RX,RY);
  timestart = SDL_GetTicks();


  while(i<25*10000 && fin)
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
      vecteur = definirPoint3d(0,0,7*sin(i*M_PI/180));   

      dessinerObjet3d(surface,plan,h);
      translationObjet3d(plan,vecteur);
 
      free(vecteur);
#endif

#ifdef S3D     
      
      free(centre1);
      centre1=getCentre(scene,1);
      

      free(centre2);
      centre2=getCentre(scene,2);
      
      //affMatrice(scene->mat->mat);
      affMatrice(scube->mat->mat);
      affMatrice(scube2->mat->mat);
      
      printf("centre1 : ");
      affPoint(centre1);
      
      
      printf("centre2 : ");
      affPoint(centre2);
           
     
      rotationScene3dv2(scene,centre1,0,5,0,1);


      rotationScene3dv2(scene,centre1,0,-5,0,2);

      //rotationScene3dv2(scene,centre2,1,0,0,2);
     
      //translationScene3dv2(scene,vecteur2,2);
      
     
      
      dessinerScene3d(surface,scene,h);
      
      
      SDL_PollEvent(&event);
      switch(event.type){
	case SDL_KEYDOWN:
	  switch(event.key.keysym.sym){
	  case SDLK_UP:
	    free(vecteur);
	    vecteur=definirVecteur3d(0,-5,0);
	    break;
	  case SDLK_DOWN:
	    free(vecteur);
	    vecteur=definirVecteur3d(0,5,0);
	    break;
	  case SDLK_RIGHT:
	    free(vecteur);
	    vecteur=definirVecteur3d(5,0,0);
	    break;
	  case SDLK_LEFT:
	    free(vecteur);
	    vecteur=definirVecteur3d(-5,0,0);
	    break;
	  case SDLK_a:
	    free(vecteur);
	    vecteur=definirVecteur3d(0,0,5);
	    break;
	  case SDLK_e:
	    free(vecteur);
	    vecteur=definirVecteur3d(0,0,-5);
	    break;
	  case SDLK_SPACE:
	    free(vecteur);
	    vecteur=definirVecteur3d(0,0,0);
	    break;
	  case SDLK_ESCAPE:
	    fin=false;
	    break;
	  }
	  break;
      }

      translationScene3dv2(scene,vecteur,1);     
      //exit(EXIT_SUCCESS);
   
#endif

      afficherFenetre(surface,screen);
      majEcran(surface);
      SDL_Delay(20);
      i += 1;

      cpt++;
      sprintf(buf,"%d FPS",(int)(cpt*1000.0/(SDL_GetTicks()-timestart)));
      SDL_WM_SetCaption(buf,NULL);
    }
  SDL_Quit();
  return 0;
}
