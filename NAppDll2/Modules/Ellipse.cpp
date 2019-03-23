#include "stdafx.h"
#include "ellipse.h"
#include "math.h"

// N : Edge(Data) 수
// *x , *y   : Edge의 (x, y) Data
// removeN   : EllipseFitting후 Ellipse에서 가장 멀리있는 점을 없애고 다시 fitting울 반복한다.
//             이 반복횟수를 나타낸다.
// ErrorLimit: EllipseFitting후 Ellipse에서 가장 멀리있는 점이 이값 이하이면 반복을 중지한다.
// *Cx, *Cy  : Ellipse Center
// *al       : 장축 
// *bl       : 단축 
// *angle    : 장축의 각도 (-90 ~ 90)
int EllipseData(int N, double *x, double *y, int RemoveN, double ErrorLimit, 
				double *Cx, double *Cy, double *al, double *bl, double *angle)
{
    int i, j, loop, nResult=-1;

	double   Matrix_A[5*5],Matrix_AA[5*5], Matrix_B[5*5], Vector[5], XVector[5];
	double   P[7];
    double   ellipse_cx, ellipse_cy, ellipse_a, ellipse_b, ellipse_theta;
	double   x1, x2, x3, x4;
	double   y1, y2, y3, y4;
	double   xy, xy2, xy3, x2y, x2y2, x3y;

	double  Temp, TempA, TempB, TempC, crossX[2], crossY[2], a,b;
	double  Dist1, Dist2, MaxDist;
	int     MaxDistPos;
	
	for(i=0;i<5;i++)
		Vector[i]=0;

	for(i=0;i<25;i++) Matrix_A[i]=0;
	for(i=0;i<N;i++)
	{
		x1=x[i];
		x2=x1*x1;
		x3=x2*x1;
		x4=x3*x1;
	
		y1=y[i];
		y2=y1*y1;
		y3=y2*y1;
		y4=y3*y1;

		xy=x1*y1;
		xy2=xy*y1;
		xy3=xy2*y1;

		x2y=xy*x1;
		x3y=x2y*x1;
		x2y2=xy2*x1;

		Matrix_A[0]+=x4;
		Matrix_A[1]+=x2y2;
		Matrix_A[2]+=x3y;
		Matrix_A[3]+=x3;
		Matrix_A[4]+=x2y;
		
		Matrix_A[6]+=y4;
		Matrix_A[7]+=xy3;
		Matrix_A[8]+=xy2;
		Matrix_A[9]+=y3;

		Matrix_A[12]+=x2y2;
		Matrix_A[13]+=x2y;
		Matrix_A[14]+=xy2;

		Matrix_A[18]+=x2;
		Matrix_A[19]+=xy;
		
		Matrix_A[24]+=y2;

		Vector[0]+=(-x2);
		Vector[1]+=(-y2);
		Vector[2]+=(-xy);
		Vector[3]+=(-x1);
		Vector[4]+=(-y1);
	}

	Matrix_A[5]=Matrix_A[1];
	Matrix_A[10]=Matrix_A[2];
	Matrix_A[15]=Matrix_A[3];
	Matrix_A[20]=Matrix_A[4];
	Matrix_A[11]=Matrix_A[7];
	Matrix_A[16]=Matrix_A[8];
	Matrix_A[21]=Matrix_A[9];
	Matrix_A[17]=Matrix_A[13];
	Matrix_A[22]=Matrix_A[14];
	Matrix_A[23]=Matrix_A[19];

	for(i=0;i<25;i++)
		Matrix_AA[i]=Matrix_A[i];

	if(matrixInverse(Matrix_AA, 5, Matrix_B)<0) goto done;

	for(i=0;i<5;i++) 
		XVector[i]=0;

	for(i=0;i<5;i++)
		for(j=0;j<5;j++)
			XVector[i]+=Matrix_B[i*5+j]*Vector[j];

	P[1]=XVector[0];
	P[2]=XVector[2];
	P[3]=XVector[1];
	P[4]=XVector[3];
	P[5]=XVector[4];
	P[6]=1;

	if (elliFitShape(P, &ellipse_cx, &ellipse_cy, &ellipse_a, &ellipse_b, &ellipse_theta) < 0) goto done;

	for(loop=0;loop<RemoveN;loop++)
	{
		MaxDist=0;
		MaxDistPos=0;
		for(i=0;i<N;i++)
		{
			FindLineFunction((double)ellipse_cx, (double)ellipse_cy,x[i], y[i], &a, &b);
						
			TempA=P[1]+a*P[2]+P[3]*a*a;
			TempB=P[2]*b+2*P[3]*a*b+P[4]+P[5]*a;
			TempC=P[3]*b*b+P[5]*b+P[6];

			Temp=TempB*TempB-4*TempA*TempC;
			if(Temp>0)
			{
				crossX[0]=(-TempB+sqrt(Temp))/(2*TempA);
				crossY[0]=a*crossX[0]+b;
				Dist1=sqrt((x[i]-crossX[0])*(x[i]-crossX[0])+(y[i]-crossY[0])*(y[i]-crossY[0]));
				crossX[1]=(-TempB-sqrt(Temp))/(2*TempA);
				crossY[1]=a*crossX[1]+b;
				Dist2=sqrt((x[i]-crossX[1])*(x[i]-crossX[1])+(y[i]-crossY[1])*(y[i]-crossY[1]));
				if(Dist2<Dist1) Dist1=Dist2;
				if(Dist1>MaxDist)
				{
					MaxDist=Dist1;
					MaxDistPos=i;
				}
			}
		}

		if (MaxDist<ErrorLimit) break;

		x1=x[MaxDistPos];
		x2=x1*x1;
		x3=x2*x1;
		x4=x3*x1;
	
		y1=y[MaxDistPos];
		y2=y1*y1;
		y3=y2*y1;
		y4=y3*y1;

		xy=x1*y1;
		xy2=xy*y1;
		xy3=xy2*y1;

		x2y=xy*x1;
		x3y=x2y*x1;
		x2y2=xy2*x1;

		Matrix_A[0]-=x4;
		Matrix_A[1]-=x2y2;
		Matrix_A[2]-=x3y;
		Matrix_A[3]-=x3;
		Matrix_A[4]-=x2y;
		
		Matrix_A[6]-=y4;
		Matrix_A[7]-=xy3;
		Matrix_A[8]-=xy2;
		Matrix_A[9]-=y3;

		Matrix_A[12]-=x2y2;
		Matrix_A[13]-=x2y;
		Matrix_A[14]-=xy2;

		Matrix_A[18]-=x2;
		Matrix_A[19]-=xy;
		
		Matrix_A[24]-=y2;

		Vector[0]-=(-x2);
		Vector[1]-=(-y2);
		Vector[2]-=(-xy);
		Vector[3]-=(-x1);
		Vector[4]-=(-y1);
		
		Matrix_A[5]=Matrix_A[1];
		Matrix_A[10]=Matrix_A[2];
		Matrix_A[15]=Matrix_A[3];
		Matrix_A[20]=Matrix_A[4];
		Matrix_A[11]=Matrix_A[7];
		Matrix_A[16]=Matrix_A[8];
		Matrix_A[21]=Matrix_A[9];
		Matrix_A[17]=Matrix_A[13];
		Matrix_A[22]=Matrix_A[14];
		Matrix_A[23]=Matrix_A[19];

		for(i=0;i<25;i++)
			Matrix_AA[i]=Matrix_A[i];
		if (matrixInverse(Matrix_AA,5,Matrix_B)<0) goto done;

		for(i=0;i<5;i++) 
			XVector[i]=0;

		for(i=0;i<5;i++)
			for(j=0;j<5;j++)
				XVector[i]+=Matrix_B[i*5+j]*Vector[j];

		P[1]=XVector[0];
		P[2]=XVector[2];
		P[3]=XVector[1];
		P[4]=XVector[3];
		P[5]=XVector[4];
		P[6]=1;

		if(elliFitShape(P, &ellipse_cx, &ellipse_cy, &ellipse_a, &ellipse_b, &ellipse_theta) <0) goto done;	

		for(i=MaxDistPos;i<N-loop-1;i++)
		{
			x[i]=x[i+1];
			y[i]=y[i+1];
		}
		N--;
	}

	*Cx=(double)ellipse_cx;
	*Cy=(double)ellipse_cy;
	if (ellipse_a>ellipse_b)
	{
		*al=ellipse_a;
		*bl=ellipse_b;
		*angle=ellipse_theta*180.0/3.1415926535;
	}
	else
	{
		*al=ellipse_b;
		*bl=ellipse_a;
		*angle=ellipse_theta*180.0/3.1415926535+90;
		if(fabs(*angle)>90.0) *angle-=180.0;
	}		

	nResult=0;

done: 
	return nResult;
}

