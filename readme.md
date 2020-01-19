# SBWIN

## About

SBWIN is a framework for Windows that software graphic processing and audio processing with a single header.

SBWIN scope to principle of graphics rendering and audio signal processing education for the me.

In addition, SBWIN does not have any graphics rendering functions.

It is designed to be written by yourself. The same applies to audio processing.

Also, Support basic input function for the windows OS.

## How to make 

1) Install VS2017

2) Run VsDevCmd.bat x64

3) Apply cd Source directory.

4) Run make.bat


## Sample program LICENSE

MIT License
https://opensource.org/licenses/mit-license.php

## Sample Images and code

![Image 000](/Images/sample_hello_sbwin.png)

```C++

#include "sbwin.h"

void update_audio(float *buf, int count, int ch)
{
	//nothing to do.
}

void update_frame(uint32_t *buf, int w, int h)
{
	static int frame_count = 0;
	printf("frame_count=%d, w=%d, h=%d\n", frame_count, w, h);
	for(int y = 0 ;  y < h; y++) {
		for(int x = 0 ;  x < w; x++) {
			buf[x + y * w] = rand();
		}
	}
	frame_count++;
}

int main(int argc, char *argv[])
{
	sb::run_app(argv[0], 160, 120, 640, 480,
		update_frame,
		update_audio);
	return 0;
}

```


## Author

gyabo(aka yasai kumaashi)

