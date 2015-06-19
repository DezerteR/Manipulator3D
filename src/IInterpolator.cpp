﻿#include <Utils/Includes.h>
#include <Utils/Utils.h>
#include <Utils/BaseStructs.h>
#include <Utils/IMGUI_V4.h>
#include "Robot.h"
#include "JacobianTransposed.h"
#include "IInterpolator.h"
#include "Widgets.h"

#define _DebugLine_  cerr<<"line: "<<__LINE__<<"\n";

extern float g_timeStep;

/// -------------------------------- LINEAR --------------------------------
glm::vec4 Linear::nextPoint(){
	if(section > maxSections){
		finished = true;
		return points[maxSections];
	}

	auto out = points[section] + sectionStep*step;
	step++;
	if(sectionMaxSteps <= step)
		nextSection();

	// std::cout<<"step: "<<step<<std::endl;
	return out;
}
void Linear::nextSection(){
	section++;
	step = 0;
	sectionStep = points[section+1] - points[section];
	sectionMaxSteps = glm::length(sectionStep)/singleStepLength;
	sectionStep /= sectionMaxSteps;
}
void Linear::generatePath(){
	visualisation.clear();
	visualisation.reserve(100);
	maxSections = points.size()-1;
	section = -1;
	step = 0;
	nextSection();
	finished = false;
	while(!finished){
		visualisation.push_back(nextPoint());
	}
}
void Linear::reset(){
	finished = false;
	section = -1;
	nextSection();
}
void Linear::genNextStep(){
	float velocity = 0.0005; // m/s
	float stepLen = velocity/g_timeStep;

	int next = (currentPoint+1)%points.size();
	vec = points[next] - points[currentPoint];
	steps = glm::length(vec)/stepLen;
	vec = glm::normalize(vec)*stepLen;
	currentPoint = (currentPoint+1)%points.size();
}
void Linear::drawParams(){
	wxg::editBox("step: ", singleStepLength, 160);
}

/// -------------------------------- BEZIERCURVE --------------------------------
glm::vec4 BezierCurve::nextPoint(){
	position = period(position + singleStepLength, 0.f, 1.f);

	return eval(position);
}
glm::vec4 BezierCurve::eval(double param){
	auto t = param;
	int n = points.size()-1;
	glm::vec4 out(0);
	float sum = 0;
	for(int i = 0; i<=n; i++){
			float tmp = Berenstein(t, n, i);
			sum += tmp;
			out += tmp * points[i];
	}
	return out/sum;
}
float BezierCurve::Berenstein(float t, int n, int i){
	// t - parametr
	// n - stopień krzywej
	// i - indeks punktu
	if(i<0 || i >n)
		return 0;
	return factorial(n)/factorial(n-i)/factorial(i)*pow(t, i)*pow(1.f-t, n-i);
}
float BezierCurve::factorial(int k){
	switch (k){
		case 0 : return 1;
		case 1 : return 1;
		case 2 : return 2;
		case 3 : return 6;
		case 4 : return 24;
		case 5 : return 120;
		case 6 : return 720;
		case 7 : return 5040;
		case 8 : return 40320;
	}
}
void BezierCurve::calculateLength(){}
void BezierCurve::generatePath(){
	visualisation.clear();
	for(float k=0.f; k<=1; k+=0.01f){
		visualisation.push_back(eval(k));
	}
}
void BezierCurve::reset(){
	position = 0;
}
void BezierCurve::drawParams(){
	wxg::editBox("step: ", singleStepLength, 160);
}

/// -------------------------------- BSPLINE --------------------------------
glm::vec4 BSpline::nextPoint(){
	position = glm::clamp(position + 0.003, 0.0, (double)numOfBeziers);
	finished = position == (double)numOfBeziers;
	return eval(position);
}
void BSpline::calculateLength(){}

void BSpline::generatePath(){
	makeNurbs();
	visualisation.clear();
	for(float k=0.f; k<=numOfBeziers; k+=0.01f){
		visualisation.push_back(eval(k));
	}
}
void BSpline::makeNurbs(){
	beziers.clear();
	// http://pl.wikipedia.org/wiki/Krzywa_B-sklejana
	// Bezier is made from points{e,f,g,e+1}
	//dla size=8 => m=5, 5xbezier, wszystkich controlsów jest bezier*3+1 0 0
	int degree = 3;
	int m = points.size() - degree;
	numOfBeziers = points.size()-degree;
	vector<glm::vec4> f(m);
	vector<glm::vec4> g(m);
	vector<glm::vec4> e(m+1);

	for(int i=1; i<=m-2; ++i){
		f[i] = (2.f*points[i+1] + points[i+2])/3.f;
		g[i] = (points[i+1] + 2.f*points[i+2])/3.f;
	}

	f[0] = points[0];
	g[0] = (points[0] + points[2])/2.f;
	f[m-1] = (points[m] + points[m+1])/2.f;
	g[m-1] = points[m+1];

	for(int i=1; i<m; ++i)
		e[i] = (g[i-1] + f[i])/2.f;
	e[0] = points[0];
	e[m] = points[m+2];

	for(int i=0; i<m; i++){
		f[i].w = weight;
		g[i].w = weight;
		beziers.push_back(e[i]);
		beziers.push_back(f[i]);
		beziers.push_back(g[i]);
	}
	beziers.push_back(e.back());

}

