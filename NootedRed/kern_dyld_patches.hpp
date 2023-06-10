//  Copyright © 2022-2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#pragma once
#include <Headers/kern_patcher.hpp>
#include <Headers/kern_util.hpp>

class DYLDPatches {
    public:
    static DYLDPatches *callback;

    void init();
    void processPatcher(KernelPatcher &patcher);

    private:
    mach_vm_address_t orgCsValidatePage {0};
    static void csValidatePage(vnode *vp, memory_object_t pager, memory_object_offset_t page_offset, const void *data,
        int *validated_p, int *tainted_p, int *nx_p);
};

/** VideoToolbox DRM model check */
static const char kVideoToolboxDRMModelOriginal[] = "MacPro5,1\0MacPro6,1\0IOService";

static const char kHwGvaId[] = "Mac-27AD2F918AE68F61";

/** AppleGVA model check */
static const char kAGVABoardIdOriginal[] = "board-id\0hw.model";
static const char kAGVABoardIdPatched[] = "hwgva-id";

static const char kCoreLSKDMSEPath[] = "/System/Library/PrivateFrameworks/CoreLSKDMSE.framework/Versions/A/CoreLSKDMSE";
static const char kCoreLSKDPath[] = "/System/Library/PrivateFrameworks/CoreLSKD.framework/Versions/A/CoreLSKD";

static const uint8_t kCoreLSKDOriginal[] = {0xC7, 0xC0, 0x01, 0x00, 0x00, 0x00, 0x0F, 0xA2};
static const uint8_t kCoreLSKDPatched[] = {0xC7, 0xC0, 0xC3, 0x06, 0x03, 0x00, 0x66, 0x90};

/** AppleGVAHEVCEncoder model check */
static const uint8_t kHEVCEncBoardIdOriginal[] = "vendor8bit\0IOService\0board-id";
static const uint8_t kHEVCEncBoardIdPatched[] = "vendor8bit\0IOService\0hwgva-id";

/**
 * `VAAcceleratorInfo::identify`
 * AMDRadeonVADriver2.bundle
 * The device info identification fails, as the device id is not present in the function.
 * Patch fallback "error" value (0x12) to Navi 10 (0xC).
 */
static const uint8_t kVAAcceleratorInfoIdentifyOriginal[] = {0x85, 0xC0, 0x74, 0x00, 0xBB, 0x12, 0x00, 0x00, 0x00, 0x89,
    0xD8, 0x48, 0x83, 0xC4, 0x00};
static const uint8_t kVAAcceleratorInfoIdentifyOriginalMask[] = {0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00};
static const uint8_t kVAAcceleratorInfoIdentifyPatched[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x0C};
static const uint8_t kVAAcceleratorInfoIdentifyPatchedMask[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};

/** Ditto */
static const uint8_t kVAAcceleratorInfoIdentifyVenturaOriginal[] = {0x48, 0xC7, 0x45, 0xF0, 0x18, 0x01, 0x00, 0x00,
    0xBB, 0x0B, 0x00, 0x00, 0x00, 0x83, 0xFE, 0x01, 0x75, 0x00};
static const uint8_t kVAAcceleratorInfoIdentifyVenturaOriginalMask[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00};
static const uint8_t kVAAcceleratorInfoIdentifyVenturaPatched[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEB, 0x00};
static const uint8_t kVAAcceleratorInfoIdentifyVenturaPatchedMask[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00};

/**
 * `VAFactory::createGraphicsEngine`
 * AMDRadeonVADriver2.bundle
 * Force use GFX 9 variant of the graphics engines
 */
static const uint8_t kVAFactoryCreateGraphicsEngineOriginal[] = {0x48, 0x8B, 0x86, 0x60, 0x04, 0x00, 0x00, 0x8B, 0x40,
    0x0C, 0x83, 0xF8, 0x07, 0x77, 0x00};
static const uint8_t kVAFactoryCreateGraphicsEngineMask[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0x00};
static const uint8_t kVAFactoryCreateGraphicsEnginePatched[] = {0xC7, 0xC0, 0x04, 0x00, 0x00, 0x00, 0x66, 0x90, 0x66,
    0x90};

