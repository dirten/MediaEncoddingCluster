#ifndef JOBDETAIL_H_
#define JOBDETAIL_H_


namespace org{
	namespace esb{
		namespace hive{
			namespace job{ 
				class JobDetail{
					public:
						JobDetail();
						int getId();
						int getSourceStream();
						int getTargetStream();	
						void setId(int);
						void setSourceStream(int);
						void setTargetStream(int);	
					private:
						int _id;
						int _source;
						int _target;
				};
			}
		}
	}
}
#endif /*JOBDETAIL_H_*/
