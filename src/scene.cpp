#include "scene.hpp"
#include <sstream>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>


p3d::scene::~scene() {
	for (auto l : *lights()) delete l;
	for (auto o : *objs()) delete o;
}

p3d::camera *p3d::scene::cam() const {
	return _cam;
}

math::vec3 *p3d::scene::b_color() const {
	return _b_color;
}

std::vector<p3d::light*> *p3d::scene::lights() const {
	return _lights;
}

std::vector<p3d::scene_obj*> *p3d::scene::objs() const {
	return _objs;
}

/*
Taken from the interwebs
It returns a string array that each element contains a subset of the original string str up to the next separator sep segment
*/
std::vector<std::string> split(std::string str, std::string sep) {
	char* cstr = const_cast<char*>(str.c_str());
	char* current;
	std::vector<std::string> arr;
	current = strtok(cstr, sep.c_str());
	while (current != NULL) {
		arr.push_back(current);
		current = strtok(NULL, sep.c_str());
	}
	return arr;
}

bool p3d::scene::load_nff(std::string name){

	using namespace std;
	string line;
	ifstream myfile(name);
	if (myfile.is_open())
	{
		std::vector<std::string> arr;
		material mat;
		bool mat_flag = false;
		
		while (getline(myfile, line))
		{
			arr = split(line, " ");


			if (arr[0] == "v") { //viewing vectors and angles

			}
			if (arr[0] == "from") {
				/*
				new math::vec3(
					strtof((arr[1]).c_str(), 0),
					strtof((arr[2]).c_str(), 0),
					strtof((arr[3]).c_str(), 0));
				*/
			}
			if (arr[0] == "at") {
				/*
				new math::vec3(
					strtof((arr[1]).c_str(), 0),
					strtof((arr[2]).c_str(), 0),
					strtof((arr[3]).c_str(), 0));
				*/
			}
			if (arr[0] == "up") {
				/*
				new math::vec3(
					strtof((arr[1]).c_str(), 0),
					strtof((arr[2]).c_str(), 0),
					strtof((arr[3]).c_str(), 0));
				*/
			}
			if (arr[0] == "angle") {
				//strtof((arr[1]).c_str(), 0);
			}
			if (arr[0] == "hither") {
				//strtof((arr[1]).c_str(), 0);
			}
			if (arr[0] == "resolution") {
				//strtof((arr[1]).c_str(), 0);
				//strtof((arr[2]).c_str(), 0);
			}
			if (arr[0] == "b") { //background color
				_b_color = new math::vec3(
					strtof((arr[1]).c_str(), 0),
					strtof((arr[2]).c_str(), 0),
					strtof((arr[3]).c_str(), 0));

			}
			if (arr[0] == "l") {//positional light location
				/*
				new math::vec3( // postion
					strtof((arr[1]).c_str(), 0),
					strtof((arr[2]).c_str(), 0),
					strtof((arr[3]).c_str(), 0));
				*/
				/*
				new math::vec3(
					strtof((arr[4]).c_str(), 0),
					strtof((arr[5]).c_str(), 0),
					strtof((arr[6]).c_str(), 0));
				*/
			}
			if (arr[0] == "f") { //object material properties
				if (arr.size() == 9) {

					math::vec3 aux =  math::vec3(
						strtof((arr[1]).c_str(), 0),
						strtof((arr[2]).c_str(), 0),
						strtof((arr[3]).c_str(), 0));
					mat = material( aux,
						strtof((arr[4]).c_str(), 0),
						strtof((arr[5]).c_str(), 0),
						strtof((arr[6]).c_str(), 0),
						strtof((arr[7]).c_str(), 0),
						strtof((arr[8]).c_str(), 0));
					mat_flag = true;
				}
				else {
					printf("material doesnt have the right amount of atributes");
				}
			}
			if (arr[0] == "c") { //cone or cylinder primitive

			}
			if (arr[0] == "s") { //sphere primitive
				
				math::vec3 pos(
					strtof((arr[1]).c_str(), 0),
					strtof((arr[2]).c_str(), 0),
					strtof((arr[3]).c_str(), 0));

				float rad = strtof((arr[4]).c_str(), 0);

				sphere * sphe = new sphere(pos, rad, material());

				_objs->push_back(sphe);
			
			}
			if (arr[0] == "p") { //polygon primitive

			}
			if (arr[0] == "pp") { //polygonal patch primitive

			}
			if (arr[0] == "pl") { //plane
				//requires 3 different non colinear points
				
			}
			
		}
		myfile.close();
	}
	else {
		cout << "Unable to open file";
		return false;
	}
	return true; //TODO change this to true
}


