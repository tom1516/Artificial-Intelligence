#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 1000
#define C 1000
#define n 0.005
#define	D 2
#define	K 2	
#define	H 15	

float	whidden[H][D],
	bhidden[H],
	shidden[H],
	zhidden[H],
	dhidden[H],
	sumhiden[H][D],
	sumhiden_bias[H],
	w1[H][D],
	b1[H];

float	sout[K],
	wout[K][H],
	bout[K],
	dout[K],
	sumout[K][H],
	sumout_bias[K],
	w2[K][H],
	b2[K];

float	Y[N][K],				
	X[N][D],				
	T[N][K],				
	TestX[C][D],
	TestT[C][K];


void init_weights();


void forward_pass(float* x, int d, float* y, int k);
void backprop(float* x,int d, float* t,int k);

int check_max(int pos);

float calc_error()
{
	int i,j;
	float er;
	
	er=0.0;
	for(i=0; i<N; i++){
		forward_pass(X[i],D,Y[i],K);
		for(j=0; j<K; j++){
			er+=(Y[i][j]-T[i][j])*(Y[i][j]-T[i][j]);
		}
	}
	er=(float)er/(2.0*N);
	return er;
}


float internal(float *x, float *y, int size);

float calc_square_error();

void generalization();
void epoxh();

void init_data(){
	int i,j;
	FILE* infile;
	infile=fopen("data.txt","r");
	for(i=0; i<N; i++){
		for(j=0; j<D; j++)
			fscanf(infile,"%f",&X[i][j]);
		for(j=0; j<K; j++)
			fscanf(infile,"%f", &T[i][j]);
	}
	fclose(infile);
	infile=fopen("test_data.txt","r");
	for(i=0; i<C; i++){
		for(j=0; j<D; j++)
			fscanf(infile,"%f",&TestX[i][j]);
		for(j=0; j<K; j++)
			fscanf(infile,"%f", &TestT[i][j]);
	}
	fclose(infile);
	
	for(i=0; i<H; i++){
		for(j=0; j<D; j++)
			whidden[i][j]=2*((float) rand()/RAND_MAX)-1;
		bhidden[i]=2*((float) rand()/RAND_MAX)-1;
	}
	for(i=0; i<K; i++){
		for(j=0; j<H; j++)
			wout[i][j]=2*((float) rand()/RAND_MAX)-1;
		bout[i]=2*((float) rand()/RAND_MAX)-1;
	}
}
void training(){
	int epoch=0;
	float error;

	do
	{
		epoxh();
		printf("Epoxh: %d Sfalma=%f\n",epoch,error);
		epoch++;
		error = calc_error();
		
	}while(error>0.05);
}

int main()
{	
	srand(time(NULL));

	init_data();
	
	training();
	
	generalization();
	return 0;
}



void generalization()
{
	int epit=0;
	int i,j,pos;
	float y[K];
	FILE *outfile=fopen("sorted","w"),*outfile1=fopen("un_sorted","w");
	epit=0;
	for(i=0; i<C; i++){
		forward_pass(TestX[i],D,y,K);
		
		if(check_max(i)==1){
			epit++;
			for(j=0; j<D; j++)
				fprintf(outfile,"%f ",TestX[i][j]);
			fprintf(outfile,"\n");
		}
		else{
			for(j=0; j<D; j++)
				fprintf(outfile1,"%f ",TestX[i][j]);
			fprintf(outfile1,"\n");
		}
		
	}
	printf("\nDynatothta genikeyshs: %f\n",(float)100.0*epit/C);
	fclose(outfile);
	
	
}


void init_weights()
{
	int i,j;
	float x,y;  

	for(i=0; i<H; i++){
		for(j=0; j<D; j++)
			whidden[i][j]=2*((float) rand()/RAND_MAX)-1;
		bhidden[i]=2*((float) rand()/RAND_MAX)-1;
	}
	for(i=0; i<K; i++){
		for(j=0; j<H; j++)
			wout[i][j]=2*((float) rand()/RAND_MAX)-1;
		bout[i]=2*((float) rand()/RAND_MAX)-1;
	}
}


