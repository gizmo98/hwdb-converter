// Copyright (c) 2022-Present Stefan Eschenbacher. All rights reserved.
//
// This vivante/verisilicon hwdb converter code, is dual-licensed under GNU General Public License 
// version 2 ("GPL") and the Apache License version 2 ("ASL"). For the GPL, 
// please see LICENSE. For the ASL, you may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND,
// either express or implied. See the LICENSE file for specific language governing
// rights and limitations of this software.

#define ESTIMATED_VALUE

#include <iostream>
#include <iomanip>
#include "common.xml.h"
#include "gc_feature_database.h"
#include "gc_feature_database_stm32.h"
#include "gc_feature_database_stm32_6_4_7.h"
#include <iostream>
#include <fstream>
using namespace std;

typedef struct  {
	int chipID;
	int chipVersion;
	int productID;
	int ecoID;
	int customerID;
} chips;

typedef struct {
	int model;
	int revision;
	int product_id;
	int customer_id;
	int eco_id;
	int stream_count;
	int register_max;
	int thread_count;
	int shader_core_count;
	int vertex_cache_size;
	int vertex_output_buffer_size;
	int pixel_pipes;
	int instruction_count;
	int num_constants;
	int buffer_size;
	int varyings_count;
	int features;
	int minor_features0;
	int minor_features1;
	int minor_features2;
	int minor_features3;
	int minor_features4;
	int minor_features5;
	int minor_features6;
	int minor_features7;
	int minor_features8;
	int minor_features9;
	int minor_features10;
	int minor_features11;
	int minor_features12;
	int minor_features13;
} linux_chips;

