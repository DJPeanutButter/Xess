#include <iostream>
#include <vector>
#include <windows.h>
#include <gl/gl.h>
#include <math.h>

#define	_GLOBAL_INIT_

#define _HOME_INIT_
#define _AWAY_INIT_
#define	_WINDOW_INIT

//MATH
#define PI	3.1415926

//GAME DATA
#define GD_ROWS	18
#define GD_COLS	12

//PIECE TYPES
#define PT_CIRCLE	0
#define PT_FAN		1
#define	PT_SELECT	2
#define PT_OPTION	3
#define PT_KNIGHT	4
#define PT_KING		5
#define PT_BISHOP	6
#define PT_ROOK		7
#define PT_QUEEN	8

struct Color{
	float r, g, b;
	
	Color ();
	Color (float r, float g, float b);
};

struct Piece{
	bool	alive;
	char	row, col, type, dir;
	Color	color;
	void	draw ();
	
	Piece ();
	Piece (Color clr);
	Piece (Color clr, char t);
	Piece (Color clr, char r, char c);
	Piece (Color clr, char r, char c, char t);
	Piece (char t);
	Piece (char r, char c);
	Piece (char r, char c, char t);
};

LRESULT CALLBACK	WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void				EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC);
void				DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC);

Color				COLOR	(float r, float g, float b);
Color				INVERT	(Color &c);

void inline			SetColor		(Color c);
void 				DrawBackground	(Color c1, Color c2);
void 				DrawBackground	(float xMin, float xMax, float yMin, float yMax, Color c1, Color c2);

Piece*				SelectPiece		(char r, char c, Piece* arrPieces, int lPieces);
Piece*				SelectPiece		(char r, char c, std::vector<Piece> &p);
void				MovePiece		(Piece &p, char r, char c);
std::vector<Piece>	FindMoves		(Piece &p);
std::vector<Piece>	FindMoves		(Piece &p, std::vector<Piece> pieces);
std::vector<Piece>	FindMoves		(Piece &p, std::vector<Piece> friendly, std::vector<Piece> hostile);
 
int					FindTarget		(Piece &p, std::vector<Piece> v);


