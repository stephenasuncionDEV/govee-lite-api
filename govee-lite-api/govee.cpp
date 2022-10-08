#include "govee.h"

Govee::Govee(const std::string& INPUT_API_KEY) {
	API_KEY = INPUT_API_KEY;
}

Govee::GoveeDeviceObj::GoveeDeviceObj(const std::string & INPUT_API_KEY, GoveeDevice device) {
	API_KEY = INPUT_API_KEY;
	info = device;
}

std::vector<Govee::GoveeDeviceObj> Govee::getDevices(bool log) {
	auto res = getReq("/devices");
	auto jsonData = json::parse(res);
	auto jsonDevices = jsonData["data"]["devices"];
	auto deviceArr = jsonDevices.get<std::vector<GoveeDevice>>();

	/*std::cout << jsonDevices.dump(4) << std::endl;*/

	std::vector<GoveeDeviceObj> ret;

	for (const auto& device : deviceArr) {
		auto deviceObj = new GoveeDeviceObj(API_KEY, device);
		ret.push_back(*deviceObj);

		if (!log) continue;

		std::cout << "Device: " << device.deviceName << std::endl;
	}

	return ret;
}

bool Govee::GoveeDeviceObj::turnOn() {
	std::string data =
		"{\"device\":\"" + this->info.device +
		"\",\"model\":\"" + this->info.model +
		"\",\"cmd\":{\"name\":\"turn\",\"value\":\"on\"}}";

	std::cout << data << std::endl;

	auto res = postReq("/devices/control", data);

	auto jsonData = json::parse(res);
	auto statusCode = jsonData["code"].get<int>();
	if (statusCode == 200) return true;
	return false;
}

bool Govee::GoveeDeviceObj::turnOff() {
	std::string data = 
		"{\"device\":\"" + this->info.device +
		"\",\"model\":\"" + this->info.model +
		"\",\"cmd\":{\"name\":\"turn\",\"value\":\"off\"}}";

	std::cout << data << std::endl;

	auto res = postReq("/devices/control", data);

	auto jsonData = json::parse(res);
	auto statusCode = jsonData["code"].get<int>();
	if (statusCode == 200) return true;
	return false;
}

std::string Govee::getReq(const std::string& endpoint) {
	CURL* curl = curl_easy_init();
	if (!curl) {
		return "null";
	}

	std::string ret;

	std::string URL = "https://developer-api.govee.com/v1" + endpoint;
	std::string HEADER = "Govee-API-Key: " + API_KEY;

	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, HEADER.c_str());

	curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);

	CURLcode code = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	if (code != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(code));
		return "null";
	}

	return ret;
}

std::string Govee::GoveeDeviceObj::postReq(const std::string& endpoint, const std::string& data) {
	CURL* curl = curl_easy_init();
	if (!curl) {
		return "null";
	}

	std::string ret;

	std::string URL = "https://developer-api.govee.com/v1" + endpoint;
	std::string HEADER = "Govee-API-Key: " + API_KEY;

	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, HEADER.c_str());

	curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.length());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);

	CURLcode code = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	if (code != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(code));
		return "null";
	}

	return ret;
}