int main()
{
	const int device_count = (sizeof(gChipInfo) / sizeof(gChipInfo[0])) + (sizeof(gChipInfo_STM32_6_2_4) / sizeof(gChipInfo_STM32_6_2_4[0])) +(sizeof(gChipInfo_STM32_4_6_7) / sizeof(gChipInfo_STM32_4_6_7[0]));
	
	int count = 0;

	chips devices[device_count];
	linux_chips linux_devices[device_count];

	std::cout <<"Device entries: " << device_count << "\n";
	std::cout << "\n";

	// get device entries from NXP's gc_features_database.h 6.4.3 (2022-04-14) 
	size_t n = sizeof(gChipInfo) / sizeof(gChipInfo[0]);
	for (int i = 0; i < n; i++) {
		bool found = false;

		for (int j = 0; j < device_count; ++j)
		{

			if ((devices[j].chipID == gChipInfo[i].chipID)
				&& (devices[j].chipVersion == gChipInfo[i].chipVersion)
				&& (devices[j].productID == gChipInfo[i].productID)
				&& (devices[j].ecoID == gChipInfo[i].ecoID)
				&& (devices[j].customerID == gChipInfo[i].customerID)
				)
			{
				found = true;
			}
		}

		if (found == false) {
			devices[count].chipID = gChipInfo[i].chipID;
			devices[count].chipVersion = gChipInfo[i].chipVersion;
			devices[count].productID = gChipInfo[i].productID;
			devices[count].ecoID = gChipInfo[i].ecoID;
			devices[count].customerID = gChipInfo[i].customerID;

			linux_devices[count].model= gChipInfo[i].chipID;
			linux_devices[count].revision = gChipInfo[i].chipVersion;
			linux_devices[count].product_id = gChipInfo[i].productID;
			linux_devices[count].customer_id = gChipInfo[i].customerID;
			linux_devices[count].eco_id = gChipInfo[i].ecoID;

			linux_devices[count].stream_count = gChipInfo[i].Streams;
			linux_devices[count].register_max = gChipInfo[i].TempRegisters;
			linux_devices[count].thread_count = gChipInfo[i].ThreadCount;
			linux_devices[count].shader_core_count = gChipInfo[i].NumShaderCores;
			linux_devices[count].vertex_cache_size = gChipInfo[i].VertexCacheSize;
			linux_devices[count].vertex_output_buffer_size = gChipInfo[i].VertexOutputBufferSize;
			linux_devices[count].pixel_pipes = gChipInfo[i].NumPixelPipes;
			linux_devices[count].instruction_count = gChipInfo[i].InstructionCount;
			linux_devices[count].num_constants = gChipInfo[i].NumberOfConstants;
			linux_devices[count].buffer_size = gChipInfo[i].BufferSize;
			linux_devices[count].varyings_count = gChipInfo[i].VaryingCount;

			uint32_t feature = (gChipInfo[i].REG_FastClear == 1 ? chipFeatures_FAST_CLEAR : 0) |
				(gChipInfo[i].REG_SpecialAntiAliasing == 1 ? chipFeatures_SPECIAL_ANTI_ALIASING : 0) |
				(gChipInfo[i].REG_Pipe3D == 1 ? chipFeatures_PIPE_3D : 0) |
				(gChipInfo[i].REG_DXTTextureCompression == 1 ? chipFeatures_DXT_TEXTURE_COMPRESSION : 0) |
				(gChipInfo[i].REG_DebugMode == 1 ? chipFeatures_DEBUG_MODE : 0) |
				(gChipInfo[i].REG_ZCompression == 1 ? chipFeatures_Z_COMPRESSION : 0) |
				(gChipInfo[i].REG_YUV420Filter == 1 ? chipFeatures_YUV420_SCALER : 0) |
				(gChipInfo[i].REG_MSAA == 1 ? chipFeatures_MSAA : 0) |
				(gChipInfo[i].REG_DC == 1 ? chipFeatures_DC : 0) |
				(gChipInfo[i].REG_Pipe2D == 1 ? chipFeatures_PIPE_2D : 0) |
				(gChipInfo[i].REG_ETC1TextureCompression == 1 ? chipFeatures_ETC1_TEXTURE_COMPRESSION : 0) |
				(gChipInfo[i].REG_FastScaler == 1 ? chipFeatures_FAST_SCALER : 0) |
				(gChipInfo[i].REG_HighDynamicRange == 1 ? chipFeatures_HIGH_DYNAMIC_RANGE : 0) |
				(gChipInfo[i].REG_YUV420Tiler == 1 ? chipFeatures_YUV420_TILER : 0) |
				(gChipInfo[i].REG_ModuleCG == 1 ? chipFeatures_MODULE_CG : 0) |
				(gChipInfo[i].REG_MinArea == 1 ? chipFeatures_MIN_AREA : 0) |
				(gChipInfo[i].REG_NoEZ == 1 ? chipFeatures_NO_EARLY_Z : 0) |
				(gChipInfo[i].REG_No422Texture == 1 ? chipFeatures_NO_422_TEXTURE : 0) |
				(gChipInfo[i].REG_BufferInterleaving == 1 ? chipFeatures_BUFFER_INTERLEAVING : 0) |
				(gChipInfo[i].REG_ByteWrite2D == 1 ? chipFeatures_BYTE_WRITE_2D : 0) |
				(gChipInfo[i].REG_NoScaler == 1 ? chipFeatures_NO_SCALER : 0) |
				(gChipInfo[i].REG_YUY2Averaging == 1 ? chipFeatures_YUY2_AVERAGING : 0) |
				(gChipInfo[i].REG_HalfPECache == 1 ? chipFeatures_HALF_PE_CACHE : 0) |
				(gChipInfo[i].REG_HalfTXCache == 1 ? chipFeatures_HALF_TX_CACHE : 0) |
				(gChipInfo[i].REG_YUY2RenderTarget == 1 ? chipFeatures_YUY2_RENDER_TARGET : 0) |
				(gChipInfo[i].REG_Mem32BitSupport == 1 ? chipFeatures_MEM32 : 0) |
				(gChipInfo[i].REG_PipeVG == 1 ? chipFeatures_PIPE_VG : 0) |
				(gChipInfo[i].REG_VGTS == 1 ? chipFeatures_VGTS : 0) |
				(gChipInfo[i].REG_FE20 == 1 ? chipFeatures_FE20 : 0) |
				(gChipInfo[i].REG_ByteWrite3D == 1 ? chipFeatures_BYTE_WRITE_3D : 0) |
				(gChipInfo[i].REG_RsYuvTarget == 1 ? chipFeatures_RS_YUV_TARGET : 0) |
				(gChipInfo[i].REG_FE20BitIndex == 1 ? chipFeatures_32_BIT_INDICES : 0);

			uint32_t features0 =
				(gChipInfo[i].REG_FlipY == 1 ? chipMinorFeatures0_FLIP_Y : 0) |
				(gChipInfo[i].REG_DualReturnBus == 1 ? chipMinorFeatures0_DUAL_RETURN_BUS : 0) |
				(gChipInfo[i].REG_EndiannessConfig == 1 ? chipMinorFeatures0_ENDIANNESS_CONFIG : 0) |
				(gChipInfo[i].REG_Texture8K == 1 ? chipMinorFeatures0_TEXTURE_8K : 0) |
				(gChipInfo[i].REG_CorrectTextureConverter == 1 ? chipMinorFeatures0_CORRECT_TEXTURE_CONVERTER : 0) |
				(gChipInfo[i].REG_SpecialMsaaLod == 1 ? chipMinorFeatures0_SPECIAL_MSAA_LOD : 0) |
				(gChipInfo[i].REG_FastClearFlush == 1 ? chipMinorFeatures0_FAST_CLEAR_FLUSH : 0) |
				(gChipInfo[i].REG_2DPE20 == 1 ? chipMinorFeatures0_2DPE20 : 0) |
				(gChipInfo[i].REG_CorrectAutoDisable == 1 ? chipMinorFeatures0_CORRECT_AUTO_DISABLE : 0) |
				(gChipInfo[i].REG_Render8K == 1 ? chipMinorFeatures0_RENDERTARGET_8K : 0) |
				(gChipInfo[i].REG_TileStatus2Bits == 1 ? chipMinorFeatures0_2BITPERTILE : 0) |
				(gChipInfo[i].REG_SeparateTileStatusWhenInterleaved == 1 ? chipMinorFeatures0_SEPARATE_TILE_STATUS_WHEN_INTERLEAVED : 0) |
				(gChipInfo[i].REG_SuperTiled32x32 == 1 ? chipMinorFeatures0_SUPER_TILED : 0) |
				(gChipInfo[i].REG_VG20 == 1 ? chipMinorFeatures0_VG_20 : 0) |
				(gChipInfo[i].REG_TSExtendedCommands == 1 ? chipMinorFeatures0_TS_EXTENDED_COMMANDS : 0) |
				(gChipInfo[i].REG_CompressionFifoFixed == 1 ? chipMinorFeatures0_COMPRESSION_FIFO_FIXED : 0) |
#ifdef ESTIMATED_VALUE
				(gChipInfo[i].REG_ExtraShaderInstructions0 == 1 ? chipMinorFeatures0_HAS_SIGN_FLOOR_CEIL : 0) | //?
				(gChipInfo[i].REG_ExtraShaderInstructions1 == 1 ? chipMinorFeatures0_HAS_SQRT_TRIG : 0) | //?
				(gChipInfo[i].REG_DefaultReg0 == 1 ? chipMinorFeatures0_MORE_MINOR_FEATURES : 0) | //?
#endif
				(gChipInfo[i].REG_VGFilter == 1 ? chipMinorFeatures0_VG_FILTER : 0) |
				(gChipInfo[i].REG_VG21 == 1 ? chipMinorFeatures0_VG_21 : 0) |
				(gChipInfo[i].REG_ShaderGetsW == 1 ? chipMinorFeatures0_SHADER_HAS_W : 0) |
				(gChipInfo[i].REG_MC20 == 1 ? chipMinorFeatures0_MC20 : 0) |
				(gChipInfo[i].REG_ShaderMSAASideband == 1 ? chipMinorFeatures0_MSAA_SIDEBAND : 0) |
				(gChipInfo[i].REG_BugFixes0 == 1 ? chipMinorFeatures0_BUG_FIXES0 : 0) |
				(gChipInfo[i].REG_VAA == 1 ? chipMinorFeatures0_VAA : 0) |
				(gChipInfo[i].REG_BypassInMSAA == 1 ? chipMinorFeatures0_BYPASS_IN_MSAA : 0) |
				(gChipInfo[i].REG_HierarchicalZ == 1 ? chipMinorFeatures0_HZ : 0) |
				(gChipInfo[i].REG_NewTexture == 1 ? chipMinorFeatures0_NEW_TEXTURE : 0) |
				(gChipInfo[i].REG_A8TargetSupport == 1 ? chipMinorFeatures0_2D_A8_TARGET : 0) |
				(gChipInfo[i].REG_CorrectStencil == 1 ? chipMinorFeatures0_CORRECT_STENCIL : 0) |
				(gChipInfo[i].REG_EnhanceVR == 1 ? chipMinorFeatures0_ENHANCE_VR : 0);

			uint32_t features1 =
				(gChipInfo[i].REG_RSUVSwizzle == 1 ? chipMinorFeatures1_RSUV_SWIZZLE : 0) |
				(gChipInfo[i].REG_V2Compression == 1 ? chipMinorFeatures1_V2_COMPRESSION : 0) |
				(gChipInfo[i].REG_VGDoubleBuffer == 1 ? chipMinorFeatures1_VG_DOUBLE_BUFFER : 0) |
				(gChipInfo[i].REG_BugFixes1 == 1 ? chipMinorFeatures1_EXTRA_EVENT_STATES : 0) |
				(gChipInfo[i].REG_BugFixes2 == 1 ? chipMinorFeatures1_NO_STRIPING_NEEDED : 0) |
				(gChipInfo[i].REG_TextureStride == 1 ? chipMinorFeatures1_TEXTURE_STRIDE : 0) |
				(gChipInfo[i].REG_BugFixes3 == 1 ? chipMinorFeatures1_BUG_FIXES3 : 0) |
				(gChipInfo[i].REG_CorrectAutoDisable1 == 1 ? chipMinorFeatures1_AUTO_DISABLE : 0) |
				(gChipInfo[i].REG_AutoRestartTS == 1 ? chipMinorFeatures1_AUTO_RESTART_TS : 0) |
				(gChipInfo[i].REG_BugFixes4 == 1 ? chipMinorFeatures1_DISABLE_PE_GATING : 0) |
				(gChipInfo[i].REG_L2Windowing == 1 ? chipMinorFeatures1_L2_WINDOWING : 0) |
				(gChipInfo[i].REG_HalfFloatPipe == 1 ? chipMinorFeatures1_HALF_FLOAT : 0) |
				(gChipInfo[i].REG_PixelDither == 1 ? chipMinorFeatures1_PIXEL_DITHER : 0) |
				(gChipInfo[i].REG_TwoStencilReference == 1 ? chipMinorFeatures1_TWO_STENCIL_REFERENCE : 0) |
				(gChipInfo[i].REG_ExtendedPixelFormat == 1 ? chipMinorFeatures1_EXTENDED_PIXEL_FORMAT : 0) |
				(gChipInfo[i].REG_CorrectMinMaxDepth == 1 ? chipMinorFeatures1_CORRECT_MIN_MAX_DEPTH : 0) |
				(gChipInfo[i].REG_DitherAndFilterPlusAlpha2D == 1 ? chipMinorFeatures1_2D_DITHER : 0) |
				(gChipInfo[i].REG_BugFixes5 == 1 ? chipMinorFeatures1_BUG_FIXES5 : 0) |
				(gChipInfo[i].REG_New2D == 1 ? chipMinorFeatures1_NEW_2D : 0) |
				(gChipInfo[i].REG_NewFloatingPointArithmetic == 1 ? chipMinorFeatures1_NEW_FP : 0) |
				(gChipInfo[i].REG_TextureHorizontalAlignmentSelect == 1 ? chipMinorFeatures1_TEXTURE_HALIGN : 0) |
				(gChipInfo[i].REG_NonPowerOfTwo == 1 ? chipMinorFeatures1_NON_POWER_OF_TWO : 0) |
				(gChipInfo[i].REG_LinearTextureSupport == 1 ? chipMinorFeatures1_LINEAR_TEXTURE_SUPPORT : 0) |
				(gChipInfo[i].REG_Halti0 == 1 ? chipMinorFeatures1_HALTI0 : 0) |
				(gChipInfo[i].REG_CorrectOverflowVG == 1 ? chipMinorFeatures1_CORRECT_OVERFLOW_VG : 0) |
				(gChipInfo[i].REG_NegativeLogFix == 1 ? chipMinorFeatures1_NEGATIVE_LOG_FIX : 0) |
				(gChipInfo[i].REG_ResolveOffset == 1 ? chipMinorFeatures1_RESOLVE_OFFSET : 0) |
				(gChipInfo[i].REG_OkToGateAxiClock == 1 ? chipMinorFeatures1_OK_TO_GATE_AXI_CLOCK : 0) |
				(gChipInfo[i].REG_MMU == 1 ? chipMinorFeatures1_MMU_VERSION : 0) |
				(gChipInfo[i].REG_WideLine == 1 ? chipMinorFeatures1_WIDE_LINE : 0) |
				(gChipInfo[i].REG_BugFixes6 == 1 ? chipMinorFeatures1_BUG_FIXES6 : 0) |
				(gChipInfo[i].REG_FcFlushStall == 1 ? chipMinorFeatures1_FC_FLUSH_STALL : 0);

			uint32_t features2 = (gChipInfo[i].REG_LineLoop == 1 ? chipMinorFeatures2_LINE_LOOP : 0) |
				(gChipInfo[i].REG_LogicOp == 1 ? chipMinorFeatures2_LOGIC_OP : 0) |
				(gChipInfo[i].REG_SeamlessCubeMap == 1 ? chipMinorFeatures2_SEAMLESS_CUBE_MAP : 0) |
				(gChipInfo[i].REG_SuperTiledTexture == 1 ? chipMinorFeatures2_SUPERTILED_TEXTURE : 0) |
				(gChipInfo[i].REG_LinearPE == 1 ? chipMinorFeatures2_LINEAR_PE : 0) |
				(gChipInfo[i].REG_RectPrimitive == 1 ? chipMinorFeatures2_RECT_PRIMITIVE : 0) |
				(gChipInfo[i].REG_Composition == 1 ? chipMinorFeatures2_COMPOSITION : 0) |
				(gChipInfo[i].REG_CorrectAutoDisableCountWidth == 1 ? chipMinorFeatures2_CORRECT_AUTO_DISABLE_COUNT : 0) |
				(gChipInfo[i].REG_PESwizzle == 1 ? chipMinorFeatures2_PE_SWIZZLE : 0) |
				(gChipInfo[i].REG_EndEvent == 1 ? chipMinorFeatures2_END_EVENT : 0) |
				(gChipInfo[i].REG_S1S8 == 1 ? chipMinorFeatures2_S1S8 : 0) |
				(gChipInfo[i].REG_Halti1 == 1 ? chipMinorFeatures2_HALTI1 : 0) |
				(gChipInfo[i].REG_RGB888 == 1 ? chipMinorFeatures2_RGB888 : 0) |
				(gChipInfo[i].REG_TX_YUVAssembler == 1 ? chipMinorFeatures2_TX__YUV_ASSEMBLER : 0) |
				(gChipInfo[i].REG_DynamicFrequencyScaling == 1 ? chipMinorFeatures2_DYNAMIC_FREQUENCY_SCALING : 0) |
				(gChipInfo[i].REG_TXFilter == 1 ? chipMinorFeatures2_TX_FILTER : 0) |
				(gChipInfo[i].REG_FullDirectFB == 1 ? chipMinorFeatures2_FULL_DIRECTFB : 0) |
				(gChipInfo[i].REG_OnePass2DFilter == 1 ? chipMinorFeatures2_2D_TILING : 0) |
				(gChipInfo[i].REG_ThreadWalkerInPS == 1 ? chipMinorFeatures2_THREAD_WALKER_IN_PS : 0) |
				(gChipInfo[i].REG_TileFiller == 1 ? chipMinorFeatures2_TILE_FILLER : 0) |
				(gChipInfo[i].REG_YUVStandard == 1 ? chipMinorFeatures2_YUV_STANDARD : 0) |
				(gChipInfo[i].REG_MultiSourceBlt == 1 ? chipMinorFeatures2_2D_MULTI_SOURCE_BLIT : 0) |
				(gChipInfo[i].REG_YUVConversion == 1 ? chipMinorFeatures2_YUV_CONVERSION : 0) |
				(gChipInfo[i].REG_FlushFixed2D == 1 ? chipMinorFeatures2_FLUSH_FIXED_2D : 0) |
				(gChipInfo[i].REG_Interleaver == 1 ? chipMinorFeatures2_INTERLEAVER : 0) |
				(gChipInfo[i].REG_MixedStreams == 1 ? chipMinorFeatures2_MIXED_STREAMS : 0) |
				(gChipInfo[i].REG_L2CacheFor2D420 == 1 ? chipMinorFeatures2_2D_420_L2CACHE : 0) |
				(gChipInfo[i].REG_BugFixes7 == 1 ? chipMinorFeatures2_BUG_FIXES7 : 0) |
				(gChipInfo[i].REG_NoIndexPattern == 1 ? chipMinorFeatures2_2D_NO_INDEX8_BRUSH : 0) |
				(gChipInfo[i].REG_TextureTileStatus == 1 ? chipMinorFeatures2_TEXTURE_TILED_READ : 0) |
				(gChipInfo[i].REG_DecompressZ16 == 1 ? chipMinorFeatures2_DECOMPRESS_Z16 : 0) |
				(gChipInfo[i].REG_BugFixes8 == 1 ? chipMinorFeatures2_BUG_FIXES8 : 0);

			uint32_t features3 = (gChipInfo[i].REG_DERotationStallFix == 1 ? chipMinorFeatures3_ROTATION_STALL_FIX : 0) |
				(gChipInfo[i].REG_OclOnly == 1 ? chipMinorFeatures3_OCL_ONLY : 0) |
#ifdef ESTIMATED_VALUE
				(gChipInfo[i].REG_NewFeatures0 == 1 ? chipMinorFeatures3_2D_MULTI_SOURCE_BLT_EX : 0) |
#endif
				(gChipInfo[i].REG_InstructionCache == 1 ? chipMinorFeatures3_INSTRUCTION_CACHE : 0) |
				(gChipInfo[i].REG_GeometryShader == 1 ? chipMinorFeatures3_GEOMETRY_SHADER : 0) |
				(gChipInfo[i].REG_TexCompressionSupertiled == 1 ? chipMinorFeatures3_TEX_COMPRESSION_SUPERTILED : 0) |
				(gChipInfo[i].REG_Generics == 1 ? chipMinorFeatures3_GENERICS : 0) |
				(gChipInfo[i].REG_BugFixes9 == 1 ? chipMinorFeatures3_BUG_FIXES9 : 0) |
				(gChipInfo[i].REG_FastMSAA == 1 ? chipMinorFeatures3_FAST_MSAA : 0) |
				(gChipInfo[i].REG_WClip == 1 ? chipMinorFeatures3_WCLIP : 0) |
				(gChipInfo[i].REG_BugFixes10 == 1 ? chipMinorFeatures3_BUG_FIXES10 : 0) |
				(gChipInfo[i].REG_UnifiedSamplers == 1 ? chipMinorFeatures3_UNIFIED_SAMPLERS : 0) |
				(gChipInfo[i].REG_BugFixes11 == 1 ? chipMinorFeatures3_BUG_FIXES11 : 0) |
				(gChipInfo[i].REG_PerformanceCounters == 1 ? chipMinorFeatures3_PERFORMANCE_COUNTERS : 0) |
				(gChipInfo[i].REG_ExtraShaderInstructions2 == 1 ? chipMinorFeatures3_HAS_FAST_TRANSCENDENTALS : 0) |
				(gChipInfo[i].REG_BugFixes12 == 1 ? chipMinorFeatures3_BUG_FIXES12 : 0) |
				(gChipInfo[i].REG_BugFixes13 == 1 ? chipMinorFeatures3_BUG_FIXES13 : 0) |
				(gChipInfo[i].REG_DEEnhancements1 == 1 ? chipMinorFeatures3_DE_ENHANCEMENTS1 : 0) |
				(gChipInfo[i].REG_ACE == 1 ? chipMinorFeatures3_ACE : 0) |
				(gChipInfo[i].REG_TXEnhancements1 == 1 ? chipMinorFeatures3_TX_ENHANCEMENTS1 : 0) |
				(gChipInfo[i].REG_SHEnhancements1 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS1 : 0) |
				(gChipInfo[i].REG_SHEnhancements2 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS2 : 0) |
				(gChipInfo[i].REG_PEEnhancements1 == 1 ? chipMinorFeatures3_PE_ENHANCEMENTS1 : 0) |
				(gChipInfo[i].REG_DEEnhancements2 == 1 ? chipMinorFeatures3_2D_FC_SOURCE : 0) |
				(gChipInfo[i].REG_BugFixes14 == 1 ? chipMinorFeatures3_BUG_FIXES_14 : 0) |
				(gChipInfo[i].REG_PowerOptimizations0 == 1 ? chipMinorFeatures3_POWER_OPTIMIZATIONS_0 : 0) |
				(gChipInfo[i].REG_NewHZ == 1 ? chipMinorFeatures3_NEW_HZ : 0) |
				(gChipInfo[i].REG_BugFixes15 == 1 ? chipMinorFeatures3_PE_DITHER_FIX : 0) |
				(gChipInfo[i].REG_DEEnhancements3 == 1 ? chipMinorFeatures3_DE_ENHANCEMENTS3 : 0) |
				(gChipInfo[i].REG_SHEnhancements3 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS3 : 0) |
				(gChipInfo[i].REG_SHEnhancements4 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS4 : 0) |
				(gChipInfo[i].REG_TXEnhancements2 == 1 ? chipMinorFeatures3_TX_ENHANCEMENTS2 : 0);

			uint32_t features4 = (gChipInfo[i].REG_FEEnhancements1 == 1 ? chipMinorFeatures4_FE_ENHANCEMENTS1 : 0) |
				(gChipInfo[i].REG_PEEnhancements2 == 1 ? chipMinorFeatures4_PE_ENHANCEMENTS2 : 0) |
				(gChipInfo[i].REG_PAEnhancements1 == 1 ? chipMinorFeatures4_FRUSTUM_CLIP_FIX : 0) |
				(gChipInfo[i].REG_DENoGamma == 1 ? chipMinorFeatures4_DE_NO_GAMMA : 0) |
				(gChipInfo[i].REG_PAEnhancements2 == 1 ? chipMinorFeatures4_PA_ENHANCEMENTS_2 : 0) |
				(gChipInfo[i].REG_DEEnhancements4 == 1 ? chipMinorFeatures4_2D_GAMMA : 0) |
				(gChipInfo[i].REG_PEEnhancements3 == 1 ? chipMinorFeatures4_SINGLE_BUFFER : 0) |
				(gChipInfo[i].REG_HIEnhancements1 == 1 ? chipMinorFeatures4_HI_ENHANCEMENTS_1 : 0) |
				(gChipInfo[i].REG_TXEnhancements3 == 1 ? chipMinorFeatures4_TX_ENHANCEMENTS_3 : 0) |
				(gChipInfo[i].REG_SHEnhancements5 == 1 ? chipMinorFeatures4_SH_ENHANCEMENTS_5 : 0) |
				(gChipInfo[i].REG_FEEnhancements2 == 1 ? chipMinorFeatures4_FE_ENHANCEMENTS_2 : 0) |
				(gChipInfo[i].REG_BugFixes16 == 1 ? chipMinorFeatures4_TX_LERP_PRECISION_FIX : 0) |
				(gChipInfo[i].REG_DEEnhancements5 == 1 ? chipMinorFeatures4_2D_COLOR_SPACE_CONVERSION : 0) |
				(gChipInfo[i].REG_TXEnhancements4 == 1 ? chipMinorFeatures4_TEXTURE_ASTC : 0) |
				(gChipInfo[i].REG_PEEnhancements4 == 1 ? chipMinorFeatures4_PE_ENHANCEMENTS_4 : 0) |
				(gChipInfo[i].REG_MCEnhancements1 == 1 ? chipMinorFeatures4_MC_ENHANCEMENTS_1 : 0) |
				(gChipInfo[i].REG_Halti2 == 1 ? chipMinorFeatures4_HALTI2 : 0) |
				(gChipInfo[i].REG_DEMirrorRotate == 1 ? chipMinorFeatures4_2D_MIRROR_EXTENSION : 0) |
				(gChipInfo[i].REG_SmallMSAA == 1 ? chipMinorFeatures4_SMALL_MSAA : 0) |
				(gChipInfo[i].REG_BugFixes17 == 1 ? chipMinorFeatures4_BUG_FIXES_17 : 0) |
				(gChipInfo[i].REG_Rasterizer2 == 1 ? chipMinorFeatures4_NEW_RA : 0) |
				(gChipInfo[i].REG_DualPipeOPF == 1 ? chipMinorFeatures4_2D_OPF_YUV_OUTPUT : 0) |
				(gChipInfo[i].REG_MultiSrcV2 == 1 ? chipMinorFeatures4_2D_MULTI_SOURCE_BLT_EX2 : 0) |
				(gChipInfo[i].REG_CSCV2 == 1 ? chipMinorFeatures4_NO_USER_CSC : 0) |
				(gChipInfo[i].REG_PAEnhancements3 == 1 ? chipMinorFeatures4_ZFIXES : 0) |
				(gChipInfo[i].REG_BugFixes18 == 1 ? chipMinorFeatures4_BUG_FIXES18 : 0) |
				(gChipInfo[i].REG_Compression2D == 1 ? chipMinorFeatures4_2D_COMPRESSION : 0) |
				(gChipInfo[i].REG_Probe == 1 ? chipMinorFeatures4_PROBE : 0) |
				(gChipInfo[i].REG_MediumPrecision == 1 ? chipMinorFeatures4_MEDIUM_PRECISION : 0) |
				(gChipInfo[i].REG_DESupertile == 1 ? chipMinorFeatures4_2D_SUPER_TILE_VERSION : 0) |
				(gChipInfo[i].REG_BugFixes19 == 1 ? chipMinorFeatures4_BUG_FIXES19 : 0) |
				(gChipInfo[i].REG_SHEnhancements6 == 1 ? chipMinorFeatures4_SH_ENHANCEMENTS6 : 0);

			uint32_t features5 = (gChipInfo[i].REG_SHEnhancements7 == 1 ? chipMinorFeatures5_SH_ENHANCEMENTS7 : 0) |
				(gChipInfo[i].REG_BugFixes20 == 1 ? chipMinorFeatures5_BUG_FIXES20 : 0) |
				(gChipInfo[i].REG_DEAddress40 == 1 ? chipMinorFeatures5_DE_ADDRESS_40 : 0) |
				(gChipInfo[i].REG_MiniMMUFix == 1 ? chipMinorFeatures5_MINI_MMU_FIX : 0) |
				(gChipInfo[i].REG_EEZ == 1 ? chipMinorFeatures5_EEZ : 0) |
				(gChipInfo[i].REG_BugFixes21 == 1 ? chipMinorFeatures5_BUG_FIXES21 : 0) |
				(gChipInfo[i].REG_ExtraVgCaps == 1 ? chipMinorFeatures5_EXTRA_VG_CAPS : 0) |
				(gChipInfo[i].REG_MultiSrcV15 == 1 ? chipMinorFeatures5_MULTI_SRC_V15 : 0) |
				(gChipInfo[i].REG_BugFixes22 == 1 ? chipMinorFeatures5_BUG_FIXES22 : 0) |
				(gChipInfo[i].REG_Halti3 == 1 ? chipMinorFeatures5_HALTI3 : 0) |
				(gChipInfo[i].REG_TessellationShaders == 1 ? chipMinorFeatures5_TESSELATION_SHADERS : 0) |
				(gChipInfo[i].REG_OnePass2DFilter == 1 ? chipMinorFeatures5_2D_ONE_PASS_FILTER_TAP : 0) |
				(gChipInfo[i].REG_MultiSrcV2StrQuad == 1 ? chipMinorFeatures5_MULTI_SRC_V2_STR_QUAD : 0) |
				(gChipInfo[i].REG_SeperateSRCAndDstCache == 1 ? chipMinorFeatures5_SEPARATE_SRC_DST : 0) |
				(gChipInfo[i].REG_Halti4 == 1 ? chipMinorFeatures5_HALTI4 : 0) |
				(gChipInfo[i].REG_RAWriteDepth == 1 ? chipMinorFeatures5_RA_WRITE_DEPTH : 0) |
				(gChipInfo[i].REG_AndroidOnly == 1 ? chipMinorFeatures5_ANDROID_ONLY : 0) |
				(gChipInfo[i].REG_HasChipProductReg == 1 ? chipMinorFeatures5_HAS_PRODUCTID : 0) |
				(gChipInfo[i].REG_TXSupportDEC == 1 ? chipMinorFeatures5_TX_SUPPORT_DEC : 0) |
				(gChipInfo[i].REG_S8MSAACompression == 1 ? chipMinorFeatures5_S8_MSAA_COMPRESSION : 0) |
				(gChipInfo[i].REG_BugFixesIn544 == 1 ? chipMinorFeatures5_PE_DITHER_FIX2 : 0) |
				(gChipInfo[i].REG_L2CacheRemove == 1 ? chipMinorFeatures5_L2_CACHE_REMOVE : 0) |
				(gChipInfo[i].REG_FEAllowRndVtxCnt == 1 ? chipMinorFeatures5_FE_ALLOW_RND_VTX_CNT : 0) |
				(gChipInfo[i].REG_CubeMapFL28 == 1 ? chipMinorFeatures5_CUBE_MAP_FL28 : 0) |
				(gChipInfo[i].REG_TX6bitFrac == 1 ? chipMinorFeatures5_TX_6BIT_FRAC : 0) |
				(gChipInfo[i].REG_FEAllowStallPrefetchEng == 1 ? chipMinorFeatures5_FE_ALLOW_STALL_PREFETCH_ENG : 0) |
				(gChipInfo[i].REG_ThirdPartyCompression == 1 ? chipMinorFeatures5_THIRD_PARTY_COMPRESSION : 0) |
				(gChipInfo[i].REG_RSS8 == 1 ? chipMinorFeatures5_RS_DEPTHSTENCIL_NATIVE_SUPPORT : 0) |
				(gChipInfo[i].REG_MSAACoherencyCheck == 1 ? chipMinorFeatures5_V2_MSAA_COMP_FIX : 0) |
				(gChipInfo[i].REG_Halti5 == 1 ? chipMinorFeatures5_HALTI5 : 0) |
				(gChipInfo[i].REG_Evis == 1 ? chipMinorFeatures5_EVIS : 0) |
				(gChipInfo[i].REG_BltEngine == 1 ? chipMinorFeatures5_BLT_ENGINE : 0);

			uint32_t features6 = (gChipInfo[i].REG_BugFixes23 == 1 ? chipMinorFeatures6_BUG_FIXES_23 : 0) |
				(gChipInfo[i].REG_BugFixes24 == 1 ? chipMinorFeatures6_BUG_FIXES_24 : 0) |
				(gChipInfo[i].REG_DEC == 1 ? chipMinorFeatures6_DEC : 0) |
				(gChipInfo[i].REG_VSTileNV12 == 1 ? chipMinorFeatures6_VS_TILE_NV12 : 0) |
				(gChipInfo[i].REG_VSTileNV12_10BIT == 1 ? chipMinorFeatures6_VS_TILE_NV12_10BIT : 0) |
				//(gChipInfo[i].REG_DisableVIP == 1 ?  : 0) |
				(gChipInfo[i].RenderTarget8 == 1 ? chipMinorFeatures6_RENDER_TARGET_8 : 0) |
				(gChipInfo[i].TxLodFlowCorrection == 1 ? chipMinorFeatures6_TEX_LOD_FLOW_CORR : 0) |
				(gChipInfo[i].FaceLod == 1 ? chipMinorFeatures6_FACE_LOD : 0) |
				(gChipInfo[i].MultiCoreSemaphoreStallV2 == 1 ? chipMinorFeatures6_MULTI_CORE_SEMAPHORE_STALL_V2 : 0) |
				(gChipInfo[i].VMSAA == 1 ? chipMinorFeatures6_VMSAA : 0) |
				(gChipInfo[i].ChipEnableLink == 1 ? chipMinorFeatures6_CHIP_ENABLE_LINK : 0) |
				(gChipInfo[i].MULTI_SRC_BLT_1_5_ENHANCEMENT == 1 ? chipMinorFeatures6_MULTI_SRC_BLT_1_5_ENHANCEMENT : 0) |
				(gChipInfo[i].MULTI_SRC_BLT_BILINEAR_FILTER == 1 ? chipMinorFeatures6_MULTI_SRC_BLT_BILINEAR_FILTER : 0) |
				(gChipInfo[i].RA_HZEZ_CLOCK_CONTROL == 1 ? chipMinorFeatures6_RA_HZEZ_CLOCK_CONTROL : 0) |
				(gChipInfo[i].CACHE128B256BPERLINE == 1 ? chipMinorFeatures6_CACHE128B256BPERLINE : 0) |
				(gChipInfo[i].V4Compression == 1 ? chipMinorFeatures6_V4_COMPRESSION : 0) |
				//(gChipInfo[i].DE2D_MAJOR_SUPER_TILE == 1 ?  : 0) | //?
				(gChipInfo[i].PE2D_MAJOR_SUPER_TILE == 1 ? chipMinorFeatures6_PE2D_MAJOR_SUPER_TILE : 0) |
				(gChipInfo[i].PE_32BPC_COLORMASK_FIX == 1 ? chipMinorFeatures6_PE_32BPC_COLORMASK_FIX : 0) |
				(gChipInfo[i].ALPHA_BLENDING_OPT == 1 ? chipMinorFeatures6_ALPHA_BLENDING_OPT : 0) |
				(gChipInfo[i].NEW_GPIPE == 1 ? chipMinorFeatures6_NEW_GPIPE : 0) |
				(gChipInfo[i].PIPELINE_32_ATTRIBUTES == 1 ? chipMinorFeatures6_PIPELINE_32_ATTRIBUTES : 0) |
				(gChipInfo[i].MSAA_SHADING == 1 ? chipMinorFeatures6_MSAA_SHADING : 0) |
				(gChipInfo[i].NO_ANISTRO_FILTER == 1 ? chipMinorFeatures6_NO_ANISTRO_FILTER : 0) |
				(gChipInfo[i].NO_ASTC == 1 ? chipMinorFeatures6_NO_ASTC : 0) |
				(gChipInfo[i].NO_DXT == 1 ? chipMinorFeatures6_NO_DXT : 0) |
				(gChipInfo[i].HWTFB == 1 ? chipMinorFeatures6_HWTFB : 0) |
				(gChipInfo[i].RA_DEPTH_WRITE_MSAA1X_FIX == 1 ? chipMinorFeatures6_RA_DEPTH_WRITE_MSAA1X_FIX : 0) |
				(gChipInfo[i].EZHZ_CLOCKGATE_FIX == 1 ? chipMinorFeatures6_EZHZ_CLOCKGATE_FIX : 0) |
				(gChipInfo[i].SH_SNAP2PAGE_FIX == 1 ? chipMinorFeatures6_SH_SNAP2PAGE_FIX : 0) |
				(gChipInfo[i].SH_HALFDEPENDENCY_FIX == 1 ? chipMinorFeatures6_SH_HALFDEPENDENCY_FIX : 0) |
				(gChipInfo[i].USC_MCFILL_FIX == 1 ? chipMinorFeatures6_USC_MCFILL_FIX : 0) |
				(gChipInfo[i].TPG_TCPERF_FIX == 1 ? chipMinorFeatures6_TPG_TCPERF_FIX : 0);

			uint32_t features7 = (gChipInfo[i].USC_MDFIFO_OVERFLOW_FIX == 1 ? chipMinorFeatures7_USC_MDFIFO_OVERFLOW_FIX : 0) |
				(gChipInfo[i].SH_TEXLD_BARRIER_IN_CS_FIX == 1 ? chipMinorFeatures7_SH_TEXLD_BARRIER_IN_CS_FIX : 0) |
				(gChipInfo[i].RS_NEW_BASEADDR == 1 ? chipMinorFeatures7_RS_NEW_BASEADDR : 0) |
				(gChipInfo[i].PE_8bpp_DUALPIPE_FIX == 1 ? chipMinorFeatures7_PE_8BPP_DUALPIPE_FIX : 0) |
				(gChipInfo[i].SH_ADVANCED_INSTR == 1 ? chipMinorFeatures7_SH_ADVANCED_INSTR : 0) |
				(gChipInfo[i].SH_FLAT_INTERPOLATION_DUAL16_FIX == 1 ? chipMinorFeatures7_SH_FLAT_INTERPOLATION_DUAL16_FIX : 0) |
				(gChipInfo[i].USC_CONTINUOUS_FLUS_FIX == 1 ? chipMinorFeatures7_USC_CONTINUOUS_FLUS_FIX : 0) |
				(gChipInfo[i].SH_SUPPORT_V4 == 1 ? chipMinorFeatures7_SH_SUPPORT_V4 : 0) |
				(gChipInfo[i].SH_SUPPORT_ALPHA_KILL == 1 ? chipMinorFeatures7_SH_SUPPORT_ALPHA_KILL : 0) |
				(gChipInfo[i].PE_NO_ALPHA_TEST == 1 ? chipMinorFeatures7_PE_NO_ALPHA_TEST : 0) |
				(gChipInfo[i].TX_LOD_NEAREST_SELECT == 1 ? chipMinorFeatures7_TX_LOD_NEAREST_SELECT : 0) |
				(gChipInfo[i].SH_FIX_LDEXP == 1 ? chipMinorFeatures7_SH_FIX_LDEXP : 0) |
				(gChipInfo[i].SUPPORT_MOVAI == 1 ? chipMinorFeatures7_SUPPORT_MOVAI : 0) |
				(gChipInfo[i].SH_SNAP2PAGE_MAXPAGES_FIX == 1 ? chipMinorFeatures7_SH_SNAP2PAGE_MAXPAGES_FIX : 0) |
				(gChipInfo[i].PE_RGBA16I_FIX == 1 ? chipMinorFeatures7_PE_RGBA16I_FIX : 0) |
				(gChipInfo[i].BLT_8bpp_256TILE_FC_FIX == 1 ? chipMinorFeatures7_BLT_8bpp_256TILE_FC_FIX : 0) |
				(gChipInfo[i].PE_64bit_FENCE_FIX == 1 ? chipMinorFeatures7_PE_64BIT_FENCE_FIX : 0) |
				(gChipInfo[i].USC_FULL_CACHE_FIX == 1 ? chipMinorFeatures7_USC_FULL_CACHE_FIX : 0) |
				(gChipInfo[i].TX_YUV_ASSEMBLER_10BIT == 1 ? chipMinorFeatures7_TX_YUV_ASSEMBLER_10BIT : 0) |
				(gChipInfo[i].FE_32bit_INDEX_FIX == 1 ? chipMinorFeatures7_FE_32BIT_INDEX_FIX : 0) |
				(gChipInfo[i].BLT_64bpp_MASKED_CLEAR_FIX == 1 ? chipMinorFeatures7_BLT_64BPP_MASKED_CLEAR_FIX : 0) |
				(gChipInfo[i].SECURITY == 1 ? chipMinorFeatures7_BIT_SECURITY : 0) |
				(gChipInfo[i].ROBUSTNESS == 1 ? chipMinorFeatures7_BIT_ROBUSTNESS : 0) |
				(gChipInfo[i].USC_ATOMIC_FIX == 1 ? chipMinorFeatures7_USC_ATOMIC_FIX : 0) |
				(gChipInfo[i].SH_PSO_MSAA1x_FIX == 1 ? chipMinorFeatures7_SH_PSO_MSAA1x_FIX : 0) |
				(gChipInfo[i].USC_VX_PERF_FIX == 1 ? chipMinorFeatures7_BIT_USC_VX_PERF_FIX : 0) |
				(gChipInfo[i].EVIS_NO_ABSDIFF == 1 ? chipMinorFeatures7_EVIS_NO_ABSDIFF : 0) |
				(gChipInfo[i].EVIS_NO_BITREPLACE == 1 ? chipMinorFeatures7_EVIS_NO_BITREPLACE : 0) |
				(gChipInfo[i].EVIS_NO_BOXFILTER == 1 ? chipMinorFeatures7_EVIS_NO_BOXFILTER : 0) |
				(gChipInfo[i].EVIS_NO_CORDIAC == 1 ? chipMinorFeatures7_EVIS_NO_CORDIAC : 0) |
				(gChipInfo[i].EVIS_NO_DP32 == 1 ? chipMinorFeatures7_EVIS_NO_DP32 : 0) |
				(gChipInfo[i].EVIS_NO_FILTER == 1 ? chipMinorFeatures7_EVIS_NO_FILTER : 0);

			uint32_t features8 =
				(gChipInfo[i].EVIS_NO_IADD == 1 ? chipMinorFeatures8_EVIS_NO_IADD : 0) |
				(gChipInfo[i].EVIS_NO_SELECTADD == 1 ? chipMinorFeatures8_EVIS_NO_SELECTADD : 0) |
				(gChipInfo[i].EVIS_LERP_7OUTPUT == 1 ? chipMinorFeatures8_EVIS_LERP_7OUTPUT : 0) |
				(gChipInfo[i].EVIS_ACCSQ_8OUTPUT == 1 ? chipMinorFeatures8_EVIS_ACCSQ_8OUTPUT : 0) |
				(gChipInfo[i].USC_GOS_ADDR_FIX == 1 ? chipMinorFeatures8_USC_GOS_ADDR_FIX : 0) |
				(gChipInfo[i].TX_8bit_UVFrac == 1 ? chipMinorFeatures8_TX_8BIT_UVFRAC : 0) |
				(gChipInfo[i].TX_DESC_CACHE_CLOCKGATE_FIX == 1 ? chipMinorFeatures8_TX_DESC_CACHE_CLOCKGATE_FIX : 0) |
				(gChipInfo[i].RSBLT_MSAA_DECOMPRESSION == 1 ? chipMinorFeatures8_RSBLT_MSAA_DECOMPRESSION : 0) |
				(gChipInfo[i].TX_INTEGER_COORDINATE == 1 ? chipMinorFeatures8_TX_INTEGER_COORDINATE : 0) |
				(gChipInfo[i].DRAWID == 1 ? chipMinorFeatures8_DRAWID : 0) |
				(gChipInfo[i].PSIO_SAMPLEMASK_IN_R0ZW_FIX == 1 ? chipMinorFeatures8_PSIO_SAMPLEMASK_IN_R0ZW_FIX : 0) |
				(gChipInfo[i].TX_INTEGER_COORDINATE_V2 == 1 ? chipMinorFeatures8_TX_INTEGER_COORDINATE_V2 : 0) |
				(gChipInfo[i].MULTI_CORE_BLOCK_SET_CONFIG == 1 ? chipMinorFeatures8_MULTI_CORE_BLOCK_SET_CONFIG : 0) |
				(gChipInfo[i].VG_RESOLVE_ENGINE == 1 ? chipMinorFeatures8_VG_RESOLVE_ENGINE : 0) |
				(gChipInfo[i].VG_PE_COLOR_KEY == 1 ? chipMinorFeatures8_VG_PE_COLOR_KEY : 0) |
				(gChipInfo[i].VG_IM_INDEX_FORMAT == 1 ? chipMinorFeatures8_VG_IM_INDEX_FORMAT : 0) |
				(gChipInfo[i].SNAPPAGE_CMD == 1 ? chipMinorFeatures8_SNAPPAGE_CMD : 0) |
				(gChipInfo[i].SH_NO_INDEX_CONST_ON_A0 == 1 ? chipMinorFeatures8_SH_NO_INDEX_CONST_ON_A0 : 0) |
				(gChipInfo[i].SH_NO_ONECONST_LIMIT == 1 ? chipMinorFeatures8_SH_NO_ONECONST_LIMIT : 0) |
				(gChipInfo[i].SH_IMG_LDST_ON_TEMP == 1 ? chipMinorFeatures8_SH_IMG_LDST_ON_TEMP : 0) |
				(gChipInfo[i].COMPUTE_ONLY == 1 ? chipMinorFeatures8_COMPUTE_ONLY : 0) |
				(gChipInfo[i].SH_IMG_LDST_CLAMP == 1 ? chipMinorFeatures8_SH_IMG_LDST_CLAMP : 0) |
				(gChipInfo[i].SH_ICACHE_ALLOC_COUNT_FIX == 1 ? chipMinorFeatures8_SH_ICACHE_ALLOC_COUNT_FIX : 0) |
				(gChipInfo[i].SH_ICACHE_PREFETCH == 1 ? chipMinorFeatures8_SH_ICACHE_PREFETCH : 0) |
				(gChipInfo[i].PE2D_SEPARATE_CACHE == 1 ? chipMinorFeatures8_PE2D_SEPARATE_CACHE : 0) |
				(gChipInfo[i].VG_AYUV_INPUT_OUTPUT == 1 ? chipMinorFeatures8_VG_AYUV_INPUT_OUTPUT : 0) |
				(gChipInfo[i].VG_DOUBLE_IMAGE == 1 ? chipMinorFeatures8_VG_DOUBLE_IMAGE : 0) |
				(gChipInfo[i].VG_RECTANGLE_STRIPE_MODE == 1 ? chipMinorFeatures8_VG_RECTANGLE_STRIPE_MODE : 0) |
				(gChipInfo[i].VG_MMU == 1 ? chipMinorFeatures8_VG_MMU : 0) |
				(gChipInfo[i].VG_IM_FILTER == 1 ? chipMinorFeatures8_VG_IM_FILTER : 0) |
				(gChipInfo[i].VG_IM_YUV_PACKET == 1 ? chipMinorFeatures8_VG_IM_YUV_PACKET : 0) |
				(gChipInfo[i].VG_IM_YUV_PLANAR == 1 ? chipMinorFeatures8_VG_IM_YUV_PLANAR : 0);

			uint32_t features9 =
				(gChipInfo[i].VG_PE_YUV_PACKET == 1 ? chipMinorFeatures9_VG_PE_YUV_PACKET : 0) |
				(gChipInfo[i].VG_COLOR_PRECISION_8_BIT == 1 ? chipMinorFeatures9_VG_COLOR_PRECISION_8_BIT : 0) |
				(gChipInfo[i].PE_MSAA_OQ_FIX == 1 ? chipMinorFeatures9_PE_MSAA_OQ_FIX : 0) |
				(gChipInfo[i].PSIO_MSAA_CL_FIX == 1 ? chipMinorFeatures9_PSIO_MSAA_CL_FIX : 0) |
				(gChipInfo[i].USC_DEFER_FILL_FIX == 1 ? chipMinorFeatures9_USC_DEFER_FILL_FIX : 0) |
				(gChipInfo[i].SH_CLOCK_GATE_FIX == 1 ? chipMinorFeatures9_SH_CLOCK_GATE_FIX : 0) |
				(gChipInfo[i].FE_NEED_DUMMYDRAW == 1 ? chipMinorFeatures9_FE_NEED_DUMMYDRAW : 0) |
				(gChipInfo[i].PE2D_LINEAR_YUV420_OUTPUT == 1 ? chipMinorFeatures9_PE2D_LINEAR_YUV420_OUTPUT : 0) |
				(gChipInfo[i].PE2D_LINEAR_YUV420_10BIT == 1 ? chipMinorFeatures9_PE2D_LINEAR_YUV420_10BIT : 0) |
				(gChipInfo[i].MULTI_CLUSTER == 1 ? chipMinorFeatures9_MULTI_CLUSTER : 0) |
				(gChipInfo[i].VG_TS_CULLING == 1 ? chipMinorFeatures9_VG_TS_CULLING : 0) |
				(gChipInfo[i].VG_FP25 == 1 ? chipMinorFeatures9_VG_FP25 : 0) |
				(gChipInfo[i].SH_MULTI_WG_PACK == 1 ? chipMinorFeatures9_SH_MULTI_WG_PACK : 0) |
				(gChipInfo[i].SH_DUAL16_SAMPLEMASK_ZW == 1 ? chipMinorFeatures9_SH_DUAL16_SAMPLEMASK_ZW : 0) |
				(gChipInfo[i].TPG_TRIVIAL_MODE_FIX == 1 ? chipMinorFeatures9_TPG_TRIVIAL_MODE_FIX : 0) |
				(gChipInfo[i].TX_ASTC_MULTISLICE_FIX == 1 ? chipMinorFeatures9_TX_ASTC_MULTISLICE_FIX : 0) |
				(gChipInfo[i].FE_ROBUST_FIX == 1 ? chipMinorFeatures9_FE_ROBUST_FIX : 0) |
				(gChipInfo[i].SH_GPIPE_ACCESS_FULLTEMPS == 1 ? chipMinorFeatures9_SH_GPIPE_ACCESS_FULLTEMPS : 0) |
				(gChipInfo[i].PSIO_INTERLOCK == 1 ? chipMinorFeatures9_PSIO_INTERLOCK : 0) |
				(gChipInfo[i].PA_WIDELINE_FIX == 1 ? chipMinorFeatures9_PA_WIDELINE_FIX : 0) |
				(gChipInfo[i].WIDELINE_HELPER_FIX == 1 ? chipMinorFeatures9_WIDELINE_HELPER_FIX : 0) |
				(gChipInfo[i].G2D_3rd_PARTY_COMPRESSION_1_1 == 1 ? chipMinorFeatures9_G2D_3RD_PARTY_COMPRESSION_1_1 : 0) |
				(gChipInfo[i].TX_FLUSH_L1CACHE == 1 ? chipMinorFeatures9_TX_FLUSH_L1CACHE : 0) |
				(gChipInfo[i].PE_DITHER_FIX2 == 1 ? chipMinorFeatures9_PE_DITHER_FIX2 : 0) |
				(gChipInfo[i].G2D_DEC400 == 1 ? chipMinorFeatures9_G2D_DEC400 : 0) |
				(gChipInfo[i].SH_TEXLD_U_FIX == 1 ? chipMinorFeatures9_SH_TEXLD_U_FIX : 0) |
				(gChipInfo[i].MC_FCCACHE_BYTEMASK == 1 ? chipMinorFeatures9_MC_FCCACHE_BYTEMASK : 0) |
				(gChipInfo[i].SH_MULTI_WG_PACK_FIX == 1 ? chipMinorFeatures9_SH_MULTI_WG_PACK_FIX : 0) |
				(gChipInfo[i].DC_OVERLAY_SCALING == 1 ? chipMinorFeatures9_DC_OVERLAY_SCALING : 0) |
				(gChipInfo[i].DC_SOURCE_ROTATION == 1 ? chipMinorFeatures9_DC_SOURCE_ROTATION : 0) |
				(gChipInfo[i].DC_TILED == 1 ? chipMinorFeatures9_DC_TILED : 0) |
				(gChipInfo[i].DC_YUV_L1 == 1 ? chipMinorFeatures9_DC_YUV_L1 : 0);

			uint32_t features10 =
				(gChipInfo[i].DC_D30_OUTPUT == 1 ? chipMinorFeatures10_DC_D30_OUTPUT : 0) |
				(gChipInfo[i].DC_MMU == 1 ? chipMinorFeatures10_DC_MMU : 0) |
				(gChipInfo[i].DC_COMPRESSION == 1 ? chipMinorFeatures10_DC_COMPRESSION : 0) |
				(gChipInfo[i].DC_QOS == 1 ? chipMinorFeatures10_DC_QOS : 0) |
				(gChipInfo[i].PE_ADVANCE_BLEND_PART0 == 1 ? chipMinorFeatures10_PE_ADVANCE_BLEND_PART0 : 0) |
				(gChipInfo[i].FE_PATCHLIST_FETCH_FIX == 1 ? chipMinorFeatures10_FE_PATCHLIST_FETCH_FIX : 0) |
				(gChipInfo[i].RA_CG_FIX == 1 ? chipMinorFeatures10_RA_CG_FIX : 0) |
				(gChipInfo[i].EVIS_VX2 == 1 ? chipMinorFeatures10_EVIS_VX2 : 0) |
				(gChipInfo[i].NN_FLOAT32_IO == 1 ? chipMinorFeatures10_NN_FLOAT : 0) |
				(gChipInfo[i].DEC400 == 1 ? chipMinorFeatures10_DEC400 : 0) |
				(gChipInfo[i].LS_SUPPORT_PERCOMP_DEPENDENCY == 1 ? chipMinorFeatures10_LS_SUPPORT_PERCOMP_DEPENDENCY : 0) |
				(gChipInfo[i].TP_ENGINE == 1 ? chipMinorFeatures10_TP_ENGINE : 0) |
				(gChipInfo[i].MULTI_CORE_BLOCK_SET_CONFIG2 == 1 ? chipMinorFeatures10_MULTI_CORE_BLOCK_SET_CONFIG2 : 0) |
				(gChipInfo[i].PE_VMSAA_COVERAGE_CACHE_FIX == 1 ? chipMinorFeatures10_PE_VMSAA_COVERAGE_CACHE_FIX : 0) |
				(gChipInfo[i].SECURITY_AHB == 1 ? chipMinorFeatures10_SECURITY_AHB : 0) |
				(gChipInfo[i].MULTICORE_SEMAPHORESTALL_V3 == 1 ? chipMinorFeatures10_MULTICORE_SEMAPHORESTALL_V3 : 0) |
				(gChipInfo[i].SMALLBATCH == 1 ? chipMinorFeatures10_SMALLBATCH : 0) |
				(gChipInfo[i].SH_CMPLX == 1 ? chipMinorFeatures10_SH_CMPLX : 0) |
				(gChipInfo[i].SH_IDIV0_SWZL_EHS == 1 ? chipMinorFeatures10_SH_IDIV0_SWZL_EHS : 0) |
				(gChipInfo[i].TX_LERP_LESS_BIT == 1 ? chipMinorFeatures10_TX_LERP_LESS_BIT : 0) |
				(gChipInfo[i].SH_GM_ENDIAN == 1 ? chipMinorFeatures10_SH_GM_ENDIAN : 0) |
				(gChipInfo[i].SH_GM_USC_UNALLOC == 1 ? chipMinorFeatures10_SH_GM_USC_UNALLOC : 0) |
				(gChipInfo[i].SH_END_OF_BB == 1 ? chipMinorFeatures10_SH_END_OF_BB : 0) |
				(gChipInfo[i].VIP_V7 == 1 ? chipMinorFeatures10_VIP_V7 : 0) |
				(gChipInfo[i].TX_BORDER_CLAMP_FIX == 1 ? chipMinorFeatures10_TX_BORDER_CLAMP_FIX : 0) |
				(gChipInfo[i].SH_IMG_LD_LASTPIXEL_FIX == 1 ? chipMinorFeatures10_SH_IMG_LD_LASTPIXEL_FIX : 0) |
				(gChipInfo[i].ASYNC_BLT == 1 ? chipMinorFeatures10_ASYNC_BLT : 0) |
				(gChipInfo[i].ASYNC_FE_FENCE_FIX == 1 ? chipMinorFeatures10_ASYNC_FE_FENCE_FIX : 0) |
				(gChipInfo[i].PSCS_THROTTLE == 1 ? chipMinorFeatures10_PSCS_THROTTLE : 0) |
				(gChipInfo[i].SEPARATE_LS == 1 ? chipMinorFeatures10_SEPARATE_LS : 0) |
				(gChipInfo[i].MCFE == 1 ? chipMinorFeatures10_MCFE : 0) |
				(gChipInfo[i].WIDELINE_TRIANGLE_EMU == 1 ? chipMinorFeatures10_WIDELINE_TRIANGLE_EMU : 0);

			uint32_t features11 =
				(gChipInfo[i].VG_RESOLUTION_8K == 1 ? chipMinorFeatures11_VG_RESOLUTION_8K : 0) |
				(gChipInfo[i].FENCE_32BIT == 1 ? chipMinorFeatures11_FENCE_32BIT : 0) |
				(gChipInfo[i].FENCE_64BIT == 1 ? chipMinorFeatures11_FENCE_64BIT : 0) |
				(gChipInfo[i].NN_INTERLEVE8 == 1 ? chipMinorFeatures11_NN_INTERLEVE8 : 0) |
				(gChipInfo[i].PE_DEPTH_ONLY_OQFIX == 1 ? chipMinorFeatures11_PE_DEPTH_ONLY_OQFIX : 0) |
				(gChipInfo[i].TP_REORDER == 1 ? chipMinorFeatures11_TP_REORDER : 0);
#if defined EXTENDED_FEATURE_LIST
				(gChipInfo[i].TX_SEAMLESS_CUBE == 1 ? chipMinorFeatures11_TX_SEAMLESS_CUBE : 0) |
				(gChipInfo[i].TX_SNORM_SUPPORT == 1 ? chipMinorFeatures11_TX_SNORM_SUPPORT : 0) |
				(gChipInfo[i].SH_SCATTER_GATHER == 1 ? chipMinorFeatures11_SH_SCATTER_GATHER : 0) |
				(gChipInfo[i].HWMANAGED_LS == 1 ? chipMinorFeatures11_HWMANAGED_LS : 0) |
				(gChipInfo[i].SH_IMAGE_ENABLE_FIX == 1 ? chipMinorFeatures11_SH_IMAGE_ENABLE_FIX : 0) |
				(gChipInfo[i].MSAA_FRAGMENT_OPERATION == 1 ? chipMinorFeatures11_MSAA_FRAGMENT_OPERATION : 0) |
				(gChipInfo[i].PE_TILE_CACHE_FLUSH_FIX == 1 ? chipMinorFeatures11_PE_TILE_CACHE_FLUSH_FIX : 0) |
				(gChipInfo[i].BLT_YUV_OUTPUT == 1 ? chipMinorFeatures11_BLT_YUV_OUTPUT : 0) |
				(gChipInfo[i].SH_IO_CG_FIX == 1 ? chipMinorFeatures11_SH_IO_CG_FIX : 0) |
				(gChipInfo[i].SH_ROBUSTNESS_FIX == 1 ? chipMinorFeatures11_SH_ROBUSTNESS_FIX : 0) |
				(gChipInfo[i].USC_ATOMIC_FIX2 == 1 ? chipMinorFeatures11_USC_ATOMIC_FIX2 : 0) |
				(gChipInfo[i].PE_A8B8G8R8 == 1 ? chipMinorFeatures11_PE_A8B8G8R8 : 0) |
				(gChipInfo[i].MULTIVIEW_RENDER == 1 ? chipMinorFeatures11_MULTIVIEW_RENDER : 0) |
				(gChipInfo[i].FE_DRAW_DIRECT == 1 ? chipMinorFeatures11_FE_DRAW_DIRECT : 0) |
				(gChipInfo[i].TX_VKBORDER_MODE == 1 ? chipMinorFeatures11_TX_VKBORDER_MODE : 0) |
				(gChipInfo[i].TX_UNNORMALIZED_COORD == 1 ? chipMinorFeatures11_TX_UNNORMALIZED_COORD : 0) |
				(gChipInfo[i].PA_LINECLIP_FIX == 1 ? chipMinorFeatures11_PA_LINECLIP_FIX : 0) |
				(gChipInfo[i].TX_8bit_UVFrac_ROUNDING_FIX == 1 ? chipMinorFeatures11_TX_8bit_UVFrac_ROUNDING_FIX : 0) |
				(gChipInfo[i].MP_ARCH == 1 ? chipMinorFeatures11_MP_ARCH : 0) |
				(gChipInfo[i].TX_NO_FIXED_FILTER == 1 ? chipMinorFeatures11_TX_NO_FIXED_FILTER : 0) |
				(gChipInfo[i].SHARE_Z == 1 ? chipMinorFeatures11_SHARE_Z : 0) |
				(gChipInfo[i].DE_2D_FAST_CLEAR == 1 ? chipMinorFeatures11_DE_2D_FAST_CLEAR : 0) |
				(gChipInfo[i].DE_TILESTATUS_ROTATION_FIX == 1 ? chipMinorFeatures11_DE_TILESTATUS_ROTATION_FIX : 0) |
				(gChipInfo[i].TX_CLEAR_PENDING_FIX == 1 ? chipMinorFeatures11_TX_CLEAR_PENDING_FIX : 0) |
				(gChipInfo[i].HI1_L2_CACHE == 1 ? chipMinorFeatures11_HI1_L2_CACHE : 0) |
				(gChipInfo[i].USC_EVICT_CTRL_FIFO_FLOP_RESET_FIX == 1 ? chipMinorFeatures11_USC_EVICT_CTRL_FIFO_FLOP_RESET_FIX : 0) |
#endif
				(gChipInfo[i].TP_REORDER == 1 ? chipMinorFeatures11_TP_REORDER : 0);

#if defined EXTENDED_FEATURE_LIST
			uint32_t features12 =
				(gChipInfo[i].FORMAT_10BIT_CROSS_4K == 1 ? chipMinorFeatures12_FORMAT_10BIT_CROSS_4K : 0) |
				(gChipInfo[i].FORMAT_P010LSB_I010 == 1 ? chipMinorFeatures12_FORMAT_P010LSB_I010 : 0) |
				(gChipInfo[i].ENDIAN_CONTROL == 1 ? chipMinorFeatures12_ENDIAN_CONTROL : 0) |
				(gChipInfo[i].G2D_RGB_PLANAR == 1 ? chipMinorFeatures12_G2D_RGB_PLANAR : 0) |
				(gChipInfo[i].G2D_RGB_PLANAR_SOURCE == 1 ? chipMinorFeatures12_G2D_RGB_PLANAR_SOURCE : 0) |
				(gChipInfo[i].G2D_DEC400EX == 1 ? chipMinorFeatures12_G2D_DEC400EX : 0) |
				(gChipInfo[i].G2D_FC_IN_DEC400EX == 1 ? chipMinorFeatures12_G2D_FC_IN_DEC400EX : 0) |
				(gChipInfo[i].G2D_NO_YUV420_SOURCE == 1 ? chipMinorFeatures12_G2D_NO_YUV420_SOURCE : 0) |
				(gChipInfo[i].G2D_YUV420_101010 == 1 ? chipMinorFeatures12_G2D_YUV420_101010 : 0) |
				(gChipInfo[i].G2D_MultiSrcBlt_Pipe == 1 ? chipMinorFeatures12_G2D_MultiSrcBlt_Pipe : 0) |
				(gChipInfo[i].G2D_Stretch_MultiSrc_Pipe == 1 ? chipMinorFeatures12_G2D_Stretch_MultiSrc_Pipe : 0) |
				(gChipInfo[i].G2D_Normalization == 1 ? chipMinorFeatures12_G2D_Normalization : 0) |
				(gChipInfo[i].G2D_Normalization_Quantization == 1 ? chipMinorFeatures12_G2D_Normalization_Quantization : 0) |
				(gChipInfo[i].G2D_FRAME_DONE_INTR == 1 ? chipMinorFeatures12_G2D_FRAME_DONE_INTR : 0) |
				(gChipInfo[i].G2D_MASK_AND_COLORKEY == 1 ? chipMinorFeatures12_G2D_MASK_AND_COLORKEY : 0) |
				(gChipInfo[i].AXIFE == 1 ? chipMinorFeatures12_AXIFE : 0) |
				(gChipInfo[i].SH_VX2_FLOATING_MAD_FIX == 1 ? chipMinorFeatures12_SH_VX2_FLOATING_MAD_FIX : 0) |
				(gChipInfo[i].TS_FC_VULKAN_SUPPORT == 1 ? chipMinorFeatures12_TS_FC_VULKAN_SUPPORT : 0) |
				(gChipInfo[i].MSAA_FLOAT_64BIT == 1 ? chipMinorFeatures12_MSAA_FLOAT_64BIT : 0) |
				(gChipInfo[i].INDIRECT_COMPUTE_ZERODIM_FIX == 1 ? chipMinorFeatures12_INDIRECT_COMPUTE_ZERODIM_FIX : 0) |
				(gChipInfo[i].Q_CHANNEL_SUPPORT == 1 ? chipMinorFeatures12_Q_CHANNEL_SUPPORT : 0) |
				(gChipInfo[i].MMU_PAGE_DESCRIPTOR == 1 ? chipMinorFeatures12_MMU_PAGE_DESCRIPTOR : 0) |
				(gChipInfo[i].YUV_LINEAR_TO_TILE_ROTATE == 1 ? chipMinorFeatures12_YUV_LINEAR_TO_TILE_ROTATE : 0) |
				(gChipInfo[i].VEC2_IMULIMAD32_SUPPORT == 1 ? chipMinorFeatures12_VEC2_IMULIMAD32_SUPPORT : 0) |
				(gChipInfo[i].VEC4_IMULIMAD32_SUPPORT == 1 ? chipMinorFeatures12_VEC4_IMULIMAD32_SUPPORT : 0) |
				(gChipInfo[i].VEC2_IDIVIMOD16_SUPPORT == 1 ? chipMinorFeatures12_VEC2_IDIVIMOD16_SUPPORT : 0) |
				(gChipInfo[i].DST_TEX_I2F_F2I_INST_DEPRECATE == 1 ? chipMinorFeatures12_DST_TEX_I2F_F2I_INST_DEPRECATE : 0) |
				(gChipInfo[i].ALU_FP16_INSTRUCTIONS == 1 ? chipMinorFeatures12_ALU_FP16_INSTRUCTIONS : 0) |
				(gChipInfo[i].DUAL16_14BIT_PC_SUPPORT == 1 ? chipMinorFeatures12_DUAL16_14BIT_PC_SUPPORT : 0) |
				(gChipInfo[i].LDST_CONV_4ROUNDING_MODES == 1 ? chipMinorFeatures12_LDST_CONV_4ROUNDING_MODES : 0) |
				(gChipInfo[i].FULL_PACK_MODE_SUPPORT == 1 ? chipMinorFeatures12_FULL_PACK_MODE_SUPPORT : 0) |
				(gChipInfo[i].DEPTH_FLOAT32_SUPPORT == 1 ? chipMinorFeatures12_DEPTH_FLOAT32_SUPPORT : 0);

			uint32_t features13 =
				(gChipInfo[i].GPU_INSPECTOR_COUNTERS == 1 ? chipMinorFeatures13_GPU_INSPECTOR_COUNTERS : 0) |
				(gChipInfo[i].FP32_TO_FP16_CONV_FIX == 1 ? chipMinorFeatures13_FP32_TO_FP16_CONV_FIX : 0) |
				(gChipInfo[i].IMGLD_COMP_COUNT_FIX == 1 ? chipMinorFeatures13_IMGLD_COMP_COUNT_FIX : 0) |
				(gChipInfo[i].IMGLD_WIDTH_LT16_FIX == 1 ? chipMinorFeatures13_IMGLD_WIDTH_LT16_FIX : 0) |
				(gChipInfo[i].TX_FILTER_ROUND_FIX == 1 ? chipMinorFeatures13_TX_FILTER_ROUND_FIX : 0);
#endif


			linux_devices[count].features = feature;
			linux_devices[count].minor_features0 = features0;
			linux_devices[count].minor_features1 = features1;
			linux_devices[count].minor_features2 = features2;
			linux_devices[count].minor_features3 = features3;
			linux_devices[count].minor_features4 = features4;
			linux_devices[count].minor_features5 = features5;
			linux_devices[count].minor_features6 = features6;
			linux_devices[count].minor_features7 = features7;
			linux_devices[count].minor_features8 = features8;
			linux_devices[count].minor_features9 = features9;
			linux_devices[count].minor_features10 = features10;
			linux_devices[count].minor_features11 = features11;
#if defined EXTENDED_FEATURE_LIST
			linux_devices[count].minor_features12 = features12;
			linux_devices[count].minor_features13 = features13;
#endif
			count++;
		}
	}

	// get device entries from ST's gc_features_database.h 6.4.7 (2021-07-01)
	n = sizeof(gChipInfo_STM32_4_6_7) / sizeof(gChipInfo_STM32_4_6_7[0]);
	for (int i = 0; i < n; i++) {
		bool found = false;

		for (int j = 0; j < device_count; ++j)
		{

			if ((devices[j].chipID == gChipInfo_STM32_4_6_7[i].chipID)
				&& (devices[j].chipVersion == gChipInfo_STM32_4_6_7[i].chipVersion)
				&& (devices[j].productID == gChipInfo_STM32_4_6_7[i].productID)
				&& (devices[j].ecoID == gChipInfo_STM32_4_6_7[i].ecoID)
				&& (devices[j].customerID == gChipInfo_STM32_4_6_7[i].customerID)
				)
			{
				found = true;
			}
		}

		if (found == false) {
			devices[count].chipID = gChipInfo_STM32_4_6_7[i].chipID;
			devices[count].chipVersion = gChipInfo_STM32_4_6_7[i].chipVersion;
			devices[count].productID = gChipInfo_STM32_4_6_7[i].productID;
			devices[count].ecoID = gChipInfo_STM32_4_6_7[i].ecoID;
			devices[count].customerID = gChipInfo_STM32_4_6_7[i].customerID;

			linux_devices[count].model = gChipInfo_STM32_4_6_7[i].chipID;
			linux_devices[count].revision = gChipInfo_STM32_4_6_7[i].chipVersion;
			linux_devices[count].product_id = gChipInfo_STM32_4_6_7[i].productID;
			linux_devices[count].customer_id = gChipInfo_STM32_4_6_7[i].customerID;
			linux_devices[count].eco_id = gChipInfo_STM32_4_6_7[i].ecoID;

			linux_devices[count].stream_count = gChipInfo_STM32_4_6_7[i].Streams;
			linux_devices[count].register_max = gChipInfo_STM32_4_6_7[i].TempRegisters;
			linux_devices[count].thread_count = gChipInfo_STM32_4_6_7[i].ThreadCount;
			linux_devices[count].shader_core_count = gChipInfo_STM32_4_6_7[i].NumShaderCores;
			linux_devices[count].vertex_cache_size = gChipInfo_STM32_4_6_7[i].VertexCacheSize;
			linux_devices[count].vertex_output_buffer_size = gChipInfo_STM32_4_6_7[i].VertexOutputBufferSize;
			linux_devices[count].pixel_pipes = gChipInfo_STM32_4_6_7[i].NumPixelPipes;
			linux_devices[count].instruction_count = gChipInfo_STM32_4_6_7[i].InstructionCount;
			linux_devices[count].num_constants = gChipInfo_STM32_4_6_7[i].NumberOfConstants;
			linux_devices[count].buffer_size = gChipInfo_STM32_4_6_7[i].BufferSize;
			linux_devices[count].varyings_count = gChipInfo_STM32_4_6_7[i].VaryingCount;

			uint32_t feature = (gChipInfo_STM32_4_6_7[i].REG_FastClear == 1 ? chipFeatures_FAST_CLEAR : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_SpecialAntiAliasing == 1 ? chipFeatures_SPECIAL_ANTI_ALIASING : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_Pipe3D == 1 ? chipFeatures_PIPE_3D : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_DXTTextureCompression == 1 ? chipFeatures_DXT_TEXTURE_COMPRESSION : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_DebugMode == 1 ? chipFeatures_DEBUG_MODE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_ZCompression == 1 ? chipFeatures_Z_COMPRESSION : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_YUV420Filter == 1 ? chipFeatures_YUV420_SCALER : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_MSAA == 1 ? chipFeatures_MSAA : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_DC == 1 ? chipFeatures_DC : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_Pipe2D == 1 ? chipFeatures_PIPE_2D : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_ETC1TextureCompression == 1 ? chipFeatures_ETC1_TEXTURE_COMPRESSION : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_FastScaler == 1 ? chipFeatures_FAST_SCALER : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_HighDynamicRange == 1 ? chipFeatures_HIGH_DYNAMIC_RANGE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_YUV420Tiler == 1 ? chipFeatures_YUV420_TILER : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_ModuleCG == 1 ? chipFeatures_MODULE_CG : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_MinArea == 1 ? chipFeatures_MIN_AREA : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_NoEZ == 1 ? chipFeatures_NO_EARLY_Z : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_No422Texture == 1 ? chipFeatures_NO_422_TEXTURE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BufferInterleaving == 1 ? chipFeatures_BUFFER_INTERLEAVING : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_ByteWrite2D == 1 ? chipFeatures_BYTE_WRITE_2D : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_NoScaler == 1 ? chipFeatures_NO_SCALER : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_YUY2Averaging == 1 ? chipFeatures_YUY2_AVERAGING : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_HalfPECache == 1 ? chipFeatures_HALF_PE_CACHE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_HalfTXCache == 1 ? chipFeatures_HALF_TX_CACHE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_YUY2RenderTarget == 1 ? chipFeatures_YUY2_RENDER_TARGET : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_Mem32BitSupport == 1 ? chipFeatures_MEM32 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_PipeVG == 1 ? chipFeatures_PIPE_VG : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_VGTS == 1 ? chipFeatures_VGTS : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_FE20 == 1 ? chipFeatures_FE20 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_ByteWrite3D == 1 ? chipFeatures_BYTE_WRITE_3D : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_RsYuvTarget == 1 ? chipFeatures_RS_YUV_TARGET : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_FE20BitIndex == 1 ? chipFeatures_32_BIT_INDICES : 0);

			uint32_t features0 =
				(gChipInfo_STM32_4_6_7[i].REG_FlipY == 1 ? chipMinorFeatures0_FLIP_Y : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_DualReturnBus == 1 ? chipMinorFeatures0_DUAL_RETURN_BUS : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_EndiannessConfig == 1 ? chipMinorFeatures0_ENDIANNESS_CONFIG : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_Texture8K == 1 ? chipMinorFeatures0_TEXTURE_8K : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_CorrectTextureConverter == 1 ? chipMinorFeatures0_CORRECT_TEXTURE_CONVERTER : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_SpecialMsaaLod == 1 ? chipMinorFeatures0_SPECIAL_MSAA_LOD : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_FastClearFlush == 1 ? chipMinorFeatures0_FAST_CLEAR_FLUSH : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_2DPE20 == 1 ? chipMinorFeatures0_2DPE20 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_CorrectAutoDisable == 1 ? chipMinorFeatures0_CORRECT_AUTO_DISABLE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_Render8K == 1 ? chipMinorFeatures0_RENDERTARGET_8K : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_TileStatus2Bits == 1 ? chipMinorFeatures0_2BITPERTILE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_SeparateTileStatusWhenInterleaved == 1 ? chipMinorFeatures0_SEPARATE_TILE_STATUS_WHEN_INTERLEAVED : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_SuperTiled32x32 == 1 ? chipMinorFeatures0_SUPER_TILED : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_VG20 == 1 ? chipMinorFeatures0_VG_20 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_TSExtendedCommands == 1 ? chipMinorFeatures0_TS_EXTENDED_COMMANDS : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_CompressionFifoFixed == 1 ? chipMinorFeatures0_COMPRESSION_FIFO_FIXED : 0) |
#ifdef ESTIMATED_VALUE
				(gChipInfo_STM32_4_6_7[i].REG_ExtraShaderInstructions0 == 1 ? chipMinorFeatures0_HAS_SIGN_FLOOR_CEIL : 0) | //?
				(gChipInfo_STM32_4_6_7[i].REG_ExtraShaderInstructions1 == 1 ? chipMinorFeatures0_HAS_SQRT_TRIG : 0) | //?
				(gChipInfo_STM32_4_6_7[i].REG_DefaultReg0 == 1 ? chipMinorFeatures0_MORE_MINOR_FEATURES : 0) | //?
#endif
				(gChipInfo_STM32_4_6_7[i].REG_VGFilter == 1 ? chipMinorFeatures0_VG_FILTER : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_VG21 == 1 ? chipMinorFeatures0_VG_21 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_ShaderGetsW == 1 ? chipMinorFeatures0_SHADER_HAS_W : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_MC20 == 1 ? chipMinorFeatures0_MC20 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_ShaderMSAASideband == 1 ? chipMinorFeatures0_MSAA_SIDEBAND : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes0 == 1 ? chipMinorFeatures0_BUG_FIXES0 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_VAA == 1 ? chipMinorFeatures0_VAA : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BypassInMSAA == 1 ? chipMinorFeatures0_BYPASS_IN_MSAA : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_HierarchicalZ == 1 ? chipMinorFeatures0_HZ : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_NewTexture == 1 ? chipMinorFeatures0_NEW_TEXTURE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_A8TargetSupport == 1 ? chipMinorFeatures0_2D_A8_TARGET : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_CorrectStencil == 1 ? chipMinorFeatures0_CORRECT_STENCIL : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_EnhanceVR == 1 ? chipMinorFeatures0_ENHANCE_VR : 0);

			uint32_t features1 =
				(gChipInfo_STM32_4_6_7[i].REG_RSUVSwizzle == 1 ? chipMinorFeatures1_RSUV_SWIZZLE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_V2Compression == 1 ? chipMinorFeatures1_V2_COMPRESSION : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_VGDoubleBuffer == 1 ? chipMinorFeatures1_VG_DOUBLE_BUFFER : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes1 == 1 ? chipMinorFeatures1_EXTRA_EVENT_STATES : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes2 == 1 ? chipMinorFeatures1_NO_STRIPING_NEEDED : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_TextureStride == 1 ? chipMinorFeatures1_TEXTURE_STRIDE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes3 == 1 ? chipMinorFeatures1_BUG_FIXES3 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_CorrectAutoDisable1 == 1 ? chipMinorFeatures1_AUTO_DISABLE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_AutoRestartTS == 1 ? chipMinorFeatures1_AUTO_RESTART_TS : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes4 == 1 ? chipMinorFeatures1_DISABLE_PE_GATING : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_L2Windowing == 1 ? chipMinorFeatures1_L2_WINDOWING : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_HalfFloatPipe == 1 ? chipMinorFeatures1_HALF_FLOAT : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_PixelDither == 1 ? chipMinorFeatures1_PIXEL_DITHER : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_TwoStencilReference == 1 ? chipMinorFeatures1_TWO_STENCIL_REFERENCE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_ExtendedPixelFormat == 1 ? chipMinorFeatures1_EXTENDED_PIXEL_FORMAT : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_CorrectMinMaxDepth == 1 ? chipMinorFeatures1_CORRECT_MIN_MAX_DEPTH : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_DitherAndFilterPlusAlpha2D == 1 ? chipMinorFeatures1_2D_DITHER : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes5 == 1 ? chipMinorFeatures1_BUG_FIXES5 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_New2D == 1 ? chipMinorFeatures1_NEW_2D : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_NewFloatingPointArithmetic == 1 ? chipMinorFeatures1_NEW_FP : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_TextureHorizontalAlignmentSelect == 1 ? chipMinorFeatures1_TEXTURE_HALIGN : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_NonPowerOfTwo == 1 ? chipMinorFeatures1_NON_POWER_OF_TWO : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_LinearTextureSupport == 1 ? chipMinorFeatures1_LINEAR_TEXTURE_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_Halti0 == 1 ? chipMinorFeatures1_HALTI0 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_CorrectOverflowVG == 1 ? chipMinorFeatures1_CORRECT_OVERFLOW_VG : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_NegativeLogFix == 1 ? chipMinorFeatures1_NEGATIVE_LOG_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_ResolveOffset == 1 ? chipMinorFeatures1_RESOLVE_OFFSET : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_OkToGateAxiClock == 1 ? chipMinorFeatures1_OK_TO_GATE_AXI_CLOCK : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_MMU == 1 ? chipMinorFeatures1_MMU_VERSION : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_WideLine == 1 ? chipMinorFeatures1_WIDE_LINE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes6 == 1 ? chipMinorFeatures1_BUG_FIXES6 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_FcFlushStall == 1 ? chipMinorFeatures1_FC_FLUSH_STALL : 0);

			uint32_t features2 = (gChipInfo_STM32_4_6_7[i].REG_LineLoop == 1 ? chipMinorFeatures2_LINE_LOOP : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_LogicOp == 1 ? chipMinorFeatures2_LOGIC_OP : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_SeamlessCubeMap == 1 ? chipMinorFeatures2_SEAMLESS_CUBE_MAP : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_SuperTiledTexture == 1 ? chipMinorFeatures2_SUPERTILED_TEXTURE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_LinearPE == 1 ? chipMinorFeatures2_LINEAR_PE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_RectPrimitive == 1 ? chipMinorFeatures2_RECT_PRIMITIVE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_Composition == 1 ? chipMinorFeatures2_COMPOSITION : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_CorrectAutoDisableCountWidth == 1 ? chipMinorFeatures2_CORRECT_AUTO_DISABLE_COUNT : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_PESwizzle == 1 ? chipMinorFeatures2_PE_SWIZZLE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_EndEvent == 1 ? chipMinorFeatures2_END_EVENT : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_S1S8 == 1 ? chipMinorFeatures2_S1S8 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_Halti1 == 1 ? chipMinorFeatures2_HALTI1 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_RGB888 == 1 ? chipMinorFeatures2_RGB888 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_TX_YUVAssembler == 1 ? chipMinorFeatures2_TX__YUV_ASSEMBLER : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_DynamicFrequencyScaling == 1 ? chipMinorFeatures2_DYNAMIC_FREQUENCY_SCALING : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_TXFilter == 1 ? chipMinorFeatures2_TX_FILTER : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_FullDirectFB == 1 ? chipMinorFeatures2_FULL_DIRECTFB : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_OnePass2DFilter == 1 ? chipMinorFeatures2_2D_TILING : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_ThreadWalkerInPS == 1 ? chipMinorFeatures2_THREAD_WALKER_IN_PS : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_TileFiller == 1 ? chipMinorFeatures2_TILE_FILLER : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_YUVStandard == 1 ? chipMinorFeatures2_YUV_STANDARD : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_MultiSourceBlt == 1 ? chipMinorFeatures2_2D_MULTI_SOURCE_BLIT : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_YUVConversion == 1 ? chipMinorFeatures2_YUV_CONVERSION : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_FlushFixed2D == 1 ? chipMinorFeatures2_FLUSH_FIXED_2D : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_Interleaver == 1 ? chipMinorFeatures2_INTERLEAVER : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_MixedStreams == 1 ? chipMinorFeatures2_MIXED_STREAMS : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_L2CacheFor2D420 == 1 ? chipMinorFeatures2_2D_420_L2CACHE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes7 == 1 ? chipMinorFeatures2_BUG_FIXES7 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_NoIndexPattern == 1 ? chipMinorFeatures2_2D_NO_INDEX8_BRUSH : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_TextureTileStatus == 1 ? chipMinorFeatures2_TEXTURE_TILED_READ : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_DecompressZ16 == 1 ? chipMinorFeatures2_DECOMPRESS_Z16 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes8 == 1 ? chipMinorFeatures2_BUG_FIXES8 : 0);

			uint32_t features3 = (gChipInfo_STM32_4_6_7[i].REG_DERotationStallFix == 1 ? chipMinorFeatures3_ROTATION_STALL_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_OclOnly == 1 ? chipMinorFeatures3_OCL_ONLY : 0) |
#ifdef ESTIMATED_VALUE
				(gChipInfo_STM32_4_6_7[i].REG_NewFeatures0 == 1 ? chipMinorFeatures3_2D_MULTI_SOURCE_BLT_EX : 0) |
#endif
				(gChipInfo_STM32_4_6_7[i].REG_InstructionCache == 1 ? chipMinorFeatures3_INSTRUCTION_CACHE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_GeometryShader == 1 ? chipMinorFeatures3_GEOMETRY_SHADER : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_TexCompressionSupertiled == 1 ? chipMinorFeatures3_TEX_COMPRESSION_SUPERTILED : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_Generics == 1 ? chipMinorFeatures3_GENERICS : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes9 == 1 ? chipMinorFeatures3_BUG_FIXES9 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_FastMSAA == 1 ? chipMinorFeatures3_FAST_MSAA : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_WClip == 1 ? chipMinorFeatures3_WCLIP : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes10 == 1 ? chipMinorFeatures3_BUG_FIXES10 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_UnifiedSamplers == 1 ? chipMinorFeatures3_UNIFIED_SAMPLERS : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes11 == 1 ? chipMinorFeatures3_BUG_FIXES11 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_PerformanceCounters == 1 ? chipMinorFeatures3_PERFORMANCE_COUNTERS : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_ExtraShaderInstructions2 == 1 ? chipMinorFeatures3_HAS_FAST_TRANSCENDENTALS : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes12 == 1 ? chipMinorFeatures3_BUG_FIXES12 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes13 == 1 ? chipMinorFeatures3_BUG_FIXES13 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_DEEnhancements1 == 1 ? chipMinorFeatures3_DE_ENHANCEMENTS1 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_ACE == 1 ? chipMinorFeatures3_ACE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_TXEnhancements1 == 1 ? chipMinorFeatures3_TX_ENHANCEMENTS1 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_SHEnhancements1 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS1 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_SHEnhancements2 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS2 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_PEEnhancements1 == 1 ? chipMinorFeatures3_PE_ENHANCEMENTS1 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_DEEnhancements2 == 1 ? chipMinorFeatures3_2D_FC_SOURCE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes14 == 1 ? chipMinorFeatures3_BUG_FIXES_14 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_PowerOptimizations0 == 1 ? chipMinorFeatures3_POWER_OPTIMIZATIONS_0 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_NewHZ == 1 ? chipMinorFeatures3_NEW_HZ : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes15 == 1 ? chipMinorFeatures3_PE_DITHER_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_DEEnhancements3 == 1 ? chipMinorFeatures3_DE_ENHANCEMENTS3 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_SHEnhancements3 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS3 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_SHEnhancements4 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS4 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_TXEnhancements2 == 1 ? chipMinorFeatures3_TX_ENHANCEMENTS2 : 0);

			uint32_t features4 = (gChipInfo_STM32_4_6_7[i].REG_FEEnhancements1 == 1 ? chipMinorFeatures4_FE_ENHANCEMENTS1 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_PEEnhancements2 == 1 ? chipMinorFeatures4_PE_ENHANCEMENTS2 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_PAEnhancements1 == 1 ? chipMinorFeatures4_FRUSTUM_CLIP_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_DENoGamma == 1 ? chipMinorFeatures4_DE_NO_GAMMA : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_PAEnhancements2 == 1 ? chipMinorFeatures4_PA_ENHANCEMENTS_2 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_DEEnhancements4 == 1 ? chipMinorFeatures4_2D_GAMMA : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_PEEnhancements3 == 1 ? chipMinorFeatures4_SINGLE_BUFFER : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_HIEnhancements1 == 1 ? chipMinorFeatures4_HI_ENHANCEMENTS_1 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_TXEnhancements3 == 1 ? chipMinorFeatures4_TX_ENHANCEMENTS_3 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_SHEnhancements5 == 1 ? chipMinorFeatures4_SH_ENHANCEMENTS_5 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_FEEnhancements2 == 1 ? chipMinorFeatures4_FE_ENHANCEMENTS_2 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes16 == 1 ? chipMinorFeatures4_TX_LERP_PRECISION_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_DEEnhancements5 == 1 ? chipMinorFeatures4_2D_COLOR_SPACE_CONVERSION : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_TXEnhancements4 == 1 ? chipMinorFeatures4_TEXTURE_ASTC : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_PEEnhancements4 == 1 ? chipMinorFeatures4_PE_ENHANCEMENTS_4 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_MCEnhancements1 == 1 ? chipMinorFeatures4_MC_ENHANCEMENTS_1 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_Halti2 == 1 ? chipMinorFeatures4_HALTI2 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_DEMirrorRotate == 1 ? chipMinorFeatures4_2D_MIRROR_EXTENSION : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_SmallMSAA == 1 ? chipMinorFeatures4_SMALL_MSAA : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes17 == 1 ? chipMinorFeatures4_BUG_FIXES_17 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_Rasterizer2 == 1 ? chipMinorFeatures4_NEW_RA : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_DualPipeOPF == 1 ? chipMinorFeatures4_2D_OPF_YUV_OUTPUT : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_MultiSrcV2 == 1 ? chipMinorFeatures4_2D_MULTI_SOURCE_BLT_EX2 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_CSCV2 == 1 ? chipMinorFeatures4_NO_USER_CSC : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_PAEnhancements3 == 1 ? chipMinorFeatures4_ZFIXES : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes18 == 1 ? chipMinorFeatures4_BUG_FIXES18 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_Compression2D == 1 ? chipMinorFeatures4_2D_COMPRESSION : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_Probe == 1 ? chipMinorFeatures4_PROBE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_MediumPrecision == 1 ? chipMinorFeatures4_MEDIUM_PRECISION : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_DESupertile == 1 ? chipMinorFeatures4_2D_SUPER_TILE_VERSION : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes19 == 1 ? chipMinorFeatures4_BUG_FIXES19 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_SHEnhancements6 == 1 ? chipMinorFeatures4_SH_ENHANCEMENTS6 : 0);

			uint32_t features5 = (gChipInfo_STM32_4_6_7[i].REG_SHEnhancements7 == 1 ? chipMinorFeatures5_SH_ENHANCEMENTS7 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes20 == 1 ? chipMinorFeatures5_BUG_FIXES20 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_DEAddress40 == 1 ? chipMinorFeatures5_DE_ADDRESS_40 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_MiniMMUFix == 1 ? chipMinorFeatures5_MINI_MMU_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_EEZ == 1 ? chipMinorFeatures5_EEZ : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes21 == 1 ? chipMinorFeatures5_BUG_FIXES21 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_ExtraVgCaps == 1 ? chipMinorFeatures5_EXTRA_VG_CAPS : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_MultiSrcV15 == 1 ? chipMinorFeatures5_MULTI_SRC_V15 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes22 == 1 ? chipMinorFeatures5_BUG_FIXES22 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_Halti3 == 1 ? chipMinorFeatures5_HALTI3 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_TessellationShaders == 1 ? chipMinorFeatures5_TESSELATION_SHADERS : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_OnePass2DFilter == 1 ? chipMinorFeatures5_2D_ONE_PASS_FILTER_TAP : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_MultiSrcV2StrQuad == 1 ? chipMinorFeatures5_MULTI_SRC_V2_STR_QUAD : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_SeperateSRCAndDstCache == 1 ? chipMinorFeatures5_SEPARATE_SRC_DST : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_Halti4 == 1 ? chipMinorFeatures5_HALTI4 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_RAWriteDepth == 1 ? chipMinorFeatures5_RA_WRITE_DEPTH : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_AndroidOnly == 1 ? chipMinorFeatures5_ANDROID_ONLY : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_HasChipProductReg == 1 ? chipMinorFeatures5_HAS_PRODUCTID : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_TXSupportDEC == 1 ? chipMinorFeatures5_TX_SUPPORT_DEC : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_S8MSAACompression == 1 ? chipMinorFeatures5_S8_MSAA_COMPRESSION : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixesIn544 == 1 ? chipMinorFeatures5_PE_DITHER_FIX2 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_L2CacheRemove == 1 ? chipMinorFeatures5_L2_CACHE_REMOVE : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_FEAllowRndVtxCnt == 1 ? chipMinorFeatures5_FE_ALLOW_RND_VTX_CNT : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_CubeMapFL28 == 1 ? chipMinorFeatures5_CUBE_MAP_FL28 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_TX6bitFrac == 1 ? chipMinorFeatures5_TX_6BIT_FRAC : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_FEAllowStallPrefetchEng == 1 ? chipMinorFeatures5_FE_ALLOW_STALL_PREFETCH_ENG : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_ThirdPartyCompression == 1 ? chipMinorFeatures5_THIRD_PARTY_COMPRESSION : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_RSS8 == 1 ? chipMinorFeatures5_RS_DEPTHSTENCIL_NATIVE_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_MSAACoherencyCheck == 1 ? chipMinorFeatures5_V2_MSAA_COMP_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_Halti5 == 1 ? chipMinorFeatures5_HALTI5 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_Evis == 1 ? chipMinorFeatures5_EVIS : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BltEngine == 1 ? chipMinorFeatures5_BLT_ENGINE : 0);

			uint32_t features6 = (gChipInfo_STM32_4_6_7[i].REG_BugFixes23 == 1 ? chipMinorFeatures6_BUG_FIXES_23 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_BugFixes24 == 1 ? chipMinorFeatures6_BUG_FIXES_24 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_DEC == 1 ? chipMinorFeatures6_DEC : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_VSTileNV12 == 1 ? chipMinorFeatures6_VS_TILE_NV12 : 0) |
				(gChipInfo_STM32_4_6_7[i].REG_VSTileNV12_10BIT == 1 ? chipMinorFeatures6_VS_TILE_NV12_10BIT : 0) |
				//(gChipInfo_STM32_4_6_7[i].REG_DisableVIP == 1 ?  : 0) |
				(gChipInfo_STM32_4_6_7[i].RenderTarget8 == 1 ? chipMinorFeatures6_RENDER_TARGET_8 : 0) |
				(gChipInfo_STM32_4_6_7[i].TxLodFlowCorrection == 1 ? chipMinorFeatures6_TEX_LOD_FLOW_CORR : 0) |
				(gChipInfo_STM32_4_6_7[i].FaceLod == 1 ? chipMinorFeatures6_FACE_LOD : 0) |
				(gChipInfo_STM32_4_6_7[i].MultiCoreSemaphoreStallV2 == 1 ? chipMinorFeatures6_MULTI_CORE_SEMAPHORE_STALL_V2 : 0) |
				(gChipInfo_STM32_4_6_7[i].VMSAA == 1 ? chipMinorFeatures6_VMSAA : 0) |
				(gChipInfo_STM32_4_6_7[i].ChipEnableLink == 1 ? chipMinorFeatures6_CHIP_ENABLE_LINK : 0) |
				(gChipInfo_STM32_4_6_7[i].MULTI_SRC_BLT_1_5_ENHANCEMENT == 1 ? chipMinorFeatures6_MULTI_SRC_BLT_1_5_ENHANCEMENT : 0) |
				(gChipInfo_STM32_4_6_7[i].MULTI_SRC_BLT_BILINEAR_FILTER == 1 ? chipMinorFeatures6_MULTI_SRC_BLT_BILINEAR_FILTER : 0) |
				(gChipInfo_STM32_4_6_7[i].RA_HZEZ_CLOCK_CONTROL == 1 ? chipMinorFeatures6_RA_HZEZ_CLOCK_CONTROL : 0) |
				(gChipInfo_STM32_4_6_7[i].CACHE128B256BPERLINE == 1 ? chipMinorFeatures6_CACHE128B256BPERLINE : 0) |
				(gChipInfo_STM32_4_6_7[i].V4Compression == 1 ? chipMinorFeatures6_V4_COMPRESSION : 0) |
				//(gChipInfo_STM32_4_6_7[i].DE2D_MAJOR_SUPER_TILE == 1 ?  : 0) | //?
				(gChipInfo_STM32_4_6_7[i].PE2D_MAJOR_SUPER_TILE == 1 ? chipMinorFeatures6_PE2D_MAJOR_SUPER_TILE : 0) |
				(gChipInfo_STM32_4_6_7[i].PE_32BPC_COLORMASK_FIX == 1 ? chipMinorFeatures6_PE_32BPC_COLORMASK_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].ALPHA_BLENDING_OPT == 1 ? chipMinorFeatures6_ALPHA_BLENDING_OPT : 0) |
				(gChipInfo_STM32_4_6_7[i].NEW_GPIPE == 1 ? chipMinorFeatures6_NEW_GPIPE : 0) |
				(gChipInfo_STM32_4_6_7[i].PIPELINE_32_ATTRIBUTES == 1 ? chipMinorFeatures6_PIPELINE_32_ATTRIBUTES : 0) |
				(gChipInfo_STM32_4_6_7[i].MSAA_SHADING == 1 ? chipMinorFeatures6_MSAA_SHADING : 0) |
				(gChipInfo_STM32_4_6_7[i].NO_ANISTRO_FILTER == 1 ? chipMinorFeatures6_NO_ANISTRO_FILTER : 0) |
				(gChipInfo_STM32_4_6_7[i].NO_ASTC == 1 ? chipMinorFeatures6_NO_ASTC : 0) |
				(gChipInfo_STM32_4_6_7[i].NO_DXT == 1 ? chipMinorFeatures6_NO_DXT : 0) |
				(gChipInfo_STM32_4_6_7[i].HWTFB == 1 ? chipMinorFeatures6_HWTFB : 0) |
				(gChipInfo_STM32_4_6_7[i].RA_DEPTH_WRITE_MSAA1X_FIX == 1 ? chipMinorFeatures6_RA_DEPTH_WRITE_MSAA1X_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].EZHZ_CLOCKGATE_FIX == 1 ? chipMinorFeatures6_EZHZ_CLOCKGATE_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_SNAP2PAGE_FIX == 1 ? chipMinorFeatures6_SH_SNAP2PAGE_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_HALFDEPENDENCY_FIX == 1 ? chipMinorFeatures6_SH_HALFDEPENDENCY_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].USC_MCFILL_FIX == 1 ? chipMinorFeatures6_USC_MCFILL_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].TPG_TCPERF_FIX == 1 ? chipMinorFeatures6_TPG_TCPERF_FIX : 0);

			uint32_t features7 = (gChipInfo_STM32_4_6_7[i].USC_MDFIFO_OVERFLOW_FIX == 1 ? chipMinorFeatures7_USC_MDFIFO_OVERFLOW_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_TEXLD_BARRIER_IN_CS_FIX == 1 ? chipMinorFeatures7_SH_TEXLD_BARRIER_IN_CS_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].RS_NEW_BASEADDR == 1 ? chipMinorFeatures7_RS_NEW_BASEADDR : 0) |
				(gChipInfo_STM32_4_6_7[i].PE_8bpp_DUALPIPE_FIX == 1 ? chipMinorFeatures7_PE_8BPP_DUALPIPE_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_ADVANCED_INSTR == 1 ? chipMinorFeatures7_SH_ADVANCED_INSTR : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_FLAT_INTERPOLATION_DUAL16_FIX == 1 ? chipMinorFeatures7_SH_FLAT_INTERPOLATION_DUAL16_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].USC_CONTINUOUS_FLUS_FIX == 1 ? chipMinorFeatures7_USC_CONTINUOUS_FLUS_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_SUPPORT_V4 == 1 ? chipMinorFeatures7_SH_SUPPORT_V4 : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_SUPPORT_ALPHA_KILL == 1 ? chipMinorFeatures7_SH_SUPPORT_ALPHA_KILL : 0) |
				(gChipInfo_STM32_4_6_7[i].PE_NO_ALPHA_TEST == 1 ? chipMinorFeatures7_PE_NO_ALPHA_TEST : 0) |
				(gChipInfo_STM32_4_6_7[i].TX_LOD_NEAREST_SELECT == 1 ? chipMinorFeatures7_TX_LOD_NEAREST_SELECT : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_FIX_LDEXP == 1 ? chipMinorFeatures7_SH_FIX_LDEXP : 0) |
				(gChipInfo_STM32_4_6_7[i].SUPPORT_MOVAI == 1 ? chipMinorFeatures7_SUPPORT_MOVAI : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_SNAP2PAGE_MAXPAGES_FIX == 1 ? chipMinorFeatures7_SH_SNAP2PAGE_MAXPAGES_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].PE_RGBA16I_FIX == 1 ? chipMinorFeatures7_PE_RGBA16I_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].BLT_8bpp_256TILE_FC_FIX == 1 ? chipMinorFeatures7_BLT_8bpp_256TILE_FC_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].PE_64bit_FENCE_FIX == 1 ? chipMinorFeatures7_PE_64BIT_FENCE_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].USC_FULL_CACHE_FIX == 1 ? chipMinorFeatures7_USC_FULL_CACHE_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].TX_YUV_ASSEMBLER_10BIT == 1 ? chipMinorFeatures7_TX_YUV_ASSEMBLER_10BIT : 0) |
				(gChipInfo_STM32_4_6_7[i].FE_32bit_INDEX_FIX == 1 ? chipMinorFeatures7_FE_32BIT_INDEX_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].BLT_64bpp_MASKED_CLEAR_FIX == 1 ? chipMinorFeatures7_BLT_64BPP_MASKED_CLEAR_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].SECURITY == 1 ? chipMinorFeatures7_BIT_SECURITY : 0) |
				(gChipInfo_STM32_4_6_7[i].ROBUSTNESS == 1 ? chipMinorFeatures7_BIT_ROBUSTNESS : 0) |
				(gChipInfo_STM32_4_6_7[i].USC_ATOMIC_FIX == 1 ? chipMinorFeatures7_USC_ATOMIC_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_PSO_MSAA1x_FIX == 1 ? chipMinorFeatures7_SH_PSO_MSAA1x_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].USC_VX_PERF_FIX == 1 ? chipMinorFeatures7_BIT_USC_VX_PERF_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].EVIS_NO_ABSDIFF == 1 ? chipMinorFeatures7_EVIS_NO_ABSDIFF : 0) |
				(gChipInfo_STM32_4_6_7[i].EVIS_NO_BITREPLACE == 1 ? chipMinorFeatures7_EVIS_NO_BITREPLACE : 0) |
				(gChipInfo_STM32_4_6_7[i].EVIS_NO_BOXFILTER == 1 ? chipMinorFeatures7_EVIS_NO_BOXFILTER : 0) |
				(gChipInfo_STM32_4_6_7[i].EVIS_NO_CORDIAC == 1 ? chipMinorFeatures7_EVIS_NO_CORDIAC : 0) |
				(gChipInfo_STM32_4_6_7[i].EVIS_NO_DP32 == 1 ? chipMinorFeatures7_EVIS_NO_DP32 : 0) |
				(gChipInfo_STM32_4_6_7[i].EVIS_NO_FILTER == 1 ? chipMinorFeatures7_EVIS_NO_FILTER : 0);

			uint32_t features8 =
				(gChipInfo_STM32_4_6_7[i].EVIS_NO_IADD == 1 ? chipMinorFeatures8_EVIS_NO_IADD : 0) |
				(gChipInfo_STM32_4_6_7[i].EVIS_NO_SELECTADD == 1 ? chipMinorFeatures8_EVIS_NO_SELECTADD : 0) |
				(gChipInfo_STM32_4_6_7[i].EVIS_LERP_7OUTPUT == 1 ? chipMinorFeatures8_EVIS_LERP_7OUTPUT : 0) |
				(gChipInfo_STM32_4_6_7[i].EVIS_ACCSQ_8OUTPUT == 1 ? chipMinorFeatures8_EVIS_ACCSQ_8OUTPUT : 0) |
				(gChipInfo_STM32_4_6_7[i].USC_GOS_ADDR_FIX == 1 ? chipMinorFeatures8_USC_GOS_ADDR_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].TX_8bit_UVFrac == 1 ? chipMinorFeatures8_TX_8BIT_UVFRAC : 0) |
				(gChipInfo_STM32_4_6_7[i].TX_DESC_CACHE_CLOCKGATE_FIX == 1 ? chipMinorFeatures8_TX_DESC_CACHE_CLOCKGATE_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].RSBLT_MSAA_DECOMPRESSION == 1 ? chipMinorFeatures8_RSBLT_MSAA_DECOMPRESSION : 0) |
				(gChipInfo_STM32_4_6_7[i].TX_INTEGER_COORDINATE == 1 ? chipMinorFeatures8_TX_INTEGER_COORDINATE : 0) |
				(gChipInfo_STM32_4_6_7[i].DRAWID == 1 ? chipMinorFeatures8_DRAWID : 0) |
				(gChipInfo_STM32_4_6_7[i].PSIO_SAMPLEMASK_IN_R0ZW_FIX == 1 ? chipMinorFeatures8_PSIO_SAMPLEMASK_IN_R0ZW_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].TX_INTEGER_COORDINATE_V2 == 1 ? chipMinorFeatures8_TX_INTEGER_COORDINATE_V2 : 0) |
				(gChipInfo_STM32_4_6_7[i].MULTI_CORE_BLOCK_SET_CONFIG == 1 ? chipMinorFeatures8_MULTI_CORE_BLOCK_SET_CONFIG : 0) |
				(gChipInfo_STM32_4_6_7[i].VG_RESOLVE_ENGINE == 1 ? chipMinorFeatures8_VG_RESOLVE_ENGINE : 0) |
				(gChipInfo_STM32_4_6_7[i].VG_PE_COLOR_KEY == 1 ? chipMinorFeatures8_VG_PE_COLOR_KEY : 0) |
				(gChipInfo_STM32_4_6_7[i].VG_IM_INDEX_FORMAT == 1 ? chipMinorFeatures8_VG_IM_INDEX_FORMAT : 0) |
				(gChipInfo_STM32_4_6_7[i].SNAPPAGE_CMD == 1 ? chipMinorFeatures8_SNAPPAGE_CMD : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_NO_INDEX_CONST_ON_A0 == 1 ? chipMinorFeatures8_SH_NO_INDEX_CONST_ON_A0 : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_NO_ONECONST_LIMIT == 1 ? chipMinorFeatures8_SH_NO_ONECONST_LIMIT : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_IMG_LDST_ON_TEMP == 1 ? chipMinorFeatures8_SH_IMG_LDST_ON_TEMP : 0) |
				(gChipInfo_STM32_4_6_7[i].COMPUTE_ONLY == 1 ? chipMinorFeatures8_COMPUTE_ONLY : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_IMG_LDST_CLAMP == 1 ? chipMinorFeatures8_SH_IMG_LDST_CLAMP : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_ICACHE_ALLOC_COUNT_FIX == 1 ? chipMinorFeatures8_SH_ICACHE_ALLOC_COUNT_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_ICACHE_PREFETCH == 1 ? chipMinorFeatures8_SH_ICACHE_PREFETCH : 0) |
				(gChipInfo_STM32_4_6_7[i].PE2D_SEPARATE_CACHE == 1 ? chipMinorFeatures8_PE2D_SEPARATE_CACHE : 0) |
				(gChipInfo_STM32_4_6_7[i].VG_AYUV_INPUT_OUTPUT == 1 ? chipMinorFeatures8_VG_AYUV_INPUT_OUTPUT : 0) |
				(gChipInfo_STM32_4_6_7[i].VG_DOUBLE_IMAGE == 1 ? chipMinorFeatures8_VG_DOUBLE_IMAGE : 0) |
				(gChipInfo_STM32_4_6_7[i].VG_RECTANGLE_STRIPE_MODE == 1 ? chipMinorFeatures8_VG_RECTANGLE_STRIPE_MODE : 0) |
				(gChipInfo_STM32_4_6_7[i].VG_MMU == 1 ? chipMinorFeatures8_VG_MMU : 0) |
				(gChipInfo_STM32_4_6_7[i].VG_IM_FILTER == 1 ? chipMinorFeatures8_VG_IM_FILTER : 0) |
				(gChipInfo_STM32_4_6_7[i].VG_IM_YUV_PACKET == 1 ? chipMinorFeatures8_VG_IM_YUV_PACKET : 0) |
				(gChipInfo_STM32_4_6_7[i].VG_IM_YUV_PLANAR == 1 ? chipMinorFeatures8_VG_IM_YUV_PLANAR : 0);

			uint32_t features9 =
				(gChipInfo_STM32_4_6_7[i].VG_PE_YUV_PACKET == 1 ? chipMinorFeatures9_VG_PE_YUV_PACKET : 0) |
				(gChipInfo_STM32_4_6_7[i].VG_COLOR_PRECISION_8_BIT == 1 ? chipMinorFeatures9_VG_COLOR_PRECISION_8_BIT : 0) |
				(gChipInfo_STM32_4_6_7[i].PE_MSAA_OQ_FIX == 1 ? chipMinorFeatures9_PE_MSAA_OQ_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].PSIO_MSAA_CL_FIX == 1 ? chipMinorFeatures9_PSIO_MSAA_CL_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].USC_DEFER_FILL_FIX == 1 ? chipMinorFeatures9_USC_DEFER_FILL_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_CLOCK_GATE_FIX == 1 ? chipMinorFeatures9_SH_CLOCK_GATE_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].FE_NEED_DUMMYDRAW == 1 ? chipMinorFeatures9_FE_NEED_DUMMYDRAW : 0) |
				(gChipInfo_STM32_4_6_7[i].PE2D_LINEAR_YUV420_OUTPUT == 1 ? chipMinorFeatures9_PE2D_LINEAR_YUV420_OUTPUT : 0) |
				(gChipInfo_STM32_4_6_7[i].PE2D_LINEAR_YUV420_10BIT == 1 ? chipMinorFeatures9_PE2D_LINEAR_YUV420_10BIT : 0) |
				(gChipInfo_STM32_4_6_7[i].MULTI_CLUSTER == 1 ? chipMinorFeatures9_MULTI_CLUSTER : 0) |
				(gChipInfo_STM32_4_6_7[i].VG_TS_CULLING == 1 ? chipMinorFeatures9_VG_TS_CULLING : 0) |
				(gChipInfo_STM32_4_6_7[i].VG_FP25 == 1 ? chipMinorFeatures9_VG_FP25 : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_MULTI_WG_PACK == 1 ? chipMinorFeatures9_SH_MULTI_WG_PACK : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_DUAL16_SAMPLEMASK_ZW == 1 ? chipMinorFeatures9_SH_DUAL16_SAMPLEMASK_ZW : 0) |
				(gChipInfo_STM32_4_6_7[i].TPG_TRIVIAL_MODE_FIX == 1 ? chipMinorFeatures9_TPG_TRIVIAL_MODE_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].TX_ASTC_MULTISLICE_FIX == 1 ? chipMinorFeatures9_TX_ASTC_MULTISLICE_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].FE_ROBUST_FIX == 1 ? chipMinorFeatures9_FE_ROBUST_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_GPIPE_ACCESS_FULLTEMPS == 1 ? chipMinorFeatures9_SH_GPIPE_ACCESS_FULLTEMPS : 0) |
				(gChipInfo_STM32_4_6_7[i].PSIO_INTERLOCK == 1 ? chipMinorFeatures9_PSIO_INTERLOCK : 0) |
				(gChipInfo_STM32_4_6_7[i].PA_WIDELINE_FIX == 1 ? chipMinorFeatures9_PA_WIDELINE_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].WIDELINE_HELPER_FIX == 1 ? chipMinorFeatures9_WIDELINE_HELPER_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].G2D_3rd_PARTY_COMPRESSION_1_1 == 1 ? chipMinorFeatures9_G2D_3RD_PARTY_COMPRESSION_1_1 : 0) |
				(gChipInfo_STM32_4_6_7[i].TX_FLUSH_L1CACHE == 1 ? chipMinorFeatures9_TX_FLUSH_L1CACHE : 0) |
				(gChipInfo_STM32_4_6_7[i].PE_DITHER_FIX2 == 1 ? chipMinorFeatures9_PE_DITHER_FIX2 : 0) |
				(gChipInfo_STM32_4_6_7[i].G2D_DEC400 == 1 ? chipMinorFeatures9_G2D_DEC400 : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_TEXLD_U_FIX == 1 ? chipMinorFeatures9_SH_TEXLD_U_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].MC_FCCACHE_BYTEMASK == 1 ? chipMinorFeatures9_MC_FCCACHE_BYTEMASK : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_MULTI_WG_PACK_FIX == 1 ? chipMinorFeatures9_SH_MULTI_WG_PACK_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].DC_OVERLAY_SCALING == 1 ? chipMinorFeatures9_DC_OVERLAY_SCALING : 0) |
				(gChipInfo_STM32_4_6_7[i].DC_SOURCE_ROTATION == 1 ? chipMinorFeatures9_DC_SOURCE_ROTATION : 0) |
				(gChipInfo_STM32_4_6_7[i].DC_TILED == 1 ? chipMinorFeatures9_DC_TILED : 0) |
				(gChipInfo_STM32_4_6_7[i].DC_YUV_L1 == 1 ? chipMinorFeatures9_DC_YUV_L1 : 0);

			uint32_t features10 =
				(gChipInfo_STM32_4_6_7[i].DC_D30_OUTPUT == 1 ? chipMinorFeatures10_DC_D30_OUTPUT : 0) |
				(gChipInfo_STM32_4_6_7[i].DC_MMU == 1 ? chipMinorFeatures10_DC_MMU : 0) |
				(gChipInfo_STM32_4_6_7[i].DC_COMPRESSION == 1 ? chipMinorFeatures10_DC_COMPRESSION : 0) |
				(gChipInfo_STM32_4_6_7[i].DC_QOS == 1 ? chipMinorFeatures10_DC_QOS : 0) |
				(gChipInfo_STM32_4_6_7[i].PE_ADVANCE_BLEND_PART0 == 1 ? chipMinorFeatures10_PE_ADVANCE_BLEND_PART0 : 0) |
				(gChipInfo_STM32_4_6_7[i].FE_PATCHLIST_FETCH_FIX == 1 ? chipMinorFeatures10_FE_PATCHLIST_FETCH_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].RA_CG_FIX == 1 ? chipMinorFeatures10_RA_CG_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].EVIS_VX2 == 1 ? chipMinorFeatures10_EVIS_VX2 : 0) |
				(gChipInfo_STM32_4_6_7[i].NN_FLOAT32_IO == 1 ? chipMinorFeatures10_NN_FLOAT : 0) |
				(gChipInfo_STM32_4_6_7[i].DEC400 == 1 ? chipMinorFeatures10_DEC400 : 0) |
				(gChipInfo_STM32_4_6_7[i].LS_SUPPORT_PERCOMP_DEPENDENCY == 1 ? chipMinorFeatures10_LS_SUPPORT_PERCOMP_DEPENDENCY : 0) |
				(gChipInfo_STM32_4_6_7[i].TP_ENGINE == 1 ? chipMinorFeatures10_TP_ENGINE : 0) |
				(gChipInfo_STM32_4_6_7[i].MULTI_CORE_BLOCK_SET_CONFIG2 == 1 ? chipMinorFeatures10_MULTI_CORE_BLOCK_SET_CONFIG2 : 0) |
				(gChipInfo_STM32_4_6_7[i].PE_VMSAA_COVERAGE_CACHE_FIX == 1 ? chipMinorFeatures10_PE_VMSAA_COVERAGE_CACHE_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].SECURITY_AHB == 1 ? chipMinorFeatures10_SECURITY_AHB : 0) |
				(gChipInfo_STM32_4_6_7[i].MULTICORE_SEMAPHORESTALL_V3 == 1 ? chipMinorFeatures10_MULTICORE_SEMAPHORESTALL_V3 : 0) |
				(gChipInfo_STM32_4_6_7[i].SMALLBATCH == 1 ? chipMinorFeatures10_SMALLBATCH : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_CMPLX == 1 ? chipMinorFeatures10_SH_CMPLX : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_IDIV0_SWZL_EHS == 1 ? chipMinorFeatures10_SH_IDIV0_SWZL_EHS : 0) |
				(gChipInfo_STM32_4_6_7[i].TX_LERP_LESS_BIT == 1 ? chipMinorFeatures10_TX_LERP_LESS_BIT : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_GM_ENDIAN == 1 ? chipMinorFeatures10_SH_GM_ENDIAN : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_GM_USC_UNALLOC == 1 ? chipMinorFeatures10_SH_GM_USC_UNALLOC : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_END_OF_BB == 1 ? chipMinorFeatures10_SH_END_OF_BB : 0) |
				(gChipInfo_STM32_4_6_7[i].VIP_V7 == 1 ? chipMinorFeatures10_VIP_V7 : 0) |
				(gChipInfo_STM32_4_6_7[i].TX_BORDER_CLAMP_FIX == 1 ? chipMinorFeatures10_TX_BORDER_CLAMP_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_IMG_LD_LASTPIXEL_FIX == 1 ? chipMinorFeatures10_SH_IMG_LD_LASTPIXEL_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].ASYNC_BLT == 1 ? chipMinorFeatures10_ASYNC_BLT : 0) |
				(gChipInfo_STM32_4_6_7[i].ASYNC_FE_FENCE_FIX == 1 ? chipMinorFeatures10_ASYNC_FE_FENCE_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].PSCS_THROTTLE == 1 ? chipMinorFeatures10_PSCS_THROTTLE : 0) |
				(gChipInfo_STM32_4_6_7[i].SEPARATE_LS == 1 ? chipMinorFeatures10_SEPARATE_LS : 0) |
				(gChipInfo_STM32_4_6_7[i].MCFE == 1 ? chipMinorFeatures10_MCFE : 0) |
				(gChipInfo_STM32_4_6_7[i].WIDELINE_TRIANGLE_EMU == 1 ? chipMinorFeatures10_WIDELINE_TRIANGLE_EMU : 0);

			uint32_t features11 =
				(gChipInfo_STM32_4_6_7[i].VG_RESOLUTION_8K == 1 ? chipMinorFeatures11_VG_RESOLUTION_8K : 0) |
				(gChipInfo_STM32_4_6_7[i].FENCE_32BIT == 1 ? chipMinorFeatures11_FENCE_32BIT : 0) |
				(gChipInfo_STM32_4_6_7[i].FENCE_64BIT == 1 ? chipMinorFeatures11_FENCE_64BIT : 0) |
				(gChipInfo_STM32_4_6_7[i].NN_INTERLEVE8 == 1 ? chipMinorFeatures11_NN_INTERLEVE8 : 0) |
				(gChipInfo_STM32_4_6_7[i].PE_DEPTH_ONLY_OQFIX == 1 ? chipMinorFeatures11_PE_DEPTH_ONLY_OQFIX : 0) |
				(gChipInfo_STM32_4_6_7[i].TP_REORDER == 1 ? chipMinorFeatures11_TP_REORDER : 0);
