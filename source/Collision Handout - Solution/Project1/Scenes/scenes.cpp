#include "Scenes/scenes.h"

Scene current_scene = INTRO;
bool Scene_Init = false;

void ChangeScene() {
	Scene_Init = false;
	if (current_scene == INTRO) {
		current_scene = TITLE;
		return;
	}
	if (current_scene == TITLE) {
		current_scene = LEVEL1;
		return;
	}
	if (current_scene == LEVEL1) {
		current_scene = LEVEL2;
		return;
	}
	if (current_scene == LEVEL2) {
		current_scene = LEVEL1;
		return;
	}
}

Scene GetCurrentScene() {
	if (current_scene == INTRO) {
		return INTRO;
	}
	if (current_scene == TITLE) {
		return TITLE;
	}
	if (current_scene == LEVEL1) {
		return LEVEL1;
	}
	if (current_scene == LEVEL2) {
		return LEVEL2;
	}
}