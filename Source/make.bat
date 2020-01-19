SET OPTIONS=/Iinclude /Ox  /EHsc /GS- /nologo

	cl  sample_hello_sbwin.cpp %OPTIONS%
	cl  sample_basic_audio.cpp %OPTIONS%
	cl  sample_basic_input.cpp %OPTIONS%
