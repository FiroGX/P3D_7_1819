#include "scene.hpp"

#include "sphere.hpp"

#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <string>


p3d::scene::scene() {}

p3d::scene::~scene() {
	for (auto l : lights()) delete l;
	for (auto o : objs()) delete o;
}

p3d::camera p3d::scene::cam() const {
	return _cam;
}

math::vec3 p3d::scene::b_color() const {
	return _b_color;
}

std::vector<p3d::light*> p3d::scene::lights() const {
	return _lights;
}

std::vector<p3d::scene_obj*> p3d::scene::objs() const {
	return _objs;
}

/*
Taken from the interwebs
It returns a string array that each element contains a subset of the original string str up to the next separator sep segment
*/
std::vector<std::string> split(const std::string str, const std::string sep) {
	std::istringstream in(str);
	std::istream_iterator<std::string> begin(in), end;
	return std::vector<std::string>(begin, end);
}

bool p3d::scene::load_nff(const std::string name) {
	std::ifstream myfile(name);
	if (myfile.is_open()) {
		std::string line;
		std::vector<std::string> arr;

		material mat;
		bool mat_flag = false;

		math::vec3 from, at, up;
		float angle, hither;
		int resX, resY;

		bool b_v, b_at, b_up, b_from,
			b_angle, b_hither, b_resolution;

		b_v = b_at = b_up = b_from =
			b_angle = b_hither = b_resolution = false;

		while (std::getline(myfile, line)) {
			arr = split(line, " ");

			if (arr[0] == "v" && !b_v) { //viewing vectors and angles
				std::cout << "viewing vectors and angles" << std::endl;
				b_v = true;
			} else if (arr[0] == "from" && b_v) {
				std::cout << "from" << std::endl;
				from = math::vec3(std::atof(arr[1].c_str()),
					std::atof(arr[2].c_str()),
					std::atof(arr[3].c_str()));
				b_from = true;
			} else if (arr[0] == "at" && b_v) {
				std::cout << "at" << std::endl;
				at = math::vec3(std::atof(arr[1].c_str()),
					std::atof(arr[2].c_str()),
					std::atof(arr[3].c_str()));
				b_at = true;
			} else if (arr[0] == "up" && b_v) {
				std::cout << "up" << std::endl;
				up = math::vec3(std::atof(arr[1].c_str()),
					std::atof(arr[2].c_str()),
					std::atof(arr[3].c_str()));
				b_up = true;
			} else if (arr[0] == "angle" && b_v) {
				std::cout << "angle" << std::endl;
				angle = std::atof(arr[1].c_str());
				b_angle = true;
			} else if (arr[0] == "hither" && b_v) {
				std::cout << "hither" << std::endl;
				hither = std::atof(arr[1].c_str());
				b_hither = true;
			} else if (arr[0] == "resolution" && b_v) {
				std::cout << "resolution" << std::endl;
				resX = std::atoi(arr[1].c_str());
				resY = std::atoi(arr[2].c_str());
				b_resolution = true;
			} else if (arr[0] == "b") { //background color
				std::cout << "background color" << std::endl;
				_b_color = math::vec3(std::atof(arr[1].c_str()),
					std::atof(arr[2].c_str()),
					std::atof(arr[3].c_str()));
			} else if (arr[0] == "l") { //positional light location
				std::cout << "creating positional light" << std::endl;
				// position
				math::vec3 pos = math::vec3(std::atof(arr[1].c_str()),
					std::atof(arr[2].c_str()),
					std::atof(arr[3].c_str()));
				// color
				math::vec3 col = math::vec3(std::atof(arr[1].c_str()),
					std::atof(arr[2].c_str()),
					std::atof(arr[3].c_str()));

				std::cout << "adding light to lights vector" << std::endl;
				light *l = new p3d::light(pos, col);
				_lights.push_back(l);
			} else if (arr[0] == "f") { //object material properties
				std::cout << "object material properties" << std::endl;
				if (arr.size() == 9) {
					math::vec3 color = math::vec3(std::atof(arr[1].c_str()),
						std::atof(arr[2].c_str()),
						std::atof(arr[3].c_str()));

					mat = material(color, std::atof(arr[4].c_str()),
						std::atof(arr[5].c_str()),
						std::atof(arr[6].c_str()),
						std::atof(arr[7].c_str()),
						std::atof(arr[8].c_str()));
					mat_flag = true;
				} else {
					std::cout <<
						"material doesn\'t have the right amount of atributes to be set"
						<< std::endl;
				}
			} else if (arr[0] == "c" && mat_flag) { //cone or cylinder primitive
				std::cout << "not implemented" << std::endl;
			} else if (arr[0] == "s" && mat_flag) { //sphere primitive
				std::cout << "creating sphere" << std::endl;
				math::vec3 pos(std::atof(arr[1].c_str()),
					std::atof(arr[2].c_str()),
					std::atof(arr[3].c_str()));

				float rad = std::atof(arr[4].c_str());
				sphere *sphe = new sphere(pos, rad, mat);

				std::cout << "adding sphere to scene objects" << std::endl;
				_objs.push_back(sphe);
			} else if (arr[0] == "p" && mat_flag) { //polygon primitive
				std::cout << "not implemented" << std::endl;
			} else if (arr[0] == "pp" && mat_flag) { //polygonal patch primitive
				std::cout << "not implemented" << std::endl;
			} else if (arr[0] == "pl" && mat_flag) { //plane
				std::cout << "plane not implemented" << std::endl;
				//requires 3 different non colinear points
			}

			//closes the viewing vectors and angles section and creates the camera
			if (b_v && b_from && b_at && b_up && b_angle && b_hither && b_resolution) {
				_cam = camera(from, at, up, hither, angle, resX, resY);

				b_v = false;
				b_from = false;
				b_at = false;
				b_up = false;
				b_angle = false;
				b_hither = false;
				b_resolution = false;
			}

		}
		myfile.close();
	} else {
		std::cout << "Unable to open file" << std::endl;
		return false;
	}
	return true;
}


