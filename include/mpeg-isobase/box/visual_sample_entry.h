#ifndef __MPEG_ISOBASE_VISUAL_SAMPLE_ENTRY__
#define __MPEG_ISOBASE_VISUAL_SAMPLE_ENTRY__

#include <mpeg-isobase/box/sample_entry.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct VisualSampleEntry: SampleEntry {

    static constexpr Type box_type = 'vide';
    static constexpr const char *box_name = "VisualSampleEntry";

    union { Footprint<2 * 16 + 3 * 32 + 2 * 16 + 3 * 32 + 16 + 32 * 8 + 2 * 16> header;
        UInt16<  0> pre_defined;                // = 0;
        UInt16< 16> reserved;                   // = 0;
        UInt32< 32>::Static::Array<3> pre_defined2;     // = 0;
        UInt16<128> width;
        UInt16<144> height;
        UInt32<160> horizresolution;            // = 0x00480000; // 72 dpi
        UInt32<192> vertresolution;             // = 0x00480000; // 72 dpi
        UInt32<224> reserved2;                  // = 0;
        UInt16<256> frame_count;                // = 1;
        Static::String<272, 32> compressorname;         // TODO: introduce proper type to process fixed-size string
        UInt16<528> depth;                      // = 0x0018;
         Int16<544> pre_defined3;               // = -1;
    };

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void VisualSampleEntry_(Composer &composer) {
        composer.get(header);

        const_field<verify>(pre_defined,      0);
        const_field<verify>(reserved,         0);
        const_field<verify>(pre_defined2,     {0, 0, 0});
        const_field<verify>(horizresolution,  0x00480000);
        const_field<verify>(vertresolution,   0x00480000);
        const_field<verify>(reserved2,        0);
        const_field<verify>(frame_count,      1);
        const_field<verify>(depth,            0x0018);
        const_field<verify>(pre_defined3,     -1);
    }

    VisualSampleEntry(Composer &composer, bool largesize = false): SampleEntry(composer, box_type, largesize) {
        VisualSampleEntry_(composer);
    }

    VisualSampleEntry(Parser &parser): SampleEntry(parser) {
        VisualSampleEntry_(parser);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_VISUAL_SAMPLE_ENTRY__

