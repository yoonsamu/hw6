#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const {
      const size_t num_seg = 5;
      unsigned long long w[num_seg] = {0};  // nitialize all segments to zero

      // calculate segments in reverse order to handle last segment correctly
      int segment_idx = num_seg - 1;
      for (int i = k.length() - 1; i >= 0; i -= 6) {
          std::string subStr = k.substr(std::max(0, i - 5), std::min(6, i + 1));

          // convert substring to a large base-36 number
          unsigned long long segment_val = letterDigitToNumber(subStr[0]);
          for (size_t x = 1; x < subStr.length(); x++) {
              segment_val = 36 * segment_val + letterDigitToNumber(subStr[x]);
          }
          w[segment_idx--] = segment_val;
          if (segment_idx < 0) break; // Exit if all segments are processed
      }

      // output the values in the segments
      for (size_t i = 0; i < num_seg; i++) {
          std::cout << "w[" << i << "] = " << w[i] << std::endl;
      }

      // calculate the hash value
      HASH_INDEX_T hash_val = 0;
      for (size_t i = 0; i < num_seg; i++) {
          hash_val += rValues[i] * w[i];
      }

      return hash_val;
    }


    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
      HASH_INDEX_T hash_val = 0;
      size_t lett = size_t(letter);
      if (lett >= size_t('A') && lett <= size_t('Z')) {
        hash_val = lett - 65;
      }
      else if (lett >= size_t('0') && lett <= size_t ('9')) {
        hash_val = lett - 22;
      }
      else {
        hash_val = lett - 97;
      }
      return hash_val;
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
