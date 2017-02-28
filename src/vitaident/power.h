#ifndef _VITAIDENT_POWER_H_
#define _VITAIDENT_POWER_H_

typedef enum {
	ClockFrequencyType_Cpu = 0,
	ClockFrequencyType_Bus,
	ClockFrequencyType_Gpu,
	ClockFrequencyType_GpuXbar
} ClockFrequencyType;

int scePowerGetBatteryTemp();
int scePowerGetBatteryVolt();
int scePowerGetBatterySOH();
int scePowerGetBatteryCycleCount();
//int scePowerGetBatteryElec(); Crashes the VITA
int scePowerGetUsingWireless();

int scePowerGetGpuXbarClockFrequency();

int getClockFrequency(ClockFrequencyType type);
const char * getBatteryStatus();
char * getBatteryPercentage();
//char * getBatteryElec();
char * getUsingWireless();
char * getBatterySOH();
char * getBatteryCycleCount();
char * getBatteryCapacity();
char * getBatteryRemainCapacity();
char * getBatteryTemp(int type);
char * getBatteryVoltage();
char * getUdcdCableState();
char * getUdcdChargingState();

#endif