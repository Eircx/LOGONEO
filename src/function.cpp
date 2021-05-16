#include <iostream>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include "funchead.h"

using namespace std;
#pragma pack(2)
typedef unsigned char BYTE;
typedef unsigned short PSHORT;
typedef unsigned int PINT;
typedef int INT;

typedef struct {
	PSHORT   Bftype;//bmp type, 0x4D42
	PINT     Bfsize;//sizeof bmp file
	PSHORT   Bfreserved1;//reserved,0
	PSHORT   Bfreserved2;//reserved,0
	PINT     BfOffbits;//offset between the fileheader and the beginning of image information
}BITMAPFILEHEADER;

typedef struct {
	PINT    Bisize;
	INT     Biwidth;
	INT     Biheight;
	PSHORT  Biplanes;
	PSHORT  Bibitcount;
	PINT    Bicompression;
	PINT    Bisizeimage; // actucl size of the picture
	INT     Bixpelspermeter;
	INT     Biypelspermeter;
	PINT    Biclrused;
	PINT    Biclrimportant;
} BITMAPINFOHEADER;

//record of the operation
struct color{
	 BYTE b; BYTE g; BYTE r;
};

struct cursor{
	double x; double y; double ang; bool ifclo; int siz; color col;
};//paintbrush location, direction, if cloak; 

struct paralist{                 //a structure to define variabs
	char name[15]; float value;        //I changed int to float
};
struct recfunct{                 // structure for a function
	char name[15]; int fbeg; int fen; int fpanum; int flistbeg;
};
struct borders{
	double x; double y;
};
const long double PI = 3.14159265358979323846264338327950288;
color rec[30000000];
color dra[5000][5000];
color bcolor;
string commands[600];
int sizx, sizy;//size of bmp
cursor pen = {0,0,PI/2,false,0,{0,0,0}};
int commandnum = 0;
color fillc;      //color to fill
color fcolor;     //color to be filled
char tmp_get[100];
paralist variab[100000];
int valistnum = 0;
int rvalistnum = 0;
int q;
recfunct funct[20]; // we support at most 20 functions
char funcpname[100][20];  
int flistnum = 0;	//number of formal variables in all functions
int funcnum = 0;     // number of functions
int funcl=0;   //layer number of function        //**
bool found=0;
bool color[600];

void input() {
	char ffpath[100] = "..\\input\\p.logo";
	cout << "[operation] please enter the input path of the input file" << endl;
	//cin >> ffpath;
	ifstream fin(ffpath);
	if (!fin){
		cout << "[error] inputfile cannot be opened correctly" << endl;
		exit(0); 
	}
	char backg[3][20] = {"@SIZE","@BACKGROUND","@POSITION"};
	char bgw [3][20];
	for (int i = 0; i < 3; i++){
		fin >> bgw[i];
		if (strcmp(bgw[i],backg[i]) != 0){
		 	cout <<"[error] mistake exists in inputfile's basic structure"<< endl;
		 	fin.close();
			exit(0); 
		}
		if (i == 0){
			fin >> sizx >> sizy;
            if(sizx<320||sizx>5000||sizy<240||sizy>5000){         // **
                cout<<"[error][setting]size out of range"<<endl;
                exit(1);
            }
		}
		else if (i == 1){
			int br, bg, bb;
			fin >> br >> bg >> bb; 
            if(color_range(br,bg,bb)){            //**
                cout<<"[caution][setting]color out of range"<<endl;
            }
			bcolor.r = br;               //set background color
			bcolor.g = bg;
			bcolor.b = bb;
		}
        else if (i == 2){
			fin >> pen.x >> pen.y;
            if(pen_range()){        //**
                cout<<"[error][setting]start position out of range"<<endl;
                exit(1);
            }
        }
	}
	while (!fin.eof()){
		string a;
		getline(fin,a);
		if (a.length() != 0){
			commands[commandnum++]=a;
		}
	}
	cout << "[hint] finished input" << endl;
}

bool color_range(int x, int y, int z){       //**
    if(x>256||y>256||z>256||x<0||y<0||z<0)
        return 1;
    return 0;
}

