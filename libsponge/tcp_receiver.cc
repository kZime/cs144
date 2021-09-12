#include "tcp_receiver.hh"
#include "wrapping_integers.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {

    size_t abs_seqno = 0;

    if (seg.header().syn) {
        if (_have_isn)
            return; // false
        _have_isn = true;
        _isn = seg.header().seqno;
    } else if (_have_isn)
        return; // false
    else { // have isn
        
    }



    if (_have_isn) {
        bool eof = seg.header().fin;
        _reassembler.push_substring(
            seg.payload().copy(),
            unwrap(seg.header().seqno, _isn, ),
            eof
        );
    }

    // DUMMY_CODE(seg);
}

optional<WrappingInt32> TCPReceiver::ackno() const { 
    if (_have_isn) {
        return wrap(_reassembler.get_first_unassembled(), _isn);
    }
    return std::nullopt;
}

size_t TCPReceiver::window_size() const { return _reassembler.get_window_size(); }
