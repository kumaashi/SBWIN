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

void update_audio(float *buf, int count, int ch) {
	static double phase = 0.0;
	printf("process buffer count=%d, ch=%d\n", count, ch);
	for(int i = 0 ; i < count; i += ch) {
		buf[i + 0] = sin(2.3 * sin(phase)) * 0.1;
		buf[i + 1] = buf[i + 0];
		phase += 0.1;
	}
}

void update_frame(uint32_t *buf, int w, int h) {
	static int frame_count = 0;
	printf("frame_count=%d, w=%d, h=%d\n", frame_count, w, h);
	for(int y = 0 ;  y < h; y++) {
		for(int x = 0 ;  x < w; x++) {
			buf[x + y * w] = rand();
		}
	}
	frame_count++;
}

int main(int argc, char *argv[]) {
	sb::run_app(argv[0], 160, 120, 640, 480,
		update_frame,
		update_audio);
	return 0;
}