bool pen_range(){              //**
    if(pen.x<0||pen.x>sizx|pen.y>sizy||pen.y<0)
        return 1;
    return 0;
}

void backcolor(){
	for (int j = 0; j < sizy; j++)
		for (int i = 0; i < sizx; i++){
			dra[j][i] = bcolor;
	}
}

void conver(){            //transfor two-dimensional array into one-dimentional, designed for output
	for (int i = 0; i < sizy; i++){
		for (int j = 0; j < sizx; j++){
			rec[i*sizx+j] = dra[i][j];
		}
	}
}

void dswap(double &x,double &y){     //exchange x y
	double tmp;
	tmp=x;
	x=y;
	y=tmp;
}

void output() {
	char outfname[100] = "..\\output\\test\\p.bmp";
	cout << "[operation] please input the file address" << endl;
	//cin >> outfname;
	cout << "[hint] began output" << endl;
	int bmpsize = sizx * sizy * 3;
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader = {0};
	infoheader.Bisize = sizeof(BITMAPINFOHEADER);
	infoheader.Biwidth = sizx;
	infoheader.Biheight = -sizy;
	infoheader.Biplanes = 1;
	infoheader.Bibitcount = 24;
	infoheader.Bicompression = 0;
	infoheader.Bisizeimage = bmpsize;
	infoheader.Bixpelspermeter = 0;
	infoheader.Biypelspermeter = 0;
	infoheader.Biclrused = 0;
	infoheader.Biclrimportant = 0;
	fileheader.Bftype = 0x4D42;
	fileheader.BfOffbits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	fileheader.Bfsize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	fileheader.Bfreserved2 = 0;
	fileheader.Bfreserved1 = 0;
	FILE *outputfile = fopen(outfname,"wb");
	if (outputfile == NULL){
		cout << "[error] output error" << endl;
		exit(7); 
	}
	fwrite(&fileheader,sizeof(BITMAPFILEHEADER),1,outputfile);
	fwrite(&infoheader,sizeof(BITMAPINFOHEADER),1,outputfile);
	fwrite(&rec,bmpsize,1,outputfile);
	fclose(outputfile);
    cout << "[hint] Congratulation!You have succesfully created a BMP file" << endl;
}

float gett_float(int i, int &g){
	for (int j = 1; j < 100; j++){
		if (commands[i][g] != ' ') break;
		g++;
	}
	stringstream ss;
	float tmp;
	do{
		ss << commands[i][g];
		g++;
	}while (commands[i][g] != ' ' && commands[i][g] != ')' && commands[i][g] != ',' && g < commands[i].length());
	ss >> tmp; 
	return tmp;
}

int gett_char(int i, int &g){              //input a char, and judge its type
	if (g >= commands[i].length()) return -1;
	while(g < commands[i].length()){
		if (commands[i][g] !=' ') break;
		g++;
	}
	if (commands[i][g] == '-') return 2;   //2: to acquire number
	for (int k = 0; k < 10; k++){
		if (commands[i][g] == '0' + k)     //2: to acquire number
			return 2;//recognize number
	}
	while(g < commands[i].length()){
		if (commands[i][g] == ',' || commands[i][g] == '('){
			g++;
			return 1;      //represents some characters: 1
		}
		else if (commands[i][g] == ')')    //represents" )"character: -1
				return -1;
		else{
			memset(tmp_get,0,sizeof(tmp_get));
			stringstream ss;
			while(commands[i][g] != ' ' &&commands[i][g] != '(' && commands[i][g] != ',' && commands[i][g] != ')' && g < commands[i].length()){
				ss << commands[i][g];
				g++;
			}
			ss >> tmp_get;
			return 0;              //get char
		}
	}
    return 0;           //may reach an void end here      **
}

void move(float x){
	double dx, dy;
	dx = x * cos(pen.ang);
	dy = x * sin(pen.ang);
	if (!pen.ifclo){
		if(2*pen.siz+1>=x){
			drawline_first(pen.x,pen.y,pen.siz,pen.x+dx,pen.y+dy,pen.ang);
		}
		else{
			drawline_second(pen.x,pen.y,pen.siz,x,pen.x+dx,pen.y+dy);
		}
	}
	pen.x += dx;
	pen.y += dy;
}


