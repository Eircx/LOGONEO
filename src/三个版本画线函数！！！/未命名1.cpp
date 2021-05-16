void drawline_first(double x,double y,int size,double xx,double yy,double v){
	double dx=xx-x,dy=yy-y;
	size = size / dmmax(cos(v),sin(v));
	if(floor(x)==floor(xx)){
		if(y > yy){
			dswap(y,yy);
		}
		for(int i=floor(y);i<=floor(yy);i++){
			for(int j=floor(x-size);j<=floor(x+size);j++)
			    dra[i][j]=pen.col;
		}
	}
	else{
		if(x > xx){
			dswap(y,yy);
			dswap(x,xx);
		}
		if(y < yy){
			double low=(ceil(x)-x)*dy/dx+y-size,delta=dy/dx,high=delta+low+2*size;
			int k=floor(x);
			for(int i=floor(y-size);i<=floor(low+2*size);i++){
				dra[i][k]=pen.col;
			}
				
			for(int i=ceil(x);i<floor(xx);i++){ 
				for(int j=floor(low);j<=floor(high);j++){
				    dra[j][i]= pen.col;	
				}
				low+=delta;
				high+=delta;
			}
			k=floor(xx);
			for(int i=floor(low);i<=floor(yy+size);i++)
			    dra[i][k]=pen.col;
		}
		else{
			double low=(ceil(x)-x)*dy/dx+y+size,delta=dy/dx,high=delta+low-2*size;
			int k=floor(x);
			for(int i=floor(y+size);i>=floor(low-2*size);i--)
				dra[i][k]=pen.col;
			for(int i=ceil(x);i<floor(xx);i++){ 
				for(int j=floor(low);j>=floor(high);j--)
				    dra[j][i]= pen.col;	
				low+=delta;
				high+=delta;
			}
			k=floor(xx);
			for(int i=floor(yy-size);i<=floor(low);i++)
			    dra[i][k]=pen.col;
		}
	}
	if(floor(y)==floor(yy)){
		if(x > xx){
			dswap(x,xx);
		}
		for(int i=floor(x);i<=floor(xx);i++){
			for(int j=floor(y-size);j<=floor(y+size);j++)
			    dra[j][i]=pen.col;
		}
	}
}
void drawline_first(double x,double y,int size,double xx,double yy,double v){
	double dx=xx-x,dy=yy-y;
	size = size / dmmax(cos(v),sin(v));
	if(floor(x)==floor(xx)){
		if(y > yy){
			dswap(y,yy);
		}
		for(int i=floor(y);i<=floor(yy);i++){
			for(int j=floor(x-size);j<=floor(x+size);j++)
			    dra[i][j]=pen.col;
		}
	}
	else{
		if(x > xx){
			dswap(y,yy);
			dswap(x,xx);
		}
		if(y < yy){
			double low=(ceil(x)-x)*dy/dx+y-size,delta=dy/dx,high=delta+low+2*size;
			int k=floor(x);
			for(int i=floor(y-size);i<=floor(low+2*size);i++){
				dra[i][k]=pen.col;
			}
				
			for(int i=ceil(x);i<floor(xx);i++){ 
				for(int j=floor(low);j<=floor(high);j++){
				    dra[j][i]= pen.col;	
				}
				low+=delta;
				high+=delta;
			}
			k=floor(xx);
			for(int i=floor(low);i<=floor(yy+size);i++)
			    dra[i][k]=pen.col;
		}
		else{
			double low=(ceil(x)-x)*dy/dx+y+size,delta=dy/dx,high=delta+low-2*size;
			int k=floor(x);
			for(int i=floor(y+size);i>=floor(low-2*size);i--)
				dra[i][k]=pen.col;
			for(int i=ceil(x);i<floor(xx);i++){ 
				for(int j=floor(low);j>=floor(high);j--)
				    dra[j][i]= pen.col;	
				low+=delta;
				high+=delta;
			}
			k=floor(xx);
			for(int i=floor(yy-size);i<=floor(low);i++)
			    dra[i][k]=pen.col;
		}
	}
	if(floor(y)==floor(yy)){
		if(x > xx){
			dswap(x,xx);
		}
		for(int i=floor(x);i<=floor(xx);i++){
			for(int j=floor(y-size);j<=floor(y+size);j++)
			    dra[j][i]=pen.col;
		}
	}
		else{
		if(y > yy){
			dswap(y,yy);
			dswap(x,xx);
		}
		if(x < xx){
			double low=(ceil(y)-y)*dx/dy+x-size,delta=dx/dy,high=delta+low+2*size;
			int k=floor(y);
			for(int i=floor(x-size);i<=floor(low+2*size);i++){
				dra[k][i]=pen.col;
			}
				
			for(int i=ceil(y);i<floor(yy);i++){ 
				for(int j=floor(low);j<=floor(high);j++){
				    dra[i][j]= pen.col;	
				}
				low+=delta;
				high+=delta;
			}
			k=floor(yy);
			for(int i=floor(low);i<=floor(xx+size);i++)
			    dra[k][i]=pen.col;
		}
		else{
			double low=(ceil(y)-y)*dx/dy+x+size,delta=dx/dy,high=delta+low-2*size;
			int k=floor(y);
			for(int i=floor(x+size);i>=floor(low-2*size);i--)
				dra[k][i]=pen.col;
			for(int i=ceil(y);i<floor(yy);i++){
				for(int j=floor(low);j>=floor(high);j--)
				    dra[i][j]= pen.col;	
				low+=delta;
				high+=delta;
			}
			k=floor(yy);
			for(int i=floor(xx-size);i<=floor(low);i++)
			    dra[k][i]=pen.col;
		}
	}
}
void drawline_second(double x,double y,int size,double len,double xx,double yy){
	double dx=xx-x,dy=yy-y,sine=fabs(dy/len),cosine=fabs(dx/len),dt=fabs(dx/dy),delta=fabs(dy/dx);
	if(floor(x)==floor(xx)){
		if(y > yy){
			dswap(y,yy);
		}
		for(int i=floor(y);i<=floor(yy);i++){
			for(int j=floor(x-size);j<=floor(x+size);j++)
			    dra[i][j]=pen.col;
		}
	}
	else{
		if(floor(y)==floor(yy)){
		    if(x > xx){
			    dswap(x,xx);
		    }
		    for(int i=floor(x);i<=floor(xx);i++){
			    for(int j=floor(y-size);j<=floor(y+size);j++)
			        dra[j][i]=pen.col;
		    }
	    }
	    else{
	    	if(sine < cosine){
	    	    if(x > xx){
			       dswap(y,yy);
			       dswap(x,xx);
                }
		        if(y < yy){
		        	struct borders{
		        		double x,y;
		        	};
		        	borders border[2][2];
		        	border[0][0].x=x+sine*size;
		        	border[0][0].y=y-cosine*size;
		        	border[0][1].x=x-sine*size;
		        	border[0][1].y=y+cosine*size;
		        	border[1][0].x=xx+sine*size;
		        	border[1][0].y=yy-cosine*size;
		        	border[1][1].x=xx-sine*size;
		        	border[1][1].y=yy+cosine*size;

			        double low=border[0][1].y-(ceil(border[0][1].x)-border[0][1].x)*dt,high=border[0][1].y+(ceil(border[0][1].x)-border[0][1].x)*delta;
			        int k=floor(border[0][0].x);
			        for(int i=floor(border[0][1].x);i<floor(border[0][0].x);i++){
						for(int j=floor(low);j<=floor(high);j++){
							dra[j][i]= pen.col;
						}
						low-=dt;
						high+=delta;
					}
					low=border[0][0].y+(ceil(border[0][0].x)-border[0][0].x)*delta;
					high=low+2*size/cosine;
			        for(int i=floor(border[0][0].y);i<=floor(high);i++)
				        dra[i][k]=pen.col;//needs optimization(take borders into consideration)
					high+=delta;	
			        for(int i=ceil(border[0][0].x);i<floor(border[1][1].x);i++){ 
				        for(int j=floor(low);j<=floor(high);j++){
				            dra[j][i]= pen.col;	
				        }
				        low+=delta;
				        high+=delta;
			        }
			        k=floor(border[1][1].x);
			        for(int i=floor(low);i<=floor(border[1][1].y);i++)
			            dra[i][k]=pen.col;//needs optimization(take borders into consideration)
			        low+=delta;
			        high=border[1][1].y;
			        for(int i=ceil(border[1][1].x);i<=floor(border[1][0].x);i++){
						for(int j=floor(low);j<=floor(high);j++){
							dra[j][i]= pen.col;
						}
						low+=delta;
						high-=dt;
					}
		        }
		        else{
			        struct borders{
		        		double x,y;
		        	};
		        	borders border[2][2];
		        	border[0][0].x=x+sine*size;
		        	border[0][0].y=y+cosine*size;
		        	border[0][1].x=x-sine*size;
		        	border[0][1].y=y-cosine*size;
		        	border[1][0].x=xx+sine*size;
		        	border[1][0].y=yy+cosine*size;
		        	border[1][1].x=xx-sine*size;
		        	border[1][1].y=yy-cosine*size;
	
			        double low=border[0][1].y-(ceil(border[0][1].x)-border[0][1].x)*delta,high=border[0][1].y+(ceil(border[0][1].x)-border[0][1].x)*dt;
			        int k=floor(border[0][0].x);
			        for(int i=floor(border[0][1].x);i<floor(border[0][0].x);i++){
						for(int j=floor(low);j<=floor(high);j++){
							dra[j][i]= pen.col;
						}
						low-=delta;
						high+=dt;
					}
					high=border[0][0].y - ( ceil(border[0][0].x) - border[0][0].x )*delta;
					low=high-2*size/cosine;
			        for(int i=floor(low);i<=floor(border[0][0].y);i++)
				        dra[i][k]=pen.col;//needs optimization(take borders into consideration)
					low-=delta;
			        for(int i=ceil(border[0][0].x);i<floor(border[1][1].x);i++){ 
				        for(int j=floor(low);j<=floor(high);j++)
				            dra[j][i]= pen.col;	
				        low-=delta;
				        high-=delta;
			        }
			        k=floor(border[1][1].x);
			        for(int i=floor(border[1][1].y);i<=floor(high);i++)
			            dra[i][k]=pen.col;//needs optimization(take borders into consideration)
			        low=border[1][1].y;
			        for(int i=ceil(border[1][1].x);i<=floor(border[1][0].x);i++){
						for(int j=floor(low);j<=floor(high);j++){
							dra[j][i]= pen.col;
						}
						low+=dt;
						high-=delta;
					}
				}
	        }
	        else{
		        if(y > yy){
			        dswap(y,yy);
			        dswap(x,xx);
		        }
		        if(x < xx){
			        struct borders{
		        		double x,y;
		        	};
		        	borders border[2][2];
		        	border[0][0].x=x+sine*size;
		        	border[0][0].y=y-cosine*size;
		        	border[0][1].x=x-sine*size;
		        	border[0][1].y=y+cosine*size;
		        	border[1][0].x=xx+sine*size;
		        	border[1][0].y=yy-cosine*size;
		        	border[1][1].x=xx-sine*size;
		        	border[1][1].y=yy+cosine*size;

			        double low=border[0][0].x-(ceil(border[0][0].y)-border[0][0].y)*delta,high=border[0][0].x+(ceil(border[0][0].y)-border[0][0].y)*dt;
			        int k=floor(border[0][1].y);
			        for(int i=floor(border[0][0].y);i<floor(border[0][1].y);i++){
						for(int j=floor(low);j<=floor(high);j++){
							dra[i][j]= pen.col;
						}
						low-=delta;
						high+=dt;
					}
					low=border[0][1].x+(ceil(border[0][1].y)-border[0][1].y)*dt;
					high=low+2*size/sine;
			        for(int i=floor(border[0][1].x);i<=floor(high);i++)
				        dra[k][i]=pen.col;//needs optimization(take borders into consideration)
					high+=dt;
			        for(int i=ceil(border[0][1].y);i<floor(border[1][0].y);i++){ 
				        for(int j=floor(low);j<=floor(high);j++){
				            dra[i][j]= pen.col;	
				        }
				        low+=dt;
				        high+=dt;
			        }
			        k=floor(border[1][0].y);
			        for(int i=floor(low);i<=floor(border[1][0].x);i++)
			            dra[k][i]=pen.col;//needs optimization(take borders into consideration)
			        low+=dt;
			        high=border[1][0].x;
			        for(int i=ceil(border[1][0].y);i<=floor(border[1][1].y);i++){
						for(int j=floor(low);j<=floor(high);j++){
							dra[i][j]= pen.col;
						}
						low+=dt;
						high-=delta;
					}
		        }
		        else{
			        struct borders{
		        		double x,y;
		        	};
		        	borders border[2][2];
		        	border[0][0].x=x+sine*size;
		        	border[0][0].y=y+cosine*size;
		        	border[0][1].x=x-sine*size;
		        	border[0][1].y=y-cosine*size;
		        	border[1][0].x=xx+sine*size;
		        	border[1][0].y=yy+cosine*size;
		        	border[1][1].x=xx-sine*size;
		        	border[1][1].y=yy-cosine*size;
	
			        double low=border[0][1].x-(ceil(border[0][1].y)-border[0][1].y)*dt,high=border[0][1].x+(ceil(border[0][1].y)-border[0][1].y)*delta;
			        int k=floor(border[0][0].y);
			        for(int i=floor(border[0][1].y);i<floor(border[0][0].y);i++){
						for(int j=floor(low);j<=floor(high);j++){
							dra[i][j]= pen.col;
						}
						low-=dt;
						high+=delta;
					}
					high=border[0][0].x-(ceil(border[0][0].y)-border[0][0].y)*dt;
					low=high-2*size/sine;
			        for(int i=floor(low);i<=floor(border[0][0].x);i++)
				        dra[k][i]=pen.col;//needs optimization(take borders into consideration)
					low-=dt;
			        for(int i=ceil(border[0][0].y);i<floor(border[1][1].y);i++){
				        for(int j=floor(low);j<=floor(high);j++){ 
				            dra[i][j]= pen.col;
						}	
				        low-=dt;
				        high-=dt;
			        }
			        k=floor(border[1][1].y);
			        for(int i=floor(border[1][1].x);i<=floor(high);i++)
			            dra[k][i]=pen.col;//needs optimization(take borders into consideration)
			        low=border[1][1].x;
			        high-=dt;
			        for(int i=ceil(border[1][1].y);i<=floor(border[1][0].y);i++){
						for(int j=floor(low);j<=floor(high);j++){
							dra[i][j]= pen.col;
						}
						low+=delta;
						high-=dt;
					}
		        }
	        }
	    }	
	}
}