int WINAPI WinMain (HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int iCmdShow){
	
#ifdef	_GLOBAL_INIT_

	WNDCLASS			wc;
    HWND				hWnd;
    HDC 				hDC;
    HGLRC				hRC;
    MSG					msg;
    BOOL				bQuit	= false;
	
	bool				homeTurn = true;
	
	Color				bg1 (0.8f, 0.5f, 0.5f), bg2(0.2f, 0.5f, 0.5f);
	Piece				sel (COLOR(1.0f, 1.0f, 0.0f), 6, 1, PT_SELECT);
	
	std::vector<Piece>	home (GD_COLS);
	std::vector<Piece>	away (GD_COLS);
	std::vector<Piece>	moves;
	
	Piece*				selPtr = NULL;
	Piece*				tarPtr = NULL;
	
	Piece
		*hRookLeft,
		*hKnightLeft,
		*hBishopLeft,
		*hQueenLeft,
		*hQueenRight,
		*hBishopRight,
		*hKnightRight,
		*hRookRight,
		
		*aRookLeft,
		*aKnightLeft,
		*aBishopLeft,
		*aQueenLeft,
		*aQueenRight,
		*aBishopRight,
		*aKnightRight,
		*aRookRight;
		
#endif
	
#ifdef	_HOME_INIT_

	for (int i=0;i<home.size();i++){
		home[i].color 	= COLOR (0.0f, 1.0f, 1.0f);
		home[i].row		= (char)-1;
	}

	hRookLeft			= &home[0];
	hRookLeft->row		= GD_ROWS-1;
	hRookLeft->col		= 0;
	hRookLeft->type		= PT_ROOK;
	
	hKnightLeft			= &home[1];
	hKnightLeft->row	= GD_ROWS-1;
	hKnightLeft->col	= 1;
	hKnightLeft->type	= PT_KNIGHT;
	
	hBishopLeft			= &home[2];
	hBishopLeft->row	= GD_ROWS-1;
	hBishopLeft->col	= 2;
	hBishopLeft->type	= PT_BISHOP;
	
	hQueenLeft			= &home[3];
	hQueenLeft->row		= GD_ROWS-1;
	hQueenLeft->col		= 3;
	hQueenLeft->type	= PT_QUEEN;
	
	hQueenRight			= &home[4];
	hQueenRight->row	= GD_ROWS-1;
	hQueenRight->col	= GD_COLS-4;
	hQueenRight->type	= PT_QUEEN;
	
	hBishopRight		= &home[5];
	hBishopRight->row	= GD_ROWS-1;
	hBishopRight->col	= GD_COLS-3;
	hBishopRight->type	= PT_BISHOP;
	
	hKnightRight		= &home[6];
	hKnightRight->row	= GD_ROWS-1;
	hKnightRight->col	= GD_COLS-2;
	hKnightRight->type	= PT_KNIGHT;
	
	hRookRight			= &home[7];
	hRookRight->row		= GD_ROWS-1;
	hRookRight->col		= GD_COLS-1;
	hRookRight->type	= PT_ROOK;
	
#endif
	
#ifdef	_AWAY_INIT_

	for (int i=0;i<away.size();i++){
		away[i].color	= COLOR (1.0f, 0.8f, 0.5f);
		away[i].row		= (char)-1;
	}
	
	aRookLeft			= &away[0];
	aRookLeft->row		= 0;
	aRookLeft->col		= 0;
	aRookLeft->type		= PT_ROOK;
	
	aKnightLeft			= &away[1];
	aKnightLeft->row	= 0;
	aKnightLeft->col	= 1;
	aKnightLeft->type	= PT_KNIGHT;
	
	aBishopLeft			= &away[2];
	aBishopLeft->row	= 0;
	aBishopLeft->col	= 2;
	aBishopLeft->type	= PT_BISHOP;
	
	aQueenLeft			= &away[3];
	aQueenLeft->row		= 0;
	aQueenLeft->col		= 3;
	aQueenLeft->type	= PT_QUEEN;
	
	aQueenRight			= &away[4];
	aQueenRight->row	= 0;
	aQueenRight->col	= GD_COLS-4;
	aQueenRight->type	= PT_QUEEN;
	
	aBishopRight		= &away[5];
	aBishopRight->row	= 0;
	aBishopRight->col	= GD_COLS-3;
	aBishopRight->type	= PT_BISHOP;
	
	aKnightRight		= &away[6];
	aKnightRight->row	= 0;
	aKnightRight->col	= GD_COLS-2;
	aKnightRight->type	= PT_KNIGHT;
	
	aRookRight			= &away[7];
	aRookRight->row		= 0;
	aRookRight->col		= GD_COLS-1;
	aRookRight->type	= PT_ROOK;

#endif
	
#ifdef	_WINDOW_INIT

	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor (NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "GLSample";
	RegisterClass (&wc);
	
	hWnd = CreateWindowEx (WS_EX_TOPMOST, "GLSample", "Turn Based Game",  WS_POPUP | WS_VISIBLE, 0, 0, GetSystemMetrics(1)*(GD_COLS/(float)GD_ROWS), GetSystemMetrics(1), NULL, NULL, hInstance, NULL);
	
	EnableOpenGL (hWnd, &hDC, &hRC);
	
#endif
	
	
	hKnightLeft->dir = DR_UP | DR_RIGHT;
	//MAIN LOOP
	while (!bQuit){
		if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)){
			if (msg.message == WM_QUIT)
				bQuit = true;
			else if (msg.message == WM_KEYUP){
				switch (msg.wParam){
					case VK_ESCAPE:
						bQuit = true;
						break;
					case VK_LEFT:
						sel.col -= 1;
						break;
					case VK_UP:
						sel.row -= 1;
						break;
					case VK_RIGHT:
						sel.col += 1;
						break;
					case VK_DOWN:
						sel.row += 1;
						break;
					case VK_SPACE:
						//NOTHING ALREADY SELECTED
						if (selPtr == NULL){
							//*
							if ((selPtr = SelectPiece (sel.row, sel.col, (homeTurn?home:away))) != NULL){
								INVERT (sel.color);
								moves = FindMoves (*selPtr, (homeTurn?home:away), (homeTurn?away:home));
							}
							//*/
						}
						//SOMETHING ALREADY SELECTED
						else{
							//*
							bool killTar = false;
							if (SelectPiece (sel.row, sel.col, moves) != NULL){
								if ((tarPtr = SelectPiece (sel.row, sel.col, (homeTurn?away:home))) != NULL){
									tarPtr->row = -1;
									killTar = true;
									selPtr->dir = 0;
								}
								MovePiece (*selPtr, sel.row, sel.col);
								
								homeTurn = not homeTurn;
							}
							if (killTar)
								(homeTurn?home:away).erase ((homeTurn?home:away).begin()+FindTarget (*tarPtr, (homeTurn?home:away)));
							INVERT (sel.color);
							selPtr = NULL;
							//*/
						}
						break;
					default:
						std::cout << "VK " << msg.wParam << " Pressed!" << std::endl;
						break;
				}
				if (sel.col==GD_COLS)
					sel.col = 0;
				if (sel.col==(char)-1)
					sel.col = GD_COLS-1;
				if (sel.row==GD_ROWS)
					sel.row = 0;
				if (sel.row==(char)-1)
					sel.row = GD_ROWS-1;
			}else{
				TranslateMessage(&msg);
				DispatchMessage	(&msg);
			}
		}else{
			glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
			glClear	(GL_COLOR_BUFFER_BIT);
			glPushMatrix();
			DrawBackground (bg1, bg2);
			for (int i=0;i<home.size();i++)
				home[i].draw();
			for (int i=0;i<away.size();i++)
				away[i].draw();
			if (selPtr != NULL)
				for (int i=0;i<moves.size();i++)
					moves[i].draw();
			sel.draw ();
			glPopMatrix	();
			SwapBuffers	(hDC);
		}
	}
	
	DisableOpenGL (hWnd, hDC, hRC);
	DestroyWindow (hWnd);
	return msg.wParam;
}

Color::Color (){
	r = 0.0f;
	g = 0.0f;
	b = 0.0f;
}

Color::Color (float red, float green, float blue){
	r = red;
	g = green;
	b = blue;
}

Piece::Piece (){	
	alive	= true;
	row		= 0;
	col		= 0;
	type	= 0;
	dir		= 0;
}

Piece::Piece (Color clr){
	alive = true;
	
	row = 0;
	col = 0;
	type= 0;
	dir	= 0;
	
	color.r = clr.r;
	color.g = clr.g;
	color.b = clr.b;
}

Piece::Piece (Color clr, char t){
	alive = true;
	
	row = 0;
	col = 0;
	type= t;
	dir	= 0;
	
	color.r = clr.r;
	color.g = clr.g;
	color.b = clr.b;
}

Piece::Piece (Color clr, char r, char c){
	alive = true;
	
	row = r;
	col = c;
	type= 0;
	dir	= 0;
	
	color.r = clr.r;
	color.g = clr.g;
	color.b = clr.b;
}

