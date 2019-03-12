#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>
#include <stdio.h>

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

hit *calculate_hit(ray &ray) {
	return nullptr;
}

bool point_in_shadow() {
	return false;
}

math::vec3 trace(ray &ray, int depth, float ref_index) {
	hit *hit = calculate_hit(ray);
	if (!hit->collided()) return *sce.b_color();
	else {
		math::vec3 color = *hit->mat()->color();
		for (light* l : *sce.lights()) {
			math::vec3 l_dir = math::normalize(*l->pos() - *hit->point());
			if (math::dot(l_dir, *hit->normal()) > 0.0f)
				if (!point_in_shadow())
					color += math::vec3(0.0f, 0.0f, 0.0f);
		}

		if (depth >= MAX_DEPTH) return color;
	}
}

// Draw function by primary ray casting from the eye towards the scene's objects
void drawScene() {

	for (int y = 0; y < RES_Y; y++) {
		for (int x = 0; x < RES_X; x++) {

			ray ray;
			//Ray ray = scene->GetCamera()->PrimaryRay(x, y);
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

	RES_X = 256; //scene->GetCamera()->GetResX();
	RES_Y = 256; // scene->GetCamera()->GetResY();
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