void turn(int x){
	pen.ang -= x / 180.0 * PI;
	if (pen.ang > 2 * PI)
		pen.ang -= int(pen.ang/PI) * 2 * PI;
	if (pen.ang < 0)
		pen.ang += int(1-pen.ang/PI) * 2 * PI;
}

double dmmax(double a, double b){    //compare absolute valua of a and b
	if (a < 0) a = -a;
	if (b < 0) b = -b;
	if (a > b) return a;
	return b;
}

void delvar(int i, int g){            //To delete a letter
	int x = gett_char(i,g);               //receive a char here         * debug *
    if(x!=0){
            cout<<"[error][line"<<i<<"]wrong type"<<endl;  //**
            exit(2);
    }
    int j = 0;
    found=0;                           //**
	for (; j < rvalistnum; j++){                  //j is smaller than valistnum
		if(strcmp(tmp_get,variab[j].name) == 0){
            found=1;
			for (int k = rvalistnum - 1; k > j; k--){
				strcpy(variab[k-1].name,variab[k].name);
				variab[k-1].value = variab[k].value;    //May it cause all the valuas the same?? ?
			}
			rvalistnum--;
            valistnum--;
            q--;
			break;
		}
	}
}

void operation() {          //main operate here
	cout << "[hint] magic pen is drawing" << endl;
	backcolor();
	int i = 0;
	while(i < commandnum){          //read rows by rows, and operate in accordance with instructions
		readcom(i);
	}
	conver();
}

bool cmpcol(int y, int x){        //compare whether color differs between (y,x) and origin color
	if (dra[y][x].r == fcolor.r && dra[y][x].g == fcolor.g && dra[y][x].b == fcolor.b) return true;
	return false;
}

float get_para(int i,int &g){       //colloct canshu and transfer to its valua
    float x; int m;
	m = gett_char(i,g);          //use its return valuas to differ situations
	if (m == 2){
		x = gett_float(i,g);       //later return x
	}
	else if (m == -1 || m == 1) return -2500;   //can't get
	else if (m == 0){             // get char
		int v = 0;
		for (int k = 0; k < rvalistnum; k++){        //variabs when calling function
			if (strcmp(tmp_get,variab[k].name) == 0){
				x = variab[k].value;     //x is that variab's valua
				v = 1;
				break;
			}	
		}
		if (funcl > 0)	{
			for (int k = q; k < valistnum; k++){      //all variables
				if (strcmp(tmp_get,variab[k].name) == 0){
					x = variab[k].value;
					v = 1;
					break;
				}	
			}
		}
		if (v == 0) return -3000;    //an undefined new variab
	}
	return x;
}

