#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>
#include "goveeStructs.h"

#define CURL_STATICLIB
#include "curl/curl.h"

#pragma comment (lib, "libcurl_a.lib")
#pragma comment (lib, "Normaliz.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Wldap32.lib")
#pragma comment (lib, "advapi32.lib")
#pragma comment (lib, "Crypt32.lib")
#pragma comment (lib, "Wtsapi32.lib")
#pragma comment (lib, "Iphlpapi.lib")

using nlohmann::json;

class Govee {
protected:
	std::string API_KEY;
	std::string getReq(const std::string& endpoint);

private:
	static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
		((std::string*)userp)->append((char*)contents, size * nmemb);
		return size * nmemb;
	}

public:
	Govee(const std::string& INPUT_API_KEY);

	class GoveeDeviceObj {
	private:
		std::string API_KEY;
		GoveeDevice info;
		std::string postReq(const std::string& endpoint, const std::string& data);
	public:
		GoveeDeviceObj(const std::string& INPUT_API_KEY, GoveeDevice device);

		bool turnOn();
		bool turnOff();
	};
	
	std::vector<GoveeDeviceObj> getDevices(bool log = false);
};

inline void from_json(const json& j, GoveeDeviceRange& r)
{
	j.at("min").get_to(r.min);
	j.at("max").get_to(r.max);
}

inline void from_json(const json& j, GoveeColorTem& c)
{
	j.at("range").get_to(c.range);
}

inline void from_json(const json& j, GoveeDeviceProperties& p)
{
	j.at("colorTem").get_to(p.colorTem);
}

inline void from_json(const json& j, GoveeDevice& d)
{
	j.at("device").get_to(d.device);
	j.at("model").get_to(d.model);
	j.at("deviceName").get_to(d.deviceName);
	j.at("controllable").get_to(d.controllable);
	j.at("retrievable").get_to(d.retrievable);

	if (j.find("supportCmds") != j.end()) {
		j.at("supportCmds").get_to(d.supportCmds);
	}
	
	j.at("properties").get_to(d.properties);
}

inline void to_json(json& j, const GoveeDevice& d)
{
	j = json{
		{ "device", d.device },
		{ "model", d.model },
		{ "deviceName", d.deviceName },
		{ "controllable", d.controllable },
		{ "retrievable", d.retrievable },
		{ "supportCmds", d.supportCmds },
		{ "properties", {
			{ "colorTem", {
				{ "range", {
					{ "min", d.properties.colorTem.range.min },
					{ "max", d.properties.colorTem.range.max }
							}
						}
					}
				}
			}
		}
	};
}