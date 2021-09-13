#include "tcp_receiver.hh"
#include "wrapping_integers.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {

    size_t abs_seqno; // of seg
    bool eof = false;

    if (seg.header().syn) {
        if (_have_isn) {
            if (_isn != seg.header().seqno) 
                return; // false
        }
        _have_isn = true;
        _isn = seg.header().seqno;
        abs_seqno = 0;
    } else { // have isn and not syn
        abs_seqno = unwrap(
            seg.header().seqno,
            _isn, 
            _reassembler.get_first_unassembled()
        );
    }

    if (not _have_isn)
        return; // false


    if (seg.header().fin) {
        eof = true;    
    }

    string str = seg.payload().copy();
    _reassembler.push_substring(
        str, 
        (seg.header().syn ? 0 : abs_seqno - 1),  // stream index 
        eof
    );


    if (_reassembler.end_input()) {
        // stop;
        return;
    }
}

optional<WrappingInt32> TCPReceiver::ackno() const { 
    if (_have_isn) {
        return wrap(_reassembler.get_first_unassembled(), _isn) + 1 + (_reassembler.end_input() ? 1 : 0);
    }
    return std::nullopt;
}

size_t TCPReceiver::window_size() const { return _reassembler.get_window_size(); }
