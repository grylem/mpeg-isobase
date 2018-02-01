#include <bitstream/omftag.h>
#include <mpeg-isobase/box/visual_sample_entry.h>


namespace mpeg {
namespace isobase {


void VisualSampleEntry::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    SampleEntry::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(pre_defined)  << "pre_defined";
    stream.tag(reserved)  << "reserved";
    stream.tag(pre_defined2)  << "pre_defined";
    stream.tag(width)  << "width";
    stream.tag(height)  << "height";
    stream.tag(horizresolution)  << "horizresolution";
    stream.tag(vertresolution)  << "vertresolution";
    stream.tag(reserved2)  << "reserved";
    stream.tag(frame_count)  << "frame_count";
    stream.tag(compressorname)  << "compressorname";
    stream.tag(depth)  << "depth";
    stream.tag(pre_defined3)  << "pre_defined";
}


}} // namespace mpeg::isobase

