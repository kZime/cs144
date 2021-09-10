#include "wrapping_integers.hh"

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    // DUMMY_CODE(n, isn);

    return WrappingInt32{isn + n};
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    uint64_t a;
    if (isn < n or isn == n) {
        a = uint64_t(n.raw_value()) - uint64_t(isn.raw_value());
    } else {
        a = uint64_t(n.raw_value()) + (1ul << 32ll) - isn.raw_value();
    }
    uint64_t ans1, ans2;
    if (checkpoint >= a) {
        uint64_t k = (checkpoint - a) >> 32ul;
        ans1 = a + (k << 32ul);
        ans2 = a + ((k + 1) << 32ul);
        if ((checkpoint - ans1) < (ans2 - checkpoint)) {
            return ans1;
        } else return ans2;

    } else {
        return a;
    }

    return {};
}
