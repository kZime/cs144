#include "stream_reassembler.hh"


using namespace std;
// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

SubStr::SubStr(const std::string &_data, const std::size_t &_index) : data(_data), index(_index) {}

bool operator<(const SubStr &a, const SubStr& other) {
    return a.index < other.index;
}

StreamReassembler::StreamReassembler(const size_t capacity) : 
    _output(capacity),
    _capacity(capacity),
    _unassembled_num(0),
    _first_unassembled(0),
    _unassembled(),
    _eof(false) {}



size_t StreamReassembler::merge(string &dataa, size_t &indexa, set<SubStr>::iterator it) {
    string datab = it->data;
    size_t indexb = it->index;

    size_t l1 = indexa, l2 = indexb;
    size_t r1 = indexa + dataa.size(), r2 = indexb + datab.size();
    if (l1 > r2 or l2 > r1) {
        return 0;
    }
    size_t l = min(l1, l2);
    size_t del = datab.size();
    indexa = l;
    if (l == l1) { // a is the left one
        if (r2 > r1)
        dataa += string(datab.begin() + r1 - l2, datab.end());
    } else {
        if (r1 > r2)
            datab += string(dataa.begin() + r2 - l1, dataa.end());
        dataa.assign(datab);
    }
    return del;
}


//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    string tmpData = data;
    size_t tmpIndex = index;
    _eof = _eof or eof;


    if (data.empty() or index + data.size() <= _first_unassembled) {
        if (_eof)
            _output.end_input();
        return;
    }

    size_t first_unacceptable = _first_unassembled + _capacity - _output.buffer_size();

    // deal with SubStr(tmpData, tmpIndex)
    if (tmpIndex < _first_unassembled) {
        tmpData = tmpData.substr(_first_unassembled - tmpIndex);
        tmpIndex = _first_unassembled;
    }

    if (tmpIndex + tmpData.size() > first_unacceptable) {
        tmpData = tmpData.substr(0, first_unacceptable - tmpIndex);
    }


    if (index + data.size() > first_unacceptable)
        _eof = 0;

    // right
    auto it = _unassembled.lower_bound(SubStr(tmpData, tmpIndex));

    while (it != _unassembled.end()) {
        // if (it->index > tmpIndex + tmpData.size()) break;
        if (size_t del = merge(tmpData, tmpIndex, it)) {
            _unassembled.erase(it++);
            _unassembled_num -= del;
        } else break;
    }




    // left
    it = _unassembled.lower_bound(SubStr(tmpData, tmpIndex));
    // if (it != _unassembled.begin()) it--;

    while (it != _unassembled.begin()) {
        if (it == _unassembled.end())
            it--;
        if (size_t del = merge(tmpData, tmpIndex, it)) {
            _unassembled_num -= del;
            if (it != _unassembled.begin())
                _unassembled.erase(it--);
            else {
                _unassembled.erase(it);
                break;  
            }
        } else break;
        // if (it->index + it->data.size() < tmpIndex) break;
    }
    


    if (tmpIndex <= _first_unassembled) {
        size_t writ_sz = _output.write(tmpData.substr(_first_unassembled - tmpIndex));
        _first_unassembled += writ_sz;
        if (writ_sz == tmpData.size() and eof) {
            _output.end_input();
        }
    } else {
        _unassembled.insert(SubStr(tmpData, tmpIndex));
        _unassembled_num += tmpData.size();
    }

    if (empty() and _eof) {
        _output.end_input();
    }

}

size_t StreamReassembler::unassembled_bytes() const { return {_unassembled_num}; }

bool StreamReassembler::empty() const { return {_unassembled_num == 0}; }

size_t StreamReassembler::get_first_unassembled() const { return {_first_unassembled}; }

size_t StreamReassembler::get_window_size() const { 
    return {_capacity - _output.buffer_size()}; 
}

bool StreamReassembler::end_input() const { return {_output.input_ended()}; }