Piece::Piece (Color clr, char r, char c, char t){
	alive = true;
	
	row = r;
	col = c;
	type= t;
	dir	= 0;
	
	color.r = clr.r;
	color.g = clr.g;
	color.b = clr.b;
}

Piece::Piece (char t){
	alive = true;
	
	row = 0;
	col = 0;
	type= t;
	dir	= 0;
}

Piece::Piece (char r, char c){
	alive = true;
	
	row = r;
	col = c;
	type= 0;
	dir	= 0;
}

Piece::Piece (char r, char c, char t){
	alive = true;
	
	row = r;
	col = c;
	type= t;
	dir	= 0;
}

void Piece::draw (){
	if (not alive)
		return;
	double dR = 2.0/(double)GD_ROWS;
	double dC = 2.0/(double)GD_COLS;
	double Cx = dC * col - 1.0 + dC/2.0;
	double Cy = 1.0 - (dR * row + dR/2.0);
	if (type == PT_CIRCLE){
		
		glColor3f (color.r, color.g, color.b);
		glPushMatrix ();
		glTranslated (Cx, Cy, 0.0);
		glBegin (GL_TRIANGLE_FAN);
		glVertex2d (0.0, 0.0);
		
		for (int i=0;i<30;i++)
			glVertex2d (cos (2.0*i*PI/30.0)*dC*0.4, sin (2.0*i*PI/30.0)*dR*0.4);
		
		glVertex2d (dC * 0.4, 0.0);
		glEnd ();
		glPopMatrix ();
	}else if (type == PT_FAN){
		glColor3f	(color.r, color.g, color.b);
		glPushMatrix ();
		glTranslated (Cx, Cy, 0.0);
		glBegin		(GL_TRIANGLES);
		glVertex2d	(0.0, 0.0);
		
		for (int i=0;i<30;i++) {
			glVertex2d	(cos (2.0*i*PI/30.0)*dC*0.4, sin (2.0*i*PI/30.0)*dR*0.4);
			if (i%2==1)
				glVertex2d (0.0, 0.0);
		}
		
		
		glVertex2d (dC * 0.4, 0.0);
		glEnd ();
		glPopMatrix ();
	}else if (type == PT_SELECT){
		glColor3f	(color.r, color.g, color.b);
		glPushMatrix ();
		glTranslated (Cx, Cy, 0.0);
		glBegin		(GL_TRIANGLE_STRIP);
		glVertex2d	(-3.0*dC/8.0, 3.0*dR/8.0);
		glVertex2d	(-dC/2.0, dR/2.0);
		glVertex2d	(-3.0*dC/8.0, -3.0*dR/8.0);
		glVertex2d	(-dC/2.0, -dR/2.0);
		glVertex2d	(3.0*dC/8.0, -3.0*dR/8.0);
		glVertex2d	(dC/2.0, -dR/2.0);
		glVertex2d	(3.0*dC/8.0, 3.0*dR/8.0);
		glVertex2d	(dC/2.0, dR/2.0);
		glVertex2d	(-3.0*dC/8.0, 3.0*dR/8.0);
		glVertex2d	(-dC/2.0, dR/2.0);
		glEnd		();
		glPopMatrix ();
	}else if (type == PT_OPTION){
		glColor3f	(color.r, color.g, color.b);
		glPushMatrix ();
		glTranslated (Cx, Cy, 0.0);
		glBegin		(GL_TRIANGLE_STRIP);
		glVertex2d	(-3.0*dC/8.0, 3.0*dR/8.0);
		glVertex2d	(-3.0*dC/8.0, -3.0*dR/8.0);
		glVertex2d	(3.0*dC/8.0, 3.0*dR/8.0);
		glVertex2d	(3.0*dC/8.0, -3.0*dR/8.0);
		glEnd		();
		glPopMatrix	();
	}else if (type == PT_ROOK){
		glColor3f	(color.r, color.g, color.b);
		glPushMatrix ();
		glTranslated (Cx, Cy, 0.0);
		glBegin		(GL_TRIANGLE_FAN);
		glVertex2d	(0.0, 0.0);
		glVertex2d	(3.0*dC/8.0, 1.0*dR/8.0);
		glVertex2d	(1.0*dC/8.0, 1.0*dR/8.0);
		glVertex2d	(1.0*dC/8.0, 3.0*dR/8.0);
		glVertex2d	(-1.0*dC/8.0, 3.0*dR/8.0);
		glVertex2d	(-1.0*dC/8.0, 1.0*dR/8.0);
		glVertex2d	(-3.0*dC/8.0, 1.0*dR/8.0);
		glVertex2d	(-3.0*dC/8.0, -1.0*dR/8.0);
		glVertex2d	(-1.0*dC/8.0, -1.0*dR/8.0);
		glVertex2d	(-1.0*dC/8.0, -3.0*dR/8.0);
		glVertex2d	(1.0*dC/8.0, -3.0*dR/8.0);
		glVertex2d	(1.0*dC/8.0, -1.0*dR/8.0);
		glVertex2d	(3.0*dC/8.0, -1.0*dR/8.0);
		glVertex2d	(3.0*dC/8.0, 1.0*dR/8.0);
		glEnd		();
		glPopMatrix ();
	}else if (type == PT_BISHOP){
		dR = (dC + dR)/2.0;
		dC = dR;
		glColor3f	(color.r, color.g, color.b);
		glPushMatrix ();
		glTranslated (Cx, Cy, 0.0);
		glRotated 	(45.0, 0.0, 0.0, 1.0);
		glBegin		(GL_TRIANGLE_FAN);
		glVertex2d	(0.0, 0.0);
		glVertex2d	(3.0*dC/8.0, 0.5*dR/8.0);
		glVertex2d	(0.5*dC/8.0, 0.5*dR/8.0);
		glVertex2d	(0.5*dC/8.0, 3.0*dR/8.0);
		glVertex2d	(-0.5*dC/8.0, 3.0*dR/8.0);
		glVertex2d	(-0.5*dC/8.0, 0.5*dR/8.0);
		glVertex2d	(-3.0*dC/8.0, 0.5*dR/8.0);
		glVertex2d	(-3.0*dC/8.0, -0.5*dR/8.0);
		glVertex2d	(-0.5*dC/8.0, -0.5*dR/8.0);
		glVertex2d	(-0.5*dC/8.0, -3.0*dR/8.0);
		glVertex2d	(0.5*dC/8.0, -3.0*dR/8.0);
		glVertex2d	(0.5*dC/8.0, -0.5*dR/8.0);
		glVertex2d	(3.0*dC/8.0, -0.5*dR/8.0);
		glVertex2d	(3.0*dC/8.0, 0.5*dR/8.0);
		glEnd		();
		glPopMatrix ();
	}else if (type == PT_KNIGHT){
		glColor3f	(color.r, color.g, color.b);
		glPushMatrix	();
		glTranslated	(Cx, Cy, 0.0);
		glBegin			(GL_TRIANGLE_STRIP);
		glVertex2d		(3.0*dC/40.0, 3.0*dR/8.0);
		glVertex2d		(3.0*dC/40.0, 9.0*dR/40.0);
		glVertex2d		(9.0*dC/40.0, 3.0*dR/8.0);
		glVertex2d		(9.0*dC/40.0, 9.0*dR/40.0);
		glEnd			();
		glBegin			(GL_TRIANGLE_STRIP);
		glVertex2d		(9.0*dC/40.0, 9.0*dR/40.0);
		glVertex2d		(9.0*dC/40.0, 3.0*dR/40.0);
		glVertex2d		(3.0*dC/8.0, 9.0*dR/40.0);
		glVertex2d		(3.0*dC/8.0, 3.0*dR/40.0);
		glEnd			();
		glBegin			(GL_TRIANGLE_STRIP);
		glVertex2d		(3.0*dC/40.0, -3.0*dR/8.0);
		glVertex2d		(3.0*dC/40.0, -9.0*dR/40.0);
		glVertex2d		(9.0*dC/40.0, -3.0*dR/8.0);
		glVertex2d		(9.0*dC/40.0, -9.0*dR/40.0);
		glEnd			();
		glBegin			(GL_TRIANGLE_STRIP);
		glVertex2d		(9.0*dC/40.0, -9.0*dR/40.0);
		glVertex2d		(9.0*dC/40.0, -3.0*dR/40.0);
		glVertex2d		(3.0*dC/8.0, -9.0*dR/40.0);
		glVertex2d		(3.0*dC/8.0, -3.0*dR/40.0);
		glEnd			();
		glBegin			(GL_TRIANGLE_STRIP);
		glVertex2d		(-3.0*dC/40.0, 3.0*dR/8.0);
		glVertex2d		(-3.0*dC/40.0, 9.0*dR/40.0);
		glVertex2d		(-9.0*dC/40.0, 3.0*dR/8.0);
		glVertex2d		(-9.0*dC/40.0, 9.0*dR/40.0);
		glEnd			();
		glBegin			(GL_TRIANGLE_STRIP);
		glVertex2d		(-9.0*dC/40.0, 9.0*dR/40.0);
		glVertex2d		(-9.0*dC/40.0, 3.0*dR/40.0);
		glVertex2d		(-3.0*dC/8.0, 9.0*dR/40.0);
		glVertex2d		(-3.0*dC/8.0, 3.0*dR/40.0);
		glEnd			();
		glBegin			(GL_TRIANGLE_STRIP);
		glVertex2d		(-3.0*dC/40.0, -3.0*dR/8.0);
		glVertex2d		(-3.0*dC/40.0, -9.0*dR/40.0);
		glVertex2d		(-9.0*dC/40.0, -3.0*dR/8.0);
		glVertex2d		(-9.0*dC/40.0, -9.0*dR/40.0);
		glEnd			();
		glBegin			(GL_TRIANGLE_STRIP);
		glVertex2d		(-9.0*dC/40.0, -9.0*dR/40.0);
		glVertex2d		(-9.0*dC/40.0, -3.0*dR/40.0);
		glVertex2d		(-3.0*dC/8.0, -9.0*dR/40.0);
		glVertex2d		(-3.0*dC/8.0, -3.0*dR/40.0);
		glEnd			();
		glPopMatrix		();
	}else if (type == PT_QUEEN){
		glColor3f	(color.r, color.g, color.b);
		glPushMatrix ();
		glTranslated (Cx, Cy, 0.0);
		glBegin		(GL_TRIANGLES);
		for (int i=0;i<16;i++) {
			glVertex2d	(cos (i*PI/8.0-PI/16.0)*dC*0.325, sin (i*PI/8.0-PI/16.0)*dR*0.325);
			if (i%2==1)
				glVertex2d (0.0, 0.0);
		}
		glVertex2d (dC * 0.325, 0.0);
		glEnd		();
		glPopMatrix	();
	}
	return;
}

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam){
    switch (message){
		case WM_CREATE:
			return 0;
		case WM_CLOSE:
			PostQuitMessage (0);
			return 0;
		case WM_DESTROY:
			return 0;
		default:
			return DefWindowProc (hWnd, message, wParam, lParam);
    }
}

