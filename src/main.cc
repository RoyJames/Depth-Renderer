#include "main.h"

int main(int argc, char **argv) {
	GLOptions options(argc, argv);
	GLUI ui(options);
	GLScene scene(options);
	GLController kb(options);

	scene.loadFromFile();
	ui.setModel(&scene);
	ui.setController(&kb);

	kb.setModel(&scene);
	ui.startUI();

	return 0;
}