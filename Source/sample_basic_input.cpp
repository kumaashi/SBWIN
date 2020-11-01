#include "sbwin.h"

void
update_audio(sb::Audio & audio)
{
	//Nothing to do.
}

void
update_frame(sb::Image &img, sb::Keys & keys)
{
	static int frame_count = 0;
	auto kdata = keys;

	if ( kdata.left )
		printf("left\n");
	if ( kdata.right )
		printf("right\n");
	if ( kdata.up )
		printf("up\n");
	if ( kdata.down )
		printf("down\n");
	if ( kdata.button0 )
		printf("button0\n");
	if ( kdata.button1 )
		printf("button1\n");
	if ( kdata.button2 )
		printf("button2\n");
	if ( kdata.button3 )
		printf("button3\n");
	printf("kdata.raw=0x%08X\n", kdata.raw);

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
