//funchead.h
#ifndef FUNCHEAD_H
#define FUNCHEAD_H
#pragma pack(2)

void turn(int);
void input(void);
void operation(void);
void backcolor(void);
void output(void);
void conver(void);
void move(float);
void readcom(int&);
bool debug(void);
int gett_char(int,int&);
float gett_float(int,int&);
void drawline_first(double,double,int,double,double,double);
void drawline_second(double,double,int,double,double,double);
void delvar(int);
void fillcol(int,int);
bool cmpcol(int,int);
float get_para(int,int&);
void dswap(double&,double&);
double dmmax(double, double);
bool color_range(int,int,int); //**
bool pen_range(void);    //**
#endif
