#include "govee.h"

Govee::Govee(const std::string& INPUT_API_KEY) {
	API_KEY = INPUT_API_KEY;
}

std::string Govee::getDevices() {
	auto res = req("/devices");
	auto jsonData = json::parse(res);
	auto deviceArr = jsonData["data"]["devices"].get<std::vector<GoveeDevice>>();

	for (const auto& device : deviceArr) {
		std::cout << device.deviceName << std::endl;
	}


	return std::string("test");
}

std::string Govee::req(std::string endpoint) {
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