/** Ditto */
static const uint8_t kVAFactoryCreateGraphicsEngineAndBltVenturaOriginal[] = {0x48, 0x8B, 0x86, 0x60, 0x04, 0x00, 0x00,
    0x8B, 0x40, 0x0C, 0x8D, 0x48, 0xFF, 0x83, 0xF9, 0x02, 0x72, 0x00, 0x8D, 0x48, 0xFD, 0x83, 0xF9, 0x02};
static const uint8_t kVAFactoryCreateGraphicsEngineAndBltVenturaMask[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

/**
 * `VAFactory::create*VP`
 * AMDRadeonVADriver2.bundle
 * Force use GFX 9 variants of the video processors
 */
static const uint8_t kVAFactoryCreateVPOriginal[] = {0x83, 0xFE, 0x07, 0x77, 0x00, 0x89, 0xF0, 0x48, 0x8D, 0x0D, 0x00,
    0x00, 0x00, 0x00, 0x48, 0x63, 0x04, 0x81, 0x48, 0x01, 0xC8, 0xFF, 0xE0, 0xBF, 0x00, 0x00, 0x00, 0x00, 0xE8, 0x00,
    0x00, 0x00, 0x00};
static const uint8_t kVAFactoryCreateVPMask[] = {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
    0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00,
    0x00, 0x00};
static const uint8_t kVAFactoryCreateVPPatched[] = {0xBE, 0x04, 0x00, 0x00, 0x00};

/** Ditto */
static const uint8_t kVAFactoryCreateVPVenturaOriginal[] = {0x8D, 0x46, 0xFF, 0x83, 0xF8, 0x02, 0x72, 0x00, 0x8D, 0x46,
    0xFD, 0x83, 0xF8, 0x02, 0x73, 0x00, 0xBF, 0x00, 0x00, 0x00, 0x00, 0xE8, 0x00, 0x00, 0x00, 0x00};
static const uint8_t kVAFactoryCreateVPVenturaOriginalMask[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00};
static const uint8_t kVAFactoryCreateVPVenturaPatched[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEB};
static const uint8_t kVAFactoryCreateVPVenturaPatchedMask[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};

/**
 * `VAFactory::createImageBlt`
 * AMDRadeonVADriver2.bundle
 * Force use GFX 9 variant of the image blitter
 */
static const uint8_t kVAFactoryCreateImageBltOriginal[] = {0x48, 0x89, 0xF7, 0x48, 0x8B, 0x86, 0x60, 0x04, 0x00, 0x00,
    0x8B, 0x40, 0x0C, 0x48, 0x83, 0xF8, 0x07, 0x77, 0x00, 0x48, 0x8D, 0x0D, 0x00, 0x00, 0x00, 0x00};
static const uint8_t kVAFactoryCreateImageBltMask[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00};
static const uint8_t kVAFactoryCreateImageBltPatched[] = {0x48, 0x89, 0xF7, 0x48, 0xB8, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00};

/**
 * `VAAddrLibInterface::init`
 * AMDRadeonVADriver2.bundle
 * Remove check for Vega family ID (0x8D) to correctly utilise GFX 9 AddrLib
 */
static const uint8_t kVAAddrLibInterfaceInitOriginal[] = {0x74, 0x00, 0x41, 0x81, 0xFC, 0x8D, 0x00, 0x00, 0x00, 0x75,
    0x00, 0xB8, 0x0D, 0x00, 0x00, 0x00};
static const uint8_t kVAAddrLibInterfaceInitOriginalMask[] = {0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static const uint8_t kVAAddrLibInterfaceInitPatched[] = {0x00, 0x00, 0x66, 0x90, 0x66, 0x90, 0x66, 0x90, 0x66, 0x90,
    0x90, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t kVAAddrLibInterfaceInitPatchedMask[] = {0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x00, 0x00, 0x00, 0x00, 0x00};

/**
 * `Vcn2DecCommand::writeUvdNoOp`
 * AMDRadeonVADriver2.bundle
 */
static const uint8_t kWriteUvdNoOpOriginal[] = {0x48, 0x8B, 0x07, 0xBE, 0x3F, 0x05, 0x00, 0x00, 0xFF, 0x50, 0x20};
static const uint8_t kWriteUvdNoOpPatched[] = {0x48, 0x8B, 0x07, 0xBE, 0xFF, 0x81, 0x00, 0x00, 0xFF, 0x50, 0x20};

/**
 * `Vcn2DecCommand::writeUvdEngineStart`
 * AMDRadeonVADriver2.bundle
 */
static const uint8_t kWriteUvdEngineStartOriginal[] = {0x48, 0x8B, 0x07, 0xBE, 0x06, 0x05, 0x00, 0x00, 0xFF, 0x50,
    0x20};
static const uint8_t kWriteUvdEngineStartPatched[] = {0x48, 0x8B, 0x07, 0xBE, 0xC6, 0x81, 0x00, 0x00, 0xFF, 0x50, 0x20};

/**
 * `Vcn2DecCommand::writeUvdGpcomVcpuCmd`
 * AMDRadeonVADriver2.bundle
 */
static const uint8_t kWriteUvdGpcomVcpuCmdOriginal[] = {0x48, 0x8B, 0x07, 0xBE, 0x03, 0x05, 0x00, 0x00, 0xFF, 0x50,
    0x20};
static const uint8_t kWriteUvdGpcomVcpuCmdPatched[] = {0x48, 0x8B, 0x07, 0xBE, 0xC3, 0x81, 0x00, 0x00, 0xFF, 0x50,
    0x20};

/**
 * `Vcn2DecCommand::writeUvdGpcomVcpuData0`
 * AMDRadeonVADriver2.bundle
 */
static const uint8_t kWriteUvdGpcomVcpuData0Original[] = {0x48, 0x8B, 0x07, 0xBE, 0x04, 0x05, 0x00, 0x00, 0xFF, 0x50,
    0x20};
static const uint8_t kWriteUvdGpcomVcpuData0Patched[] = {0x48, 0x8B, 0x07, 0xBE, 0xC4, 0x81, 0x00, 0x00, 0xFF, 0x50,
    0x20};

/**
 * `Vcn2DecCommand::writeUvdGpcomVcpuData1`
 * AMDRadeonVADriver2.bundle
 */
static const uint8_t kWriteUvdGpcomVcpuData1Original[] = {0x48, 0x8B, 0x07, 0xBE, 0x05, 0x05, 0x00, 0x00, 0xFF, 0x50,
    0x20};
static const uint8_t kWriteUvdGpcomVcpuData1Patched[] = {0x48, 0x8B, 0x07, 0xBE, 0xC5, 0x81, 0x00, 0x00, 0xFF, 0x50,
    0x20};

/**
 * `Vcn2EncCommand::addEncodePacket`
 * AMDRadeonVADriver2.bundle
 */
static const uint8_t kAddEncodePacketOriginal[] = {0x49, 0x89, 0x40, 0x18, 0xBE, 0x0F, 0x00, 0x00, 0x00, 0xBA, 0x2C,
    0x00, 0x00, 0x00};
static const uint8_t kAddEncodePacketPatched[] = {0x49, 0x89, 0x40, 0x18, 0xBE, 0x0B, 0x00, 0x00, 0x00, 0xBA, 0x2C,
    0x00, 0x00, 0x00};

/**
 * `Vcn2EncCommand::addSliceHeaderPacket`
 * AMDRadeonVADriver2.bundle
 */
static const uint8_t kAddSliceHeaderPacketOriginal[] = {0x00, 0x00, 0x00, 0xBE, 0x0B, 0x00, 0x00, 0x00, 0xBA, 0xC0,
    0x00, 0x00, 0x00, 0x00, 0xE9, 0x00, 0x00, 0x00, 0x00};
static const uint8_t kAddSliceHeaderPacketMask[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00};
static const uint8_t kAddSliceHeaderPacketPatched[] = {0x00, 0x00, 0x00, 0xBE, 0x0A, 0x00, 0x00, 0x00, 0xBA, 0xC0, 0x00,
    0x00, 0x00, 0x00, 0xE9, 0x00, 0x00, 0x00, 0x00};

/**
 * `Vcn2EncCommand::addIntraRefreshPacket`
 * AMDRadeonVADriver2.bundle
 */
static const uint8_t kAddIntraRefreshPacketOriginal[] = {0x01, 0x00, 0x00, 0xBE, 0x10, 0x00, 0x00, 0x00, 0xBA, 0x0C,
    0x00, 0x00, 0x00, 0x00, 0xE9, 0x00, 0x00, 0x00, 0x00};
static const uint8_t kAddIntraRefreshPacketMask[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00};
static const uint8_t kAddIntraRefreshPacketPatched[] = {0x01, 0x00, 0x00, 0xBE, 0x0C, 0x00, 0x00, 0x00, 0xBA, 0x0C,
    0x00, 0x00, 0x00, 0x00, 0xE9, 0x00, 0x00, 0x00, 0x00};

/**
 * `Vcn2EncCommand::addContextBufferPacket`
 * AMDRadeonVADriver2.bundle
 */
static const uint8_t kAddContextBufferPacketOriginal[] = {0x49, 0x89, 0x40, 0x18, 0x41, 0xC7, 0x40, 0x14, 0x01, 0x00,
    0x00, 0x00, 0xBE, 0x11, 0x00, 0x00, 0x00, 0xBA, 0x58, 0x02, 0x00, 0x00};
static const uint8_t kAddContextBufferPacketPatched[] = {0x49, 0x89, 0x40, 0x18, 0x41, 0xC7, 0x40, 0x14, 0x01, 0x00,
    0x00, 0x00, 0xBE, 0x0D, 0x00, 0x00, 0x00, 0xBA, 0x58, 0x02, 0x00, 0x00};

/**
 * `Vcn2EncCommand::addBitstreamBufferPacket`
 * AMDRadeonVADriver2.bundle
 */
static const uint8_t kAddBitstreamBufferPacketOriginal[] = {0x48, 0x8B, 0x46, 0x38, 0x49, 0x89, 0x40, 0x18, 0xBE, 0x12,
    0x00, 0x00, 0x00, 0xBA, 0x14, 0x00, 0x00, 0x00};
static const uint8_t kAddBitstreamBufferPacketPatched[] = {0x48, 0x8B, 0x46, 0x38, 0x49, 0x89, 0x40, 0x18, 0xBE, 0x0E,
    0x00, 0x00, 0x00, 0xBA, 0x14, 0x00, 0x00, 0x00};

/**
 * `Vcn2EncCommand::addFeedbackBufferPacket`
 * AMDRadeonVADriver2.bundle
 */
static const uint8_t kAddFeedbackBufferPacketOriginal[] = {0x48, 0x8B, 0x46, 0x40, 0x49, 0x89, 0x40, 0x18, 0xBE, 0x15,
    0x00, 0x00, 0x00, 0xBA, 0x14, 0x00, 0x00, 0x00};
static const uint8_t kAddFeedbackBufferPacketPatched[] = {0x48, 0x8B, 0x46, 0x40, 0x49, 0x89, 0x40, 0x18, 0xBE, 0x10,
    0x00, 0x00, 0x00, 0xBA, 0x14, 0x00, 0x00, 0x00};

/**
 * `Vcn2EncCommand::addInputFormatPacket` and `Vcn2EncCommand::addOutputFormatPacket`
 * AMDRadeonVADriver2.bundle
 * VCN 1 does not have these packets, therefore we make these methods do nothing
 */
static const uint8_t kAddInputFormatPacketOriginal[] = {0x55, 0x48, 0x89, 0xE5, 0x48, 0x8D, 0x80, 0x80, 0x05, 0x00,
    0x00, 0xBE, 0x0C, 0x00, 0x00, 0x00, 0xBA, 0x1C, 0x00, 0x00, 0x00};
static const uint8_t kAddOutputFormatPacketOriginal[] = {0x55, 0x48, 0x89, 0xE5, 0x48, 0x8D, 0x80, 0xA0, 0x05, 0x00,
    0x00, 0xBE, 0x0D, 0x00, 0x00, 0x00, 0xBA, 0x10, 0x00, 0x00, 0x00};
static const uint8_t kAddFormatPacketMask[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0xF0, 0xFF, 0x00, 0x00, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static const uint8_t kRetZero[] = {0x48, 0xC7, 0xC0, 0x00, 0x00, 0x00, 0x00, 0xC3, 0x90};
