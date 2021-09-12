#ifndef SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH
#define SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH

#include "byte_stream.hh"

#include <cstdint>
#include <string>
#include <set>

class SubStr {
  public:
    std::string data;
    std::size_t index;
    SubStr(const std::string &data, const std::size_t &index);
    bool operator<(const SubStr& other);
};


//! \brief A class that assembles a series of excerpts from a byte stream (possibly out of order,
//! possibly overlapping) into an in-order byte stream.
class StreamReassembler {
  private:
    // Your code here -- add private members as necessary.

    ByteStream _output;  //!< The reassembled in-order byte stream
    std::size_t _capacity;    //!< The maximum number of bytes
    std::size_t _unassembled_num; //!< The number of bytes that have not yet been assembled
    std::size_t _first_unassembled; //!< The index of the first byte that has not yet been assembled
    std::set<SubStr> _unassembled; //!< The set of unassembled bytes
    bool _eof; //!< Whether the end of the stream has been reached

  public:
    //! \brief Construct a `StreamReassembler` that will store up to `capacity` bytes.
    //! \note This capacity limits both the bytes that have been reassembled,
    //! and those that have not yet been reassembled.
    StreamReassembler(const std::size_t capacity);




    size_t merge(std::string &data, std::size_t &index, std::set<SubStr>::iterator it);
    // void merge(std::string &dataa, std::size_t &indexa, std::set<SubStr>::iterator &it) {

    //! \brief Receive a substring and write any newly contiguous bytes into the stream.
    //!
    //! The StreamReassembler will stay within the memory limits of the `capacity`.
    //! Bytes that would exceed the capacity are silently discarded.
    //!
    //! \param data the substring
    //! \param index indicates the index (place in sequence) of the first byte in `data`
    //! \param eof the last byte of `data` will be the last byte in the entire stream
    void push_substring(const std::string &data, const uint64_t index, const bool eof); 

    //! \name Access the reassembled byte stream
    //!@{
    const ByteStream &stream_out() const { return _output; }
    ByteStream &stream_out() { return _output; }
    //!@}

    //! The number of bytes in the substrings stored but not yet reassembled
    //!
    //! \note If the byte at a particular index has been pushed more than once, it
    //! should only be counted once for the purpose of this function.
    std::size_t unassembled_bytes() const;

    //! \brief Is the internal state empty (other than the output stream)?
    //! \returns `true` if no substrings are waiting to be assembled
    bool empty() const;

    size_t get_first_unassembled() const;
    size_t get_window_size() const;
};

#endif  // SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH
