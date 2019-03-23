//N : Edge(Data) 수
//*x , *y  : Edge의 (x, y) Data
//removeN : EllipseFitting후 Ellipse에서 가장 멀리있는 점을 없애고 다시 fitting울 반복한다.
//          이 반복횟수를 나타낸다.
//ErrorLimit: EllipseFitting후 Ellipse에서 가장 멀리있는 점이 이값 이하이면 반복을 중지한다.
// *Cx, *Cy : Ellipse Center
// *al      : 장축 
// *bl      : 단축 
// *angle   : 장축의 각도 (-90 ~ 90)

int EllipseData(int N, double *x, double *y, int RemoveN, double ErrorLimit, 
				double *Cx, double *Cy, double *al, double *bl, double *angle);

//SubRoutin ------------------------------------------------------------------------------------------------
int matrixInverse(double *a,int n,double *b);
int	elliFitShape(double *para,double *centX,double *centY, double *a,double *b,double *theta);
int FindLineFunction(double x1, double y1, double x2, double y2, double *a, double *b);

//Recipes---------------------------------------------------------------------------------------------------
double*	vector(int nl,int nh);
double**	matrix(int nrl,int nrh,int ncl,int nch);
void	free_vector(double *v,int nl,int nh);
void	free_matrix(double **m,int nrl,int nrh,int ncl,int nch);

int	jacobi(double **a,int n,double *d,double **v,int *nrot);

int	qromb(double (*func)(double),double a,double b,double *intgrl);

void	svdcmp(double **a,int m,int n,double *w,double **v);

void	svbksb(double **u,double *w,double **v,int m,int n,double *b,double *x);
//--------------------------------------------------------------------------------