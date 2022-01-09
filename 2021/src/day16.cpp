#include <bits/stdc++.h>

#include "solutions.h"

namespace {

class Packet {
 private:
  uint8_t m_version = 0;
  uint8_t m_type = 0;
  size_t m_length;
  std::vector<Packet> m_subPackets;
  uint64_t m_literalValue;

 public:
  Packet() : m_version{0}, m_type{0}, m_length{0} {};
  Packet(std::vector<bool> &transmission, size_t bitOffset) {
    size_t bitPos = bitOffset;

    m_version = static_cast<uint8_t>(decodeBits(transmission, bitPos, 3));
    m_type = static_cast<uint8_t>(decodeBits(transmission, bitPos + 3, 3));
    m_literalValue = 0;
    bitPos += 6;

    if (m_type == 4) {
      bitPos = decodeLiteralValue(transmission, bitPos);
      //std::cout << "lit: " << m_literalValue << "\n";
    } else {
      bool lengthInPackets = transmission[bitPos++];
      uint16_t opLength;
      if (lengthInPackets) {
        opLength = static_cast<uint16_t>(decodeBits(transmission, bitPos, 11));
        bitPos += 11;
        //std::cout << "op packets: " << opLength << "\n";
        for (uint16_t i{0}; i < opLength; ++i) {
          m_subPackets.emplace_back(transmission, bitPos);
          bitPos += m_subPackets.back().length();
        }
      } else {
        opLength = static_cast<uint16_t>(decodeBits(transmission, bitPos, 15));
        bitPos += 15;
        //std::cout << "op bits: " << opLength << "\n";
        size_t len{0};
        while (len < opLength) {
          m_subPackets.emplace_back(transmission, bitPos);
          bitPos += m_subPackets.back().length();
          len += m_subPackets.back().length();
        }
      }
    }

    m_length = bitPos - bitOffset;
  }

  size_t length() {
    return m_length;
  }

  uint64_t addVersions() {
    uint64_t result = m_version;

    for (auto &p : m_subPackets) {
      result += p.addVersions();
    }
    return result;
  }

  uint64_t getValue() {
    if (m_type == 4)
      return m_literalValue;

    uint64_t result{0};
    uint64_t lastVal{0};
    bool first{true};

    for (auto &p : m_subPackets) {
      uint64_t val = p.getValue();
      switch (m_type) {
        case 0:
          result = first ? val : (result + val);
          break;
        case 1:
          result = first ? val : (result * val);
          break;
        case 2:
          result = first ? val : std::min(result, val);
          ;
          break;
        case 3:
          result = first ? val : std::max(result, val);
          ;
          break;
        case 5:
          result = first ? 0 : ((lastVal > val) ? 1 : 0);
          break;
        case 6:
          result = first ? 0 : ((lastVal < val) ? 1 : 0);
          break;
        case 7:
          result = first ? 0 : ((lastVal == val) ? 1 : 0);
          break;
      }
      first = false;
      lastVal = val;
    }

    return result;
  }

 private:
  uint64_t decodeBits(std::vector<bool> &transmission, size_t bitOffset, size_t len) {
    uint64_t result{0};

    if (len == 0 || bitOffset >= transmission.size())
      return 0;

    if (len + bitOffset >= transmission.size()) {
      len = transmission.size() - bitOffset;
    }

    for (size_t i{0}; i < len; ++i) {
      result <<= 1;
      result |= transmission[i + bitOffset] ? 1 : 0;
    }

    return result;
  }

  size_t decodeLiteralValue(std::vector<bool> &transmission, size_t bitOffset) {
    bool next{true};
    while (next && bitOffset < transmission.size()) {
      next = transmission[bitOffset++];
      uint8_t val = decodeBits(transmission, bitOffset, 4);

      m_literalValue <<= 4;
      m_literalValue |= val & 0xF;
      bitOffset += 4;
    }
    return bitOffset;
  }

  char toHex(uint8_t num) {
    char c = '#';

    if (num < 10) {
      c = '0' + num;
    } else if (num < 16) {
      c = 'A' + num - 10;
    }

    return c;
  }
};

class Transmission {
 private:
  Packet m_outerPacket;
  std::vector<bool> m_data;

 public:
  Transmission(const std::string &hex) {
    for (auto c : hex) {
      uint8_t val;
      if (c >= '0' && c <= '9') {
        val = c - '0';
      } else if (c >= 'A' && c <= 'F') {
        val = c - 'A' + 10;
      } else {
        assert(false);
      }

      for (uint8_t mask{0b00001000}; mask > 0; mask >>= 1) {
        m_data.push_back(val & mask);
      }
    }

    //printData();
    m_outerPacket = Packet(m_data, 0);
  }

  void printData(void) {
    for (auto b : m_data) {
      std::cout << (b ? "1" : "0");
    }
    std::cout << "\n";
  }

  uint64_t addVerions() {
    return m_outerPacket.addVersions();
  }

  uint64_t getValue() {
    return m_outerPacket.getValue();
  }

 private:
};
}  //namespace

uint64_t day16_1(const std::string &input) {
  Transmission t(input);
  return t.addVerions();
}
uint64_t day16_2(const std::string &input) {
  Transmission t(input);
  return t.getValue();
}

void day16(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    if (part == 1) {
      runTest(day16_1("8A004A801A8002F478"), 16u);
      runTest(day16_1("620080001611562C8802118E34"), 12u);
      runTest(day16_1("C0015000016115A2E0802F182340"), 23u);
      runTest(day16_1("A0016C880162017C3686B18A3D4780"), 31u);
    } else {
      runTest(day16_2("C200B40A82"), 3u);
      runTest(day16_2("04005AC33890"), 54u);
      runTest(day16_2("880086C3E88112"), 7u);
      runTest(day16_2("CE00C43D881120"), 9u);

      runTest(day16_2("D8005AC2A8F0"), 1u);
      runTest(day16_2("F600BC2D8F"), 0u);
      runTest(day16_2("9C005AC2F8F0"), 0u);
      runTest(day16_2("9C0141080250320F1802104A08"), 1u);
    }
  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day16_1(slist.get(0)) << "\n";
      } else {
        std::cout << "result: " << day16_2(slist.get(0)) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}