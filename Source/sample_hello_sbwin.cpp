#include "sbwin.h"

void
update_audio(sb::Audio & audio)
{
	//nothing to do.
}

void
update_frame(sb::Image &img, sb::Keys & keys)
{
	static int frame_count = 0;
	printf("frame_count=%d\n", frame_count);
	for (int y = 0 ; y < img.GetHeight(); y++) {
		for (int x = 0 ;  x < img.GetWidth(); x++) {
			img.SetPixel(x, y, rand());
		}
	}
	frame_count++;
}

int
main(int argc, char *argv[])
{
	sb::run_app(argv[0], 160, 120, 1280, 720,
		update_frame,
		update_audio);
	return 0;
}