#if defined EXTENDED_FEATURE_LIST
			(gChipInfo_STM32_4_6_7[i].TX_SEAMLESS_CUBE == 1 ? chipMinorFeatures11_TX_SEAMLESS_CUBE : 0) |
				(gChipInfo_STM32_4_6_7[i].TX_SNORM_SUPPORT == 1 ? chipMinorFeatures11_TX_SNORM_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_SCATTER_GATHER == 1 ? chipMinorFeatures11_SH_SCATTER_GATHER : 0) |
				(gChipInfo_STM32_4_6_7[i].HWMANAGED_LS == 1 ? chipMinorFeatures11_HWMANAGED_LS : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_IMAGE_ENABLE_FIX == 1 ? chipMinorFeatures11_SH_IMAGE_ENABLE_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].MSAA_FRAGMENT_OPERATION == 1 ? chipMinorFeatures11_MSAA_FRAGMENT_OPERATION : 0) |
				(gChipInfo_STM32_4_6_7[i].PE_TILE_CACHE_FLUSH_FIX == 1 ? chipMinorFeatures11_PE_TILE_CACHE_FLUSH_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].BLT_YUV_OUTPUT == 1 ? chipMinorFeatures11_BLT_YUV_OUTPUT : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_IO_CG_FIX == 1 ? chipMinorFeatures11_SH_IO_CG_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_ROBUSTNESS_FIX == 1 ? chipMinorFeatures11_SH_ROBUSTNESS_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].USC_ATOMIC_FIX2 == 1 ? chipMinorFeatures11_USC_ATOMIC_FIX2 : 0) |
				(gChipInfo_STM32_4_6_7[i].PE_A8B8G8R8 == 1 ? chipMinorFeatures11_PE_A8B8G8R8 : 0) |
				(gChipInfo_STM32_4_6_7[i].MULTIVIEW_RENDER == 1 ? chipMinorFeatures11_MULTIVIEW_RENDER : 0) |
				(gChipInfo_STM32_4_6_7[i].FE_DRAW_DIRECT == 1 ? chipMinorFeatures11_FE_DRAW_DIRECT : 0) |
				(gChipInfo_STM32_4_6_7[i].TX_VKBORDER_MODE == 1 ? chipMinorFeatures11_TX_VKBORDER_MODE : 0) |
				(gChipInfo_STM32_4_6_7[i].TX_UNNORMALIZED_COORD == 1 ? chipMinorFeatures11_TX_UNNORMALIZED_COORD : 0) |
				(gChipInfo_STM32_4_6_7[i].PA_LINECLIP_FIX == 1 ? chipMinorFeatures11_PA_LINECLIP_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].TX_8bit_UVFrac_ROUNDING_FIX == 1 ? chipMinorFeatures11_TX_8bit_UVFrac_ROUNDING_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].MP_ARCH == 1 ? chipMinorFeatures11_MP_ARCH : 0) |
				(gChipInfo_STM32_4_6_7[i].TX_NO_FIXED_FILTER == 1 ? chipMinorFeatures11_TX_NO_FIXED_FILTER : 0) |
				(gChipInfo_STM32_4_6_7[i].SHARE_Z == 1 ? chipMinorFeatures11_SHARE_Z : 0) |
				(gChipInfo_STM32_4_6_7[i].DE_2D_FAST_CLEAR == 1 ? chipMinorFeatures11_DE_2D_FAST_CLEAR : 0) |
				(gChipInfo_STM32_4_6_7[i].DE_TILESTATUS_ROTATION_FIX == 1 ? chipMinorFeatures11_DE_TILESTATUS_ROTATION_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].TX_CLEAR_PENDING_FIX == 1 ? chipMinorFeatures11_TX_CLEAR_PENDING_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].HI1_L2_CACHE == 1 ? chipMinorFeatures11_HI1_L2_CACHE : 0) |
				(gChipInfo_STM32_4_6_7[i].USC_EVICT_CTRL_FIFO_FLOP_RESET_FIX == 1 ? chipMinorFeatures11_USC_EVICT_CTRL_FIFO_FLOP_RESET_FIX : 0) |
