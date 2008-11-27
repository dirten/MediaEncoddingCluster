#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"





using namespace org::esb::av;
int main(int argc, char ** argv){

        if(argc<2)return 1;
        int packet_start=0;
        int packet_count=50;
        if(argc>2)
            packet_start=atoi(argv[2]);
        if(argc>3)
            packet_count=atoi(argv[3]);
        File file(argv[1]);
        FormatInputStream fis(&file);
        fis.dumpFormat();
      AVFormatContext * f=fis.getFormatContext();
      
      cout << endl;
      cout <<"<File Information>"<<endl;
//      cout <<"-----------------"<<endl;
      cout << "#streams\tformatname\ttimestamp\tstart\tduration\tfilename"<<endl;
      cout << "---------------------------------------------------------------------------------"<<endl;
      cout <<f->nb_streams<<"\t\t";
      cout <<f->iformat->name<<"\t\t";
      cout <<f->timestamp<<"\t\t";
      cout <<f->start_time<<"\t";
      cout <<f->duration<<"\t";
      cout <<f->filename<<"\t";
      
      cout << endl;
        int streams=fis.getStreamCount();
        cout <<endl;
        cout <<"<Stream Information>"<<endl;
        cout << "#\tindex\tcodec\tnum\tden\tquality\tstart\tduration"<<endl;
        cout <<"-------------------------------------------------------------------------"<<endl;
        for (int a=0;a<streams;a++){
                AVStream * s=fis.getFormatContext()->streams[a];
          cout << a<<"\t";
          cout << s->index<<"\t";
          cout << s->codec->codec_id<<"\t";
          cout << s->time_base.num<<"\t";
          cout << s->time_base.den<<"\t";
          cout << s->quality<<"\t";
          cout << s->start_time<<"\t";
          cout << s->duration<<"\t";
          cout << endl;
        }


        cout <<endl;
        cout <<"<Codec Information>"<<endl;
        cout << "#\tindex\tcodec\tnum\tden\tquality\tduration"<<endl;
        cout <<"-------------------------------------------------------------------------"<<endl;
        for (int a=0;a<streams;a++){
                AVStream * s=fis.getFormatContext()->streams[a];
          cout << a<<"\t";
          cout << s->index<<"\t";
          cout << s->codec->codec_id<<"\t";
          cout << s->codec->time_base.num<<"\t";
          cout << s->codec->time_base.den<<"\t";
//          cout << s->codec->quality<<"\t";
//          cout << s->duration<<"\t";
          cout << endl;
        }

      PacketInputStream pis(&fis);
      cout<<endl;
      cout <<"<Packet Information>"<<endl;
      cout << "#\tpts\tdts\tsize\tindex\tflags\tiskey\tduration\tpos"<<endl;
      cout <<"------------------------------------------------------------------------"<<endl;
      for(int a=0;a<packet_start+packet_count;a++){
        if(a<packet_start)continue;
        Packet p;
        pis.readPacket(p);
        cout <<a<<"\t";
        cout <<p.packet->pts<<"\t";
        cout <<p.packet->dts<<"\t";
        cout <<p.packet->size<<"\t";
        cout <<p.packet->stream_index<<"\t";
        cout <<p.packet->flags<<"\t";
        cout <<p.isKeyFrame()<<"\t";
        cout <<p.packet->duration<<"\t\t";
        cout <<p.packet->pos<<"\t";
        cout << endl;      
      }

        cout << endl;      
        cout << endl;      

return 0;
}

