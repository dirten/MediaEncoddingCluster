#ifndef HIVECONTROL_H
#define HIVECONTROL_H


class HiveControl{
    private:
	bool isRunning;
    public:
	HiveControl();
//	static HiveControl *instance;
	~HiveControl();
	void startup(void);
	void shutdown(void);
	static HiveControl* getInstance();
};


#endif



