#include "SpriteData.h"


const char* strLB[2][1] = {
	("End of State"),
	("End of Frame")
};

const char* strToolMgtBtn[2] = { "SAVE", "LOAD" };

const char* strStateName[6] = {
	"Idle", "Run", "Attack1", "Attack2", "Hurt", "Dead"
};

const char* strUpDown[2] = { "▲", "▼" };

const char* strDataMgtBtn[3] = { "ADD", "DEL", "UDT" };

const char* strPosDelta[3] = { "x", "y", "delta" };

const char* strRt[2][5] = { 
	{"attCol", "x", "y", "width", "height"},
	{"defCol", "x", "y", "width", "height"}
};

const char* strStateBtn[3] = { "ADD", "DEL", "UDT" };
 