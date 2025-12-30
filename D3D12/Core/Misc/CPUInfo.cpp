#include "CPUInfo.h"

const CPUInfo::InstructionSet CPUInfo::sCPU_Rep;

std::string CPUInfo::Vendor()
{
    return sCPU_Rep.vendor_;
}

std::string CPUInfo::Brand()
{
    return sCPU_Rep.brand_;
}

bool CPUInfo::SSE3Support()
{
    return sCPU_Rep.f_1_ECX_[0];
}

bool CPUInfo::PCLMULQDQSupport()
{
    return sCPU_Rep.f_1_ECX_[1];;
}

bool CPUInfo::MONITORSupport()
{
    return sCPU_Rep.f_1_ECX_[3];
}

bool CPUInfo::SSSE3Support()
{
    return sCPU_Rep.f_1_ECX_[9];
}

bool CPUInfo::FMASupport()
{
    return sCPU_Rep.f_1_ECX_[12];
}

bool CPUInfo::CMPXCHG16BSupport()
{
    return sCPU_Rep.f_1_ECX_[13];
}

bool CPUInfo::SSE41Support()
{
    return sCPU_Rep.f_1_ECX_[19];
}

bool CPUInfo::SSE42Support()
{
    return sCPU_Rep.f_1_ECX_[20];
}

bool CPUInfo::MOVBESupport()
{
    return sCPU_Rep.f_1_ECX_[22];
}

bool CPUInfo::POPCNTSupport()
{
    return sCPU_Rep.f_1_ECX_[23];
}

bool CPUInfo::AESSupport()
{
    return sCPU_Rep.f_1_ECX_[25];
}

bool CPUInfo::XSAVESupport()
{
    return sCPU_Rep.f_1_ECX_[26];
}

bool CPUInfo::OSXSAVESupport()
{
    return sCPU_Rep.f_1_ECX_[27];
}

bool CPUInfo::AVXSupport()
{
    return sCPU_Rep.f_1_ECX_[28];
}

bool CPUInfo::F16CSupport()
{
    return sCPU_Rep.f_1_ECX_[29];
}

bool CPUInfo::RDRANDSupport()
{
    return sCPU_Rep.f_1_ECX_[30];
}

bool CPUInfo::MSRSupport()
{
    return sCPU_Rep.f_1_EDX_[5];
}

bool CPUInfo::CX8Support()
{
    return sCPU_Rep.f_1_EDX_[8];
}

bool CPUInfo::SEPSupport()
{
    return sCPU_Rep.f_1_EDX_[11];
}

bool CPUInfo::CMOVSupport()
{
    return sCPU_Rep.f_1_EDX_[15];
}

bool CPUInfo::CLFSHSupport()
{
    return sCPU_Rep.f_1_EDX_[19];
}

bool CPUInfo::MMXSupport()
{
    return sCPU_Rep.f_1_EDX_[23];
}

bool CPUInfo::FXSRSupport()
{
    return sCPU_Rep.f_1_EDX_[24];
}

bool CPUInfo::SSESupport()
{
    return sCPU_Rep.f_1_EDX_[25];
}

bool CPUInfo::SSE2Support()
{
    return sCPU_Rep.f_1_EDX_[26];
}

bool CPUInfo::FSGSBASESupport()
{
    return sCPU_Rep.f_7_EBX_[0];
}

bool CPUInfo::BMI1Support()
{
    return sCPU_Rep.f_7_EBX_[3];
}

bool CPUInfo::HLESupport()
{
    return sCPU_Rep.f_7_EBX_[4];
}

bool CPUInfo::AVX2Support()
{
    return sCPU_Rep.f_7_EBX_[5];
}

bool CPUInfo::BMI2Support()
{
    return sCPU_Rep.f_7_EBX_[8];
}

bool CPUInfo::ERMSSupport()
{
    return sCPU_Rep.f_7_EBX_[9];
}

bool CPUInfo::INVPCIDSupport()
{
    return sCPU_Rep.f_7_EBX_[10];
}

bool CPUInfo::RTMSupport()
{
    return sCPU_Rep.f_7_EBX_[11];
}

bool CPUInfo::AVX512FSupport()
{
    return sCPU_Rep.f_7_EBX_[16];
}

bool CPUInfo::RDSEEDSupport()
{
    return sCPU_Rep.f_7_EBX_[18];
}

bool CPUInfo::ADXSupport()
{
    return sCPU_Rep.f_7_EBX_[19];
}

bool CPUInfo::AVX512PFSupport()
{
    return sCPU_Rep.f_7_EBX_[26];
}

bool CPUInfo::AVX512ERSupport()
{
    return sCPU_Rep.f_7_EBX_[27];
}

bool CPUInfo::AVX512CDSupport()
{
    return sCPU_Rep.f_7_EBX_[28];
}

bool CPUInfo::SHASupport()
{
    return sCPU_Rep.f_7_EBX_[29];
}

bool CPUInfo::PREFETCHWT1Support()
{
    return sCPU_Rep.f_7_ECX_[0];
}

bool CPUInfo::LAHFSupport()
{
    return sCPU_Rep.f_81_ECX_[0];
}

bool CPUInfo::LZCNTSupport()
{
    return sCPU_Rep.f_81_ECX_[5];
}

bool CPUInfo::ABMSupport()
{
    return sCPU_Rep.f_81_ECX_[5];
}

bool CPUInfo::SSE4aSupport()
{
    return sCPU_Rep.f_81_ECX_[6];
}

bool CPUInfo::XOPSupport()
{
    return sCPU_Rep.f_81_ECX_[11];
}

bool CPUInfo::TBMSupport()
{
    return sCPU_Rep.f_81_ECX_[21];
}

bool CPUInfo::SYSCALLSupport()
{
    return sCPU_Rep.isIntel_ && sCPU_Rep.f_81_EDX_[11];
}

bool CPUInfo::MMXEXTSupport()
{
    return sCPU_Rep.isAMD_ && sCPU_Rep.f_81_EDX_[22];
}

bool CPUInfo::RDTSCPSupport()
{
    return sCPU_Rep.isIntel_ && sCPU_Rep.f_81_EDX_[27];
}

bool CPUInfo::_3DNOWEXTSupport()
{
    return sCPU_Rep.isAMD_ && sCPU_Rep.f_81_EDX_[30];
}

bool CPUInfo::_3DNOWSupport()
{
    return sCPU_Rep.isAMD_ && sCPU_Rep.f_81_EDX_[31];
}