float sigma(float u)
{
	return (1.0/(1.0+exp(-1*u)));
}

float sigma_par(float u)
{
	return (sigma(u)*(1.0 -sigma(u)));
}

float internal(float *x, float *y, int size)
{
	int i;
	float sum;
	sum =0.0;
	for(i=0; i<size; i++){
		sum = sum + x[i]*y[i];
	}
	return sum;
}

void forward_pass(float* x, int d, float* y, int k)
{
	float sum;
	int i, j;

	for(i=0; i<H;i++){
		shidden[i] = internal(whidden[i], x, d) + bhidden[i];
		zhidden[i]=sigma(shidden[i]);
	}
	
	for(i=0; i<k; i++){
		sout[i] = internal(wout[i],zhidden,H) + bout[i];
		y[i]=sigma(sout[i]);
	}
}

void backprop(float* x,int d, float* t,int k)
{
	int i,j;
	float sum;
	float y[k];
	
	forward_pass(x,d,y,k);		

	for(i=0;i<k;i++)
		dout[i] = (y[i] - t[i]) * sigma_par(sout[i]);
	
	for(i=0;i<H;i++){
		sum = 0.0;
		for(j=0;j<k;j++)
			sum = sum + wout[j][i ]*dout[j];
		dhidden[i] = sum * sigma_par(shidden[i]) ;
	}

	for(i=0;i<H;i++){
		for(j=0;j<d;j++)
			w1[i][j] = dhidden[i]*x[j];
		b1[i] = dhidden[i];
	}
	for(i=0;i<k;i++){
		for(j=0;j<H;j++)
			w2[i][j] = dout[i]*zhidden[j];
		b2[i] = dout[i];
	}
}

void init_sum()
{
	int j,k;
	for(j=0; j<H; j++){
		for(k=0; k<D; k++)
			sumhiden[j][k]=0.0;
		sumhiden_bias[j]=0.0;
	}
	for(j=0; j<K; j++){
		for(k=0; k<H; k++)
			sumout[j][k]=0.0;
		sumout_bias[j]=0.0;
	}
}


void calc_sum(){
	int i,j,k;
	for(i=0; i<N ; i++){
		backprop(X[i],D,T[i],K);
		for(j=0; j<H; j++){
			for(k=0; k<D; k++)
				sumhiden[j][k]+=w1[j][k];
			sumhiden_bias[j]+=b1[j];
		}
		for(j=0; j<K; j++){
			for(k=0; k<H; k++)
				sumout[j][k]+=w2[j][k];
			sumout_bias[j]+=b2[j];
		}
	}
}
void calc_weights(){
	int i,j;
	for(i=0; i<H; i++){
		for(j=0; j<D; j++)
			whidden[i][j]-= (float) (n*sumhiden[i][j]);
		bhidden[i]-= (float)(n*sumhiden_bias[i]);
	}
	for(i=0; i<K; i++){
		for(j=0; j<H; j++)
			wout[i][j]-= (float) (n*sumout[i][j]);
		bout[i]-=(float) (n*sumout_bias[i]);
	}
	
}
void epoxh()
{
	int i,j,k;
	float x[D],t[K];
	
	init_sum();
	calc_sum();
	calc_weights();
	
}

int check_max(int pos)
{
	int i,maxpos,j;
	float max;
	FILE* fp=NULL;
	
	max=-2;
	maxpos=-1;
	for(i=0; i<K; i++)
		if(Y[pos][i]>max){
			max=Y[pos][i];
			maxpos=i;
		}
	
	for(i=0; i<K; i++)
		if(TestT[pos][i]==1)
			break;
	
	
	if(maxpos==i){
		return 1;
	}
	else{
		return 0;
	}
	
}


