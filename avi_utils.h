#ifndef _avi_utils_H
#define _avi_utils_H

// AVI utilities -- for creating avi files
// (c) 2002 Lucian Wischik. No restrictions on use.


DECLARE_HANDLE(HAVI);
HAVI CreateAvi(const char *fn, int frameperiod, const WAVEFORMATEX *wfx);
HRESULT AddAviFrame(HAVI avi, HBITMAP hbm);
HRESULT AddAviAudio(HAVI avi, void *dat, unsigned long numbytes);
HRESULT AddAviWav(HAVI avi, const char *wav, DWORD flags);
HRESULT SetAviVideoCompression(HAVI avi, HBITMAP hbm, AVICOMPRESSOPTIONS *opts, bool ShowDialog, HWND hparent);
HRESULT CloseAvi(HAVI avi);
unsigned int FormatAviMessage(HRESULT code, char *buf,unsigned int len);

#endif


 