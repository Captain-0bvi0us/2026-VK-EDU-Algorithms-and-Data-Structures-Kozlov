#include "Huffman.h"

#include <algorithm>
#include <cstdint>
#include <queue>
#include <utility>
#include <vector>

namespace {

std::vector<byte> ReadAll(IInputStream& in) {
    std::vector<byte> data;
    byte b = 0;
    while (in.Read(b)) {
        data.push_back(b);
    }
    return data;
}

void WriteU32(IOutputStream& out, uint32_t x) {
    out.Write(static_cast<byte>(x & 0xFF));
    out.Write(static_cast<byte>((x >> 8) & 0xFF));
    out.Write(static_cast<byte>((x >> 16) & 0xFF));
    out.Write(static_cast<byte>((x >> 24) & 0xFF));
}

bool ReadU32(IInputStream& in, uint32_t& x) {
    x = 0;
    byte b = 0;
    for (int i = 0; i < 4; ++i) {
        if (!in.Read(b)) {
            return false;
        }
        x |= static_cast<uint32_t>(b) << (8 * i);
    }
    return true;
}

class BitWriter {
public:
    explicit BitWriter(IOutputStream& out) : out_(out) {}

    void WriteBits(unsigned val, int n) {
        for (int i = n - 1; i >= 0; --i) {
            WriteBit((val >> i) & 1);
        }
    }

    void Flush() {
        if (used_bits_ > 0) {
            buffer_ <<= static_cast<unsigned>(8 - used_bits_);
            out_.Write(static_cast<byte>(buffer_));
            buffer_ = 0;
            used_bits_ = 0;
        }
    }

private:
    IOutputStream& out_;
    unsigned buffer_ = 0;
    int used_bits_ = 0;

    void WriteBit(int bit) {
        buffer_ = (buffer_ << 1) | (bit & 1);
        ++used_bits_;
        if (used_bits_ == 8) {
            out_.Write(static_cast<byte>(buffer_));
            buffer_ = 0;
            used_bits_ = 0;
        }
    }
};

class BitReader {
public:
    explicit BitReader(IInputStream& in) : in_(in) {}

    bool ReadBit(int& bit) {
        if (bits_left_ == 0) {
            if (!in_.Read(cur_byte_)) {
                return false;
            }
            bits_left_ = 8;
        }
        bit = (cur_byte_ >> (bits_left_ - 1)) & 1;
        --bits_left_;
        return true;
    }

    bool ReadBits10(unsigned& v) {
        v = 0;
        for (int j = 0; j < 10; ++j) {
            int b = 0;
            if (!ReadBit(b)) {
                return false;
            }
            v = (v << 1) | static_cast<unsigned>(b);
        }
        return true;
    }

private:
    IInputStream& in_;
    byte cur_byte_ = 0;
    int bits_left_ = 0;
};

struct HNode {
    int freq = 0;
    int sym = -1;
    HNode* left = nullptr;
    HNode* right = nullptr;

