#include <iostream>
#include "govee.h"

int main() {
	std::cout << "govee-lite-api by Stephen Asuncion" << std::endl;

	auto govee = new Govee("YOUR GOVEE API KEY");
	auto devices = govee->getDevices(true);

	devices[0].turnOff();

	system("Pause");
	return 0;
}