int matrixInverse(double *a,int n,double *b)
{
	int i,j,k,l,l1;
	double c,d;
	double SMALLEST=0.00000001;
	for (i=0;i<n;i++) {
		 for (j=0; j<n;j++)
			 *(b+i*n+j) = 0.;
		 *(b+i*n+i) = 1.0;
	 }
	 for(l=0;l<n;l++){
		 d = fabs((*(a+l*n+l)));
		 if( d<SMALLEST ){
			 l1 = l;
			 d = fabs((*(a+l1*n+l)));
			 while (d < SMALLEST && (++l1<n))
				 d = fabs((*(a+l1*n+l)));
			 if (l1>=n)
				 return -1;
			 for (j=0;j<n;j++){
				 *(a+l*n+j) += (*(a+l1*n+j));
				 *(b+l*n+j) += (*(b+l1*n+j));
			 }
		 }
		 c = 1.0/(*(a+l*n+l));
		 for (j=l;j<n;j++) *(a+l*n+j) *= c;
		 for (j=0;j<n;j++) *(b+l*n+j) *= c;
		 k = l+1;
		 for (i=k;i<n;i++){
			 c = *(a+i*n+l);
			 for (j=l;j<n;j++) *(a+i*n+j) -= (*(a+l*n+j) * c);
			 for (j=0;j<n;j++) *(b+i*n+j) -= (*(b+l*n+j) * c);
		 }
	 }
	 for (l=n-1; l>=0;l--){
		 for (k=1;k<=l;k++){
			 i = l-k;
			 c = *(a+i*n+l);
			 for (j=l;j<n;j++) *(a+i*n+j) -= (*(a+l*n+j) * c);
			 for (j=0;j<n;j++) *(b+i*n+j) -= (*(b+l*n+j) * c);
		 }
	 }
	 return 0;
}


