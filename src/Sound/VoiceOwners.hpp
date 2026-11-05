#pragma once

#include "../extern/dtl/array.hpp"

struct EFESubEffect;

// Sound's shared resident state; only sound operations may read or publish it.
namespace SoundInternal
{
    extern dtl::array<EFESubEffect* volatile, 14> soundOwners;
}
