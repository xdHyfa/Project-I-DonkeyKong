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
		current_scene = CUTSCENE;;
		return;
	}
	if (current_scene == CUTSCENE) {
		current_scene = HOWHIGH;
		return;
	}
	if (current_scene == HOWHIGH) {
		current_scene = LEVEL1;
		return;
	
	}
	if (current_scene == LEVEL1) {
		current_scene = WINCUTSCENE;
		return;
	}
	if (current_scene == WINCUTSCENE) {
		current_scene = HOWHIGH2;
		return;
	}
	if (current_scene == HOWHIGH2) {
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
	if (current_scene == CUTSCENE) {
		return CUTSCENE;
	}
	if (current_scene == HOWHIGH) { 
		return HOWHIGH; 
	}
	if (current_scene == LEVEL1) {
		return LEVEL1;
	}
	if (current_scene == WINCUTSCENE) {
		return WINCUTSCENE;
	}
	if (current_scene == HOWHIGH2) {
		return HOWHIGH2;
	}
	if (current_scene == LEVEL2) {
		return LEVEL2;
	}
}