//-1 : Failure
//0 : OK
int	elliFitShape(double *para,double *centX,double *centY, double *a,double *b,double *theta) 
{
int	ok=-1;
double	A,B,C,D,E,F;
double	b2m4ac;
double	cx,cy;
double	f0;
double	**MA=0,**v=0;
double	*d=0;
int	nrot;
	
    // get param
	A = para[1];
	B = para[2];
	C = para[3];
	D = para[4];
	E = para[5];
	F = para[6];
	// check ellipse
	if ((b2m4ac=B*B-4.0*A*C)>=0.0)
		goto done;
	// get center
	*centX = cx = (2.0*C*D-B*E)/b2m4ac;
	*centY = cy = (2.0*A*E-B*D)/b2m4ac;
	// get f0
	f0 = A*cx*cx+B*cx*cy+C*cy*cy+D*cx+E*cy+F;
	// define MA
	MA = matrix(1,2,1,2);
	MA[1][1] = A;
	MA[1][2] = MA[2][1] = 0.5*B;
	MA[2][2] = C;

	// get eigen values and vectors
	d = vector(1,2);
	v = matrix(1,2,1,2);
	if (!jacobi(MA,2,d,v,&nrot))
		goto done;
	free_matrix(MA,1,2,1,2);	MA = 0;

	// get a,b
	*a = sqrt(-f0/d[1]);
	*b = sqrt(-f0/d[2]);

	// get theta
	*theta = atan2(v[2][1],v[1][1]);

	// done
	ok = 0;

done:
	if (MA)	free_matrix(MA,1,2,1,2);
	if (d)	free_vector(d,1,2);
	if (v)	free_matrix(v,1,2,1,2);
	return ok;
}


