#ifndef __MPEG_ISOBASE_COMPACT_SAMPLE_SIZE_BOX__
#define __MPEG_ISOBASE_COMPACT_SAMPLE_SIZE_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/box_entries.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct CompactSampleSizeBox: FullBox {

    static constexpr Type container_type = 'stbl';
    static constexpr Type box_type = 'stz2';
    static constexpr const char *box_name = "CompactSampleSizeBox";

    union { Footprint<2 * 32> header;

        UInt<24, 0> reserved;       // = 0 ;
        UInt8<  24> field_size;     // = 4 | 8 | 16
        UInt32< 32> sample_count;
    };

    struct Entry;
    struct Parsed {
        using Entries = Box::Parsed::Entries<CompactSampleSizeBox, Entry>;
    };
    struct Composed {
        using Entries = Box::Composed::Entries<CompactSampleSizeBox, Entry>;
    };

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void CompactSampleSizeBox_(Composer &composer) {
        composer.get(header);
        const_field<verify>(reserved, 0);
    }

    CompactSampleSizeBox(Composer &composer, uint8_t field_size, uint32_t sample_count, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        CompactSampleSizeBox_(composer);
        assert(field_size == 4 || field_size == 8 || field_size == 16);
        this->field_size = field_size;
        this->sample_count = sample_count;
    }

    CompactSampleSizeBox(Parser &parser): FullBox(parser, Flags(0)) {
        CompactSampleSizeBox_(parser);
        assert(field_size == 4 || field_size == 8 || field_size == 16);
        assert(8 * parser.box_payload_size(*this) / field_size == sample_count);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

    virtual void parse_payload(Remainder left_payload);
};

struct CompactSampleSizeBox::Entry : Box::Entry {
    union { Footprint<32> header;
        UInt32<0> entry_size;
    };

    Entry(const Entry &entry): header(entry.header), Box::Entry(entry) {}

    template <typename Composer>
    Entry(const CompactSampleSizeBox &, Composer &composer) {
        composer.get(header);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_COMPACT_SAMPLE_SIZE_BOX__

