#ifndef __MPEG_ISOBASE_DATA_ENTRY_URN_BOX__
#define __MPEG_ISOBASE_DATA_ENTRY_URN_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct DataEntryUrnBox: FullBox {

    static constexpr Type container_type = 'dref';
    static constexpr Type box_type = 'urn ';
    static constexpr const char *box_name = "DataEntryUrnBox";

    String name, location;

    DataEntryUrnBox(Composer &composer, const std::string &name, const std::string &location, Version version = 0, Flags flags = 0, bool largesize = false): FullBox(composer, box_type, version, flags, largesize) {

        composer.get(this->name, name, this->location, location);
    }

    DataEntryUrnBox(Parser &parser): FullBox(parser) {
        parser.get(*this, name, location);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_DATA_ENTRY_URN_BOX__