void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC){
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    *hDC = GetDC (hWnd);

    ZeroMemory (&pfd, sizeof (pfd));

    pfd.nSize = sizeof (pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat (*hDC, &pfd);
    SetPixelFormat (*hDC, iFormat, &pfd);

    *hRC = wglCreateContext( *hDC );
    wglMakeCurrent( *hDC, *hRC );
	
	return;
}

void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC){
    wglMakeCurrent (NULL, NULL);
    wglDeleteContext (hRC);
    ReleaseDC (hWnd, hDC);
	
	return;
}

Color COLOR (float r, float g, float b){
	Color retVal;
	retVal.r = r;
	retVal.g = g;
	retVal.b = b;
	return retVal;
}

Color INVERT (Color &c){
	c.r = 1.0f - c.r;
	c.g = 1.0f - c.g;
	c.b = 1.0f - c.b;
	return c;
}

void inline SetColor (Color c){
	glColor3f (c.r, c.g, c.b);
	return;
}

void DrawBackground (Color c1, Color c2){
	float dR = 2.0f/(float)GD_ROWS;
	float dC = 2.0f/(float)GD_COLS;
	
	glBegin		(GL_TRIANGLES);
	SetColor	(c1);
	
	glVertex2f	(-1.0f, -1.0f);
	glVertex2f	( 1.0f, -1.0f);
	glVertex2f	(-1.0f,	 1.0f);
	
	glVertex2f	( 1.0f, -1.0f);
	glVertex2f	(-1.0f,	 1.0f);
	glVertex2f	( 1.0f,  1.0f);
	
	SetColor	(c2);
	
	for (char i=0;i<GD_COLS;i++){
		for (char j=0;j<GD_ROWS;j++){
			if ((i+j)%2==1)
				continue;
			glVertex2f (-1.0f+(float)i*dC,		-1.0f+(float)j*dR);
			glVertex2f (-1.0f+(float)i*dC+dC,	-1.0f+(float)j*dR);
			glVertex2f (-1.0f+(float)i*dC,		-1.0f+(float)j*dR+dR);
			
			glVertex2f (-1.0f+(float)i*dC+dC,	-1.0f+(float)j*dR);
			glVertex2f (-1.0f+(float)i*dC,		-1.0f+(float)j*dR+dR);
			glVertex2f (-1.0f+(float)i*dC+dC,	-1.0f+(float)j*dR+dR);
		}
	}
	glEnd ();
	return;
}

