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
#include "math/vec3.hpp"

#define MAX_DEPTH 3
#define JITTERING false
#define SAMPLE_SIZE 2

#define DOF false
#define FOCAL_PLANE 1
#define APERTURE 1

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

/* Intersection of a ray with the closest object */
hit calculate_hit(const ray &ray) {
	hit closest;
	for (auto obj : sce.objs()) {
		hit hit = obj->calculate_intersection(ray);
		if (hit.collided() && hit.distance() < closest.distance())
			closest = hit;
	}
	return closest;
}

/* Shadow Feeler calculations; returns a bool according to collision */
bool point_in_shadow(const math::vec3 &hit_pos, const math::vec3 &l_dir) {
	ray ray(hit_pos, l_dir);
	ray.offset(math::KEPSILON);
	hit shadow_feeler = calculate_hit(ray);
	return shadow_feeler.collided();
}

/* Tracing method. Returns color of the pixel */
math::vec3 trace(const ray &ray, int depth, float ref_index) {
	hit hit = calculate_hit(ray);
	if (!hit.collided()) return sce.b_color();
	else {
		math::vec3 color(0.0f, 0.0f, 0.0f);
		for (auto* l : sce.lights()) {  //for each light source
			math::vec3 l_dir = math::normalize(l->pos() - hit.point());
			float lambert = math::dot(l_dir, hit.normal());
			if (lambert > 0.0f)
				if (!point_in_shadow(hit.point(), l_dir)) {  //cast a shadow feeler
					color += hit.mat().color() * l->color() * lambert * hit.mat().kd();
					math::vec3 half_vec = math::normalize(-ray.d() + l_dir);
					color += l->color() * hit.mat().ks() * std::pow(math::dot(hit.normal(), half_vec), hit.mat().shine());
				}
		}

		if (depth >= MAX_DEPTH) return color;

		// if the object is reflective
		if (hit.mat().ks() > 0) {

			math::vec3 normal = hit.normal();
			float cosi = math::dot(-ray.d(), normal);

			if (cosi < 0){ // Inside of surface
				normal = -normal;
			}

			math::vec3 dir = ray.d() - normal * math::dot(ray.d(), normal) * 2.0f;
			p3d::ray refl_ray = p3d::ray(hit.point(), dir);
			refl_ray.offset(math::KEPSILON);
			math::vec3 refl_color = trace(refl_ray, depth + 1, ref_index);
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
				math::vec3 refr_color = trace(refr_ray, depth + 1, ref_index);
				color += refr_color * hit.mat().t();
			}
		}

		return color;
	}
}

math::vec3 jitter(int x, int y, int size) {
  math::vec3 color;
  std::vector<std::pair<float,float>> pixel_samples(size*size);
	std::vector<std::pair<float,float>> light_samples(size*size);

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
    ray ray = sce.cam().primaryRay(x+pixel_samples[i].first, y+pixel_samples[i].second);
    color += trace(ray, 1, 1.0);
  }

  return color / (size * size);
}

math::vec3 dof(int x, int y, int size) {
	/*
	To simulate DOF:
– Compute the point p where the center ray hits the focal plane;
– Use p and the sample point on the lens to compute the
direction of the primary ray so that this ray also goes through p;
– Ray-trace the primary ray into the scene; the center ray does
not contribute to the pixel color
	*/

	math::vec3 color;
	std::vector<std::pair<float, float>> rays(size*size);

	for (int i = 0; i < size*size; i++) {
		rays[i].first = (float)std::rand() * 2.0f / (float)RAND_MAX - 1 ;
		rays[i].second = (float)std::rand()* 2.0f / (float)RAND_MAX - 1 ;
	
		if (rays[i].first * rays[i].first + rays[i].second * rays[i].second <= 1) { // se os parametros aleatorios estiverem dentro do circulo de circunferencia 1
		}
	}
	//See if the points chosen are inside a circle??



	for (int i = 0; i < size * size; i++) {
		int p = i / size;
		int q = i % size;
		ray ray = sce.cam().primaryRay(x + (rays[i].first + p) / size, y + (rays[i].second + q) / size);
		color += trace(ray, 1, 1.0);
	}

	return color / (size * size);
	math::vec3 focal_plane_o = sce.cam().eye() + sce.cam().at() * FOCAL_PLANE;
	math::vec3 focal_plane_d = sce.cam().at();


	return color;
}

// Draw function by primary ray casting from the eye towards the scene's objects
void drawScene() {

  std::srand(std::time(0) * std::time(0));
    for (int y = 0; y < RES_Y; y++) {
		for (int x = 0; x < RES_X; x++) {
          math::vec3 color;
          if (JITTERING) {
            color = jitter(x,y,SAMPLE_SIZE);
          } else {
			ray ray = sce.cam().primaryRay(x + 0.5f, y + 0.5f); //for each pixel, cast a primary ray
            color = trace(ray, 1, 1.0); //depth=1, ior=1.0
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

	if (!(sce.load_nff("scenes/mount_low.nff")))
		return 0;


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
