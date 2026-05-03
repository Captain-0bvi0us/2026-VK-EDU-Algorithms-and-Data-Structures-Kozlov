#include "Huffman.h"

#include <vector>

struct VecIn : IInputStream {
    std::vector<byte> data;
    size_t pos = 0;
    bool Read(byte& value) override {
        if (pos >= data.size()) {
            return false;
        }
        value = data[pos++];
        return true;
    }
};

struct VecOut : IOutputStream {
    std::vector<byte> data;
    void Write(byte value) override { data.push_back(value); }
};

int main() {
    VecIn orig;
    orig.data = {0, 1, 2, 2, 2, 'a', 'a', 255};
    VecOut comp;
    Encode(orig, comp);

    VecIn dec_in;
    dec_in.data = std::move(comp.data);
    VecOut dec_out;
    Decode(dec_in, dec_out);

    const std::vector<byte> expected = {0, 1, 2, 2, 2, 'a', 'a', 255};
    return dec_out.data == expected ? 0 : 1;
}
