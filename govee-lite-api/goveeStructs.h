#pragma once

#include <string>
#include <vector>

struct GoveeDeviceRange {
	int min;
	int max;
};

struct GoveeColorTem {
	GoveeDeviceRange range;
};

struct GoveeDeviceProperties {
	GoveeColorTem colorTem;
};

struct GoveeDevice {
	std::string device;
	std::string model;
	std::string deviceName;
	bool controllable;
	bool retrievable;
	std::vector<std::string> supportCmds;
	GoveeDeviceProperties properties;
};