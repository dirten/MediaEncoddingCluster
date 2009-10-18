extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <libavutil/fifo.h>
}
#include <iostream>
int main(int argc, char ** argv){
	av_register_all();
	if(argc==1){
		return 0;
	}
	AVFormatContext * formatCtx= avformat_alloc_context();
	AVInputFormat *file_iformat = av_find_input_format("mpeg2ts");
	AVFormatParameters params, *ap = &params;
	memset(ap, 0, sizeof(*ap));
	ap->prealloced_context = 1;

	char * filename=argv[1];
	if (av_open_input_file(&formatCtx, filename, file_iformat, 0, ap) != 0) {
		std::cout<<"could not open file" << filename <<std::endl;
		return 0;
	}
	if (av_find_stream_info(formatCtx) < 0) {
		std::cout<<"could not read stream info " << filename <<std::endl;
		return 0;
	}
	while(true){
		AVPacket pkt;
		int ret= av_read_frame(formatCtx, &pkt);
		if(pkt.stream_index==1){
			std::cout <<pkt.stream_index;
		}
	}

}