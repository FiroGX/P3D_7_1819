#include <stdlib.h>
#include <GL/glut.h>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <cmath>
#include <ctime>
#include <random>

#include "hit.hpp"
#include "ray.hpp"
#include "scene.hpp"
#include "area_light.hpp"
#include "math/vec3.hpp"

#define MAX_DEPTH 3

#define JITTERING true
#define SAMPLE_SIZE 2
#define GRID false

#define DOF true
#define FOCAL_PLANE_DISTANCE 5;
#define APERTURE 0.01	// lens radius

using namespace p3d;

scene sce;
int RES_X, RES_Y;

void reshape(int w, int h) {

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0, RES_X - 1, 0, RES_Y - 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/* Shadow Feeler calculations; returns a bool according to collision */
bool point_in_shadow(const math::vec3 &hit_pos, const math::vec3 &l_dir) {
	ray ray(hit_pos, l_dir);
	ray.offset(math::KEPSILON);
	hit shadow_feeler = sce.calculate_hit(ray);
	return shadow_feeler.collided();
}

/* Calculates the color of the intersection point */
math::vec3 point_shading(math::vec3 &light_direction, const hit &hit, const ray &ray, light *l) {
	math::vec3 color(0.0f, 0.0f, 0.0f);
	float lambert = math::dot(light_direction, hit.normal());
	if (lambert > 0.0f)
		if (!point_in_shadow(hit.point(), light_direction)) {  	//cast a shadow feeler normally
			color += hit.mat().color() * l->color() * lambert * hit.mat().kd();
			math::vec3 half_vec = math::normalize(-ray.d() + light_direction);
			color += l->color() * hit.mat().ks() * std::pow(math::dot(hit.normal(), half_vec), hit.mat().shine());
		}
	return color;
}

/* Local color calculation */
math::vec3 local_color(const ray &ray, const hit &hit, const std::pair<float, float> &light_sample) {
	math::vec3 color(0.0f, 0.0f, 0.0f);
	for (auto* l : sce.lights()) {  //for each light source

		if (!l->is_area()) { //if light-source is a point light
			math::vec3 l_dir = math::normalize(l->pos() - hit.point());
			color += point_shading(l_dir, hit, ray, l);
		} //further cases are area lights (is_area() returns true)
		else if (JITTERING) { //area light with jittering flag is on
			p3d::area_light *area = static_cast<p3d::area_light*>(l);
			math::vec3 l_dir = math::normalize(area->pos(light_sample.first, light_sample.second) - ray.o());
			color += point_shading(l_dir, hit, ray, area);
		}
		else { //area light with no jittering
			math::vec3 color_samples(0.0f, 0.0f, 0.0f);
			for (int i = 0; i < SAMPLE_SIZE * SAMPLE_SIZE; i++) {
				//division of the light source in SAMPLE_SIZE * SAMPLE_SIZE
				int p = i / SAMPLE_SIZE;
				int q = i % SAMPLE_SIZE;

				//defining random points in each light source sub-division
				float a_epsilon = (((float)std::rand() / (float)RAND_MAX) + p) / SAMPLE_SIZE;
				float b_epsilon = (((float)std::rand() / (float)RAND_MAX) + q) / SAMPLE_SIZE;

				//calculate the light samples direction
				p3d::area_light *area = static_cast<p3d::area_light*>(l);
				math::vec3 l_dir = math::normalize(area->pos(a_epsilon, b_epsilon) - ray.o());
				color_samples += point_shading(l_dir, hit, ray, area);
			}
			color += color_samples / (SAMPLE_SIZE * SAMPLE_SIZE);
		}
	}
	return color;
}

/* Tracing method. Returns color of the pixel */
math::vec3 trace(const ray &ray, int depth, float ref_index, std::pair<float, float> light_sample) {
	hit hit = sce.calculate_hit(ray);
	if (!hit.collided()) return sce.b_color();
	else {
		//calculating local color using the light sample
		math::vec3 color = local_color(ray, hit, light_sample);

		if (depth >= MAX_DEPTH) return color;

		// if the object is reflective
		if (hit.mat().ks() > 0) {

			math::vec3 normal = hit.normal();
			float cosi = math::dot(-ray.d(), normal);

			if (cosi < 0) { // Inside of surface
				normal = -normal;
			}

			math::vec3 dir = ray.d() - normal * math::dot(ray.d(), normal) * 2.0f;
			p3d::ray refl_ray = p3d::ray(hit.point(), dir);
			refl_ray.offset(math::KEPSILON);
			math::vec3 refl_color = trace(refl_ray, depth + 1, ref_index, light_sample);
			color += refl_color * hit.mat().ks();
		}

		// if the object is translucent
		if (hit.mat().t() > 0) {
			math::vec3 normal = hit.normal();
			float cosi = math::dot(-ray.d(), normal);
			float ior;
			if (cosi > 0) // Outside of surface
				ior = ref_index / hit.mat().ref_index();
			else {  // inside of surface
				ior = hit.mat().ref_index() / ref_index;
				normal = -normal;
			}

			// refracted ray calculations
			math::vec3 vt = normal * math::dot(-ray.d(), normal) + ray.d();
			float sini = vt.magnitude();
			float sint = sini * ior;

			if ((sint * sint) <= 1) {
				float cost = std::sqrt(1 - sint * sint);
				math::vec3 dir = math::normalize(vt) * sint - normal * cost;
				p3d::ray refr_ray = p3d::ray(hit.point(), dir);
				refr_ray.offset(math::KEPSILON);
				math::vec3 refr_color = trace(refr_ray, depth + 1, ref_index, light_sample);
				color += refr_color * hit.mat().t();
			}
		}
		//printf("%f\n", hit.distance());
		return color;
	}
}

math::vec3 jitter(int x, int y, int size) {
	math::vec3 color;
	std::vector<std::pair<float, float>> pixel_samples(size*size);
	std::vector<std::pair<float, float>> light_samples(size*size);

	for (int i = 0; i < size*size; i++) {
		//division of pixels (and light sources) in size*size
		int p = i / size;
		int q = i % size;

		//defining random points in each pixel sub-division
		pixel_samples[i].first = (((float)std::rand() / (float)RAND_MAX) + p) / size;
		pixel_samples[i].second = (((float)std::rand() / (float)RAND_MAX) + q) / size;

		//defining random points in each light source sub-division
		light_samples[i].first = (((float)std::rand() / (float)RAND_MAX) + p) / size;
		light_samples[i].second = (((float)std::rand() / (float)RAND_MAX) + q) / size;
	}

	//shuffling the light samples
	std::shuffle(light_samples.begin(), light_samples.end(), std::default_random_engine());

	//calculation and tracing of the sample primary rays
	for (int i = 0; i < size*size; i++) {
		ray ray = sce.cam().primaryRay(x + pixel_samples[i].first, y + pixel_samples[i].second);
		color += trace(ray, 1, 1.0, light_samples[i]); //casts ray and sends light sample
	}

	return color / (size * size); //returns average color by # of samples
}

math::vec3 dof(int x, int y, int size) { // with Antialising
	/*
	To simulate DOF:
	Compute the point p where the center ray hits the focal plane;
	Use p and the sample point on the lens to compute the
	direction of the primary ray so that this ray also goes through p;
	Ray-trace the primary ray into the scene; the center ray does
	not contribute to the pixel color
	*/

	ray rayDirPixelFromEye = sce.cam().primaryRay(x, y); // para ficar com o raio até ao pixel

	float width = sce.cam().width(),
		height = sce.cam().height(),
		resX = sce.cam().resX(),
		resY = sce.cam().resY();

	float pixel_x = width * (x / resX - 0.5f); //x component on the pixel
	float pixel_y = height * (y / resY - 0.5f); //y component on the pixel


	//POINT P	// single point camera
	math::vec3 df = sce.cam().eye() - sce.cam().at();

	float focaldistance = df.magnitude(); //FOCAL_PLANE_DISTANCE; //REMEMBER TO CHANGE THIS TO FOCAL_PLANE_DISTANCE

	math::vec3 directionToP(-sce.cam().df() * sce.cam().ze()
		+ pixel_x * sce.cam().xe()
		+ pixel_y * sce.cam().ye());

	float distanceToP = (focaldistance * directionToP.magnitude()) / sce.cam().df(); // distance from eye to point P

	//printf("DISTANCE TO P %f\n",distanceToP);
	math::vec3 pointP = distanceToP * directionToP;


	//LENS
	// � uma area circular de centro eye da camara e raio APERTURE

	math::vec3 color;
	std::vector<std::pair<float, float>> lens_samples(size*size);

	for (int i = 0; i < size*size; i++) {

		bool inCircle = false;
		//defining random points in each lens sub-division
		float randX;
		float randY;

		while (!inCircle) {
			randX = (((float)std::rand() / (float)RAND_MAX) * 2) - 1;
			randY = (((float)std::rand() / (float)RAND_MAX) * 2) - 1;
			if (std::sqrtf(std::pow(randX, 2) + std::pow(randY, 2)) <= 1) {
				inCircle = true;
			}
		}

		lens_samples[i].first = randX * APERTURE;
		lens_samples[i].second = randY * APERTURE;
	}

	//shuffling lens samples
	std::shuffle(lens_samples.begin(), lens_samples.end(), std::default_random_engine());

	//calculation and tracing of the sample primary rays
	for (int i = 0; i < size*size; i++) {

		math::vec3 lensPoint = sce.cam().eye() +
			(sce.cam().xe() * lens_samples[i].first) +
			(sce.cam().ye() * lens_samples[i].second);

		ray ray(lensPoint, pointP - lensPoint);
		color += trace(ray, 1, 1.0, lens_samples[i]);
	}

	return color / (size * size); //returns average color by # of samples
}

// TEST to both jitter and dof
math::vec3 jitterDof(int x, int y, int size) {		// its a bit redundant as we already have jitter/aliasing and dof seperatly
	math::vec3 color;
	std::vector<std::pair<float, float>> pixel_samples(size*size);
	std::vector<std::pair<float, float>> light_samples(size*size);
	std::vector<std::pair<float, float>> lens_samples(size*size);
	std::vector<math::vec3> focal_point_samples(size*size);

	ray rayDirPixelFromEye = sce.cam().primaryRay(x, y); // para ficar com o raio até ao pixel

	float width = sce.cam().width(),
		height = sce.cam().height(),
		resX = sce.cam().resX(),
		resY = sce.cam().resY();

	float pixel_x = width * (x / resX - 0.5f); //x component on the pixel
	float pixel_y = height * (y / resY - 0.5f); //y component on the pixel


	for (int i = 0; i < size*size; i++) {
		//division of pixels (and light sources) in size*size
		int p = i / size;
		int q = i % size;

		//defining random points in each pixel sub-division
		pixel_samples[i].first = (((float)std::rand() / (float)RAND_MAX) + p) / size;
		pixel_samples[i].second = (((float)std::rand() / (float)RAND_MAX) + q) / size;

		//defining random points in each light source sub-division
		light_samples[i].first = (((float)std::rand() / (float)RAND_MAX) + p) / size;
		light_samples[i].second = (((float)std::rand() / (float)RAND_MAX) + q) / size;

		//LENS - its not jittered yet
		bool inCircle = false;
		//defining random points in each lens sub-division
		float randX;
		float randY;
		while (!inCircle) {
			randX = (((float)std::rand() / (float)RAND_MAX) * 2) - 1;
			randY = (((float)std::rand() / (float)RAND_MAX) * 2) - 1;
			if (std::sqrtf(std::pow(randX, 2) + std::pow(randY, 2)) <= 1) {
				inCircle = true;
			}
		}
		lens_samples[i].first = randX * APERTURE;
		lens_samples[i].second = randY * APERTURE;
	}

	//POINTs P should have as many as size * size
	math::vec3 df = sce.cam().eye() - sce.cam().at();
	float focaldistance = df.magnitude(); //FOCAL_PLANE_DISTANCE;

	for (int i = 0; i < size*size; i++) {
		math::vec3 directionToP(-sce.cam().df() * sce.cam().ze()
			+ (pixel_x + pixel_samples[i].first)  * sce.cam().xe()
			+ (pixel_y + pixel_samples[i].second) * sce.cam().ye());
		float distanceToP = (focaldistance * directionToP.magnitude()) / sce.cam().df(); // distance from eye to point P
		focal_point_samples[i]  = distanceToP * directionToP;
	}

	//shuffling the light, lens and focal samples
	std::shuffle(light_samples.begin(), light_samples.end(), std::default_random_engine());
	//std::shuffle(lens_samples.begin(), lens_samples.end(), std::default_random_engine());
	std::shuffle(focal_point_samples.begin(), focal_point_samples.end(), std::default_random_engine());

	//calculation and tracing of the sample primary rays
	for (int i = 0; i < size*size; i++) {

		math::vec3 lensPoint = sce.cam().eye() +
			(sce.cam().xe() * lens_samples[i].first) +
			(sce.cam().ye() * lens_samples[i].second);

		ray ray(lensPoint, focal_point_samples[i] - lensPoint);
		color += trace(ray, 1, 1.0, light_samples[i]);
	}

	return color / (size * size); //returns average color by # of samples
}

// Draw function by primary ray casting from the eye towards the scene's objects
void drawScene() {

	std::srand(std::time(0) * std::time(0));
	for (int y = 0; y < RES_Y; y++) {
		for (int x = 0; x < RES_X; x++) {
			math::vec3 color;
			if (JITTERING && DOF) { //with jitter and dof
				color = jitterDof(x, y, SAMPLE_SIZE);
			}
			else {
				if (JITTERING) {	//with only jitter
					color = jitter(x, y, SAMPLE_SIZE);
				}
				else {
					if (DOF) {	//with only dof
						color = dof(x, y, SAMPLE_SIZE);
					}
					else {	// base case without jitter or dof
						ray ray = sce.cam().primaryRay(x + 0.5f, y + 0.5f); //for each pixel, cast a primary ray
						color = trace(ray, 1, 1.0, std::pair<float, float>(0.0f, 0.0f)); //depth=1, ior=1.0
					}
				}
			}

			glBegin(GL_POINTS);
			glColor3f(color.x(), color.y(), color.z());
			glVertex2f(x, y);

			glEnd();
			glFlush();
		}
	}

	printf("Terminou!\n");
}

int main(int argc, char**argv) {

	if (!(sce.load_nff("scenes/balls_low.nff")))
		return 0;

	if (GRID)
		sce.setup_grid();

	RES_X = sce.cam().resX();
	RES_Y = sce.cam().resY();
	printf("resx = %d  resy= %d.\n", RES_X, RES_Y);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(RES_X, RES_Y);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("JAP Ray Tracing");
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glutReshapeFunc(reshape);
	glutDisplayFunc(drawScene);
	glDisable(GL_DEPTH_TEST);

	glutMainLoop();
	return 0;
}