#endif
				(gChipInfo_STM32_4_6_7[i].TP_REORDER == 1 ? chipMinorFeatures11_TP_REORDER : 0);

#if defined EXTENDED_FEATURE_LIST
			uint32_t features12 =
				(gChipInfo_STM32_4_6_7[i].FORMAT_10BIT_CROSS_4K == 1 ? chipMinorFeatures12_FORMAT_10BIT_CROSS_4K : 0) |
				(gChipInfo_STM32_4_6_7[i].FORMAT_P010LSB_I010 == 1 ? chipMinorFeatures12_FORMAT_P010LSB_I010 : 0) |
				(gChipInfo_STM32_4_6_7[i].ENDIAN_CONTROL == 1 ? chipMinorFeatures12_ENDIAN_CONTROL : 0) |
				(gChipInfo_STM32_4_6_7[i].G2D_RGB_PLANAR == 1 ? chipMinorFeatures12_G2D_RGB_PLANAR : 0) |
				(gChipInfo_STM32_4_6_7[i].G2D_RGB_PLANAR_SOURCE == 1 ? chipMinorFeatures12_G2D_RGB_PLANAR_SOURCE : 0) |
				(gChipInfo_STM32_4_6_7[i].G2D_DEC400EX == 1 ? chipMinorFeatures12_G2D_DEC400EX : 0) |
				(gChipInfo_STM32_4_6_7[i].G2D_FC_IN_DEC400EX == 1 ? chipMinorFeatures12_G2D_FC_IN_DEC400EX : 0) |
				(gChipInfo_STM32_4_6_7[i].G2D_NO_YUV420_SOURCE == 1 ? chipMinorFeatures12_G2D_NO_YUV420_SOURCE : 0) |
				(gChipInfo_STM32_4_6_7[i].G2D_YUV420_101010 == 1 ? chipMinorFeatures12_G2D_YUV420_101010 : 0) |
				(gChipInfo_STM32_4_6_7[i].G2D_MultiSrcBlt_Pipe == 1 ? chipMinorFeatures12_G2D_MultiSrcBlt_Pipe : 0) |
				(gChipInfo_STM32_4_6_7[i].G2D_Stretch_MultiSrc_Pipe == 1 ? chipMinorFeatures12_G2D_Stretch_MultiSrc_Pipe : 0) |
				(gChipInfo_STM32_4_6_7[i].G2D_Normalization == 1 ? chipMinorFeatures12_G2D_Normalization : 0) |
				(gChipInfo_STM32_4_6_7[i].G2D_Normalization_Quantization == 1 ? chipMinorFeatures12_G2D_Normalization_Quantization : 0) |
				(gChipInfo_STM32_4_6_7[i].G2D_FRAME_DONE_INTR == 1 ? chipMinorFeatures12_G2D_FRAME_DONE_INTR : 0) |
				(gChipInfo_STM32_4_6_7[i].G2D_MASK_AND_COLORKEY == 1 ? chipMinorFeatures12_G2D_MASK_AND_COLORKEY : 0) |
				(gChipInfo_STM32_4_6_7[i].AXIFE == 1 ? chipMinorFeatures12_AXIFE : 0) |
				(gChipInfo_STM32_4_6_7[i].SH_VX2_FLOATING_MAD_FIX == 1 ? chipMinorFeatures12_SH_VX2_FLOATING_MAD_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].TS_FC_VULKAN_SUPPORT == 1 ? chipMinorFeatures12_TS_FC_VULKAN_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_7[i].MSAA_FLOAT_64BIT == 1 ? chipMinorFeatures12_MSAA_FLOAT_64BIT : 0) |
				(gChipInfo_STM32_4_6_7[i].INDIRECT_COMPUTE_ZERODIM_FIX == 1 ? chipMinorFeatures12_INDIRECT_COMPUTE_ZERODIM_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].Q_CHANNEL_SUPPORT == 1 ? chipMinorFeatures12_Q_CHANNEL_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_7[i].MMU_PAGE_DESCRIPTOR == 1 ? chipMinorFeatures12_MMU_PAGE_DESCRIPTOR : 0) |
				(gChipInfo_STM32_4_6_7[i].YUV_LINEAR_TO_TILE_ROTATE == 1 ? chipMinorFeatures12_YUV_LINEAR_TO_TILE_ROTATE : 0) |
				(gChipInfo_STM32_4_6_7[i].VEC2_IMULIMAD32_SUPPORT == 1 ? chipMinorFeatures12_VEC2_IMULIMAD32_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_7[i].VEC4_IMULIMAD32_SUPPORT == 1 ? chipMinorFeatures12_VEC4_IMULIMAD32_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_7[i].VEC2_IDIVIMOD16_SUPPORT == 1 ? chipMinorFeatures12_VEC2_IDIVIMOD16_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_7[i].DST_TEX_I2F_F2I_INST_DEPRECATE == 1 ? chipMinorFeatures12_DST_TEX_I2F_F2I_INST_DEPRECATE : 0) |
				(gChipInfo_STM32_4_6_7[i].ALU_FP16_INSTRUCTIONS == 1 ? chipMinorFeatures12_ALU_FP16_INSTRUCTIONS : 0) |
				(gChipInfo_STM32_4_6_7[i].DUAL16_14BIT_PC_SUPPORT == 1 ? chipMinorFeatures12_DUAL16_14BIT_PC_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_7[i].LDST_CONV_4ROUNDING_MODES == 1 ? chipMinorFeatures12_LDST_CONV_4ROUNDING_MODES : 0) |
				(gChipInfo_STM32_4_6_7[i].FULL_PACK_MODE_SUPPORT == 1 ? chipMinorFeatures12_FULL_PACK_MODE_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_7[i].DEPTH_FLOAT32_SUPPORT == 1 ? chipMinorFeatures12_DEPTH_FLOAT32_SUPPORT : 0);

			uint32_t features13 =
				(gChipInfo_STM32_4_6_7[i].GPU_INSPECTOR_COUNTERS == 1 ? chipMinorFeatures13_GPU_INSPECTOR_COUNTERS : 0) |
				(gChipInfo_STM32_4_6_7[i].FP32_TO_FP16_CONV_FIX == 1 ? chipMinorFeatures13_FP32_TO_FP16_CONV_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].IMGLD_COMP_COUNT_FIX == 1 ? chipMinorFeatures13_IMGLD_COMP_COUNT_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].IMGLD_WIDTH_LT16_FIX == 1 ? chipMinorFeatures13_IMGLD_WIDTH_LT16_FIX : 0) |
				(gChipInfo_STM32_4_6_7[i].TX_FILTER_ROUND_FIX == 1 ? chipMinorFeatures13_TX_FILTER_ROUND_FIX : 0);
