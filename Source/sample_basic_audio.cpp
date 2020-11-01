#include "sbwin.h"

void
update_audio(sb::Audio & audio)
{
	static double phase = 0.0;
	auto buf = audio.buf;
	auto count = audio.count;
	auto ch = audio.ch;
	printf("process buffer count=%d, ch=%d\n", count, ch);
	for (int i = 0 ; i < count; i += ch) {
		buf[i + 0] = sin(2.3 * sin(phase)) * 0.1;
		buf[i + 1] = buf[i + 0];
		phase += 0.1;
	}
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
	sb::run_app(argv[0], 160, 120, 640, 480,
		update_frame,
		update_audio);
	return 0;
}
