﻿#pragma once
#include "Robot.h"

// szerokość jest drugim parametrem
class Matrix {
public:
	u32 width;
	u32 height;
	std::vector<double> array;
	Matrix(u32 h, u32 w);
	Matrix& operator =(const Matrix &matrix);
	Matrix(const Matrix &matrix);
	~Matrix(){
		// delete [] array;
	}
	double& operator()(u32 h, u32 w=0);
	float operator()(u32 h, u32 w=0) const ;

	Matrix operator+(Matrix &matrix);
	Matrix operator*(Matrix &matrix);
	Matrix operator*(float val);
	float mulRowColumn(u32 row, u32 column, Matrix &second);

	void insertRow(u32 row, std::vector<float> &&vec);
	void insertColumn(u32 column, std::vector<float> &&vec);
	friend std::ostream & operator <<(std::ostream& output, const Matrix &mat){

		for(u32 h=0; h<mat.height; h++){
			for(u32 w=0; w<mat.width; w++)
				output<<std::to_string(mat(h,w))+"\t";
			output<<"\n";
		}
		return output;
	}
	Matrix& transpose();
	Matrix transposed();
	std::vector<double>& getVector();
};

float dot(const Matrix &a, const Matrix &b);
Matrix mul(const Matrix &a, const Matrix &b);


Matrix buildJacobian(Robot &robot);
Matrix buildJacobian(Robot &robot, std::vector<double> &variables);

void clamp(std::vector<double> &vec, double min, double max);
void clamp(Matrix &mat, double min, double max);



void BADBADBADRobotIKRealtimeTest(Robot &robot);

void test(Robot &robot);
