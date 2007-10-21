#include "FormatInputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/lang/Exception.h"
extern "C" {
#include "avformat.h"
}

#include <iostream>
using namespace std;

using namespace org::esb::io;
using namespace org::esb::lang;
namespace org {
    namespace esb {
        namespace av {
            FormatInputStream::FormatInputStream(File * source) {
                _sourceFile=source;
                // Open video file                
                if(av_open_input_file(&formatCtx, _sourceFile->getPath(), NULL, 0, NULL)!=0){
//    		    throw Exception(__FILE__, __LINE__, "FormatInputStream<init> - could not open File");
                    cout << "Konnte Datei " << _sourceFile->getPath() << " nicht oeffnen" <<endl;
                }

                if(av_find_stream_info(formatCtx)<0){
                    cout << "Konnte StreamInfo von " << _sourceFile->getPath() << " nicht ermitteln" <<endl;
                }
//                dump_format(formatCtx, 0, _sourceFile->getPath(), false);
            }

            AVFormatContext * FormatInputStream::getFormatContext() {
                return formatCtx;
            }
          FormatInputStream::~FormatInputStream() {
        		close();
          }

            int FormatInputStream::getStreamCount(){
                return formatCtx->nb_streams;
            }

            InputStream * FormatInputStream::getStream(int streamIndex){
                selectedStream=streamIndex;
                return this;
            }

	    int FormatInputStream::available(bool withBlocking){
	        return 0;
	    }
            long FormatInputStream::getFileSize(){
                return formatCtx->file_size;
            }

            int FormatInputStream::read() {
                return 0;
            }
            int FormatInputStream::read(unsigned char * buffer, int length) {
                return 0;
            }

            int FormatInputStream::read(vector<unsigned char>&buffer) {
                return 0;
            }

            void FormatInputStream::close() {
                av_close_input_file(formatCtx);
            }
        }
    }
}


