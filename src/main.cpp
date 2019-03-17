#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>
#include <stdio.h>
#include <cmath>

#include "hit.hpp"
#include "ray.hpp"
#include "scene.hpp"
#include "math/vec3.hpp"

#define MAX_DEPTH 6

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

hit calculate_hit(const ray &ray) {
	hit closest;
	for (auto obj : sce.objs()) {
		hit hit = obj->calculate_intersection(ray);
		if (hit.collided() && hit.distance() < closest.distance())
			closest = hit;
	}
	return closest;
}

bool point_in_shadow(const math::vec3 &hit_pos, const math::vec3 &l_dir) {
	ray ray(hit_pos, l_dir);
	hit shadow_feeler = calculate_hit(ray);
	return shadow_feeler.collided();
}

math::vec3 trace(const ray &ray, int depth, float ref_index) {
	hit hit = calculate_hit(ray);
	if (!hit.collided()) return sce.b_color();
	else {
 		math::vec3 color(0.0f, 0.0f, 0.0f);
		for (light* l : sce.lights()) {
			math::vec3 l_dir = math::normalize(l->pos() - hit.point());
			float lambert = math::dot(l_dir, hit.normal());
			if (lambert > 0.0f)
				if (!point_in_shadow(hit.point(), l_dir)) {
					color += hit.mat().color() * l->color() * lambert * hit.mat().kd();
					math::vec3 half_vec = math::normalize(-ray.d() + l_dir);
					color += l->color() * hit.mat().ks() * std::pow(math::dot(hit.normal(), half_vec), hit.mat().shine());
				}
		}

		if (depth >= MAX_DEPTH) return color;

        if (hit.mat().ks() > 0) {
            math::vec3 dir = ray.d() - hit.normal() * math::dot(ray.d(),hit.normal()) * 2.0f;
            p3d::ray ref_ray = p3d::ray(hit.point(), dir);
            math::vec3 ref_color = trace(ref_ray, depth + 1, ref_index);
            color += ref_color * hit.mat().ks();
        }

		return color;
	}
}

// Draw function by primary ray casting from the eye towards the scene's objects
void drawScene() {

	for (int y = 0; y < RES_Y; y++) {
		for (int x = 0; x < RES_X; x++) {

			ray ray = sce.cam().primaryRay(x, y);
			math::vec3 color = trace(ray, 1, 1.0); //depth=1, ior=1.0
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

	if (!(sce.load_nff("jap.nff")))
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
