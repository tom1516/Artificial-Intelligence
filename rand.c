#include <stdio.h>
#include <stdlib.h>

void create_points(float xmin,float xmax,float ymin,float ymax,FILE *outfile,int num,int category){
	int i,j;
	float x,y;
	if(category>0){
		for(i=0;i<num;i++){
			x=((float)rand()/RAND_MAX)*(xmax-xmin)+xmin;
			y=((float)rand()/RAND_MAX)*(ymax-ymin)+ymin;
			fprintf(outfile,"%f %f %d\n",x,y,category);
		}
	}
	else{
		for(i=0;i<num;i++){
			x=((float)rand()/RAND_MAX)*(xmax-xmin)+xmin;
			y=((float)rand()/RAND_MAX)*(ymax-ymin)+ymin;
			if(x*x+y*y<0.25)
				fprintf(outfile,"%f %f 1 0\n",x,y,category);
			else
				fprintf(outfile,"%f %f 0 1\n",x,y,category);
		}
	}
}

main(){
	
	int i,j;
	FILE *outfile;
	srand(10);
	
	outfile=fopen("data.txt","w");
	create_points(-1,1,-1,1,outfile,1000,0);
	fclose(outfile);

	outfile=fopen("test_data.txt","w");
	create_points(-1,1,-1,1,outfile,1000,0);
	fclose(outfile);
	
	outfile=fopen("points_2.txt","w");
	create_points(-1.25,-0.75,-1.25,-0.75,outfile,100,1);
	create_points(-1.25,-0.75,0.75,1.25,outfile,100,2);
	create_points(0.75,1.25,-1.25,-0.75,outfile,100,3);
	create_points(0.75,1.25,0.75,1.25,outfile,100,4);
	fclose(outfile);
}