Piece* SelectPiece (char r, char c, Piece* arrPieces, int lPieces){
	for (int i=0;i<lPieces;i++)
		if (arrPieces[i].row == r and arrPieces[i].col == c)
			return &arrPieces[i];
	return NULL;
}

Piece* SelectPiece (char r, char c, std::vector<Piece> &p){
	for (int i=0;i<p.size();i++)
		if (p[i].row == r and p[i].col == c)
			return &p[i];
	return NULL;
}

void MovePiece (Piece &p, char r, char c){
	p.row = r;
	p.col = c;
	return;
}

std::vector<Piece> FindMoves (Piece &p){
	std::vector<Piece>	retVal;
	Piece				temp;
	
	temp.color	= COLOR (1.0f, 1.0f, 1.0f);
	temp.type	= PT_OPTION;
	
	int o = 0;
	switch (p.type){
		case PT_QUEEN:
		case PT_ROOK:
			/*119*/
			temp.row = p.row;
			for (int i=0;i<GD_COLS;i++){
				if (p.col == i){
					continue;
				}
				temp.col = i;
				retVal.push_back (temp);
			}
			temp.col = p.col;
			for (int i=0;i<GD_ROWS;i++){
				if (p.row == i){
					continue;
				}
				temp.row = i;
				retVal.push_back (temp);
			}//*/
			/*105-135
			for (int d=1;p.row-d>=0 or p.row+d<GD_ROWS or p.col-d>=0 or p.col+d<GD_COLS;d++){
				if (p.row-d>=0){
					temp.row = p.row-d;
					temp.col = p.col;
					retVal.push_back (temp);
				}
				if (p.row+d<GD_ROWS){
					temp.row = p.row+d;
					temp.col = p.col;
					retVal.push_back (temp);
				}
				if (p.col-d>=0){
					temp.row = p.row;
					temp.col = p.col-d;
					retVal.push_back (temp);
				}
				if (p.col+d<GD_COLS){
					temp.row = p.row;
					temp.col = p.col+d;
					retVal.push_back (temp);
				}
			}
			//*/
			if (p.type != PT_QUEEN)
				break;
		case PT_BISHOP:
			for (int d=1;p.row-d>=0;d++){
				if (p.col-d>=0){
					temp.row = p.row-d;
					temp.col = p.col-d;
					retVal.push_back (temp);
				}
				if (p.col+d<GD_COLS){
					temp.row = p.row-d;
					temp.col = p.col+d;
					retVal.push_back (temp);
				}
			}
			for (int d=1;p.row+d<GD_ROWS;d++){
				if (p.col-d>=0){
					temp.row = p.row+d;
					temp.col = p.col-d;
					retVal.push_back (temp);
				}
				if (p.col+d<GD_COLS){
					temp.row = p.row+d;
					temp.col = p.col+d;
					retVal.push_back (temp);
				}
			}
			break;
		case PT_KING:
			temp.row = p.row+1;
			temp.col = p.col+1;
			retVal.push_back (temp);
			temp.col = p.col;
			retVal.push_back (temp);
			temp.col = p.col-1;
			retVal.push_back (temp);
			temp.row = p.row;
			retVal.push_back (temp);
			temp.col = p.col+1;
			retVal.push_back (temp);
			temp.row = p.row-1;
			retVal.push_back (temp);
			temp.col = p.col;
			retVal.push_back (temp);
			temp.col = p.col-1;
			retVal.push_back (temp);
			break;
		case PT_KNIGHT:
			temp.row = p.row+2;
			temp.col = p.col+1;
			retVal.push_back (temp);
			temp.col = p.col-1;
			retVal.push_back (temp);
			temp.row = p.row+1;
			temp.col = p.col+2;
			retVal.push_back (temp);
			temp.col = p.col-2;
			retVal.push_back (temp);
			temp.row = p.row-1;
			retVal.push_back (temp);
			temp.col = p.col+2;
			retVal.push_back (temp);
			temp.row = p.row-2;
			temp.col = p.col+1;
			retVal.push_back (temp);
			temp.col = p.col-1;
			retVal.push_back (temp);
			break;
		default:
			break;
	}
	return retVal;
}

