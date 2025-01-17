#pragma once

#include "RadarData.h"
#include "RadarDataHolder.h"
#include "AsyncTask.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

class AsyncPollFilesTask;

class RadarCollection{
public:
	
	
	// event emitted when new current data is available
	class RadarUpdateEvent{
	public:
		// radar data for event
		RadarData * data;
		// the holder for the product
		//RadarDataHolder::ProductHolder* productHolder;
		// time to use for animations if not zero
		float minTimeTillNext = 0;
	};
	
	
	
	// settings for radar data
	RadarDataSettings radarDataSettings = {};
	
	
	// TODO: add a maximum number of current async tasks
	int maxLoading = 100;
	
	// should the position be animated
	bool automaticallyAdvance = true;
	
	// time between automatic advances
	float autoAdvanceInterval = 1;
	
	// should the data be polled for changes
	bool poll = true;
	
	// time between polling data
	float pollInterval = 1;
	
	// if it should print debug data
	bool verbose = false;
	
	RadarCollection();
	
	~RadarCollection();
	
	// allocate data
	void Allocate(int cacheSize);
	
	// free all allocated data
	void Free();
	
	// Clears radar data
	void Clear();
	
	// read file info from a directory
	void ReadFiles(std::string path);
	
	// get unix timestamp from the name of a file
	static double ParseFileNameDate(std::string filename);
	
	// poll current directory for updates
	void PollFiles();
	
	// reload file at index, -1 for current
	void ReloadFile(int index);
	
	// moves the curent position
	void Move(int delta);
	
	// initiates a move for the user
	void MoveManual(int delta);
	
	// set the radar product
	void ChangeProduct(RadarData::VolumeType volumeType);
	
	// this function should be called regularly on the main thread
	void EventLoop();
	
	// register a callback for when radar data
	void RegisterListener(std::function<void(RadarUpdateEvent)> callback);
	
	// get the radar data holder, may not be loaded
	RadarDataHolder* GetCurrentRadarData();
	
	// visually describe the state of the cache
	std::string StateString();
	
	// return the state of the cache
	std::vector<RadarDataHolder::State> StateVector();
	
	// gets current position buffer
	int GetCurrentPosition();
	
	// visually describe the state of the cache in the console
	void LogState();
	
	static void Testing();
	
	
private:	
	// location of directory to load from
	std::string filePath = "";
	
	// file to initially start on when loading directory
	std::string defaultFileName = "";

	// true if allocate has been called
	bool allocated = false;

	// holds currently laoded radar data
	RadarDataHolder* cache = NULL;
	
	// size of cache, expected to be even with one spot for the current data and one empty spot at all times, should be a minimum of 4
	int cacheSize = 0;
	
	// size of one side of cache when balanced excluding curent pos and empty spot
	int cacheSizeSide = 0;
	
	// number of items that should be reserved in either direction from the current position to allow loading new data
	// if the separation position is less than this value away from the current position than the other side is freed to make room the expand the close side
	// this allows new data to load in as the current position moves
	// set based on allocation size
	int reservedCacheSize = 0;
	
	// minimum value for reservedCacheSize
	int reservedCacheSizeMin = 0;
	
	// maximum value for reservedCacheSize
	int reservedCacheSizeMax = 0;
	
	// how fast resevered size can change
	float reservedCacheChangePerSecond = 2.0f;
	
	// next time to change the reserved size
	double nextReservedCacheChangeTime = 0;
	
	// desired number of spaces between the current position and the closest loading position, used for changing reserved size
	int desiredLoadingDistance = 0;
	
	// if the desired loading distance has remained free of loading
	bool isDesiredLoadingDistanceClear = true;
	
	// current position in the cache buffer
	int currentPosition = 0;
	
	// number of items cached before the current position
	int cachedBefore = 0;
	
	// number of items cached after the current position
	int cachedAfter = 0;
	
	// 1 for forward -1 for backward, the given direction will be prioritized for loading and is the direction of auto advance
	int lastMoveDirection = 1;
	
	// if radar data needs to be emitted to listeners when it is loaded
	bool needToEmit = true;
	
	// info about locations of files in the directory and time
	float firstItemTime = -1;
	int firstItemIndex = -1;
	float lastItemTime = -1;
	int lastItemIndex = -1;
	
	// next system time to advance
	double nextAdvanceTime = 0;
	
	// next system time to poll
	double nextPollTime = 0;
	
	// list of files
	std::vector<RadarFile> radarFiles = {};
	
	// callbacks to emit to
	std::vector<std::function<void(RadarUpdateEvent)>> listeners = {};
	
	// unload old data to make room for new data
	void UnloadOldData();
	
	// load in new data
	void LoadNewData();
	
	// emit radar data to all listeners
	void Emit(RadarDataHolder* holder);
	
	// finish polling data on main thread after poll task has completed
	void PollFilesFinalize();
	
	AsyncPollFilesTask* pollFilesTask = NULL;
	
	int runs = 0;
};