#include "ProcData.h"


const char* _sprPathChar[MAX_CHAR] = {
	"goblin",
	"kobold",
	"windwisp",
	"minotaur",
	"golem",
	"adventurer"	
};

const char* _sprPathProj[MAX_PROJ] = {
	"projectile-windwisp",
};

TileData _tileData[KIND_TILE] = {
	{
		"cave", 12,
		{
			"cave-corner", "cave-empty", "cave-empty2", "cave-inner-corner", "cave-platform",
			"cave-platform-corner-left", "cave-platform-corner-right", "cave-side", "cave-single-platform",
			"cave-slope1", "cave-slope2", "cave-top"
		}
	},

	{
		"desert", 15,
		{
			"desert-corner-bottom", "desert-corner-top", "desert-empty", "desert-inner-corner", "desert-platfom",
			"desert-platform-corner", "desert-single-platform", "desert-slope1", "desert-slope2", "desert-top",
			"desert-wall-empty", "desert-wall-empty2", "desert-wall-side", "desert-wall-slope", "desert-wall-top"
		}
	},

	{	"freeze", 11,
		{
			"freeze-bottom", "freeze-corner", "freeze-corner-bottom", "freeze-emtpy", "freeze-inner-corner",
			"freeze-side", "freeze-slope1", "freeze-slope2", "freeze-slope-bottom1", "freeze-slope-bottom2", "freeze-top"
		}
	},

	{
		"grass", 22,
		{
			"grass-bottom-inner-corner-left", "grass-bottom-inner-corner-right", "grass-empty", "grass-empty2", "grass-grass", "grass-grass2",
			"grass-platform", "grass-platform-corner-left", "grass-platform-corner-right", "grass-side-left", "grass-side-right", "grass-single-platform",
			"grass-slope1-left", "grass-slope1-right", "grass-slope2-left","grass-slope2-right", "grass-top", "grass-top-corner-left", "grass-top-corner-right",
			"grass-platfom-bottom", "grass-platform-corner-left", "grass-platform-corner-right"
		}
	},

	{
		"soil", 17,
		{
			"soil-cave-transition1","soil-cave-transition2","soil-corner-left","soil-corner-right","soil-inner-corner-left",
			"soil-inner-corner-right","soil-platform","soil-platorm-corner-left","soil-platorm-corner-right","soil-single-platform","soil-slope1-left",
			"soil-slope1-right", "soil-slope2-left", "soil-slope2-right", "soil-top", "soil-underwater-side", "soil-underwater-top"
		}
	},

	{
		"wall", 5,
		{
			"wall-empty","wall-empty2","wall-side","wall-slope","wall-top",
		}
	},
};


const char* pathEntity[MAX_CHAR] = { "assets/adventurer-profile.png", "assets/goblin-profile.png", "assets/kobold-profile.png",
									 "assets/windwisp-profile.png", "assets/minotaur-profile.png", "assets/golem-profile.png" };

const char* bgImgPath[1] = { "assets/background/sky-green.png" };



