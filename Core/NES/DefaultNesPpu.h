#pragma once
#include "pch.h"
#include "NES/NesPpu.h"

class DefaultNesPpu final : public NesPpu<DefaultNesPpu>
{
public:
	DefaultNesPpu(NesConsole* console) : NesPpu(console)
	{
	}

	DefaultNesPpu(NesConsole* console, bool isSecondaryPpu) : NesPpu(console, isSecondaryPpu)
	{
	}

	__forceinline void StoreSpriteInformation(bool verticalMirror, uint16_t tileAddr, uint8_t lineOffset) { }
	__forceinline void StoreTileInformation() {}
	__forceinline bool RemoveSpriteLimit() { return _console->GetNesConfig().RemoveSpriteLimit; }
	__forceinline bool UseAdaptiveSpriteLimit() { return _console->GetNesConfig().AdaptiveSpriteLimit; }

	void* OnBeforeSendFrame() { return nullptr; }

	__forceinline void ProcessScanline()
	{
		ProcessScanlineImpl();
	}

	__forceinline void DrawPixel()
	{
		//This is called 3.7 million times per second - needs to be as fast as possible.
		if(IsRenderingEnabled() || ((_videoRamAddr & 0x3F00) != 0x3F00)) {
			uint32_t color = GetPixelColor();
			_currentOutputBuffer[(_scanline << 8) + _cycle - 1] = _paletteRam[color];
		} else {
			//"If the current VRAM address points in the range $3F00-$3FFF during forced blanking, the color indicated by this palette location will be shown on screen instead of the backdrop color."
			uint8_t color = _videoRamAddr & 0x1F;
			_extOutput = color & 0x0F;
			BaseNesPpu* ppu2 = _console->GetPpu2();
			if(!_isSecondaryPpu && IsExtInputMode() && ppu2 && ppu2->IsExtOutputMode()) {
				color = ppu2->GetExtOutput();
			}
			_currentOutputBuffer[(_scanline << 8) + _cycle - 1] = _paletteRam[color];
		}
	}
};
