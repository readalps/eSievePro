#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <windows.h>

typedef unsigned char u8;
typedef unsigned long ulong;
static ulong s_last = 0;
static u8* s_pOdd = NULL;
static std::vector<ulong> s_vecPrime;

bool renewCurrentPrime(ulong& primeIdx)
{
    while (primeIdx < s_last) {
        ++primeIdx;
        if (s_pOdd[primeIdx - 1] == 1)
            return true;
    }
    return false;
}

void showDetails()
{
    ulong pos = 0;
    while (true) {
        if (pos >= s_vecPrime.size())
            break;
        std::cout << s_vecPrime[pos++];
        if (pos < s_vecPrime.size())
            std::cout << ",";
        if (pos % 500 == 0) {
            printf("\n %u primes listed, %u left to show [c] continue; <q> quit: ", pos, s_vecPrime.size() - pos);
            std::string strInput;
            getline(std::cin, strInput);
            if (strInput == "q")
                break;
            std::cout << std::endl;
        }
        if (pos % 100 == 0)
            std::cout << std::endl;
    }
}

bool eSievePro(ulong raw_last, bool bDetail)
{
    DWORD tickBegin = GetTickCount();
    s_last = raw_last / 2;
    s_pOdd = new u8[s_last];
    if (!s_pOdd) {
        printf("Lack of memory.\n");
        return false;
    }
    ulong sum = 1;
    ulong curPrime = 2;
    if (bDetail)
        s_vecPrime.push_back(curPrime);
    if (raw_last == 3)
        goto Ending;

    memset(s_pOdd, 1, s_last);
    ulong curPrimeIdx = 0;
    while (true) {
        renewCurrentPrime(curPrimeIdx);
        ++sum;
        curPrime = (curPrimeIdx + curPrimeIdx) + 1;
        if (bDetail)
            s_vecPrime.push_back(curPrime);
        s_pOdd[curPrimeIdx - 1] = 0;
        ulong sqr = curPrime * curPrime;
        if (sqr > raw_last)
            break;
        ulong step = curPrime + curPrime;
        for (ulong idx = sqr; idx < raw_last; idx += step) {
            s_pOdd[(idx - 1) / 2 - 1] = 0;
        }
    }
    /// pick up all the left primes
    for (ulong idx = curPrime + 2; idx < raw_last; idx += 2) {
        ulong halfIdx = (idx - 1) / 2 - 1;
        if (s_pOdd[halfIdx] == 1) {
            ++sum;
            if (bDetail)
                s_vecPrime.push_back(idx);
        }
    }

Ending:
    printf(" %u primes found in %u milliseconds.\n\n", sum, GetTickCount() - tickBegin);
    delete []s_pOdd;
    return true;
}

int main()
{
    printf(" Eratosthenes sieve: a method to find out all primes below the number that you specify here please: ");
    std::string strInput;
    getline(std::cin, strInput);
    ulong raw_last = strtoul(strInput.c_str(), 0, 10);
    if (raw_last <= 2) {
        printf("\n Wrong input.\n");
        return 0;
    }
    printf("\n Only the sum of all primes needed [y/n](y as default): ");
    getline(std::cin, strInput);
    bool bDetail = (strInput == "n");
    if (bDetail)
        printf("\n Start to work out all primes below %u...\n", raw_last);
    else
        printf("\n Start to work out the sum of all primes below %u...\n", raw_last);
    if (!eSievePro(raw_last, bDetail))
        return 0;
    if (bDetail)
        showDetails();
    return 0;
}
