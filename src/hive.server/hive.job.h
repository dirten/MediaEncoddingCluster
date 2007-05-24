#ifndef HIVE_JOB_H
#define HIVE_JOB_H
class HiveJob{
    public:
	void setInputFilename(char * filename);
	const char * getInputFilename();

	void setOutputFilename(char * filename);
	const char * getOutputFilename();

	void setOutputCodec(char * codec);
	const char * getOutputCodec();
	
	void setOutputFormat(char * format);
	const char * getOutputFormat();
};
#endif

