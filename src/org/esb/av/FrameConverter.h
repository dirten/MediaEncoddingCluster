#ifndef ORG_ESB_AV_FRAMECONVERTER_H
#define ORG_ESB_AV_FRAMECONVERTER_H


namespace org{
namespace esb{
namespace av{
class FrameConverter{
	public:
		FrameConverter(FrameFormat& targetFormat);
		Frame convert(Frame & input);

};
}}}
#endif


