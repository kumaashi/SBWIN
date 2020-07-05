/*
 *
 * Copyright (c) 2020 gyabo <gyaboyan@gmail.com>
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include "sbwin.h"

void
update_audio(float *buf, int count, int ch)
{
	//Nothing to do.
}

void
update_frame(uint32_t *buf, int w, int h)
{
	static int frame_count = 0;
	auto kdata = sb::get_keydata();

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
