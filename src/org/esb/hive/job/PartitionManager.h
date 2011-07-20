/* 
 * File:   PartitionManager.h
 * Author: HoelscJ
 *
 * Created on 18. Juli 2011, 09:56
 */

#ifndef PARTITIONMANAGER_H
#define	PARTITIONMANAGER_H
namespace org {
  namespace esb {
    namespace hive {
      class PartitionManager {
      public:
        static PartitionManager getInstance();
      private:
        PartitionManager();
        virtual ~PartitionManager();
        static PartitionManager _instance;

      };
    }
  }
}

#endif	/* PARTITIONMANAGER_H */

