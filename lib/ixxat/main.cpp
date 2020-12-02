
#include <chrono>
#include "pch.h"
#include "Ixxat.h"
#include "libixxat.h"

void Message(const std::string &txt)
{
	std::cout << txt << std::endl;
}

int main()
{
	int nbDev = ixxat_enumerate();

	if (nbDev > 0)
	{
		char buffer[100];
		ixxat_get_name(0, buffer, 100);

		std::string dev_name(buffer);

		if (ixxat_init(dev_name.c_str(), "125K"))
		{
			Message("init ixxat OK!");

			ixxat_enumerate(); // test if we can still enumerate while a device is in use
			unsigned long id;
			char data_read[8] = { 0 };
			char data[8] = { 9, 10, 11, 12, 13, 14, 15, 16 };

			for (int i = 0; i < 100; i++)
			{
				ixxat_send(dev_name.c_str(), 0x42, data, 8);
				std::this_thread::sleep_for(std::chrono::seconds(1));
				ixxat_read(dev_name.c_str(), &id, &data_read[0]);
			}
			getchar();

			ixxat_stop(dev_name.c_str());
		}
		else
		{
			Message("Cannot init ixxat");
		}
	}
	else
	{
		Message("Cannot find any IXXAT device");
	}
}