    HNode(int f, int s) : freq(f), sym(s) {}
};

struct HNodeCmp {
    bool operator()(const HNode* a, const HNode* b) const {
        if (a->freq != b->freq) {
            return a->freq > b->freq;
        }
        return a->sym > b->sym;
    }
};

void FreeTree(HNode* n) {
    if (n == nullptr) {
        return;
    }
    FreeTree(n->left);
    FreeTree(n->right);
    delete n;
}

void CollectLengths(HNode* n, int depth, std::vector<int>& lens) {
    if (n == nullptr) {
        return;
    }
    if (n->left == nullptr && n->right == nullptr) {
        const int len = std::max(1, depth);
        lens[static_cast<size_t>(n->sym)] = len;
        return;
    }
    CollectLengths(n->left, depth + 1, lens);
    CollectLengths(n->right, depth + 1, lens);
}

void BuildLengths(const std::vector<int>& freq, std::vector<int>& lens) {
    lens.assign(256, 0);
    std::priority_queue<HNode*, std::vector<HNode*>, HNodeCmp> pq;
    for (int i = 0; i < 256; ++i) {
        if (freq[static_cast<size_t>(i)] > 0) {
            pq.push(new HNode(freq[static_cast<size_t>(i)], i));
        }
    }
    if (pq.empty()) {
        return;
    }
    HNode* root = nullptr;
    if (pq.size() == 1) {
        root = pq.top();
        pq.pop();
        lens[static_cast<size_t>(root->sym)] = 1;
        delete root;
        return;
    }
    while (pq.size() > 1) {
        HNode* a = pq.top();
        pq.pop();
        HNode* b = pq.top();
        pq.pop();
        HNode* p = new HNode(a->freq + b->freq, -1);
        p->left = a;
        p->right = b;
        pq.push(p);
    }
    root = pq.top();
    pq.pop();
    CollectLengths(root, 0, lens);
    FreeTree(root);
}

void CanonicalCodes(const std::vector<int>& lens, std::vector<uint32_t>& codes) {
    codes.assign(256, 0);
    std::vector<std::pair<int, int>> items;
    for (int s = 0; s < 256; ++s) {
        if (lens[static_cast<size_t>(s)] > 0) {
            items.push_back({lens[static_cast<size_t>(s)], s});
        }
    }
    if (items.empty()) {
        return;
    }
    std::sort(items.begin(), items.end());
    codes[static_cast<size_t>(items[0].second)] = 0;
    for (size_t i = 1; i < items.size(); ++i) {
        uint32_t c = codes[static_cast<size_t>(items[i - 1].second)];
        c = (c + 1) << (items[i].first - items[i - 1].first);
        codes[static_cast<size_t>(items[i].second)] = c;
    }
}

struct TrieNode {
    TrieNode* child[2]{};
    bool leaf = false;
    int sym = 0;

    ~TrieNode() {
        delete child[0];
        delete child[1];
    }
};

void TrieInsert(TrieNode* r, uint32_t code, int len, int sym) {
    TrieNode* cur = r;
    for (int i = len - 1; i >= 0; --i) {
        const int b = (code >> i) & 1;
        if (cur->child[b] == nullptr) {
            cur->child[b] = new TrieNode();
        }
        cur = cur->child[b];
    }
    cur->leaf = true;
    cur->sym = sym;
}

}  // namespace

void Encode(IInputStream& original, IOutputStream& compressed) {
    const std::vector<byte> data = ReadAll(original);
    const auto n = static_cast<uint32_t>(data.size());

    std::vector<int> freq(256, 0);
    for (byte b : data) {
        ++freq[b];
    }

    std::vector<int> lens;
    BuildLengths(freq, lens);

    WriteU32(compressed, n);

    BitWriter bits(compressed);
    for (int i = 0; i < 256; ++i) {
        bits.WriteBits(static_cast<unsigned>(lens[static_cast<size_t>(i)]), 10);
    }

    std::vector<uint32_t> codes;
    CanonicalCodes(lens, codes);
    for (byte b : data) {
        const int len = lens[b];
        bits.WriteBits(codes[b], len);
    }
    bits.Flush();
}

void Decode(IInputStream& compressed_stream, IOutputStream& original) {
    uint32_t n = 0;
    if (!ReadU32(compressed_stream, n)) {
        return;
    }

    BitReader bits(compressed_stream);
    std::vector<int> lens(256, 0);
    for (int i = 0; i < 256; ++i) {
        unsigned v = 0;
        if (!bits.ReadBits10(v)) {
            return;
        }
        lens[static_cast<size_t>(i)] = static_cast<int>(v);
    }

    std::vector<uint32_t> codes;
    CanonicalCodes(lens, codes);

    TrieNode root;
    for (int s = 0; s < 256; ++s) {
        if (lens[static_cast<size_t>(s)] > 0) {
            TrieInsert(&root, codes[static_cast<size_t>(s)], lens[static_cast<size_t>(s)], s);
        }
    }

    for (uint32_t i = 0; i < n; ++i) {
        TrieNode* cur = &root;
        while (!cur->leaf) {
            int b = 0;
            if (!bits.ReadBit(b)) {
                return;
            }
            cur = cur->child[b];
            if (cur == nullptr) {
                return;
            }
        }
        original.Write(static_cast<byte>(cur->sym));
    }
}
