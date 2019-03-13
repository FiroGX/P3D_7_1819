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
		math::vec3 from, at, up;
		float angle, hither, resX, resY;
		
		camera * cam;
		bool b_v = false,
			b_from = false,
			b_at = false,
			b_up = false,
			b_angle = false,
			b_hither = false,
			b_resolution = false;

		while (getline(myfile, line))
		{
			arr = split(line, " ");

			if (arr[0] == "v") { //viewing vectors and angles
				printf("viewing vectors and angles\n");
				b_v = true;
			}
			if (arr[0] == "from" && b_v) {
				printf("from\n");
				from = math::vec3(
					strtof((arr[1]).c_str(), 0),
					strtof((arr[2]).c_str(), 0),
					strtof((arr[3]).c_str(), 0));
				b_from = true;
			}
			if (arr[0] == "at" && b_v) {
				printf("at\n");
				at = math::vec3(
					strtof((arr[1]).c_str(), 0),
					strtof((arr[2]).c_str(), 0),
					strtof((arr[3]).c_str(), 0));
				b_at = true;
			}
			if (arr[0] == "up" && b_v) {
				printf("up\n");
				up = math::vec3(
					strtof((arr[1]).c_str(), 0),
					strtof((arr[2]).c_str(), 0),
					strtof((arr[3]).c_str(), 0));
				b_up = true;
			}
			if (arr[0] == "angle" && b_v) {
				printf("angle\n");
				angle = strtof((arr[1]).c_str(), 0);
				b_angle = true;
			}
			if (arr[0] == "hither" && b_v) {
				printf("hither\n");
				hither = strtof((arr[1]).c_str(), 0);
				b_hither = true;
			}
			if (arr[0] == "resolution" && b_v) {
				printf("resolution\n");
				resX = strtof((arr[1]).c_str(), 0);
				resY = strtof((arr[2]).c_str(), 0);
				b_resolution = true;
			}

			//closes the viewing vectors and angles section and creates the camera
			if (b_v && b_from && b_at && b_up && b_angle && b_hither && b_resolution) {
				
				cam = new camera(from, at, up, hither, 120.0f /*???*/, resX, resY);
				_cam = cam;

				b_v = false;
				b_from = false;
				b_at = false;
				b_up = false;
				b_angle = false;
				b_hither = false;
				b_resolution = false;
			}



			if (arr[0] == "b") { //background color
				printf("background color\n");
				_b_color = new math::vec3(
					strtof((arr[1]).c_str(), 0),
					strtof((arr[2]).c_str(), 0),
					strtof((arr[3]).c_str(), 0));

			}
			if (arr[0] == "l") {//positional light location
				
				printf("Creating positional light:");

				math::vec3 pos = math::vec3( // postion
					strtof((arr[1]).c_str(), 0),
					strtof((arr[2]).c_str(), 0),
					strtof((arr[3]).c_str(), 0));
				
				math::vec3 col = math::vec3( //color
					strtof((arr[4]).c_str(), 0),
					strtof((arr[5]).c_str(), 0),
					strtof((arr[6]).c_str(), 0));

				printf("Adding light to lights vector\n");
				light * luz = new p3d::light(pos, col);

				_lights->push_back(luz);
				
			}
			if (arr[0] == "f") { //object material properties
				printf("object material properties\n");
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
					printf("material doesnt have the right amount of atributes to be set\n");
				}
			}

			if (arr[0] == "c" && mat_flag) { //cone or cylinder primitive
					printf("not implemented\n");
				}

			if (arr[0] == "s" && mat_flag) { //sphere primitive
				printf("creating sphere: ");
					math::vec3 pos(
						strtof((arr[1]).c_str(), 0),
						strtof((arr[2]).c_str(), 0),
						strtof((arr[3]).c_str(), 0));
		
					float rad = strtof((arr[4]).c_str(), 0);
					sphere * sphe = new sphere(pos, rad, mat);
					
					printf("adding sphere to scene objects\n");
					_objs->push_back(sphe);
					
				}

			if (arr[0] == "p" && mat_flag) { //polygon primitive
					printf("not implemented\n");
				}
			if (arr[0] == "pp" && mat_flag) { //polygonal patch primitive
					printf("not implemented\n");
				}
			if (arr[0] == "pl" && mat_flag) { //plane

				printf("creating plane (not implemented)\n");
					//requires 3 different non colinear points
					
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


