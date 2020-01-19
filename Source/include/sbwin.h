#pragma once

#include <stdint.h>
#include <vector>
#include <thread>
#include <vector>
#include <windows.h>
#include <dwmapi.h>
#include <mmsystem.h>
#include <mmreg.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dwmapi.lib")

namespace sb
{

union key_data {
	struct {
		unsigned left : 1;
		unsigned right : 1;
		unsigned up : 1;
		unsigned down : 1;
		unsigned button0 : 1;
		unsigned button1 : 1;
		unsigned button2 : 1;
		unsigned button3 : 1;
		unsigned mouse_x : 12;
		unsigned mouse_y : 12;
	};
	uint32_t raw;
};
static key_data g_kdata;

static LRESULT WINAPI
msg_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	auto param = wParam & 0xFFF0;
	switch (msg) {
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SYSCOMMAND:
		if (param == SC_MONITORPOWER || param == SC_SCREENSAVE)
			return 0;
		break;
	case WM_IME_SETCONTEXT:
		lParam &= ~ISC_SHOWUIALL;
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

key_data get_keydata()
{
	return g_kdata;
}


inline void run_app(const char *name, int w, int h, int sw, int sh,
	void (*update_frame)(uint32_t *buf, int w, int h),
	void (*update_audio)(float *buf, int count, int ch))
{
	auto instance = GetModuleHandle(NULL);
	auto style = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME;
	auto ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	RECT rc = {0, 0, sw, sh};
	WNDCLASSEX twc = {
		sizeof(WNDCLASSEX), CS_CLASSDC, msg_proc, 0L, 0L, instance,
		LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)GetStockObject(BLACK_BRUSH), NULL, name, NULL
	};
	BITMAPINFOHEADER bih = {
		sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB, 0, 0, 0, 0, 0
	};
	BITMAPINFO bi = { bih };

	RegisterClassEx(&twc);
	AdjustWindowRectEx(&rc, style, FALSE, ex_style);
	rc.right -= rc.left;
	rc.bottom -= rc.top;
	auto hwnd = CreateWindowEx(ex_style, name, name, style,
			(GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2,
			(GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2,
			//0, 0,
			rc.right, rc.bottom, NULL, NULL, instance, NULL);
	ShowWindow(hwnd, SW_SHOW);
	SetFocus(hwnd);
	auto hdc = GetDC(hwnd);
	void *pBits = nullptr;
	auto hDIB = CreateDIBSection(
			NULL, &bi, DIB_RGB_COLORS, (void **)&pBits, 0, 0);
	auto hDIBDC = CreateCompatibleDC(NULL);
	SaveDC(hDIBDC);
	SelectObject(hDIBDC, hDIB);
	bool isActive = true;

	auto audio = std::thread([&] {
		const auto BufNum = 6;
		const DWORD Samples = 512;
		DWORD count  = 0;
		HWAVEOUT hwo = NULL;
		HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		WAVEFORMATEX wfx = {
			WAVE_FORMAT_IEEE_FLOAT, 2, 44100 / 2, 0, 0, 32, 0
		};

		wfx.nBlockAlign = (wfx.nChannels * wfx.wBitsPerSample) / 8;
		wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
		std::vector<WAVEHDR> whdr(BufNum);
		std::vector< std::vector<char> > soundbuffer(BufNum);

		waveOutOpen(&hwo, WAVE_MAPPER, &wfx, (DWORD_PTR)hEvent, 0,
			CALLBACK_EVENT);
		for (int i = 0 ; i < BufNum; i++)
		{
			soundbuffer[i].resize(Samples * wfx.nBlockAlign);
			WAVEHDR temp = {
				&soundbuffer[i][0], Samples * wfx.nBlockAlign,
				0, 0, 0, 0, NULL, 0
			};
			whdr[i] = temp;
			waveOutPrepareHeader(hwo, &whdr[i], sizeof(WAVEHDR));
			waveOutWrite(hwo, &whdr[i], sizeof(WAVEHDR));
		}

		for (uint64_t count = 0; isActive; )
		{
			auto idx = count % BufNum;
			auto & ref = whdr[idx];
			if (WaitForSingleObject(hEvent, 100) == WAIT_TIMEOUT)
				continue;
			if (ref.dwFlags & WHDR_DONE) {
				update_audio(
					(float *)ref.lpData,
					ref.dwBufferLength / sizeof(float), 2);
				waveOutWrite(hwo, &whdr[idx], sizeof(WAVEHDR));
				count++;
			}
		}

		do
		{
			count = 0;
			for (int i = 0; i < BufNum; i++)
				count += (whdr[i].dwFlags & WHDR_DONE) ? 0 : 1;
			if (count) Sleep(50);
		} while (count);

		for (int i = 0 ; i < BufNum ; i++)
			waveOutUnprepareHeader(hwo, &whdr[i], sizeof(WAVEHDR));

		waveOutReset(hwo);
		waveOutClose(hwo);
		CloseHandle(hEvent);
	});

	MSG msg;
	for (uint64_t frame = 0 ; isActive; frame++) {
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				isActive = false;
				break;
			} else {
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}

		g_kdata.raw = 0;
		SHORT assign[] = {
			VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN, 'Z', 'X', 'C', 'V',
		};
		for (int i = 0 ; i < sizeof(assign) / sizeof(assign[0]); i++) {
			if (GetAsyncKeyState(assign[i]) & 0x8000)
				g_kdata.raw |= (1 << i);
		}
		POINT pt = {};
		POINT ptm = {};
		GetCursorPos(&pt);
		ScreenToClient(hwnd, &ptm);
		ptm.x += pt.x;
		ptm.y += pt.y;
		g_kdata.mouse_x = ptm.x;
		g_kdata.mouse_y = ptm.y;

		update_frame((uint32_t *)pBits, w, h);
		if (GetAsyncKeyState(VK_F12) & 0x8000) {
		} else {
			DwmFlush();
		}
		if (sw == w && sh == h) {
			BitBlt(
				hdc, 0, 0, w, h,
				hDIBDC, 0, 0, SRCCOPY);
		} else {
			StretchBlt(
				hdc, 0, 0, sw, sh,
				hDIBDC, 0, 0, w, h, SRCCOPY);
		}
	}
	audio.join();
}

} //namespace sb
