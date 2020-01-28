#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 400
#define M 5

#define e 0.0000000001

float points[N][3];
float centers[M][2];

/*anagnwsh tou arxeiou grammh grammh, h trith sthlh agnoeitai*/
void readFromFile(){
	int i,j;
	FILE *infile;
	infile=fopen("points_2.txt","r");
	if(infile==NULL){
		printf("Error opening file\n");
		exit(0);
	}
	
	for(i=0;i<N;i++){
		fscanf(infile,"%f %f %d",&points[i][0],&points[i][1],&j);
	}
}

float min(int x){
	float m=points[0][x];
	int i;
	
	for(i=0;i<N;i++){
		if(m>points[i][x]){
			m=points[i][x];
		}
	}
}

float max(int x){
	float m=points[0][x];
	int i;
	
	for(i=0;i<N;i++){
		if(m<points[i][x]){
			m=points[i][x];
		}
	}
}
/*epilogh M tyxaiwn shmeiwn gia arxika kentra*/
void pick_centers(){
	int i,j;
	
	for(i=0;i<M;i++){
		j=rand()%N;
		//j=i*100 +50;
		centers[i][0]=points[j][0];
		centers[i][1]=points[j][1];		
	}
}

/*ypologismos ths eykleidias apostashs twn dyo dianysmatwn*/

float diff(float x[3], float y[3]){
	return( sqrt((x[0]-y[0])*(x[0]-y[0]) + (x[1]-y[1])*(x[1]-y[1])));
}

float diff2(float x[3], float y[2]){
	return( sqrt((x[0]-y[0])*(x[0]-y[0]) + (x[1]-y[1])*(x[1]-y[1])));
}
/*epistrefei thn kathgoria tou protypou x*/
int categorise(int x){
	float d1,d2;
	int cat=0,i;
	d1=diff(points[x],centers[0]);
	/*ypologizei tis apostaseis apo ta kentra kai epistrefei to kentro me thn mikroterh apostash*/
	for(i=1;i<M;i++){
		d2=diff(points[x],centers[i]);
		if(d2<d1){
			cat=i;
			d1=d2;
		}
	}
	
	return(cat);
	
}
/*ypologizei ta nea kentra ws ton meso oro twn shmeiwn poy anhkoyn sthn kathgoria*/
void new_centers(float h){
	int i,x;
	
	for(i=0;i<N;i++){
		x=categorise(i);
		points[i][2]=x;
		centers[x][0]+=h*(points[i][0]-centers[x][0]);
		centers[x][1]+=h*(points[i][1]-centers[x][1]);
	}
}

float error(float old[M][2]){
	float err=0;
	int i;
	
	for(i=0;i<M;i++){
		err+=abs(old[i][0]-centers[i][0]) +abs(old[i][1]-centers[i][1]);
	}
	printf("error= %f\n",err);
	return(err);
}
/*o agori8mos kmeans*/
void lvq(){
	int i,j=0;
	float old[M][2];
	int count[M];
	float h=0.1;
	/*epilegh arxikwn kentrwn*/
	pick_centers();
	
	
	while(1){
		/*kratame ta palia kentra gia na doyme pote sygklienei*/
		
		for(i=0;i<M;i++){
			old[i][0]=centers[i][0];
			old[i][1]=centers[i][1];
			count[i]=0;
		}
		
		/*ypologismos newn kentrwn*/
		new_centers(h);
		
		
		
		for(i=0;i<M;i++){
			printf("old %f %f\n",centers[i][0],centers[i][1]);
		}
		/*ypologismos newn kentrwn*/
		
		for(i=0;i<M;i++){
			printf("new %f %f\n",centers[i][0],centers[i][1]);
		}
		printf("\n\n");
		/*termatismos otan yparxei sygklish*/
		if(error(old)<e){
			break;
		}
		j++;
		printf("%d\n",j);
		h=h*0.95;
	}
}
float variance(){
	int i;
	float sum;
	
	for(i=0;i<N;i++){
		sum+=diff2(points[i],centers[(int)points[i][2]]);
	}
	return(sum/N);
}

void write_to_files(){
	int i,j;
	FILE *outfile;
	char name[10];
	for(i=0;i<M;i++){
		sprintf(name,"%d",i);
		outfile=fopen(name,"w");
		if(outfile==NULL){
			printf("Error allocating memory\n");
			exit(0);
		}
		for(j=0;j<N;j++){
			if((int)points[j][2]==i)
				fprintf(outfile,"%f %f\n",points[j][0],points[j][1]);
		}
	}
	
	outfile=fopen("center","w");
	if(outfile==NULL){
		printf("Error allocating memory\n");
		exit(0);
	}
	for(i=0;i<M;i++){
		fprintf(outfile,"%f %f\n",centers[i][0],centers[i][1]);
	}
	fclose(outfile);
	
}

main(){
	int i,j;
	srand(10);
	readFromFile();
	lvq();
	for(i=0;i<M;i++){
		printf("%f %f\n",centers[i][0],centers[i][1]);
	}
	printf("Variance = %f\n",variance());
	write_to_files();
}