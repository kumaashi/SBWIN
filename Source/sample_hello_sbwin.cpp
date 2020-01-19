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
