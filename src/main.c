#include <stdio.h>
#include <SDL2/SDL.h>

#include <vo/ver.h>

int main() {
	printf("Virtual Orchestra %d.%d.%d\n", VO_VER_MAJOR, VO_VER_MINOR, VO_VER_PATCH);

	return 0;
}