void readcom(int &i){         //read and operate the ith row
	int g = 0;
	int m;
    m = gett_char(i,g); int c=q;
	if (strcmp(tmp_get,"MOVE") == 0){      //move
		float xx = get_para(i,g);
        if(xx<-2000){                       //**
            (xx==-2500)?cout<<"[error][line"<<i<<"]wrong type"<<endl:cout<<"[error][line"<<i<<"]undefined variab"<<endl;
            exit(2);
        }
        move(xx);
        if(pen_range()){
            cout<<"[error][line "<<i<<"]pen out of range"<<endl;
            exit(1);
        }
		i++;
	}
	else if (strcmp(tmp_get,"COLOR") == 0){   //change color
		pen.ifclo = false;                    //be visible (or there's no need to change color)
		for (int k = 0; k < 3; k++){
			float tmp = get_para(i,g);
			if (tmp < -2400) break;
			if(!color[i])
			if (tmp > 255 || tmp < 0){
                cout<<"[caution][line "<<i<<"]color out of range"<<endl;    //**
                color[i]=1;
			} 
			if (k == 0)
				pen.col.r = tmp;
			if (k == 1)
				pen.col.g = tmp;
			if (k == 2)
				pen.col.b = tmp;
		}
		i++;
	}
	else if (strcmp(tmp_get,"CLOAK") == 0){    //be invisible
		pen.ifclo = true;
		i++;
	}
	else if (strcmp(tmp_get,"TURN") == 0){     //turn
		int x = get_para(i,g);
		turn(x);
		i++;
	}
	else if (strcmp(tmp_get,"DEF") == 0){      //def a new variab
		int x; 
		x = gett_char(i,g);
		strcpy(variab[valistnum].name,tmp_get);        //debug: if have been defined **
		variab[valistnum].value = gett_float(i,g);
		int l=strlen(tmp_get);
		for(int k=0;k<l;k++){
			int judge=tmp_get[k]-'a';
			if(judge<0||judge>25){
				cout<<"[error][line"<<i<<"]incorrect definition of vairab"<<endl;
				exit(2);
			}
		}
		valistnum++;
		if (funcl == 0){
			rvalistnum++;
			q++;
		}
            for (int j=0; j < valistnum-1; j++){               //**
                if(strcmp(tmp_get,variab[j].name) == 0){
                    cout<<"[error][line"<<i<<"]redefine of "<<tmp_get<<endl;
                    exit(2);
                }
        }
		i++;
	}
	else if (strcmp(tmp_get,"SIZE") == 0) {     //change pen size
		float x = get_para(i,g);
		pen.siz = x;
		i++;
	}
	else if (strcmp(tmp_get,"ADD") == 0){
        int m = gett_char(i,g);
        if(m!=0){
            cout<<"[error][line"<<i<<"]wrong type"<<endl;  //**
            exit(2);
        }
		int j = 0;
		for (; j < rvalistnum; j++){
			if(strcmp(tmp_get,variab[j].name) == 0){
				float x = get_para(i,g);
                if(x<-2400){
                    cout<<"[error][line"<<i<<"]wrong type"<<endl;  //**
                    exit(2);
                }
				variab[j].value += x;
				break;
			}
		}
        if (funcl > 0){
        j = q;
        for (; j < valistnum; j++){
            if(strcmp(tmp_get,variab[j].name) == 0){
                    float x = get_para(i,g);
                    if(x<=-2400){
                        cout<<"[error][line"<<i<<"]wrong type"<<endl;  //**
                        exit(2);
                    }
                    variab[j].value += x;
                    break;
                }
            }
        }
		i++;
	}
    else if (strcmp(tmp_get,"PRO") == 0){         //multiple  **
        int m = gett_char(i,g);
        if(m!=0){
            cout<<"[error][line"<<i<<"]wrong type"<<endl;
            exit(2);
        }
        int j = 0;
        for (; j < rvalistnum; j++){
            if(strcmp(tmp_get,variab[j].name) == 0){
                float x = get_para(i,g);
                if(x<-2400){
                    cout<<"[error][line"<<i<<"]wrong type"<<endl;  //**
                    exit(2);
                }
                variab[j].value = variab[j].value * x;
                break;
            }
        }
        if (funcl > 0){
        j = q;
        for (; j < valistnum; j++){
        if(strcmp(tmp_get,variab[j].name) == 0){
                float x = get_para(i,g);
                if(x<=-2400){
                    cout<<"[error][line"<<i<<"]wrong type"<<endl;  //**
                    exit(2);
                }
                variab[j].value *= x;
                break;
            }
        }
        }
        i++;
    }
	else if (strcmp(tmp_get,"DELETE") == 0){   //delete a parameter
		delvar(i,g);
        if(!found){                           //**
            cout<<"[error][line"<<i<<"]this variab doesnot exist"<<endl;
            exit(2);
        }
		i++;
	}
	else if (strcmp(tmp_get,"LOOP") == 0){     //start a loop
		int looplev = 1;
		float lnum = get_para(i,g);     //record cycle number of the loop
		g = 0;
		int lbeg, lend, pa, v;
		lbeg = i + 1;             //record begin of loop
		lend = i + 1;             //record end of loop
		do{
			v = gett_char(lend,g);
			if (strcmp(tmp_get,"LOOP") == 0)
				looplev++;
			else if (strcmp(tmp_get,"END") == 0){
				v = gett_char(lend,g);
				if (strcmp(tmp_get,"LOOP") == 0)
					looplev--;
			}
			g = 0;
			lend++;
            if(lend>commandnum){                          //**
                cout<<"[error][line "<<i<<"]LOOP without END"<<endl;
                exit(3);
            }
		} while(looplev > 0); //find the end of loop
		g = 0; 
		for (int m = 0; m < lnum; m++){
			pa = lbeg;                 //read that line
			while(pa < lend - 1){
				readcom(pa);
			}
		}
		i = lend;   //the line below the "END LOOP" line
	} 
	else if (strcmp(tmp_get,"FILL") == 0){  //fill color (extensive function)
		float t[3];
		for (int k = 0; k < 3; k++){
			t[k] = get_para(i,g);
		}
		fillc.r = t[0];
		fillc.g = t[1];
		fillc.b = t[2];
		fcolor = dra[int(pen.y)][int(pen.x)];
		fillcol(int(pen.y),int(pen.x)); 
		i++;
	}
	else if (strcmp(tmp_get,"FUNC") == 0){       //create a function
		int m;
		funct[funcnum].fbeg = i + 1;            //start line
		funct[funcnum].fen = i + 1;             //end line
		while(funct[funcnum].fen <= commandnum){
			int va = 0;
			m = gett_char(funct[funcnum].fen,va);
			if (strcmp(tmp_get,"END") == 0){
				m = gett_char(funct[funcnum].fen,va);
				if (strcmp(tmp_get,"FUNC") == 0){
					break;
				}
			}
			funct[funcnum].fen++;
            if(funct[funcnum].fen==commandnum){           //**
                cout<<"[error][line "<<i<<"]FUNC without END"<<endl;
                exit(3);
            }
		}
		funct[funcnum].fen--;           //now locate at its last line
		m = gett_char(i,g);
		strcpy(funct[funcnum].name,tmp_get);
		funct[funcnum].flistbeg = flistnum;   //parameter inside function
		funct[funcnum].fpanum = 0;            //parameter number in this function
		while(m != -1){
			m = gett_char(i,g);
			if (m == 1){
				m = gett_char(i,g);
				if (m == -1) break;
				strcpy(funcpname[flistnum],tmp_get); //input func parameter name
				funct[funcnum].fpanum++;         //parameter number++
				flistnum++;
			}
		}
		i = funct[funcnum].fen + 2;
		funcnum++;
	}
	else if (strcmp(tmp_get,"CALL") == 0){//call function
        int qq = q;
		int m = gett_char(i,g);
		int k = -1;
        found=0;
		for (; k < funcnum; k++){
			if (strcmp(funct[k].name,tmp_get) == 0){
                found=1;
				break;
			}
		}
        if(!found){
            cout<<"[error][line"<<i<<"]undefined function"<<endl;    //**
            exit(4);
        }
		int v = funct[k].fpanum + funct[k].flistbeg;
		int w = funct[k].flistbeg;
		int f = 0;
		while(w < v){
			m = gett_char(i,g);
			if (m == 1){
				float a = get_para(i,g);
				variab[valistnum+f].value = a;
				strcpy(variab[valistnum+f].name,funcpname[w]);   //make correlation of parameters
				w++;
				f++;
			}
		}
        q = valistnum;
        valistnum += funct[k].fpanum;
        funcl++;        //be inside function
		int ifunc = funct[k].fbeg;
		while(ifunc <= funct[k].fen){  //call function
			readcom(ifunc);
		}
		for (int h = q; h < valistnum; h++){
			strcpy(variab[h].name,"");
			variab[h].value = 0;
		}
		valistnum = q;
        q = qq;//  **
        funcl--;      //**
		i++;
	}
    else if (strcmp(tmp_get,"IF") == 0){
        bool type;
        int m = gett_char(i,g);
        int j = 0; float x = 0;
        for (; j < rvalistnum; j++){
            if(strcmp(tmp_get,variab[j].name) == 0){
                m=gett_char(i,g);
                if(strcmp(tmp_get,"MORETHAN")==0) type=1;
                else if(strcmp(tmp_get,"LESSTHAN")==0) type=0;
                else{
                    cout<<"[error][line"<<i<<"]incorrect use of IF"<<endl;    //**
                    exit(5);
                }
                x = get_para(i,g);
                break;
            }
        }
        if(x==0){ j = q;
        for (; j < valistnum; j++){
            if(strcmp(tmp_get,variab[j].name) == 0){
                m=gett_char(i,g);
                if(strcmp(tmp_get,"MORETHAN")==0) type=1;
                else if(strcmp(tmp_get,"LESSTHAN")==0) type=0;
                else{
                    cout<<"[error][line"<<i<<"]incorrect use of IF"<<endl;    //**
                    exit(5);
                }
                x = get_para(i,g);
                break;
            }
        }
        }
        m = gett_char(i,g);
        if(strcmp(tmp_get,"STOP")==0){
        if(type){
        if(variab[j].value>x)
            i+=500; }
        else{
            if(variab[j].value<x) i+=500;}
            i++;
        }
        else if(strcmp(tmp_get,"THEN")==0){
            bool satisfy = 0;
            if(type){
                if(variab[j].value>x) satisfy = true;  }
            else
                if(variab[j].value<x) satisfy = true;
            if(satisfy){
                int line=i+1;
                while(line<commandnum){
                    int g_shade = 0;
                    int m = gett_char(line, g_shade);
                    if(strcmp(tmp_get,"END")==0){
                        m = gett_char(line, g_shade);
                        if(strcmp(tmp_get,"IF")==0)
                            break;}
                    readcom(line);
                }
                i = line+1;
                if(line==commandnum){
                    cout<<"[error][line"<<i<<"]IF without END"<<endl;    //**
                    exit(3);
                }
            }
            else{
                int line=i+1;
                for(;line<commandnum;line++){
                    int g_shade = 0;
                    int m = gett_char(line, g_shade);
                    if(strcmp(tmp_get,"END")==0){
                        m = gett_char(line, g_shade);
                        if(strcmp(tmp_get,"IF")==0)
                            break;}
                }
                if(line==commandnum){
                    cout<<"[error][line"<<i<<"]IF without END"<<endl;    //**
                    exit(3);
                }
                i = line+1;
            }
        }
        else{
            cout<<"[error][line"<<i<<"]IF without related word"<<endl;
            exit(5);
        }
    }
    else if(strcmp(tmp_get,"\0")==0||strcmp(tmp_get,"\n")==0) i++;
    else{
        cout<<"[error][line"<<i<<"]incorrect program"<<endl;
        exit(6);
    }
}

