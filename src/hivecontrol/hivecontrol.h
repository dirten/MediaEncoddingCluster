#ifndef HIVECONTROL_H
#define HIVECONTROL_H


class HiveControl{
    private:
	bool isRunning;
    public:
	HiveControl();
//	static HiveControl *instance;
	~HiveControl();
	bool startup(void);
	bool shutdown(void);
	static HiveControl* getInstance();
};


#endif