std::vector<Piece> FindMoves (Piece &p, std::vector<Piece> pieces){
	std::vector<Piece>	retVal;
	Piece				temp;
	
	temp.color	= COLOR (1.0f, 1.0f, 1.0f);
	temp.type	= PT_OPTION;
	switch (p.type){
		case PT_QUEEN:
		case PT_ROOK:
			/*119
			temp.row = p.row;
			for (int i=0;i<GD_COLS;i++){
				if (p.col == i){
					continue;
				}
				temp.col = i;
				retVal.push_back (temp);
			}
			temp.col = p.col;
			for (int i=0;i<GD_ROWS;i++){
				if (p.row == i){
					continue;
				}
				temp.row = i;
				retVal.push_back (temp);
			}//*/
			/*105-135*/
			temp.col = p.col;
			
			for (temp.row=p.row-1;temp.row>=0;temp.row--)
				if (SelectPiece (temp.row, temp.col, pieces) != NULL){
					temp.color = COLOR (1.0f, 0.0f, 0.0f);
					retVal.push_back (temp);
					temp.color = COLOR (1.0f, 1.0f, 1.0f);
					break;
				}else
					retVal.push_back (temp);
			
			for (temp.row=p.row+1;temp.row<GD_ROWS;temp.row++)
				if (SelectPiece (temp.row, temp.col, pieces) != NULL){
					temp.color = COLOR (1.0f, 0.0f, 0.0f);
					retVal.push_back (temp);
					temp.color = COLOR (1.0f, 1.0f, 1.0f);
					break;
				}else
					retVal.push_back (temp);
			
			temp.row = p.row;
			
			for (temp.col=p.col-1;temp.col>=0;temp.col--)
				if (SelectPiece (temp.row, temp.col, pieces) != NULL){
					temp.color = COLOR (1.0f, 0.0f, 0.0f);
					retVal.push_back (temp);
					temp.color = COLOR (1.0f, 1.0f, 1.0f);
					break;
				}else
					retVal.push_back (temp);
			
			for (temp.col=p.col+1;temp.col<GD_COLS;temp.col++)
				if (SelectPiece (temp.row, temp.col, pieces) != NULL){
					temp.color = COLOR (1.0f, 0.0f, 0.0f);
					retVal.push_back (temp);
					temp.color = COLOR (1.0f, 1.0f, 1.0f);
					break;
				}else
					retVal.push_back (temp);
			
			//*/
			if (p.type != PT_QUEEN)
				break;
		case PT_BISHOP:
			for (int d=1;p.row-d>=0;d++){
				temp.row = p.row-d;
				temp.col = p.col-d;
				if (SelectPiece (temp.row, temp.col, pieces) != NULL){
					temp.color = COLOR (1.0f, 0.0f, 0.0f);
					retVal.push_back (temp);
					temp.color = COLOR (1.0f, 1.0f, 1.0f);
					break;
				}else
					retVal.push_back (temp);
			}
			for (int d=1;p.row-d>=0;d++){
				temp.row = p.row-d;
				temp.col = p.col+d;
				if (SelectPiece (temp.row, temp.col, pieces) != NULL){
					temp.color = COLOR (1.0f, 0.0f, 0.0f);
					retVal.push_back (temp);
					temp.color = COLOR (1.0f, 1.0f, 1.0f);
					break;
				}else
					retVal.push_back (temp);
			}
			for (int d=1;p.row+d<GD_ROWS;d++){
				temp.row = p.row+d;
				temp.col = p.col-d;
				if (SelectPiece (temp.row, temp.col, pieces) != NULL){
					temp.color = COLOR (1.0f, 0.0f, 0.0f);
					retVal.push_back (temp);
					temp.color = COLOR (1.0f, 1.0f, 1.0f);
					break;
				}else
					retVal.push_back (temp);
			}
			for (int d=1;p.row+d<GD_ROWS;d++){
				temp.row = p.row+d;
				temp.col = p.col+d;
				if (SelectPiece (temp.row, temp.col, pieces) != NULL){
					temp.color = COLOR (1.0f, 0.0f, 0.0f);
					retVal.push_back (temp);
					temp.color = COLOR (1.0f, 1.0f, 1.0f);
					break;
				}else
					retVal.push_back (temp);
			}
			break;
		case PT_KING:
			temp.row = p.row+1;
			temp.col = p.col+1;
			if (SelectPiece (temp.row, temp.col, pieces) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else
				retVal.push_back (temp);
			temp.col = p.col;
			if (SelectPiece (temp.row, temp.col, pieces) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else
				retVal.push_back (temp);
			temp.col = p.col-1;
			if (SelectPiece (temp.row, temp.col, pieces) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else
				retVal.push_back (temp);
			temp.row = p.row;
			if (SelectPiece (temp.row, temp.col, pieces) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else
				retVal.push_back (temp);
			temp.col = p.col+1;
			if (SelectPiece (temp.row, temp.col, pieces) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else
				retVal.push_back (temp);
			temp.row = p.row-1;
			if (SelectPiece (temp.row, temp.col, pieces) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else
				retVal.push_back (temp);
			temp.col = p.col;
			if (SelectPiece (temp.row, temp.col, pieces) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else
				retVal.push_back (temp);
			temp.col = p.col-1;
			if (SelectPiece (temp.row, temp.col, pieces) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else
				retVal.push_back (temp);
			break;
		case PT_KNIGHT:
			temp.row = p.row+2;
			temp.col = p.col+1;
			if (SelectPiece (temp.row, temp.col, pieces) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else
				retVal.push_back (temp);
			temp.col = p.col-1;
			if (SelectPiece (temp.row, temp.col, pieces) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else
				retVal.push_back (temp);
			temp.row = p.row+1;
			temp.col = p.col+2;
			if (SelectPiece (temp.row, temp.col, pieces) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else
				retVal.push_back (temp);
			temp.col = p.col-2;
			if (SelectPiece (temp.row, temp.col, pieces) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else
				retVal.push_back (temp);
			temp.row = p.row-1;
			if (SelectPiece (temp.row, temp.col, pieces) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else
				retVal.push_back (temp);
			temp.col = p.col+2;
			if (SelectPiece (temp.row, temp.col, pieces) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else
				retVal.push_back (temp);
			temp.row = p.row-2;
			temp.col = p.col+1;
			if (SelectPiece (temp.row, temp.col, pieces) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else
				retVal.push_back (temp);
			temp.col = p.col-1;
			if (SelectPiece (temp.row, temp.col, pieces) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else
				retVal.push_back (temp);
			break;
		default:
			break;
	}
	return retVal;
}

std::vector<Piece> FindMoves (Piece &p, std::vector<Piece> friendly, std::vector<Piece> hostile){
	std::vector<Piece>	retVal;
	Piece				temp;
	
	temp.color	= COLOR (1.0f, 1.0f, 1.0f);
	temp.type	= PT_OPTION;
	switch (p.type){
		case PT_QUEEN:
		case PT_ROOK:
			/*119
			temp.row = p.row;
			for (int i=0;i<GD_COLS;i++){
				if (p.col == i){
					continue;
				}
				temp.col = i;
				retVal.push_back (temp);
			}
			temp.col = p.col;
			for (int i=0;i<GD_ROWS;i++){
				if (p.row == i){
					continue;
				}
				temp.row = i;
				retVal.push_back (temp);
			}//*/
			/*105-135*/
			temp.col = p.col;
			
			for (temp.row=p.row-1;temp.row>=0;temp.row--)
				if (SelectPiece (temp.row, temp.col, hostile) != NULL){
					temp.color = COLOR (1.0f, 0.0f, 0.0f);
					retVal.push_back (temp);
					temp.color = COLOR (1.0f, 1.0f, 1.0f);
					break;
				}else if (SelectPiece (temp.row, temp.col, friendly) != NULL)
					break;
				else
					retVal.push_back (temp);
			
			for (temp.row=p.row+1;temp.row<GD_ROWS;temp.row++)
				if (SelectPiece (temp.row, temp.col, hostile) != NULL){
					temp.color = COLOR (1.0f, 0.0f, 0.0f);
					retVal.push_back (temp);
					temp.color = COLOR (1.0f, 1.0f, 1.0f);
					break;
				}else if (SelectPiece (temp.row, temp.col, friendly) != NULL)
					break;
				else
					retVal.push_back (temp);
			
			temp.row = p.row;
			
			for (temp.col=p.col-1;temp.col>=0;temp.col--)
				if (SelectPiece (temp.row, temp.col, hostile) != NULL){
					temp.color = COLOR (1.0f, 0.0f, 0.0f);
					retVal.push_back (temp);
					temp.color = COLOR (1.0f, 1.0f, 1.0f);
					break;
				}else if (SelectPiece (temp.row, temp.col, friendly) != NULL)
					break;
				else
					retVal.push_back (temp);
			
			for (temp.col=p.col+1;temp.col<GD_COLS;temp.col++)
				if (SelectPiece (temp.row, temp.col, hostile) != NULL){
					temp.color = COLOR (1.0f, 0.0f, 0.0f);
					retVal.push_back (temp);
					temp.color = COLOR (1.0f, 1.0f, 1.0f);
					break;
				}else if (SelectPiece (temp.row, temp.col, friendly) != NULL)
					break;
				else
					retVal.push_back (temp);
			
			//*/
			if (p.type != PT_QUEEN)
				break;
		case PT_BISHOP:
			for (int d=1;p.row-d>=0;d++){
				temp.row = p.row-d;
				temp.col = p.col-d;
				if (SelectPiece (temp.row, temp.col, hostile) != NULL){
					temp.color = COLOR (1.0f, 0.0f, 0.0f);
					retVal.push_back (temp);
					temp.color = COLOR (1.0f, 1.0f, 1.0f);
					break;
				}else if (SelectPiece (temp.row, temp.col, friendly) != NULL)
					break;
				else
					retVal.push_back (temp);
			}
			for (int d=1;p.row-d>=0;d++){
				temp.row = p.row-d;
				temp.col = p.col+d;
				if (SelectPiece (temp.row, temp.col, hostile) != NULL){
					temp.color = COLOR (1.0f, 0.0f, 0.0f);
					retVal.push_back (temp);
					temp.color = COLOR (1.0f, 1.0f, 1.0f);
					break;
				}else if (SelectPiece (temp.row, temp.col, friendly) != NULL)
					break;
				else
					retVal.push_back (temp);
			}
			for (int d=1;p.row+d<GD_ROWS;d++){
				temp.row = p.row+d;
				temp.col = p.col-d;
				if (SelectPiece (temp.row, temp.col, hostile) != NULL){
					temp.color = COLOR (1.0f, 0.0f, 0.0f);
					retVal.push_back (temp);
					temp.color = COLOR (1.0f, 1.0f, 1.0f);
					break;
				}else if (SelectPiece (temp.row, temp.col, friendly) != NULL)
					break;
				else
					retVal.push_back (temp);
			}
			for (int d=1;p.row+d<GD_ROWS;d++){
				temp.row = p.row+d;
				temp.col = p.col+d;
				if (SelectPiece (temp.row, temp.col, hostile) != NULL){
					temp.color = COLOR (1.0f, 0.0f, 0.0f);
					retVal.push_back (temp);
					temp.color = COLOR (1.0f, 1.0f, 1.0f);
					break;
				}else if (SelectPiece (temp.row, temp.col, friendly) != NULL)
					break;
				else
					retVal.push_back (temp);
			}
			break;
		case PT_KING:
			temp.row = p.row+1;
			temp.col = p.col+1;
			if (SelectPiece (temp.row, temp.col, hostile) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else if (SelectPiece (temp.row, temp.col, friendly) == NULL)
				retVal.push_back (temp);
			temp.col = p.col;
			if (SelectPiece (temp.row, temp.col, hostile) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else if (SelectPiece (temp.row, temp.col, friendly) == NULL)
				retVal.push_back (temp);
			temp.col = p.col-1;
			if (SelectPiece (temp.row, temp.col, hostile) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else if (SelectPiece (temp.row, temp.col, friendly) == NULL)
				retVal.push_back (temp);
			temp.row = p.row;
			if (SelectPiece (temp.row, temp.col, hostile) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else if (SelectPiece (temp.row, temp.col, friendly) == NULL)
				retVal.push_back (temp);
			temp.col = p.col+1;
			if (SelectPiece (temp.row, temp.col, hostile) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else if (SelectPiece (temp.row, temp.col, friendly) == NULL)
				retVal.push_back (temp);
			temp.row = p.row-1;
			if (SelectPiece (temp.row, temp.col, hostile) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else if (SelectPiece (temp.row, temp.col, friendly) == NULL)
				retVal.push_back (temp);
			temp.col = p.col;
			if (SelectPiece (temp.row, temp.col, hostile) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else if (SelectPiece (temp.row, temp.col, friendly) == NULL)
				retVal.push_back (temp);
			temp.col = p.col-1;
			if (SelectPiece (temp.row, temp.col, hostile) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else if (SelectPiece (temp.row, temp.col, friendly) == NULL)
				retVal.push_back (temp);
			break;
		case PT_KNIGHT:
			temp.row = p.row+2;
			temp.col = p.col+1;
			if (SelectPiece (temp.row, temp.col, hostile) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else if (SelectPiece (temp.row, temp.col, friendly) == NULL)
				retVal.push_back (temp);
			temp.col = p.col-1;
			if (SelectPiece (temp.row, temp.col, hostile) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else if (SelectPiece (temp.row, temp.col, friendly) == NULL)
				retVal.push_back (temp);
			temp.row = p.row+1;
			temp.col = p.col+2;
			if (SelectPiece (temp.row, temp.col, hostile) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else if (SelectPiece (temp.row, temp.col, friendly) == NULL)
				retVal.push_back (temp);
			temp.col = p.col-2;
			if (SelectPiece (temp.row, temp.col, hostile) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else if (SelectPiece (temp.row, temp.col, friendly) == NULL)
				retVal.push_back (temp);
			temp.row = p.row-1;
			if (SelectPiece (temp.row, temp.col, hostile) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else if (SelectPiece (temp.row, temp.col, friendly) == NULL)
				retVal.push_back (temp);
			temp.col = p.col+2;
			if (SelectPiece (temp.row, temp.col, hostile) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else if (SelectPiece (temp.row, temp.col, friendly) == NULL)
				retVal.push_back (temp);
			temp.row = p.row-2;
			temp.col = p.col+1;
			if (SelectPiece (temp.row, temp.col, hostile) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else if (SelectPiece (temp.row, temp.col, friendly) == NULL)
				retVal.push_back (temp);
			temp.col = p.col-1;
			if (SelectPiece (temp.row, temp.col, hostile) != NULL){
				temp.color = COLOR (1.0f, 0.0f, 0.0f);
				retVal.push_back (temp);
				temp.color = COLOR (1.0f, 1.0f, 1.0f);
			}else if (SelectPiece (temp.row, temp.col, friendly) == NULL)
				retVal.push_back (temp);
			break;
		default:
			break;
	}
	return retVal;
}

int FindTarget (Piece &p, std::vector<Piece> v){
	for (int i=0;i<v.size();i++){
		if (p.row == v[i].row and p.col == v[i].col)
			return i;
	}
	return -1;
}