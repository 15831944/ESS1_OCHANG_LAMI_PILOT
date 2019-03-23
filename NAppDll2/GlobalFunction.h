//0 과 1이 동시에 오는 경우 ------- 자동으로 0이 먼저 수행된다. 다음에는 nStart=1이므로 1이 수행된다.
// Template를 Rotate하는 함수 (160msec)
int MakeRotatedTemp(LPBYTE fmS, int left_S, int top_S, int right_S, int bottom_S, 
					LPBYTE fmD, int left_D, int top_D, int right_D, int bottom_D, 
					int nPitch, BYTE color, double Cx, double Cy, double angle);
