#include "FormatInputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/lang/Exception.h"

#include <iostream>
#include <boost/cast.hpp>
using namespace std;

using namespace org::esb::io;
using namespace org::esb::lang;
namespace org {
    namespace esb {
        namespace av {

            FormatInputStream::FormatInputStream(File * source) {
				_isValid=false;
                _sourceFile = source;
                //                formatCtx= av_alloc_format_context();
                logdebug("openning MediaFile:" << source->getPath());
                if (av_open_input_file(&formatCtx, _sourceFile->getPath().c_str(), NULL, 0, NULL) != 0) {
//                    throw Exception("FormatInputStream<init> - could not open File");
                    logerror("Konnte Datei " << _sourceFile->getPath() << " nicht oeffnen");
					return;
                }
                if (av_find_stream_info(formatCtx) < 0) {
                    logerror("Konnte StreamInfo von " << _sourceFile->getPath() << " nicht ermitteln");
					return;
				}
				_isValid=true;
            }

			bool FormatInputStream::isValid(){return _isValid;}

			AVFormatContext * FormatInputStream::getFormatContext() {
                return formatCtx;
            }

            void FormatInputStream::dumpFormat() {
                dump_format(formatCtx, 0, _sourceFile->getPath().c_str(), false);
            }

            FormatInputStream::~FormatInputStream() {
                close();
            }

            int FormatInputStream::getStreamCount() {
                return formatCtx->nb_streams;
            }

            InputStream * FormatInputStream::getStream(int streamIndex) {
                selectedStream = streamIndex;
                return this;
            }

            AVInputStream * FormatInputStream::getAVStream(int streamIndex) {
                //		AVInputStream * str=(AVInputStream*)formatCtx->streams[streamIndex];
                return (AVInputStream*) formatCtx->streams[streamIndex];
            }

            long long int FormatInputStream::available(bool withBlocking) {
                return 0;
            }

            long long int FormatInputStream::getFileSize() {
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

            int FormatInputStream::seek(int stream_index, int64_t timestamp) {
                return av_seek_frame(formatCtx, stream_index, timestamp, AVSEEK_FLAG_ANY);
                //return av_seek_frame(formatCtx, stream_index, timestamp,AVSEEK_FLAG_BACKWARD	);
            }

            void FormatInputStream::close() {
                if (formatCtx)
                    av_close_input_file(formatCtx);
            }
        }
    }
}


