#ifndef CHESSBOARD_DETECTION_H
#define CHESSBOARD_DETECTION_H

#include <opencv2/opencv.hpp>

#define DRAW_DEBUG_WINDOWS		1
#if DRAW_DEBUG_WINDOWS
#define DRAW_DEBUG_WINDOWS_EXT	1
#endif

#define SMALL_RESOLUTION		0
#if SMALL_RESOLUTION
// netbook - wolno chodzi, nie b�dziemy spowalnia�
#define DEFINED_FPS				40
#else
#define DEFINED_FPS				10
#endif

#define HUGE_CHESSBOARD			1

#define MOVE_HISTORY_SIZE		3
typedef cv::Vec3i Scalar3i;


extern Scalar3i markerLower;
extern Scalar3i markerUpper;

extern int calibrationOn;
extern int drawWindowsLevel;
extern int drawResult;
extern int globalContrast;
extern int autoContrast;
extern int figuresContrast;
extern int whiteFigureThreshold;

extern const char* OUT_WINDOW_NAME;
extern const char* HAND_WND_NAME;
extern const char* TRACKBARS_WND_NAME;
extern const char* FIGURES_FINDING_WND_NAME;
extern const char* FIGURES_FINDING_CANNY_WND_NAME;

void onDrawWindows(int, void*);
void onContrastChange(int, void*);
void onWhiteThreshChange(int, void*);

/** Wynik rozpoznania stanu szachownicy. */
enum DetectionState {
	UNCHANGED,		/// nic si� nie zmieni�o
	MOVE,			/// wykonano ruch (pobierz przez getLastMove())
	INCORRECT,		/// niepoprawny ruch (nale�y poinformowa� gracza o konieczno�ci cofni�cia figur do poprzedniego po�o�enia)
	IMPOSSIBLE		/// ustalenie stanu jest niemo�liwe (np. nie uda�o si� znale�� szachownicy na obrazie)
};


#endif // CHESSBOARD_DETECTION_H
