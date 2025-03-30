#include "NXModule.hpp"

#ifdef __SWITCH__

#include <cstring>

nn::ro::detail::RoModule** nn::ro::detail::GetModuleListHead() {
    const uintptr_t insns_base = reinterpret_cast<uintptr_t>(&nn::ro::Finalize) + 0x2C;
    const uint32_t *insns = reinterpret_cast<uint32_t*>(insns_base);

    // Extract the ADRP and LDR instructions to compute the module list pointer
    const uintptr_t adrp_value = (insns_base & ~(0xFFF)) + ((insns[0] >> 5 & 0x7FFFF) << 14) + ((insns[0] >> 29 & 2) << 12);
    const uintptr_t ldr_offset = (insns[1] >> 10 & 0xFFF) << (insns[1] >> 30);

    return *reinterpret_cast<RoModule ***>(adrp_value + ldr_offset);
}

nn::ro::detail::RoModule* nn::ro::detail::FindRoModuleById(std::array<std::uint8_t, 20> moduleId) {
    RoModule *pHead = *GetModuleListHead();
    for (RoModule* moduleEntry = pHead; moduleEntry; moduleEntry = moduleEntry->next) {
		// https://switchbrew.org/wiki/NRO
        // Check against "nn::ro::detail::ModuleId"
        if (!std::memcmp(reinterpret_cast<uint8_t*>(moduleEntry->baseAddress + 0x40), moduleId.data(), 20)) {
            return moduleEntry;
        }

        if (moduleEntry->next == pHead) {
            break; // We went through them all
        }
    }

    return nullptr;
}

// uintptr_t bsppacknrobase = 0;
uintptr_t clientnrobase = 0;
uintptr_t datacachenrobase = 0;
uintptr_t enginenrobase = 0;
uintptr_t filesystem_stdionrobase = 0;
uintptr_t inputsystemnrobase = 0;
uintptr_t launchernrobase = 0;
uintptr_t localizenrobase = 0;
uintptr_t matchmakingnrobase = 0;
uintptr_t materialsystemnrobase = 0;
uintptr_t scenefilecachenrobase = 0;
uintptr_t servernrobase = 0;
// uintptr_t shaderapiemptynrobase = 0;
uintptr_t soundemittersystemnrobase = 0;
uintptr_t studiorendernrobase = 0;
uintptr_t tier0nrobase = 0;
uintptr_t vgui2nrobase = 0;
uintptr_t vguimatsurfacenrobase = 0;
uintptr_t vphysicsnrobase = 0;
uintptr_t vscriptnrobase = 0;
uintptr_t vstdlibnrobase = 0;

bool InitNXModuleBases() {
	// bsppacknrobase = 				nn::ro::detail::FindRoModuleById("A7B9C386738256724BACEED44CBB1D6854BECF33")->baseAddress;
	clientnrobase =					nn::ro::detail::FindRoModuleById("ABC37131A88D191BE4CB813EE84E11B21B94FC2C")->baseAddress;
	datacachenrobase = 				nn::ro::detail::FindRoModuleById("4027DD610E3D38EE1EDA9905B01B832FCEF0D766")->baseAddress;
	enginenrobase =					nn::ro::detail::FindRoModuleById("12820975A58BAE4A9EAE7AA82E5C1DCCAE260A19")->baseAddress;
	filesystem_stdionrobase = 		nn::ro::detail::FindRoModuleById("60A9E75E3662E6A48A24F7C958257F2639D516BC")->baseAddress;
	inputsystemnrobase = 			nn::ro::detail::FindRoModuleById("7A53B88C390C142866ADDFE5F45B9DBFDFBCB1E3")->baseAddress;
	launchernrobase = 				nn::ro::detail::FindRoModuleById("B857C14E84176F5BD6C518640DD9AA2908D6CFFA")->baseAddress;
	localizenrobase =				nn::ro::detail::FindRoModuleById("04EB58AF45B255EC0C7CFAE0BC012BC104058862")->baseAddress;
	matchmakingnrobase = 			nn::ro::detail::FindRoModuleById("6CF7AD9EFCD6308002DAF158C91653F26087C6A4")->baseAddress;
	materialsystemnrobase =			nn::ro::detail::FindRoModuleById("69BD850200ACBBFF7D6B6EF312D64E53FA73680B")->baseAddress;
	scenefilecachenrobase =			nn::ro::detail::FindRoModuleById("D44A4378FFF04D7938CE29CBB40252B29C1F411C")->baseAddress;
	servernrobase =					nn::ro::detail::FindRoModuleById("42D59B8BF6F02B1FE75ABF5FA3A1435713047DEA")->baseAddress;
	// shaderapiemptynrobase =			nn::ro::detail::FindRoModuleById("97E6F3B867D20CFA477A363A97D80E658FC50157")->baseAddress;
	soundemittersystemnrobase =		nn::ro::detail::FindRoModuleById("1F08714AC92B1BB2ACA56A1C32E08754D460EA77")->baseAddress;
	studiorendernrobase =			nn::ro::detail::FindRoModuleById("E4B6C3C75DCD40F2EBFEFC973207617C89D89690")->baseAddress;
	tier0nrobase = 					nn::ro::detail::FindRoModuleById("41C209763789790E429719CE754C71DB78124048")->baseAddress;
	vgui2nrobase =					nn::ro::detail::FindRoModuleById("A83F79040F84193C2F21A6C39D3A6BAB1D5219BD")->baseAddress;
	vguimatsurfacenrobase =			nn::ro::detail::FindRoModuleById("4E6E411761F7287DAE9F6AA90217924EE28D4F44")->baseAddress;
	vphysicsnrobase = 				nn::ro::detail::FindRoModuleById("C01B27E514E9AEB01E6AA982C106C685EBD7D91A")->baseAddress;
	vscriptnrobase = 				nn::ro::detail::FindRoModuleById("75010F2CCA80434E3C5800D929F3E4067241728D")->baseAddress;
	vstdlibnrobase = 				nn::ro::detail::FindRoModuleById("98DCFE5D1F736DC1971DF35429AB5265F590ABC4")->baseAddress;

	if (/*!bsppacknrobase ||*/ !clientnrobase || !datacachenrobase || !enginenrobase || !filesystem_stdionrobase ||
		!inputsystemnrobase || !launchernrobase || !localizenrobase || !matchmakingnrobase || !materialsystemnrobase ||
		!scenefilecachenrobase || !servernrobase || /*!shaderapiemptynrobase ||*/ !soundemittersystemnrobase ||
		!studiorendernrobase || !tier0nrobase || !vgui2nrobase || !vguimatsurfacenrobase || !vphysicsnrobase ||
		!vscriptnrobase || !vstdlibnrobase) {
		return false; // Something failed
	}

	return true;
}

#endif // __SWITCH__