glm::vec4 BSpline::eval(double param){
	int n = 3;
	double integral;
	double t = modf(param, &integral);
	int start = floor(param)*3;
	glm::vec4 out(0);
	float sum = 0;
	for(int i = start; i<start+4; i++){
			float tmp = Berenstein(t, n, i-start) * beziers[i].w;
			sum += tmp;
			out += tmp * beziers[i];
	}
	return out/sum;
}
float BSpline::Berenstein(float t, int n, int i){
	// t - parametr
	// n - stopień krzywej
	// i - indeks punktu
	if(i<0 || i >n)
		return 0;
	return factorial(n)/factorial(n-i)/factorial(i)*pow(t, i)*pow(1.f-t, n-i);
}
float BSpline::factorial(int k){
	switch (k){
		case 0 : return 1;
		case 1 : return 1;
		case 2 : return 2;
		case 3 : return 6;
		case 4 : return 24;
		case 5 : return 120;
		case 6 : return 720;
		case 7 : return 5040;
		case 8 : return 40320;
	}
}
void BSpline::reset(){
	position = 0;
}

std::vector<BezierCurve> BSpline::split(){}
void BSpline::drawParams(){
	wxg::editBox("weight: ", weight, 160); /// a może slider?
}

/// -------------------------------- NURBS --------------------------------

/// -------------------------------- HERMITECARDINAL --------------------------------
glm::vec4 HermiteCardinal::nextPoint(){
	position = glm::clamp(position + 0.003, 1.0, (double)(numOfSegments));
	finished = position == (double)(numOfSegments);
	return eval(position);
}
void HermiteCardinal::reset(){}
void HermiteCardinal::generatePath(){
    numOfSegments = points.size()-2;

	visualisation.clear();
	visualisation.reserve(numOfSegments*100);
	for(float k=1.f; k<=numOfSegments; k+=0.01f){
		visualisation.push_back(eval(k));
	}

}
void HermiteCardinal::calculateLength(){}
glm::vec4 HermiteCardinal::eval(double param){
	double integral;
	double t = modf(param, &integral);
	double t3 = t*t*t;
	double t2 = t*t;
	int i = integral;

	auto &p0 = points[i-1];
	auto &p1 = points[i];
	auto &p2 = points[i+1];
	auto &p3 = points[i+2];

	auto m1 = float(1-tension)*(p2-p0)/weight;
	auto m2 = float(1-tension)*(p3-p1)/weight;

	return float(2*t3 - 3*t2 + 1)*p1 + float(t3 - 2*t2 + t)*m1 + float(-2*t3 + 3*t2)*p2 + float(t3 - t2)*m2;
}
void HermiteCardinal::drawParams(){
	wxg::editBox("weight: ", weight, 160);
	wxg::editBox("tension: ", tension, 160);
}

/// -------------------------------- HERMITEFINITEDIFFERENCE --------------------------------
glm::vec4 HermiteFiniteDifference::nextPoint(){
	position = glm::clamp(position + 0.003, 0.0, (double)(numOfSegments));
	finished = position == (double)(numOfSegments);
	return eval(position);
}
void HermiteFiniteDifference::reset(){}
void HermiteFiniteDifference::generatePath(){
    numOfSegments = points.size()-1;

	visualisation.clear();
	visualisation.reserve(numOfSegments*100);
	for(float k=0.f; k<=numOfSegments; k+=0.01f){
		visualisation.push_back(eval(k));
	}

}
void HermiteFiniteDifference::calculateLength(){}
glm::vec4 HermiteFiniteDifference::eval(double param){
	double integral;
	double t = modf(param, &integral);
	double t3 = t*t*t;
	double t2 = t*t;
	int i = integral;

	if(t == 0.f)
		return points[i];
	if(i == numOfSegments)
		return points[numOfSegments];

	auto &p1 = points[i];
	auto &p2 = points[i+1];
	auto m1 = (p2 - p1)/weight; // p2-p1  + (p1-p0)
	auto m2 = (p2 - p1)/weight; // (p3-p2)  + p2-p1

	if(i!=0){
		auto &p0 = points[i-1];
		m1 += (p1 - p0)/weight;
	}
	if(i < numOfSegments-1){
		auto &p3 = points[i+2];
		m2 += (p3 - p2);
	}

	return float(2*t3 - 3*t2 + 1)*p1 + float(t3 - 2*t2 + t)*m1 + float(-2*t3 + 3*t2)*p2 + float(t3 - t2)*m2;
}
void HermiteFiniteDifference::drawParams(){
	wxg::editBox("weight: ", weight, 160);
	}

