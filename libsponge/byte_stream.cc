#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : 
    _buffer(),
    _capacity(capacity),
    _size(0),
    _readn(0),
    _writen(0),
    _end_input(false) {}

size_t ByteStream::write(const string &data) {
    size_t i;
    for (i = 0; i < data.size() and _size != _capacity; i++) {
        _buffer.push_back(data[i]);
        _size++;
        _writen++;
    }
    return {i};
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    return string(_buffer.begin(), _buffer.begin() + min(len, _size));
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    for (size_t i = 0; i < len; i++) {
        _buffer.pop_front();
    }
    _size -= len;
    _readn += len;
    return;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string

std::string ByteStream::read(const size_t len) {
    size_t l = min(len, _size);
    string s = peek_output(l);
    _size -= l;
    _readn += l;
    return s;
}

void ByteStream::end_input() {_end_input = true;}

bool ByteStream::input_ended() const { return {_end_input}; }

size_t ByteStream::buffer_size() const { return {_size}; }

bool ByteStream::buffer_empty() const { return {_size == 0}; }

bool ByteStream::eof() const { return input_ended() and buffer_empty(); }

size_t ByteStream::bytes_written() const { return {_writen}; }

size_t ByteStream::bytes_read() const { return {_readn}; }

size_t ByteStream::remaining_capacity() const { return _capacity - _size; }
