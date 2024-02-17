#include <iostream>
#include <Windows.h>
#include <regex>
#include "snap7.h"
#include "s7.h"
#include "LineNotify.h"

using std::cout;
using std::cin;

TS7Client* PLC_S7_Client;
byte Data[MAXBYTE]{};

struct PLC {
	std::string IP{};
	int DBnumber{};
	int DataADDR{};
	int DataLEN{};
};

bool Determine_IPv4(const std::string& str) {
	std::regex ip_regex("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
	return std::regex_match(str, ip_regex);
}

bool PLC_connect(std::string PLCIP) {
	PLC_S7_Client = new TS7Client;
	int ErrorCode = PLC_S7_Client->ConnectTo(PLCIP.c_str(), 0, 1);
	if (ErrorCode == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void PLC_disconnect() {
	PLC_S7_Client->Disconnect();
	delete PLC_S7_Client;
}

void PLC_ReadDB(PLC plc) {
	PLC_S7_Client->DBRead(plc.DBnumber, plc.DataADDR, plc.DataLEN, &Data);
}

int main() {
	PLC PLC_1;
	int PLCdata = 0;
	int mPLCdata = 0;

	// Enter IP
	cout << "Enter the PLC IP address: ";
	cin >> PLC_1.IP;
	if (!Determine_IPv4(PLC_1.IP)) {
		cout << "IPv4 Format Error.\n";
		system("pause");
		return 0;
	}

	// connect
	if (PLC_connect(PLC_1.IP)) {
		cout << "Connect to PLC:" << PLC_1.IP << " Successfully!\n";
	}
	else {
		cout << "Connect to PLC:" << PLC_1.IP << " Fail.\n";
		system("pause");
		return 0;
	}

	// Enter DB
	cout << "Enter DB number: ";
	cin >> PLC_1.DBnumber;
	cout << "Enter DB DataADDR: ";
	cin >> PLC_1.DataADDR;
	cout << "Enter DB DataLEN: ";
	cin >> PLC_1.DataLEN;

	PLC_ReadDB(PLC_1);
	mPLCdata = S7_GetIntAt(Data, 0);

	// if 
	while (true) {
		PLC_ReadDB(PLC_1);
		PLCdata = S7_GetIntAt(Data, 0);
		if (mPLCdata != PLCdata) {
			cout << "It was found that the data changed, from " << mPLCdata << " to " << PLCdata << '\n';
			Send_LineNotify("It was found that the data changed, from " + std::to_string(mPLCdata) + " to " + std::to_string(PLCdata));
			mPLCdata = PLCdata;
		}
		Sleep(1000);
	}
	

	// disconnect
	PLC_disconnect();

	system("pause");
	return 0;
}