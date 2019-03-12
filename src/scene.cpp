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
		while (getline(myfile, line))
		{
			arr = split(line, " ");
			if (arr[0] == "v") { //viewing vectors and angles

			}
			if (arr[0] == "from") {

			}
			if (arr[0] == "at") {

			}
			if (arr[0] == "up") {

			}
			if (arr[0] == "angle") {

			}
			if (arr[0] == "hither") {

			}
			if (arr[0] == "resolution") {

			}
			if (arr[0] == "b") { //background color

			}
			if (arr[0] == "l") {//positional light location

			}
			if (arr[0] == "f") { //object material properties

			}
			if (arr[0] == "c") { //cone or cylinder primitive

			}
			if (arr[0] == "s") { //sphere primitive

			}
			if (arr[0] == "p") { //polygon primitive

			}
			if (arr[0] == "pp") { //polygonal patch primitive

			}
			
		}
		myfile.close();
	}
	else {
		cout << "Unable to open file";
		return false;
	}
	return true;
}


