#include <iostream>
#include <vector>
#include <bitset>
#include <array>
#include <string>
#include <intrin.h>

class CPUInfo
{
    class InstructionSet;

public:
    static std::string Vendor();
    static std::string Brand();

    static bool SSE3Support();
    static bool PCLMULQDQSupport();
    static bool MONITORSupport();
    static bool SSSE3Support();
    static bool FMASupport();
    static bool CMPXCHG16BSupport();
    static bool SSE41Support();
    static bool SSE42Support();
    static bool MOVBESupport();
    static bool POPCNTSupport();
    static bool AESSupport();
    static bool XSAVESupport();
    static bool OSXSAVESupport();
    static bool AVXSupport();
    static bool F16CSupport();
    static bool RDRANDSupport();

    static bool MSRSupport();
    static bool CX8Support();
    static bool SEPSupport();
    static bool CMOVSupport();
    static bool CLFSHSupport();
    static bool MMXSupport();
    static bool FXSRSupport();
    static bool SSESupport();
    static bool SSE2Support();

    static bool FSGSBASESupport();
    static bool BMI1Support();
    static bool HLESupport();
    static bool AVX2Support();
    static bool BMI2Support();
    static bool ERMSSupport();
    static bool INVPCIDSupport();
    static bool RTMSupport();
    static bool AVX512FSupport();
    static bool RDSEEDSupport();
    static bool ADXSupport();
    static bool AVX512PFSupport();
    static bool AVX512ERSupport();
    static bool AVX512CDSupport();
    static bool SHASupport();

    static bool PREFETCHWT1Support();

    static bool LAHFSupport();
    static bool LZCNTSupport();
    static bool ABMSupport();
    static bool SSE4aSupport();
    static bool XOPSupport();
    static bool TBMSupport();

    static bool SYSCALLSupport();
    static bool MMXEXTSupport();
    static bool RDTSCPSupport();
    static bool _3DNOWEXTSupport();
    static bool _3DNOWSupport();

private:
    static const InstructionSet sCPU_Rep;

private:
    class InstructionSet
    {
    public:
        int nIds_;
        int nExIds_;
        std::string vendor_;
        std::string brand_;
        bool isIntel_;
        bool isAMD_;
        std::bitset<32> f_1_ECX_;
        std::bitset<32> f_1_EDX_;
        std::bitset<32> f_7_EBX_;
        std::bitset<32> f_7_ECX_;
        std::bitset<32> f_81_ECX_;
        std::bitset<32> f_81_EDX_;
        std::vector<std::array<int, 4>> data_;
        std::vector<std::array<int, 4>> extdata_;

        InstructionSet()
            : nIds_{ 0 },
            nExIds_{ 0 },
            isIntel_{ false },
            isAMD_{ false },
            f_1_ECX_{ 0 },
            f_1_EDX_{ 0 },
            f_7_EBX_{ 0 },
            f_7_ECX_{ 0 },
            f_81_ECX_{ 0 },
            f_81_EDX_{ 0 },
            data_{},
            extdata_{}
        {
            std::array<int, 4> cpui{};

            // Calling __cpuid with 0x0 as the function_id argument
            // gets the number of the highest valid function ID.
            __cpuid(cpui.data(), 0);
            nIds_ = cpui[0];

            for (int i = 0; i <= nIds_; ++i)
            {
                __cpuidex(cpui.data(), i, 0);
                data_.push_back(cpui);
            }

            // Capture vendor string
            char vendor[0x20];
            memset(vendor, 0, sizeof(vendor));
            *reinterpret_cast<int*>(vendor) = data_[0][1];
            *reinterpret_cast<int*>(vendor + 4) = data_[0][3];
            *reinterpret_cast<int*>(vendor + 8) = data_[0][2];
            vendor_ = vendor;
            if (vendor_ == "GenuineIntel")
            {
                isIntel_ = true;
            }
            else if (vendor_ == "AuthenticAMD")
            {
                isAMD_ = true;
            }

            // load bitset with flags for function 0x00000001
            if (nIds_ >= 1)
            {
                f_1_ECX_ = data_[1][2];
                f_1_EDX_ = data_[1][3];
            }

            // load bitset with flags for function 0x00000007
            if (nIds_ >= 7)
            {
                f_7_EBX_ = data_[7][1];
                f_7_ECX_ = data_[7][2];
            }

            // Calling __cpuid with 0x80000000 as the function_id argument
            // gets the number of the highest valid extended ID.
            __cpuid(cpui.data(), 0x80000000);
            nExIds_ = cpui[0];

            char brand[0x40];
            memset(brand, 0, sizeof(brand));

            for (int i = 0x80000000; i <= nExIds_; ++i)
            {
                __cpuidex(cpui.data(), i, 0);
                extdata_.push_back(cpui);
            }

            // load bitset with flags for function 0x80000001
            if (nExIds_ >= 0x80000001)
            {
                f_81_ECX_ = extdata_[1][2];
                f_81_EDX_ = extdata_[1][3];
            }

            // Interpret CPU brand string if reported
            if (nExIds_ >= 0x80000004)
            {
                memcpy(brand, extdata_[2].data(), sizeof(cpui));
                memcpy(brand + 16, extdata_[3].data(), sizeof(cpui));
                memcpy(brand + 32, extdata_[4].data(), sizeof(cpui));
                brand_ = brand;
            }
        }
    };
};
