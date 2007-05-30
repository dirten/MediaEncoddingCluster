#include "FormatInputStream.h"
#include "org/esb/io/File.h"
#include <iostream>
using namespace std;

using namespace org::esb::io;
namespace org {
    namespace esb {
        namespace av {
            FormatInputStream::FormatInputStream(File * source) {
                _sourceFile=source;
                // Open video file                
                
                if(av_open_input_file(&formatCtx, _sourceFile->getPath(), NULL, 0, NULL)!=0){
                    cout << "Konnte Datei " << _sourceFile->getPath() << " nicht oeffnen" <<endl;
                }

                if(av_find_stream_info(formatCtx)<0){
                    cout << "Konnte StreamInfo von " << _sourceFile->getPath() << " nicht ermitteln" <<endl;
                }
                
//                dump_format(formatCtx, 0, _sourceFile->getPath(), false);

            }

            FormatInputStream::~FormatInputStream() {
		close();
            }

            int FormatInputStream::getStreamCount(){
                return formatCtx->nb_streams;
            }

            AVInputStream * FormatInputStream::getStream(int streamIndex){
                return new AVInputStream(formatCtx->streams[streamIndex]);
            }

	    int FormatInputStream::available(bool withBlocking){
	    
	    }
            long FormatInputStream::getFileSize(){
                return formatCtx->file_size;
            }

            int FormatInputStream::read(unsigned char * buffer, int length) {
                return 0;
            }

            void FormatInputStream::close() {
                av_close_input_file(formatCtx);
            }
        }
    }
}


