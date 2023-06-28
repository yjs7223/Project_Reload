// Copyright (c), Sami Kangasmaa 2022

#pragma once

#include "CoreMinimal.h"
#include "Launch/Resources/Version.h"

// Simple macro to convert engine version into single integer (eg. 500 or 427)
#define COVER_ENGINE_VERSION (ENGINE_MAJOR_VERSION * 100 + ENGINE_MINOR_VERSION)