/// -------------------------------- HERMITEFINITEDIFFERENCECLOSED --------------------------------
glm::vec4 HermiteFiniteDifferenceClosed::nextPoint(){
	position = glm::clamp(position + 0.003, 0.0, (double)(numOfSegments));
	finished = position == (double)(numOfSegments);
	return eval(position);
}
void HermiteFiniteDifferenceClosed::reset(){}
void HermiteFiniteDifferenceClosed::generatePath(){
    numOfSegments = points.size();

	visualisation.clear();
	visualisation.reserve(numOfSegments*100);
	for(float k=0.f; k<=numOfSegments; k+=0.01f){
		visualisation.push_back(eval(k));
	}

}
void HermiteFiniteDifferenceClosed::calculateLength(){}
glm::vec4 HermiteFiniteDifferenceClosed::eval(double param){
	double integral;
	double t = modf(param, &integral);
	double t3 = t*t*t;
	double t2 = t*t;
	int i = integral;

	if(t == 0.f)
		return points[i];
	if(i == numOfSegments)
		return points[0];

	auto &p0 = points[(i-1+numOfSegments)%numOfSegments];
	auto &p1 = points[i];
	auto &p2 = points[(i+1)%numOfSegments];
	auto &p3 = points[(i+2)%numOfSegments];
	auto m1 = (p2 - p1)/weight + (p1 - p0)/weight; // p2-p1  + (p1-p0)
	auto m2 = (p2 - p1)/weight + (p3 - p2)/weight; // (p3-p2)  + p2-p1
	return float(2*t3 - 3*t2 + 1)*p1 + float(t3 - 2*t2 + t)*m1 + float(-2*t3 + 3*t2)*p2 + float(t3 - t2)*m2;
}
void HermiteFiniteDifferenceClosed::drawParams(){
	wxg::editBox("weight: ", weight, 160);
	}

/// -------------------------------- FACTORY --------------------------------
shared_ptr<IInterpolator> build(const std::string type, vector<glm::vec4> &points){}
shared_ptr<IInterpolator> InterpolatorFactory::build(Interpolator type, vector<glm::vec4> &points){
	shared_ptr<IInterpolator> out;
	if(type == Interpolator::Linear){
		out = make_shared<Linear>(points);
	}
	else if(type == Interpolator::BezierCurve){
		out = make_shared<BezierCurve>(points);
	}
	else if(type == Interpolator::BSpline){
		out = make_shared<BSpline>(points);
	}
	// else if(type == Interpolator::NURBS){
		// out = make_shared<NURBS>(points);
	// }
	else if(type == Interpolator::HermiteCardinal){
		out = make_shared<HermiteCardinal>(points);
	}
	else if(type == Interpolator::HermiteFiniteDifference){
		out = make_shared<HermiteFiniteDifference>(points);
	}
	else if(type == Interpolator::HermiteFiniteDifferenceClosed){
		out = make_shared<HermiteFiniteDifferenceClosed>(points);
	}

	// out.build();

	return out;
}


std::string sLinear = "Linear";
std::string sBezierCurve = "Bezier Curve";
std::string sBSpline = "B-Spline";
std::string sNURBS = "NURBS";
std::string sHermiteCardinal = "Hermite Cardinal";
std::string sHermiteFiniteDifference = "Hermite Finite Difference";
std::string sHermiteFiniteDifferenceClosed = "Hermite Finite Difference - Closed";

const std::string& InterpolatorTranslate(Interpolator type){
	if(type == Interpolator::Linear)
		return sLinear;
	if(type == Interpolator::BezierCurve)
		return sBezierCurve;
	if(type == Interpolator::BSpline)
		return sBSpline;
	if(type == Interpolator::NURBS)
		return sNURBS;
	if(type == Interpolator::HermiteCardinal)
		return sHermiteCardinal;
	if(type == Interpolator::HermiteFiniteDifference)
		return sHermiteFiniteDifference;
	if(type == Interpolator::HermiteFiniteDifferenceClosed)
		return sHermiteFiniteDifferenceClosed;
}

