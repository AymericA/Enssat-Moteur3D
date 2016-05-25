#include "lib_surface.h"
#include "lib_2d.h"

t_point2d *definirPoint2d(int x, int y)	// attention malloc il faut penser au free pour les retours
{
  t_point2d *p;
  p = (t_point2d *) malloc(sizeof(t_point2d));
  p->x = x;
  p->y = y;
  return p;
}

void __copierPoint2d(t_point2d * src, t_point2d * dest) // pas d'allocation de memoire il faut un dest deja alloue, fonction en dehors du .h cependant
{
  dest->x = src->x;
  dest->y = src->y;
}

t_triangle2d *definirTriangle2d(t_point2d * a, t_point2d * b, t_point2d * c)	// attention malloc
{
  t_triangle2d *t;
  t = (t_triangle2d *) malloc(sizeof(t_triangle2d));
  __copierPoint2d(a, &(t->t[0]));
  __copierPoint2d(b, &(t->t[1]));
  __copierPoint2d(c, &(t->t[2]));
  return t;
}

void __echangerPointsTriangle(t_triangle2d * triangle, int a, int b)
{
  t_point2d p;
  __copierPoint2d(&(triangle->t[a]), &p);
  __copierPoint2d(&(triangle->t[b]), &(triangle->t[a]));
  __copierPoint2d(&p, &(triangle->t[b]));
}

void __ordonnerPointsTriangle(t_triangle2d * triangle) /* ordonne dans le sens croissant des y */
{
  if (triangle->t[0].y > triangle->t[1].y)    /* on passe le plus grand des
						 deux premier au milieu */
    {
      __echangerPointsTriangle(triangle, 0, 1);
    }
  if (triangle->t[1].y > triangle->t[2].y)    /* on passe le plus grand des
						 deux dernier au fin */
    {
      __echangerPointsTriangle(triangle, 1, 2);
    }
  if (triangle->t[0].y > triangle->t[1].y)    /* on passe le plus grand des
						 deux premier au milieu */
    {
      __echangerPointsTriangle(triangle, 0, 1);
    }
}

void __majMinMax(int x,int y,int *xmin,int *xmax)
{
  if (x<0) x=0;
  if (x>=RX) x=RX-1;
  if (y>=0 &&  y<RY){
    if ( x < xmin[y] )
      xmin[y] = x;
    if ( x > xmax[y] )
      xmax[y] = x;
  }
}

#if 0
/*
 * Algo de Bresenham
 */
void __calculerBornes(int xi,int yi,int xf,int yf,
		      int *xmin,int *xmax,int ymin)
{
  int cumul;
  int i;
  int x = xi,y = yi;
  int dx = xf - xi;
  int dy = yf - yi;
  int xinc = ( dx > 0 ) ? 1 : -1;
  int yinc = ( dy > 0 ) ? 1 : -1;
  dx = abs(dx);
  dy = abs(dy);
  if(dx>dy){
    cumul = dx/2;
    for(i=0;i<dx;i++){
      x += xinc;
      cumul += dy;
      if(cumul>dx){
	cumul -= dx;
	y += yinc;
      }
      __majMinMax(x,y,xmin,xmax);
    }
  }
  else{
    cumul = dy/2;
    for(i=0;i<dy;i++){
      y += yinc;
      cumul += dx;
      if(cumul>dy){
	cumul -= dy;
	x += xinc;
      }
      __majMinMax(x,y,xmin,xmax);
    }
  }
}
#else
void __calculerBornes(int xi,int yi,int xf,int yf,
		      int *xmin,int *xmax,int ymin)
{
  int y;
  
  if (xi!=xf && yi!=yf)
    {
      double a,b;
      a=((double)(yf-yi))/(xf-xi);
      b=yf-a*xf;

      for(y=MIN(yi,yf);y< MAX(yi,yf);y++)
	{
	  __majMinMax((y-b)/a,y,xmin,xmax);
	}
    } else if (xi==xf) { /* xi==xf && yi!=yf */
    for(y=MIN(yi,yf);y< MAX(yi,yf);y++)
      {
	__majMinMax(xi,y,xmin,xmax);
      }    
  } else if (yi>=0 && yi<RY) { /* xi!=xf && yi==yf */
    int xm = MIN(xi,xf), xM = MAX(xi,xf);
    if ( xm>0 && xm < xmin[yi] )
      xmin[yi] = xi;
    if ( xi<RX && xi > xmax[yi] )
      xmax[yi] = xi;
  }
}
#endif
void __imprimerTriangle2d(t_triangle2d * triangle)
{
  printf("%d %d - %d %d - %d %d\n", triangle->t[0].x, triangle->t[0].y, triangle->t[1].x, triangle->t[1].y, triangle->t[2].x, triangle->t[2].y); fflush(stdout);
}

/*
 * Algo "scanline"
 */
void remplirTriangle2d(t_surface * surface, t_triangle2d * triangle,double A,double B,double C,double D,double h, Uint32 c)
{
  double t;
  int ymin, ymax;
  int *xmin = surface->xmin, *xmax = surface->xmax;
  int x, y;
  __ordonnerPointsTriangle(triangle);
  /*__imprimerTriangle2d(triangle);*/
  ymin = triangle->t[0].y;
  ymax = triangle->t[2].y;
  /*  for ( y = 0 ; y < RY ; y++ ){
      xmax[y] = 0;
      xmin[y] = RX-1;
      }
  */
  
  __calculerBornes(triangle->t[0].x, triangle->t[0].y,
		   triangle->t[1].x, triangle->t[1].y,
		   xmin,xmax,ymin);
  __calculerBornes(triangle->t[1].x, triangle->t[1].y,
		   triangle->t[2].x, triangle->t[2].y,
		   xmin,xmax,ymin);
  __calculerBornes(triangle->t[0].x, triangle->t[0].y,
		   triangle->t[2].x, triangle->t[2].y,
		   xmin,xmax,ymin);

  double Ch = C*h;
  double Dh = D*h;
  double By = B*(MAX(0,ymin)-RY/2.0);
  for (y = MAX(0,ymin) ; y <= MIN(RY-1,ymax) ; y++ ){
    double Ax = A*(xmin[y]-RX/2.0);
    for (x = xmin[y] ; x <= xmax[y] ; x++ ){
      //double z=t*h;
      //double tmp=A*(x-RX/2)+B*(y-RY/2)+C*h;
      double tmp = Ax+By+Ch;
      if(tmp){ //vérif aplanéisme
	t=-Dh/tmp;
	if(t>screen[y][x].z && t<0){ //deuxième vérif pixel pas derrière caméra
	  screen[y][x].couleur=c;
	  screen[y][x].z=t;
	}
      }
      Ax += A/2;
    }
    By += B/2;
    xmin[y] = RX-1;
    xmax[y] = 0;
  }

}
