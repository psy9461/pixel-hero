#pragma once

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;

typedef void (*VOID_METHOD)();
typedef void (*FLOAT_METHOD)(float);

struct Texture
{
	uint32 texID;// GLuint texID;
	float width, height;
	float potWidth, potHeight;
	int retainCount;
};

enum iKeyStat
{
	iKeyStatBegan = 0,
	iKeyStatMoved,
	iKeyStatEnded,
};
struct iPoint;
typedef void (*KEY_METHOD)(iKeyStat, iPoint);

struct iSize;
struct iRect;

#define keysA		1	// 0000 0001  left
#define keysD		2	// 0000 0010  right
#define keysW		4	// 0000 0100  up
#define keysS		8	// 0000 1000  down
#define keysSpace	16	// 0001 0000  space
#define keysM		32

#define keysLeft	64	
#define keysRight	128	
#define keysUp		256	
#define keysDown	512 
#define keysESC		1024 
#define keysCTRL	2048
#define keysALT		4096
#define keysENT		8192

// anc
#define LEFT		 1// 0000 0001
#define RIGHT		 2// 0000 0010
#define HCENTER		 4// 0000 0100

#define TOP			 8// 0000 1000
#define BOTTOM		16// 0001 0000
#define VCENTER		32// 0010 0000

#define REVERSE_NONE	0
#define REVERSE_WIDTH	1
#define REVERSE_HEIGHT	2
