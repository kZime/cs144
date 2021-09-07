#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : 
    _output(capacity),
    _capacity(capacity),
    _unassembled(0),
    _unasem() {}


//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    _eof = _eof or eof;
    if (data.empty() or index + data.size() <= _first_unassembled) {
        if (_eof) {
            _output.end_input();
        }
        return;
    }

    if (index <= _first_unassembled) {
        size_t writ_sz = _output.write(string(data.begin() + _first_unassembled - index, data.end()));
        _first_unassembled += writ_sz;
        if (_eof) {
            _output.end_input();
            return;
        }
        if (!_unassembled.empty()) {
            if (_unassembled.front().index == _first_unassembled) {
                size_t cnt = 0;
                string str;
                size_t pre = _first_unassembled;
                for (auto tmp : _unassembled) {
                    if (tmp.index == pre) {
                        str += tmp.data;
                        cnt++;
                        pre += tmp.data.size();
                    } else {
                        break;
                    }
                }
                while(cnt--) {
                    _unassembled.erase(_unassembled.begin());
                }
                push_substring(str, _first_unassembled, eof);
                if (_eof) {
                    
                }
            }
        }   
    }

}

size_t StreamReassembler::unassembled_bytes() const { return {}; }

bool StreamReassembler::empty() const { return {}; }
