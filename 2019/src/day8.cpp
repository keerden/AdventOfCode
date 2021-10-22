#include <bits/stdc++.h>

#include "util.h"

namespace {
class ImageLayer {
 private:
  size_t m_width;
  size_t m_height;
  std::string m_data;

 public:
  ImageLayer(size_t width, size_t height) : m_width{width}, m_height{height} {
    size_t total = m_width * m_height;
    m_data = std::string(total, '2');
  }

  bool setData(const std::string &data, size_t &index) {
    size_t total = m_width * m_height;

    if (data.length() < total + index)
      return false;

    m_data = data.substr(index, total);
    index += total;

    return true;
  }

  size_t count(char toFind) {
    size_t result{0};
    for (char c : m_data) {
      if (c == toFind)
        ++result;
    }
    return result;
  }

  void merge(ImageLayer below) {
    assert((m_width == below.m_width) && (m_height == below.m_height));
    assert(m_data.length() == below.m_data.length());

    for (size_t i{0}; i < m_data.length(); ++i) {
      if (m_data[i] == '2') {
        m_data[i] = below.m_data[i];
      }
    }
  }

  std::string getLayer() {
    return m_data;
  }

  void printLayer(bool fill) {
    for (size_t h{0}, offset{0}; h < m_height; ++h, offset += m_width) {
      for (size_t w{0}; w < m_width; ++w) {
        if (fill) {
          switch (m_data[w + offset]) {
            case '0':
              std::cout << " ";
              break;
            case '1':
              std::cout << "#";
              break;
            case '2':
              std::cout << ".";
              break;
            default:
              std::cout << "?";
              break;
          }
        } else {
          std::cout << m_data[w + offset];
        }
      }
      std::cout << "\n";
    }
  }
};
}  // namespace

size_t day8_1(const std::string &data, size_t width, size_t height) {
  size_t pos{0};
  size_t layerSize{width * height};
  size_t numLayers = data.length() / layerSize;

  size_t min_count{std::numeric_limits<std::size_t>::max()};
  size_t min_layer{};

  std::vector<ImageLayer> image;

  for (size_t i{0}; i < numLayers; ++i) {
    image.emplace_back(width, height);
    assert(image[i].setData(data, pos));
    size_t count = image[i].count('0');
    if (count < min_count) {
      min_count = count;
      min_layer = i;
    }
  }

  return image[min_layer].count('1') * image[min_layer].count('2');
}

ImageLayer constructImage(const std::string &data, size_t width, size_t height) {
  size_t pos{0};
  size_t layerSize{width * height};
  size_t numLayers = data.length() / layerSize;

  ImageLayer result(width, height);
  ImageLayer toMerge(width, height);

  for (size_t i{0}; i < numLayers; ++i) {
    assert(toMerge.setData(data, pos));
    result.merge(toMerge);
  }

  return result;
}

int day8_2(const std::string &data, size_t width, size_t height) {
  ImageLayer img{constructImage(data, width, height)};
  img.printLayer(true);

  return 0;
}

void day8(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    if (part == 1) {
      runTest(day8_1("123456789012", 3, 2), (size_t)1);
      runTest(day8_1("122256789012", 3, 2), (size_t)3);
      runTest(day8_1("111456722012", 3, 2), (size_t)0);
    } else {
      runTest(constructImage("0222112222120000", 2, 2).getLayer(), "0110");
    }

  } else {
    if (slist.fromFile(filename)) {
      int result;

      result = (part == 1) ? day8_1(slist.get(0), 25, 6) : day8_2(slist.get(0), 25, 6);
      std::cout << "result: " << result << "\n";

    } else {
      std::cout << "file not found!\n";
    }
  }
}