int FindLineFunction(double x1, double y1, double x2, double y2, double *a, double *b)
{
	if(x1==x2)
	{
		*a=0;
		*b=y1;
	}
	else
	{
		*a=(y1-y2)/(x1-x2);
		*b=y1-*a*x1;
	}

	return 0;
}

//Recipse----------------------------------------------------
// proto
#define	ROTATE(a,i,j,k,l) g=a[i][j];h=a[k][l];a[i][j]=g-s*(h+g*tau);\
a[k][l]=h+s*(g-h*tau);

// functions
double*	vector(int nl,int nh) {
double *v;
	v=(double *)malloc((unsigned) (nh-nl+1)*sizeof(double));
	return v-nl;
}

double**	matrix(int nrl,int nrh,int ncl,int nch) {
int i;
double **m;
	m=(double **) malloc((unsigned) (nrh-nrl+1)*sizeof(double*));
	m -= nrl;
	for(i=nrl;i<=nrh;i++) {
		m[i]=(double *) malloc((unsigned) (nch-ncl+1)*sizeof(double));
		m[i] -= ncl;
	}
	return m;
}

void	free_vector(double *v,int nl,int nh) {
	free((char*) (v+nl));
}

void	free_matrix(double **m,int nrl,int nrh,int ncl,int nch) {
int i;
	for(i=nrh;i>=nrl;i--) free((char*) (m[i]+ncl));
	free((char*) (m+nrl));
}

int	jacobi(double **a,int n,double *d,double **v,int *nrot) {
int	j,iq,ip,i;
double	tresh,theta,tau,t,sm,s,h,g,c,*b,*z;
	b = vector(1,n);
	z = vector(1,n);
	for (ip=1; ip<=n; ip++) {
		for (iq=1; iq<=n; iq++)
			v[ip][iq] = 0.0;
		v[ip][ip] = 1.0;
	}
	for (ip=1; ip<=n; ip++) {
		b[ip] = d[ip] = a[ip][ip];
		z[ip] = 0.0;
	}
	*nrot = 0;
	for (i=1; i<=50; i++) {
		sm = 0.0;
		for (ip=1; ip<=n-1; ip++) {
			for (iq=ip+1; iq<=n; iq++)
				sm += fabs(a[ip][iq]);
		}
		if (sm==0.0) {
			free_vector(z,1,n);
			free_vector(b,1,n);
			return 1;
		}
		if (i<4)
			tresh = 0.2*sm/(n*n);
		else
			tresh = 0.0;
		for (ip=1; ip<=n-1; ip++) {
			for (iq=ip+1; iq<=n; iq++) {
				g = 100.0*fabs(a[ip][iq]);
				if (i>4 && fabs(d[ip])+g==fabs(d[ip])
				 && fabs(d[iq])+g==fabs(d[iq]))
					a[ip][iq] = 0.0;
				else if (fabs(a[ip][iq])>tresh) {
					h = d[iq]-d[ip];
					if (fabs(h)+g==fabs(h))
						t = a[ip][iq]/h;
					else {
						theta = 0.5*h/a[ip][iq];
						t = 1.0/(fabs(theta)+sqrt(1.0+theta*theta));
						if (theta<0.0)
							t = -t;
					}
					c = 1.0/sqrt(1+t*t);
					s = t*c;
					tau = s/(1.0+c);
					h = t*a[ip][iq];
					z[ip] -= h;
					z[iq] += h;
					d[ip] -= h;
					d[iq] += h;
					a[ip][iq] = 0.0;
					for (j=1; j<=ip-1; j++) {
						ROTATE(a,j,ip,j,iq);
					}
					for (j=ip+1; j<=iq-1; j++) {
						ROTATE(a,ip,j,j,iq);
					}
					for (j=iq+1; j<=n; j++) {
						ROTATE(a,ip,j,iq,j);
					}
					for (j=1; j<=n; j++) {
						ROTATE(v,j,ip,j,iq);
					}
					++(*nrot);
				}
			}
		}
		for (ip=1; ip<=n; ip++) {
			b[ip] += z[ip];
			d[ip] = b[ip];
			z[ip] = 0.0;
		}
	}
	free_vector(z,1,n);
	free_vector(b,1,n);

	return 0;
}
//-------------------------------------------------------------------------