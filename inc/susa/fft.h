/*
 * This file is part of Susa.
 *
 * Susa is free software: you can redistribute it and/or modify
 * it under the terms of the Lesser GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.
 *
 * Susa is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Lesser GNU General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with Susa.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file fft.h
 * @brief The Fast Fourier Transform (FFT)
 * @author Behrooz Aliabadi
 * @version 1.0.0
 */

#ifndef SUSA_FFT_H
#define SUSA_FFT_H

namespace susa {

/**
 * @brief The Fast Fourier Transform (FFT) class.
 *
 * @ingroup Signal
 */
template <class T> class fft {
  public:
    /**
    * @brief Fast Fourier Transform (FFT) using the radix2 algorithm
    *
    * @todo It only supports 2^N input size. It should cope with any vector size.
    * @param vec_arg Input STL vector
    * @return returns STL vector
    */
    std::vector <T> radix2(std::vector <T> vec_arg);
  private:
    std::vector <T> bit_reverse(std::vector <T>);
};


template <class T> std::vector <T> fft<T>::radix2(std::vector <T> vec_arg)
{
    unsigned short int N = vec_arg.size();
    std::vector <T> vec_BitReverse = bit_reverse(vec_arg);
    std::complex <double> ct;
    T tTemp=0;
    int l=0;
    unsigned short int usiStage = susa::log2(N);

    for (int i=0; i<usiStage; i++) {                    // Stage counter
        for (int j=0; j < (1 << (usiStage - i - 1)); j++)
        {     // Seperated butterflies
            l = (j == 0) ? j:j * (1 << (i + 1));
            for (int k=0; k<(1<<i); k++)
            {              // Adjacent butterflies
                tTemp = vec_BitReverse[k+l];
                vec_BitReverse[k + l] = tTemp + vec_BitReverse[k +(1 << i) + l] *
                  exp(std::complex<double>(0,(-2 * k * PI * (1 << (usiStage - i - 1))) / N));

                vec_BitReverse[k + (1 << i) + l] = tTemp - vec_BitReverse[k + (1 << i) + l] *
                  exp(std::complex<double>(0,(-2 * k * PI * (1 << (usiStage - i - 1))) / N));
            }
        }
    }
    return vec_BitReverse;
}

template <class T> std::vector <T> fft<T>::bit_reverse(std::vector <T> vec_arg)
{
    unsigned int N = vec_arg.size();
    unsigned short int intBR;
    T TSwap=0;


    for (unsigned short int i=1; i < N/2; i++ ) {
        intBR = 0;
        for (unsigned short int j=0; j < susa::log2(N); j++) {
            intBR = intBR | (( i >> j)&1)<<(susa::log2(N)-j-1);
        }
        TSwap = vec_arg[i];
        vec_arg[i] = vec_arg[intBR];
        vec_arg[intBR] = TSwap;
    }
    return vec_arg;
}

} // NAMESPACE SUSA
#endif // SUSA_FFT_H
