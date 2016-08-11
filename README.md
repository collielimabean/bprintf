# bprintf

These couple functions were created to resolve an annoyance with packing data into raw byte buffers. When serializing, say, a struct to a char buffer, you have a couple choices. Declare the struct as packed, which there is no cross-platform way to do so, and then memcpy/cast into char *. However, this leaves you at the mercy of the architecture endianness. If you're sending that serialized data from a little to big endian system, well, your data will misinterpreted. The other way is to do a byte-by-byte assignment into the destination buffer, and that's a very, very manual process.

So, why not have a printf/scanf-like function that handles this for you? That's what bprintf (and its helper, bscanf) are meant to do. This implementation is meant to be very simple.

To build, you will need a C compiler and cmake. 