#endif


			linux_devices[count].features = feature;
			linux_devices[count].minor_features0 = features0;
			linux_devices[count].minor_features1 = features1;
			linux_devices[count].minor_features2 = features2;
			linux_devices[count].minor_features3 = features3;
			linux_devices[count].minor_features4 = features4;
			linux_devices[count].minor_features5 = features5;
			linux_devices[count].minor_features6 = features6;
			linux_devices[count].minor_features7 = features7;
			linux_devices[count].minor_features8 = features8;
			linux_devices[count].minor_features9 = features9;
			linux_devices[count].minor_features10 = features10;
			linux_devices[count].minor_features11 = features11;
#if defined EXTENDED_FEATURE_LIST
			linux_devices[count].minor_features12 = features12;
			linux_devices[count].minor_features13 = features13;
#endif
			count++;
		}
	}

	// get device entries from STM32's gc_festures_database.h 6.2.4 (2018-12-18)
	// NXP's NN_FLOAT32_IO is reneamed to NN_FLOAT
	n = sizeof(gChipInfo_STM32_6_2_4) / sizeof(gChipInfo_STM32_6_2_4[0]);
	for (int i = 0; i < n; i++) {
		bool found = false;

		for (int j = 0; j < device_count; ++j)
		{

			if ((devices[j].chipID == gChipInfo_STM32_6_2_4[i].chipID)
				&& (devices[j].chipVersion == gChipInfo_STM32_6_2_4[i].chipVersion)
				&& (devices[j].productID == gChipInfo_STM32_6_2_4[i].productID)
				&& (devices[j].ecoID == gChipInfo_STM32_6_2_4[i].ecoID)
				&& (devices[j].customerID == gChipInfo_STM32_6_2_4[i].customerID)
				)
			{
				found = true;
			}
		}

		if (found == false) {
			devices[count].chipID = gChipInfo_STM32_6_2_4[i].chipID;
			devices[count].chipVersion = gChipInfo_STM32_6_2_4[i].chipVersion;
			devices[count].productID = gChipInfo_STM32_6_2_4[i].productID;
			devices[count].ecoID = gChipInfo_STM32_6_2_4[i].ecoID;
			devices[count].customerID = gChipInfo_STM32_6_2_4[i].customerID;

			linux_devices[count].model = gChipInfo_STM32_6_2_4[i].chipID;
			linux_devices[count].revision = gChipInfo_STM32_6_2_4[i].chipVersion;
			linux_devices[count].product_id = gChipInfo_STM32_6_2_4[i].productID;
			linux_devices[count].customer_id = gChipInfo_STM32_6_2_4[i].customerID;
			linux_devices[count].eco_id = gChipInfo_STM32_6_2_4[i].ecoID;

			linux_devices[count].stream_count = gChipInfo_STM32_6_2_4[i].Streams;
			linux_devices[count].register_max = gChipInfo_STM32_6_2_4[i].TempRegisters;
			linux_devices[count].thread_count = gChipInfo_STM32_6_2_4[i].ThreadCount;
			linux_devices[count].shader_core_count = gChipInfo_STM32_6_2_4[i].NumShaderCores;
			linux_devices[count].vertex_cache_size = gChipInfo_STM32_6_2_4[i].VertexCacheSize;
			linux_devices[count].vertex_output_buffer_size = gChipInfo_STM32_6_2_4[i].VertexOutputBufferSize;
			linux_devices[count].pixel_pipes = gChipInfo_STM32_6_2_4[i].NumPixelPipes;
			linux_devices[count].instruction_count = gChipInfo_STM32_6_2_4[i].InstructionCount;
			linux_devices[count].num_constants = gChipInfo_STM32_6_2_4[i].NumberOfConstants;
			linux_devices[count].buffer_size = gChipInfo_STM32_6_2_4[i].BufferSize;
			linux_devices[count].varyings_count = gChipInfo_STM32_6_2_4[i].VaryingCount;

			uint32_t feature = (gChipInfo_STM32_6_2_4[i].REG_FastClear == 1 ? chipFeatures_FAST_CLEAR : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_SpecialAntiAliasing == 1 ? chipFeatures_SPECIAL_ANTI_ALIASING : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_Pipe3D == 1 ? chipFeatures_PIPE_3D : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_DXTTextureCompression == 1 ? chipFeatures_DXT_TEXTURE_COMPRESSION : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_DebugMode == 1 ? chipFeatures_DEBUG_MODE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_ZCompression == 1 ? chipFeatures_Z_COMPRESSION : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_YUV420Filter == 1 ? chipFeatures_YUV420_SCALER : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_MSAA == 1 ? chipFeatures_MSAA : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_DC == 1 ? chipFeatures_DC : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_Pipe2D == 1 ? chipFeatures_PIPE_2D : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_ETC1TextureCompression == 1 ? chipFeatures_ETC1_TEXTURE_COMPRESSION : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_FastScaler == 1 ? chipFeatures_FAST_SCALER : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_HighDynamicRange == 1 ? chipFeatures_HIGH_DYNAMIC_RANGE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_YUV420Tiler == 1 ? chipFeatures_YUV420_TILER : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_ModuleCG == 1 ? chipFeatures_MODULE_CG : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_MinArea == 1 ? chipFeatures_MIN_AREA : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_NoEZ == 1 ? chipFeatures_NO_EARLY_Z : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_No422Texture == 1 ? chipFeatures_NO_422_TEXTURE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BufferInterleaving == 1 ? chipFeatures_BUFFER_INTERLEAVING : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_ByteWrite2D == 1 ? chipFeatures_BYTE_WRITE_2D : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_NoScaler == 1 ? chipFeatures_NO_SCALER : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_YUY2Averaging == 1 ? chipFeatures_YUY2_AVERAGING : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_HalfPECache == 1 ? chipFeatures_HALF_PE_CACHE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_HalfTXCache == 1 ? chipFeatures_HALF_TX_CACHE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_YUY2RenderTarget == 1 ? chipFeatures_YUY2_RENDER_TARGET : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_Mem32BitSupport == 1 ? chipFeatures_MEM32 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_PipeVG == 1 ? chipFeatures_PIPE_VG : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_VGTS == 1 ? chipFeatures_VGTS : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_FE20 == 1 ? chipFeatures_FE20 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_ByteWrite3D == 1 ? chipFeatures_BYTE_WRITE_3D : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_RsYuvTarget == 1 ? chipFeatures_RS_YUV_TARGET : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_FE20BitIndex == 1 ? chipFeatures_32_BIT_INDICES : 0);

			uint32_t features0 =
				(gChipInfo_STM32_6_2_4[i].REG_FlipY == 1 ? chipMinorFeatures0_FLIP_Y : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_DualReturnBus == 1 ? chipMinorFeatures0_DUAL_RETURN_BUS : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_EndiannessConfig == 1 ? chipMinorFeatures0_ENDIANNESS_CONFIG : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_Texture8K == 1 ? chipMinorFeatures0_TEXTURE_8K : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_CorrectTextureConverter == 1 ? chipMinorFeatures0_CORRECT_TEXTURE_CONVERTER : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_SpecialMsaaLod == 1 ? chipMinorFeatures0_SPECIAL_MSAA_LOD : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_FastClearFlush == 1 ? chipMinorFeatures0_FAST_CLEAR_FLUSH : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_2DPE20 == 1 ? chipMinorFeatures0_2DPE20 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_CorrectAutoDisable == 1 ? chipMinorFeatures0_CORRECT_AUTO_DISABLE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_Render8K == 1 ? chipMinorFeatures0_RENDERTARGET_8K : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_TileStatus2Bits == 1 ? chipMinorFeatures0_2BITPERTILE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_SeparateTileStatusWhenInterleaved == 1 ? chipMinorFeatures0_SEPARATE_TILE_STATUS_WHEN_INTERLEAVED : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_SuperTiled32x32 == 1 ? chipMinorFeatures0_SUPER_TILED : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_VG20 == 1 ? chipMinorFeatures0_VG_20 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_TSExtendedCommands == 1 ? chipMinorFeatures0_TS_EXTENDED_COMMANDS : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_CompressionFifoFixed == 1 ? chipMinorFeatures0_COMPRESSION_FIFO_FIXED : 0) |
#ifdef ESTIMATED_VALUE
				(gChipInfo_STM32_6_2_4[i].REG_ExtraShaderInstructions0 == 1 ? chipMinorFeatures0_HAS_SIGN_FLOOR_CEIL : 0) | //?
				(gChipInfo_STM32_6_2_4[i].REG_ExtraShaderInstructions1 == 1 ? chipMinorFeatures0_HAS_SQRT_TRIG : 0) | //?
				(gChipInfo_STM32_6_2_4[i].REG_DefaultReg0 == 1 ? chipMinorFeatures0_MORE_MINOR_FEATURES : 0) | //?
#endif
				(gChipInfo_STM32_6_2_4[i].REG_VGFilter == 1 ? chipMinorFeatures0_VG_FILTER : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_VG21 == 1 ? chipMinorFeatures0_VG_21 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_ShaderGetsW == 1 ? chipMinorFeatures0_SHADER_HAS_W : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_MC20 == 1 ? chipMinorFeatures0_MC20 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_ShaderMSAASideband == 1 ? chipMinorFeatures0_MSAA_SIDEBAND : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes0 == 1 ? chipMinorFeatures0_BUG_FIXES0 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_VAA == 1 ? chipMinorFeatures0_VAA : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BypassInMSAA == 1 ? chipMinorFeatures0_BYPASS_IN_MSAA : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_HierarchicalZ == 1 ? chipMinorFeatures0_HZ : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_NewTexture == 1 ? chipMinorFeatures0_NEW_TEXTURE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_A8TargetSupport == 1 ? chipMinorFeatures0_2D_A8_TARGET : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_CorrectStencil == 1 ? chipMinorFeatures0_CORRECT_STENCIL : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_EnhanceVR == 1 ? chipMinorFeatures0_ENHANCE_VR : 0);

			uint32_t features1 =
				(gChipInfo_STM32_6_2_4[i].REG_RSUVSwizzle == 1 ? chipMinorFeatures1_RSUV_SWIZZLE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_V2Compression == 1 ? chipMinorFeatures1_V2_COMPRESSION : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_VGDoubleBuffer == 1 ? chipMinorFeatures1_VG_DOUBLE_BUFFER : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes1 == 1 ? chipMinorFeatures1_EXTRA_EVENT_STATES : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes2 == 1 ? chipMinorFeatures1_NO_STRIPING_NEEDED : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_TextureStride == 1 ? chipMinorFeatures1_TEXTURE_STRIDE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes3 == 1 ? chipMinorFeatures1_BUG_FIXES3 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_CorrectAutoDisable1 == 1 ? chipMinorFeatures1_AUTO_DISABLE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_AutoRestartTS == 1 ? chipMinorFeatures1_AUTO_RESTART_TS : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes4 == 1 ? chipMinorFeatures1_DISABLE_PE_GATING : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_L2Windowing == 1 ? chipMinorFeatures1_L2_WINDOWING : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_HalfFloatPipe == 1 ? chipMinorFeatures1_HALF_FLOAT : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_PixelDither == 1 ? chipMinorFeatures1_PIXEL_DITHER : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_TwoStencilReference == 1 ? chipMinorFeatures1_TWO_STENCIL_REFERENCE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_ExtendedPixelFormat == 1 ? chipMinorFeatures1_EXTENDED_PIXEL_FORMAT : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_CorrectMinMaxDepth == 1 ? chipMinorFeatures1_CORRECT_MIN_MAX_DEPTH : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_DitherAndFilterPlusAlpha2D == 1 ? chipMinorFeatures1_2D_DITHER : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes5 == 1 ? chipMinorFeatures1_BUG_FIXES5 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_New2D == 1 ? chipMinorFeatures1_NEW_2D : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_NewFloatingPointArithmetic == 1 ? chipMinorFeatures1_NEW_FP : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_TextureHorizontalAlignmentSelect == 1 ? chipMinorFeatures1_TEXTURE_HALIGN : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_NonPowerOfTwo == 1 ? chipMinorFeatures1_NON_POWER_OF_TWO : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_LinearTextureSupport == 1 ? chipMinorFeatures1_LINEAR_TEXTURE_SUPPORT : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_Halti0 == 1 ? chipMinorFeatures1_HALTI0 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_CorrectOverflowVG == 1 ? chipMinorFeatures1_CORRECT_OVERFLOW_VG : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_NegativeLogFix == 1 ? chipMinorFeatures1_NEGATIVE_LOG_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_ResolveOffset == 1 ? chipMinorFeatures1_RESOLVE_OFFSET : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_OkToGateAxiClock == 1 ? chipMinorFeatures1_OK_TO_GATE_AXI_CLOCK : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_MMU == 1 ? chipMinorFeatures1_MMU_VERSION : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_WideLine == 1 ? chipMinorFeatures1_WIDE_LINE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes6 == 1 ? chipMinorFeatures1_BUG_FIXES6 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_FcFlushStall == 1 ? chipMinorFeatures1_FC_FLUSH_STALL : 0);

			uint32_t features2 = (gChipInfo_STM32_6_2_4[i].REG_LineLoop == 1 ? chipMinorFeatures2_LINE_LOOP : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_LogicOp == 1 ? chipMinorFeatures2_LOGIC_OP : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_SeamlessCubeMap == 1 ? chipMinorFeatures2_SEAMLESS_CUBE_MAP : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_SuperTiledTexture == 1 ? chipMinorFeatures2_SUPERTILED_TEXTURE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_LinearPE == 1 ? chipMinorFeatures2_LINEAR_PE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_RectPrimitive == 1 ? chipMinorFeatures2_RECT_PRIMITIVE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_Composition == 1 ? chipMinorFeatures2_COMPOSITION : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_CorrectAutoDisableCountWidth == 1 ? chipMinorFeatures2_CORRECT_AUTO_DISABLE_COUNT : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_PESwizzle == 1 ? chipMinorFeatures2_PE_SWIZZLE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_EndEvent == 1 ? chipMinorFeatures2_END_EVENT : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_S1S8 == 1 ? chipMinorFeatures2_S1S8 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_Halti1 == 1 ? chipMinorFeatures2_HALTI1 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_RGB888 == 1 ? chipMinorFeatures2_RGB888 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_TX_YUVAssembler == 1 ? chipMinorFeatures2_TX__YUV_ASSEMBLER : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_DynamicFrequencyScaling == 1 ? chipMinorFeatures2_DYNAMIC_FREQUENCY_SCALING : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_TXFilter == 1 ? chipMinorFeatures2_TX_FILTER : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_FullDirectFB == 1 ? chipMinorFeatures2_FULL_DIRECTFB : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_OnePass2DFilter == 1 ? chipMinorFeatures2_2D_TILING : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_ThreadWalkerInPS == 1 ? chipMinorFeatures2_THREAD_WALKER_IN_PS : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_TileFiller == 1 ? chipMinorFeatures2_TILE_FILLER : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_YUVStandard == 1 ? chipMinorFeatures2_YUV_STANDARD : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_MultiSourceBlt == 1 ? chipMinorFeatures2_2D_MULTI_SOURCE_BLIT : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_YUVConversion == 1 ? chipMinorFeatures2_YUV_CONVERSION : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_FlushFixed2D == 1 ? chipMinorFeatures2_FLUSH_FIXED_2D : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_Interleaver == 1 ? chipMinorFeatures2_INTERLEAVER : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_MixedStreams == 1 ? chipMinorFeatures2_MIXED_STREAMS : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_L2CacheFor2D420 == 1 ? chipMinorFeatures2_2D_420_L2CACHE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes7 == 1 ? chipMinorFeatures2_BUG_FIXES7 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_NoIndexPattern == 1 ? chipMinorFeatures2_2D_NO_INDEX8_BRUSH : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_TextureTileStatus == 1 ? chipMinorFeatures2_TEXTURE_TILED_READ : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_DecompressZ16 == 1 ? chipMinorFeatures2_DECOMPRESS_Z16 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes8 == 1 ? chipMinorFeatures2_BUG_FIXES8 : 0);

			uint32_t features3 = (gChipInfo_STM32_6_2_4[i].REG_DERotationStallFix == 1 ? chipMinorFeatures3_ROTATION_STALL_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_OclOnly == 1 ? chipMinorFeatures3_OCL_ONLY : 0) |
#ifdef ESTIMATED_VALUE
				(gChipInfo_STM32_6_2_4[i].REG_NewFeatures0 == 1 ? chipMinorFeatures3_2D_MULTI_SOURCE_BLT_EX : 0) |
#endif
				(gChipInfo_STM32_6_2_4[i].REG_InstructionCache == 1 ? chipMinorFeatures3_INSTRUCTION_CACHE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_GeometryShader == 1 ? chipMinorFeatures3_GEOMETRY_SHADER : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_TexCompressionSupertiled == 1 ? chipMinorFeatures3_TEX_COMPRESSION_SUPERTILED : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_Generics == 1 ? chipMinorFeatures3_GENERICS : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes9 == 1 ? chipMinorFeatures3_BUG_FIXES9 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_FastMSAA == 1 ? chipMinorFeatures3_FAST_MSAA : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_WClip == 1 ? chipMinorFeatures3_WCLIP : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes10 == 1 ? chipMinorFeatures3_BUG_FIXES10 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_UnifiedSamplers == 1 ? chipMinorFeatures3_UNIFIED_SAMPLERS : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes11 == 1 ? chipMinorFeatures3_BUG_FIXES11 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_PerformanceCounters == 1 ? chipMinorFeatures3_PERFORMANCE_COUNTERS : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_ExtraShaderInstructions2 == 1 ? chipMinorFeatures3_HAS_FAST_TRANSCENDENTALS : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes12 == 1 ? chipMinorFeatures3_BUG_FIXES12 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes13 == 1 ? chipMinorFeatures3_BUG_FIXES13 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_DEEnhancements1 == 1 ? chipMinorFeatures3_DE_ENHANCEMENTS1 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_ACE == 1 ? chipMinorFeatures3_ACE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_TXEnhancements1 == 1 ? chipMinorFeatures3_TX_ENHANCEMENTS1 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_SHEnhancements1 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS1 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_SHEnhancements2 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS2 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_PEEnhancements1 == 1 ? chipMinorFeatures3_PE_ENHANCEMENTS1 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_DEEnhancements2 == 1 ? chipMinorFeatures3_2D_FC_SOURCE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes14 == 1 ? chipMinorFeatures3_BUG_FIXES_14 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_PowerOptimizations0 == 1 ? chipMinorFeatures3_POWER_OPTIMIZATIONS_0 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_NewHZ == 1 ? chipMinorFeatures3_NEW_HZ : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes15 == 1 ? chipMinorFeatures3_PE_DITHER_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_DEEnhancements3 == 1 ? chipMinorFeatures3_DE_ENHANCEMENTS3 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_SHEnhancements3 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS3 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_SHEnhancements4 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS4 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_TXEnhancements2 == 1 ? chipMinorFeatures3_TX_ENHANCEMENTS2 : 0);

			uint32_t features4 = (gChipInfo_STM32_6_2_4[i].REG_FEEnhancements1 == 1 ? chipMinorFeatures4_FE_ENHANCEMENTS1 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_PEEnhancements2 == 1 ? chipMinorFeatures4_PE_ENHANCEMENTS2 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_PAEnhancements1 == 1 ? chipMinorFeatures4_FRUSTUM_CLIP_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_DENoGamma == 1 ? chipMinorFeatures4_DE_NO_GAMMA : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_PAEnhancements2 == 1 ? chipMinorFeatures4_PA_ENHANCEMENTS_2 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_DEEnhancements4 == 1 ? chipMinorFeatures4_2D_GAMMA : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_PEEnhancements3 == 1 ? chipMinorFeatures4_SINGLE_BUFFER : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_HIEnhancements1 == 1 ? chipMinorFeatures4_HI_ENHANCEMENTS_1 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_TXEnhancements3 == 1 ? chipMinorFeatures4_TX_ENHANCEMENTS_3 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_SHEnhancements5 == 1 ? chipMinorFeatures4_SH_ENHANCEMENTS_5 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_FEEnhancements2 == 1 ? chipMinorFeatures4_FE_ENHANCEMENTS_2 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes16 == 1 ? chipMinorFeatures4_TX_LERP_PRECISION_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_DEEnhancements5 == 1 ? chipMinorFeatures4_2D_COLOR_SPACE_CONVERSION : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_TXEnhancements4 == 1 ? chipMinorFeatures4_TEXTURE_ASTC : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_PEEnhancements4 == 1 ? chipMinorFeatures4_PE_ENHANCEMENTS_4 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_MCEnhancements1 == 1 ? chipMinorFeatures4_MC_ENHANCEMENTS_1 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_Halti2 == 1 ? chipMinorFeatures4_HALTI2 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_DEMirrorRotate == 1 ? chipMinorFeatures4_2D_MIRROR_EXTENSION : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_SmallMSAA == 1 ? chipMinorFeatures4_SMALL_MSAA : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes17 == 1 ? chipMinorFeatures4_BUG_FIXES_17 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_Rasterizer2 == 1 ? chipMinorFeatures4_NEW_RA : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_DualPipeOPF == 1 ? chipMinorFeatures4_2D_OPF_YUV_OUTPUT : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_MultiSrcV2 == 1 ? chipMinorFeatures4_2D_MULTI_SOURCE_BLT_EX2 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_CSCV2 == 1 ? chipMinorFeatures4_NO_USER_CSC : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_PAEnhancements3 == 1 ? chipMinorFeatures4_ZFIXES : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes18 == 1 ? chipMinorFeatures4_BUG_FIXES18 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_Compression2D == 1 ? chipMinorFeatures4_2D_COMPRESSION : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_Probe == 1 ? chipMinorFeatures4_PROBE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_MediumPrecision == 1 ? chipMinorFeatures4_MEDIUM_PRECISION : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_DESupertile == 1 ? chipMinorFeatures4_2D_SUPER_TILE_VERSION : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes19 == 1 ? chipMinorFeatures4_BUG_FIXES19 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_SHEnhancements6 == 1 ? chipMinorFeatures4_SH_ENHANCEMENTS6 : 0);

			uint32_t features5 = (gChipInfo_STM32_6_2_4[i].REG_SHEnhancements7 == 1 ? chipMinorFeatures5_SH_ENHANCEMENTS7 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes20 == 1 ? chipMinorFeatures5_BUG_FIXES20 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_DEAddress40 == 1 ? chipMinorFeatures5_DE_ADDRESS_40 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_MiniMMUFix == 1 ? chipMinorFeatures5_MINI_MMU_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_EEZ == 1 ? chipMinorFeatures5_EEZ : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes21 == 1 ? chipMinorFeatures5_BUG_FIXES21 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_ExtraVgCaps == 1 ? chipMinorFeatures5_EXTRA_VG_CAPS : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_MultiSrcV15 == 1 ? chipMinorFeatures5_MULTI_SRC_V15 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes22 == 1 ? chipMinorFeatures5_BUG_FIXES22 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_Halti3 == 1 ? chipMinorFeatures5_HALTI3 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_TessellationShaders == 1 ? chipMinorFeatures5_TESSELATION_SHADERS : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_OnePass2DFilter == 1 ? chipMinorFeatures5_2D_ONE_PASS_FILTER_TAP : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_MultiSrcV2StrQuad == 1 ? chipMinorFeatures5_MULTI_SRC_V2_STR_QUAD : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_SeperateSRCAndDstCache == 1 ? chipMinorFeatures5_SEPARATE_SRC_DST : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_Halti4 == 1 ? chipMinorFeatures5_HALTI4 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_RAWriteDepth == 1 ? chipMinorFeatures5_RA_WRITE_DEPTH : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_AndroidOnly == 1 ? chipMinorFeatures5_ANDROID_ONLY : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_HasChipProductReg == 1 ? chipMinorFeatures5_HAS_PRODUCTID : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_TXSupportDEC == 1 ? chipMinorFeatures5_TX_SUPPORT_DEC : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_S8MSAACompression == 1 ? chipMinorFeatures5_S8_MSAA_COMPRESSION : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixesIn544 == 1 ? chipMinorFeatures5_PE_DITHER_FIX2 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_L2CacheRemove == 1 ? chipMinorFeatures5_L2_CACHE_REMOVE : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_FEAllowRndVtxCnt == 1 ? chipMinorFeatures5_FE_ALLOW_RND_VTX_CNT : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_CubeMapFL28 == 1 ? chipMinorFeatures5_CUBE_MAP_FL28 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_TX6bitFrac == 1 ? chipMinorFeatures5_TX_6BIT_FRAC : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_FEAllowStallPrefetchEng == 1 ? chipMinorFeatures5_FE_ALLOW_STALL_PREFETCH_ENG : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_ThirdPartyCompression == 1 ? chipMinorFeatures5_THIRD_PARTY_COMPRESSION : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_RSS8 == 1 ? chipMinorFeatures5_RS_DEPTHSTENCIL_NATIVE_SUPPORT : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_MSAACoherencyCheck == 1 ? chipMinorFeatures5_V2_MSAA_COMP_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_Halti5 == 1 ? chipMinorFeatures5_HALTI5 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_Evis == 1 ? chipMinorFeatures5_EVIS : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BltEngine == 1 ? chipMinorFeatures5_BLT_ENGINE : 0);

			uint32_t features6 = (gChipInfo_STM32_6_2_4[i].REG_BugFixes23 == 1 ? chipMinorFeatures6_BUG_FIXES_23 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_BugFixes24 == 1 ? chipMinorFeatures6_BUG_FIXES_24 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_DEC == 1 ? chipMinorFeatures6_DEC : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_VSTileNV12 == 1 ? chipMinorFeatures6_VS_TILE_NV12 : 0) |
				(gChipInfo_STM32_6_2_4[i].REG_VSTileNV12_10BIT == 1 ? chipMinorFeatures6_VS_TILE_NV12_10BIT : 0) |
				//(gChipInfo_STM32_6_2_4[i].REG_DisableVIP == 1 ?  : 0) |
				(gChipInfo_STM32_6_2_4[i].RenderTarget8 == 1 ? chipMinorFeatures6_RENDER_TARGET_8 : 0) |
				(gChipInfo_STM32_6_2_4[i].TxLodFlowCorrection == 1 ? chipMinorFeatures6_TEX_LOD_FLOW_CORR : 0) |
				(gChipInfo_STM32_6_2_4[i].FaceLod == 1 ? chipMinorFeatures6_FACE_LOD : 0) |
				(gChipInfo_STM32_6_2_4[i].MultiCoreSemaphoreStallV2 == 1 ? chipMinorFeatures6_MULTI_CORE_SEMAPHORE_STALL_V2 : 0) |
				(gChipInfo_STM32_6_2_4[i].VMSAA == 1 ? chipMinorFeatures6_VMSAA : 0) |
				(gChipInfo_STM32_6_2_4[i].ChipEnableLink == 1 ? chipMinorFeatures6_CHIP_ENABLE_LINK : 0) |
				(gChipInfo_STM32_6_2_4[i].MULTI_SRC_BLT_1_5_ENHANCEMENT == 1 ? chipMinorFeatures6_MULTI_SRC_BLT_1_5_ENHANCEMENT : 0) |
				(gChipInfo_STM32_6_2_4[i].MULTI_SRC_BLT_BILINEAR_FILTER == 1 ? chipMinorFeatures6_MULTI_SRC_BLT_BILINEAR_FILTER : 0) |
				(gChipInfo_STM32_6_2_4[i].RA_HZEZ_CLOCK_CONTROL == 1 ? chipMinorFeatures6_RA_HZEZ_CLOCK_CONTROL : 0) |
				(gChipInfo_STM32_6_2_4[i].CACHE128B256BPERLINE == 1 ? chipMinorFeatures6_CACHE128B256BPERLINE : 0) |
				(gChipInfo_STM32_6_2_4[i].V4Compression == 1 ? chipMinorFeatures6_V4_COMPRESSION : 0) |
				//(gChipInfo_STM32_6_2_4[i].DE2D_MAJOR_SUPER_TILE == 1 ?  : 0) | //?
				(gChipInfo_STM32_6_2_4[i].PE2D_MAJOR_SUPER_TILE == 1 ? chipMinorFeatures6_PE2D_MAJOR_SUPER_TILE : 0) |
				(gChipInfo_STM32_6_2_4[i].PE_32BPC_COLORMASK_FIX == 1 ? chipMinorFeatures6_PE_32BPC_COLORMASK_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].ALPHA_BLENDING_OPT == 1 ? chipMinorFeatures6_ALPHA_BLENDING_OPT : 0) |
				(gChipInfo_STM32_6_2_4[i].NEW_GPIPE == 1 ? chipMinorFeatures6_NEW_GPIPE : 0) |
				(gChipInfo_STM32_6_2_4[i].PIPELINE_32_ATTRIBUTES == 1 ? chipMinorFeatures6_PIPELINE_32_ATTRIBUTES : 0) |
				(gChipInfo_STM32_6_2_4[i].MSAA_SHADING == 1 ? chipMinorFeatures6_MSAA_SHADING : 0) |
				(gChipInfo_STM32_6_2_4[i].NO_ANISTRO_FILTER == 1 ? chipMinorFeatures6_NO_ANISTRO_FILTER : 0) |
				(gChipInfo_STM32_6_2_4[i].NO_ASTC == 1 ? chipMinorFeatures6_NO_ASTC : 0) |
				(gChipInfo_STM32_6_2_4[i].NO_DXT == 1 ? chipMinorFeatures6_NO_DXT : 0) |
				(gChipInfo_STM32_6_2_4[i].HWTFB == 1 ? chipMinorFeatures6_HWTFB : 0) |
				(gChipInfo_STM32_6_2_4[i].RA_DEPTH_WRITE_MSAA1X_FIX == 1 ? chipMinorFeatures6_RA_DEPTH_WRITE_MSAA1X_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].EZHZ_CLOCKGATE_FIX == 1 ? chipMinorFeatures6_EZHZ_CLOCKGATE_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_SNAP2PAGE_FIX == 1 ? chipMinorFeatures6_SH_SNAP2PAGE_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_HALFDEPENDENCY_FIX == 1 ? chipMinorFeatures6_SH_HALFDEPENDENCY_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].USC_MCFILL_FIX == 1 ? chipMinorFeatures6_USC_MCFILL_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].TPG_TCPERF_FIX == 1 ? chipMinorFeatures6_TPG_TCPERF_FIX : 0);

			uint32_t features7 = (gChipInfo_STM32_6_2_4[i].USC_MDFIFO_OVERFLOW_FIX == 1 ? chipMinorFeatures7_USC_MDFIFO_OVERFLOW_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_TEXLD_BARRIER_IN_CS_FIX == 1 ? chipMinorFeatures7_SH_TEXLD_BARRIER_IN_CS_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].RS_NEW_BASEADDR == 1 ? chipMinorFeatures7_RS_NEW_BASEADDR : 0) |
				(gChipInfo_STM32_6_2_4[i].PE_8bpp_DUALPIPE_FIX == 1 ? chipMinorFeatures7_PE_8BPP_DUALPIPE_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_ADVANCED_INSTR == 1 ? chipMinorFeatures7_SH_ADVANCED_INSTR : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_FLAT_INTERPOLATION_DUAL16_FIX == 1 ? chipMinorFeatures7_SH_FLAT_INTERPOLATION_DUAL16_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].USC_CONTINUOUS_FLUS_FIX == 1 ? chipMinorFeatures7_USC_CONTINUOUS_FLUS_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_SUPPORT_V4 == 1 ? chipMinorFeatures7_SH_SUPPORT_V4 : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_SUPPORT_ALPHA_KILL == 1 ? chipMinorFeatures7_SH_SUPPORT_ALPHA_KILL : 0) |
				(gChipInfo_STM32_6_2_4[i].PE_NO_ALPHA_TEST == 1 ? chipMinorFeatures7_PE_NO_ALPHA_TEST : 0) |
				(gChipInfo_STM32_6_2_4[i].TX_LOD_NEAREST_SELECT == 1 ? chipMinorFeatures7_TX_LOD_NEAREST_SELECT : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_FIX_LDEXP == 1 ? chipMinorFeatures7_SH_FIX_LDEXP : 0) |
				(gChipInfo_STM32_6_2_4[i].SUPPORT_MOVAI == 1 ? chipMinorFeatures7_SUPPORT_MOVAI : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_SNAP2PAGE_MAXPAGES_FIX == 1 ? chipMinorFeatures7_SH_SNAP2PAGE_MAXPAGES_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].PE_RGBA16I_FIX == 1 ? chipMinorFeatures7_PE_RGBA16I_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].BLT_8bpp_256TILE_FC_FIX == 1 ? chipMinorFeatures7_BLT_8bpp_256TILE_FC_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].PE_64bit_FENCE_FIX == 1 ? chipMinorFeatures7_PE_64BIT_FENCE_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].USC_FULL_CACHE_FIX == 1 ? chipMinorFeatures7_USC_FULL_CACHE_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].TX_YUV_ASSEMBLER_10BIT == 1 ? chipMinorFeatures7_TX_YUV_ASSEMBLER_10BIT : 0) |
				(gChipInfo_STM32_6_2_4[i].FE_32bit_INDEX_FIX == 1 ? chipMinorFeatures7_FE_32BIT_INDEX_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].BLT_64bpp_MASKED_CLEAR_FIX == 1 ? chipMinorFeatures7_BLT_64BPP_MASKED_CLEAR_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].SECURITY == 1 ? chipMinorFeatures7_BIT_SECURITY : 0) |
				(gChipInfo_STM32_6_2_4[i].ROBUSTNESS == 1 ? chipMinorFeatures7_BIT_ROBUSTNESS : 0) |
				(gChipInfo_STM32_6_2_4[i].USC_ATOMIC_FIX == 1 ? chipMinorFeatures7_USC_ATOMIC_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_PSO_MSAA1x_FIX == 1 ? chipMinorFeatures7_SH_PSO_MSAA1x_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].USC_VX_PERF_FIX == 1 ? chipMinorFeatures7_BIT_USC_VX_PERF_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].EVIS_NO_ABSDIFF == 1 ? chipMinorFeatures7_EVIS_NO_ABSDIFF : 0) |
				(gChipInfo_STM32_6_2_4[i].EVIS_NO_BITREPLACE == 1 ? chipMinorFeatures7_EVIS_NO_BITREPLACE : 0) |
				(gChipInfo_STM32_6_2_4[i].EVIS_NO_BOXFILTER == 1 ? chipMinorFeatures7_EVIS_NO_BOXFILTER : 0) |
				(gChipInfo_STM32_6_2_4[i].EVIS_NO_CORDIAC == 1 ? chipMinorFeatures7_EVIS_NO_CORDIAC : 0) |
				(gChipInfo_STM32_6_2_4[i].EVIS_NO_DP32 == 1 ? chipMinorFeatures7_EVIS_NO_DP32 : 0) |
				(gChipInfo_STM32_6_2_4[i].EVIS_NO_FILTER == 1 ? chipMinorFeatures7_EVIS_NO_FILTER : 0);

			uint32_t features8 =
				(gChipInfo_STM32_6_2_4[i].EVIS_NO_IADD == 1 ? chipMinorFeatures8_EVIS_NO_IADD : 0) |
				(gChipInfo_STM32_6_2_4[i].EVIS_NO_SELECTADD == 1 ? chipMinorFeatures8_EVIS_NO_SELECTADD : 0) |
				(gChipInfo_STM32_6_2_4[i].EVIS_LERP_7OUTPUT == 1 ? chipMinorFeatures8_EVIS_LERP_7OUTPUT : 0) |
				(gChipInfo_STM32_6_2_4[i].EVIS_ACCSQ_8OUTPUT == 1 ? chipMinorFeatures8_EVIS_ACCSQ_8OUTPUT : 0) |
				(gChipInfo_STM32_6_2_4[i].USC_GOS_ADDR_FIX == 1 ? chipMinorFeatures8_USC_GOS_ADDR_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].TX_8bit_UVFrac == 1 ? chipMinorFeatures8_TX_8BIT_UVFRAC : 0) |
				(gChipInfo_STM32_6_2_4[i].TX_DESC_CACHE_CLOCKGATE_FIX == 1 ? chipMinorFeatures8_TX_DESC_CACHE_CLOCKGATE_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].RSBLT_MSAA_DECOMPRESSION == 1 ? chipMinorFeatures8_RSBLT_MSAA_DECOMPRESSION : 0) |
				(gChipInfo_STM32_6_2_4[i].TX_INTEGER_COORDINATE == 1 ? chipMinorFeatures8_TX_INTEGER_COORDINATE : 0) |
				(gChipInfo_STM32_6_2_4[i].DRAWID == 1 ? chipMinorFeatures8_DRAWID : 0) |
				(gChipInfo_STM32_6_2_4[i].PSIO_SAMPLEMASK_IN_R0ZW_FIX == 1 ? chipMinorFeatures8_PSIO_SAMPLEMASK_IN_R0ZW_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].TX_INTEGER_COORDINATE_V2 == 1 ? chipMinorFeatures8_TX_INTEGER_COORDINATE_V2 : 0) |
				(gChipInfo_STM32_6_2_4[i].MULTI_CORE_BLOCK_SET_CONFIG == 1 ? chipMinorFeatures8_MULTI_CORE_BLOCK_SET_CONFIG : 0) |
				(gChipInfo_STM32_6_2_4[i].VG_RESOLVE_ENGINE == 1 ? chipMinorFeatures8_VG_RESOLVE_ENGINE : 0) |
				(gChipInfo_STM32_6_2_4[i].VG_PE_COLOR_KEY == 1 ? chipMinorFeatures8_VG_PE_COLOR_KEY : 0) |
				(gChipInfo_STM32_6_2_4[i].VG_IM_INDEX_FORMAT == 1 ? chipMinorFeatures8_VG_IM_INDEX_FORMAT : 0) |
				(gChipInfo_STM32_6_2_4[i].SNAPPAGE_CMD == 1 ? chipMinorFeatures8_SNAPPAGE_CMD : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_NO_INDEX_CONST_ON_A0 == 1 ? chipMinorFeatures8_SH_NO_INDEX_CONST_ON_A0 : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_NO_ONECONST_LIMIT == 1 ? chipMinorFeatures8_SH_NO_ONECONST_LIMIT : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_IMG_LDST_ON_TEMP == 1 ? chipMinorFeatures8_SH_IMG_LDST_ON_TEMP : 0) |
				(gChipInfo_STM32_6_2_4[i].COMPUTE_ONLY == 1 ? chipMinorFeatures8_COMPUTE_ONLY : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_IMG_LDST_CLAMP == 1 ? chipMinorFeatures8_SH_IMG_LDST_CLAMP : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_ICACHE_ALLOC_COUNT_FIX == 1 ? chipMinorFeatures8_SH_ICACHE_ALLOC_COUNT_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_ICACHE_PREFETCH == 1 ? chipMinorFeatures8_SH_ICACHE_PREFETCH : 0) |
				(gChipInfo_STM32_6_2_4[i].PE2D_SEPARATE_CACHE == 1 ? chipMinorFeatures8_PE2D_SEPARATE_CACHE : 0) |
				(gChipInfo_STM32_6_2_4[i].VG_AYUV_INPUT_OUTPUT == 1 ? chipMinorFeatures8_VG_AYUV_INPUT_OUTPUT : 0) |
				(gChipInfo_STM32_6_2_4[i].VG_DOUBLE_IMAGE == 1 ? chipMinorFeatures8_VG_DOUBLE_IMAGE : 0) |
				(gChipInfo_STM32_6_2_4[i].VG_RECTANGLE_STRIPE_MODE == 1 ? chipMinorFeatures8_VG_RECTANGLE_STRIPE_MODE : 0) |
				(gChipInfo_STM32_6_2_4[i].VG_MMU == 1 ? chipMinorFeatures8_VG_MMU : 0) |
				(gChipInfo_STM32_6_2_4[i].VG_IM_FILTER == 1 ? chipMinorFeatures8_VG_IM_FILTER : 0) |
				(gChipInfo_STM32_6_2_4[i].VG_IM_YUV_PACKET == 1 ? chipMinorFeatures8_VG_IM_YUV_PACKET : 0) |
				(gChipInfo_STM32_6_2_4[i].VG_IM_YUV_PLANAR == 1 ? chipMinorFeatures8_VG_IM_YUV_PLANAR : 0);

			uint32_t features9 =
				(gChipInfo_STM32_6_2_4[i].VG_PE_YUV_PACKET == 1 ? chipMinorFeatures9_VG_PE_YUV_PACKET : 0) |
				(gChipInfo_STM32_6_2_4[i].VG_COLOR_PRECISION_8_BIT == 1 ? chipMinorFeatures9_VG_COLOR_PRECISION_8_BIT : 0) |
				(gChipInfo_STM32_6_2_4[i].PE_MSAA_OQ_FIX == 1 ? chipMinorFeatures9_PE_MSAA_OQ_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].PSIO_MSAA_CL_FIX == 1 ? chipMinorFeatures9_PSIO_MSAA_CL_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].USC_DEFER_FILL_FIX == 1 ? chipMinorFeatures9_USC_DEFER_FILL_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_CLOCK_GATE_FIX == 1 ? chipMinorFeatures9_SH_CLOCK_GATE_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].FE_NEED_DUMMYDRAW == 1 ? chipMinorFeatures9_FE_NEED_DUMMYDRAW : 0) |
				(gChipInfo_STM32_6_2_4[i].PE2D_LINEAR_YUV420_OUTPUT == 1 ? chipMinorFeatures9_PE2D_LINEAR_YUV420_OUTPUT : 0) |
				(gChipInfo_STM32_6_2_4[i].PE2D_LINEAR_YUV420_10BIT == 1 ? chipMinorFeatures9_PE2D_LINEAR_YUV420_10BIT : 0) |
				(gChipInfo_STM32_6_2_4[i].MULTI_CLUSTER == 1 ? chipMinorFeatures9_MULTI_CLUSTER : 0) |
				(gChipInfo_STM32_6_2_4[i].VG_TS_CULLING == 1 ? chipMinorFeatures9_VG_TS_CULLING : 0) |
				(gChipInfo_STM32_6_2_4[i].VG_FP25 == 1 ? chipMinorFeatures9_VG_FP25 : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_MULTI_WG_PACK == 1 ? chipMinorFeatures9_SH_MULTI_WG_PACK : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_DUAL16_SAMPLEMASK_ZW == 1 ? chipMinorFeatures9_SH_DUAL16_SAMPLEMASK_ZW : 0) |
				(gChipInfo_STM32_6_2_4[i].TPG_TRIVIAL_MODE_FIX == 1 ? chipMinorFeatures9_TPG_TRIVIAL_MODE_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].TX_ASTC_MULTISLICE_FIX == 1 ? chipMinorFeatures9_TX_ASTC_MULTISLICE_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].FE_ROBUST_FIX == 1 ? chipMinorFeatures9_FE_ROBUST_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_GPIPE_ACCESS_FULLTEMPS == 1 ? chipMinorFeatures9_SH_GPIPE_ACCESS_FULLTEMPS : 0) |
				(gChipInfo_STM32_6_2_4[i].PSIO_INTERLOCK == 1 ? chipMinorFeatures9_PSIO_INTERLOCK : 0) |
				(gChipInfo_STM32_6_2_4[i].PA_WIDELINE_FIX == 1 ? chipMinorFeatures9_PA_WIDELINE_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].WIDELINE_HELPER_FIX == 1 ? chipMinorFeatures9_WIDELINE_HELPER_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].G2D_3rd_PARTY_COMPRESSION_1_1 == 1 ? chipMinorFeatures9_G2D_3RD_PARTY_COMPRESSION_1_1 : 0) |
				(gChipInfo_STM32_6_2_4[i].TX_FLUSH_L1CACHE == 1 ? chipMinorFeatures9_TX_FLUSH_L1CACHE : 0) |
				(gChipInfo_STM32_6_2_4[i].PE_DITHER_FIX2 == 1 ? chipMinorFeatures9_PE_DITHER_FIX2 : 0) |
				(gChipInfo_STM32_6_2_4[i].G2D_DEC400 == 1 ? chipMinorFeatures9_G2D_DEC400 : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_TEXLD_U_FIX == 1 ? chipMinorFeatures9_SH_TEXLD_U_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].MC_FCCACHE_BYTEMASK == 1 ? chipMinorFeatures9_MC_FCCACHE_BYTEMASK : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_MULTI_WG_PACK_FIX == 1 ? chipMinorFeatures9_SH_MULTI_WG_PACK_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].DC_OVERLAY_SCALING == 1 ? chipMinorFeatures9_DC_OVERLAY_SCALING : 0) |
				(gChipInfo_STM32_6_2_4[i].DC_SOURCE_ROTATION == 1 ? chipMinorFeatures9_DC_SOURCE_ROTATION : 0) |
				(gChipInfo_STM32_6_2_4[i].DC_TILED == 1 ? chipMinorFeatures9_DC_TILED : 0) |
				(gChipInfo_STM32_6_2_4[i].DC_YUV_L1 == 1 ? chipMinorFeatures9_DC_YUV_L1 : 0);

			uint32_t features10 =
				(gChipInfo_STM32_6_2_4[i].DC_D30_OUTPUT == 1 ? chipMinorFeatures10_DC_D30_OUTPUT : 0) |
				(gChipInfo_STM32_6_2_4[i].DC_MMU == 1 ? chipMinorFeatures10_DC_MMU : 0) |
				(gChipInfo_STM32_6_2_4[i].DC_COMPRESSION == 1 ? chipMinorFeatures10_DC_COMPRESSION : 0) |
				(gChipInfo_STM32_6_2_4[i].DC_QOS == 1 ? chipMinorFeatures10_DC_QOS : 0) |
				(gChipInfo_STM32_6_2_4[i].PE_ADVANCE_BLEND_PART0 == 1 ? chipMinorFeatures10_PE_ADVANCE_BLEND_PART0 : 0) |
				(gChipInfo_STM32_6_2_4[i].FE_PATCHLIST_FETCH_FIX == 1 ? chipMinorFeatures10_FE_PATCHLIST_FETCH_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].RA_CG_FIX == 1 ? chipMinorFeatures10_RA_CG_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].EVIS_VX2 == 1 ? chipMinorFeatures10_EVIS_VX2 : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_FLOAT == 1 ? chipMinorFeatures10_NN_FLOAT : 0) |
				(gChipInfo_STM32_6_2_4[i].DEC400 == 1 ? chipMinorFeatures10_DEC400 : 0) |
				(gChipInfo_STM32_6_2_4[i].LS_SUPPORT_PERCOMP_DEPENDENCY == 1 ? chipMinorFeatures10_LS_SUPPORT_PERCOMP_DEPENDENCY : 0) |
				(gChipInfo_STM32_6_2_4[i].TP_ENGINE == 1 ? chipMinorFeatures10_TP_ENGINE : 0) |
				(gChipInfo_STM32_6_2_4[i].MULTI_CORE_BLOCK_SET_CONFIG2 == 1 ? chipMinorFeatures10_MULTI_CORE_BLOCK_SET_CONFIG2 : 0) |
				(gChipInfo_STM32_6_2_4[i].PE_VMSAA_COVERAGE_CACHE_FIX == 1 ? chipMinorFeatures10_PE_VMSAA_COVERAGE_CACHE_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].SECURITY_AHB == 1 ? chipMinorFeatures10_SECURITY_AHB : 0) |
				(gChipInfo_STM32_6_2_4[i].MULTICORE_SEMAPHORESTALL_V3 == 1 ? chipMinorFeatures10_MULTICORE_SEMAPHORESTALL_V3 : 0) |
				(gChipInfo_STM32_6_2_4[i].SMALLBATCH == 1 ? chipMinorFeatures10_SMALLBATCH : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_CMPLX == 1 ? chipMinorFeatures10_SH_CMPLX : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_IDIV0_SWZL_EHS == 1 ? chipMinorFeatures10_SH_IDIV0_SWZL_EHS : 0) |
				(gChipInfo_STM32_6_2_4[i].TX_LERP_LESS_BIT == 1 ? chipMinorFeatures10_TX_LERP_LESS_BIT : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_GM_ENDIAN == 1 ? chipMinorFeatures10_SH_GM_ENDIAN : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_GM_USC_UNALLOC == 1 ? chipMinorFeatures10_SH_GM_USC_UNALLOC : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_END_OF_BB == 1 ? chipMinorFeatures10_SH_END_OF_BB : 0) |
				(gChipInfo_STM32_6_2_4[i].VIP_V7 == 1 ? chipMinorFeatures10_VIP_V7 : 0) |
				(gChipInfo_STM32_6_2_4[i].TX_BORDER_CLAMP_FIX == 1 ? chipMinorFeatures10_TX_BORDER_CLAMP_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_IMG_LD_LASTPIXEL_FIX == 1 ? chipMinorFeatures10_SH_IMG_LD_LASTPIXEL_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].ASYNC_BLT == 1 ? chipMinorFeatures10_ASYNC_BLT : 0) |
				(gChipInfo_STM32_6_2_4[i].ASYNC_FE_FENCE_FIX == 1 ? chipMinorFeatures10_ASYNC_FE_FENCE_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].PSCS_THROTTLE == 1 ? chipMinorFeatures10_PSCS_THROTTLE : 0) |
				(gChipInfo_STM32_6_2_4[i].SEPARATE_LS == 1 ? chipMinorFeatures10_SEPARATE_LS : 0) |
				(gChipInfo_STM32_6_2_4[i].MCFE == 1 ? chipMinorFeatures10_MCFE : 0) |
				(gChipInfo_STM32_6_2_4[i].WIDELINE_TRIANGLE_EMU == 1 ? chipMinorFeatures10_WIDELINE_TRIANGLE_EMU : 0);

			uint32_t features11 =
				(gChipInfo_STM32_6_2_4[i].VG_RESOLUTION_8K == 1 ? chipMinorFeatures11_VG_RESOLUTION_8K : 0) |
				(gChipInfo_STM32_6_2_4[i].FENCE_32BIT == 1 ? chipMinorFeatures11_FENCE_32BIT : 0) |
				(gChipInfo_STM32_6_2_4[i].FENCE_64BIT == 1 ? chipMinorFeatures11_FENCE_64BIT : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_INTERLEVE8 == 1 ? chipMinorFeatures11_NN_INTERLEVE8 : 0) |
				(gChipInfo_STM32_6_2_4[i].PE_DEPTH_ONLY_OQFIX == 1 ? chipMinorFeatures11_PE_DEPTH_ONLY_OQFIX : 0) |
				(gChipInfo_STM32_6_2_4[i].TP_REORDER == 1 ? chipMinorFeatures11_TP_REORDER : 0);

			linux_devices[count].features = feature;
			linux_devices[count].minor_features0 = features0;
			linux_devices[count].minor_features1 = features1;
			linux_devices[count].minor_features2 = features2;
			linux_devices[count].minor_features3 = features3;
			linux_devices[count].minor_features4 = features4;
			linux_devices[count].minor_features5 = features5;
			linux_devices[count].minor_features6 = features6;
			linux_devices[count].minor_features7 = features7;
			linux_devices[count].minor_features8 = features8;
			linux_devices[count].minor_features9 = features9;
			linux_devices[count].minor_features10 = features10;
			linux_devices[count].minor_features11 = features11;
			linux_devices[count].minor_features12 = 0;
			linux_devices[count].minor_features13 = 0;
			count++;
		}
	}

	ofstream myfile;
	myfile.open("features.txt");

	// Sort entries and write it to console and features.txt
	for (int model = 0; model < 0x9000; model++) {
		for (int revision = 0; revision < 0x9000; revision++) {
			for (int i = 0; i < count; i++) {
				if ((linux_devices[i].model == model) && ((linux_devices[i].revision & 0xFFFF) == revision)) {

					// write to console
					std::cout << "	{\n";
					std::cout << "		.model = 0x" << std::hex << linux_devices[i].model << ",\n";
					std::cout << "		.revision = 0x" << std::hex << linux_devices[i].revision << ",\n";
					std::cout << "		.product_id = 0x" << std::hex << linux_devices[i].product_id << ",\n";
					std::cout << "		.customer_id = 0x" << std::hex << linux_devices[i].customer_id << ",\n";
					std::cout << "		.eco_id = " << std::dec << linux_devices[i].eco_id << ",\n";


					std::cout << "		.stream_count = " << std::dec << linux_devices[i].stream_count << ",\n";
					std::cout << "		.register_max = " << linux_devices[i].register_max << ",\n";
					std::cout << "		.thread_count = " << linux_devices[i].thread_count << ",\n";
					std::cout << "		.shader_core_count = " << linux_devices[i].shader_core_count << ",\n";
					std::cout << "		.vertex_cache_size = " << linux_devices[i].vertex_cache_size << ",\n";
					std::cout << "		.vertex_output_buffer_size = " << linux_devices[i].vertex_output_buffer_size << ",\n";
					std::cout << "		.pixel_pipes = " << linux_devices[i].pixel_pipes << ",\n";
					std::cout << "		.instruction_count = " << linux_devices[i].instruction_count << ",\n";
					std::cout << "		.num_constants = " << linux_devices[i].num_constants << ",\n";
					std::cout << "		.buffer_size = " << linux_devices[i].buffer_size << ",\n";
					std::cout << "		.varyings_count = " << linux_devices[i].varyings_count << ",\n";

					std::cout.fill('0');

					std::cout << "		.features = 0x" << std::hex << linux_devices[i].features << ",\n";
					std::cout << "		.minor_features0 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features0 << ",\n";
					std::cout << "		.minor_features1 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features1 << ",\n";
					std::cout << "		.minor_features2 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features2 << ",\n";
					std::cout << "		.minor_features3 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features3 << ",\n";
					std::cout << "		.minor_features4 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features4 << ",\n";
					std::cout << "		.minor_features5 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features5 << ",\n";
					std::cout << "		.minor_features6 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features6 << ",\n";
					std::cout << "		.minor_features7 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features7 << ",\n";
					std::cout << "		.minor_features8 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features8 << ",\n";
					std::cout << "		.minor_features9 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features9 << ",\n";
					std::cout << "		.minor_features10 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features10 << ",\n";
					std::cout << "		.minor_features11 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features11 << ",\n";
#if defined EXTENDED_FEATURE_LIST
					std::cout << "		.minor_features12 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features12 << ",\n";
					std::cout << "		.minor_features13 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features13 << ",\n";
#endif

					std::cout << "	},\n";


					// write output to features.txt
					myfile << "\t{\n";
					myfile << "\t\t.model = 0x" << std::hex << linux_devices[i].model << ",\n";
					myfile << "\t\t.revision = 0x" << std::hex << linux_devices[i].revision << ",\n";
					myfile << "\t\t.product_id = 0x" << std::hex << linux_devices[i].product_id << ",\n";
					myfile << "\t\t.customer_id = 0x" << std::hex << linux_devices[i].customer_id << ",\n";
					myfile << "\t\t.eco_id = " << std::dec << linux_devices[i].eco_id << ",\n";


					myfile << "\t\t.stream_count = " << std::dec << linux_devices[i].stream_count << ",\n";
					myfile << "\t\t.register_max = " << linux_devices[i].register_max << ",\n";
					myfile << "\t\t.thread_count = " << linux_devices[i].thread_count << ",\n";
					myfile << "\t\t.shader_core_count = " << linux_devices[i].shader_core_count << ",\n";
					myfile << "\t\t.vertex_cache_size = " << linux_devices[i].vertex_cache_size << ",\n";
					myfile << "\t\t.vertex_output_buffer_size = " << linux_devices[i].vertex_output_buffer_size << ",\n";
					myfile << "\t\t.pixel_pipes = " << linux_devices[i].pixel_pipes << ",\n";
					myfile << "\t\t.instruction_count = " << linux_devices[i].instruction_count << ",\n";
					myfile << "\t\t.num_constants = " << linux_devices[i].num_constants << ",\n";
					myfile << "\t\t.buffer_size = " << linux_devices[i].buffer_size << ",\n";
					myfile << "\t\t.varyings_count = " << linux_devices[i].varyings_count << ",\n";

					myfile.fill('0');

					myfile << "\t\t.features = 0x" << std::hex << std::setw(8) << linux_devices[i].features << ",\n";
					myfile << "\t\t.minor_features0 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features0 << ",\n";
					myfile << "\t\t.minor_features1 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features1 << ",\n";
					myfile << "\t\t.minor_features2 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features2 << ",\n";
					myfile << "\t\t.minor_features3 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features3 << ",\n";
					myfile << "\t\t.minor_features4 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features4 << ",\n";
					myfile << "\t\t.minor_features5 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features5 << ",\n";
					myfile << "\t\t.minor_features6 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features6 << ",\n";
					myfile << "\t\t.minor_features7 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features7 << ",\n";
					myfile << "\t\t.minor_features8 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features8 << ",\n";
					myfile << "\t\t.minor_features9 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features9 << ",\n";
					myfile << "\t\t.minor_features10 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features10 << ",\n";
					myfile << "\t\t.minor_features11 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features11 << ",\n";
#if defined EXTENDED_FEATURE_LIST
					myfile << "\t\t.minor_features12 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features12 << ",\n";
					myfile << "\t\t.minor_features13 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features13 << ",\n";
#endif
					myfile << "\t},\n";			
				}
			}
		}
	}

	myfile.close();

	std::cout << "\n";
	std::cout << std::dec << count << " Geraete gefunden \n" ;
	std::cout << "Zum Beenden eine Taste druecken...\n";
	getchar();	
}
