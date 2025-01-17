#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>

class Globe;
class RadarCollection;

class GlobalState{
public:
	
	class Waypoint{
	public:
		double latitude = 0;
		double longitude = 0;
		double altitude = 0;
		int colorR = 255;
		int colorG = 255;
		int colorB = 255;
		std::string name;
		bool enabled = true;
	};
	
	enum ViewMode {
		VIEW_MODE_VOLUMETRIC = 0,
		VIEW_MODE_SLICE = 1,
	};
	
	enum SliceMode {
		SLICE_MODE_SWEEP_ANGLE = 0,
		SLICE_MODE_CONSTANT_ALTITUDE = 1,
		SLICE_MODE_VERTICAL = 2,
	};

	bool animate = false; // animate the scene
	bool animateCutoff = false; // animate the cutoff
	bool temporalInterpolation = true; // interpolate data over time when animating
	bool spatialInterpolation = true; // interpolate data over time when animating
	bool isMouseCaptured = false; // true if mouse is currently captured
	bool vrMode = false; // true if in vr
	bool pollData = true; // should the data be polled for updates
	bool devShowCacheState = false; // if the state of the cache buffer should be displayed on screen
	bool devShowImGui = false; // if it is safe to show ImGui debuging windows
	bool developmentMode = false; // show development features
	bool vsync = true; // enable vsync
	bool enableFuzz = true; // add noise to shader to achieve a dithering effect
	bool temporalAntiAliasing = false; // temporal anti aliasing can smooth out fuzz, it is used in the pawn
	bool audioControlledHeight = false;
	bool audioControlledOpacity = false;
	bool audioControlledCutoff = false;
	
	float maxFPS = 60.0f; // maximum frames per second
	float animateSpeed = 5.0f; // speed of animation
	float animateCutoffSpeed = 0.2f; // speed of animation
	float cutoff = 0.0f; // how much of the radar intensity range to hide starting at the lowest
	float opacityMultiplier = 1.0f; // how opaque the radar volume is
	float fadeSpeed = 0.0f; // speed of fade
	float moveSpeed = 300.0f; // speed of movement
	float rotateSpeed = 200.0f; // speed of rotation
	float guiScale = 1.0f; // scale of gui for high dpi displays
	float quality = 0.0f; // a float representing quality with 0 being normal
	float qualityCustomStepSize = 5.0f; // if quality is set to 10 this value is used
	float verticalScale = 1.0f; // multiply vertical scale
	float audioControlMultiplier = 5.0f;
	bool enableMap = true; // if the globe should be rendered
	bool enableMapTiles = true; // if tiles should be shown
	bool enableMapGIS = true; // if GIS objects should be shown
	float mapBrightness = 0.2f; // brightness of map texture
	float mapBrightnessGIS = 0.5f; // GIS brightness relative to map brightness
	
	int volumeType = 1; // type of radar product RadarData::VolumeType, defaults to 1 which is reflectivity
	
	ViewMode viewMode = VIEW_MODE_VOLUMETRIC; // what mode should be used to display the radar data in space
	SliceMode sliceMode = SLICE_MODE_SWEEP_ANGLE; // type of slice to do
	float sliceAltitude = 5000; // height above sea level to slice
	float sliceAngle = 0.5; // angle of slice in degrees to slice
	float sliceVerticalLocationX = 0; // location of vertical slice
	float sliceVerticalLocationY = 0; // location of vertical slice
	float sliceVerticalRotation = 0; // rotation of vertical slice
	bool sliceVolumetric = false; // if the slice should be volumetric instead of flat
	
	std::vector<Waypoint> locationMarkers = {};
	
	// globe that is lined up with radar data
	Globe* globe;
	
	// copy of global state with default values
	GlobalState* defaults = NULL;
	
	// this section contains pointers to objects within the application
	// using these should be avoided where possible and always check that they are not null
	RadarCollection* refRadarCollection;
	
	// register a callback for the given event name. returns a uid to remove callback
	uint64_t RegisterEvent(std::string name, std::function<void(std::string, void*)> callback);
	// remove callback
	void UnregisterEvent(uint64_t uid);
	// emit an event to all registered callbacks
	void EmitEvent(std::string name, std::string stringData, void* extraData);
	void EmitEvent(std::string name);
	
	
	//Testing
	float testFloat = 1; // test float
	bool testBool = false;
	void test(); // test fuction
	
	GlobalState();
	~GlobalState();
	
private:
	std::unordered_map<std::string, std::unordered_map<uint64_t, std::function<void(std::string, void*)>>> callbacks = {};
	std::unordered_map<uint64_t, std::string> callbacksUidNames = {};
	uint64_t callbackUid = 1;
	// constructor to prevent infinite recursion through defaults
	GlobalState(bool doNotInitDefaults);
};