void fillcol(int y,int x){
	int* lef = new int[sizy/2];
	int* rig = new int[sizy/2];
	int lefnum = 0, rignum = 0;
	bool iflef = false, ifrig = false;
	for (int i = 0; i <= y; i++){
		if (!cmpcol(y-i,x)) break;
		dra[y-i][x] = fillc;
		if (x > 0){
			if (cmpcol(y-i,x-1) && !iflef){
				iflef = true;
				lef[lefnum] = y-i;
				lefnum++;
			}
			else if (!cmpcol(y-i,x-1)){
				if (iflef)
					iflef = false;
			}
		}
		if (x + 1 < sizx){
			if (cmpcol(y-i,x+1) && !ifrig){
				ifrig = true;
				rig[rignum] = y-i;
				rignum++;
			}
			else if (!cmpcol(y-i,x+1)){
				if (ifrig)
					ifrig = false;
			}
		}
	}
	iflef = false;
	ifrig = false;
	for (int i = 1; i < sizy - y; i++){
		if (!cmpcol(y+i,x)) break;
		dra[y+i][x] = fillc;
		if (x > 0){
			if (cmpcol(y+i,x-1) && !iflef){
				iflef = true;
				lef[lefnum] = y+i;
				lefnum++;
			}
			else if (!cmpcol(y+i,x-1)){
				if (iflef)
					iflef = false;
			}
		}
		if (x + 1 < sizx){
			if (cmpcol(y+i,x+1) && !ifrig){
				ifrig = true;
				rig[rignum] = y+i;
				rignum++;
			}
			else if (!cmpcol(y+i,x+1)){
				if (ifrig)
					ifrig = false;
			}
		}
	}
	if (lefnum + rignum != 0) {
		for (int i = 0; i < lefnum; i++)
			fillcol(lef[i],x-1);
		for (int i = 0; i < rignum; i++)
			fillcol(rig[i],x+1);
	}
	delete [] lef;
	delete [] rig;
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
