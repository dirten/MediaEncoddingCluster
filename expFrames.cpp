#include <highgui.h>
#include "xml.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include "features.h"
#include <fcntl.h>
#include <avformat.h>

inline bool GetNextFrame (AVFormatContext*, AVCodecContext*, int, AVFrame*);

using namespace std;
IplImage picto;
int num_shotIdfiles = 278;
long verbosity = 3;

int main() {

	DOMBuilder *parser = NULL;
	CVideoInfo** video_infos = NULL;

	AVFormatContext *pFormatCtx;

	int i;

	int shot_counter;
	int numBytes;


	//////////////
	//xml auslesen
	//////////////
	/*
	   DOMImplementation *impl = NULL;
	   if(!initializeXML(&impl, parser))
	   {
	   return false;
	   } */

	XERCES_STD_QUALIFIER ifstream fin;

	// the input is a list file
	//XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc = 0;
	//char* xmlFile = new char[255];

	for( int counter = 0; counter < 278; counter++ )
		video_infos[counter] = NULL;


	video_infos = new CVideoInfo*[278];
	if ( !InitializeProgram (video_infos, &parser, "mpeg7"))
	{	fprintf (stderr, "Could not initialize xml subprogram. Exiting.\n");
		return 1;
	}


	for( int counter = 1; counter < 278; counter++ )
	{
		shot_counter=0;
		///////////////////////////////////////////////////////////////
		//Formel, um aus dem MediaTimePoint die Framenummer zu bekommen 
		/*
		   framenumber = (int)( totalseconds * 30000 + numberOfFractions) / 1001;
		   e.g. PT16S4484N30000F   or   T00:00:16:4484F30000
		   totalseconds = 16
		   numberOfFractions = 4484;
		   framenumber = (16 * 30000 + 4484) / 1001 = 484
		 */

		int framenum=0;

		int cf = 0;
		while(video_infos[counter]->shots[shot_counter]->time_point_second){

			int prevframenum;

			//(void)sscanf (video_infos[counter]->shots[shot_counter]->time_point_frickel_second, "%dF%*s", &sec);
			prevframenum=framenum;
			framenum = (int)((video_infos[counter]->shots[shot_counter]->time_point_second*30000)+video_infos[counter]->shots[shot_counter]->time_point_frickel_second)/1001;
			framenum -= 20;

			//Videoname = video_infos[i].video_name;




			/////////////////
			//videos einlesen
			/////////////////

			int             videoStream;
			AVCodecContext  *pCodecCtx;
			AVCodec         *pCodec;
			AVFrame         *pFrame; 
			AVFrame         *pFrameRGB;
			uint8_t         *buffer;
			CvSize size;
			if (framenum < prevframenum){
				cf = 0;

				av_open_input_file(&pFormatCtx, video_infos[counter]->video_name, NULL, 0, NULL);
				av_find_stream_info(pFormatCtx);
				videoStream = -1;
				for(i = 0; i < pFormatCtx->nb_streams; i++)
					if(pFormatCtx->streams[i]->codec->codec_type==CODEC_TYPE_VIDEO)
					{   videoStream=i;
						break;
					}
				pCodecCtx = pFormatCtx->streams[videoStream]->codec;
				pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
				if(pCodec->capabilities & CODEC_CAP_TRUNCATED)
					pCodecCtx->flags|=CODEC_FLAG_TRUNCATED;
				avcodec_open(pCodecCtx, pCodec);
				pFrame = avcodec_alloc_frame();
				pFrameRGB = avcodec_alloc_frame();
				numBytes = avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,pCodecCtx->height);
				buffer = new uint8_t[numBytes];
				memset(buffer, 0, numBytes);
				avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,pCodecCtx->width, pCodecCtx->height);
				size = cvSize(pCodecCtx->width,pCodecCtx->height);

				//////////////////////
				//Ende videos einlesen
				//////////////////////        
			}
			////////////////////
			//Frames extrahieren
			////////////////////

			while(GetNextFrame(pFormatCtx, pCodecCtx, videoStream, pFrame))
			{
				cf++;
				if(cf != framenum)
					continue;

				/* iplimage draus machen */
				img_convert((AVPicture *)pFrameRGB, PIX_FMT_RGB24,(AVPicture*)pFrame, pCodecCtx->pix_fmt,pCodecCtx->width,pCodecCtx->height);
				cvInitImageHeader(&picto, size, IPL_DEPTH_8U, 3, IPL_ORIGIN_TL, 4);
				picto.widthStep = pFrameRGB->linesize[0];
				cvSetData(&picto, pFrameRGB->data[0],pFrameRGB->linesize[0]);
				cvCvtColor(&picto, &picto, CV_BGR2RGB);
				cvSmooth(&picto, &picto, CV_BLUR,1);




				//save Frames    
				char szFilename[96];
				sprintf(szFilename, "%s_b.jpg", video_infos[counter]->shots[shot_counter]->segment_id);
				cvSaveImage(szFilename, &picto);
				shot_counter++;




				/////////////////////////
				//Ende Frames extrahieren
				/////////////////////////


			}
		}
		//Ende der Schleife 
		//while(video_infos[counter].shots[shot_counter].time_point_second != NULL)


		////////////////////////////////////////////
		//Ende der Schleife mp7-xml-dateien einlesen
		////////////////////////////////////////////


		// clean up
		//(*parser)->resetDocumentPool();
		CleanUpProgram (parser, video_infos);
		///////////////////
		//ende xml auslesen
		///////////////////

	}








	//video_infos[i].shots[o].time_point_hour
	//video_infos[i].shots[o].time_point_minute
	//video_infos[i].shots[o].time_point_second







	return 0;


}

inline bool /* getnextframe */
GetNextFrame (AVFormatContext *pFormatCtx, AVCodecContext *pCodecCtx,
		int videoStream, AVFrame *pFrame)
{
	static AVPacket packet;
	static int		bytesRemaining = 0;
	static uint8_t	*rawData;
	static bool		fFirstTime = true;
	int				bytesDecoded;
	int				frameFinished;
	if (fFirstTime)
	{	fFirstTime = false;
		packet.data = NULL;
	}
	while (true)
	{	while (bytesRemaining > 0)
		{	bytesDecoded = avcodec_decode_video (pCodecCtx, pFrame, &frameFinished, rawData, bytesRemaining);
			if (bytesDecoded < 0)
			{	perror ("Error while decoding frame");
				return false;
			}
			bytesRemaining -= bytesDecoded;
			rawData += bytesDecoded;
			if (frameFinished)
				return true;
		}
		do
		{	if (packet.data != NULL)
				av_free_packet (&packet);
			if (av_read_packet (pFormatCtx, &packet) < 0)
				goto loop_exit;
		} while (packet.stream_index != videoStream);
		bytesRemaining = packet.size;
		rawData = packet.data;
	}
loop_exit:
	bytesDecoded = avcodec_decode_video (pCodecCtx, pFrame, &frameFinished, rawData, bytesRemaining);
	if (bytesDecoded <= 0)
		return false;
	if (packet.data != NULL)
		av_free_packet (&packet);
	return frameFinished != 0;
}
