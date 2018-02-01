#ifndef __MPEG_ISOBASE_BOX_ENTRIES__
#define __MPEG_ISOBASE_BOX_ENTRIES__

#include <bitstream/sstream.h>
#include <mpeg-isobase/box.h>
#include <mpeg-isobase/parser.h>


namespace mpeg {
namespace isobase {


struct Box::Entry: bitstream::Header, bitstream::output::meta::Header {

    const bitstream::Composer *composer;
    unsigned long index;

    Entry() = default;
    Entry(const Entry &entry) :index(entry.index), composer(entry.composer) { }

    virtual void output_header(bitstream::output::meta::header::Stream &stream) const;

    virtual const char *name() const { return "Entry"; }
};


template <typename BoxType, typename EntryType, typename Composer_>
struct Box::Entries {
    const BoxType &box;
    Composer_ &composer;
    uint32_t count;


    Entries(const BoxType &box, Composer_ &composer, uint32_t count)
        : box(box), composer(composer), count(count) {
    }

    struct Iterator {
        Entries &entries;
        unsigned long index;

        Iterator(Entries &entries, unsigned long index) : entries(entries), index(index) {}
        bool operator != (const Iterator &it) const { return index != it.index; }
        
        void operator++() {
            auto entry_size = entries.composer.hstream.consumed();
            entries.composer.stream.get_blob(entry_size);   // TODO: make it more efficient by fetching blob for many objects not one
            ++index;
        }

        const EntryType operator * () const {
            return const_cast<Entries &>(*this).operator * ();
        }

        EntryType operator * () {
            entries.composer.hstream.reset();
            EntryType entry{entries.box, entries.composer};
            entry.index = index;
            entry.composer = &entries.composer;
            return entry;
        }
    };

    Iterator begin() { return Iterator{*this, 0}; }
    Iterator end() { return Iterator{*this, count}; }
    const Iterator begin() const { return Iterator{const_cast<Entries &>(*this), 0}; }
    const Iterator end() const { return Iterator{const_cast<Entries &>(*this), count}; }
};


template <typename BoxType, typename EntryType>
struct Box::Parsed::Entries: Box::Entries<BoxType, EntryType, Box::Parser> {

    Remainder &left;

    Entries(const BoxType &box, Box::Parser &parser, Remainder &left,
            uint32_t (*entries)(const BoxType &) = [](const auto &box) -> uint32_t { return box.entry_count; } )
        : Box::Entries<BoxType, EntryType, Box::Parser>(box, parser, entries(box)), left(left) {
    }

    struct Iterator: Box::Entries<BoxType, EntryType, Box::Parser>::Iterator {
        using Box::Entries<BoxType, EntryType, Box::Parser>::Iterator::Iterator;

        void operator++() {
            Entries &entries = static_cast<Entries &>(this->entries);
            auto entry_size = this->entries.composer.hstream.consumed();
            entries.left.reduce(entry_size, [&]() {
                return Parser::Exception(SStream() <<
                        "size of #" << this->index << " entry (" << entry_size << " bytes) outreaches "
                        "what's left within container's box payload (" << int64_t(entries.left) << " bytes).");
            });
            Box::Entries<BoxType, EntryType, Box::Parser>::Iterator::operator++();
        }
    };

    Iterator begin() { return Iterator{*this, 0}; }
    Iterator end() { return Iterator{*this, this->count}; }
    const Iterator begin() const { return Iterator{const_cast<Entries &>(*this), 0}; }
    const Iterator end() const { return Iterator{const_cast<Entries &>(*this), this->count}; }
};


template <typename BoxType, typename EntryType>
struct Box::Composed::Entries: Box::Entries<BoxType, EntryType, Box::Composer> {

    Entries(const BoxType &box, Box::Parser &parser,
            uint32_t (*entries)(const BoxType &) = [](const auto &box) -> uint32_t { return box.entry_count; } )
        : Box::Entries<BoxType, EntryType, Box::Composer>(box, parser, entries(box)) {
    }
};

}} // namespace mpeg::isobase


#endif // __MPEG_ISOBASE_BOX_ENTRIES__

