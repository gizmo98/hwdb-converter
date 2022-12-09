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
//#define NN_CORE_COUNT
//#define EXTENDED_FEATURE_LIST

#include <iostream>
#include <iomanip>
#include "common.xml.h"
#include "gc_feature_database.h"
#include "gc_feature_database_stm32.h"
#include "gc_feature_database_stm32_6_4_9.h"
#include "gc_feature_database_csky.h"
#include "gc_feature_database_amlogic_6_4_6_2.h"
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
	int nn_core_count;
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
	int minor_features14;
	int minor_features15;
	int minor_features16;
	int minor_features17;
	int minor_features18;
	int minor_features19;
	int minor_features20;
} linux_chips;

int main()
{
	const int device_count = (sizeof(gChipInfo) / sizeof(gChipInfo[0])) + (sizeof(gChipInfo_STM32_6_2_4) / sizeof(gChipInfo_STM32_6_2_4[0])) +(sizeof(gChipInfo_STM32_4_6_9) / sizeof(gChipInfo_STM32_4_6_9[0])) + (sizeof(gChipInfo_csky) / sizeof(gChipInfo_csky[0])) + (sizeof(gChipInfo_AMLOGIC) / sizeof(gChipInfo_AMLOGIC[0]));
	
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
				(gChipInfo[i].TX_FILTER_ROUND_FIX == 1 ? chipMinorFeatures13_TX_FILTER_ROUND_FIX : 0) |
				(gChipInfo[i].SH_FP32_FMA_SUPPORT == 1 ? chipMinorFeatures13_SH_FP32_FMA_SUPPORT : 0) |
				(gChipInfo[i].PE_64BPP_LINEAR_FORMAT == 1 ? chipMinorFeatures13_PE_64BPP_LINEAR_FORMAT : 0) |
				(gChipInfo[i].TX_ETC2_COMPRESSION == 1 ? chipMinorFeatures13_TX_ETC2_COMPRESSION : 0) |
				(gChipInfo[i].HIGHP_VEC2 == 1 ? chipMinorFeatures13_HIGHP_VEC2 : 0) |
				(gChipInfo[i].MMU_PD_42_BIT_ADDRESS == 1 ? chipMinorFeatures13_MMU_PD_42_BIT_ADDRESS : 0) |
				(gChipInfo[i].BLT_ROBUSTNESS_FIX == 1 ? chipMinorFeatures13_BLT_ROBUSTNESS_FIX : 0) |
				(gChipInfo[i].TFB_PERF_FIX == 1 ? chipMinorFeatures13_TFB_PERF_FIX : 0) |
				(gChipInfo[i].SH_SUPERSCALAR_ARCH == 1 ? chipMinorFeatures13_SH_SUPERSCALAR_ARCH : 0) | 00
				(gChipInfo[i].PA_ZEROAREA_LINE_FIX == 1 ? chipMinorFeatures13_PA_ZEROAREA_LINE_FIX : 0) |
				(gChipInfo[i].VG_IMAGE_16K == 1 ? chipMinorFeatures13_VG_IMAGE_16K : 0) |
				(gChipInfo[i].VG_FORMAT_ARGB2222 == 1 ? chipMinorFeatures13_VG_FORMAT_ARGB2222 : 0) |
				(gChipInfo[i].VIP_HW_FINAL_RELEASE == 1 ? chipMinorFeatures13_VIP_HW_FINAL_RELEASE : 0) |
				(gChipInfo[i].NN_SINGLEPORT_ACCUMBUFFER == 1 ? chipMinorFeatures13_NN_SINGLEPORT_ACCUMBUFFER : 0) |
				(gChipInfo[i].NN_STRIDE_SUPPORT == 1 ? chipMinorFeatures13_NN_STRIDE_SUPPORT : 0) |
				(gChipInfo[i].SWTILING_PHASE1 == 1 ? chipMinorFeatures13_SWTILING_PHASE1 : 0) |
				(gChipInfo[i].SWTILING_PHASE2 == 1 ? chipMinorFeatures13_SWTILING_PHASE2 : 0) |
				(gChipInfo[i].TP_SIMPLE_INT16 == 1 ? chipMinorFeatures13_TP_SIMPLE_INT16 : 0) |
				(gChipInfo[i].TP_REAL_INT16 == 1 ? chipMinorFeatures13_TP_REAL_INT16 : 0) |
				(gChipInfo[i].TP_ROI_POOLING == 1 ? chipMinorFeatures13_TP_ROI_POOLING : 0) |
				(gChipInfo[i].TP_MAX_POOLING_STRIDE1 == 1 ? chipMinorFeatures13_TP_MAX_POOLING_STRIDE1 : 0) |
				(gChipInfo[i].TP_LRN == 1 ? chipMinorFeatures13_TP_LRN : 0) |
				(gChipInfo[i].TF_QUANTIZATION == 1 ? chipMinorFeatures13_TF_QUANTIZATION : 0) |
				(gChipInfo[i].NN_NONZERO_BORDER == 1 ? chipMinorFeatures13_NN_NONZERO_BORDER : 0) |
				(gChipInfo[i].NN_MIRROR_BORDER == 1 ? chipMinorFeatures13_NN_MIRROR_BORDER : 0) |
				(gChipInfo[i].AI_GPU == 1 ? chipMinorFeatures13_AI_GPU : 0);

			uint32_t features14 =
				(gChipInfo[i].TP_TENSOR_ADD_MUL == 1 ? chipMinorFeatures13_TP_TENSOR_ADD_MUL : 0) |
				(gChipInfo[i].NN_DEPTHWISE_INT16XINT8 == 1 ? chipMinorFeatures13_NN_DEPTHWISE_INT16XINT8 : 0) |
				(gChipInfo[i].NN_DEPTHWISE_8BIT_VIP_V7 == 1 ? chipMinorFeatures14_NN_DEPTHWISE_8BIT_VIP_V7 : 0) |
				(gChipInfo[i].TP_SOFTMAX == 1 ? chipMinorFeatures14_TP_SOFTMAX : 0) |
				(gChipInfo[i].NN_23BITS_POST_MULTIPLIER_VIP_V7 == 1 ? chipMinorFeatures14_NN_23BITS_POST_MULTIPLIER_VIP_V7 : 0) |
				(gChipInfo[i].TP_23BITS_POST_MULTIPLIER_VIP_V7 == 1 ? chipMinorFeatures14_TP_23BITS_POST_MULTIPLIER_VIP_V7 : 0) |
				(gChipInfo[i].CONV_INT16X8BIT_VIP_V7 == 1 ? chipMinorFeatures14_CONV_INT16X8BIT_VIP_V7 : 0) |
				(gChipInfo[i].NN_REMOVE_POOLING == 1 ? chipMinorFeatures14_NN_REMOVE_POOLING : 0) |
				(gChipInfo[i].NN_40BIT_BIAS == 1 ? chipMinorFeatures14_NN_40BIT_BIAS : 0) |
				(gChipInfo[i].TP_REMOVE_USC == 1 ? chipMinorFeatures14_TP_REMOVE_USC : 0) |
				(gChipInfo[i].NN_ZDP6 == 1 ? chipMinorFeatures14_NN_ZDP6 : 0) |
				(gChipInfo[i].NN_XYDP9 == 1 ? chipMinorFeatures14_NN_XYDP9 : 0) |
				(gChipInfo[i].NN_FIRST_PIXEL_POOLING == 1 ? chipMinorFeatures14_NN_FIRST_PIXEL_POOLING : 0) |
				(gChipInfo[i].NN_ZDP3 == 1 ? chipMinorFeatures14_NN_ZDP3 : 0) |
				(gChipInfo[i].NN_XYDP6 == 1 ? chipMinorFeatures14_NN_XYDP6 : 0) |
				(gChipInfo[i].SWTILING_PHASE3 == 1 ? chipMinorFeatures14_SWTILING_PHASE3 : 0) |
				(gChipInfo[i].USC_STAY_LRU == 1 ? chipMinorFeatures14_USC_STAY_LRU : 0) |
				(gChipInfo[i].COEF_COMPRESSION_ENHANCEMENT == 1 ? chipMinorFeatures14_COEF_COMPRESSION_ENHANCEMENT : 0) |
				(gChipInfo[i].TP_COEF_COMPRESSION_ENHANCEMENT == 1 ? chipMinorFeatures14_TP_COEF_COMPRESSION_ENHANCEMENT : 0) |
				(gChipInfo[i].NN_COEF_DECOMPRESS_PERF2X == 1 ? chipMinorFeatures14_NN_COEF_DECOMPRESS_PERF2X : 0) |
				(gChipInfo[i].TP_SMALLBATCH_PHASE1 == 1 ? chipMinorFeatures14_TP_SMALLBATCH_PHASE1 : 0) |
				(gChipInfo[i].OCB_COUNTER == 1 ? chipMinorFeatures14_OCB_COUNTER : 0) |
				(gChipInfo[i].SCALER == 1 ? chipMinorFeatures14_SCALER : 0) |
				(gChipInfo[i].SCALER_4K == 1 ? chipMinorFeatures14_SCALER_4K : 0) |
				(gChipInfo[i].INPUT_4BIT == 1 ? chipMinorFeatures14_INPUT_4BIT : 0) |
				(gChipInfo[i].NN_NO_Z_LOCATION_OFFSET == 1 ? chipMinorFeatures14_NN_NO_Z_LOCATION_OFFSET : 0) |
				(gChipInfo[i].OCB_REMAP_PHYSICAL_ADDRESS == 1 ? chipMinorFeatures14_OCB_REMAP_PHYSICAL_ADDRESS : 0) |
				(gChipInfo[i].NN_SLOW_OUTPUT == 1 ? chipMinorFeatures14_NN_SLOW_OUTPUT : 0) |
				(gChipInfo[i].NO_NARROW_POST_PROCESS_PIPE == 1 ? chipMinorFeatures14_NO_NARROW_POST_PROCESS_PIPE : 0) |
				(gChipInfo[i].TP_NN_PROBE == 1 ? chipMinorFeatures14_TP_NN_PROBE : 0) |
				(gChipInfo[i].NN_DEPTHWISE_SUPPORT == 1 ? chipMinorFeatures14_NN_DEPTHWISE_SUPPORT : 0) |
				(gChipInfo[i].NN_XYDP0 == 1 ? chipMinorFeatures14_NN_XYDP0 : 0) |
				(gChipInfo[i].NN_WRITE_WITHOUT_USC == 1 ? chipMinorFeatures14_NN_WRITE_WITHOUT_USC : 0) |
				(gChipInfo[i].NN_HW_LIMITATION_NATIVE_KER_1x2_2x1 == 1 ? chipMinorFeatures14_NN_HW_LIMITATION_NATIVE_KER_1x2_2x1 : 0);
			uint32_t features15 =
				(gChipInfo[i].NN_SMALLBATCH_PHASE1 == 1 ? chipMinorFeatures15_NN_SMALLBATCH_PHASE1 : 0) |
				(gChipInfo[i].NN_SLICE_PADDING_TO_64BYTE_ALIGN == 1 ? chipMinorFeatures15_NN_SLICE_PADDING_TO_64BYTE_ALIGN : 0) |
				(gChipInfo[i].NN_DW_1x1_CONV_MERGE == 1 ? chipMinorFeatures15_NN_DW_1x1_CONV_MERGE : 0) |
				(gChipInfo[i].TP_BFLOAT16 == 1 ? chipMinorFeatures15_TP_BFLOAT16 : 0) |
				(gChipInfo[i].TP_23BITS_POST_MULTIPLIER == 1 ? chipMinorFeatures15_TP_23BITS_POST_MULTIPLIER : 0) |
				(gChipInfo[i].NN_TRANSPOSE == 1 ? chipMinorFeatures15_NN_TRANSPOSE : 0) |
				(gChipInfo[i].NN_ZDP_TRANSPOSE_CH9_ONLY == 1 ? chipMinorFeatures15_NN_ZDP_TRANSPOSE_CH9_ONLY : 0) |
				(gChipInfo[i].USE_SINGLE_PORT_VIPSRAM == 1 ? chipMinorFeatures15_USE_SINGLE_PORT_VIPSRAM : 0) |
				(gChipInfo[i].NN_LEAKY_RELU == 1 ? chipMinorFeatures15_NN_LEAKY_RELU : 0) |
				(gChipInfo[i].NN_PRELU == 1 ? chipMinorFeatures15_NN_PRELU : 0) |
				(gChipInfo[i].NN_PER_CHANNEL_QUANT == 1 ? chipMinorFeatures15_NN_PER_CHANNEL_QUANT : 0) |
				(gChipInfo[i].NN_PER_CHANNEL_QUANT_ASYM == 1 ? chipMinorFeatures15_NN_PER_CHANNEL_QUANT_ASYM : 0) |
				(gChipInfo[i].NN_ASYMMETRIC_INT8 == 1 ? chipMinorFeatures15_NN_ASYMMETRIC_INT8 : 0) |
				(gChipInfo[i].NN_FLOAT_POST_MULT == 1 ? chipMinorFeatures15_NN_FLOAT_POST_MULT : 0) |
				(gChipInfo[i].PRELU_LEAKLY_RELU_CLAMP == 1 ? chipMinorFeatures15_PRELU_LEAKLY_RELU_CLAMP : 0) |
				(gChipInfo[i].TPLITE_BFLOAT16 == 1 ? chipMinorFeatures15_TPLITE_BFLOAT16 : 0) |
				(gChipInfo[i].PREPROCESS_IMG_BUF_640BYTE_LIMIT == 1 ? chipMinorFeatures15_PREPROCESS_IMG_BUF_640BYTE_LIMIT : 0) |
				(gChipInfo[i].NN_POST_OUT_SUPPORT_FP16 == 1 ? chipMinorFeatures15_NN_POST_OUT_SUPPORT_FP16 : 0) |
				(gChipInfo[i].NN_POST_OUT_SUPPORT_BF16 == 1 ? chipMinorFeatures15_NN_POST_OUT_SUPPORT_BF16 : 0) |
				(gChipInfo[i].NN_POST_OUT_SUPPORT_FP32 == 1 ? chipMinorFeatures15_NN_POST_OUT_SUPPORT_FP32 : 0) |
				(gChipInfo[i].TP_KERNEL_1BYTE_ALGIN == 1 ? chipMinorFeatures15_TP_KERNEL_1BYTE_ALGIN : 0) |
				(gChipInfo[i].BFLOAT_COEF_COMPRESSION_ZERO_COEFBIT14_INVERSE == 1 ? chipMinorFeatures15_BFLOAT_COEF_COMPRESSION_ZERO_COEFBIT14_INVERSE : 0) |
				(gChipInfo[i].NN_COMPRESSION_BYPASSS == 1 ? chipMinorFeatures15_NN_COMPRESSION_BYPASSS : 0) |
				(gChipInfo[i].TP_3_USC == 1 ? chipMinorFeatures15_TP_3_USC : 0) |
				(gChipInfo[i].BFP_COEF_AUTO_PAD_INCOMPLETE_ZERO_IN_KZ_PLANE == 1 ? chipMinorFeatures15_BFP_COEF_AUTO_PAD_INCOMPLETE_ZERO_IN_KZ_PLANE : 0) |
				(gChipInfo[i].NN_NATIVE_STRIDE_TWO == 1 ? chipMinorFeatures15_NN_NATIVE_STRIDE_TWO : 0) |
				(gChipInfo[i].NN_TENSOR_ADD == 1 ? chipMinorFeatures15_NN_TENSOR_ADD : 0) |
				(gChipInfo[i].TP_FLOAT32_IO == 1 ? chipMinorFeatures15_TP_FLOAT32_IO : 0) |
				(gChipInfo[i].NN_SMALL_BATCH_PHASE2 == 1 ? chipMinorFeatures15_NN_SMALL_BATCH_PHASE2 : 0) |
				(gChipInfo[i].TILE_ACCESS_CAPABILITY == 1 ? chipMinorFeatures15_TILE_ACCESS_CAPABILITY : 0) |
				(gChipInfo[i].FAST_DP3_PREPROCESSOR == 1 ? chipMinorFeatures15_FAST_DP3_PREPROCESSOR : 0) |
				(gChipInfo[i].DEPTHWISE_SUPPORT_16BIT_FORMAT == 1 ? chipMinorFeatures15_DEPTHWISE_SUPPORT_16BIT_FORMAT : 0);
			uint32_t features16 =
				(gChipInfo[i].NN_SUPPORT_ALU == 1 ? chipMinorFeatures16_NN_SUPPORT_ALU : 0) |
				(gChipInfo[i].NN_ENHANCED_MAX_POOLING == 1 ? chipMinorFeatures16_NN_ENHANCED_MAX_POOLING : 0) |
				(gChipInfo[i].NN_TRANSPOSE_PHASE2 == 1 ? chipMinorFeatures16_NN_TRANSPOSE_PHASE2 : 0) |
				(gChipInfo[i].NN_TENSOR_ADD_FIELD_MOVE_TO_EXT_CMD == 1 ? chipMinorFeatures16_NN_TENSOR_ADD_FIELD_MOVE_TO_EXT_CMD : 0) |
				(gChipInfo[i].NN_CMD_SUPPORT_SLICE == 1 ? chipMinorFeatures16_NN_CMD_SUPPORT_SLICE : 0) |
				(gChipInfo[i].NN_CONV_CORE_BYPASS == 1 ? chipMinorFeatures16_NN_CONV_CORE_BYPASS : 0) |
				(gChipInfo[i].NN_TENSOR_ADD_RELU == 1 ? chipMinorFeatures16_NN_TENSOR_ADD_RELU : 0) |
				(gChipInfo[i].TPLITE_SUPPORT_TP_DATA_TRANSPOSE == 1 ? chipMinorFeatures16_TPLITE_SUPPORT_TP_DATA_TRANSPOSE : 0) |
				(gChipInfo[i].NN_SUPPORT_CONV_1D == 1 ? chipMinorFeatures16_NN_SUPPORT_CONV_1D : 0) |
				(gChipInfo[i].USE_VIPSRAM_FOR_KERNEL_STREAMING == 1 ? chipMinorFeatures16_USE_VIPSRAM_FOR_KERNEL_STREAMING : 0) |
				(gChipInfo[i].NN_SUPPORT_DUMMY_TILE == 1 ? chipMinorFeatures16_NN_SUPPORT_DUMMY_TILE : 0) |
				(gChipInfo[i].NN_SUPPORT_KERNEL_1BYTE_ALIGN == 1 ? chipMinorFeatures16_NN_SUPPORT_KERNEL_1BYTE_ALIGN : 0) |
				(gChipInfo[i].NN_1x1_NON_POOLING_PACKING == 1 ? chipMinorFeatures16_NN_1x1_NON_POOLING_PACKING : 0) |
				(gChipInfo[i].NN_SUPPORT_BOTH_CONV_NATIVE_STRIDE2_AND_POOLING == 1 ? chipMinorFeatures16_NN_SUPPORT_BOTH_CONV_NATIVE_STRIDE2_AND_POOLING : 0) |
				(gChipInfo[i].NN_SUPPORT_CONV1x1_AND_NATIVE_CONV_STRIDE2 == 1 ? chipMinorFeatures16_NN_SUPPORT_CONV1x1_AND_NATIVE_CONV_STRIDE2 : 0) |
				(gChipInfo[i].TP_REMOVE_FC == 1 ? chipMinorFeatures16_TP_REMOVE_FC : 0) |
				(gChipInfo[i].VIP_REMOVE_MMU == 1 ? chipMinorFeatures16_VIP_REMOVE_MMU : 0) |
				(gChipInfo[i].NN_RD_IMG_NEED_EXTRA_SPACE == 1 ? chipMinorFeatures16_NN_RD_IMG_NEED_EXTRA_SPACE : 0) |
				(gChipInfo[i].VIP_INDIV_CLK_NN == 1 ? chipMinorFeatures16_VIP_INDIV_CLK_NN : 0) |
				(gChipInfo[i].VIP_EXPORT_CLK_DIV2 == 1 ? chipMinorFeatures16_VIP_EXPORT_CLK_DIV2 : 0) |
				(gChipInfo[i].NN_2D_AVERAGE_OUTPUT == 1 ? chipMinorFeatures16_NN_2D_AVERAGE_OUTPUT : 0) |
				(gChipInfo[i].NN_JOB_CANCELATION == 1 ? chipMinorFeatures16_NN_JOB_CANCELATION : 0) |
				(gChipInfo[i].NN_SUPPORT_GEMM == 1 ? chipMinorFeatures16_NN_SUPPORT_GEMM : 0) |
				(gChipInfo[i].NN_SUPPORT_INLINE_NWHC_AND_MATRIX_TRANSPOSE == 1 ? chipMinorFeatures16_NN_SUPPORT_INLINE_NWHC_AND_MATRIX_TRANSPOSE : 0) |
				(gChipInfo[i].NN_SUPPORT_BATCH == 1 ? chipMinorFeatures16_NN_SUPPORT_BATCH : 0) |
				(gChipInfo[i].VIP_SUPPORT_DEC == 1 ? chipMinorFeatures16_VIP_SUPPORT_DEC : 0) |
				(gChipInfo[i].NN_SUPPORT_MULTI_AXI_ID == 1 ? chipMinorFeatures16_NN_SUPPORT_MULTI_AXI_ID : 0) |
				(gChipInfo[i].NN_POST_OUT_SUPPORT_INT32 == 1 ? chipMinorFeatures16_NN_POST_OUT_SUPPORT_INT32 : 0) |
				(gChipInfo[i].NN_SUPPORT_STREAMPROCESSOR == 1 ? chipMinorFeatures16_NN_SUPPORT_STREAMPROCESSOR : 0) |
				(gChipInfo[i].NN_DISTRIBUTED_VIPSRAM == 1 ? chipMinorFeatures16_NN_DISTRIBUTED_VIPSRAM : 0) |
				(gChipInfo[i].NN_FC_ENHANCEMENT == 1 ? chipMinorFeatures16_NN_FC_ENHANCEMENT : 0) |
				(gChipInfo[i].NN_4BIT_PHASE1 == 1 ? chipMinorFeatures16_NN_4BIT_PHASE1 : 0);

			uint32_t features17 =
				(gChipInfo[i].VIP_DEC400 == 1 ? chipMinorFeatures17_VIP_DEC400 : 0) |
				(gChipInfo[i].NN_POST_MULT_SUPPORT_FP_CONV == 1 ? chipMinorFeatures17_NN_POST_MULT_SUPPORT_FP_CONV : 0) |
				(gChipInfo[i].NN_SUPPORT_16_8_QUANTIZATION == 1 ? chipMinorFeatures17_NN_SUPPORT_16_8_QUANTIZATION : 0) |
				(gChipInfo[i].SPECIAL_8BIT_SIGN_ABS_CONV == 1 ? chipMinorFeatures17_SPECIAL_8BIT_SIGN_ABS_CONV : 0) |
				(gChipInfo[i].NN_SUPPORT_CONFIGURABLE_FASTXDP3 == 1 ? chipMinorFeatures17_NN_SUPPORT_CONFIGURABLE_FASTXDP3 : 0) |
				(gChipInfo[i].NN_USE_CORE_SHARING_IMGBUF_AND_SEQ_NO_ZEROSKIPPING == 1 ? chipMinorFeatures17_NN_USE_CORE_SHARING_IMGBUF_AND_SEQ_NO_ZEROSKIPPING : 0) |
				(gChipInfo[i].SUPPORT_DECONVNxN_S_LESS_THAN_16 == 1 ? chipMinorFeatures17_SUPPORT_DECONVNxN_S_LESS_THAN_16 : 0) |
				(gChipInfo[i].NN_PICOCORE_DEPTHWISE == 1 ? chipMinorFeatures17_NN_PICOCORE_DEPTHWISE : 0) |
				(gChipInfo[i].VIP_SUPPORT_TENSOR_TRANSFER == 1 ? chipMinorFeatures17_VIP_SUPPORT_TENSOR_TRANSFER : 0) |
				(gChipInfo[i].NN_SUPPORT_CMD_LOOP == 1 ? chipMinorFeatures17_NN_SUPPORT_CMD_LOOP : 0) |
				(gChipInfo[i].VIP_SUPPORT_X_FRAME_COMPRESSION == 1 ? chipMinorFeatures17_VIP_SUPPORT_X_FRAME_COMPRESSION : 0) |
				(gChipInfo[i].NN_SMALL_ACCUM == 1 ? chipMinorFeatures17_NN_SMALL_ACCUM : 0) |
				(gChipInfo[i].NN_SINGLE_POSTMULT_FIELDS_IN_BITSTREAM == 1 ? chipMinorFeatures17_NN_SINGLE_POSTMULT_FIELDS_IN_BITSTREAM : 0) |
				(gChipInfo[i].POST_MULTIPLIER_LOW_POWER_MODE == 1 ? chipMinorFeatures17_POST_MULTIPLIER_LOW_POWER_MODE : 0) |
				(gChipInfo[i].NN_PER3DTILE_BUBBLE_FIX == 1 ? chipMinorFeatures17_NN_PER3DTILE_BUBBLE_FIX : 0) |
				(gChipInfo[i].NN_CACHELINE_MODE_PERF_FIX == 1 ? chipMinorFeatures17_NN_CACHELINE_MODE_PERF_FIX : 0) |
				(gChipInfo[i].NN_CONV1x1_PERF_FIX == 1 ? chipMinorFeatures17_NN_CONV1x1_PERF_FIX : 0) |
				(gChipInfo[i].TP_REORDER_FIX == 1 ? chipMinorFeatures17_TP_REORDER_FIX : 0) |
				(gChipInfo[i].NN_CONVOUT_FIFO_DEPTH_FIX == 1 ? chipMinorFeatures17_NN_CONVOUT_FIFO_DEPTH_FIX : 0) |
				(gChipInfo[i].NN_ZXDP3_KERNEL_READ_CONFLICT_FIX == 1 ? chipMinorFeatures17_NN_ZXDP3_KERNEL_READ_CONFLICT_FIX : 0) |
				(gChipInfo[i].NN_ZDP3_NO_COMPRESS_FIX == 1 ? chipMinorFeatures17_NN_ZDP3_NO_COMPRESS_FIX : 0) |
				(gChipInfo[i].NN_ASYNC_COPY_PERF_FIX == 1 ? chipMinorFeatures17_NN_ASYNC_COPY_PERF_FIX : 0) |
				(gChipInfo[i].HI_REORDER_FIX == 1 ? chipMinorFeatures17_HI_REORDER_FIX : 0) |
				(gChipInfo[i].INCORRECT_WR_REQ_TO_USC_BETWEEN_REORDER_AND_NORMAL_LAYER_FIX == 1 ? chipMinorFeatures17_INCORRECT_WR_REQ_TO_USC_BETWEEN_REORDER_AND_NORMAL_LAYER_FIX : 0) |
				(gChipInfo[i].TP_REORDER_LAYER_SUSPEND_FIX == 1 ? chipMinorFeatures17_TP_REORDER_LAYER_SUSPEND_FIX : 0) |
				(gChipInfo[i].NN_ASYNC_COPY_MERGE_FIX == 1 ? chipMinorFeatures17_NN_ASYNC_COPY_MERGE_FIX : 0) |
				(gChipInfo[i].USC_INVALIDATE_CACHE_LINE_FIX == 1 ? chipMinorFeatures17_USC_INVALIDATE_CACHE_LINE_FIX : 0) |
				(gChipInfo[i].NN_REQ_SLOWARBITRATION_FIX == 1 ? chipMinorFeatures17_NN_REQ_SLOWARBITRATION_FIX : 0) |
				(gChipInfo[i].IMAGE_PARTIAL_CACHE_FIX == 1 ? chipMinorFeatures17_IMAGE_PARTIAL_CACHE_FIX : 0) |
				(gChipInfo[i].FULLCACHE_KERNELHEAD_FIX == 1 ? chipMinorFeatures17_FULLCACHE_KERNELHEAD_FIX : 0) |
				(gChipInfo[i].NN_ZDP_INIMAGE_SIZE_FIX == 1 ? chipMinorFeatures17_NN_ZDP_INIMAGE_SIZE_FIX : 0) |
				(gChipInfo[i].IDLE_BEFORE_FLUSH_COMPLETE_FIX == 1 ? chipMinorFeatures17_IDLE_BEFORE_FLUSH_COMPLETE_FIX : 0);

			uint32_t features18 =
				(gChipInfo[i].NO_FLUSH_USC_FIX == 1 ? chipMinorFeatures18_NO_FLUSH_USC_FIX : 0) |
				(gChipInfo[i].SMALL_BATCH_FLOPS_RESET_FIX == 1 ? chipMinorFeatures18_SMALL_BATCH_FLOPS_RESET_FIX : 0) |
				(gChipInfo[i].SMALL_BATCH_DISBLE_FIX == 1 ? chipMinorFeatures18_SMALL_BATCH_DISBLE_FIX : 0) |
				(gChipInfo[i].OUTPUT_CONVERT_UINT8_INT8_TO_UINT16_INT16_FIX == 1 ? chipMinorFeatures18_OUTPUT_CONVERT_UINT8_INT8_TO_UINT16_INT16_FIX : 0) |
				(gChipInfo[i].IMAGE_NOT_PACKED_IN_SRAM_FIX == 1 ? chipMinorFeatures18_IMAGE_NOT_PACKED_IN_SRAM_FIX : 0) |
				(gChipInfo[i].COEF_DELTA_CORD_OVERFLOW_ZRL_8BIT_FIX == 1 ? chipMinorFeatures18_COEF_DELTA_CORD_OVERFLOW_ZRL_8BIT_FIX : 0) |
				(gChipInfo[i].USC_INDIVIDUAL_PORT_WRT_EARLY_EVICT_DATA_CORRUPT_FIX == 1 ? chipMinorFeatures18_USC_INDIVIDUAL_PORT_WRT_EARLY_EVICT_DATA_CORRUPT_FIX : 0) |
				(gChipInfo[i].LOW_EFFICIENCY_OF_ID_WRITE_IMGBUF_FIX == 1 ? chipMinorFeatures18_LOW_EFFICIENCY_OF_ID_WRITE_IMGBUF_FIX : 0) |
				(gChipInfo[i].KERNEL_VIP_SRAM_READ_BW_LIMITATION_FIX == 1 ? chipMinorFeatures18_KERNEL_VIP_SRAM_READ_BW_LIMITATION_FIX : 0) |
				(gChipInfo[i].USC_BOTTLENECK_FIX == 1 ? chipMinorFeatures18_USC_BOTTLENECK_FIX : 0) |
				(gChipInfo[i].KERNEL_PER_CORE_LESS_THAN_THIRD_COEF_BUFF_DEPTH_FIX == 1 ? chipMinorFeatures18_KERNEL_PER_CORE_LESS_THAN_THIRD_COEF_BUFF_DEPTH_FIX : 0) |
				(gChipInfo[i].NN_TILE_NUM_BIGGER_THAN_1024_FIX == 1 ? chipMinorFeatures18_NN_TILE_NUM_BIGGER_THAN_1024_FIX : 0) |
				(gChipInfo[i].KERNEL_SIZE_WASTE_IN_PARTIAL_MODE_FIX == 1 ? chipMinorFeatures18_KERNEL_SIZE_WASTE_IN_PARTIAL_MODE_FIX : 0) |
				(gChipInfo[i].NN_COMMAND_KERNEL_REQUEST_CONFICT_FIX == 1 ? chipMinorFeatures18_NN_COMMAND_KERNEL_REQUEST_CONFICT_FIX : 0) |
				(gChipInfo[i].TP_REORDER_INTILE_X_SIZE_512_FIX == 1 ? chipMinorFeatures18_TP_REORDER_INTILE_X_SIZE_512_FIX : 0) |
				(gChipInfo[i].IMG_POP_PIPELINE_PAUSE_FIX == 1 ? chipMinorFeatures18_IMG_POP_PIPELINE_PAUSE_FIX : 0) |
				(gChipInfo[i].FULLCACHE_KERNEL_INTERLEAVE_FIX == 1 ? chipMinorFeatures18_FULLCACHE_KERNEL_INTERLEAVE_FIX : 0) |
				(gChipInfo[i].V8_SINGLE_PORT_ACCUMULATION_BUFFER_RW_CONFICT_ZERO_SKIP_PERF_FIX == 1 ? chipMinorFeatures18_V8_SINGLE_PORT_ACCUMULATION_BUFFER_RW_CONFICT_ZERO_SKIP_PERF_FIX : 0) |
				(gChipInfo[i].V8_ACCUMLATION_READ_OUT_HAS_BUBBLES_PERF_FIX == 1 ? chipMinorFeatures18_V8_ACCUMLATION_READ_OUT_HAS_BUBBLES_PERF_FIX : 0) |
				(gChipInfo[i].DEPTHWISE_NEIGHBOR_IMG_DATA_TRANSFER_NOT_EFFICIENT_FIX == 1 ? chipMinorFeatures18_DEPTHWISE_NEIGHBOR_IMG_DATA_TRANSFER_NOT_EFFICIENT_FIX : 0) |
				(gChipInfo[i].DR_JD_DIFF_CONDITION_FOR_CACHELINE_MODE_PRE_FIX == 1 ? chipMinorFeatures18_DR_JD_DIFF_CONDITION_FOR_CACHELINE_MODE_PRE_FIX : 0) |
				(gChipInfo[i].TP_ACCESS_VIPSRAM_OT_IS_ONE_FIX == 1 ? chipMinorFeatures18_TP_ACCESS_VIPSRAM_OT_IS_ONE_FIX : 0) |
				(gChipInfo[i].EVIS2_FLOP_RESET_FIX == 1 ? chipMinorFeatures18_EVIS2_FLOP_RESET_FIX : 0) |
				(gChipInfo[i].OUTIMAGE_X_BITWIDTH_LIMIT_FOR_NN_TRANSPOSE_FIX == 1 ? chipMinorFeatures18_OUTIMAGE_X_BITWIDTH_LIMIT_FOR_NN_TRANSPOSE_FIX : 0) |
				(gChipInfo[i].USC_ASYNC_CP_RTN_FLOP_RESET_FIX == 1 ? chipMinorFeatures18_USC_ASYNC_CP_RTN_FLOP_RESET_FIX : 0) |
				(gChipInfo[i].IMG_ADDR_NOT_WRAP_IF_OVER_OCB_ADDR_FIX == 1 ? chipMinorFeatures18_IMG_ADDR_NOT_WRAP_IF_OVER_OCB_ADDR_FIX : 0) |
				(gChipInfo[i].NEGATIVE_POST_SHIFT_FIX == 1 ? chipMinorFeatures18_NEGATIVE_POST_SHIFT_FIX : 0) |
				(gChipInfo[i].INIMAGE_2DTILE_NOT_LESS_160PIXEL_FIX == 1 ? chipMinorFeatures18_INIMAGE_2DTILE_NOT_LESS_160PIXEL_FIX : 0) |
				(gChipInfo[i].IMG_CAHCE_MODE_MUST_0_IN_IMG_DIRECT_MODE_FIX == 1 ? chipMinorFeatures18_IMG_CAHCE_MODE_MUST_0_IN_IMG_DIRECT_MODE_FIX : 0) |
				(gChipInfo[i].BURST_COLLECT_DUMMY_DATA_WASTE_CYCLES_FIX == 1 ? chipMinorFeatures18_BURST_COLLECT_DUMMY_DATA_WASTE_CYCLES_FIX : 0) |
				(gChipInfo[i].INIMG_NOT_64BYTE_ALIGN_CACHELINE_MODE_FIX == 1 ? chipMinorFeatures18_INIMG_NOT_64BYTE_ALIGN_CACHELINE_MODE_FIX : 0) |
				(gChipInfo[i].TP_FC_FLOAT_LAST_PIXEL_NEGATIVE_0_FIX == 1 ? chipMinorFeatures18_TP_FC_FLOAT_LAST_PIXEL_NEGATIVE_0_FIX : 0);

			uint32_t features19 =
				(gChipInfo[i].NN_WASTE_COEF_READ_WRITE_BANDWIDTH_128BYTE_VIPSRAM_IN_FULL_PATIAL_CACHE_MODE_FIX == 1 ? chipMinorFeatures19_NN_WASTE_COEF_READ_WRITE_BANDWIDTH_128BYTE_VIPSRAM_IN_FULL_PATIAL_CACHE_MODE_FIX : 0) |
				(gChipInfo[i].NN_IN_TILE_DATA_IS_ALL_PAD_FIX == 1 ? chipMinorFeatures19_NN_IN_TILE_DATA_IS_ALL_PAD_FIX : 0) |
				(gChipInfo[i].NN_TP_INSTR_COMPLETE_IN_SAME_CYCLE_WITH_WAIT_EVENT_FIX == 1 ? chipMinorFeatures19_NN_TP_INSTR_COMPLETE_IN_SAME_CYCLE_WITH_WAIT_EVENT_FIX : 0) |
				(gChipInfo[i].CORE_IMAGE_TRANSER_NOT_EFFICIENT_BETWEEN_PARTITION_FIX == 1 ? chipMinorFeatures19_CORE_IMAGE_TRANSER_NOT_EFFICIENT_BETWEEN_PARTITION_FIX : 0) |
				(gChipInfo[i].TP_FC_KERNEL_STREAM_MUST_LESS_THAN_OR_EQUAL_TO_64BYTE_WHEN_1BYTE_ALGINE_FIX == 1 ? chipMinorFeatures19_TP_FC_KERNEL_STREAM_MUST_LESS_THAN_OR_EQUAL_TO_64BYTE_WHEN_1BYTE_ALGINE_FIX : 0) |
				(gChipInfo[i].NN_KERNEL_1x1_NO_PAD_FIX == 1 ? chipMinorFeatures19_NN_KERNEL_1x1_NO_PAD_FIX : 0) |
				(gChipInfo[i].NN_DEPTHWISE_AFTER_16BIT_LAYER_LIMIT_FIX == 1 ? chipMinorFeatures19_NN_DEPTHWISE_AFTER_16BIT_LAYER_LIMIT_FIX : 0) |
				(gChipInfo[i].TP_NOT_FULL_USE_CACHE_LINE_FIX == 1 ? chipMinorFeatures19_TP_NOT_FULL_USE_CACHE_LINE_FIX : 0) |
				(gChipInfo[i].SH_MOVAI_MOVAR_UNUSED_COMPONENTS_WRITE_DIRTY_DATA_FIX == 1 ? chipMinorFeatures19_SH_MOVAI_MOVAR_UNUSED_COMPONENTS_WRITE_DIRTY_DATA_FIX : 0) |
				(gChipInfo[i].BURST_COLLECT_CONSUMES_MC_DATA_WIDTH_PER_CYCLE_FIX == 1 ? chipMinorFeatures19_BURST_COLLECT_CONSUMES_MC_DATA_WIDTH_PER_CYCLE_FIX : 0) |
				(gChipInfo[i].TP_ASSYM_INT8_FIX == 1 ? chipMinorFeatures19_TP_ASSYM_INT8_FIX : 0) |
				(gChipInfo[i].NN_PAD_SLICE_ERROR_WHEN_TRANSPSE_FIX == 1 ? chipMinorFeatures19_NN_PAD_SLICE_ERROR_WHEN_TRANSPSE_FIX : 0) |
				(gChipInfo[i].NN_2ND_IMG_BASE_ADDR_FIX == 1 ? chipMinorFeatures19_NN_2ND_IMG_BASE_ADDR_FIX : 0) |
				(gChipInfo[i].NN_TP_SYSTEM_FIX == 1 ? chipMinorFeatures19_NN_TP_SYSTEM_FIX : 0) |
				(gChipInfo[i].NN_INTILE_YSIZE_128_LIMIT_FIX == 1 ? chipMinorFeatures19_NN_INTILE_YSIZE_128_LIMIT_FIX : 0) |
				(gChipInfo[i].SH_CLOCK_GATOR_IDLE_CONDITON_FIX == 1 ? chipMinorFeatures19_SH_CLOCK_GATOR_IDLE_CONDITON_FIX : 0) |
				(gChipInfo[i].NN_BURST_COLLECTER_LAST_FLAG_FIX == 1 ? chipMinorFeatures19_NN_BURST_COLLECTER_LAST_FLAG_FIX : 0) |
				(gChipInfo[i].V83_CONVERTER_FOR_NEG_0_FIX == 1 ? chipMinorFeatures19_V83_CONVERTER_FOR_NEG_0_FIX : 0) |
				(gChipInfo[i].NN_2ND_IMG_SMALL_3D_TILE_FIX == 1 ? chipMinorFeatures19_NN_2ND_IMG_SMALL_3D_TILE_FIX : 0) |
				(gChipInfo[i].NN_TILE_YSIZE_127_LIMITATION_FIX == 1 ? chipMinorFeatures19_NN_TILE_YSIZE_127_LIMITATION_FIX : 0) |
				(gChipInfo[i].NN_CONV_1D_16BIT_FORMAT_INTILE_SIZE_LIMITATION_FIX == 1 ? chipMinorFeatures19_NN_CONV_1D_16BIT_FORMAT_INTILE_SIZE_LIMITATION_FIX : 0) |
				(gChipInfo[i].NN_VIPSRAM_DOUBLE_BUFFER_FIX == 1 ? chipMinorFeatures19_NN_VIPSRAM_DOUBLE_BUFFER_FIX : 0) |
				(gChipInfo[i].NN_JD_DIRECT_MODE_FIX == 1 ? chipMinorFeatures19_NN_JD_DIRECT_MODE_FIX : 0) |
				(gChipInfo[i].NN_KERNEL_DIRECT_WRONG_PUSH_FIX == 1 ? chipMinorFeatures19_NN_KERNEL_DIRECT_WRONG_PUSH_FIX : 0) |
				(gChipInfo[i].HI_DEFAULT_ENABLE_REORDER_FIX == 1 ? chipMinorFeatures19_HI_DEFAULT_ENABLE_REORDER_FIX : 0) |
				(gChipInfo[i].V8_DIRECT_MODE_START_ADDR_BIAS_FOR_NEGATIVE_OFFSET_FIX == 1 ? chipMinorFeatures19_V8_DIRECT_MODE_START_ADDR_BIAS_FOR_NEGATIVE_OFFSET_FIX : 0) |
				(gChipInfo[i].V83_INTILESIZE_1X1_10BITS_FIX == 1 ? chipMinorFeatures19_V83_INTILESIZE_1X1_10BITS_FIX : 0) |
				(gChipInfo[i].FASTXDP3_ONLY_IN_DEPTHWISE_FIX == 1 ? chipMinorFeatures19_FASTXDP3_ONLY_IN_DEPTHWISE_FIX : 0) |
				(gChipInfo[i].USC_PAUSE_TP_WR_REQ_MORE_THAN_256_CYCLES_FIX == 1 ? chipMinorFeatures19_USC_PAUSE_TP_WR_REQ_MORE_THAN_256_CYCLES_FIX : 0) |
				(gChipInfo[i].DEPTHWISE_FLOAT_FIX == 1 ? chipMinorFeatures19_DEPTHWISE_FLOAT_FIX : 0) |
				(gChipInfo[i].TP_CIRCULAR_BUF_WRAP_ADDRESS_OVERFLOW_FIX == 1 ? chipMinorFeatures19_TP_CIRCULAR_BUF_WRAP_ADDRESS_OVERFLOW_FIX : 0) |
				(gChipInfo[i].NN_CIRCULAR_BUF_WRAP_ADDRESS_OVERFLOW_FIX == 1 ? chipMinorFeatures19_NN_CIRCULAR_BUF_WRAP_ADDRESS_OVERFLOW_FIX : 0);

			uint32_t features20 =
				(gChipInfo[i].CLOCK_DIV2_FREQ_CHANGE_FIX == 1 ? chipMinorFeatures20_CLOCK_DIV2_FREQ_CHANGE_FIX : 0) |
				(gChipInfo[i].SMALL_TILE_TENSOR_ADD_FIX == 1 ? chipMinorFeatures20_SMALL_TILE_TENSOR_ADD_FIX : 0) |
				(gChipInfo[i].DECOMPRESSOR_DEPTHWISE_FLOAT_FIX == 1 ? chipMinorFeatures20_DECOMPRESSOR_DEPTHWISE_FLOAT_FIX : 0) |
				(gChipInfo[i].TP_CIRCULAR_BUF_WRAP_ADDRESS_LESS_FIX == 1 ? chipMinorFeatures20_TP_CIRCULAR_BUF_WRAP_ADDRESS_LESS_FIX : 0) |
				(gChipInfo[i].V83_NUMOFPENDINGTILES_FOR_2NDIMAGE_FIX == 1 ? chipMinorFeatures20_V83_NUMOFPENDINGTILES_FOR_2NDIMAGE_FIX : 0) |
				(gChipInfo[i].V83_1ST_CACHE_MODE_VIPSRAM_RD_UPDATE_FIX == 1 ? chipMinorFeatures20_V83_1ST_CACHE_MODE_VIPSRAM_RD_UPDATE_FIX : 0) |
				(gChipInfo[i].V83_1ST_KERNEL_STREAM_BUFFER_UPDATE_FIX == 1 ? chipMinorFeatures20_V83_1ST_KERNEL_STREAM_BUFFER_UPDATE_FIX : 0) |
				(gChipInfo[i].USC_RW_SAME_CACHELINE_UPDATE_FIX == 1 ? chipMinorFeatures20_USC_RW_SAME_CACHELINE_UPDATE_FIX : 0) |
				(gChipInfo[i].NN_KERNEL_MSS_SBP2_DIRECT_STEAM_STEAM_FIX == 1 ? chipMinorFeatures20_NN_KERNEL_MSS_SBP2_DIRECT_STEAM_STEAM_FIX : 0) |
				(gChipInfo[i].CORE_NUM_OF_KID_FOR_MULTI_LAYER_FIX == 1 ? chipMinorFeatures20_CORE_NUM_OF_KID_FOR_MULTI_LAYER_FIX : 0) |
				(gChipInfo[i].KERNEL_XSIZE_YSIZE_NUM_FIX == 1 ? chipMinorFeatures20_KERNEL_XSIZE_YSIZE_NUM_FIX : 0) |
				(gChipInfo[i].NN_1ST_AND_2ND_INIMAGE_RAISE_VIPSRAM_RD_UPDATE_AT_SAME_TIME_FIX == 1 ? chipMinorFeatures20_NN_1ST_AND_2ND_INIMAGE_RAISE_VIPSRAM_RD_UPDATE_AT_SAME_TIME_FIX : 0) |
				(gChipInfo[i].NN_1ST_AND_2ND_INIMAGE_RAISE_VIPSRAM_RD_UPDATE_AT_SAME_TIME_PHASE1_FIX == 1 ? chipMinorFeatures20_NN_1ST_AND_2ND_INIMAGE_RAISE_VIPSRAM_RD_UPDATE_AT_SAME_TIME_PHASE1_FIX : 0) |
				(gChipInfo[i].NN_1ST_AND_2ND_INIMAGE_RAISE_VIPSRAM_RD_UPDATE_AT_SAME_TIME_PHASE2_FIX == 1 ? chipMinorFeatures20_NN_1ST_AND_2ND_INIMAGE_RAISE_VIPSRAM_RD_UPDATE_AT_SAME_TIME_PHASE2_FIX : 0) |
				(gChipInfo[i].TP_SPECIAL_LIST_PARSER_FIX == 1 ? chipMinorFeatures20_TP_SPECIAL_LIST_PARSER_FIX : 0) |
				(gChipInfo[i].DECOMPRESSOR_TREATS_TOTAL_CORES_AS_ACTIVE_CORES_FIX == 1 ? chipMinorFeatures20_DECOMPRESSOR_TREATS_TOTAL_CORES_AS_ACTIVE_CORES_FIX : 0) |
				(gChipInfo[i].DIRECT_INIMAGE_XSTIDE_LE_13BIT_FIX == 1 ? chipMinorFeatures20_DIRECT_INIMAGE_XSTIDE_LE_13BIT_FIX : 0) |
				(gChipInfo[i].SECONDIMG_TILE_SIDEBANFIFO_FIX == 1 ? chipMinorFeatures20_SECONDIMG_TILE_SIDEBANFIFO_FIX : 0) |
				(gChipInfo[i].TRSPB2_ENDADDR_EQUAL_SRAMEND_FIX == 1 ? chipMinorFeatures20_TRSPB2_ENDADDR_EQUAL_SRAMEND_FIX : 0) |
				(gChipInfo[i].NN_FP16_ALU == 1 ? chipMinorFeatures20_NN_FP16_ALU : 0) |
				(gChipInfo[i].NN_INT16_ALU == 1 ? chipMinorFeatures20_NN_INT16_ALU : 0) |
				(gChipInfo[i].NN_INT8_SCALE == 1 ? chipMinorFeatures20_NN_INT8_SCALE : 0) |
				(gChipInfo[i].NN_POWER_ISOLATION == 1 ? chipMinorFeatures20_NN_POWER_ISOLATION : 0) |
				(gChipInfo[i].ZRL_7BIT == 1 ? chipMinorFeatures20_ZRL_7BIT : 0) |
				(gChipInfo[i].NN_SMALLBATCH == 1 ? chipMinorFeatures20_NN_SMALLBATCH : 0) |
				(gChipInfo[i].TP_SMALLBATCH == 1 ? chipMinorFeatures20_TP_SMALLBATCH : 0) |
				(gChipInfo[i].ZRL_8BIT == 1 ? chipMinorFeatures20_ZRL_8BIT : 0) |
				(gChipInfo[i].DDR_BURST_LEN_256B == 1 ? chipMinorFeatures20_DDR_BURST_LEN_256B : 0) |
				(gChipInfo[i].XY_OFFSET_LIMITATION_FIX == 1 ? chipMinorFeatures20_XY_OFFSET_LIMITATION_FIX : 0) |
				(gChipInfo[i].NN_NONZERO_MIRROR_BORDER == 1 ? chipMinorFeatures20_NN_NONZERO_MIRROR_BORDER : 0) |
				(gChipInfo[i].IMAGE_PARTIAL_CACHE == 1 ? chipMinorFeatures20_IMAGE_PARTIAL_CACHE : 0);
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
			linux_devices[count].minor_features14 = features14;
			linux_devices[count].minor_features15 = features15;
			linux_devices[count].minor_features16 = features16;
			linux_devices[count].minor_features17 = features17;
			linux_devices[count].minor_features18 = features18;
			linux_devices[count].minor_features19 = features19;
			linux_devices[count].minor_features20 = features20;
#endif

#if defined NN_CORE_COUNT
			linux_devices[count].nn_core_count = gChipInfo[i].NNCoreCount;
#endif
			count++;
		}
	}

	// get device entries from ST's gc_features_database.h 6.4.9 (2021-12-30)
	n = sizeof(gChipInfo_STM32_4_6_9) / sizeof(gChipInfo_STM32_4_6_9[0]);
	for (int i = 0; i < n; i++) {
		bool found = false;

		for (int j = 0; j < device_count; ++j)
		{

			if ((devices[j].chipID == gChipInfo_STM32_4_6_9[i].chipID)
				&& (devices[j].chipVersion == gChipInfo_STM32_4_6_9[i].chipVersion)
				&& (devices[j].productID == gChipInfo_STM32_4_6_9[i].productID)
				&& (devices[j].ecoID == gChipInfo_STM32_4_6_9[i].ecoID)
				&& (devices[j].customerID == gChipInfo_STM32_4_6_9[i].customerID)
				)
			{
				found = true;
			}
		}

		if (found == false) {
			devices[count].chipID = gChipInfo_STM32_4_6_9[i].chipID;
			devices[count].chipVersion = gChipInfo_STM32_4_6_9[i].chipVersion;
			devices[count].productID = gChipInfo_STM32_4_6_9[i].productID;
			devices[count].ecoID = gChipInfo_STM32_4_6_9[i].ecoID;
			devices[count].customerID = gChipInfo_STM32_4_6_9[i].customerID;

			linux_devices[count].model = gChipInfo_STM32_4_6_9[i].chipID;
			linux_devices[count].revision = gChipInfo_STM32_4_6_9[i].chipVersion;
			linux_devices[count].product_id = gChipInfo_STM32_4_6_9[i].productID;
			linux_devices[count].customer_id = gChipInfo_STM32_4_6_9[i].customerID;
			linux_devices[count].eco_id = gChipInfo_STM32_4_6_9[i].ecoID;

			linux_devices[count].stream_count = gChipInfo_STM32_4_6_9[i].Streams;
			linux_devices[count].register_max = gChipInfo_STM32_4_6_9[i].TempRegisters;
			linux_devices[count].thread_count = gChipInfo_STM32_4_6_9[i].ThreadCount;
			linux_devices[count].shader_core_count = gChipInfo_STM32_4_6_9[i].NumShaderCores;
			linux_devices[count].vertex_cache_size = gChipInfo_STM32_4_6_9[i].VertexCacheSize;
			linux_devices[count].vertex_output_buffer_size = gChipInfo_STM32_4_6_9[i].VertexOutputBufferSize;
			linux_devices[count].pixel_pipes = gChipInfo_STM32_4_6_9[i].NumPixelPipes;
			linux_devices[count].instruction_count = gChipInfo_STM32_4_6_9[i].InstructionCount;
			linux_devices[count].num_constants = gChipInfo_STM32_4_6_9[i].NumberOfConstants;
			linux_devices[count].buffer_size = gChipInfo_STM32_4_6_9[i].BufferSize;
			linux_devices[count].varyings_count = gChipInfo_STM32_4_6_9[i].VaryingCount;

			uint32_t feature = (gChipInfo_STM32_4_6_9[i].REG_FastClear == 1 ? chipFeatures_FAST_CLEAR : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_SpecialAntiAliasing == 1 ? chipFeatures_SPECIAL_ANTI_ALIASING : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_Pipe3D == 1 ? chipFeatures_PIPE_3D : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_DXTTextureCompression == 1 ? chipFeatures_DXT_TEXTURE_COMPRESSION : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_DebugMode == 1 ? chipFeatures_DEBUG_MODE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_ZCompression == 1 ? chipFeatures_Z_COMPRESSION : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_YUV420Filter == 1 ? chipFeatures_YUV420_SCALER : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_MSAA == 1 ? chipFeatures_MSAA : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_DC == 1 ? chipFeatures_DC : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_Pipe2D == 1 ? chipFeatures_PIPE_2D : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_ETC1TextureCompression == 1 ? chipFeatures_ETC1_TEXTURE_COMPRESSION : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_FastScaler == 1 ? chipFeatures_FAST_SCALER : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_HighDynamicRange == 1 ? chipFeatures_HIGH_DYNAMIC_RANGE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_YUV420Tiler == 1 ? chipFeatures_YUV420_TILER : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_ModuleCG == 1 ? chipFeatures_MODULE_CG : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_MinArea == 1 ? chipFeatures_MIN_AREA : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_NoEZ == 1 ? chipFeatures_NO_EARLY_Z : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_No422Texture == 1 ? chipFeatures_NO_422_TEXTURE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BufferInterleaving == 1 ? chipFeatures_BUFFER_INTERLEAVING : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_ByteWrite2D == 1 ? chipFeatures_BYTE_WRITE_2D : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_NoScaler == 1 ? chipFeatures_NO_SCALER : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_YUY2Averaging == 1 ? chipFeatures_YUY2_AVERAGING : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_HalfPECache == 1 ? chipFeatures_HALF_PE_CACHE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_HalfTXCache == 1 ? chipFeatures_HALF_TX_CACHE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_YUY2RenderTarget == 1 ? chipFeatures_YUY2_RENDER_TARGET : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_Mem32BitSupport == 1 ? chipFeatures_MEM32 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_PipeVG == 1 ? chipFeatures_PIPE_VG : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_VGTS == 1 ? chipFeatures_VGTS : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_FE20 == 1 ? chipFeatures_FE20 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_ByteWrite3D == 1 ? chipFeatures_BYTE_WRITE_3D : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_RsYuvTarget == 1 ? chipFeatures_RS_YUV_TARGET : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_FE20BitIndex == 1 ? chipFeatures_32_BIT_INDICES : 0);

			uint32_t features0 =
				(gChipInfo_STM32_4_6_9[i].REG_FlipY == 1 ? chipMinorFeatures0_FLIP_Y : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_DualReturnBus == 1 ? chipMinorFeatures0_DUAL_RETURN_BUS : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_EndiannessConfig == 1 ? chipMinorFeatures0_ENDIANNESS_CONFIG : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_Texture8K == 1 ? chipMinorFeatures0_TEXTURE_8K : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_CorrectTextureConverter == 1 ? chipMinorFeatures0_CORRECT_TEXTURE_CONVERTER : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_SpecialMsaaLod == 1 ? chipMinorFeatures0_SPECIAL_MSAA_LOD : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_FastClearFlush == 1 ? chipMinorFeatures0_FAST_CLEAR_FLUSH : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_2DPE20 == 1 ? chipMinorFeatures0_2DPE20 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_CorrectAutoDisable == 1 ? chipMinorFeatures0_CORRECT_AUTO_DISABLE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_Render8K == 1 ? chipMinorFeatures0_RENDERTARGET_8K : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_TileStatus2Bits == 1 ? chipMinorFeatures0_2BITPERTILE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_SeparateTileStatusWhenInterleaved == 1 ? chipMinorFeatures0_SEPARATE_TILE_STATUS_WHEN_INTERLEAVED : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_SuperTiled32x32 == 1 ? chipMinorFeatures0_SUPER_TILED : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_VG20 == 1 ? chipMinorFeatures0_VG_20 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_TSExtendedCommands == 1 ? chipMinorFeatures0_TS_EXTENDED_COMMANDS : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_CompressionFifoFixed == 1 ? chipMinorFeatures0_COMPRESSION_FIFO_FIXED : 0) |
#ifdef ESTIMATED_VALUE
				(gChipInfo_STM32_4_6_9[i].REG_ExtraShaderInstructions0 == 1 ? chipMinorFeatures0_HAS_SIGN_FLOOR_CEIL : 0) | //?
				(gChipInfo_STM32_4_6_9[i].REG_ExtraShaderInstructions1 == 1 ? chipMinorFeatures0_HAS_SQRT_TRIG : 0) | //?
				(gChipInfo_STM32_4_6_9[i].REG_DefaultReg0 == 1 ? chipMinorFeatures0_MORE_MINOR_FEATURES : 0) | //?
#endif
				(gChipInfo_STM32_4_6_9[i].REG_VGFilter == 1 ? chipMinorFeatures0_VG_FILTER : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_VG21 == 1 ? chipMinorFeatures0_VG_21 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_ShaderGetsW == 1 ? chipMinorFeatures0_SHADER_HAS_W : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_MC20 == 1 ? chipMinorFeatures0_MC20 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_ShaderMSAASideband == 1 ? chipMinorFeatures0_MSAA_SIDEBAND : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes0 == 1 ? chipMinorFeatures0_BUG_FIXES0 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_VAA == 1 ? chipMinorFeatures0_VAA : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BypassInMSAA == 1 ? chipMinorFeatures0_BYPASS_IN_MSAA : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_HierarchicalZ == 1 ? chipMinorFeatures0_HZ : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_NewTexture == 1 ? chipMinorFeatures0_NEW_TEXTURE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_A8TargetSupport == 1 ? chipMinorFeatures0_2D_A8_TARGET : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_CorrectStencil == 1 ? chipMinorFeatures0_CORRECT_STENCIL : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_EnhanceVR == 1 ? chipMinorFeatures0_ENHANCE_VR : 0);

			uint32_t features1 =
				(gChipInfo_STM32_4_6_9[i].REG_RSUVSwizzle == 1 ? chipMinorFeatures1_RSUV_SWIZZLE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_V2Compression == 1 ? chipMinorFeatures1_V2_COMPRESSION : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_VGDoubleBuffer == 1 ? chipMinorFeatures1_VG_DOUBLE_BUFFER : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes1 == 1 ? chipMinorFeatures1_EXTRA_EVENT_STATES : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes2 == 1 ? chipMinorFeatures1_NO_STRIPING_NEEDED : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_TextureStride == 1 ? chipMinorFeatures1_TEXTURE_STRIDE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes3 == 1 ? chipMinorFeatures1_BUG_FIXES3 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_CorrectAutoDisable1 == 1 ? chipMinorFeatures1_AUTO_DISABLE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_AutoRestartTS == 1 ? chipMinorFeatures1_AUTO_RESTART_TS : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes4 == 1 ? chipMinorFeatures1_DISABLE_PE_GATING : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_L2Windowing == 1 ? chipMinorFeatures1_L2_WINDOWING : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_HalfFloatPipe == 1 ? chipMinorFeatures1_HALF_FLOAT : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_PixelDither == 1 ? chipMinorFeatures1_PIXEL_DITHER : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_TwoStencilReference == 1 ? chipMinorFeatures1_TWO_STENCIL_REFERENCE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_ExtendedPixelFormat == 1 ? chipMinorFeatures1_EXTENDED_PIXEL_FORMAT : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_CorrectMinMaxDepth == 1 ? chipMinorFeatures1_CORRECT_MIN_MAX_DEPTH : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_DitherAndFilterPlusAlpha2D == 1 ? chipMinorFeatures1_2D_DITHER : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes5 == 1 ? chipMinorFeatures1_BUG_FIXES5 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_New2D == 1 ? chipMinorFeatures1_NEW_2D : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_NewFloatingPointArithmetic == 1 ? chipMinorFeatures1_NEW_FP : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_TextureHorizontalAlignmentSelect == 1 ? chipMinorFeatures1_TEXTURE_HALIGN : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_NonPowerOfTwo == 1 ? chipMinorFeatures1_NON_POWER_OF_TWO : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_LinearTextureSupport == 1 ? chipMinorFeatures1_LINEAR_TEXTURE_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_Halti0 == 1 ? chipMinorFeatures1_HALTI0 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_CorrectOverflowVG == 1 ? chipMinorFeatures1_CORRECT_OVERFLOW_VG : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_NegativeLogFix == 1 ? chipMinorFeatures1_NEGATIVE_LOG_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_ResolveOffset == 1 ? chipMinorFeatures1_RESOLVE_OFFSET : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_OkToGateAxiClock == 1 ? chipMinorFeatures1_OK_TO_GATE_AXI_CLOCK : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_MMU == 1 ? chipMinorFeatures1_MMU_VERSION : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_WideLine == 1 ? chipMinorFeatures1_WIDE_LINE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes6 == 1 ? chipMinorFeatures1_BUG_FIXES6 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_FcFlushStall == 1 ? chipMinorFeatures1_FC_FLUSH_STALL : 0);

			uint32_t features2 = (gChipInfo_STM32_4_6_9[i].REG_LineLoop == 1 ? chipMinorFeatures2_LINE_LOOP : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_LogicOp == 1 ? chipMinorFeatures2_LOGIC_OP : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_SeamlessCubeMap == 1 ? chipMinorFeatures2_SEAMLESS_CUBE_MAP : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_SuperTiledTexture == 1 ? chipMinorFeatures2_SUPERTILED_TEXTURE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_LinearPE == 1 ? chipMinorFeatures2_LINEAR_PE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_RectPrimitive == 1 ? chipMinorFeatures2_RECT_PRIMITIVE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_Composition == 1 ? chipMinorFeatures2_COMPOSITION : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_CorrectAutoDisableCountWidth == 1 ? chipMinorFeatures2_CORRECT_AUTO_DISABLE_COUNT : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_PESwizzle == 1 ? chipMinorFeatures2_PE_SWIZZLE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_EndEvent == 1 ? chipMinorFeatures2_END_EVENT : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_S1S8 == 1 ? chipMinorFeatures2_S1S8 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_Halti1 == 1 ? chipMinorFeatures2_HALTI1 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_RGB888 == 1 ? chipMinorFeatures2_RGB888 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_TX_YUVAssembler == 1 ? chipMinorFeatures2_TX__YUV_ASSEMBLER : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_DynamicFrequencyScaling == 1 ? chipMinorFeatures2_DYNAMIC_FREQUENCY_SCALING : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_TXFilter == 1 ? chipMinorFeatures2_TX_FILTER : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_FullDirectFB == 1 ? chipMinorFeatures2_FULL_DIRECTFB : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_OnePass2DFilter == 1 ? chipMinorFeatures2_2D_TILING : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_ThreadWalkerInPS == 1 ? chipMinorFeatures2_THREAD_WALKER_IN_PS : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_TileFiller == 1 ? chipMinorFeatures2_TILE_FILLER : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_YUVStandard == 1 ? chipMinorFeatures2_YUV_STANDARD : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_MultiSourceBlt == 1 ? chipMinorFeatures2_2D_MULTI_SOURCE_BLIT : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_YUVConversion == 1 ? chipMinorFeatures2_YUV_CONVERSION : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_FlushFixed2D == 1 ? chipMinorFeatures2_FLUSH_FIXED_2D : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_Interleaver == 1 ? chipMinorFeatures2_INTERLEAVER : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_MixedStreams == 1 ? chipMinorFeatures2_MIXED_STREAMS : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_L2CacheFor2D420 == 1 ? chipMinorFeatures2_2D_420_L2CACHE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes7 == 1 ? chipMinorFeatures2_BUG_FIXES7 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_NoIndexPattern == 1 ? chipMinorFeatures2_2D_NO_INDEX8_BRUSH : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_TextureTileStatus == 1 ? chipMinorFeatures2_TEXTURE_TILED_READ : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_DecompressZ16 == 1 ? chipMinorFeatures2_DECOMPRESS_Z16 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes8 == 1 ? chipMinorFeatures2_BUG_FIXES8 : 0);

			uint32_t features3 = (gChipInfo_STM32_4_6_9[i].REG_DERotationStallFix == 1 ? chipMinorFeatures3_ROTATION_STALL_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_OclOnly == 1 ? chipMinorFeatures3_OCL_ONLY : 0) |
#ifdef ESTIMATED_VALUE
				(gChipInfo_STM32_4_6_9[i].REG_NewFeatures0 == 1 ? chipMinorFeatures3_2D_MULTI_SOURCE_BLT_EX : 0) |
#endif
				(gChipInfo_STM32_4_6_9[i].REG_InstructionCache == 1 ? chipMinorFeatures3_INSTRUCTION_CACHE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_GeometryShader == 1 ? chipMinorFeatures3_GEOMETRY_SHADER : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_TexCompressionSupertiled == 1 ? chipMinorFeatures3_TEX_COMPRESSION_SUPERTILED : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_Generics == 1 ? chipMinorFeatures3_GENERICS : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes9 == 1 ? chipMinorFeatures3_BUG_FIXES9 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_FastMSAA == 1 ? chipMinorFeatures3_FAST_MSAA : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_WClip == 1 ? chipMinorFeatures3_WCLIP : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes10 == 1 ? chipMinorFeatures3_BUG_FIXES10 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_UnifiedSamplers == 1 ? chipMinorFeatures3_UNIFIED_SAMPLERS : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes11 == 1 ? chipMinorFeatures3_BUG_FIXES11 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_PerformanceCounters == 1 ? chipMinorFeatures3_PERFORMANCE_COUNTERS : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_ExtraShaderInstructions2 == 1 ? chipMinorFeatures3_HAS_FAST_TRANSCENDENTALS : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes12 == 1 ? chipMinorFeatures3_BUG_FIXES12 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes13 == 1 ? chipMinorFeatures3_BUG_FIXES13 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_DEEnhancements1 == 1 ? chipMinorFeatures3_DE_ENHANCEMENTS1 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_ACE == 1 ? chipMinorFeatures3_ACE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_TXEnhancements1 == 1 ? chipMinorFeatures3_TX_ENHANCEMENTS1 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_SHEnhancements1 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS1 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_SHEnhancements2 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS2 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_PEEnhancements1 == 1 ? chipMinorFeatures3_PE_ENHANCEMENTS1 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_DEEnhancements2 == 1 ? chipMinorFeatures3_2D_FC_SOURCE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes14 == 1 ? chipMinorFeatures3_BUG_FIXES_14 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_PowerOptimizations0 == 1 ? chipMinorFeatures3_POWER_OPTIMIZATIONS_0 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_NewHZ == 1 ? chipMinorFeatures3_NEW_HZ : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes15 == 1 ? chipMinorFeatures3_PE_DITHER_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_DEEnhancements3 == 1 ? chipMinorFeatures3_DE_ENHANCEMENTS3 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_SHEnhancements3 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS3 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_SHEnhancements4 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS4 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_TXEnhancements2 == 1 ? chipMinorFeatures3_TX_ENHANCEMENTS2 : 0);

			uint32_t features4 = (gChipInfo_STM32_4_6_9[i].REG_FEEnhancements1 == 1 ? chipMinorFeatures4_FE_ENHANCEMENTS1 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_PEEnhancements2 == 1 ? chipMinorFeatures4_PE_ENHANCEMENTS2 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_PAEnhancements1 == 1 ? chipMinorFeatures4_FRUSTUM_CLIP_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_DENoGamma == 1 ? chipMinorFeatures4_DE_NO_GAMMA : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_PAEnhancements2 == 1 ? chipMinorFeatures4_PA_ENHANCEMENTS_2 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_DEEnhancements4 == 1 ? chipMinorFeatures4_2D_GAMMA : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_PEEnhancements3 == 1 ? chipMinorFeatures4_SINGLE_BUFFER : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_HIEnhancements1 == 1 ? chipMinorFeatures4_HI_ENHANCEMENTS_1 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_TXEnhancements3 == 1 ? chipMinorFeatures4_TX_ENHANCEMENTS_3 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_SHEnhancements5 == 1 ? chipMinorFeatures4_SH_ENHANCEMENTS_5 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_FEEnhancements2 == 1 ? chipMinorFeatures4_FE_ENHANCEMENTS_2 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes16 == 1 ? chipMinorFeatures4_TX_LERP_PRECISION_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_DEEnhancements5 == 1 ? chipMinorFeatures4_2D_COLOR_SPACE_CONVERSION : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_TXEnhancements4 == 1 ? chipMinorFeatures4_TEXTURE_ASTC : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_PEEnhancements4 == 1 ? chipMinorFeatures4_PE_ENHANCEMENTS_4 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_MCEnhancements1 == 1 ? chipMinorFeatures4_MC_ENHANCEMENTS_1 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_Halti2 == 1 ? chipMinorFeatures4_HALTI2 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_DEMirrorRotate == 1 ? chipMinorFeatures4_2D_MIRROR_EXTENSION : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_SmallMSAA == 1 ? chipMinorFeatures4_SMALL_MSAA : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes17 == 1 ? chipMinorFeatures4_BUG_FIXES_17 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_Rasterizer2 == 1 ? chipMinorFeatures4_NEW_RA : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_DualPipeOPF == 1 ? chipMinorFeatures4_2D_OPF_YUV_OUTPUT : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_MultiSrcV2 == 1 ? chipMinorFeatures4_2D_MULTI_SOURCE_BLT_EX2 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_CSCV2 == 1 ? chipMinorFeatures4_NO_USER_CSC : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_PAEnhancements3 == 1 ? chipMinorFeatures4_ZFIXES : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes18 == 1 ? chipMinorFeatures4_BUG_FIXES18 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_Compression2D == 1 ? chipMinorFeatures4_2D_COMPRESSION : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_Probe == 1 ? chipMinorFeatures4_PROBE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_MediumPrecision == 1 ? chipMinorFeatures4_MEDIUM_PRECISION : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_DESupertile == 1 ? chipMinorFeatures4_2D_SUPER_TILE_VERSION : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes19 == 1 ? chipMinorFeatures4_BUG_FIXES19 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_SHEnhancements6 == 1 ? chipMinorFeatures4_SH_ENHANCEMENTS6 : 0);

			uint32_t features5 = (gChipInfo_STM32_4_6_9[i].REG_SHEnhancements7 == 1 ? chipMinorFeatures5_SH_ENHANCEMENTS7 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes20 == 1 ? chipMinorFeatures5_BUG_FIXES20 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_DEAddress40 == 1 ? chipMinorFeatures5_DE_ADDRESS_40 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_MiniMMUFix == 1 ? chipMinorFeatures5_MINI_MMU_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_EEZ == 1 ? chipMinorFeatures5_EEZ : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes21 == 1 ? chipMinorFeatures5_BUG_FIXES21 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_ExtraVgCaps == 1 ? chipMinorFeatures5_EXTRA_VG_CAPS : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_MultiSrcV15 == 1 ? chipMinorFeatures5_MULTI_SRC_V15 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes22 == 1 ? chipMinorFeatures5_BUG_FIXES22 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_Halti3 == 1 ? chipMinorFeatures5_HALTI3 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_TessellationShaders == 1 ? chipMinorFeatures5_TESSELATION_SHADERS : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_OnePass2DFilter == 1 ? chipMinorFeatures5_2D_ONE_PASS_FILTER_TAP : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_MultiSrcV2StrQuad == 1 ? chipMinorFeatures5_MULTI_SRC_V2_STR_QUAD : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_SeperateSRCAndDstCache == 1 ? chipMinorFeatures5_SEPARATE_SRC_DST : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_Halti4 == 1 ? chipMinorFeatures5_HALTI4 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_RAWriteDepth == 1 ? chipMinorFeatures5_RA_WRITE_DEPTH : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_AndroidOnly == 1 ? chipMinorFeatures5_ANDROID_ONLY : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_HasChipProductReg == 1 ? chipMinorFeatures5_HAS_PRODUCTID : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_TXSupportDEC == 1 ? chipMinorFeatures5_TX_SUPPORT_DEC : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_S8MSAACompression == 1 ? chipMinorFeatures5_S8_MSAA_COMPRESSION : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixesIn544 == 1 ? chipMinorFeatures5_PE_DITHER_FIX2 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_L2CacheRemove == 1 ? chipMinorFeatures5_L2_CACHE_REMOVE : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_FEAllowRndVtxCnt == 1 ? chipMinorFeatures5_FE_ALLOW_RND_VTX_CNT : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_CubeMapFL28 == 1 ? chipMinorFeatures5_CUBE_MAP_FL28 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_TX6bitFrac == 1 ? chipMinorFeatures5_TX_6BIT_FRAC : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_FEAllowStallPrefetchEng == 1 ? chipMinorFeatures5_FE_ALLOW_STALL_PREFETCH_ENG : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_ThirdPartyCompression == 1 ? chipMinorFeatures5_THIRD_PARTY_COMPRESSION : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_RSS8 == 1 ? chipMinorFeatures5_RS_DEPTHSTENCIL_NATIVE_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_MSAACoherencyCheck == 1 ? chipMinorFeatures5_V2_MSAA_COMP_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_Halti5 == 1 ? chipMinorFeatures5_HALTI5 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_Evis == 1 ? chipMinorFeatures5_EVIS : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BltEngine == 1 ? chipMinorFeatures5_BLT_ENGINE : 0);

			uint32_t features6 = (gChipInfo_STM32_4_6_9[i].REG_BugFixes23 == 1 ? chipMinorFeatures6_BUG_FIXES_23 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_BugFixes24 == 1 ? chipMinorFeatures6_BUG_FIXES_24 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_DEC == 1 ? chipMinorFeatures6_DEC : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_VSTileNV12 == 1 ? chipMinorFeatures6_VS_TILE_NV12 : 0) |
				(gChipInfo_STM32_4_6_9[i].REG_VSTileNV12_10BIT == 1 ? chipMinorFeatures6_VS_TILE_NV12_10BIT : 0) |
				//(gChipInfo_STM32_4_6_9[i].REG_DisableVIP == 1 ?  : 0) |
				(gChipInfo_STM32_4_6_9[i].RenderTarget8 == 1 ? chipMinorFeatures6_RENDER_TARGET_8 : 0) |
				(gChipInfo_STM32_4_6_9[i].TxLodFlowCorrection == 1 ? chipMinorFeatures6_TEX_LOD_FLOW_CORR : 0) |
				(gChipInfo_STM32_4_6_9[i].FaceLod == 1 ? chipMinorFeatures6_FACE_LOD : 0) |
				(gChipInfo_STM32_4_6_9[i].MultiCoreSemaphoreStallV2 == 1 ? chipMinorFeatures6_MULTI_CORE_SEMAPHORE_STALL_V2 : 0) |
				(gChipInfo_STM32_4_6_9[i].VMSAA == 1 ? chipMinorFeatures6_VMSAA : 0) |
				(gChipInfo_STM32_4_6_9[i].ChipEnableLink == 1 ? chipMinorFeatures6_CHIP_ENABLE_LINK : 0) |
				(gChipInfo_STM32_4_6_9[i].MULTI_SRC_BLT_1_5_ENHANCEMENT == 1 ? chipMinorFeatures6_MULTI_SRC_BLT_1_5_ENHANCEMENT : 0) |
				(gChipInfo_STM32_4_6_9[i].MULTI_SRC_BLT_BILINEAR_FILTER == 1 ? chipMinorFeatures6_MULTI_SRC_BLT_BILINEAR_FILTER : 0) |
				(gChipInfo_STM32_4_6_9[i].RA_HZEZ_CLOCK_CONTROL == 1 ? chipMinorFeatures6_RA_HZEZ_CLOCK_CONTROL : 0) |
				(gChipInfo_STM32_4_6_9[i].CACHE128B256BPERLINE == 1 ? chipMinorFeatures6_CACHE128B256BPERLINE : 0) |
				(gChipInfo_STM32_4_6_9[i].V4Compression == 1 ? chipMinorFeatures6_V4_COMPRESSION : 0) |
				//(gChipInfo_STM32_4_6_9[i].DE2D_MAJOR_SUPER_TILE == 1 ?  : 0) | //?
				(gChipInfo_STM32_4_6_9[i].PE2D_MAJOR_SUPER_TILE == 1 ? chipMinorFeatures6_PE2D_MAJOR_SUPER_TILE : 0) |
				(gChipInfo_STM32_4_6_9[i].PE_32BPC_COLORMASK_FIX == 1 ? chipMinorFeatures6_PE_32BPC_COLORMASK_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].ALPHA_BLENDING_OPT == 1 ? chipMinorFeatures6_ALPHA_BLENDING_OPT : 0) |
				(gChipInfo_STM32_4_6_9[i].NEW_GPIPE == 1 ? chipMinorFeatures6_NEW_GPIPE : 0) |
				(gChipInfo_STM32_4_6_9[i].PIPELINE_32_ATTRIBUTES == 1 ? chipMinorFeatures6_PIPELINE_32_ATTRIBUTES : 0) |
				(gChipInfo_STM32_4_6_9[i].MSAA_SHADING == 1 ? chipMinorFeatures6_MSAA_SHADING : 0) |
				(gChipInfo_STM32_4_6_9[i].NO_ANISTRO_FILTER == 1 ? chipMinorFeatures6_NO_ANISTRO_FILTER : 0) |
				(gChipInfo_STM32_4_6_9[i].NO_ASTC == 1 ? chipMinorFeatures6_NO_ASTC : 0) |
				(gChipInfo_STM32_4_6_9[i].NO_DXT == 1 ? chipMinorFeatures6_NO_DXT : 0) |
				(gChipInfo_STM32_4_6_9[i].HWTFB == 1 ? chipMinorFeatures6_HWTFB : 0) |
				(gChipInfo_STM32_4_6_9[i].RA_DEPTH_WRITE_MSAA1X_FIX == 1 ? chipMinorFeatures6_RA_DEPTH_WRITE_MSAA1X_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].EZHZ_CLOCKGATE_FIX == 1 ? chipMinorFeatures6_EZHZ_CLOCKGATE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_SNAP2PAGE_FIX == 1 ? chipMinorFeatures6_SH_SNAP2PAGE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_HALFDEPENDENCY_FIX == 1 ? chipMinorFeatures6_SH_HALFDEPENDENCY_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].USC_MCFILL_FIX == 1 ? chipMinorFeatures6_USC_MCFILL_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].TPG_TCPERF_FIX == 1 ? chipMinorFeatures6_TPG_TCPERF_FIX : 0);

			uint32_t features7 = (gChipInfo_STM32_4_6_9[i].USC_MDFIFO_OVERFLOW_FIX == 1 ? chipMinorFeatures7_USC_MDFIFO_OVERFLOW_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_TEXLD_BARRIER_IN_CS_FIX == 1 ? chipMinorFeatures7_SH_TEXLD_BARRIER_IN_CS_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].RS_NEW_BASEADDR == 1 ? chipMinorFeatures7_RS_NEW_BASEADDR : 0) |
				(gChipInfo_STM32_4_6_9[i].PE_8bpp_DUALPIPE_FIX == 1 ? chipMinorFeatures7_PE_8BPP_DUALPIPE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_ADVANCED_INSTR == 1 ? chipMinorFeatures7_SH_ADVANCED_INSTR : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_FLAT_INTERPOLATION_DUAL16_FIX == 1 ? chipMinorFeatures7_SH_FLAT_INTERPOLATION_DUAL16_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].USC_CONTINUOUS_FLUS_FIX == 1 ? chipMinorFeatures7_USC_CONTINUOUS_FLUS_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_SUPPORT_V4 == 1 ? chipMinorFeatures7_SH_SUPPORT_V4 : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_SUPPORT_ALPHA_KILL == 1 ? chipMinorFeatures7_SH_SUPPORT_ALPHA_KILL : 0) |
				(gChipInfo_STM32_4_6_9[i].PE_NO_ALPHA_TEST == 1 ? chipMinorFeatures7_PE_NO_ALPHA_TEST : 0) |
				(gChipInfo_STM32_4_6_9[i].TX_LOD_NEAREST_SELECT == 1 ? chipMinorFeatures7_TX_LOD_NEAREST_SELECT : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_FIX_LDEXP == 1 ? chipMinorFeatures7_SH_FIX_LDEXP : 0) |
				(gChipInfo_STM32_4_6_9[i].SUPPORT_MOVAI == 1 ? chipMinorFeatures7_SUPPORT_MOVAI : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_SNAP2PAGE_MAXPAGES_FIX == 1 ? chipMinorFeatures7_SH_SNAP2PAGE_MAXPAGES_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].PE_RGBA16I_FIX == 1 ? chipMinorFeatures7_PE_RGBA16I_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].BLT_8bpp_256TILE_FC_FIX == 1 ? chipMinorFeatures7_BLT_8bpp_256TILE_FC_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].PE_64bit_FENCE_FIX == 1 ? chipMinorFeatures7_PE_64BIT_FENCE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].USC_FULL_CACHE_FIX == 1 ? chipMinorFeatures7_USC_FULL_CACHE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].TX_YUV_ASSEMBLER_10BIT == 1 ? chipMinorFeatures7_TX_YUV_ASSEMBLER_10BIT : 0) |
				(gChipInfo_STM32_4_6_9[i].FE_32bit_INDEX_FIX == 1 ? chipMinorFeatures7_FE_32BIT_INDEX_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].BLT_64bpp_MASKED_CLEAR_FIX == 1 ? chipMinorFeatures7_BLT_64BPP_MASKED_CLEAR_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].SECURITY == 1 ? chipMinorFeatures7_BIT_SECURITY : 0) |
				(gChipInfo_STM32_4_6_9[i].ROBUSTNESS == 1 ? chipMinorFeatures7_BIT_ROBUSTNESS : 0) |
				(gChipInfo_STM32_4_6_9[i].USC_ATOMIC_FIX == 1 ? chipMinorFeatures7_USC_ATOMIC_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_PSO_MSAA1x_FIX == 1 ? chipMinorFeatures7_SH_PSO_MSAA1x_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].USC_VX_PERF_FIX == 1 ? chipMinorFeatures7_BIT_USC_VX_PERF_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].EVIS_NO_ABSDIFF == 1 ? chipMinorFeatures7_EVIS_NO_ABSDIFF : 0) |
				(gChipInfo_STM32_4_6_9[i].EVIS_NO_BITREPLACE == 1 ? chipMinorFeatures7_EVIS_NO_BITREPLACE : 0) |
				(gChipInfo_STM32_4_6_9[i].EVIS_NO_BOXFILTER == 1 ? chipMinorFeatures7_EVIS_NO_BOXFILTER : 0) |
				(gChipInfo_STM32_4_6_9[i].EVIS_NO_CORDIAC == 1 ? chipMinorFeatures7_EVIS_NO_CORDIAC : 0) |
				(gChipInfo_STM32_4_6_9[i].EVIS_NO_DP32 == 1 ? chipMinorFeatures7_EVIS_NO_DP32 : 0) |
				(gChipInfo_STM32_4_6_9[i].EVIS_NO_FILTER == 1 ? chipMinorFeatures7_EVIS_NO_FILTER : 0);

			uint32_t features8 =
				(gChipInfo_STM32_4_6_9[i].EVIS_NO_IADD == 1 ? chipMinorFeatures8_EVIS_NO_IADD : 0) |
				(gChipInfo_STM32_4_6_9[i].EVIS_NO_SELECTADD == 1 ? chipMinorFeatures8_EVIS_NO_SELECTADD : 0) |
				(gChipInfo_STM32_4_6_9[i].EVIS_LERP_7OUTPUT == 1 ? chipMinorFeatures8_EVIS_LERP_7OUTPUT : 0) |
				(gChipInfo_STM32_4_6_9[i].EVIS_ACCSQ_8OUTPUT == 1 ? chipMinorFeatures8_EVIS_ACCSQ_8OUTPUT : 0) |
				(gChipInfo_STM32_4_6_9[i].USC_GOS_ADDR_FIX == 1 ? chipMinorFeatures8_USC_GOS_ADDR_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].TX_8bit_UVFrac == 1 ? chipMinorFeatures8_TX_8BIT_UVFRAC : 0) |
				(gChipInfo_STM32_4_6_9[i].TX_DESC_CACHE_CLOCKGATE_FIX == 1 ? chipMinorFeatures8_TX_DESC_CACHE_CLOCKGATE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].RSBLT_MSAA_DECOMPRESSION == 1 ? chipMinorFeatures8_RSBLT_MSAA_DECOMPRESSION : 0) |
				(gChipInfo_STM32_4_6_9[i].TX_INTEGER_COORDINATE == 1 ? chipMinorFeatures8_TX_INTEGER_COORDINATE : 0) |
				(gChipInfo_STM32_4_6_9[i].DRAWID == 1 ? chipMinorFeatures8_DRAWID : 0) |
				(gChipInfo_STM32_4_6_9[i].PSIO_SAMPLEMASK_IN_R0ZW_FIX == 1 ? chipMinorFeatures8_PSIO_SAMPLEMASK_IN_R0ZW_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].TX_INTEGER_COORDINATE_V2 == 1 ? chipMinorFeatures8_TX_INTEGER_COORDINATE_V2 : 0) |
				(gChipInfo_STM32_4_6_9[i].MULTI_CORE_BLOCK_SET_CONFIG == 1 ? chipMinorFeatures8_MULTI_CORE_BLOCK_SET_CONFIG : 0) |
				(gChipInfo_STM32_4_6_9[i].VG_RESOLVE_ENGINE == 1 ? chipMinorFeatures8_VG_RESOLVE_ENGINE : 0) |
				(gChipInfo_STM32_4_6_9[i].VG_PE_COLOR_KEY == 1 ? chipMinorFeatures8_VG_PE_COLOR_KEY : 0) |
				(gChipInfo_STM32_4_6_9[i].VG_IM_INDEX_FORMAT == 1 ? chipMinorFeatures8_VG_IM_INDEX_FORMAT : 0) |
				(gChipInfo_STM32_4_6_9[i].SNAPPAGE_CMD == 1 ? chipMinorFeatures8_SNAPPAGE_CMD : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_NO_INDEX_CONST_ON_A0 == 1 ? chipMinorFeatures8_SH_NO_INDEX_CONST_ON_A0 : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_NO_ONECONST_LIMIT == 1 ? chipMinorFeatures8_SH_NO_ONECONST_LIMIT : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_IMG_LDST_ON_TEMP == 1 ? chipMinorFeatures8_SH_IMG_LDST_ON_TEMP : 0) |
				(gChipInfo_STM32_4_6_9[i].COMPUTE_ONLY == 1 ? chipMinorFeatures8_COMPUTE_ONLY : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_IMG_LDST_CLAMP == 1 ? chipMinorFeatures8_SH_IMG_LDST_CLAMP : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_ICACHE_ALLOC_COUNT_FIX == 1 ? chipMinorFeatures8_SH_ICACHE_ALLOC_COUNT_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_ICACHE_PREFETCH == 1 ? chipMinorFeatures8_SH_ICACHE_PREFETCH : 0) |
				(gChipInfo_STM32_4_6_9[i].PE2D_SEPARATE_CACHE == 1 ? chipMinorFeatures8_PE2D_SEPARATE_CACHE : 0) |
				(gChipInfo_STM32_4_6_9[i].VG_AYUV_INPUT_OUTPUT == 1 ? chipMinorFeatures8_VG_AYUV_INPUT_OUTPUT : 0) |
				(gChipInfo_STM32_4_6_9[i].VG_DOUBLE_IMAGE == 1 ? chipMinorFeatures8_VG_DOUBLE_IMAGE : 0) |
				(gChipInfo_STM32_4_6_9[i].VG_RECTANGLE_STRIPE_MODE == 1 ? chipMinorFeatures8_VG_RECTANGLE_STRIPE_MODE : 0) |
				(gChipInfo_STM32_4_6_9[i].VG_MMU == 1 ? chipMinorFeatures8_VG_MMU : 0) |
				(gChipInfo_STM32_4_6_9[i].VG_IM_FILTER == 1 ? chipMinorFeatures8_VG_IM_FILTER : 0) |
				(gChipInfo_STM32_4_6_9[i].VG_IM_YUV_PACKET == 1 ? chipMinorFeatures8_VG_IM_YUV_PACKET : 0) |
				(gChipInfo_STM32_4_6_9[i].VG_IM_YUV_PLANAR == 1 ? chipMinorFeatures8_VG_IM_YUV_PLANAR : 0);

			uint32_t features9 =
				(gChipInfo_STM32_4_6_9[i].VG_PE_YUV_PACKET == 1 ? chipMinorFeatures9_VG_PE_YUV_PACKET : 0) |
				(gChipInfo_STM32_4_6_9[i].VG_COLOR_PRECISION_8_BIT == 1 ? chipMinorFeatures9_VG_COLOR_PRECISION_8_BIT : 0) |
				(gChipInfo_STM32_4_6_9[i].PE_MSAA_OQ_FIX == 1 ? chipMinorFeatures9_PE_MSAA_OQ_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].PSIO_MSAA_CL_FIX == 1 ? chipMinorFeatures9_PSIO_MSAA_CL_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].USC_DEFER_FILL_FIX == 1 ? chipMinorFeatures9_USC_DEFER_FILL_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_CLOCK_GATE_FIX == 1 ? chipMinorFeatures9_SH_CLOCK_GATE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].FE_NEED_DUMMYDRAW == 1 ? chipMinorFeatures9_FE_NEED_DUMMYDRAW : 0) |
				(gChipInfo_STM32_4_6_9[i].PE2D_LINEAR_YUV420_OUTPUT == 1 ? chipMinorFeatures9_PE2D_LINEAR_YUV420_OUTPUT : 0) |
				(gChipInfo_STM32_4_6_9[i].PE2D_LINEAR_YUV420_10BIT == 1 ? chipMinorFeatures9_PE2D_LINEAR_YUV420_10BIT : 0) |
				(gChipInfo_STM32_4_6_9[i].MULTI_CLUSTER == 1 ? chipMinorFeatures9_MULTI_CLUSTER : 0) |
				(gChipInfo_STM32_4_6_9[i].VG_TS_CULLING == 1 ? chipMinorFeatures9_VG_TS_CULLING : 0) |
				(gChipInfo_STM32_4_6_9[i].VG_FP25 == 1 ? chipMinorFeatures9_VG_FP25 : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_MULTI_WG_PACK == 1 ? chipMinorFeatures9_SH_MULTI_WG_PACK : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_DUAL16_SAMPLEMASK_ZW == 1 ? chipMinorFeatures9_SH_DUAL16_SAMPLEMASK_ZW : 0) |
				(gChipInfo_STM32_4_6_9[i].TPG_TRIVIAL_MODE_FIX == 1 ? chipMinorFeatures9_TPG_TRIVIAL_MODE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].TX_ASTC_MULTISLICE_FIX == 1 ? chipMinorFeatures9_TX_ASTC_MULTISLICE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].FE_ROBUST_FIX == 1 ? chipMinorFeatures9_FE_ROBUST_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_GPIPE_ACCESS_FULLTEMPS == 1 ? chipMinorFeatures9_SH_GPIPE_ACCESS_FULLTEMPS : 0) |
				(gChipInfo_STM32_4_6_9[i].PSIO_INTERLOCK == 1 ? chipMinorFeatures9_PSIO_INTERLOCK : 0) |
				(gChipInfo_STM32_4_6_9[i].PA_WIDELINE_FIX == 1 ? chipMinorFeatures9_PA_WIDELINE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].WIDELINE_HELPER_FIX == 1 ? chipMinorFeatures9_WIDELINE_HELPER_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].G2D_3rd_PARTY_COMPRESSION_1_1 == 1 ? chipMinorFeatures9_G2D_3RD_PARTY_COMPRESSION_1_1 : 0) |
				(gChipInfo_STM32_4_6_9[i].TX_FLUSH_L1CACHE == 1 ? chipMinorFeatures9_TX_FLUSH_L1CACHE : 0) |
				(gChipInfo_STM32_4_6_9[i].PE_DITHER_FIX2 == 1 ? chipMinorFeatures9_PE_DITHER_FIX2 : 0) |
				(gChipInfo_STM32_4_6_9[i].G2D_DEC400 == 1 ? chipMinorFeatures9_G2D_DEC400 : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_TEXLD_U_FIX == 1 ? chipMinorFeatures9_SH_TEXLD_U_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].MC_FCCACHE_BYTEMASK == 1 ? chipMinorFeatures9_MC_FCCACHE_BYTEMASK : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_MULTI_WG_PACK_FIX == 1 ? chipMinorFeatures9_SH_MULTI_WG_PACK_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].DC_OVERLAY_SCALING == 1 ? chipMinorFeatures9_DC_OVERLAY_SCALING : 0) |
				(gChipInfo_STM32_4_6_9[i].DC_SOURCE_ROTATION == 1 ? chipMinorFeatures9_DC_SOURCE_ROTATION : 0) |
				(gChipInfo_STM32_4_6_9[i].DC_TILED == 1 ? chipMinorFeatures9_DC_TILED : 0) |
				(gChipInfo_STM32_4_6_9[i].DC_YUV_L1 == 1 ? chipMinorFeatures9_DC_YUV_L1 : 0);

			uint32_t features10 =
				(gChipInfo_STM32_4_6_9[i].DC_D30_OUTPUT == 1 ? chipMinorFeatures10_DC_D30_OUTPUT : 0) |
				(gChipInfo_STM32_4_6_9[i].DC_MMU == 1 ? chipMinorFeatures10_DC_MMU : 0) |
				(gChipInfo_STM32_4_6_9[i].DC_COMPRESSION == 1 ? chipMinorFeatures10_DC_COMPRESSION : 0) |
				(gChipInfo_STM32_4_6_9[i].DC_QOS == 1 ? chipMinorFeatures10_DC_QOS : 0) |
				(gChipInfo_STM32_4_6_9[i].PE_ADVANCE_BLEND_PART0 == 1 ? chipMinorFeatures10_PE_ADVANCE_BLEND_PART0 : 0) |
				(gChipInfo_STM32_4_6_9[i].FE_PATCHLIST_FETCH_FIX == 1 ? chipMinorFeatures10_FE_PATCHLIST_FETCH_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].RA_CG_FIX == 1 ? chipMinorFeatures10_RA_CG_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].EVIS_VX2 == 1 ? chipMinorFeatures10_EVIS_VX2 : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_FLOAT32_IO == 1 ? chipMinorFeatures10_NN_FLOAT : 0) |
				(gChipInfo_STM32_4_6_9[i].DEC400 == 1 ? chipMinorFeatures10_DEC400 : 0) |
				(gChipInfo_STM32_4_6_9[i].LS_SUPPORT_PERCOMP_DEPENDENCY == 1 ? chipMinorFeatures10_LS_SUPPORT_PERCOMP_DEPENDENCY : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_ENGINE == 1 ? chipMinorFeatures10_TP_ENGINE : 0) |
				(gChipInfo_STM32_4_6_9[i].MULTI_CORE_BLOCK_SET_CONFIG2 == 1 ? chipMinorFeatures10_MULTI_CORE_BLOCK_SET_CONFIG2 : 0) |
				(gChipInfo_STM32_4_6_9[i].PE_VMSAA_COVERAGE_CACHE_FIX == 1 ? chipMinorFeatures10_PE_VMSAA_COVERAGE_CACHE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].SECURITY_AHB == 1 ? chipMinorFeatures10_SECURITY_AHB : 0) |
				(gChipInfo_STM32_4_6_9[i].MULTICORE_SEMAPHORESTALL_V3 == 1 ? chipMinorFeatures10_MULTICORE_SEMAPHORESTALL_V3 : 0) |
				(gChipInfo_STM32_4_6_9[i].SMALLBATCH == 1 ? chipMinorFeatures10_SMALLBATCH : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_CMPLX == 1 ? chipMinorFeatures10_SH_CMPLX : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_IDIV0_SWZL_EHS == 1 ? chipMinorFeatures10_SH_IDIV0_SWZL_EHS : 0) |
				(gChipInfo_STM32_4_6_9[i].TX_LERP_LESS_BIT == 1 ? chipMinorFeatures10_TX_LERP_LESS_BIT : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_GM_ENDIAN == 1 ? chipMinorFeatures10_SH_GM_ENDIAN : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_GM_USC_UNALLOC == 1 ? chipMinorFeatures10_SH_GM_USC_UNALLOC : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_END_OF_BB == 1 ? chipMinorFeatures10_SH_END_OF_BB : 0) |
				(gChipInfo_STM32_4_6_9[i].VIP_V7 == 1 ? chipMinorFeatures10_VIP_V7 : 0) |
				(gChipInfo_STM32_4_6_9[i].TX_BORDER_CLAMP_FIX == 1 ? chipMinorFeatures10_TX_BORDER_CLAMP_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_IMG_LD_LASTPIXEL_FIX == 1 ? chipMinorFeatures10_SH_IMG_LD_LASTPIXEL_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].ASYNC_BLT == 1 ? chipMinorFeatures10_ASYNC_BLT : 0) |
				(gChipInfo_STM32_4_6_9[i].ASYNC_FE_FENCE_FIX == 1 ? chipMinorFeatures10_ASYNC_FE_FENCE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].PSCS_THROTTLE == 1 ? chipMinorFeatures10_PSCS_THROTTLE : 0) |
				(gChipInfo_STM32_4_6_9[i].SEPARATE_LS == 1 ? chipMinorFeatures10_SEPARATE_LS : 0) |
				(gChipInfo_STM32_4_6_9[i].MCFE == 1 ? chipMinorFeatures10_MCFE : 0) |
				(gChipInfo_STM32_4_6_9[i].WIDELINE_TRIANGLE_EMU == 1 ? chipMinorFeatures10_WIDELINE_TRIANGLE_EMU : 0);

			uint32_t features11 =
				(gChipInfo_STM32_4_6_9[i].VG_RESOLUTION_8K == 1 ? chipMinorFeatures11_VG_RESOLUTION_8K : 0) |
				(gChipInfo_STM32_4_6_9[i].FENCE_32BIT == 1 ? chipMinorFeatures11_FENCE_32BIT : 0) |
				(gChipInfo_STM32_4_6_9[i].FENCE_64BIT == 1 ? chipMinorFeatures11_FENCE_64BIT : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_INTERLEVE8 == 1 ? chipMinorFeatures11_NN_INTERLEVE8 : 0) |
				(gChipInfo_STM32_4_6_9[i].PE_DEPTH_ONLY_OQFIX == 1 ? chipMinorFeatures11_PE_DEPTH_ONLY_OQFIX : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_REORDER == 1 ? chipMinorFeatures11_TP_REORDER : 0);
#if defined EXTENDED_FEATURE_LIST
			(gChipInfo_STM32_4_6_9[i].TX_SEAMLESS_CUBE == 1 ? chipMinorFeatures11_TX_SEAMLESS_CUBE : 0) |
				(gChipInfo_STM32_4_6_9[i].TX_SNORM_SUPPORT == 1 ? chipMinorFeatures11_TX_SNORM_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_SCATTER_GATHER == 1 ? chipMinorFeatures11_SH_SCATTER_GATHER : 0) |
				(gChipInfo_STM32_4_6_9[i].HWMANAGED_LS == 1 ? chipMinorFeatures11_HWMANAGED_LS : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_IMAGE_ENABLE_FIX == 1 ? chipMinorFeatures11_SH_IMAGE_ENABLE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].MSAA_FRAGMENT_OPERATION == 1 ? chipMinorFeatures11_MSAA_FRAGMENT_OPERATION : 0) |
				(gChipInfo_STM32_4_6_9[i].PE_TILE_CACHE_FLUSH_FIX == 1 ? chipMinorFeatures11_PE_TILE_CACHE_FLUSH_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].BLT_YUV_OUTPUT == 1 ? chipMinorFeatures11_BLT_YUV_OUTPUT : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_IO_CG_FIX == 1 ? chipMinorFeatures11_SH_IO_CG_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_ROBUSTNESS_FIX == 1 ? chipMinorFeatures11_SH_ROBUSTNESS_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].USC_ATOMIC_FIX2 == 1 ? chipMinorFeatures11_USC_ATOMIC_FIX2 : 0) |
				(gChipInfo_STM32_4_6_9[i].PE_A8B8G8R8 == 1 ? chipMinorFeatures11_PE_A8B8G8R8 : 0) |
				(gChipInfo_STM32_4_6_9[i].MULTIVIEW_RENDER == 1 ? chipMinorFeatures11_MULTIVIEW_RENDER : 0) |
				(gChipInfo_STM32_4_6_9[i].FE_DRAW_DIRECT == 1 ? chipMinorFeatures11_FE_DRAW_DIRECT : 0) |
				(gChipInfo_STM32_4_6_9[i].TX_VKBORDER_MODE == 1 ? chipMinorFeatures11_TX_VKBORDER_MODE : 0) |
				(gChipInfo_STM32_4_6_9[i].TX_UNNORMALIZED_COORD == 1 ? chipMinorFeatures11_TX_UNNORMALIZED_COORD : 0) |
				(gChipInfo_STM32_4_6_9[i].PA_LINECLIP_FIX == 1 ? chipMinorFeatures11_PA_LINECLIP_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].TX_8bit_UVFrac_ROUNDING_FIX == 1 ? chipMinorFeatures11_TX_8bit_UVFrac_ROUNDING_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].MP_ARCH == 1 ? chipMinorFeatures11_MP_ARCH : 0) |
				(gChipInfo_STM32_4_6_9[i].TX_NO_FIXED_FILTER == 1 ? chipMinorFeatures11_TX_NO_FIXED_FILTER : 0) |
				(gChipInfo_STM32_4_6_9[i].SHARE_Z == 1 ? chipMinorFeatures11_SHARE_Z : 0) |
				(gChipInfo_STM32_4_6_9[i].DE_2D_FAST_CLEAR == 1 ? chipMinorFeatures11_DE_2D_FAST_CLEAR : 0) |
				(gChipInfo_STM32_4_6_9[i].DE_TILESTATUS_ROTATION_FIX == 1 ? chipMinorFeatures11_DE_TILESTATUS_ROTATION_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].TX_CLEAR_PENDING_FIX == 1 ? chipMinorFeatures11_TX_CLEAR_PENDING_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].HI1_L2_CACHE == 1 ? chipMinorFeatures11_HI1_L2_CACHE : 0) |
				(gChipInfo_STM32_4_6_9[i].USC_EVICT_CTRL_FIFO_FLOP_RESET_FIX == 1 ? chipMinorFeatures11_USC_EVICT_CTRL_FIFO_FLOP_RESET_FIX : 0) |
#endif
				(gChipInfo_STM32_4_6_9[i].TP_REORDER == 1 ? chipMinorFeatures11_TP_REORDER : 0);

#if defined EXTENDED_FEATURE_LIST
			uint32_t features12 =
				(gChipInfo_STM32_4_6_9[i].FORMAT_10BIT_CROSS_4K == 1 ? chipMinorFeatures12_FORMAT_10BIT_CROSS_4K : 0) |
				(gChipInfo_STM32_4_6_9[i].FORMAT_P010LSB_I010 == 1 ? chipMinorFeatures12_FORMAT_P010LSB_I010 : 0) |
				(gChipInfo_STM32_4_6_9[i].ENDIAN_CONTROL == 1 ? chipMinorFeatures12_ENDIAN_CONTROL : 0) |
				(gChipInfo_STM32_4_6_9[i].G2D_RGB_PLANAR == 1 ? chipMinorFeatures12_G2D_RGB_PLANAR : 0) |
				(gChipInfo_STM32_4_6_9[i].G2D_RGB_PLANAR_SOURCE == 1 ? chipMinorFeatures12_G2D_RGB_PLANAR_SOURCE : 0) |
				(gChipInfo_STM32_4_6_9[i].G2D_DEC400EX == 1 ? chipMinorFeatures12_G2D_DEC400EX : 0) |
				(gChipInfo_STM32_4_6_9[i].G2D_FC_IN_DEC400EX == 1 ? chipMinorFeatures12_G2D_FC_IN_DEC400EX : 0) |
				(gChipInfo_STM32_4_6_9[i].G2D_NO_YUV420_SOURCE == 1 ? chipMinorFeatures12_G2D_NO_YUV420_SOURCE : 0) |
				(gChipInfo_STM32_4_6_9[i].G2D_YUV420_101010 == 1 ? chipMinorFeatures12_G2D_YUV420_101010 : 0) |
				(gChipInfo_STM32_4_6_9[i].G2D_MultiSrcBlt_Pipe == 1 ? chipMinorFeatures12_G2D_MultiSrcBlt_Pipe : 0) |
				(gChipInfo_STM32_4_6_9[i].G2D_Stretch_MultiSrc_Pipe == 1 ? chipMinorFeatures12_G2D_Stretch_MultiSrc_Pipe : 0) |
				(gChipInfo_STM32_4_6_9[i].G2D_Normalization == 1 ? chipMinorFeatures12_G2D_Normalization : 0) |
				//(gChipInfo_STM32_4_6_9[i].G2D_Normalization_Quantization == 1 ? chipMinorFeatures12_G2D_Normalization_Quantization : 0) |
				(gChipInfo_STM32_4_6_9[i].G2D_FRAME_DONE_INTR == 1 ? chipMinorFeatures12_G2D_FRAME_DONE_INTR : 0) |
				(gChipInfo_STM32_4_6_9[i].G2D_MASK_AND_COLORKEY == 1 ? chipMinorFeatures12_G2D_MASK_AND_COLORKEY : 0) |
				(gChipInfo_STM32_4_6_9[i].AXIFE == 1 ? chipMinorFeatures12_AXIFE : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_VX2_FLOATING_MAD_FIX == 1 ? chipMinorFeatures12_SH_VX2_FLOATING_MAD_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].TS_FC_VULKAN_SUPPORT == 1 ? chipMinorFeatures12_TS_FC_VULKAN_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_9[i].MSAA_FLOAT_64BIT == 1 ? chipMinorFeatures12_MSAA_FLOAT_64BIT : 0) |
				(gChipInfo_STM32_4_6_9[i].INDIRECT_COMPUTE_ZERODIM_FIX == 1 ? chipMinorFeatures12_INDIRECT_COMPUTE_ZERODIM_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].Q_CHANNEL_SUPPORT == 1 ? chipMinorFeatures12_Q_CHANNEL_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_9[i].MMU_PAGE_DESCRIPTOR == 1 ? chipMinorFeatures12_MMU_PAGE_DESCRIPTOR : 0) |
				(gChipInfo_STM32_4_6_9[i].YUV_LINEAR_TO_TILE_ROTATE == 1 ? chipMinorFeatures12_YUV_LINEAR_TO_TILE_ROTATE : 0) |
				(gChipInfo_STM32_4_6_9[i].VEC2_IMULIMAD32_SUPPORT == 1 ? chipMinorFeatures12_VEC2_IMULIMAD32_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_9[i].VEC4_IMULIMAD32_SUPPORT == 1 ? chipMinorFeatures12_VEC4_IMULIMAD32_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_9[i].VEC2_IDIVIMOD16_SUPPORT == 1 ? chipMinorFeatures12_VEC2_IDIVIMOD16_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_9[i].DST_TEX_I2F_F2I_INST_DEPRECATE == 1 ? chipMinorFeatures12_DST_TEX_I2F_F2I_INST_DEPRECATE : 0) |
				(gChipInfo_STM32_4_6_9[i].ALU_FP16_INSTRUCTIONS == 1 ? chipMinorFeatures12_ALU_FP16_INSTRUCTIONS : 0) |
				(gChipInfo_STM32_4_6_9[i].DUAL16_14BIT_PC_SUPPORT == 1 ? chipMinorFeatures12_DUAL16_14BIT_PC_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_9[i].LDST_CONV_4ROUNDING_MODES == 1 ? chipMinorFeatures12_LDST_CONV_4ROUNDING_MODES : 0) |
				(gChipInfo_STM32_4_6_9[i].FULL_PACK_MODE_SUPPORT == 1 ? chipMinorFeatures12_FULL_PACK_MODE_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_9[i].DEPTH_FLOAT32_SUPPORT == 1 ? chipMinorFeatures12_DEPTH_FLOAT32_SUPPORT : 0);

			uint32_t features13 =
				(gChipInfo_STM32_4_6_9[i].GPU_INSPECTOR_COUNTERS == 1 ? chipMinorFeatures13_GPU_INSPECTOR_COUNTERS : 0) |
				(gChipInfo_STM32_4_6_9[i].FP32_TO_FP16_CONV_FIX == 1 ? chipMinorFeatures13_FP32_TO_FP16_CONV_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].IMGLD_COMP_COUNT_FIX == 1 ? chipMinorFeatures13_IMGLD_COMP_COUNT_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].IMGLD_WIDTH_LT16_FIX == 1 ? chipMinorFeatures13_IMGLD_WIDTH_LT16_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].TX_FILTER_ROUND_FIX == 1 ? chipMinorFeatures13_TX_FILTER_ROUND_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_FP32_FMA_SUPPORT == 1 ? chipMinorFeatures13_SH_FP32_FMA_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_9[i].PE_64BPP_LINEAR_FORMAT == 1 ? chipMinorFeatures13_PE_64BPP_LINEAR_FORMAT : 0) |
				(gChipInfo_STM32_4_6_9[i].TX_ETC2_COMPRESSION == 1 ? chipMinorFeatures13_TX_ETC2_COMPRESSION : 0) |
				(gChipInfo_STM32_4_6_9[i].HIGHP_VEC2 == 1 ? chipMinorFeatures13_HIGHP_VEC2 : 0) |
				(gChipInfo_STM32_4_6_9[i].MMU_PD_42_BIT_ADDRESS == 1 ? chipMinorFeatures13_MMU_PD_42_BIT_ADDRESS : 0) |
				(gChipInfo_STM32_4_6_9[i].BLT_ROBUSTNESS_FIX == 1 ? chipMinorFeatures13_BLT_ROBUSTNESS_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].TFB_PERF_FIX == 1 ? chipMinorFeatures13_TFB_PERF_FIX : 0) |
				//(gChipInfo_STM32_4_6_9[i].SH_SUPERSCALAR_ARCH == 1 ? chipMinorFeatures13_SH_SUPERSCALAR_ARCH : 0) | 00
				//(gChipInfo_STM32_4_6_9[i].PA_ZEROAREA_LINE_FIX == 1 ? chipMinorFeatures13_PA_ZEROAREA_LINE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].VG_IMAGE_16K == 1 ? chipMinorFeatures13_VG_IMAGE_16K : 0) |
				(gChipInfo_STM32_4_6_9[i].VG_FORMAT_ARGB2222 == 1 ? chipMinorFeatures13_VG_FORMAT_ARGB2222 : 0) |
				(gChipInfo_STM32_4_6_9[i].VIP_HW_FINAL_RELEASE == 1 ? chipMinorFeatures13_VIP_HW_FINAL_RELEASE : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_SINGLEPORT_ACCUMBUFFER == 1 ? chipMinorFeatures13_NN_SINGLEPORT_ACCUMBUFFER : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_STRIDE_SUPPORT == 1 ? chipMinorFeatures13_NN_STRIDE_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_9[i].SWTILING_PHASE1 == 1 ? chipMinorFeatures13_SWTILING_PHASE1 : 0) |
				(gChipInfo_STM32_4_6_9[i].SWTILING_PHASE2 == 1 ? chipMinorFeatures13_SWTILING_PHASE2 : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_SIMPLE_INT16 == 1 ? chipMinorFeatures13_TP_SIMPLE_INT16 : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_REAL_INT16 == 1 ? chipMinorFeatures13_TP_REAL_INT16 : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_ROI_POOLING == 1 ? chipMinorFeatures13_TP_ROI_POOLING : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_MAX_POOLING_STRIDE1 == 1 ? chipMinorFeatures13_TP_MAX_POOLING_STRIDE1 : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_LRN == 1 ? chipMinorFeatures13_TP_LRN : 0) |
				(gChipInfo_STM32_4_6_9[i].TF_QUANTIZATION == 1 ? chipMinorFeatures13_TF_QUANTIZATION : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_NONZERO_BORDER == 1 ? chipMinorFeatures13_NN_NONZERO_BORDER : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_MIRROR_BORDER == 1 ? chipMinorFeatures13_NN_MIRROR_BORDER : 0) |
				(gChipInfo_STM32_4_6_9[i].AI_GPU == 1 ? chipMinorFeatures13_AI_GPU : 0);

			uint32_t features14 =
				(gChipInfo_STM32_4_6_9[i].TP_TENSOR_ADD_MUL == 1 ? chipMinorFeatures13_TP_TENSOR_ADD_MUL : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_DEPTHWISE_INT16XINT8 == 1 ? chipMinorFeatures13_NN_DEPTHWISE_INT16XINT8 : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_DEPTHWISE_8BIT_VIP_V7 == 1 ? chipMinorFeatures14_NN_DEPTHWISE_8BIT_VIP_V7 : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_SOFTMAX == 1 ? chipMinorFeatures14_TP_SOFTMAX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_23BITS_POST_MULTIPLIER_VIP_V7 == 1 ? chipMinorFeatures14_NN_23BITS_POST_MULTIPLIER_VIP_V7 : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_23BITS_POST_MULTIPLIER_VIP_V7 == 1 ? chipMinorFeatures14_TP_23BITS_POST_MULTIPLIER_VIP_V7 : 0) |
				(gChipInfo_STM32_4_6_9[i].CONV_INT16X8BIT_VIP_V7 == 1 ? chipMinorFeatures14_CONV_INT16X8BIT_VIP_V7 : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_REMOVE_POOLING == 1 ? chipMinorFeatures14_NN_REMOVE_POOLING : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_40BIT_BIAS == 1 ? chipMinorFeatures14_NN_40BIT_BIAS : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_REMOVE_USC == 1 ? chipMinorFeatures14_TP_REMOVE_USC : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_ZDP6 == 1 ? chipMinorFeatures14_NN_ZDP6 : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_XYDP9 == 1 ? chipMinorFeatures14_NN_XYDP9 : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_FIRST_PIXEL_POOLING == 1 ? chipMinorFeatures14_NN_FIRST_PIXEL_POOLING : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_ZDP3 == 1 ? chipMinorFeatures14_NN_ZDP3 : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_XYDP6 == 1 ? chipMinorFeatures14_NN_XYDP6 : 0) |
				(gChipInfo_STM32_4_6_9[i].SWTILING_PHASE3 == 1 ? chipMinorFeatures14_SWTILING_PHASE3 : 0) |
				(gChipInfo_STM32_4_6_9[i].USC_STAY_LRU == 1 ? chipMinorFeatures14_USC_STAY_LRU : 0) |
				(gChipInfo_STM32_4_6_9[i].COEF_COMPRESSION_ENHANCEMENT == 1 ? chipMinorFeatures14_COEF_COMPRESSION_ENHANCEMENT : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_COEF_COMPRESSION_ENHANCEMENT == 1 ? chipMinorFeatures14_TP_COEF_COMPRESSION_ENHANCEMENT : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_COEF_DECOMPRESS_PERF2X == 1 ? chipMinorFeatures14_NN_COEF_DECOMPRESS_PERF2X : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_SMALLBATCH_PHASE1 == 1 ? chipMinorFeatures14_TP_SMALLBATCH_PHASE1 : 0) |
				(gChipInfo_STM32_4_6_9[i].OCB_COUNTER == 1 ? chipMinorFeatures14_OCB_COUNTER : 0) |
				(gChipInfo_STM32_4_6_9[i].SCALER == 1 ? chipMinorFeatures14_SCALER : 0) |
				(gChipInfo_STM32_4_6_9[i].SCALER_4K == 1 ? chipMinorFeatures14_SCALER_4K : 0) |
				(gChipInfo_STM32_4_6_9[i].INPUT_4BIT == 1 ? chipMinorFeatures14_INPUT_4BIT : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_NO_Z_LOCATION_OFFSET == 1 ? chipMinorFeatures14_NN_NO_Z_LOCATION_OFFSET : 0) |
				(gChipInfo_STM32_4_6_9[i].OCB_REMAP_PHYSICAL_ADDRESS == 1 ? chipMinorFeatures14_OCB_REMAP_PHYSICAL_ADDRESS : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_SLOW_OUTPUT == 1 ? chipMinorFeatures14_NN_SLOW_OUTPUT : 0) |
				(gChipInfo_STM32_4_6_9[i].NO_NARROW_POST_PROCESS_PIPE == 1 ? chipMinorFeatures14_NO_NARROW_POST_PROCESS_PIPE : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_NN_PROBE == 1 ? chipMinorFeatures14_TP_NN_PROBE : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_DEPTHWISE_SUPPORT == 1 ? chipMinorFeatures14_NN_DEPTHWISE_SUPPORT : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_XYDP0 == 1 ? chipMinorFeatures14_NN_XYDP0 : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_WRITE_WITHOUT_USC == 1 ? chipMinorFeatures14_NN_WRITE_WITHOUT_USC : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_HW_LIMITATION_NATIVE_KER_1x2_2x1 == 1 ? chipMinorFeatures14_NN_HW_LIMITATION_NATIVE_KER_1x2_2x1 : 0);

			uint32_t features15 =
				(gChipInfo_STM32_4_6_9[i].NN_SMALLBATCH_PHASE1 == 1 ? chipMinorFeatures15_NN_SMALLBATCH_PHASE1 : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_SLICE_PADDING_TO_64BYTE_ALIGN == 1 ? chipMinorFeatures15_NN_SLICE_PADDING_TO_64BYTE_ALIGN : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_DW_1x1_CONV_MERGE == 1 ? chipMinorFeatures15_NN_DW_1x1_CONV_MERGE : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_BFLOAT16 == 1 ? chipMinorFeatures15_TP_BFLOAT16 : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_23BITS_POST_MULTIPLIER == 1 ? chipMinorFeatures15_TP_23BITS_POST_MULTIPLIER : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_TRANSPOSE == 1 ? chipMinorFeatures15_NN_TRANSPOSE : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_ZDP_TRANSPOSE_CH9_ONLY == 1 ? chipMinorFeatures15_NN_ZDP_TRANSPOSE_CH9_ONLY : 0) |
				(gChipInfo_STM32_4_6_9[i].USE_SINGLE_PORT_VIPSRAM == 1 ? chipMinorFeatures15_USE_SINGLE_PORT_VIPSRAM : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_LEAKY_RELU == 1 ? chipMinorFeatures15_NN_LEAKY_RELU : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_PRELU == 1 ? chipMinorFeatures15_NN_PRELU : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_PER_CHANNEL_QUANT == 1 ? chipMinorFeatures15_NN_PER_CHANNEL_QUANT : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_PER_CHANNEL_QUANT_ASYM == 1 ? chipMinorFeatures15_NN_PER_CHANNEL_QUANT_ASYM : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_ASYMMETRIC_INT8 == 1 ? chipMinorFeatures15_NN_ASYMMETRIC_INT8 : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_FLOAT_POST_MULT == 1 ? chipMinorFeatures15_NN_FLOAT_POST_MULT : 0) |
				(gChipInfo_STM32_4_6_9[i].PRELU_LEAKLY_RELU_CLAMP == 1 ? chipMinorFeatures15_PRELU_LEAKLY_RELU_CLAMP : 0) |
				(gChipInfo_STM32_4_6_9[i].TPLITE_BFLOAT16 == 1 ? chipMinorFeatures15_TPLITE_BFLOAT16 : 0) |
				(gChipInfo_STM32_4_6_9[i].PREPROCESS_IMG_BUF_640BYTE_LIMIT == 1 ? chipMinorFeatures15_PREPROCESS_IMG_BUF_640BYTE_LIMIT : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_POST_OUT_SUPPORT_FP16 == 1 ? chipMinorFeatures15_NN_POST_OUT_SUPPORT_FP16 : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_POST_OUT_SUPPORT_BF16 == 1 ? chipMinorFeatures15_NN_POST_OUT_SUPPORT_BF16 : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_POST_OUT_SUPPORT_FP32 == 1 ? chipMinorFeatures15_NN_POST_OUT_SUPPORT_FP32 : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_KERNEL_1BYTE_ALGIN == 1 ? chipMinorFeatures15_TP_KERNEL_1BYTE_ALGIN : 0) |
				(gChipInfo_STM32_4_6_9[i].BFLOAT_COEF_COMPRESSION_ZERO_COEFBIT14_INVERSE == 1 ? chipMinorFeatures15_BFLOAT_COEF_COMPRESSION_ZERO_COEFBIT14_INVERSE : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_COMPRESSION_BYPASSS == 1 ? chipMinorFeatures15_NN_COMPRESSION_BYPASSS : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_3_USC == 1 ? chipMinorFeatures15_TP_3_USC : 0) |
				(gChipInfo_STM32_4_6_9[i].BFP_COEF_AUTO_PAD_INCOMPLETE_ZERO_IN_KZ_PLANE == 1 ? chipMinorFeatures15_BFP_COEF_AUTO_PAD_INCOMPLETE_ZERO_IN_KZ_PLANE : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_NATIVE_STRIDE_TWO == 1 ? chipMinorFeatures15_NN_NATIVE_STRIDE_TWO : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_TENSOR_ADD == 1 ? chipMinorFeatures15_NN_TENSOR_ADD : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_FLOAT32_IO == 1 ? chipMinorFeatures15_TP_FLOAT32_IO : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_SMALL_BATCH_PHASE2 == 1 ? chipMinorFeatures15_NN_SMALL_BATCH_PHASE2 : 0) |
				(gChipInfo_STM32_4_6_9[i].TILE_ACCESS_CAPABILITY == 1 ? chipMinorFeatures15_TILE_ACCESS_CAPABILITY : 0) |
				(gChipInfo_STM32_4_6_9[i].FAST_DP3_PREPROCESSOR == 1 ? chipMinorFeatures15_FAST_DP3_PREPROCESSOR : 0) |
				(gChipInfo_STM32_4_6_9[i].DEPTHWISE_SUPPORT_16BIT_FORMAT == 1 ? chipMinorFeatures15_DEPTHWISE_SUPPORT_16BIT_FORMAT : 0);

			uint32_t features16 =
				(gChipInfo_STM32_4_6_9[i].NN_SUPPORT_ALU == 1 ? chipMinorFeatures16_NN_SUPPORT_ALU : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_ENHANCED_MAX_POOLING == 1 ? chipMinorFeatures16_NN_ENHANCED_MAX_POOLING : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_TRANSPOSE_PHASE2 == 1 ? chipMinorFeatures16_NN_TRANSPOSE_PHASE2 : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_TENSOR_ADD_FIELD_MOVE_TO_EXT_CMD == 1 ? chipMinorFeatures16_NN_TENSOR_ADD_FIELD_MOVE_TO_EXT_CMD : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_CMD_SUPPORT_SLICE == 1 ? chipMinorFeatures16_NN_CMD_SUPPORT_SLICE : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_CONV_CORE_BYPASS == 1 ? chipMinorFeatures16_NN_CONV_CORE_BYPASS : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_TENSOR_ADD_RELU == 1 ? chipMinorFeatures16_NN_TENSOR_ADD_RELU : 0) |
				(gChipInfo_STM32_4_6_9[i].TPLITE_SUPPORT_TP_DATA_TRANSPOSE == 1 ? chipMinorFeatures16_TPLITE_SUPPORT_TP_DATA_TRANSPOSE : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_SUPPORT_CONV_1D == 1 ? chipMinorFeatures16_NN_SUPPORT_CONV_1D : 0) |
				(gChipInfo_STM32_4_6_9[i].USE_VIPSRAM_FOR_KERNEL_STREAMING == 1 ? chipMinorFeatures16_USE_VIPSRAM_FOR_KERNEL_STREAMING : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_SUPPORT_DUMMY_TILE == 1 ? chipMinorFeatures16_NN_SUPPORT_DUMMY_TILE : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_SUPPORT_KERNEL_1BYTE_ALIGN == 1 ? chipMinorFeatures16_NN_SUPPORT_KERNEL_1BYTE_ALIGN : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_1x1_NON_POOLING_PACKING == 1 ? chipMinorFeatures16_NN_1x1_NON_POOLING_PACKING : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_SUPPORT_BOTH_CONV_NATIVE_STRIDE2_AND_POOLING == 1 ? chipMinorFeatures16_NN_SUPPORT_BOTH_CONV_NATIVE_STRIDE2_AND_POOLING : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_SUPPORT_CONV1x1_AND_NATIVE_CONV_STRIDE2 == 1 ? chipMinorFeatures16_NN_SUPPORT_CONV1x1_AND_NATIVE_CONV_STRIDE2 : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_REMOVE_FC == 1 ? chipMinorFeatures16_TP_REMOVE_FC : 0) |
				(gChipInfo_STM32_4_6_9[i].VIP_REMOVE_MMU == 1 ? chipMinorFeatures16_VIP_REMOVE_MMU : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_RD_IMG_NEED_EXTRA_SPACE == 1 ? chipMinorFeatures16_NN_RD_IMG_NEED_EXTRA_SPACE : 0) |
				(gChipInfo_STM32_4_6_9[i].VIP_INDIV_CLK_NN == 1 ? chipMinorFeatures16_VIP_INDIV_CLK_NN : 0) |
				(gChipInfo_STM32_4_6_9[i].VIP_EXPORT_CLK_DIV2 == 1 ? chipMinorFeatures16_VIP_EXPORT_CLK_DIV2 : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_2D_AVERAGE_OUTPUT == 1 ? chipMinorFeatures16_NN_2D_AVERAGE_OUTPUT : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_JOB_CANCELATION == 1 ? chipMinorFeatures16_NN_JOB_CANCELATION : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_SUPPORT_GEMM == 1 ? chipMinorFeatures16_NN_SUPPORT_GEMM : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_SUPPORT_INLINE_NWHC_AND_MATRIX_TRANSPOSE == 1 ? chipMinorFeatures16_NN_SUPPORT_INLINE_NWHC_AND_MATRIX_TRANSPOSE : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_SUPPORT_BATCH == 1 ? chipMinorFeatures16_NN_SUPPORT_BATCH : 0) |
				(gChipInfo_STM32_4_6_9[i].VIP_SUPPORT_DEC == 1 ? chipMinorFeatures16_VIP_SUPPORT_DEC : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_SUPPORT_MULTI_AXI_ID == 1 ? chipMinorFeatures16_NN_SUPPORT_MULTI_AXI_ID : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_POST_OUT_SUPPORT_INT32 == 1 ? chipMinorFeatures16_NN_POST_OUT_SUPPORT_INT32 : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_SUPPORT_STREAMPROCESSOR == 1 ? chipMinorFeatures16_NN_SUPPORT_STREAMPROCESSOR : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_DISTRIBUTED_VIPSRAM == 1 ? chipMinorFeatures16_NN_DISTRIBUTED_VIPSRAM : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_FC_ENHANCEMENT == 1 ? chipMinorFeatures16_NN_FC_ENHANCEMENT : 0);
				(gChipInfo_STM32_4_6_9[i].NN_4BIT_PHASE1 == 1 ? chipMinorFeatures16_NN_4BIT_PHASE1 : 0);

			uint32_t features17 =
				(gChipInfo_STM32_4_6_9[i].VIP_DEC400 == 1 ? chipMinorFeatures17_VIP_DEC400 : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_POST_MULT_SUPPORT_FP_CONV == 1 ? chipMinorFeatures17_NN_POST_MULT_SUPPORT_FP_CONV : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_SUPPORT_16_8_QUANTIZATION == 1 ? chipMinorFeatures17_NN_SUPPORT_16_8_QUANTIZATION : 0) |
				(gChipInfo_STM32_4_6_9[i].SPECIAL_8BIT_SIGN_ABS_CONV == 1 ? chipMinorFeatures17_SPECIAL_8BIT_SIGN_ABS_CONV : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_SUPPORT_CONFIGURABLE_FASTXDP3 == 1 ? chipMinorFeatures17_NN_SUPPORT_CONFIGURABLE_FASTXDP3 : 0) |
				//(gChipInfo_STM32_4_6_9[i].NN_USE_CORE_SHARING_IMGBUF_AND_SEQ_NO_ZEROSKIPPING == 1 ? chipMinorFeatures17_NN_USE_CORE_SHARING_IMGBUF_AND_SEQ_NO_ZEROSKIPPING : 0) |
				//(gChipInfo_STM32_4_6_9[i].SUPPORT_DECONVNxN_S_LESS_THAN_16 == 1 ? chipMinorFeatures17_SUPPORT_DECONVNxN_S_LESS_THAN_16 : 0) |
				//(gChipInfo_STM32_4_6_9[i].NN_PICOCORE_DEPTHWISE == 1 ? chipMinorFeatures17_NN_PICOCORE_DEPTHWISE : 0) |
				(gChipInfo_STM32_4_6_9[i].VIP_SUPPORT_TENSOR_TRANSFER == 1 ? chipMinorFeatures17_VIP_SUPPORT_TENSOR_TRANSFER : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_SUPPORT_CMD_LOOP == 1 ? chipMinorFeatures17_NN_SUPPORT_CMD_LOOP : 0) |
				//(gChipInfo_STM32_4_6_9[i].VIP_SUPPORT_X_FRAME_COMPRESSION == 1 ? chipMinorFeatures17_VIP_SUPPORT_X_FRAME_COMPRESSION : 0) |
				//(gChipInfo_STM32_4_6_9[i].NN_SMALL_ACCUM == 1 ? chipMinorFeatures17_NN_SMALL_ACCUM : 0) |
				//(gChipInfo_STM32_4_6_9[i].NN_SINGLE_POSTMULT_FIELDS_IN_BITSTREAM == 1 ? chipMinorFeatures17_NN_SINGLE_POSTMULT_FIELDS_IN_BITSTREAM : 0) |
				//(gChipInfo_STM32_4_6_9[i].POST_MULTIPLIER_LOW_POWER_MODE == 1 ? chipMinorFeatures17_POST_MULTIPLIER_LOW_POWER_MODE : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_PER3DTILE_BUBBLE_FIX == 1 ? chipMinorFeatures17_NN_PER3DTILE_BUBBLE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_CACHELINE_MODE_PERF_FIX == 1 ? chipMinorFeatures17_NN_CACHELINE_MODE_PERF_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_CONV1x1_PERF_FIX == 1 ? chipMinorFeatures17_NN_CONV1x1_PERF_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_REORDER_FIX == 1 ? chipMinorFeatures17_TP_REORDER_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_CONVOUT_FIFO_DEPTH_FIX == 1 ? chipMinorFeatures17_NN_CONVOUT_FIFO_DEPTH_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_ZXDP3_KERNEL_READ_CONFLICT_FIX == 1 ? chipMinorFeatures17_NN_ZXDP3_KERNEL_READ_CONFLICT_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_ZDP3_NO_COMPRESS_FIX == 1 ? chipMinorFeatures17_NN_ZDP3_NO_COMPRESS_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_ASYNC_COPY_PERF_FIX == 1 ? chipMinorFeatures17_NN_ASYNC_COPY_PERF_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].HI_REORDER_FIX == 1 ? chipMinorFeatures17_HI_REORDER_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].INCORRECT_WR_REQ_TO_USC_BETWEEN_REORDER_AND_NORMAL_LAYER_FIX == 1 ? chipMinorFeatures17_INCORRECT_WR_REQ_TO_USC_BETWEEN_REORDER_AND_NORMAL_LAYER_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_REORDER_LAYER_SUSPEND_FIX == 1 ? chipMinorFeatures17_TP_REORDER_LAYER_SUSPEND_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_ASYNC_COPY_MERGE_FIX == 1 ? chipMinorFeatures17_NN_ASYNC_COPY_MERGE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].USC_INVALIDATE_CACHE_LINE_FIX == 1 ? chipMinorFeatures17_USC_INVALIDATE_CACHE_LINE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_REQ_SLOWARBITRATION_FIX == 1 ? chipMinorFeatures17_NN_REQ_SLOWARBITRATION_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].IMAGE_PARTIAL_CACHE_FIX == 1 ? chipMinorFeatures17_IMAGE_PARTIAL_CACHE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].FULLCACHE_KERNELHEAD_FIX == 1 ? chipMinorFeatures17_FULLCACHE_KERNELHEAD_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_ZDP_INIMAGE_SIZE_FIX == 1 ? chipMinorFeatures17_NN_ZDP_INIMAGE_SIZE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].IDLE_BEFORE_FLUSH_COMPLETE_FIX == 1 ? chipMinorFeatures17_IDLE_BEFORE_FLUSH_COMPLETE_FIX : 0);

			uint32_t features18 =
				(gChipInfo_STM32_4_6_9[i].NO_FLUSH_USC_FIX == 1 ? chipMinorFeatures18_NO_FLUSH_USC_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].SMALL_BATCH_FLOPS_RESET_FIX == 1 ? chipMinorFeatures18_SMALL_BATCH_FLOPS_RESET_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].SMALL_BATCH_DISBLE_FIX == 1 ? chipMinorFeatures18_SMALL_BATCH_DISBLE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].OUTPUT_CONVERT_UINT8_INT8_TO_UINT16_INT16_FIX == 1 ? chipMinorFeatures18_OUTPUT_CONVERT_UINT8_INT8_TO_UINT16_INT16_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].IMAGE_NOT_PACKED_IN_SRAM_FIX == 1 ? chipMinorFeatures18_IMAGE_NOT_PACKED_IN_SRAM_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].COEF_DELTA_CORD_OVERFLOW_ZRL_8BIT_FIX == 1 ? chipMinorFeatures18_COEF_DELTA_CORD_OVERFLOW_ZRL_8BIT_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].USC_INDIVIDUAL_PORT_WRT_EARLY_EVICT_DATA_CORRUPT_FIX == 1 ? chipMinorFeatures18_USC_INDIVIDUAL_PORT_WRT_EARLY_EVICT_DATA_CORRUPT_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].LOW_EFFICIENCY_OF_ID_WRITE_IMGBUF_FIX == 1 ? chipMinorFeatures18_LOW_EFFICIENCY_OF_ID_WRITE_IMGBUF_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].KERNEL_VIP_SRAM_READ_BW_LIMITATION_FIX == 1 ? chipMinorFeatures18_KERNEL_VIP_SRAM_READ_BW_LIMITATION_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].USC_BOTTLENECK_FIX == 1 ? chipMinorFeatures18_USC_BOTTLENECK_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].KERNEL_PER_CORE_LESS_THAN_THIRD_COEF_BUFF_DEPTH_FIX == 1 ? chipMinorFeatures18_KERNEL_PER_CORE_LESS_THAN_THIRD_COEF_BUFF_DEPTH_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_TILE_NUM_BIGGER_THAN_1024_FIX == 1 ? chipMinorFeatures18_NN_TILE_NUM_BIGGER_THAN_1024_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].KERNEL_SIZE_WASTE_IN_PARTIAL_MODE_FIX == 1 ? chipMinorFeatures18_KERNEL_SIZE_WASTE_IN_PARTIAL_MODE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_COMMAND_KERNEL_REQUEST_CONFICT_FIX == 1 ? chipMinorFeatures18_NN_COMMAND_KERNEL_REQUEST_CONFICT_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_REORDER_INTILE_X_SIZE_512_FIX == 1 ? chipMinorFeatures18_TP_REORDER_INTILE_X_SIZE_512_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].IMG_POP_PIPELINE_PAUSE_FIX == 1 ? chipMinorFeatures18_IMG_POP_PIPELINE_PAUSE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].FULLCACHE_KERNEL_INTERLEAVE_FIX == 1 ? chipMinorFeatures18_FULLCACHE_KERNEL_INTERLEAVE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].V8_SINGLE_PORT_ACCUMULATION_BUFFER_RW_CONFICT_ZERO_SKIP_PERF_FIX == 1 ? chipMinorFeatures18_V8_SINGLE_PORT_ACCUMULATION_BUFFER_RW_CONFICT_ZERO_SKIP_PERF_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].V8_ACCUMLATION_READ_OUT_HAS_BUBBLES_PERF_FIX == 1 ? chipMinorFeatures18_V8_ACCUMLATION_READ_OUT_HAS_BUBBLES_PERF_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].DEPTHWISE_NEIGHBOR_IMG_DATA_TRANSFER_NOT_EFFICIENT_FIX == 1 ? chipMinorFeatures18_DEPTHWISE_NEIGHBOR_IMG_DATA_TRANSFER_NOT_EFFICIENT_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].DR_JD_DIFF_CONDITION_FOR_CACHELINE_MODE_PRE_FIX == 1 ? chipMinorFeatures18_DR_JD_DIFF_CONDITION_FOR_CACHELINE_MODE_PRE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_ACCESS_VIPSRAM_OT_IS_ONE_FIX == 1 ? chipMinorFeatures18_TP_ACCESS_VIPSRAM_OT_IS_ONE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].EVIS2_FLOP_RESET_FIX == 1 ? chipMinorFeatures18_EVIS2_FLOP_RESET_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].OUTIMAGE_X_BITWIDTH_LIMIT_FOR_NN_TRANSPOSE_FIX == 1 ? chipMinorFeatures18_OUTIMAGE_X_BITWIDTH_LIMIT_FOR_NN_TRANSPOSE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].USC_ASYNC_CP_RTN_FLOP_RESET_FIX == 1 ? chipMinorFeatures18_USC_ASYNC_CP_RTN_FLOP_RESET_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].IMG_ADDR_NOT_WRAP_IF_OVER_OCB_ADDR_FIX == 1 ? chipMinorFeatures18_IMG_ADDR_NOT_WRAP_IF_OVER_OCB_ADDR_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NEGATIVE_POST_SHIFT_FIX == 1 ? chipMinorFeatures18_NEGATIVE_POST_SHIFT_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].INIMAGE_2DTILE_NOT_LESS_160PIXEL_FIX == 1 ? chipMinorFeatures18_INIMAGE_2DTILE_NOT_LESS_160PIXEL_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].IMG_CAHCE_MODE_MUST_0_IN_IMG_DIRECT_MODE_FIX == 1 ? chipMinorFeatures18_IMG_CAHCE_MODE_MUST_0_IN_IMG_DIRECT_MODE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].BURST_COLLECT_DUMMY_DATA_WASTE_CYCLES_FIX == 1 ? chipMinorFeatures18_BURST_COLLECT_DUMMY_DATA_WASTE_CYCLES_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].INIMG_NOT_64BYTE_ALIGN_CACHELINE_MODE_FIX == 1 ? chipMinorFeatures18_INIMG_NOT_64BYTE_ALIGN_CACHELINE_MODE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_FC_FLOAT_LAST_PIXEL_NEGATIVE_0_FIX == 1 ? chipMinorFeatures18_TP_FC_FLOAT_LAST_PIXEL_NEGATIVE_0_FIX : 0);

			uint32_t features19 =
				(gChipInfo_STM32_4_6_9[i].NN_WASTE_COEF_READ_WRITE_BANDWIDTH_128BYTE_VIPSRAM_IN_FULL_PATIAL_CACHE_MODE_FIX == 1 ? chipMinorFeatures19_NN_WASTE_COEF_READ_WRITE_BANDWIDTH_128BYTE_VIPSRAM_IN_FULL_PATIAL_CACHE_MODE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_IN_TILE_DATA_IS_ALL_PAD_FIX == 1 ? chipMinorFeatures19_NN_IN_TILE_DATA_IS_ALL_PAD_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_TP_INSTR_COMPLETE_IN_SAME_CYCLE_WITH_WAIT_EVENT_FIX == 1 ? chipMinorFeatures19_NN_TP_INSTR_COMPLETE_IN_SAME_CYCLE_WITH_WAIT_EVENT_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].CORE_IMAGE_TRANSER_NOT_EFFICIENT_BETWEEN_PARTITION_FIX == 1 ? chipMinorFeatures19_CORE_IMAGE_TRANSER_NOT_EFFICIENT_BETWEEN_PARTITION_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_FC_KERNEL_STREAM_MUST_LESS_THAN_OR_EQUAL_TO_64BYTE_WHEN_1BYTE_ALGINE_FIX == 1 ? chipMinorFeatures19_TP_FC_KERNEL_STREAM_MUST_LESS_THAN_OR_EQUAL_TO_64BYTE_WHEN_1BYTE_ALGINE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_KERNEL_1x1_NO_PAD_FIX == 1 ? chipMinorFeatures19_NN_KERNEL_1x1_NO_PAD_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_DEPTHWISE_AFTER_16BIT_LAYER_LIMIT_FIX == 1 ? chipMinorFeatures19_NN_DEPTHWISE_AFTER_16BIT_LAYER_LIMIT_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_NOT_FULL_USE_CACHE_LINE_FIX == 1 ? chipMinorFeatures19_TP_NOT_FULL_USE_CACHE_LINE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_MOVAI_MOVAR_UNUSED_COMPONENTS_WRITE_DIRTY_DATA_FIX == 1 ? chipMinorFeatures19_SH_MOVAI_MOVAR_UNUSED_COMPONENTS_WRITE_DIRTY_DATA_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].BURST_COLLECT_CONSUMES_MC_DATA_WIDTH_PER_CYCLE_FIX == 1 ? chipMinorFeatures19_BURST_COLLECT_CONSUMES_MC_DATA_WIDTH_PER_CYCLE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_ASSYM_INT8_FIX == 1 ? chipMinorFeatures19_TP_ASSYM_INT8_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_PAD_SLICE_ERROR_WHEN_TRANSPSE_FIX == 1 ? chipMinorFeatures19_NN_PAD_SLICE_ERROR_WHEN_TRANSPSE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_2ND_IMG_BASE_ADDR_FIX == 1 ? chipMinorFeatures19_NN_2ND_IMG_BASE_ADDR_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_TP_SYSTEM_FIX == 1 ? chipMinorFeatures19_NN_TP_SYSTEM_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_INTILE_YSIZE_128_LIMIT_FIX == 1 ? chipMinorFeatures19_NN_INTILE_YSIZE_128_LIMIT_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].SH_CLOCK_GATOR_IDLE_CONDITON_FIX == 1 ? chipMinorFeatures19_SH_CLOCK_GATOR_IDLE_CONDITON_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_BURST_COLLECTER_LAST_FLAG_FIX == 1 ? chipMinorFeatures19_NN_BURST_COLLECTER_LAST_FLAG_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].V83_CONVERTER_FOR_NEG_0_FIX == 1 ? chipMinorFeatures19_V83_CONVERTER_FOR_NEG_0_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_2ND_IMG_SMALL_3D_TILE_FIX == 1 ? chipMinorFeatures19_NN_2ND_IMG_SMALL_3D_TILE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_TILE_YSIZE_127_LIMITATION_FIX == 1 ? chipMinorFeatures19_NN_TILE_YSIZE_127_LIMITATION_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_CONV_1D_16BIT_FORMAT_INTILE_SIZE_LIMITATION_FIX == 1 ? chipMinorFeatures19_NN_CONV_1D_16BIT_FORMAT_INTILE_SIZE_LIMITATION_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_VIPSRAM_DOUBLE_BUFFER_FIX == 1 ? chipMinorFeatures19_NN_VIPSRAM_DOUBLE_BUFFER_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_JD_DIRECT_MODE_FIX == 1 ? chipMinorFeatures19_NN_JD_DIRECT_MODE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_KERNEL_DIRECT_WRONG_PUSH_FIX == 1 ? chipMinorFeatures19_NN_KERNEL_DIRECT_WRONG_PUSH_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].HI_DEFAULT_ENABLE_REORDER_FIX == 1 ? chipMinorFeatures19_HI_DEFAULT_ENABLE_REORDER_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].V8_DIRECT_MODE_START_ADDR_BIAS_FOR_NEGATIVE_OFFSET_FIX == 1 ? chipMinorFeatures19_V8_DIRECT_MODE_START_ADDR_BIAS_FOR_NEGATIVE_OFFSET_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].V83_INTILESIZE_1X1_10BITS_FIX == 1 ? chipMinorFeatures19_V83_INTILESIZE_1X1_10BITS_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].FASTXDP3_ONLY_IN_DEPTHWISE_FIX == 1 ? chipMinorFeatures19_FASTXDP3_ONLY_IN_DEPTHWISE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].USC_PAUSE_TP_WR_REQ_MORE_THAN_256_CYCLES_FIX == 1 ? chipMinorFeatures19_USC_PAUSE_TP_WR_REQ_MORE_THAN_256_CYCLES_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].DEPTHWISE_FLOAT_FIX == 1 ? chipMinorFeatures19_DEPTHWISE_FLOAT_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_CIRCULAR_BUF_WRAP_ADDRESS_OVERFLOW_FIX == 1 ? chipMinorFeatures19_TP_CIRCULAR_BUF_WRAP_ADDRESS_OVERFLOW_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_CIRCULAR_BUF_WRAP_ADDRESS_OVERFLOW_FIX == 1 ? chipMinorFeatures19_NN_CIRCULAR_BUF_WRAP_ADDRESS_OVERFLOW_FIX : 0);

			uint32_t features20 =
				(gChipInfo_STM32_4_6_9[i].CLOCK_DIV2_FREQ_CHANGE_FIX == 1 ? chipMinorFeatures20_CLOCK_DIV2_FREQ_CHANGE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].SMALL_TILE_TENSOR_ADD_FIX == 1 ? chipMinorFeatures20_SMALL_TILE_TENSOR_ADD_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].DECOMPRESSOR_DEPTHWISE_FLOAT_FIX == 1 ? chipMinorFeatures20_DECOMPRESSOR_DEPTHWISE_FLOAT_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_CIRCULAR_BUF_WRAP_ADDRESS_LESS_FIX == 1 ? chipMinorFeatures20_TP_CIRCULAR_BUF_WRAP_ADDRESS_LESS_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].V83_NUMOFPENDINGTILES_FOR_2NDIMAGE_FIX == 1 ? chipMinorFeatures20_V83_NUMOFPENDINGTILES_FOR_2NDIMAGE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].V83_1ST_CACHE_MODE_VIPSRAM_RD_UPDATE_FIX == 1 ? chipMinorFeatures20_V83_1ST_CACHE_MODE_VIPSRAM_RD_UPDATE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].V83_1ST_KERNEL_STREAM_BUFFER_UPDATE_FIX == 1 ? chipMinorFeatures20_V83_1ST_KERNEL_STREAM_BUFFER_UPDATE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].USC_RW_SAME_CACHELINE_UPDATE_FIX == 1 ? chipMinorFeatures20_USC_RW_SAME_CACHELINE_UPDATE_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_KERNEL_MSS_SBP2_DIRECT_STEAM_STEAM_FIX == 1 ? chipMinorFeatures20_NN_KERNEL_MSS_SBP2_DIRECT_STEAM_STEAM_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].CORE_NUM_OF_KID_FOR_MULTI_LAYER_FIX == 1 ? chipMinorFeatures20_CORE_NUM_OF_KID_FOR_MULTI_LAYER_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].KERNEL_XSIZE_YSIZE_NUM_FIX == 1 ? chipMinorFeatures20_KERNEL_XSIZE_YSIZE_NUM_FIX : 0) |
				//(gChipInfo_STM32_4_6_9[i].NN_1ST_AND_2ND_INIMAGE_RAISE_VIPSRAM_RD_UPDATE_AT_SAME_TIME_FIX == 1 ? chipMinorFeatures20_NN_1ST_AND_2ND_INIMAGE_RAISE_VIPSRAM_RD_UPDATE_AT_SAME_TIME_FIX : 0) |
				//(gChipInfo_STM32_4_6_9[i].NN_1ST_AND_2ND_INIMAGE_RAISE_VIPSRAM_RD_UPDATE_AT_SAME_TIME_PHASE1_FIX == 1 ? chipMinorFeatures20_NN_1ST_AND_2ND_INIMAGE_RAISE_VIPSRAM_RD_UPDATE_AT_SAME_TIME_PHASE1_FIX : 0) |
				//(gChipInfo_STM32_4_6_9[i].NN_1ST_AND_2ND_INIMAGE_RAISE_VIPSRAM_RD_UPDATE_AT_SAME_TIME_PHASE2_FIX == 1 ? chipMinorFeatures20_NN_1ST_AND_2ND_INIMAGE_RAISE_VIPSRAM_RD_UPDATE_AT_SAME_TIME_PHASE2_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_SPECIAL_LIST_PARSER_FIX == 1 ? chipMinorFeatures20_TP_SPECIAL_LIST_PARSER_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].DECOMPRESSOR_TREATS_TOTAL_CORES_AS_ACTIVE_CORES_FIX == 1 ? chipMinorFeatures20_DECOMPRESSOR_TREATS_TOTAL_CORES_AS_ACTIVE_CORES_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].DIRECT_INIMAGE_XSTIDE_LE_13BIT_FIX == 1 ? chipMinorFeatures20_DIRECT_INIMAGE_XSTIDE_LE_13BIT_FIX : 0) |
				//(gChipInfo_STM32_4_6_9[i].SECONDIMG_TILE_SIDEBANFIFO_FIX == 1 ? chipMinorFeatures20_SECONDIMG_TILE_SIDEBANFIFO_FIX : 0) |
				//(gChipInfo_STM32_4_6_9[i].TRSPB2_ENDADDR_EQUAL_SRAMEND_FIX == 1 ? chipMinorFeatures20_TRSPB2_ENDADDR_EQUAL_SRAMEND_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_FP16_ALU == 1 ? chipMinorFeatures20_NN_FP16_ALU : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_INT16_ALU == 1 ? chipMinorFeatures20_NN_INT16_ALU : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_INT8_SCALE == 1 ? chipMinorFeatures20_NN_INT8_SCALE : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_POWER_ISOLATION == 1 ? chipMinorFeatures20_NN_POWER_ISOLATION : 0) |
				(gChipInfo_STM32_4_6_9[i].ZRL_7BIT == 1 ? chipMinorFeatures20_ZRL_7BIT : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_SMALLBATCH == 1 ? chipMinorFeatures20_NN_SMALLBATCH : 0) |
				(gChipInfo_STM32_4_6_9[i].TP_SMALLBATCH == 1 ? chipMinorFeatures20_TP_SMALLBATCH : 0) |
				(gChipInfo_STM32_4_6_9[i].ZRL_8BIT == 1 ? chipMinorFeatures20_ZRL_8BIT : 0) |
				(gChipInfo_STM32_4_6_9[i].DDR_BURST_LEN_256B == 1 ? chipMinorFeatures20_DDR_BURST_LEN_256B : 0) |
				(gChipInfo_STM32_4_6_9[i].XY_OFFSET_LIMITATION_FIX == 1 ? chipMinorFeatures20_XY_OFFSET_LIMITATION_FIX : 0) |
				(gChipInfo_STM32_4_6_9[i].NN_NONZERO_MIRROR_BORDER == 1 ? chipMinorFeatures20_NN_NONZERO_MIRROR_BORDER : 0) |
				(gChipInfo_STM32_4_6_9[i].IMAGE_PARTIAL_CACHE == 1 ? chipMinorFeatures20_IMAGE_PARTIAL_CACHE : 0);
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
			linux_devices[count].minor_features14 = features14;
			linux_devices[count].minor_features15 = features15;
			linux_devices[count].minor_features16 = features16;
			linux_devices[count].minor_features17 = features17;
			linux_devices[count].minor_features18 = features18;
			linux_devices[count].minor_features19 = features19;
			linux_devices[count].minor_features20 = features20;
#endif

#if defined NN_CORE_COUNT
			linux_devices[count].nn_core_count = gChipInfo_STM32_4_6_9[i].NNCoreCount;
#endif
			count++;
		}
	}

	// get device entries from AMLOGIC's gc_features_database.h 6.4.6.2 (2021-04-12)
	n = sizeof(gChipInfo_AMLOGIC) / sizeof(gChipInfo_AMLOGIC[0]);
	for (int i = 0; i < n; i++) {
		bool found = false;

		for (int j = 0; j < device_count; ++j)
		{

			if ((devices[j].chipID == gChipInfo_AMLOGIC[i].chipID)
				&& (devices[j].chipVersion == gChipInfo_AMLOGIC[i].chipVersion)
				&& (devices[j].productID == gChipInfo_AMLOGIC[i].productID)
				&& (devices[j].ecoID == gChipInfo_AMLOGIC[i].ecoID)
				&& (devices[j].customerID == gChipInfo_AMLOGIC[i].customerID)
				)
			{
				found = true;
			}
		}

		if (found == false) {
			devices[count].chipID = gChipInfo_AMLOGIC[i].chipID;
			devices[count].chipVersion = gChipInfo_AMLOGIC[i].chipVersion;
			devices[count].productID = gChipInfo_AMLOGIC[i].productID;
			devices[count].ecoID = gChipInfo_AMLOGIC[i].ecoID;
			devices[count].customerID = gChipInfo_AMLOGIC[i].customerID;

			linux_devices[count].model = gChipInfo_AMLOGIC[i].chipID;
			linux_devices[count].revision = gChipInfo_AMLOGIC[i].chipVersion;
			linux_devices[count].product_id = gChipInfo_AMLOGIC[i].productID;
			linux_devices[count].customer_id = gChipInfo_AMLOGIC[i].customerID;
			linux_devices[count].eco_id = gChipInfo_AMLOGIC[i].ecoID;

			linux_devices[count].stream_count = gChipInfo_AMLOGIC[i].Streams;
			linux_devices[count].register_max = gChipInfo_AMLOGIC[i].TempRegisters;
			linux_devices[count].thread_count = gChipInfo_AMLOGIC[i].ThreadCount;
			linux_devices[count].shader_core_count = gChipInfo_AMLOGIC[i].NumShaderCores;
			linux_devices[count].vertex_cache_size = gChipInfo_AMLOGIC[i].VertexCacheSize;
			linux_devices[count].vertex_output_buffer_size = gChipInfo_AMLOGIC[i].VertexOutputBufferSize;
			linux_devices[count].pixel_pipes = gChipInfo_AMLOGIC[i].NumPixelPipes;
			linux_devices[count].instruction_count = gChipInfo_AMLOGIC[i].InstructionCount;
			linux_devices[count].num_constants = gChipInfo_AMLOGIC[i].NumberOfConstants;
			linux_devices[count].buffer_size = gChipInfo_AMLOGIC[i].BufferSize;
			linux_devices[count].varyings_count = gChipInfo_AMLOGIC[i].VaryingCount;

			uint32_t feature = (gChipInfo_AMLOGIC[i].REG_FastClear == 1 ? chipFeatures_FAST_CLEAR : 0) |
				(gChipInfo_AMLOGIC[i].REG_SpecialAntiAliasing == 1 ? chipFeatures_SPECIAL_ANTI_ALIASING : 0) |
				(gChipInfo_AMLOGIC[i].REG_Pipe3D == 1 ? chipFeatures_PIPE_3D : 0) |
				(gChipInfo_AMLOGIC[i].REG_DXTTextureCompression == 1 ? chipFeatures_DXT_TEXTURE_COMPRESSION : 0) |
				(gChipInfo_AMLOGIC[i].REG_DebugMode == 1 ? chipFeatures_DEBUG_MODE : 0) |
				(gChipInfo_AMLOGIC[i].REG_ZCompression == 1 ? chipFeatures_Z_COMPRESSION : 0) |
				(gChipInfo_AMLOGIC[i].REG_YUV420Filter == 1 ? chipFeatures_YUV420_SCALER : 0) |
				(gChipInfo_AMLOGIC[i].REG_MSAA == 1 ? chipFeatures_MSAA : 0) |
				(gChipInfo_AMLOGIC[i].REG_DC == 1 ? chipFeatures_DC : 0) |
				(gChipInfo_AMLOGIC[i].REG_Pipe2D == 1 ? chipFeatures_PIPE_2D : 0) |
				(gChipInfo_AMLOGIC[i].REG_ETC1TextureCompression == 1 ? chipFeatures_ETC1_TEXTURE_COMPRESSION : 0) |
				(gChipInfo_AMLOGIC[i].REG_FastScaler == 1 ? chipFeatures_FAST_SCALER : 0) |
				(gChipInfo_AMLOGIC[i].REG_HighDynamicRange == 1 ? chipFeatures_HIGH_DYNAMIC_RANGE : 0) |
				(gChipInfo_AMLOGIC[i].REG_YUV420Tiler == 1 ? chipFeatures_YUV420_TILER : 0) |
				(gChipInfo_AMLOGIC[i].REG_ModuleCG == 1 ? chipFeatures_MODULE_CG : 0) |
				(gChipInfo_AMLOGIC[i].REG_MinArea == 1 ? chipFeatures_MIN_AREA : 0) |
				(gChipInfo_AMLOGIC[i].REG_NoEZ == 1 ? chipFeatures_NO_EARLY_Z : 0) |
				(gChipInfo_AMLOGIC[i].REG_No422Texture == 1 ? chipFeatures_NO_422_TEXTURE : 0) |
				(gChipInfo_AMLOGIC[i].REG_BufferInterleaving == 1 ? chipFeatures_BUFFER_INTERLEAVING : 0) |
				(gChipInfo_AMLOGIC[i].REG_ByteWrite2D == 1 ? chipFeatures_BYTE_WRITE_2D : 0) |
				(gChipInfo_AMLOGIC[i].REG_NoScaler == 1 ? chipFeatures_NO_SCALER : 0) |
				(gChipInfo_AMLOGIC[i].REG_YUY2Averaging == 1 ? chipFeatures_YUY2_AVERAGING : 0) |
				(gChipInfo_AMLOGIC[i].REG_HalfPECache == 1 ? chipFeatures_HALF_PE_CACHE : 0) |
				(gChipInfo_AMLOGIC[i].REG_HalfTXCache == 1 ? chipFeatures_HALF_TX_CACHE : 0) |
				(gChipInfo_AMLOGIC[i].REG_YUY2RenderTarget == 1 ? chipFeatures_YUY2_RENDER_TARGET : 0) |
				(gChipInfo_AMLOGIC[i].REG_Mem32BitSupport == 1 ? chipFeatures_MEM32 : 0) |
				(gChipInfo_AMLOGIC[i].REG_PipeVG == 1 ? chipFeatures_PIPE_VG : 0) |
				(gChipInfo_AMLOGIC[i].REG_VGTS == 1 ? chipFeatures_VGTS : 0) |
				(gChipInfo_AMLOGIC[i].REG_FE20 == 1 ? chipFeatures_FE20 : 0) |
				(gChipInfo_AMLOGIC[i].REG_ByteWrite3D == 1 ? chipFeatures_BYTE_WRITE_3D : 0) |
				(gChipInfo_AMLOGIC[i].REG_RsYuvTarget == 1 ? chipFeatures_RS_YUV_TARGET : 0) |
				(gChipInfo_AMLOGIC[i].REG_FE20BitIndex == 1 ? chipFeatures_32_BIT_INDICES : 0);

			uint32_t features0 =
				(gChipInfo_AMLOGIC[i].REG_FlipY == 1 ? chipMinorFeatures0_FLIP_Y : 0) |
				(gChipInfo_AMLOGIC[i].REG_DualReturnBus == 1 ? chipMinorFeatures0_DUAL_RETURN_BUS : 0) |
				(gChipInfo_AMLOGIC[i].REG_EndiannessConfig == 1 ? chipMinorFeatures0_ENDIANNESS_CONFIG : 0) |
				(gChipInfo_AMLOGIC[i].REG_Texture8K == 1 ? chipMinorFeatures0_TEXTURE_8K : 0) |
				(gChipInfo_AMLOGIC[i].REG_CorrectTextureConverter == 1 ? chipMinorFeatures0_CORRECT_TEXTURE_CONVERTER : 0) |
				(gChipInfo_AMLOGIC[i].REG_SpecialMsaaLod == 1 ? chipMinorFeatures0_SPECIAL_MSAA_LOD : 0) |
				(gChipInfo_AMLOGIC[i].REG_FastClearFlush == 1 ? chipMinorFeatures0_FAST_CLEAR_FLUSH : 0) |
				(gChipInfo_AMLOGIC[i].REG_2DPE20 == 1 ? chipMinorFeatures0_2DPE20 : 0) |
				(gChipInfo_AMLOGIC[i].REG_CorrectAutoDisable == 1 ? chipMinorFeatures0_CORRECT_AUTO_DISABLE : 0) |
				(gChipInfo_AMLOGIC[i].REG_Render8K == 1 ? chipMinorFeatures0_RENDERTARGET_8K : 0) |
				(gChipInfo_AMLOGIC[i].REG_TileStatus2Bits == 1 ? chipMinorFeatures0_2BITPERTILE : 0) |
				(gChipInfo_AMLOGIC[i].REG_SeparateTileStatusWhenInterleaved == 1 ? chipMinorFeatures0_SEPARATE_TILE_STATUS_WHEN_INTERLEAVED : 0) |
				(gChipInfo_AMLOGIC[i].REG_SuperTiled32x32 == 1 ? chipMinorFeatures0_SUPER_TILED : 0) |
				(gChipInfo_AMLOGIC[i].REG_VG20 == 1 ? chipMinorFeatures0_VG_20 : 0) |
				(gChipInfo_AMLOGIC[i].REG_TSExtendedCommands == 1 ? chipMinorFeatures0_TS_EXTENDED_COMMANDS : 0) |
				(gChipInfo_AMLOGIC[i].REG_CompressionFifoFixed == 1 ? chipMinorFeatures0_COMPRESSION_FIFO_FIXED : 0) |
#ifdef ESTIMATED_VALUE
				(gChipInfo_AMLOGIC[i].REG_ExtraShaderInstructions0 == 1 ? chipMinorFeatures0_HAS_SIGN_FLOOR_CEIL : 0) | //?
				(gChipInfo_AMLOGIC[i].REG_ExtraShaderInstructions1 == 1 ? chipMinorFeatures0_HAS_SQRT_TRIG : 0) | //?
				(gChipInfo_AMLOGIC[i].REG_DefaultReg0 == 1 ? chipMinorFeatures0_MORE_MINOR_FEATURES : 0) | //?
#endif
				(gChipInfo_AMLOGIC[i].REG_VGFilter == 1 ? chipMinorFeatures0_VG_FILTER : 0) |
				(gChipInfo_AMLOGIC[i].REG_VG21 == 1 ? chipMinorFeatures0_VG_21 : 0) |
				(gChipInfo_AMLOGIC[i].REG_ShaderGetsW == 1 ? chipMinorFeatures0_SHADER_HAS_W : 0) |
				(gChipInfo_AMLOGIC[i].REG_MC20 == 1 ? chipMinorFeatures0_MC20 : 0) |
				(gChipInfo_AMLOGIC[i].REG_ShaderMSAASideband == 1 ? chipMinorFeatures0_MSAA_SIDEBAND : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes0 == 1 ? chipMinorFeatures0_BUG_FIXES0 : 0) |
				(gChipInfo_AMLOGIC[i].REG_VAA == 1 ? chipMinorFeatures0_VAA : 0) |
				(gChipInfo_AMLOGIC[i].REG_BypassInMSAA == 1 ? chipMinorFeatures0_BYPASS_IN_MSAA : 0) |
				(gChipInfo_AMLOGIC[i].REG_HierarchicalZ == 1 ? chipMinorFeatures0_HZ : 0) |
				(gChipInfo_AMLOGIC[i].REG_NewTexture == 1 ? chipMinorFeatures0_NEW_TEXTURE : 0) |
				(gChipInfo_AMLOGIC[i].REG_A8TargetSupport == 1 ? chipMinorFeatures0_2D_A8_TARGET : 0) |
				(gChipInfo_AMLOGIC[i].REG_CorrectStencil == 1 ? chipMinorFeatures0_CORRECT_STENCIL : 0) |
				(gChipInfo_AMLOGIC[i].REG_EnhanceVR == 1 ? chipMinorFeatures0_ENHANCE_VR : 0);

			uint32_t features1 =
				(gChipInfo_AMLOGIC[i].REG_RSUVSwizzle == 1 ? chipMinorFeatures1_RSUV_SWIZZLE : 0) |
				(gChipInfo_AMLOGIC[i].REG_V2Compression == 1 ? chipMinorFeatures1_V2_COMPRESSION : 0) |
				(gChipInfo_AMLOGIC[i].REG_VGDoubleBuffer == 1 ? chipMinorFeatures1_VG_DOUBLE_BUFFER : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes1 == 1 ? chipMinorFeatures1_EXTRA_EVENT_STATES : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes2 == 1 ? chipMinorFeatures1_NO_STRIPING_NEEDED : 0) |
				(gChipInfo_AMLOGIC[i].REG_TextureStride == 1 ? chipMinorFeatures1_TEXTURE_STRIDE : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes3 == 1 ? chipMinorFeatures1_BUG_FIXES3 : 0) |
				(gChipInfo_AMLOGIC[i].REG_CorrectAutoDisable1 == 1 ? chipMinorFeatures1_AUTO_DISABLE : 0) |
				(gChipInfo_AMLOGIC[i].REG_AutoRestartTS == 1 ? chipMinorFeatures1_AUTO_RESTART_TS : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes4 == 1 ? chipMinorFeatures1_DISABLE_PE_GATING : 0) |
				(gChipInfo_AMLOGIC[i].REG_L2Windowing == 1 ? chipMinorFeatures1_L2_WINDOWING : 0) |
				(gChipInfo_AMLOGIC[i].REG_HalfFloatPipe == 1 ? chipMinorFeatures1_HALF_FLOAT : 0) |
				(gChipInfo_AMLOGIC[i].REG_PixelDither == 1 ? chipMinorFeatures1_PIXEL_DITHER : 0) |
				(gChipInfo_AMLOGIC[i].REG_TwoStencilReference == 1 ? chipMinorFeatures1_TWO_STENCIL_REFERENCE : 0) |
				(gChipInfo_AMLOGIC[i].REG_ExtendedPixelFormat == 1 ? chipMinorFeatures1_EXTENDED_PIXEL_FORMAT : 0) |
				(gChipInfo_AMLOGIC[i].REG_CorrectMinMaxDepth == 1 ? chipMinorFeatures1_CORRECT_MIN_MAX_DEPTH : 0) |
				(gChipInfo_AMLOGIC[i].REG_DitherAndFilterPlusAlpha2D == 1 ? chipMinorFeatures1_2D_DITHER : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes5 == 1 ? chipMinorFeatures1_BUG_FIXES5 : 0) |
				(gChipInfo_AMLOGIC[i].REG_New2D == 1 ? chipMinorFeatures1_NEW_2D : 0) |
				(gChipInfo_AMLOGIC[i].REG_NewFloatingPointArithmetic == 1 ? chipMinorFeatures1_NEW_FP : 0) |
				(gChipInfo_AMLOGIC[i].REG_TextureHorizontalAlignmentSelect == 1 ? chipMinorFeatures1_TEXTURE_HALIGN : 0) |
				(gChipInfo_AMLOGIC[i].REG_NonPowerOfTwo == 1 ? chipMinorFeatures1_NON_POWER_OF_TWO : 0) |
				(gChipInfo_AMLOGIC[i].REG_LinearTextureSupport == 1 ? chipMinorFeatures1_LINEAR_TEXTURE_SUPPORT : 0) |
				(gChipInfo_AMLOGIC[i].REG_Halti0 == 1 ? chipMinorFeatures1_HALTI0 : 0) |
				(gChipInfo_AMLOGIC[i].REG_CorrectOverflowVG == 1 ? chipMinorFeatures1_CORRECT_OVERFLOW_VG : 0) |
				(gChipInfo_AMLOGIC[i].REG_NegativeLogFix == 1 ? chipMinorFeatures1_NEGATIVE_LOG_FIX : 0) |
				(gChipInfo_AMLOGIC[i].REG_ResolveOffset == 1 ? chipMinorFeatures1_RESOLVE_OFFSET : 0) |
				(gChipInfo_AMLOGIC[i].REG_OkToGateAxiClock == 1 ? chipMinorFeatures1_OK_TO_GATE_AXI_CLOCK : 0) |
				(gChipInfo_AMLOGIC[i].REG_MMU == 1 ? chipMinorFeatures1_MMU_VERSION : 0) |
				(gChipInfo_AMLOGIC[i].REG_WideLine == 1 ? chipMinorFeatures1_WIDE_LINE : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes6 == 1 ? chipMinorFeatures1_BUG_FIXES6 : 0) |
				(gChipInfo_AMLOGIC[i].REG_FcFlushStall == 1 ? chipMinorFeatures1_FC_FLUSH_STALL : 0);

			uint32_t features2 = (gChipInfo_AMLOGIC[i].REG_LineLoop == 1 ? chipMinorFeatures2_LINE_LOOP : 0) |
				(gChipInfo_AMLOGIC[i].REG_LogicOp == 1 ? chipMinorFeatures2_LOGIC_OP : 0) |
				(gChipInfo_AMLOGIC[i].REG_SeamlessCubeMap == 1 ? chipMinorFeatures2_SEAMLESS_CUBE_MAP : 0) |
				(gChipInfo_AMLOGIC[i].REG_SuperTiledTexture == 1 ? chipMinorFeatures2_SUPERTILED_TEXTURE : 0) |
				(gChipInfo_AMLOGIC[i].REG_LinearPE == 1 ? chipMinorFeatures2_LINEAR_PE : 0) |
				(gChipInfo_AMLOGIC[i].REG_RectPrimitive == 1 ? chipMinorFeatures2_RECT_PRIMITIVE : 0) |
				(gChipInfo_AMLOGIC[i].REG_Composition == 1 ? chipMinorFeatures2_COMPOSITION : 0) |
				(gChipInfo_AMLOGIC[i].REG_CorrectAutoDisableCountWidth == 1 ? chipMinorFeatures2_CORRECT_AUTO_DISABLE_COUNT : 0) |
				(gChipInfo_AMLOGIC[i].REG_PESwizzle == 1 ? chipMinorFeatures2_PE_SWIZZLE : 0) |
				(gChipInfo_AMLOGIC[i].REG_EndEvent == 1 ? chipMinorFeatures2_END_EVENT : 0) |
				(gChipInfo_AMLOGIC[i].REG_S1S8 == 1 ? chipMinorFeatures2_S1S8 : 0) |
				(gChipInfo_AMLOGIC[i].REG_Halti1 == 1 ? chipMinorFeatures2_HALTI1 : 0) |
				(gChipInfo_AMLOGIC[i].REG_RGB888 == 1 ? chipMinorFeatures2_RGB888 : 0) |
				(gChipInfo_AMLOGIC[i].REG_TX_YUVAssembler == 1 ? chipMinorFeatures2_TX__YUV_ASSEMBLER : 0) |
				(gChipInfo_AMLOGIC[i].REG_DynamicFrequencyScaling == 1 ? chipMinorFeatures2_DYNAMIC_FREQUENCY_SCALING : 0) |
				(gChipInfo_AMLOGIC[i].REG_TXFilter == 1 ? chipMinorFeatures2_TX_FILTER : 0) |
				(gChipInfo_AMLOGIC[i].REG_FullDirectFB == 1 ? chipMinorFeatures2_FULL_DIRECTFB : 0) |
				(gChipInfo_AMLOGIC[i].REG_OnePass2DFilter == 1 ? chipMinorFeatures2_2D_TILING : 0) |
				(gChipInfo_AMLOGIC[i].REG_ThreadWalkerInPS == 1 ? chipMinorFeatures2_THREAD_WALKER_IN_PS : 0) |
				(gChipInfo_AMLOGIC[i].REG_TileFiller == 1 ? chipMinorFeatures2_TILE_FILLER : 0) |
				(gChipInfo_AMLOGIC[i].REG_YUVStandard == 1 ? chipMinorFeatures2_YUV_STANDARD : 0) |
				(gChipInfo_AMLOGIC[i].REG_MultiSourceBlt == 1 ? chipMinorFeatures2_2D_MULTI_SOURCE_BLIT : 0) |
				(gChipInfo_AMLOGIC[i].REG_YUVConversion == 1 ? chipMinorFeatures2_YUV_CONVERSION : 0) |
				(gChipInfo_AMLOGIC[i].REG_FlushFixed2D == 1 ? chipMinorFeatures2_FLUSH_FIXED_2D : 0) |
				(gChipInfo_AMLOGIC[i].REG_Interleaver == 1 ? chipMinorFeatures2_INTERLEAVER : 0) |
				(gChipInfo_AMLOGIC[i].REG_MixedStreams == 1 ? chipMinorFeatures2_MIXED_STREAMS : 0) |
				(gChipInfo_AMLOGIC[i].REG_L2CacheFor2D420 == 1 ? chipMinorFeatures2_2D_420_L2CACHE : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes7 == 1 ? chipMinorFeatures2_BUG_FIXES7 : 0) |
				(gChipInfo_AMLOGIC[i].REG_NoIndexPattern == 1 ? chipMinorFeatures2_2D_NO_INDEX8_BRUSH : 0) |
				(gChipInfo_AMLOGIC[i].REG_TextureTileStatus == 1 ? chipMinorFeatures2_TEXTURE_TILED_READ : 0) |
				(gChipInfo_AMLOGIC[i].REG_DecompressZ16 == 1 ? chipMinorFeatures2_DECOMPRESS_Z16 : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes8 == 1 ? chipMinorFeatures2_BUG_FIXES8 : 0);

			uint32_t features3 = (gChipInfo_AMLOGIC[i].REG_DERotationStallFix == 1 ? chipMinorFeatures3_ROTATION_STALL_FIX : 0) |
				(gChipInfo_AMLOGIC[i].REG_OclOnly == 1 ? chipMinorFeatures3_OCL_ONLY : 0) |
#ifdef ESTIMATED_VALUE
				(gChipInfo_AMLOGIC[i].REG_NewFeatures0 == 1 ? chipMinorFeatures3_2D_MULTI_SOURCE_BLT_EX : 0) |
#endif
				(gChipInfo_AMLOGIC[i].REG_InstructionCache == 1 ? chipMinorFeatures3_INSTRUCTION_CACHE : 0) |
				(gChipInfo_AMLOGIC[i].REG_GeometryShader == 1 ? chipMinorFeatures3_GEOMETRY_SHADER : 0) |
				(gChipInfo_AMLOGIC[i].REG_TexCompressionSupertiled == 1 ? chipMinorFeatures3_TEX_COMPRESSION_SUPERTILED : 0) |
				(gChipInfo_AMLOGIC[i].REG_Generics == 1 ? chipMinorFeatures3_GENERICS : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes9 == 1 ? chipMinorFeatures3_BUG_FIXES9 : 0) |
				(gChipInfo_AMLOGIC[i].REG_FastMSAA == 1 ? chipMinorFeatures3_FAST_MSAA : 0) |
				(gChipInfo_AMLOGIC[i].REG_WClip == 1 ? chipMinorFeatures3_WCLIP : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes10 == 1 ? chipMinorFeatures3_BUG_FIXES10 : 0) |
				(gChipInfo_AMLOGIC[i].REG_UnifiedSamplers == 1 ? chipMinorFeatures3_UNIFIED_SAMPLERS : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes11 == 1 ? chipMinorFeatures3_BUG_FIXES11 : 0) |
				(gChipInfo_AMLOGIC[i].REG_PerformanceCounters == 1 ? chipMinorFeatures3_PERFORMANCE_COUNTERS : 0) |
				(gChipInfo_AMLOGIC[i].REG_ExtraShaderInstructions2 == 1 ? chipMinorFeatures3_HAS_FAST_TRANSCENDENTALS : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes12 == 1 ? chipMinorFeatures3_BUG_FIXES12 : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes13 == 1 ? chipMinorFeatures3_BUG_FIXES13 : 0) |
				(gChipInfo_AMLOGIC[i].REG_DEEnhancements1 == 1 ? chipMinorFeatures3_DE_ENHANCEMENTS1 : 0) |
				(gChipInfo_AMLOGIC[i].REG_ACE == 1 ? chipMinorFeatures3_ACE : 0) |
				(gChipInfo_AMLOGIC[i].REG_TXEnhancements1 == 1 ? chipMinorFeatures3_TX_ENHANCEMENTS1 : 0) |
				(gChipInfo_AMLOGIC[i].REG_SHEnhancements1 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS1 : 0) |
				(gChipInfo_AMLOGIC[i].REG_SHEnhancements2 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS2 : 0) |
				(gChipInfo_AMLOGIC[i].REG_PEEnhancements1 == 1 ? chipMinorFeatures3_PE_ENHANCEMENTS1 : 0) |
				(gChipInfo_AMLOGIC[i].REG_DEEnhancements2 == 1 ? chipMinorFeatures3_2D_FC_SOURCE : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes14 == 1 ? chipMinorFeatures3_BUG_FIXES_14 : 0) |
				(gChipInfo_AMLOGIC[i].REG_PowerOptimizations0 == 1 ? chipMinorFeatures3_POWER_OPTIMIZATIONS_0 : 0) |
				(gChipInfo_AMLOGIC[i].REG_NewHZ == 1 ? chipMinorFeatures3_NEW_HZ : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes15 == 1 ? chipMinorFeatures3_PE_DITHER_FIX : 0) |
				(gChipInfo_AMLOGIC[i].REG_DEEnhancements3 == 1 ? chipMinorFeatures3_DE_ENHANCEMENTS3 : 0) |
				(gChipInfo_AMLOGIC[i].REG_SHEnhancements3 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS3 : 0) |
				(gChipInfo_AMLOGIC[i].REG_SHEnhancements4 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS4 : 0) |
				(gChipInfo_AMLOGIC[i].REG_TXEnhancements2 == 1 ? chipMinorFeatures3_TX_ENHANCEMENTS2 : 0);

			uint32_t features4 = (gChipInfo_AMLOGIC[i].REG_FEEnhancements1 == 1 ? chipMinorFeatures4_FE_ENHANCEMENTS1 : 0) |
				(gChipInfo_AMLOGIC[i].REG_PEEnhancements2 == 1 ? chipMinorFeatures4_PE_ENHANCEMENTS2 : 0) |
				(gChipInfo_AMLOGIC[i].REG_PAEnhancements1 == 1 ? chipMinorFeatures4_FRUSTUM_CLIP_FIX : 0) |
				(gChipInfo_AMLOGIC[i].REG_DENoGamma == 1 ? chipMinorFeatures4_DE_NO_GAMMA : 0) |
				(gChipInfo_AMLOGIC[i].REG_PAEnhancements2 == 1 ? chipMinorFeatures4_PA_ENHANCEMENTS_2 : 0) |
				(gChipInfo_AMLOGIC[i].REG_DEEnhancements4 == 1 ? chipMinorFeatures4_2D_GAMMA : 0) |
				(gChipInfo_AMLOGIC[i].REG_PEEnhancements3 == 1 ? chipMinorFeatures4_SINGLE_BUFFER : 0) |
				(gChipInfo_AMLOGIC[i].REG_HIEnhancements1 == 1 ? chipMinorFeatures4_HI_ENHANCEMENTS_1 : 0) |
				(gChipInfo_AMLOGIC[i].REG_TXEnhancements3 == 1 ? chipMinorFeatures4_TX_ENHANCEMENTS_3 : 0) |
				(gChipInfo_AMLOGIC[i].REG_SHEnhancements5 == 1 ? chipMinorFeatures4_SH_ENHANCEMENTS_5 : 0) |
				(gChipInfo_AMLOGIC[i].REG_FEEnhancements2 == 1 ? chipMinorFeatures4_FE_ENHANCEMENTS_2 : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes16 == 1 ? chipMinorFeatures4_TX_LERP_PRECISION_FIX : 0) |
				(gChipInfo_AMLOGIC[i].REG_DEEnhancements5 == 1 ? chipMinorFeatures4_2D_COLOR_SPACE_CONVERSION : 0) |
				(gChipInfo_AMLOGIC[i].REG_TXEnhancements4 == 1 ? chipMinorFeatures4_TEXTURE_ASTC : 0) |
				(gChipInfo_AMLOGIC[i].REG_PEEnhancements4 == 1 ? chipMinorFeatures4_PE_ENHANCEMENTS_4 : 0) |
				(gChipInfo_AMLOGIC[i].REG_MCEnhancements1 == 1 ? chipMinorFeatures4_MC_ENHANCEMENTS_1 : 0) |
				(gChipInfo_AMLOGIC[i].REG_Halti2 == 1 ? chipMinorFeatures4_HALTI2 : 0) |
				(gChipInfo_AMLOGIC[i].REG_DEMirrorRotate == 1 ? chipMinorFeatures4_2D_MIRROR_EXTENSION : 0) |
				(gChipInfo_AMLOGIC[i].REG_SmallMSAA == 1 ? chipMinorFeatures4_SMALL_MSAA : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes17 == 1 ? chipMinorFeatures4_BUG_FIXES_17 : 0) |
				(gChipInfo_AMLOGIC[i].REG_Rasterizer2 == 1 ? chipMinorFeatures4_NEW_RA : 0) |
				(gChipInfo_AMLOGIC[i].REG_DualPipeOPF == 1 ? chipMinorFeatures4_2D_OPF_YUV_OUTPUT : 0) |
				(gChipInfo_AMLOGIC[i].REG_MultiSrcV2 == 1 ? chipMinorFeatures4_2D_MULTI_SOURCE_BLT_EX2 : 0) |
				(gChipInfo_AMLOGIC[i].REG_CSCV2 == 1 ? chipMinorFeatures4_NO_USER_CSC : 0) |
				(gChipInfo_AMLOGIC[i].REG_PAEnhancements3 == 1 ? chipMinorFeatures4_ZFIXES : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes18 == 1 ? chipMinorFeatures4_BUG_FIXES18 : 0) |
				(gChipInfo_AMLOGIC[i].REG_Compression2D == 1 ? chipMinorFeatures4_2D_COMPRESSION : 0) |
				(gChipInfo_AMLOGIC[i].REG_Probe == 1 ? chipMinorFeatures4_PROBE : 0) |
				(gChipInfo_AMLOGIC[i].REG_MediumPrecision == 1 ? chipMinorFeatures4_MEDIUM_PRECISION : 0) |
				(gChipInfo_AMLOGIC[i].REG_DESupertile == 1 ? chipMinorFeatures4_2D_SUPER_TILE_VERSION : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes19 == 1 ? chipMinorFeatures4_BUG_FIXES19 : 0) |
				(gChipInfo_AMLOGIC[i].REG_SHEnhancements6 == 1 ? chipMinorFeatures4_SH_ENHANCEMENTS6 : 0);

			uint32_t features5 = (gChipInfo_AMLOGIC[i].REG_SHEnhancements7 == 1 ? chipMinorFeatures5_SH_ENHANCEMENTS7 : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes20 == 1 ? chipMinorFeatures5_BUG_FIXES20 : 0) |
				(gChipInfo_AMLOGIC[i].REG_DEAddress40 == 1 ? chipMinorFeatures5_DE_ADDRESS_40 : 0) |
				(gChipInfo_AMLOGIC[i].REG_MiniMMUFix == 1 ? chipMinorFeatures5_MINI_MMU_FIX : 0) |
				(gChipInfo_AMLOGIC[i].REG_EEZ == 1 ? chipMinorFeatures5_EEZ : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes21 == 1 ? chipMinorFeatures5_BUG_FIXES21 : 0) |
				(gChipInfo_AMLOGIC[i].REG_ExtraVgCaps == 1 ? chipMinorFeatures5_EXTRA_VG_CAPS : 0) |
				(gChipInfo_AMLOGIC[i].REG_MultiSrcV15 == 1 ? chipMinorFeatures5_MULTI_SRC_V15 : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes22 == 1 ? chipMinorFeatures5_BUG_FIXES22 : 0) |
				(gChipInfo_AMLOGIC[i].REG_Halti3 == 1 ? chipMinorFeatures5_HALTI3 : 0) |
				(gChipInfo_AMLOGIC[i].REG_TessellationShaders == 1 ? chipMinorFeatures5_TESSELATION_SHADERS : 0) |
				(gChipInfo_AMLOGIC[i].REG_OnePass2DFilter == 1 ? chipMinorFeatures5_2D_ONE_PASS_FILTER_TAP : 0) |
				(gChipInfo_AMLOGIC[i].REG_MultiSrcV2StrQuad == 1 ? chipMinorFeatures5_MULTI_SRC_V2_STR_QUAD : 0) |
				(gChipInfo_AMLOGIC[i].REG_SeperateSRCAndDstCache == 1 ? chipMinorFeatures5_SEPARATE_SRC_DST : 0) |
				(gChipInfo_AMLOGIC[i].REG_Halti4 == 1 ? chipMinorFeatures5_HALTI4 : 0) |
				(gChipInfo_AMLOGIC[i].REG_RAWriteDepth == 1 ? chipMinorFeatures5_RA_WRITE_DEPTH : 0) |
				(gChipInfo_AMLOGIC[i].REG_AndroidOnly == 1 ? chipMinorFeatures5_ANDROID_ONLY : 0) |
				(gChipInfo_AMLOGIC[i].REG_HasChipProductReg == 1 ? chipMinorFeatures5_HAS_PRODUCTID : 0) |
				(gChipInfo_AMLOGIC[i].REG_TXSupportDEC == 1 ? chipMinorFeatures5_TX_SUPPORT_DEC : 0) |
				(gChipInfo_AMLOGIC[i].REG_S8MSAACompression == 1 ? chipMinorFeatures5_S8_MSAA_COMPRESSION : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixesIn544 == 1 ? chipMinorFeatures5_PE_DITHER_FIX2 : 0) |
				(gChipInfo_AMLOGIC[i].REG_L2CacheRemove == 1 ? chipMinorFeatures5_L2_CACHE_REMOVE : 0) |
				(gChipInfo_AMLOGIC[i].REG_FEAllowRndVtxCnt == 1 ? chipMinorFeatures5_FE_ALLOW_RND_VTX_CNT : 0) |
				(gChipInfo_AMLOGIC[i].REG_CubeMapFL28 == 1 ? chipMinorFeatures5_CUBE_MAP_FL28 : 0) |
				(gChipInfo_AMLOGIC[i].REG_TX6bitFrac == 1 ? chipMinorFeatures5_TX_6BIT_FRAC : 0) |
				(gChipInfo_AMLOGIC[i].REG_FEAllowStallPrefetchEng == 1 ? chipMinorFeatures5_FE_ALLOW_STALL_PREFETCH_ENG : 0) |
				(gChipInfo_AMLOGIC[i].REG_ThirdPartyCompression == 1 ? chipMinorFeatures5_THIRD_PARTY_COMPRESSION : 0) |
				(gChipInfo_AMLOGIC[i].REG_RSS8 == 1 ? chipMinorFeatures5_RS_DEPTHSTENCIL_NATIVE_SUPPORT : 0) |
				(gChipInfo_AMLOGIC[i].REG_MSAACoherencyCheck == 1 ? chipMinorFeatures5_V2_MSAA_COMP_FIX : 0) |
				(gChipInfo_AMLOGIC[i].REG_Halti5 == 1 ? chipMinorFeatures5_HALTI5 : 0) |
				(gChipInfo_AMLOGIC[i].REG_Evis == 1 ? chipMinorFeatures5_EVIS : 0) |
				(gChipInfo_AMLOGIC[i].REG_BltEngine == 1 ? chipMinorFeatures5_BLT_ENGINE : 0);

			uint32_t features6 = (gChipInfo_AMLOGIC[i].REG_BugFixes23 == 1 ? chipMinorFeatures6_BUG_FIXES_23 : 0) |
				(gChipInfo_AMLOGIC[i].REG_BugFixes24 == 1 ? chipMinorFeatures6_BUG_FIXES_24 : 0) |
				(gChipInfo_AMLOGIC[i].REG_DEC == 1 ? chipMinorFeatures6_DEC : 0) |
				(gChipInfo_AMLOGIC[i].REG_VSTileNV12 == 1 ? chipMinorFeatures6_VS_TILE_NV12 : 0) |
				(gChipInfo_AMLOGIC[i].REG_VSTileNV12_10BIT == 1 ? chipMinorFeatures6_VS_TILE_NV12_10BIT : 0) |
				//(gChipInfo_AMLOGIC[i].REG_DisableVIP == 1 ?  : 0) |
				(gChipInfo_AMLOGIC[i].RenderTarget8 == 1 ? chipMinorFeatures6_RENDER_TARGET_8 : 0) |
				(gChipInfo_AMLOGIC[i].TxLodFlowCorrection == 1 ? chipMinorFeatures6_TEX_LOD_FLOW_CORR : 0) |
				(gChipInfo_AMLOGIC[i].FaceLod == 1 ? chipMinorFeatures6_FACE_LOD : 0) |
				(gChipInfo_AMLOGIC[i].MultiCoreSemaphoreStallV2 == 1 ? chipMinorFeatures6_MULTI_CORE_SEMAPHORE_STALL_V2 : 0) |
				(gChipInfo_AMLOGIC[i].VMSAA == 1 ? chipMinorFeatures6_VMSAA : 0) |
				(gChipInfo_AMLOGIC[i].ChipEnableLink == 1 ? chipMinorFeatures6_CHIP_ENABLE_LINK : 0) |
				(gChipInfo_AMLOGIC[i].MULTI_SRC_BLT_1_5_ENHANCEMENT == 1 ? chipMinorFeatures6_MULTI_SRC_BLT_1_5_ENHANCEMENT : 0) |
				(gChipInfo_AMLOGIC[i].MULTI_SRC_BLT_BILINEAR_FILTER == 1 ? chipMinorFeatures6_MULTI_SRC_BLT_BILINEAR_FILTER : 0) |
				(gChipInfo_AMLOGIC[i].RA_HZEZ_CLOCK_CONTROL == 1 ? chipMinorFeatures6_RA_HZEZ_CLOCK_CONTROL : 0) |
				(gChipInfo_AMLOGIC[i].CACHE128B256BPERLINE == 1 ? chipMinorFeatures6_CACHE128B256BPERLINE : 0) |
				(gChipInfo_AMLOGIC[i].V4Compression == 1 ? chipMinorFeatures6_V4_COMPRESSION : 0) |
				//(gChipInfo_AMLOGIC[i].DE2D_MAJOR_SUPER_TILE == 1 ?  : 0) | //?
				(gChipInfo_AMLOGIC[i].PE2D_MAJOR_SUPER_TILE == 1 ? chipMinorFeatures6_PE2D_MAJOR_SUPER_TILE : 0) |
				(gChipInfo_AMLOGIC[i].PE_32BPC_COLORMASK_FIX == 1 ? chipMinorFeatures6_PE_32BPC_COLORMASK_FIX : 0) |
				(gChipInfo_AMLOGIC[i].ALPHA_BLENDING_OPT == 1 ? chipMinorFeatures6_ALPHA_BLENDING_OPT : 0) |
				(gChipInfo_AMLOGIC[i].NEW_GPIPE == 1 ? chipMinorFeatures6_NEW_GPIPE : 0) |
				(gChipInfo_AMLOGIC[i].PIPELINE_32_ATTRIBUTES == 1 ? chipMinorFeatures6_PIPELINE_32_ATTRIBUTES : 0) |
				(gChipInfo_AMLOGIC[i].MSAA_SHADING == 1 ? chipMinorFeatures6_MSAA_SHADING : 0) |
				(gChipInfo_AMLOGIC[i].NO_ANISTRO_FILTER == 1 ? chipMinorFeatures6_NO_ANISTRO_FILTER : 0) |
				(gChipInfo_AMLOGIC[i].NO_ASTC == 1 ? chipMinorFeatures6_NO_ASTC : 0) |
				(gChipInfo_AMLOGIC[i].NO_DXT == 1 ? chipMinorFeatures6_NO_DXT : 0) |
				(gChipInfo_AMLOGIC[i].HWTFB == 1 ? chipMinorFeatures6_HWTFB : 0) |
				(gChipInfo_AMLOGIC[i].RA_DEPTH_WRITE_MSAA1X_FIX == 1 ? chipMinorFeatures6_RA_DEPTH_WRITE_MSAA1X_FIX : 0) |
				(gChipInfo_AMLOGIC[i].EZHZ_CLOCKGATE_FIX == 1 ? chipMinorFeatures6_EZHZ_CLOCKGATE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].SH_SNAP2PAGE_FIX == 1 ? chipMinorFeatures6_SH_SNAP2PAGE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].SH_HALFDEPENDENCY_FIX == 1 ? chipMinorFeatures6_SH_HALFDEPENDENCY_FIX : 0) |
				(gChipInfo_AMLOGIC[i].USC_MCFILL_FIX == 1 ? chipMinorFeatures6_USC_MCFILL_FIX : 0) |
				(gChipInfo_AMLOGIC[i].TPG_TCPERF_FIX == 1 ? chipMinorFeatures6_TPG_TCPERF_FIX : 0);

			uint32_t features7 = (gChipInfo_AMLOGIC[i].USC_MDFIFO_OVERFLOW_FIX == 1 ? chipMinorFeatures7_USC_MDFIFO_OVERFLOW_FIX : 0) |
				(gChipInfo_AMLOGIC[i].SH_TEXLD_BARRIER_IN_CS_FIX == 1 ? chipMinorFeatures7_SH_TEXLD_BARRIER_IN_CS_FIX : 0) |
				(gChipInfo_AMLOGIC[i].RS_NEW_BASEADDR == 1 ? chipMinorFeatures7_RS_NEW_BASEADDR : 0) |
				(gChipInfo_AMLOGIC[i].PE_8bpp_DUALPIPE_FIX == 1 ? chipMinorFeatures7_PE_8BPP_DUALPIPE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].SH_ADVANCED_INSTR == 1 ? chipMinorFeatures7_SH_ADVANCED_INSTR : 0) |
				(gChipInfo_AMLOGIC[i].SH_FLAT_INTERPOLATION_DUAL16_FIX == 1 ? chipMinorFeatures7_SH_FLAT_INTERPOLATION_DUAL16_FIX : 0) |
				(gChipInfo_AMLOGIC[i].USC_CONTINUOUS_FLUS_FIX == 1 ? chipMinorFeatures7_USC_CONTINUOUS_FLUS_FIX : 0) |
				(gChipInfo_AMLOGIC[i].SH_SUPPORT_V4 == 1 ? chipMinorFeatures7_SH_SUPPORT_V4 : 0) |
				(gChipInfo_AMLOGIC[i].SH_SUPPORT_ALPHA_KILL == 1 ? chipMinorFeatures7_SH_SUPPORT_ALPHA_KILL : 0) |
				(gChipInfo_AMLOGIC[i].PE_NO_ALPHA_TEST == 1 ? chipMinorFeatures7_PE_NO_ALPHA_TEST : 0) |
				(gChipInfo_AMLOGIC[i].TX_LOD_NEAREST_SELECT == 1 ? chipMinorFeatures7_TX_LOD_NEAREST_SELECT : 0) |
				(gChipInfo_AMLOGIC[i].SH_FIX_LDEXP == 1 ? chipMinorFeatures7_SH_FIX_LDEXP : 0) |
				(gChipInfo_AMLOGIC[i].SUPPORT_MOVAI == 1 ? chipMinorFeatures7_SUPPORT_MOVAI : 0) |
				(gChipInfo_AMLOGIC[i].SH_SNAP2PAGE_MAXPAGES_FIX == 1 ? chipMinorFeatures7_SH_SNAP2PAGE_MAXPAGES_FIX : 0) |
				(gChipInfo_AMLOGIC[i].PE_RGBA16I_FIX == 1 ? chipMinorFeatures7_PE_RGBA16I_FIX : 0) |
				(gChipInfo_AMLOGIC[i].BLT_8bpp_256TILE_FC_FIX == 1 ? chipMinorFeatures7_BLT_8bpp_256TILE_FC_FIX : 0) |
				(gChipInfo_AMLOGIC[i].PE_64bit_FENCE_FIX == 1 ? chipMinorFeatures7_PE_64BIT_FENCE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].USC_FULL_CACHE_FIX == 1 ? chipMinorFeatures7_USC_FULL_CACHE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].TX_YUV_ASSEMBLER_10BIT == 1 ? chipMinorFeatures7_TX_YUV_ASSEMBLER_10BIT : 0) |
				(gChipInfo_AMLOGIC[i].FE_32bit_INDEX_FIX == 1 ? chipMinorFeatures7_FE_32BIT_INDEX_FIX : 0) |
				(gChipInfo_AMLOGIC[i].BLT_64bpp_MASKED_CLEAR_FIX == 1 ? chipMinorFeatures7_BLT_64BPP_MASKED_CLEAR_FIX : 0) |
				(gChipInfo_AMLOGIC[i].SECURITY == 1 ? chipMinorFeatures7_BIT_SECURITY : 0) |
				(gChipInfo_AMLOGIC[i].ROBUSTNESS == 1 ? chipMinorFeatures7_BIT_ROBUSTNESS : 0) |
				(gChipInfo_AMLOGIC[i].USC_ATOMIC_FIX == 1 ? chipMinorFeatures7_USC_ATOMIC_FIX : 0) |
				(gChipInfo_AMLOGIC[i].SH_PSO_MSAA1x_FIX == 1 ? chipMinorFeatures7_SH_PSO_MSAA1x_FIX : 0) |
				(gChipInfo_AMLOGIC[i].USC_VX_PERF_FIX == 1 ? chipMinorFeatures7_BIT_USC_VX_PERF_FIX : 0) |
				(gChipInfo_AMLOGIC[i].EVIS_NO_ABSDIFF == 1 ? chipMinorFeatures7_EVIS_NO_ABSDIFF : 0) |
				(gChipInfo_AMLOGIC[i].EVIS_NO_BITREPLACE == 1 ? chipMinorFeatures7_EVIS_NO_BITREPLACE : 0) |
				(gChipInfo_AMLOGIC[i].EVIS_NO_BOXFILTER == 1 ? chipMinorFeatures7_EVIS_NO_BOXFILTER : 0) |
				(gChipInfo_AMLOGIC[i].EVIS_NO_CORDIAC == 1 ? chipMinorFeatures7_EVIS_NO_CORDIAC : 0) |
				(gChipInfo_AMLOGIC[i].EVIS_NO_DP32 == 1 ? chipMinorFeatures7_EVIS_NO_DP32 : 0) |
				(gChipInfo_AMLOGIC[i].EVIS_NO_FILTER == 1 ? chipMinorFeatures7_EVIS_NO_FILTER : 0);

			uint32_t features8 =
				(gChipInfo_AMLOGIC[i].EVIS_NO_IADD == 1 ? chipMinorFeatures8_EVIS_NO_IADD : 0) |
				(gChipInfo_AMLOGIC[i].EVIS_NO_SELECTADD == 1 ? chipMinorFeatures8_EVIS_NO_SELECTADD : 0) |
				(gChipInfo_AMLOGIC[i].EVIS_LERP_7OUTPUT == 1 ? chipMinorFeatures8_EVIS_LERP_7OUTPUT : 0) |
				(gChipInfo_AMLOGIC[i].EVIS_ACCSQ_8OUTPUT == 1 ? chipMinorFeatures8_EVIS_ACCSQ_8OUTPUT : 0) |
				(gChipInfo_AMLOGIC[i].USC_GOS_ADDR_FIX == 1 ? chipMinorFeatures8_USC_GOS_ADDR_FIX : 0) |
				(gChipInfo_AMLOGIC[i].TX_8bit_UVFrac == 1 ? chipMinorFeatures8_TX_8BIT_UVFRAC : 0) |
				(gChipInfo_AMLOGIC[i].TX_DESC_CACHE_CLOCKGATE_FIX == 1 ? chipMinorFeatures8_TX_DESC_CACHE_CLOCKGATE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].RSBLT_MSAA_DECOMPRESSION == 1 ? chipMinorFeatures8_RSBLT_MSAA_DECOMPRESSION : 0) |
				(gChipInfo_AMLOGIC[i].TX_INTEGER_COORDINATE == 1 ? chipMinorFeatures8_TX_INTEGER_COORDINATE : 0) |
				(gChipInfo_AMLOGIC[i].DRAWID == 1 ? chipMinorFeatures8_DRAWID : 0) |
				(gChipInfo_AMLOGIC[i].PSIO_SAMPLEMASK_IN_R0ZW_FIX == 1 ? chipMinorFeatures8_PSIO_SAMPLEMASK_IN_R0ZW_FIX : 0) |
				(gChipInfo_AMLOGIC[i].TX_INTEGER_COORDINATE_V2 == 1 ? chipMinorFeatures8_TX_INTEGER_COORDINATE_V2 : 0) |
				(gChipInfo_AMLOGIC[i].MULTI_CORE_BLOCK_SET_CONFIG == 1 ? chipMinorFeatures8_MULTI_CORE_BLOCK_SET_CONFIG : 0) |
				(gChipInfo_AMLOGIC[i].VG_RESOLVE_ENGINE == 1 ? chipMinorFeatures8_VG_RESOLVE_ENGINE : 0) |
				(gChipInfo_AMLOGIC[i].VG_PE_COLOR_KEY == 1 ? chipMinorFeatures8_VG_PE_COLOR_KEY : 0) |
				(gChipInfo_AMLOGIC[i].VG_IM_INDEX_FORMAT == 1 ? chipMinorFeatures8_VG_IM_INDEX_FORMAT : 0) |
				(gChipInfo_AMLOGIC[i].SNAPPAGE_CMD == 1 ? chipMinorFeatures8_SNAPPAGE_CMD : 0) |
				(gChipInfo_AMLOGIC[i].SH_NO_INDEX_CONST_ON_A0 == 1 ? chipMinorFeatures8_SH_NO_INDEX_CONST_ON_A0 : 0) |
				(gChipInfo_AMLOGIC[i].SH_NO_ONECONST_LIMIT == 1 ? chipMinorFeatures8_SH_NO_ONECONST_LIMIT : 0) |
				(gChipInfo_AMLOGIC[i].SH_IMG_LDST_ON_TEMP == 1 ? chipMinorFeatures8_SH_IMG_LDST_ON_TEMP : 0) |
				(gChipInfo_AMLOGIC[i].COMPUTE_ONLY == 1 ? chipMinorFeatures8_COMPUTE_ONLY : 0) |
				(gChipInfo_AMLOGIC[i].SH_IMG_LDST_CLAMP == 1 ? chipMinorFeatures8_SH_IMG_LDST_CLAMP : 0) |
				(gChipInfo_AMLOGIC[i].SH_ICACHE_ALLOC_COUNT_FIX == 1 ? chipMinorFeatures8_SH_ICACHE_ALLOC_COUNT_FIX : 0) |
				(gChipInfo_AMLOGIC[i].SH_ICACHE_PREFETCH == 1 ? chipMinorFeatures8_SH_ICACHE_PREFETCH : 0) |
				(gChipInfo_AMLOGIC[i].PE2D_SEPARATE_CACHE == 1 ? chipMinorFeatures8_PE2D_SEPARATE_CACHE : 0) |
				(gChipInfo_AMLOGIC[i].VG_AYUV_INPUT_OUTPUT == 1 ? chipMinorFeatures8_VG_AYUV_INPUT_OUTPUT : 0) |
				(gChipInfo_AMLOGIC[i].VG_DOUBLE_IMAGE == 1 ? chipMinorFeatures8_VG_DOUBLE_IMAGE : 0) |
				(gChipInfo_AMLOGIC[i].VG_RECTANGLE_STRIPE_MODE == 1 ? chipMinorFeatures8_VG_RECTANGLE_STRIPE_MODE : 0) |
				(gChipInfo_AMLOGIC[i].VG_MMU == 1 ? chipMinorFeatures8_VG_MMU : 0) |
				(gChipInfo_AMLOGIC[i].VG_IM_FILTER == 1 ? chipMinorFeatures8_VG_IM_FILTER : 0) |
				(gChipInfo_AMLOGIC[i].VG_IM_YUV_PACKET == 1 ? chipMinorFeatures8_VG_IM_YUV_PACKET : 0) |
				(gChipInfo_AMLOGIC[i].VG_IM_YUV_PLANAR == 1 ? chipMinorFeatures8_VG_IM_YUV_PLANAR : 0);

			uint32_t features9 =
				(gChipInfo_AMLOGIC[i].VG_PE_YUV_PACKET == 1 ? chipMinorFeatures9_VG_PE_YUV_PACKET : 0) |
				(gChipInfo_AMLOGIC[i].VG_COLOR_PRECISION_8_BIT == 1 ? chipMinorFeatures9_VG_COLOR_PRECISION_8_BIT : 0) |
				(gChipInfo_AMLOGIC[i].PE_MSAA_OQ_FIX == 1 ? chipMinorFeatures9_PE_MSAA_OQ_FIX : 0) |
				(gChipInfo_AMLOGIC[i].PSIO_MSAA_CL_FIX == 1 ? chipMinorFeatures9_PSIO_MSAA_CL_FIX : 0) |
				(gChipInfo_AMLOGIC[i].USC_DEFER_FILL_FIX == 1 ? chipMinorFeatures9_USC_DEFER_FILL_FIX : 0) |
				(gChipInfo_AMLOGIC[i].SH_CLOCK_GATE_FIX == 1 ? chipMinorFeatures9_SH_CLOCK_GATE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].FE_NEED_DUMMYDRAW == 1 ? chipMinorFeatures9_FE_NEED_DUMMYDRAW : 0) |
				(gChipInfo_AMLOGIC[i].PE2D_LINEAR_YUV420_OUTPUT == 1 ? chipMinorFeatures9_PE2D_LINEAR_YUV420_OUTPUT : 0) |
				(gChipInfo_AMLOGIC[i].PE2D_LINEAR_YUV420_10BIT == 1 ? chipMinorFeatures9_PE2D_LINEAR_YUV420_10BIT : 0) |
				(gChipInfo_AMLOGIC[i].MULTI_CLUSTER == 1 ? chipMinorFeatures9_MULTI_CLUSTER : 0) |
				(gChipInfo_AMLOGIC[i].VG_TS_CULLING == 1 ? chipMinorFeatures9_VG_TS_CULLING : 0) |
				(gChipInfo_AMLOGIC[i].VG_FP25 == 1 ? chipMinorFeatures9_VG_FP25 : 0) |
				(gChipInfo_AMLOGIC[i].SH_MULTI_WG_PACK == 1 ? chipMinorFeatures9_SH_MULTI_WG_PACK : 0) |
				(gChipInfo_AMLOGIC[i].SH_DUAL16_SAMPLEMASK_ZW == 1 ? chipMinorFeatures9_SH_DUAL16_SAMPLEMASK_ZW : 0) |
				(gChipInfo_AMLOGIC[i].TPG_TRIVIAL_MODE_FIX == 1 ? chipMinorFeatures9_TPG_TRIVIAL_MODE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].TX_ASTC_MULTISLICE_FIX == 1 ? chipMinorFeatures9_TX_ASTC_MULTISLICE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].FE_ROBUST_FIX == 1 ? chipMinorFeatures9_FE_ROBUST_FIX : 0) |
				(gChipInfo_AMLOGIC[i].SH_GPIPE_ACCESS_FULLTEMPS == 1 ? chipMinorFeatures9_SH_GPIPE_ACCESS_FULLTEMPS : 0) |
				(gChipInfo_AMLOGIC[i].PSIO_INTERLOCK == 1 ? chipMinorFeatures9_PSIO_INTERLOCK : 0) |
				(gChipInfo_AMLOGIC[i].PA_WIDELINE_FIX == 1 ? chipMinorFeatures9_PA_WIDELINE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].WIDELINE_HELPER_FIX == 1 ? chipMinorFeatures9_WIDELINE_HELPER_FIX : 0) |
				(gChipInfo_AMLOGIC[i].G2D_3rd_PARTY_COMPRESSION_1_1 == 1 ? chipMinorFeatures9_G2D_3RD_PARTY_COMPRESSION_1_1 : 0) |
				(gChipInfo_AMLOGIC[i].TX_FLUSH_L1CACHE == 1 ? chipMinorFeatures9_TX_FLUSH_L1CACHE : 0) |
				(gChipInfo_AMLOGIC[i].PE_DITHER_FIX2 == 1 ? chipMinorFeatures9_PE_DITHER_FIX2 : 0) |
				(gChipInfo_AMLOGIC[i].G2D_DEC400 == 1 ? chipMinorFeatures9_G2D_DEC400 : 0) |
				(gChipInfo_AMLOGIC[i].SH_TEXLD_U_FIX == 1 ? chipMinorFeatures9_SH_TEXLD_U_FIX : 0) |
				(gChipInfo_AMLOGIC[i].MC_FCCACHE_BYTEMASK == 1 ? chipMinorFeatures9_MC_FCCACHE_BYTEMASK : 0) |
				(gChipInfo_AMLOGIC[i].SH_MULTI_WG_PACK_FIX == 1 ? chipMinorFeatures9_SH_MULTI_WG_PACK_FIX : 0) |
				(gChipInfo_AMLOGIC[i].DC_OVERLAY_SCALING == 1 ? chipMinorFeatures9_DC_OVERLAY_SCALING : 0) |
				(gChipInfo_AMLOGIC[i].DC_SOURCE_ROTATION == 1 ? chipMinorFeatures9_DC_SOURCE_ROTATION : 0) |
				(gChipInfo_AMLOGIC[i].DC_TILED == 1 ? chipMinorFeatures9_DC_TILED : 0) |
				(gChipInfo_AMLOGIC[i].DC_YUV_L1 == 1 ? chipMinorFeatures9_DC_YUV_L1 : 0);

			uint32_t features10 =
				(gChipInfo_AMLOGIC[i].DC_D30_OUTPUT == 1 ? chipMinorFeatures10_DC_D30_OUTPUT : 0) |
				(gChipInfo_AMLOGIC[i].DC_MMU == 1 ? chipMinorFeatures10_DC_MMU : 0) |
				(gChipInfo_AMLOGIC[i].DC_COMPRESSION == 1 ? chipMinorFeatures10_DC_COMPRESSION : 0) |
				(gChipInfo_AMLOGIC[i].DC_QOS == 1 ? chipMinorFeatures10_DC_QOS : 0) |
				(gChipInfo_AMLOGIC[i].PE_ADVANCE_BLEND_PART0 == 1 ? chipMinorFeatures10_PE_ADVANCE_BLEND_PART0 : 0) |
				(gChipInfo_AMLOGIC[i].FE_PATCHLIST_FETCH_FIX == 1 ? chipMinorFeatures10_FE_PATCHLIST_FETCH_FIX : 0) |
				(gChipInfo_AMLOGIC[i].RA_CG_FIX == 1 ? chipMinorFeatures10_RA_CG_FIX : 0) |
				(gChipInfo_AMLOGIC[i].EVIS_VX2 == 1 ? chipMinorFeatures10_EVIS_VX2 : 0) |
				(gChipInfo_AMLOGIC[i].NN_FLOAT32_IO == 1 ? chipMinorFeatures10_NN_FLOAT : 0) |
				(gChipInfo_AMLOGIC[i].DEC400 == 1 ? chipMinorFeatures10_DEC400 : 0) |
				(gChipInfo_AMLOGIC[i].LS_SUPPORT_PERCOMP_DEPENDENCY == 1 ? chipMinorFeatures10_LS_SUPPORT_PERCOMP_DEPENDENCY : 0) |
				(gChipInfo_AMLOGIC[i].TP_ENGINE == 1 ? chipMinorFeatures10_TP_ENGINE : 0) |
				(gChipInfo_AMLOGIC[i].MULTI_CORE_BLOCK_SET_CONFIG2 == 1 ? chipMinorFeatures10_MULTI_CORE_BLOCK_SET_CONFIG2 : 0) |
				(gChipInfo_AMLOGIC[i].PE_VMSAA_COVERAGE_CACHE_FIX == 1 ? chipMinorFeatures10_PE_VMSAA_COVERAGE_CACHE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].SECURITY_AHB == 1 ? chipMinorFeatures10_SECURITY_AHB : 0) |
				(gChipInfo_AMLOGIC[i].MULTICORE_SEMAPHORESTALL_V3 == 1 ? chipMinorFeatures10_MULTICORE_SEMAPHORESTALL_V3 : 0) |
				(gChipInfo_AMLOGIC[i].SMALLBATCH == 1 ? chipMinorFeatures10_SMALLBATCH : 0) |
				(gChipInfo_AMLOGIC[i].SH_CMPLX == 1 ? chipMinorFeatures10_SH_CMPLX : 0) |
				(gChipInfo_AMLOGIC[i].SH_IDIV0_SWZL_EHS == 1 ? chipMinorFeatures10_SH_IDIV0_SWZL_EHS : 0) |
				(gChipInfo_AMLOGIC[i].TX_LERP_LESS_BIT == 1 ? chipMinorFeatures10_TX_LERP_LESS_BIT : 0) |
				(gChipInfo_AMLOGIC[i].SH_GM_ENDIAN == 1 ? chipMinorFeatures10_SH_GM_ENDIAN : 0) |
				(gChipInfo_AMLOGIC[i].SH_GM_USC_UNALLOC == 1 ? chipMinorFeatures10_SH_GM_USC_UNALLOC : 0) |
				(gChipInfo_AMLOGIC[i].SH_END_OF_BB == 1 ? chipMinorFeatures10_SH_END_OF_BB : 0) |
				(gChipInfo_AMLOGIC[i].VIP_V7 == 1 ? chipMinorFeatures10_VIP_V7 : 0) |
				(gChipInfo_AMLOGIC[i].TX_BORDER_CLAMP_FIX == 1 ? chipMinorFeatures10_TX_BORDER_CLAMP_FIX : 0) |
				(gChipInfo_AMLOGIC[i].SH_IMG_LD_LASTPIXEL_FIX == 1 ? chipMinorFeatures10_SH_IMG_LD_LASTPIXEL_FIX : 0) |
				(gChipInfo_AMLOGIC[i].ASYNC_BLT == 1 ? chipMinorFeatures10_ASYNC_BLT : 0) |
				(gChipInfo_AMLOGIC[i].ASYNC_FE_FENCE_FIX == 1 ? chipMinorFeatures10_ASYNC_FE_FENCE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].PSCS_THROTTLE == 1 ? chipMinorFeatures10_PSCS_THROTTLE : 0) |
				(gChipInfo_AMLOGIC[i].SEPARATE_LS == 1 ? chipMinorFeatures10_SEPARATE_LS : 0) |
				(gChipInfo_AMLOGIC[i].MCFE == 1 ? chipMinorFeatures10_MCFE : 0) |
				(gChipInfo_AMLOGIC[i].WIDELINE_TRIANGLE_EMU == 1 ? chipMinorFeatures10_WIDELINE_TRIANGLE_EMU : 0);

			uint32_t features11 =
				(gChipInfo_AMLOGIC[i].VG_RESOLUTION_8K == 1 ? chipMinorFeatures11_VG_RESOLUTION_8K : 0) |
				(gChipInfo_AMLOGIC[i].FENCE_32BIT == 1 ? chipMinorFeatures11_FENCE_32BIT : 0) |
				(gChipInfo_AMLOGIC[i].FENCE_64BIT == 1 ? chipMinorFeatures11_FENCE_64BIT : 0) |
				(gChipInfo_AMLOGIC[i].NN_INTERLEVE8 == 1 ? chipMinorFeatures11_NN_INTERLEVE8 : 0) |
				(gChipInfo_AMLOGIC[i].PE_DEPTH_ONLY_OQFIX == 1 ? chipMinorFeatures11_PE_DEPTH_ONLY_OQFIX : 0) |
				(gChipInfo_AMLOGIC[i].TP_REORDER == 1 ? chipMinorFeatures11_TP_REORDER : 0);
#if defined EXTENDED_FEATURE_LIST
			(gChipInfo_AMLOGIC[i].TX_SEAMLESS_CUBE == 1 ? chipMinorFeatures11_TX_SEAMLESS_CUBE : 0) |
				(gChipInfo_AMLOGIC[i].TX_SNORM_SUPPORT == 1 ? chipMinorFeatures11_TX_SNORM_SUPPORT : 0) |
				(gChipInfo_AMLOGIC[i].SH_SCATTER_GATHER == 1 ? chipMinorFeatures11_SH_SCATTER_GATHER : 0) |
				(gChipInfo_AMLOGIC[i].HWMANAGED_LS == 1 ? chipMinorFeatures11_HWMANAGED_LS : 0) |
				(gChipInfo_AMLOGIC[i].SH_IMAGE_ENABLE_FIX == 1 ? chipMinorFeatures11_SH_IMAGE_ENABLE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].MSAA_FRAGMENT_OPERATION == 1 ? chipMinorFeatures11_MSAA_FRAGMENT_OPERATION : 0) |
				(gChipInfo_AMLOGIC[i].PE_TILE_CACHE_FLUSH_FIX == 1 ? chipMinorFeatures11_PE_TILE_CACHE_FLUSH_FIX : 0) |
				(gChipInfo_AMLOGIC[i].BLT_YUV_OUTPUT == 1 ? chipMinorFeatures11_BLT_YUV_OUTPUT : 0) |
				(gChipInfo_AMLOGIC[i].SH_IO_CG_FIX == 1 ? chipMinorFeatures11_SH_IO_CG_FIX : 0) |
				(gChipInfo_AMLOGIC[i].SH_ROBUSTNESS_FIX == 1 ? chipMinorFeatures11_SH_ROBUSTNESS_FIX : 0) |
				(gChipInfo_AMLOGIC[i].USC_ATOMIC_FIX2 == 1 ? chipMinorFeatures11_USC_ATOMIC_FIX2 : 0) |
				(gChipInfo_AMLOGIC[i].PE_A8B8G8R8 == 1 ? chipMinorFeatures11_PE_A8B8G8R8 : 0) |
				(gChipInfo_AMLOGIC[i].MULTIVIEW_RENDER == 1 ? chipMinorFeatures11_MULTIVIEW_RENDER : 0) |
				(gChipInfo_AMLOGIC[i].FE_DRAW_DIRECT == 1 ? chipMinorFeatures11_FE_DRAW_DIRECT : 0) |
				(gChipInfo_AMLOGIC[i].TX_VKBORDER_MODE == 1 ? chipMinorFeatures11_TX_VKBORDER_MODE : 0) |
				(gChipInfo_AMLOGIC[i].TX_UNNORMALIZED_COORD == 1 ? chipMinorFeatures11_TX_UNNORMALIZED_COORD : 0) |
				(gChipInfo_AMLOGIC[i].PA_LINECLIP_FIX == 1 ? chipMinorFeatures11_PA_LINECLIP_FIX : 0) |
				(gChipInfo_AMLOGIC[i].TX_8bit_UVFrac_ROUNDING_FIX == 1 ? chipMinorFeatures11_TX_8bit_UVFrac_ROUNDING_FIX : 0) |
				(gChipInfo_AMLOGIC[i].MP_ARCH == 1 ? chipMinorFeatures11_MP_ARCH : 0) |
				(gChipInfo_AMLOGIC[i].TX_NO_FIXED_FILTER == 1 ? chipMinorFeatures11_TX_NO_FIXED_FILTER : 0) |
				(gChipInfo_AMLOGIC[i].SHARE_Z == 1 ? chipMinorFeatures11_SHARE_Z : 0) |
				(gChipInfo_AMLOGIC[i].DE_2D_FAST_CLEAR == 1 ? chipMinorFeatures11_DE_2D_FAST_CLEAR : 0) |
				(gChipInfo_AMLOGIC[i].DE_TILESTATUS_ROTATION_FIX == 1 ? chipMinorFeatures11_DE_TILESTATUS_ROTATION_FIX : 0) |
				(gChipInfo_AMLOGIC[i].TX_CLEAR_PENDING_FIX == 1 ? chipMinorFeatures11_TX_CLEAR_PENDING_FIX : 0) |
				(gChipInfo_AMLOGIC[i].HI1_L2_CACHE == 1 ? chipMinorFeatures11_HI1_L2_CACHE : 0) |
				(gChipInfo_AMLOGIC[i].USC_EVICT_CTRL_FIFO_FLOP_RESET_FIX == 1 ? chipMinorFeatures11_USC_EVICT_CTRL_FIFO_FLOP_RESET_FIX : 0) |
#endif
				(gChipInfo_AMLOGIC[i].TP_REORDER == 1 ? chipMinorFeatures11_TP_REORDER : 0);

#if defined EXTENDED_FEATURE_LIST
			uint32_t features12 =
				(gChipInfo_AMLOGIC[i].FORMAT_10BIT_CROSS_4K == 1 ? chipMinorFeatures12_FORMAT_10BIT_CROSS_4K : 0) |
				(gChipInfo_AMLOGIC[i].FORMAT_P010LSB_I010 == 1 ? chipMinorFeatures12_FORMAT_P010LSB_I010 : 0) |
				(gChipInfo_AMLOGIC[i].ENDIAN_CONTROL == 1 ? chipMinorFeatures12_ENDIAN_CONTROL : 0) |
				(gChipInfo_AMLOGIC[i].G2D_RGB_PLANAR == 1 ? chipMinorFeatures12_G2D_RGB_PLANAR : 0) |
				//(gChipInfo_AMLOGIC[i].G2D_RGB_PLANAR_SOURCE == 1 ? chipMinorFeatures12_G2D_RGB_PLANAR_SOURCE : 0) |
				(gChipInfo_AMLOGIC[i].G2D_DEC400EX == 1 ? chipMinorFeatures12_G2D_DEC400EX : 0) |
				//(gChipInfo_AMLOGIC[i].G2D_FC_IN_DEC400EX == 1 ? chipMinorFeatures12_G2D_FC_IN_DEC400EX : 0) |
				(gChipInfo_AMLOGIC[i].G2D_NO_YUV420_SOURCE == 1 ? chipMinorFeatures12_G2D_NO_YUV420_SOURCE : 0) |
				(gChipInfo_AMLOGIC[i].G2D_YUV420_101010 == 1 ? chipMinorFeatures12_G2D_YUV420_101010 : 0) |
				//(gChipInfo_AMLOGIC[i].G2D_MultiSrcBlt_Pipe == 1 ? chipMinorFeatures12_G2D_MultiSrcBlt_Pipe : 0) |
				//(gChipInfo_AMLOGIC[i].G2D_Stretch_MultiSrc_Pipe == 1 ? chipMinorFeatures12_G2D_Stretch_MultiSrc_Pipe : 0) |
				//(gChipInfo_AMLOGIC[i].G2D_Normalization == 1 ? chipMinorFeatures12_G2D_Normalization : 0) |
				//(gChipInfo_AMLOGIC[i].G2D_Normalization_Quantization == 1 ? chipMinorFeatures12_G2D_Normalization_Quantization : 0) |
				//(gChipInfo_AMLOGIC[i].G2D_FRAME_DONE_INTR == 1 ? chipMinorFeatures12_G2D_FRAME_DONE_INTR : 0) |
				//(gChipInfo_AMLOGIC[i].G2D_MASK_AND_COLORKEY == 1 ? chipMinorFeatures12_G2D_MASK_AND_COLORKEY : 0) |
				//(gChipInfo_AMLOGIC[i].AXIFE == 1 ? chipMinorFeatures12_AXIFE : 0) |
				(gChipInfo_AMLOGIC[i].SH_VX2_FLOATING_MAD_FIX == 1 ? chipMinorFeatures12_SH_VX2_FLOATING_MAD_FIX : 0) |
				(gChipInfo_AMLOGIC[i].TS_FC_VULKAN_SUPPORT == 1 ? chipMinorFeatures12_TS_FC_VULKAN_SUPPORT : 0) |
				(gChipInfo_AMLOGIC[i].MSAA_FLOAT_64BIT == 1 ? chipMinorFeatures12_MSAA_FLOAT_64BIT : 0) |
				(gChipInfo_AMLOGIC[i].INDIRECT_COMPUTE_ZERODIM_FIX == 1 ? chipMinorFeatures12_INDIRECT_COMPUTE_ZERODIM_FIX : 0) |
				(gChipInfo_AMLOGIC[i].Q_CHANNEL_SUPPORT == 1 ? chipMinorFeatures12_Q_CHANNEL_SUPPORT : 0) |
				(gChipInfo_AMLOGIC[i].MMU_PAGE_DESCRIPTOR == 1 ? chipMinorFeatures12_MMU_PAGE_DESCRIPTOR : 0) |
				(gChipInfo_AMLOGIC[i].YUV_LINEAR_TO_TILE_ROTATE == 1 ? chipMinorFeatures12_YUV_LINEAR_TO_TILE_ROTATE : 0) |
				(gChipInfo_AMLOGIC[i].VEC2_IMULIMAD32_SUPPORT == 1 ? chipMinorFeatures12_VEC2_IMULIMAD32_SUPPORT : 0) |
				(gChipInfo_AMLOGIC[i].VEC4_IMULIMAD32_SUPPORT == 1 ? chipMinorFeatures12_VEC4_IMULIMAD32_SUPPORT : 0) |
				(gChipInfo_AMLOGIC[i].VEC2_IDIVIMOD16_SUPPORT == 1 ? chipMinorFeatures12_VEC2_IDIVIMOD16_SUPPORT : 0) |
				(gChipInfo_AMLOGIC[i].DST_TEX_I2F_F2I_INST_DEPRECATE == 1 ? chipMinorFeatures12_DST_TEX_I2F_F2I_INST_DEPRECATE : 0) |
				(gChipInfo_AMLOGIC[i].ALU_FP16_INSTRUCTIONS == 1 ? chipMinorFeatures12_ALU_FP16_INSTRUCTIONS : 0) |
				(gChipInfo_AMLOGIC[i].DUAL16_14BIT_PC_SUPPORT == 1 ? chipMinorFeatures12_DUAL16_14BIT_PC_SUPPORT : 0) |
				(gChipInfo_AMLOGIC[i].LDST_CONV_4ROUNDING_MODES == 1 ? chipMinorFeatures12_LDST_CONV_4ROUNDING_MODES : 0) |
				(gChipInfo_AMLOGIC[i].FULL_PACK_MODE_SUPPORT == 1 ? chipMinorFeatures12_FULL_PACK_MODE_SUPPORT : 0) |
				(gChipInfo_AMLOGIC[i].DEPTH_FLOAT32_SUPPORT == 1 ? chipMinorFeatures12_DEPTH_FLOAT32_SUPPORT : 0);

			uint32_t features13 =
				(gChipInfo_AMLOGIC[i].GPU_INSPECTOR_COUNTERS == 1 ? chipMinorFeatures13_GPU_INSPECTOR_COUNTERS : 0) |
				(gChipInfo_AMLOGIC[i].FP32_TO_FP16_CONV_FIX == 1 ? chipMinorFeatures13_FP32_TO_FP16_CONV_FIX : 0) |
				(gChipInfo_AMLOGIC[i].IMGLD_COMP_COUNT_FIX == 1 ? chipMinorFeatures13_IMGLD_COMP_COUNT_FIX : 0) |
				(gChipInfo_AMLOGIC[i].IMGLD_WIDTH_LT16_FIX == 1 ? chipMinorFeatures13_IMGLD_WIDTH_LT16_FIX : 0) |
				(gChipInfo_AMLOGIC[i].TX_FILTER_ROUND_FIX == 1 ? chipMinorFeatures13_TX_FILTER_ROUND_FIX : 0) |
				(gChipInfo_AMLOGIC[i].SH_FP32_FMA_SUPPORT == 1 ? chipMinorFeatures13_SH_FP32_FMA_SUPPORT : 0) |
				//(gChipInfo_AMLOGIC[i].PE_64BPP_LINEAR_FORMAT == 1 ? chipMinorFeatures13_PE_64BPP_LINEAR_FORMAT : 0) |
				(gChipInfo_AMLOGIC[i].TX_ETC2_COMPRESSION == 1 ? chipMinorFeatures13_TX_ETC2_COMPRESSION : 0) |
				//(gChipInfo_AMLOGIC[i].HIGHP_VEC2 == 1 ? chipMinorFeatures13_HIGHP_VEC2 : 0) |
				//(gChipInfo_AMLOGIC[i].MMU_PD_42_BIT_ADDRESS == 1 ? chipMinorFeatures13_MMU_PD_42_BIT_ADDRESS : 0) |
				//(gChipInfo_AMLOGIC[i].BLT_ROBUSTNESS_FIX == 1 ? chipMinorFeatures13_BLT_ROBUSTNESS_FIX : 0) |
				//(gChipInfo_AMLOGIC[i].TFB_PERF_FIX == 1 ? chipMinorFeatures13_TFB_PERF_FIX : 0) |
				//(gChipInfo_AMLOGIC[i].SH_SUPERSCALAR_ARCH == 1 ? chipMinorFeatures13_SH_SUPERSCALAR_ARCH : 0) | 00
				//(gChipInfo_AMLOGIC[i].PA_ZEROAREA_LINE_FIX == 1 ? chipMinorFeatures13_PA_ZEROAREA_LINE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].VG_IMAGE_16K == 1 ? chipMinorFeatures13_VG_IMAGE_16K : 0) |
				(gChipInfo_AMLOGIC[i].VG_FORMAT_ARGB2222 == 1 ? chipMinorFeatures13_VG_FORMAT_ARGB2222 : 0) |
				(gChipInfo_AMLOGIC[i].VIP_HW_FINAL_RELEASE == 1 ? chipMinorFeatures13_VIP_HW_FINAL_RELEASE : 0) |
				(gChipInfo_AMLOGIC[i].NN_SINGLEPORT_ACCUMBUFFER == 1 ? chipMinorFeatures13_NN_SINGLEPORT_ACCUMBUFFER : 0) |
				(gChipInfo_AMLOGIC[i].NN_STRIDE_SUPPORT == 1 ? chipMinorFeatures13_NN_STRIDE_SUPPORT : 0) |
				(gChipInfo_AMLOGIC[i].SWTILING_PHASE1 == 1 ? chipMinorFeatures13_SWTILING_PHASE1 : 0) |
				(gChipInfo_AMLOGIC[i].SWTILING_PHASE2 == 1 ? chipMinorFeatures13_SWTILING_PHASE2 : 0) |
				(gChipInfo_AMLOGIC[i].TP_SIMPLE_INT16 == 1 ? chipMinorFeatures13_TP_SIMPLE_INT16 : 0) |
				(gChipInfo_AMLOGIC[i].TP_REAL_INT16 == 1 ? chipMinorFeatures13_TP_REAL_INT16 : 0) |
				(gChipInfo_AMLOGIC[i].TP_ROI_POOLING == 1 ? chipMinorFeatures13_TP_ROI_POOLING : 0) |
				(gChipInfo_AMLOGIC[i].TP_MAX_POOLING_STRIDE1 == 1 ? chipMinorFeatures13_TP_MAX_POOLING_STRIDE1 : 0) |
				(gChipInfo_AMLOGIC[i].TP_LRN == 1 ? chipMinorFeatures13_TP_LRN : 0) |
				(gChipInfo_AMLOGIC[i].TF_QUANTIZATION == 1 ? chipMinorFeatures13_TF_QUANTIZATION : 0) |
				(gChipInfo_AMLOGIC[i].NN_NONZERO_BORDER == 1 ? chipMinorFeatures13_NN_NONZERO_BORDER : 0) |
				(gChipInfo_AMLOGIC[i].NN_MIRROR_BORDER == 1 ? chipMinorFeatures13_NN_MIRROR_BORDER : 0) |
				(gChipInfo_AMLOGIC[i].AI_GPU == 1 ? chipMinorFeatures13_AI_GPU : 0);

			uint32_t features14 =
				(gChipInfo_AMLOGIC[i].TP_TENSOR_ADD_MUL == 1 ? chipMinorFeatures13_TP_TENSOR_ADD_MUL : 0) |
				(gChipInfo_AMLOGIC[i].NN_DEPTHWISE_INT16XINT8 == 1 ? chipMinorFeatures13_NN_DEPTHWISE_INT16XINT8 : 0) |
				(gChipInfo_AMLOGIC[i].NN_DEPTHWISE_8BIT_VIP_V7 == 1 ? chipMinorFeatures14_NN_DEPTHWISE_8BIT_VIP_V7 : 0) |
				(gChipInfo_AMLOGIC[i].TP_SOFTMAX == 1 ? chipMinorFeatures14_TP_SOFTMAX : 0) |
				(gChipInfo_AMLOGIC[i].NN_23BITS_POST_MULTIPLIER_VIP_V7 == 1 ? chipMinorFeatures14_NN_23BITS_POST_MULTIPLIER_VIP_V7 : 0) |
				(gChipInfo_AMLOGIC[i].TP_23BITS_POST_MULTIPLIER_VIP_V7 == 1 ? chipMinorFeatures14_TP_23BITS_POST_MULTIPLIER_VIP_V7 : 0) |
				(gChipInfo_AMLOGIC[i].CONV_INT16X8BIT_VIP_V7 == 1 ? chipMinorFeatures14_CONV_INT16X8BIT_VIP_V7 : 0) |
				(gChipInfo_AMLOGIC[i].NN_REMOVE_POOLING == 1 ? chipMinorFeatures14_NN_REMOVE_POOLING : 0) |
				(gChipInfo_AMLOGIC[i].NN_40BIT_BIAS == 1 ? chipMinorFeatures14_NN_40BIT_BIAS : 0) |
				(gChipInfo_AMLOGIC[i].TP_REMOVE_USC == 1 ? chipMinorFeatures14_TP_REMOVE_USC : 0) |
				(gChipInfo_AMLOGIC[i].NN_ZDP6 == 1 ? chipMinorFeatures14_NN_ZDP6 : 0) |
				(gChipInfo_AMLOGIC[i].NN_XYDP9 == 1 ? chipMinorFeatures14_NN_XYDP9 : 0) |
				(gChipInfo_AMLOGIC[i].NN_FIRST_PIXEL_POOLING == 1 ? chipMinorFeatures14_NN_FIRST_PIXEL_POOLING : 0) |
				(gChipInfo_AMLOGIC[i].NN_ZDP3 == 1 ? chipMinorFeatures14_NN_ZDP3 : 0) |
				(gChipInfo_AMLOGIC[i].NN_XYDP6 == 1 ? chipMinorFeatures14_NN_XYDP6 : 0) |
				(gChipInfo_AMLOGIC[i].SWTILING_PHASE3 == 1 ? chipMinorFeatures14_SWTILING_PHASE3 : 0) |
				(gChipInfo_AMLOGIC[i].USC_STAY_LRU == 1 ? chipMinorFeatures14_USC_STAY_LRU : 0) |
				(gChipInfo_AMLOGIC[i].COEF_COMPRESSION_ENHANCEMENT == 1 ? chipMinorFeatures14_COEF_COMPRESSION_ENHANCEMENT : 0) |
				(gChipInfo_AMLOGIC[i].TP_COEF_COMPRESSION_ENHANCEMENT == 1 ? chipMinorFeatures14_TP_COEF_COMPRESSION_ENHANCEMENT : 0) |
				(gChipInfo_AMLOGIC[i].NN_COEF_DECOMPRESS_PERF2X == 1 ? chipMinorFeatures14_NN_COEF_DECOMPRESS_PERF2X : 0) |
				(gChipInfo_AMLOGIC[i].TP_SMALLBATCH_PHASE1 == 1 ? chipMinorFeatures14_TP_SMALLBATCH_PHASE1 : 0) |
				(gChipInfo_AMLOGIC[i].OCB_COUNTER == 1 ? chipMinorFeatures14_OCB_COUNTER : 0) |
				(gChipInfo_AMLOGIC[i].SCALER == 1 ? chipMinorFeatures14_SCALER : 0) |
				(gChipInfo_AMLOGIC[i].SCALER_4K == 1 ? chipMinorFeatures14_SCALER_4K : 0) |
				(gChipInfo_AMLOGIC[i].INPUT_4BIT == 1 ? chipMinorFeatures14_INPUT_4BIT : 0) |
				(gChipInfo_AMLOGIC[i].NN_NO_Z_LOCATION_OFFSET == 1 ? chipMinorFeatures14_NN_NO_Z_LOCATION_OFFSET : 0) |
				(gChipInfo_AMLOGIC[i].OCB_REMAP_PHYSICAL_ADDRESS == 1 ? chipMinorFeatures14_OCB_REMAP_PHYSICAL_ADDRESS : 0) |
				(gChipInfo_AMLOGIC[i].NN_SLOW_OUTPUT == 1 ? chipMinorFeatures14_NN_SLOW_OUTPUT : 0) |
				(gChipInfo_AMLOGIC[i].NO_NARROW_POST_PROCESS_PIPE == 1 ? chipMinorFeatures14_NO_NARROW_POST_PROCESS_PIPE : 0) |
				(gChipInfo_AMLOGIC[i].TP_NN_PROBE == 1 ? chipMinorFeatures14_TP_NN_PROBE : 0) |
				(gChipInfo_AMLOGIC[i].NN_DEPTHWISE_SUPPORT == 1 ? chipMinorFeatures14_NN_DEPTHWISE_SUPPORT : 0) |
				(gChipInfo_AMLOGIC[i].NN_XYDP0 == 1 ? chipMinorFeatures14_NN_XYDP0 : 0) |
				(gChipInfo_AMLOGIC[i].NN_WRITE_WITHOUT_USC == 1 ? chipMinorFeatures14_NN_WRITE_WITHOUT_USC : 0) |
				(gChipInfo_AMLOGIC[i].NN_HW_LIMITATION_NATIVE_KER_1x2_2x1 == 1 ? chipMinorFeatures14_NN_HW_LIMITATION_NATIVE_KER_1x2_2x1 : 0);

			uint32_t features15 =
				(gChipInfo_AMLOGIC[i].NN_SMALLBATCH_PHASE1 == 1 ? chipMinorFeatures15_NN_SMALLBATCH_PHASE1 : 0) |
				(gChipInfo_AMLOGIC[i].NN_SLICE_PADDING_TO_64BYTE_ALIGN == 1 ? chipMinorFeatures15_NN_SLICE_PADDING_TO_64BYTE_ALIGN : 0) |
				(gChipInfo_AMLOGIC[i].NN_DW_1x1_CONV_MERGE == 1 ? chipMinorFeatures15_NN_DW_1x1_CONV_MERGE : 0) |
				(gChipInfo_AMLOGIC[i].TP_BFLOAT16 == 1 ? chipMinorFeatures15_TP_BFLOAT16 : 0) |
				(gChipInfo_AMLOGIC[i].TP_23BITS_POST_MULTIPLIER == 1 ? chipMinorFeatures15_TP_23BITS_POST_MULTIPLIER : 0) |
				(gChipInfo_AMLOGIC[i].NN_TRANSPOSE == 1 ? chipMinorFeatures15_NN_TRANSPOSE : 0) |
				(gChipInfo_AMLOGIC[i].NN_ZDP_TRANSPOSE_CH9_ONLY == 1 ? chipMinorFeatures15_NN_ZDP_TRANSPOSE_CH9_ONLY : 0) |
				(gChipInfo_AMLOGIC[i].USE_SINGLE_PORT_VIPSRAM == 1 ? chipMinorFeatures15_USE_SINGLE_PORT_VIPSRAM : 0) |
				(gChipInfo_AMLOGIC[i].NN_LEAKY_RELU == 1 ? chipMinorFeatures15_NN_LEAKY_RELU : 0) |
				(gChipInfo_AMLOGIC[i].NN_PRELU == 1 ? chipMinorFeatures15_NN_PRELU : 0) |
				(gChipInfo_AMLOGIC[i].NN_PER_CHANNEL_QUANT == 1 ? chipMinorFeatures15_NN_PER_CHANNEL_QUANT : 0) |
				(gChipInfo_AMLOGIC[i].NN_PER_CHANNEL_QUANT_ASYM == 1 ? chipMinorFeatures15_NN_PER_CHANNEL_QUANT_ASYM : 0) |
				(gChipInfo_AMLOGIC[i].NN_ASYMMETRIC_INT8 == 1 ? chipMinorFeatures15_NN_ASYMMETRIC_INT8 : 0) |
				(gChipInfo_AMLOGIC[i].NN_FLOAT_POST_MULT == 1 ? chipMinorFeatures15_NN_FLOAT_POST_MULT : 0) |
				(gChipInfo_AMLOGIC[i].PRELU_LEAKLY_RELU_CLAMP == 1 ? chipMinorFeatures15_PRELU_LEAKLY_RELU_CLAMP : 0) |
				(gChipInfo_AMLOGIC[i].TPLITE_BFLOAT16 == 1 ? chipMinorFeatures15_TPLITE_BFLOAT16 : 0) |
				(gChipInfo_AMLOGIC[i].PREPROCESS_IMG_BUF_640BYTE_LIMIT == 1 ? chipMinorFeatures15_PREPROCESS_IMG_BUF_640BYTE_LIMIT : 0) |
				(gChipInfo_AMLOGIC[i].NN_POST_OUT_SUPPORT_FP16 == 1 ? chipMinorFeatures15_NN_POST_OUT_SUPPORT_FP16 : 0) |
				(gChipInfo_AMLOGIC[i].NN_POST_OUT_SUPPORT_BF16 == 1 ? chipMinorFeatures15_NN_POST_OUT_SUPPORT_BF16 : 0) |
				(gChipInfo_AMLOGIC[i].NN_POST_OUT_SUPPORT_FP32 == 1 ? chipMinorFeatures15_NN_POST_OUT_SUPPORT_FP32 : 0) |
				(gChipInfo_AMLOGIC[i].TP_KERNEL_1BYTE_ALGIN == 1 ? chipMinorFeatures15_TP_KERNEL_1BYTE_ALGIN : 0) |
				(gChipInfo_AMLOGIC[i].BFLOAT_COEF_COMPRESSION_ZERO_COEFBIT14_INVERSE == 1 ? chipMinorFeatures15_BFLOAT_COEF_COMPRESSION_ZERO_COEFBIT14_INVERSE : 0) |
				(gChipInfo_AMLOGIC[i].NN_COMPRESSION_BYPASSS == 1 ? chipMinorFeatures15_NN_COMPRESSION_BYPASSS : 0) |
				(gChipInfo_AMLOGIC[i].TP_3_USC == 1 ? chipMinorFeatures15_TP_3_USC : 0) |
				(gChipInfo_AMLOGIC[i].BFP_COEF_AUTO_PAD_INCOMPLETE_ZERO_IN_KZ_PLANE == 1 ? chipMinorFeatures15_BFP_COEF_AUTO_PAD_INCOMPLETE_ZERO_IN_KZ_PLANE : 0) |
				(gChipInfo_AMLOGIC[i].NN_NATIVE_STRIDE_TWO == 1 ? chipMinorFeatures15_NN_NATIVE_STRIDE_TWO : 0) |
				(gChipInfo_AMLOGIC[i].NN_TENSOR_ADD == 1 ? chipMinorFeatures15_NN_TENSOR_ADD : 0) |
				(gChipInfo_AMLOGIC[i].TP_FLOAT32_IO == 1 ? chipMinorFeatures15_TP_FLOAT32_IO : 0) |
				(gChipInfo_AMLOGIC[i].NN_SMALL_BATCH_PHASE2 == 1 ? chipMinorFeatures15_NN_SMALL_BATCH_PHASE2 : 0) |
				(gChipInfo_AMLOGIC[i].TILE_ACCESS_CAPABILITY == 1 ? chipMinorFeatures15_TILE_ACCESS_CAPABILITY : 0) |
				(gChipInfo_AMLOGIC[i].FAST_DP3_PREPROCESSOR == 1 ? chipMinorFeatures15_FAST_DP3_PREPROCESSOR : 0) |
				(gChipInfo_AMLOGIC[i].DEPTHWISE_SUPPORT_16BIT_FORMAT == 1 ? chipMinorFeatures15_DEPTHWISE_SUPPORT_16BIT_FORMAT : 0);

			uint32_t features16 =
				(gChipInfo_AMLOGIC[i].NN_SUPPORT_ALU == 1 ? chipMinorFeatures16_NN_SUPPORT_ALU : 0) |
				(gChipInfo_AMLOGIC[i].NN_ENHANCED_MAX_POOLING == 1 ? chipMinorFeatures16_NN_ENHANCED_MAX_POOLING : 0) |
				(gChipInfo_AMLOGIC[i].NN_TRANSPOSE_PHASE2 == 1 ? chipMinorFeatures16_NN_TRANSPOSE_PHASE2 : 0) |
				(gChipInfo_AMLOGIC[i].NN_TENSOR_ADD_FIELD_MOVE_TO_EXT_CMD == 1 ? chipMinorFeatures16_NN_TENSOR_ADD_FIELD_MOVE_TO_EXT_CMD : 0) |
				//(gChipInfo_AMLOGIC[i].NN_CMD_SUPPORT_SLICE == 1 ? chipMinorFeatures16_NN_CMD_SUPPORT_SLICE : 0) |
				(gChipInfo_AMLOGIC[i].NN_CONV_CORE_BYPASS == 1 ? chipMinorFeatures16_NN_CONV_CORE_BYPASS : 0) |
				(gChipInfo_AMLOGIC[i].NN_TENSOR_ADD_RELU == 1 ? chipMinorFeatures16_NN_TENSOR_ADD_RELU : 0) |
				(gChipInfo_AMLOGIC[i].TPLITE_SUPPORT_TP_DATA_TRANSPOSE == 1 ? chipMinorFeatures16_TPLITE_SUPPORT_TP_DATA_TRANSPOSE : 0) |
				(gChipInfo_AMLOGIC[i].NN_SUPPORT_CONV_1D == 1 ? chipMinorFeatures16_NN_SUPPORT_CONV_1D : 0) |
				(gChipInfo_AMLOGIC[i].USE_VIPSRAM_FOR_KERNEL_STREAMING == 1 ? chipMinorFeatures16_USE_VIPSRAM_FOR_KERNEL_STREAMING : 0) |
				(gChipInfo_AMLOGIC[i].NN_SUPPORT_DUMMY_TILE == 1 ? chipMinorFeatures16_NN_SUPPORT_DUMMY_TILE : 0) |
				(gChipInfo_AMLOGIC[i].NN_SUPPORT_KERNEL_1BYTE_ALIGN == 1 ? chipMinorFeatures16_NN_SUPPORT_KERNEL_1BYTE_ALIGN : 0) |
				(gChipInfo_AMLOGIC[i].NN_1x1_NON_POOLING_PACKING == 1 ? chipMinorFeatures16_NN_1x1_NON_POOLING_PACKING : 0) |
				(gChipInfo_AMLOGIC[i].NN_SUPPORT_BOTH_CONV_NATIVE_STRIDE2_AND_POOLING == 1 ? chipMinorFeatures16_NN_SUPPORT_BOTH_CONV_NATIVE_STRIDE2_AND_POOLING : 0) |
				(gChipInfo_AMLOGIC[i].NN_SUPPORT_CONV1x1_AND_NATIVE_CONV_STRIDE2 == 1 ? chipMinorFeatures16_NN_SUPPORT_CONV1x1_AND_NATIVE_CONV_STRIDE2 : 0) |
				(gChipInfo_AMLOGIC[i].TP_REMOVE_FC == 1 ? chipMinorFeatures16_TP_REMOVE_FC : 0) |
				(gChipInfo_AMLOGIC[i].VIP_REMOVE_MMU == 1 ? chipMinorFeatures16_VIP_REMOVE_MMU : 0) |
				//(gChipInfo_AMLOGIC[i].NN_RD_IMG_NEED_EXTRA_SPACE == 1 ? chipMinorFeatures16_NN_RD_IMG_NEED_EXTRA_SPACE : 0) |
				//(gChipInfo_AMLOGIC[i].VIP_INDIV_CLK_NN == 1 ? chipMinorFeatures16_VIP_INDIV_CLK_NN : 0) |
				//(gChipInfo_AMLOGIC[i].VIP_EXPORT_CLK_DIV2 == 1 ? chipMinorFeatures16_VIP_EXPORT_CLK_DIV2 : 0) |
				(gChipInfo_AMLOGIC[i].NN_2D_AVERAGE_OUTPUT == 1 ? chipMinorFeatures16_NN_2D_AVERAGE_OUTPUT : 0) |
				(gChipInfo_AMLOGIC[i].NN_JOB_CANCELATION == 1 ? chipMinorFeatures16_NN_JOB_CANCELATION : 0) |
				//(gChipInfo_AMLOGIC[i].NN_SUPPORT_GEMM == 1 ? chipMinorFeatures16_NN_SUPPORT_GEMM : 0) |
				//(gChipInfo_AMLOGIC[i].NN_SUPPORT_INLINE_NWHC_AND_MATRIX_TRANSPOSE == 1 ? chipMinorFeatures16_NN_SUPPORT_INLINE_NWHC_AND_MATRIX_TRANSPOSE : 0) |
				(gChipInfo_AMLOGIC[i].NN_SUPPORT_BATCH == 1 ? chipMinorFeatures16_NN_SUPPORT_BATCH : 0) |
				//(gChipInfo_AMLOGIC[i].VIP_SUPPORT_DEC == 1 ? chipMinorFeatures16_VIP_SUPPORT_DEC : 0) |
				//(gChipInfo_AMLOGIC[i].NN_SUPPORT_MULTI_AXI_ID == 1 ? chipMinorFeatures16_NN_SUPPORT_MULTI_AXI_ID : 0) |
				//(gChipInfo_AMLOGIC[i].NN_POST_OUT_SUPPORT_INT32 == 1 ? chipMinorFeatures16_NN_POST_OUT_SUPPORT_INT32 : 0) |
				//(gChipInfo_AMLOGIC[i].NN_SUPPORT_STREAMPROCESSOR == 1 ? chipMinorFeatures16_NN_SUPPORT_STREAMPROCESSOR : 0) |
				(gChipInfo_AMLOGIC[i].NN_DISTRIBUTED_VIPSRAM == 1 ? chipMinorFeatures16_NN_DISTRIBUTED_VIPSRAM : 0) |
				(gChipInfo_AMLOGIC[i].NN_FC_ENHANCEMENT == 1 ? chipMinorFeatures16_NN_FC_ENHANCEMENT : 0);
			//(gChipInfo_AMLOGIC[i].NN_4BIT_PHASE1 == 1 ? chipMinorFeatures16_NN_4BIT_PHASE1 : 0);

			uint32_t features17 =
				(gChipInfo_AMLOGIC[i].VIP_DEC400 == 1 ? chipMinorFeatures17_VIP_DEC400 : 0) |
				/*(gChipInfo_AMLOGIC[i].NN_POST_MULT_SUPPORT_FP_CONV == 1 ? chipMinorFeatures17_NN_POST_MULT_SUPPORT_FP_CONV : 0) |
				(gChipInfo_AMLOGIC[i].NN_SUPPORT_16_8_QUANTIZATION == 1 ? chipMinorFeatures17_NN_SUPPORT_16_8_QUANTIZATION : 0) |
				(gChipInfo_AMLOGIC[i].SPECIAL_8BIT_SIGN_ABS_CONV == 1 ? chipMinorFeatures17_SPECIAL_8BIT_SIGN_ABS_CONV : 0) |
				(gChipInfo_AMLOGIC[i].NN_SUPPORT_CONFIGURABLE_FASTXDP3 == 1 ? chipMinorFeatures17_NN_SUPPORT_CONFIGURABLE_FASTXDP3 : 0) |
				(gChipInfo_AMLOGIC[i].NN_USE_CORE_SHARING_IMGBUF_AND_SEQ_NO_ZEROSKIPPING == 1 ? chipMinorFeatures17_NN_USE_CORE_SHARING_IMGBUF_AND_SEQ_NO_ZEROSKIPPING : 0) |
				(gChipInfo_AMLOGIC[i].SUPPORT_DECONVNxN_S_LESS_THAN_16 == 1 ? chipMinorFeatures17_SUPPORT_DECONVNxN_S_LESS_THAN_16 : 0) |
				(gChipInfo_AMLOGIC[i].NN_PICOCORE_DEPTHWISE == 1 ? chipMinorFeatures17_NN_PICOCORE_DEPTHWISE : 0) |
				(gChipInfo_AMLOGIC[i].VIP_SUPPORT_TENSOR_TRANSFER == 1 ? chipMinorFeatures17_VIP_SUPPORT_TENSOR_TRANSFER : 0) |
				(gChipInfo_AMLOGIC[i].NN_SUPPORT_CMD_LOOP == 1 ? chipMinorFeatures17_NN_SUPPORT_CMD_LOOP : 0) |
				(gChipInfo_AMLOGIC[i].VIP_SUPPORT_X_FRAME_COMPRESSION == 1 ? chipMinorFeatures17_VIP_SUPPORT_X_FRAME_COMPRESSION : 0) |
				(gChipInfo_AMLOGIC[i].NN_SMALL_ACCUM == 1 ? chipMinorFeatures17_NN_SMALL_ACCUM : 0) |
				(gChipInfo_AMLOGIC[i].NN_SINGLE_POSTMULT_FIELDS_IN_BITSTREAM == 1 ? chipMinorFeatures17_NN_SINGLE_POSTMULT_FIELDS_IN_BITSTREAM : 0) |
				(gChipInfo_AMLOGIC[i].POST_MULTIPLIER_LOW_POWER_MODE == 1 ? chipMinorFeatures17_POST_MULTIPLIER_LOW_POWER_MODE : 0) |*/
				(gChipInfo_AMLOGIC[i].NN_PER3DTILE_BUBBLE_FIX == 1 ? chipMinorFeatures17_NN_PER3DTILE_BUBBLE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_CACHELINE_MODE_PERF_FIX == 1 ? chipMinorFeatures17_NN_CACHELINE_MODE_PERF_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_CONV1x1_PERF_FIX == 1 ? chipMinorFeatures17_NN_CONV1x1_PERF_FIX : 0) |
				(gChipInfo_AMLOGIC[i].TP_REORDER_FIX == 1 ? chipMinorFeatures17_TP_REORDER_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_CONVOUT_FIFO_DEPTH_FIX == 1 ? chipMinorFeatures17_NN_CONVOUT_FIFO_DEPTH_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_ZXDP3_KERNEL_READ_CONFLICT_FIX == 1 ? chipMinorFeatures17_NN_ZXDP3_KERNEL_READ_CONFLICT_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_ZDP3_NO_COMPRESS_FIX == 1 ? chipMinorFeatures17_NN_ZDP3_NO_COMPRESS_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_ASYNC_COPY_PERF_FIX == 1 ? chipMinorFeatures17_NN_ASYNC_COPY_PERF_FIX : 0) |
				(gChipInfo_AMLOGIC[i].HI_REORDER_FIX == 1 ? chipMinorFeatures17_HI_REORDER_FIX : 0) |
				(gChipInfo_AMLOGIC[i].INCORRECT_WR_REQ_TO_USC_BETWEEN_REORDER_AND_NORMAL_LAYER_FIX == 1 ? chipMinorFeatures17_INCORRECT_WR_REQ_TO_USC_BETWEEN_REORDER_AND_NORMAL_LAYER_FIX : 0) |
				(gChipInfo_AMLOGIC[i].TP_REORDER_LAYER_SUSPEND_FIX == 1 ? chipMinorFeatures17_TP_REORDER_LAYER_SUSPEND_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_ASYNC_COPY_MERGE_FIX == 1 ? chipMinorFeatures17_NN_ASYNC_COPY_MERGE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].USC_INVALIDATE_CACHE_LINE_FIX == 1 ? chipMinorFeatures17_USC_INVALIDATE_CACHE_LINE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_REQ_SLOWARBITRATION_FIX == 1 ? chipMinorFeatures17_NN_REQ_SLOWARBITRATION_FIX : 0) |
				(gChipInfo_AMLOGIC[i].IMAGE_PARTIAL_CACHE_FIX == 1 ? chipMinorFeatures17_IMAGE_PARTIAL_CACHE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].FULLCACHE_KERNELHEAD_FIX == 1 ? chipMinorFeatures17_FULLCACHE_KERNELHEAD_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_ZDP_INIMAGE_SIZE_FIX == 1 ? chipMinorFeatures17_NN_ZDP_INIMAGE_SIZE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].IDLE_BEFORE_FLUSH_COMPLETE_FIX == 1 ? chipMinorFeatures17_IDLE_BEFORE_FLUSH_COMPLETE_FIX : 0);

			uint32_t features18 =
				(gChipInfo_AMLOGIC[i].NO_FLUSH_USC_FIX == 1 ? chipMinorFeatures18_NO_FLUSH_USC_FIX : 0) |
				(gChipInfo_AMLOGIC[i].SMALL_BATCH_FLOPS_RESET_FIX == 1 ? chipMinorFeatures18_SMALL_BATCH_FLOPS_RESET_FIX : 0) |
				(gChipInfo_AMLOGIC[i].SMALL_BATCH_DISBLE_FIX == 1 ? chipMinorFeatures18_SMALL_BATCH_DISBLE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].OUTPUT_CONVERT_UINT8_INT8_TO_UINT16_INT16_FIX == 1 ? chipMinorFeatures18_OUTPUT_CONVERT_UINT8_INT8_TO_UINT16_INT16_FIX : 0) |
				(gChipInfo_AMLOGIC[i].IMAGE_NOT_PACKED_IN_SRAM_FIX == 1 ? chipMinorFeatures18_IMAGE_NOT_PACKED_IN_SRAM_FIX : 0) |
				(gChipInfo_AMLOGIC[i].COEF_DELTA_CORD_OVERFLOW_ZRL_8BIT_FIX == 1 ? chipMinorFeatures18_COEF_DELTA_CORD_OVERFLOW_ZRL_8BIT_FIX : 0) |
				(gChipInfo_AMLOGIC[i].USC_INDIVIDUAL_PORT_WRT_EARLY_EVICT_DATA_CORRUPT_FIX == 1 ? chipMinorFeatures18_USC_INDIVIDUAL_PORT_WRT_EARLY_EVICT_DATA_CORRUPT_FIX : 0) |
				(gChipInfo_AMLOGIC[i].LOW_EFFICIENCY_OF_ID_WRITE_IMGBUF_FIX == 1 ? chipMinorFeatures18_LOW_EFFICIENCY_OF_ID_WRITE_IMGBUF_FIX : 0) |
				(gChipInfo_AMLOGIC[i].KERNEL_VIP_SRAM_READ_BW_LIMITATION_FIX == 1 ? chipMinorFeatures18_KERNEL_VIP_SRAM_READ_BW_LIMITATION_FIX : 0) |
				(gChipInfo_AMLOGIC[i].USC_BOTTLENECK_FIX == 1 ? chipMinorFeatures18_USC_BOTTLENECK_FIX : 0) |
				(gChipInfo_AMLOGIC[i].KERNEL_PER_CORE_LESS_THAN_THIRD_COEF_BUFF_DEPTH_FIX == 1 ? chipMinorFeatures18_KERNEL_PER_CORE_LESS_THAN_THIRD_COEF_BUFF_DEPTH_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_TILE_NUM_BIGGER_THAN_1024_FIX == 1 ? chipMinorFeatures18_NN_TILE_NUM_BIGGER_THAN_1024_FIX : 0) |
				(gChipInfo_AMLOGIC[i].KERNEL_SIZE_WASTE_IN_PARTIAL_MODE_FIX == 1 ? chipMinorFeatures18_KERNEL_SIZE_WASTE_IN_PARTIAL_MODE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_COMMAND_KERNEL_REQUEST_CONFICT_FIX == 1 ? chipMinorFeatures18_NN_COMMAND_KERNEL_REQUEST_CONFICT_FIX : 0) |
				(gChipInfo_AMLOGIC[i].TP_REORDER_INTILE_X_SIZE_512_FIX == 1 ? chipMinorFeatures18_TP_REORDER_INTILE_X_SIZE_512_FIX : 0) |
				(gChipInfo_AMLOGIC[i].IMG_POP_PIPELINE_PAUSE_FIX == 1 ? chipMinorFeatures18_IMG_POP_PIPELINE_PAUSE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].FULLCACHE_KERNEL_INTERLEAVE_FIX == 1 ? chipMinorFeatures18_FULLCACHE_KERNEL_INTERLEAVE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].V8_SINGLE_PORT_ACCUMULATION_BUFFER_RW_CONFICT_ZERO_SKIP_PERF_FIX == 1 ? chipMinorFeatures18_V8_SINGLE_PORT_ACCUMULATION_BUFFER_RW_CONFICT_ZERO_SKIP_PERF_FIX : 0) |
				(gChipInfo_AMLOGIC[i].V8_ACCUMLATION_READ_OUT_HAS_BUBBLES_PERF_FIX == 1 ? chipMinorFeatures18_V8_ACCUMLATION_READ_OUT_HAS_BUBBLES_PERF_FIX : 0) |
				(gChipInfo_AMLOGIC[i].DEPTHWISE_NEIGHBOR_IMG_DATA_TRANSFER_NOT_EFFICIENT_FIX == 1 ? chipMinorFeatures18_DEPTHWISE_NEIGHBOR_IMG_DATA_TRANSFER_NOT_EFFICIENT_FIX : 0) |
				(gChipInfo_AMLOGIC[i].DR_JD_DIFF_CONDITION_FOR_CACHELINE_MODE_PRE_FIX == 1 ? chipMinorFeatures18_DR_JD_DIFF_CONDITION_FOR_CACHELINE_MODE_PRE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].TP_ACCESS_VIPSRAM_OT_IS_ONE_FIX == 1 ? chipMinorFeatures18_TP_ACCESS_VIPSRAM_OT_IS_ONE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].EVIS2_FLOP_RESET_FIX == 1 ? chipMinorFeatures18_EVIS2_FLOP_RESET_FIX : 0) |
				(gChipInfo_AMLOGIC[i].OUTIMAGE_X_BITWIDTH_LIMIT_FOR_NN_TRANSPOSE_FIX == 1 ? chipMinorFeatures18_OUTIMAGE_X_BITWIDTH_LIMIT_FOR_NN_TRANSPOSE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].USC_ASYNC_CP_RTN_FLOP_RESET_FIX == 1 ? chipMinorFeatures18_USC_ASYNC_CP_RTN_FLOP_RESET_FIX : 0) |
				(gChipInfo_AMLOGIC[i].IMG_ADDR_NOT_WRAP_IF_OVER_OCB_ADDR_FIX == 1 ? chipMinorFeatures18_IMG_ADDR_NOT_WRAP_IF_OVER_OCB_ADDR_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NEGATIVE_POST_SHIFT_FIX == 1 ? chipMinorFeatures18_NEGATIVE_POST_SHIFT_FIX : 0) |
				(gChipInfo_AMLOGIC[i].INIMAGE_2DTILE_NOT_LESS_160PIXEL_FIX == 1 ? chipMinorFeatures18_INIMAGE_2DTILE_NOT_LESS_160PIXEL_FIX : 0) |
				(gChipInfo_AMLOGIC[i].IMG_CAHCE_MODE_MUST_0_IN_IMG_DIRECT_MODE_FIX == 1 ? chipMinorFeatures18_IMG_CAHCE_MODE_MUST_0_IN_IMG_DIRECT_MODE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].BURST_COLLECT_DUMMY_DATA_WASTE_CYCLES_FIX == 1 ? chipMinorFeatures18_BURST_COLLECT_DUMMY_DATA_WASTE_CYCLES_FIX : 0) |
				(gChipInfo_AMLOGIC[i].INIMG_NOT_64BYTE_ALIGN_CACHELINE_MODE_FIX == 1 ? chipMinorFeatures18_INIMG_NOT_64BYTE_ALIGN_CACHELINE_MODE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].TP_FC_FLOAT_LAST_PIXEL_NEGATIVE_0_FIX == 1 ? chipMinorFeatures18_TP_FC_FLOAT_LAST_PIXEL_NEGATIVE_0_FIX : 0);

			uint32_t features19 =
				(gChipInfo_AMLOGIC[i].NN_WASTE_COEF_READ_WRITE_BANDWIDTH_128BYTE_VIPSRAM_IN_FULL_PATIAL_CACHE_MODE_FIX == 1 ? chipMinorFeatures19_NN_WASTE_COEF_READ_WRITE_BANDWIDTH_128BYTE_VIPSRAM_IN_FULL_PATIAL_CACHE_MODE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_IN_TILE_DATA_IS_ALL_PAD_FIX == 1 ? chipMinorFeatures19_NN_IN_TILE_DATA_IS_ALL_PAD_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_TP_INSTR_COMPLETE_IN_SAME_CYCLE_WITH_WAIT_EVENT_FIX == 1 ? chipMinorFeatures19_NN_TP_INSTR_COMPLETE_IN_SAME_CYCLE_WITH_WAIT_EVENT_FIX : 0) |
				(gChipInfo_AMLOGIC[i].CORE_IMAGE_TRANSER_NOT_EFFICIENT_BETWEEN_PARTITION_FIX == 1 ? chipMinorFeatures19_CORE_IMAGE_TRANSER_NOT_EFFICIENT_BETWEEN_PARTITION_FIX : 0) |
				(gChipInfo_AMLOGIC[i].TP_FC_KERNEL_STREAM_MUST_LESS_THAN_OR_EQUAL_TO_64BYTE_WHEN_1BYTE_ALGINE_FIX == 1 ? chipMinorFeatures19_TP_FC_KERNEL_STREAM_MUST_LESS_THAN_OR_EQUAL_TO_64BYTE_WHEN_1BYTE_ALGINE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_KERNEL_1x1_NO_PAD_FIX == 1 ? chipMinorFeatures19_NN_KERNEL_1x1_NO_PAD_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_DEPTHWISE_AFTER_16BIT_LAYER_LIMIT_FIX == 1 ? chipMinorFeatures19_NN_DEPTHWISE_AFTER_16BIT_LAYER_LIMIT_FIX : 0) |
				(gChipInfo_AMLOGIC[i].TP_NOT_FULL_USE_CACHE_LINE_FIX == 1 ? chipMinorFeatures19_TP_NOT_FULL_USE_CACHE_LINE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].SH_MOVAI_MOVAR_UNUSED_COMPONENTS_WRITE_DIRTY_DATA_FIX == 1 ? chipMinorFeatures19_SH_MOVAI_MOVAR_UNUSED_COMPONENTS_WRITE_DIRTY_DATA_FIX : 0) |
				(gChipInfo_AMLOGIC[i].BURST_COLLECT_CONSUMES_MC_DATA_WIDTH_PER_CYCLE_FIX == 1 ? chipMinorFeatures19_BURST_COLLECT_CONSUMES_MC_DATA_WIDTH_PER_CYCLE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].TP_ASSYM_INT8_FIX == 1 ? chipMinorFeatures19_TP_ASSYM_INT8_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_PAD_SLICE_ERROR_WHEN_TRANSPSE_FIX == 1 ? chipMinorFeatures19_NN_PAD_SLICE_ERROR_WHEN_TRANSPSE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_2ND_IMG_BASE_ADDR_FIX == 1 ? chipMinorFeatures19_NN_2ND_IMG_BASE_ADDR_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_TP_SYSTEM_FIX == 1 ? chipMinorFeatures19_NN_TP_SYSTEM_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_INTILE_YSIZE_128_LIMIT_FIX == 1 ? chipMinorFeatures19_NN_INTILE_YSIZE_128_LIMIT_FIX : 0) |
				(gChipInfo_AMLOGIC[i].SH_CLOCK_GATOR_IDLE_CONDITON_FIX == 1 ? chipMinorFeatures19_SH_CLOCK_GATOR_IDLE_CONDITON_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_BURST_COLLECTER_LAST_FLAG_FIX == 1 ? chipMinorFeatures19_NN_BURST_COLLECTER_LAST_FLAG_FIX : 0) |
				//(gChipInfo_AMLOGIC[i].V83_CONVERTER_FOR_NEG_0_FIX == 1 ? chipMinorFeatures19_V83_CONVERTER_FOR_NEG_0_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_2ND_IMG_SMALL_3D_TILE_FIX == 1 ? chipMinorFeatures19_NN_2ND_IMG_SMALL_3D_TILE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_TILE_YSIZE_127_LIMITATION_FIX == 1 ? chipMinorFeatures19_NN_TILE_YSIZE_127_LIMITATION_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_CONV_1D_16BIT_FORMAT_INTILE_SIZE_LIMITATION_FIX == 1 ? chipMinorFeatures19_NN_CONV_1D_16BIT_FORMAT_INTILE_SIZE_LIMITATION_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_VIPSRAM_DOUBLE_BUFFER_FIX == 1 ? chipMinorFeatures19_NN_VIPSRAM_DOUBLE_BUFFER_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_JD_DIRECT_MODE_FIX == 1 ? chipMinorFeatures19_NN_JD_DIRECT_MODE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_KERNEL_DIRECT_WRONG_PUSH_FIX == 1 ? chipMinorFeatures19_NN_KERNEL_DIRECT_WRONG_PUSH_FIX : 0) |
				(gChipInfo_AMLOGIC[i].HI_DEFAULT_ENABLE_REORDER_FIX == 1 ? chipMinorFeatures19_HI_DEFAULT_ENABLE_REORDER_FIX : 0) |
				//(gChipInfo_AMLOGIC[i].V8_DIRECT_MODE_START_ADDR_BIAS_FOR_NEGATIVE_OFFSET_FIX == 1 ? chipMinorFeatures19_V8_DIRECT_MODE_START_ADDR_BIAS_FOR_NEGATIVE_OFFSET_FIX : 0) |
				(gChipInfo_AMLOGIC[i].V83_INTILESIZE_1X1_10BITS_FIX == 1 ? chipMinorFeatures19_V83_INTILESIZE_1X1_10BITS_FIX : 0);
				//(gChipInfo_AMLOGIC[i].FASTXDP3_ONLY_IN_DEPTHWISE_FIX == 1 ? chipMinorFeatures19_FASTXDP3_ONLY_IN_DEPTHWISE_FIX : 0) |
				//(gChipInfo_AMLOGIC[i].USC_PAUSE_TP_WR_REQ_MORE_THAN_256_CYCLES_FIX == 1 ? chipMinorFeatures19_USC_PAUSE_TP_WR_REQ_MORE_THAN_256_CYCLES_FIX : 0) |
				//(gChipInfo_AMLOGIC[i].DEPTHWISE_FLOAT_FIX == 1 ? chipMinorFeatures19_DEPTHWISE_FLOAT_FIX : 0) |
				//(gChipInfo_AMLOGIC[i].TP_CIRCULAR_BUF_WRAP_ADDRESS_OVERFLOW_FIX == 1 ? chipMinorFeatures19_TP_CIRCULAR_BUF_WRAP_ADDRESS_OVERFLOW_FIX : 0) |
				//(gChipInfo_AMLOGIC[i].NN_CIRCULAR_BUF_WRAP_ADDRESS_OVERFLOW_FIX == 1 ? chipMinorFeatures19_NN_CIRCULAR_BUF_WRAP_ADDRESS_OVERFLOW_FIX : 0);

			uint32_t features20 =
				//(gChipInfo_AMLOGIC[i].CLOCK_DIV2_FREQ_CHANGE_FIX == 1 ? chipMinorFeatures20_CLOCK_DIV2_FREQ_CHANGE_FIX : 0) |
				//(gChipInfo_AMLOGIC[i].SMALL_TILE_TENSOR_ADD_FIX == 1 ? chipMinorFeatures20_SMALL_TILE_TENSOR_ADD_FIX : 0) |
				//(gChipInfo_AMLOGIC[i].DECOMPRESSOR_DEPTHWISE_FLOAT_FIX == 1 ? chipMinorFeatures20_DECOMPRESSOR_DEPTHWISE_FLOAT_FIX : 0) |
				/*(gChipInfo_AMLOGIC[i].TP_CIRCULAR_BUF_WRAP_ADDRESS_LESS_FIX == 1 ? chipMinorFeatures20_TP_CIRCULAR_BUF_WRAP_ADDRESS_LESS_FIX : 0) |
				(gChipInfo_AMLOGIC[i].V83_NUMOFPENDINGTILES_FOR_2NDIMAGE_FIX == 1 ? chipMinorFeatures20_V83_NUMOFPENDINGTILES_FOR_2NDIMAGE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].V83_1ST_CACHE_MODE_VIPSRAM_RD_UPDATE_FIX == 1 ? chipMinorFeatures20_V83_1ST_CACHE_MODE_VIPSRAM_RD_UPDATE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].V83_1ST_KERNEL_STREAM_BUFFER_UPDATE_FIX == 1 ? chipMinorFeatures20_V83_1ST_KERNEL_STREAM_BUFFER_UPDATE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].USC_RW_SAME_CACHELINE_UPDATE_FIX == 1 ? chipMinorFeatures20_USC_RW_SAME_CACHELINE_UPDATE_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_KERNEL_MSS_SBP2_DIRECT_STEAM_STEAM_FIX == 1 ? chipMinorFeatures20_NN_KERNEL_MSS_SBP2_DIRECT_STEAM_STEAM_FIX : 0) |
				(gChipInfo_AMLOGIC[i].CORE_NUM_OF_KID_FOR_MULTI_LAYER_FIX == 1 ? chipMinorFeatures20_CORE_NUM_OF_KID_FOR_MULTI_LAYER_FIX : 0) |
				(gChipInfo_AMLOGIC[i].KERNEL_XSIZE_YSIZE_NUM_FIX == 1 ? chipMinorFeatures20_KERNEL_XSIZE_YSIZE_NUM_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_1ST_AND_2ND_INIMAGE_RAISE_VIPSRAM_RD_UPDATE_AT_SAME_TIME_FIX == 1 ? chipMinorFeatures20_NN_1ST_AND_2ND_INIMAGE_RAISE_VIPSRAM_RD_UPDATE_AT_SAME_TIME_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_1ST_AND_2ND_INIMAGE_RAISE_VIPSRAM_RD_UPDATE_AT_SAME_TIME_PHASE1_FIX == 1 ? chipMinorFeatures20_NN_1ST_AND_2ND_INIMAGE_RAISE_VIPSRAM_RD_UPDATE_AT_SAME_TIME_PHASE1_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_1ST_AND_2ND_INIMAGE_RAISE_VIPSRAM_RD_UPDATE_AT_SAME_TIME_PHASE2_FIX == 1 ? chipMinorFeatures20_NN_1ST_AND_2ND_INIMAGE_RAISE_VIPSRAM_RD_UPDATE_AT_SAME_TIME_PHASE2_FIX : 0) |
				(gChipInfo_AMLOGIC[i].TP_SPECIAL_LIST_PARSER_FIX == 1 ? chipMinorFeatures20_TP_SPECIAL_LIST_PARSER_FIX : 0) |
				(gChipInfo_AMLOGIC[i].DECOMPRESSOR_TREATS_TOTAL_CORES_AS_ACTIVE_CORES_FIX == 1 ? chipMinorFeatures20_DECOMPRESSOR_TREATS_TOTAL_CORES_AS_ACTIVE_CORES_FIX : 0) |
				(gChipInfo_AMLOGIC[i].DIRECT_INIMAGE_XSTIDE_LE_13BIT_FIX == 1 ? chipMinorFeatures20_DIRECT_INIMAGE_XSTIDE_LE_13BIT_FIX : 0) |
				(gChipInfo_AMLOGIC[i].SECONDIMG_TILE_SIDEBANFIFO_FIX == 1 ? chipMinorFeatures20_SECONDIMG_TILE_SIDEBANFIFO_FIX : 0) |
				(gChipInfo_AMLOGIC[i].TRSPB2_ENDADDR_EQUAL_SRAMEND_FIX == 1 ? chipMinorFeatures20_TRSPB2_ENDADDR_EQUAL_SRAMEND_FIX : 0) |*/
				(gChipInfo_AMLOGIC[i].NN_FP16_ALU == 1 ? chipMinorFeatures20_NN_FP16_ALU : 0) |
				(gChipInfo_AMLOGIC[i].NN_INT16_ALU == 1 ? chipMinorFeatures20_NN_INT16_ALU : 0) |
				(gChipInfo_AMLOGIC[i].NN_INT8_SCALE == 1 ? chipMinorFeatures20_NN_INT8_SCALE : 0) |
				(gChipInfo_AMLOGIC[i].NN_POWER_ISOLATION == 1 ? chipMinorFeatures20_NN_POWER_ISOLATION : 0) |
				(gChipInfo_AMLOGIC[i].ZRL_7BIT == 1 ? chipMinorFeatures20_ZRL_7BIT : 0) |
				(gChipInfo_AMLOGIC[i].NN_SMALLBATCH == 1 ? chipMinorFeatures20_NN_SMALLBATCH : 0) |
				(gChipInfo_AMLOGIC[i].TP_SMALLBATCH == 1 ? chipMinorFeatures20_TP_SMALLBATCH : 0) |
				(gChipInfo_AMLOGIC[i].ZRL_8BIT == 1 ? chipMinorFeatures20_ZRL_8BIT : 0) |
				(gChipInfo_AMLOGIC[i].DDR_BURST_LEN_256B == 1 ? chipMinorFeatures20_DDR_BURST_LEN_256B : 0) |
				(gChipInfo_AMLOGIC[i].XY_OFFSET_LIMITATION_FIX == 1 ? chipMinorFeatures20_XY_OFFSET_LIMITATION_FIX : 0) |
				(gChipInfo_AMLOGIC[i].NN_NONZERO_MIRROR_BORDER == 1 ? chipMinorFeatures20_NN_NONZERO_MIRROR_BORDER : 0) |
				(gChipInfo_AMLOGIC[i].IMAGE_PARTIAL_CACHE == 1 ? chipMinorFeatures20_IMAGE_PARTIAL_CACHE : 0);
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
			linux_devices[count].minor_features14 = features14;
			linux_devices[count].minor_features15 = features15;
			linux_devices[count].minor_features16 = features16;
			linux_devices[count].minor_features17 = features17;
			linux_devices[count].minor_features18 = features18;
			linux_devices[count].minor_features19 = features19;
			linux_devices[count].minor_features20 = features20;
#endif

#if defined NN_CORE_COUNT
			linux_devices[count].nn_core_count = gChipInfo_AMLOGIC[i].NNCoreCount;
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
#if defined EXTENDED_FEATURE_LIST
				(gChipInfo_STM32_6_2_4[i].TX_SEAMLESS_CUBE == 1 ? chipMinorFeatures11_TX_SEAMLESS_CUBE : 0) |
				(gChipInfo_STM32_6_2_4[i].TX_SNORM_SUPPORT == 1 ? chipMinorFeatures11_TX_SNORM_SUPPORT : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_SCATTER_GATHER == 1 ? chipMinorFeatures11_SH_SCATTER_GATHER : 0) |
				(gChipInfo_STM32_6_2_4[i].HWMANAGED_LS == 1 ? chipMinorFeatures11_HWMANAGED_LS : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_IMAGE_ENABLE_FIX == 1 ? chipMinorFeatures11_SH_IMAGE_ENABLE_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].MSAA_FRAGMENT_OPERATION == 1 ? chipMinorFeatures11_MSAA_FRAGMENT_OPERATION : 0) |
				(gChipInfo_STM32_6_2_4[i].PE_TILE_CACHE_FLUSH_FIX == 1 ? chipMinorFeatures11_PE_TILE_CACHE_FLUSH_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].BLT_YUV_OUTPUT == 1 ? chipMinorFeatures11_BLT_YUV_OUTPUT : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_IO_CG_FIX == 1 ? chipMinorFeatures11_SH_IO_CG_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].SH_ROBUSTNESS_FIX == 1 ? chipMinorFeatures11_SH_ROBUSTNESS_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].USC_ATOMIC_FIX2 == 1 ? chipMinorFeatures11_USC_ATOMIC_FIX2 : 0) |
				(gChipInfo_STM32_6_2_4[i].PE_A8B8G8R8 == 1 ? chipMinorFeatures11_PE_A8B8G8R8 : 0) |
				(gChipInfo_STM32_6_2_4[i].MULTIVIEW_RENDER == 1 ? chipMinorFeatures11_MULTIVIEW_RENDER : 0) |
				(gChipInfo_STM32_6_2_4[i].FE_DRAW_DIRECT == 1 ? chipMinorFeatures11_FE_DRAW_DIRECT : 0) |
				(gChipInfo_STM32_6_2_4[i].TX_VKBORDER_MODE == 1 ? chipMinorFeatures11_TX_VKBORDER_MODE : 0) |
				(gChipInfo_STM32_6_2_4[i].TX_UNNORMALIZED_COORD == 1 ? chipMinorFeatures11_TX_UNNORMALIZED_COORD : 0) |
				(gChipInfo_STM32_6_2_4[i].PA_LINECLIP_FIX == 1 ? chipMinorFeatures11_PA_LINECLIP_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].TX_8bit_UVFrac_ROUNDING_FIX == 1 ? chipMinorFeatures11_TX_8bit_UVFrac_ROUNDING_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].MP_ARCH == 1 ? chipMinorFeatures11_MP_ARCH : 0) |
#endif
				(gChipInfo_STM32_6_2_4[i].TP_REORDER == 1 ? chipMinorFeatures11_TP_REORDER : 0);

#if defined EXTENDED_FEATURE_LIST
			uint32_t features12 = 0;

			uint32_t features13 =
				(gChipInfo_STM32_6_2_4[i].VG_IMAGE_16K == 1 ? chipMinorFeatures13_VG_IMAGE_16K : 0) |
				(gChipInfo_STM32_6_2_4[i].VG_FORMAT_ARGB2222 == 1 ? chipMinorFeatures13_VG_FORMAT_ARGB2222 : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_SINGLEPORT_ACCUMBUFFER == 1 ? chipMinorFeatures13_NN_SINGLEPORT_ACCUMBUFFER : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_STRIDE_SUPPORT == 1 ? chipMinorFeatures13_NN_STRIDE_SUPPORT : 0) |
				(gChipInfo_STM32_6_2_4[i].SWTILING_PHASE1 == 1 ? chipMinorFeatures13_SWTILING_PHASE1 : 0) |
				(gChipInfo_STM32_6_2_4[i].SWTILING_PHASE2 == 1 ? chipMinorFeatures13_SWTILING_PHASE2 : 0) |
				(gChipInfo_STM32_6_2_4[i].TP_SIMPLE_INT16 == 1 ? chipMinorFeatures13_TP_SIMPLE_INT16 : 0) |
				(gChipInfo_STM32_6_2_4[i].TP_REAL_INT16 == 1 ? chipMinorFeatures13_TP_REAL_INT16 : 0) |
				(gChipInfo_STM32_6_2_4[i].TP_ROI_POOLING == 1 ? chipMinorFeatures13_TP_ROI_POOLING : 0) |
				(gChipInfo_STM32_6_2_4[i].TP_MAX_POOLING_STRIDE1 == 1 ? chipMinorFeatures13_TP_MAX_POOLING_STRIDE1 : 0) |
				(gChipInfo_STM32_6_2_4[i].TP_LRN == 1 ? chipMinorFeatures13_TP_LRN : 0) |
				(gChipInfo_STM32_6_2_4[i].TF_QUANTIZATION == 1 ? chipMinorFeatures13_TF_QUANTIZATION : 0);

			uint32_t features14 =
				(gChipInfo_STM32_6_2_4[i].NN_ZDP6 == 1 ? chipMinorFeatures14_NN_ZDP6 : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_XYDP9 == 1 ? chipMinorFeatures14_NN_XYDP9 : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_FIRST_PIXEL_POOLING == 1 ? chipMinorFeatures14_NN_FIRST_PIXEL_POOLING : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_ZDP3 == 1 ? chipMinorFeatures14_NN_ZDP3 : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_XYDP6 == 1 ? chipMinorFeatures14_NN_XYDP6 : 0) |
				(gChipInfo_STM32_6_2_4[i].SWTILING_PHASE3 == 1 ? chipMinorFeatures14_SWTILING_PHASE3 : 0) |
				(gChipInfo_STM32_6_2_4[i].USC_STAY_LRU == 1 ? chipMinorFeatures14_USC_STAY_LRU : 0) |
				(gChipInfo_STM32_6_2_4[i].COEF_COMPRESSION_ENHANCEMENT == 1 ? chipMinorFeatures14_COEF_COMPRESSION_ENHANCEMENT : 0) |
				(gChipInfo_STM32_6_2_4[i].TP_COEF_COMPRESSION_ENHANCEMENT == 1 ? chipMinorFeatures14_TP_COEF_COMPRESSION_ENHANCEMENT : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_COEF_DECOMPRESS_PERF2X == 1 ? chipMinorFeatures14_NN_COEF_DECOMPRESS_PERF2X : 0) |
				(gChipInfo_STM32_6_2_4[i].TP_SMALLBATCH_PHASE1 == 1 ? chipMinorFeatures14_TP_SMALLBATCH_PHASE1 : 0) |
				(gChipInfo_STM32_6_2_4[i].OCB_COUNTER == 1 ? chipMinorFeatures14_OCB_COUNTER : 0) |
				(gChipInfo_STM32_6_2_4[i].SCALER == 1 ? chipMinorFeatures14_SCALER : 0) |
				(gChipInfo_STM32_6_2_4[i].INPUT_4BIT == 1 ? chipMinorFeatures14_INPUT_4BIT : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_DEPTHWISE_SUPPORT == 1 ? chipMinorFeatures14_NN_DEPTHWISE_SUPPORT : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_XYDP0 == 1 ? chipMinorFeatures14_NN_XYDP0 : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_WRITE_WITHOUT_USC == 1 ? chipMinorFeatures14_NN_WRITE_WITHOUT_USC : 0);

			uint32_t features15 =
				(gChipInfo_STM32_6_2_4[i].NN_SMALLBATCH_PHASE1 == 1 ? chipMinorFeatures15_NN_SMALLBATCH_PHASE1 : 0);

			uint32_t features16 = 0;

			uint32_t features17 =
				(gChipInfo_STM32_6_2_4[i].VIP_DEC400 == 1 ? chipMinorFeatures17_VIP_DEC400 : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_PER3DTILE_BUBBLE_FIX == 1 ? chipMinorFeatures17_NN_PER3DTILE_BUBBLE_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_CACHELINE_MODE_PERF_FIX == 1 ? chipMinorFeatures17_NN_CACHELINE_MODE_PERF_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_CONV1x1_PERF_FIX == 1 ? chipMinorFeatures17_NN_CONV1x1_PERF_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].TP_REORDER_FIX == 1 ? chipMinorFeatures17_TP_REORDER_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_CONVOUT_FIFO_DEPTH_FIX == 1 ? chipMinorFeatures17_NN_CONVOUT_FIFO_DEPTH_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_ZXDP3_KERNEL_READ_CONFLICT_FIX == 1 ? chipMinorFeatures17_NN_ZXDP3_KERNEL_READ_CONFLICT_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_ZDP3_NO_COMPRESS_FIX == 1 ? chipMinorFeatures17_NN_ZDP3_NO_COMPRESS_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_ASYNC_COPY_PERF_FIX == 1 ? chipMinorFeatures17_NN_ASYNC_COPY_PERF_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].HI_REORDER_FIX == 1 ? chipMinorFeatures17_HI_REORDER_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_ASYNC_COPY_MERGE_FIX == 1 ? chipMinorFeatures17_NN_ASYNC_COPY_MERGE_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_REQ_SLOWARBITRATION_FIX == 1 ? chipMinorFeatures17_NN_REQ_SLOWARBITRATION_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].FULLCACHE_KERNELHEAD_FIX == 1 ? chipMinorFeatures17_FULLCACHE_KERNELHEAD_FIX : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_ZDP_INIMAGE_SIZE_FIX == 1 ? chipMinorFeatures17_NN_ZDP_INIMAGE_SIZE_FIX : 0);

			uint32_t features18 = 0; 

			uint32_t features19 = 0;

			uint32_t features20 =
				(gChipInfo_STM32_6_2_4[i].NN_FP16_ALU == 1 ? chipMinorFeatures20_NN_FP16_ALU : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_INT16_ALU == 1 ? chipMinorFeatures20_NN_INT16_ALU : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_INT8_SCALE == 1 ? chipMinorFeatures20_NN_INT8_SCALE : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_POWER_ISOLATION == 1 ? chipMinorFeatures20_NN_POWER_ISOLATION : 0) |
				(gChipInfo_STM32_6_2_4[i].ZRL_7BIT == 1 ? chipMinorFeatures20_ZRL_7BIT : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_SMALLBATCH == 1 ? chipMinorFeatures20_NN_SMALLBATCH : 0) |
				(gChipInfo_STM32_6_2_4[i].TP_SMALLBATCH == 1 ? chipMinorFeatures20_TP_SMALLBATCH : 0) |
				(gChipInfo_STM32_6_2_4[i].ZRL_8BIT == 1 ? chipMinorFeatures20_ZRL_8BIT : 0) |
				(gChipInfo_STM32_6_2_4[i].NN_NONZERO_MIRROR_BORDER == 1 ? chipMinorFeatures20_NN_NONZERO_MIRROR_BORDER : 0) |
				(gChipInfo_STM32_6_2_4[i].IMAGE_PARTIAL_CACHE == 1 ? chipMinorFeatures20_IMAGE_PARTIAL_CACHE : 0);
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
			linux_devices[count].minor_features14 = features14;
			linux_devices[count].minor_features15 = features15;
			linux_devices[count].minor_features16 = features16;
			linux_devices[count].minor_features17 = features17;
			linux_devices[count].minor_features18 = features18;
			linux_devices[count].minor_features19 = features19;
			linux_devices[count].minor_features20 = features20;
#endif

#if defined NN_CORE_COUNT
			linux_devices[count].nn_core_count = gChipInfo_STM32_6_2_4[i].NNCoreCount;
#endif
			count++;
		}
	}

	// get device entries from CSKY's gc_festures_database.h (2018-03-16)
	// NXP's NN_FLOAT32_IO is reneamed to NN_FLOAT
	n = sizeof(gChipInfo_csky) / sizeof(gChipInfo_csky[0]);
	for (int i = 0; i < n; i++) {
		bool found = false;

		for (int j = 0; j < device_count; ++j)
		{

			if ((devices[j].chipID == gChipInfo_csky[i].chipID)
				&& (devices[j].chipVersion == gChipInfo_csky[i].chipVersion)
				&& (devices[j].productID == gChipInfo_csky[i].productID)
				&& (devices[j].ecoID == gChipInfo_csky[i].ecoID)
				&& (devices[j].customerID == gChipInfo_csky[i].customerID)
				)
			{
				found = true;
			}
		}

		if (found == false) {
			devices[count].chipID = gChipInfo_csky[i].chipID;
			devices[count].chipVersion = gChipInfo_csky[i].chipVersion;
			devices[count].productID = gChipInfo_csky[i].productID;
			devices[count].ecoID = gChipInfo_csky[i].ecoID;
			devices[count].customerID = gChipInfo_csky[i].customerID;

			linux_devices[count].model = gChipInfo_csky[i].chipID;
			linux_devices[count].revision = gChipInfo_csky[i].chipVersion;
			linux_devices[count].product_id = gChipInfo_csky[i].productID;
			linux_devices[count].customer_id = gChipInfo_csky[i].customerID;
			linux_devices[count].eco_id = gChipInfo_csky[i].ecoID;

			linux_devices[count].stream_count = gChipInfo_csky[i].Streams;
			linux_devices[count].register_max = gChipInfo_csky[i].TempRegisters;
			linux_devices[count].thread_count = gChipInfo_csky[i].ThreadCount;
			linux_devices[count].shader_core_count = gChipInfo_csky[i].NumShaderCores;
			linux_devices[count].vertex_cache_size = gChipInfo_csky[i].VertexCacheSize;
			linux_devices[count].vertex_output_buffer_size = gChipInfo_csky[i].VertexOutputBufferSize;
			linux_devices[count].pixel_pipes = gChipInfo_csky[i].NumPixelPipes;
			linux_devices[count].instruction_count = gChipInfo_csky[i].InstructionCount;
			linux_devices[count].num_constants = gChipInfo_csky[i].NumberOfConstants;
			linux_devices[count].buffer_size = gChipInfo_csky[i].BufferSize;
			linux_devices[count].varyings_count = gChipInfo_csky[i].VaryingCount;

			uint32_t feature = (gChipInfo_csky[i].REG_FastClear == 1 ? chipFeatures_FAST_CLEAR : 0) |
				(gChipInfo_csky[i].REG_SpecialAntiAliasing == 1 ? chipFeatures_SPECIAL_ANTI_ALIASING : 0) |
				(gChipInfo_csky[i].REG_Pipe3D == 1 ? chipFeatures_PIPE_3D : 0) |
				(gChipInfo_csky[i].REG_DXTTextureCompression == 1 ? chipFeatures_DXT_TEXTURE_COMPRESSION : 0) |
				(gChipInfo_csky[i].REG_DebugMode == 1 ? chipFeatures_DEBUG_MODE : 0) |
				(gChipInfo_csky[i].REG_ZCompression == 1 ? chipFeatures_Z_COMPRESSION : 0) |
				(gChipInfo_csky[i].REG_YUV420Filter == 1 ? chipFeatures_YUV420_SCALER : 0) |
				(gChipInfo_csky[i].REG_MSAA == 1 ? chipFeatures_MSAA : 0) |
				(gChipInfo_csky[i].REG_DC == 1 ? chipFeatures_DC : 0) |
				(gChipInfo_csky[i].REG_Pipe2D == 1 ? chipFeatures_PIPE_2D : 0) |
				(gChipInfo_csky[i].REG_ETC1TextureCompression == 1 ? chipFeatures_ETC1_TEXTURE_COMPRESSION : 0) |
				(gChipInfo_csky[i].REG_FastScaler == 1 ? chipFeatures_FAST_SCALER : 0) |
				(gChipInfo_csky[i].REG_HighDynamicRange == 1 ? chipFeatures_HIGH_DYNAMIC_RANGE : 0) |
				(gChipInfo_csky[i].REG_YUV420Tiler == 1 ? chipFeatures_YUV420_TILER : 0) |
				(gChipInfo_csky[i].REG_ModuleCG == 1 ? chipFeatures_MODULE_CG : 0) |
				(gChipInfo_csky[i].REG_MinArea == 1 ? chipFeatures_MIN_AREA : 0) |
				(gChipInfo_csky[i].REG_NoEZ == 1 ? chipFeatures_NO_EARLY_Z : 0) |
				(gChipInfo_csky[i].REG_No422Texture == 1 ? chipFeatures_NO_422_TEXTURE : 0) |
				(gChipInfo_csky[i].REG_BufferInterleaving == 1 ? chipFeatures_BUFFER_INTERLEAVING : 0) |
				(gChipInfo_csky[i].REG_ByteWrite2D == 1 ? chipFeatures_BYTE_WRITE_2D : 0) |
				(gChipInfo_csky[i].REG_NoScaler == 1 ? chipFeatures_NO_SCALER : 0) |
				(gChipInfo_csky[i].REG_YUY2Averaging == 1 ? chipFeatures_YUY2_AVERAGING : 0) |
				(gChipInfo_csky[i].REG_HalfPECache == 1 ? chipFeatures_HALF_PE_CACHE : 0) |
				(gChipInfo_csky[i].REG_HalfTXCache == 1 ? chipFeatures_HALF_TX_CACHE : 0) |
				(gChipInfo_csky[i].REG_YUY2RenderTarget == 1 ? chipFeatures_YUY2_RENDER_TARGET : 0) |
				(gChipInfo_csky[i].REG_Mem32BitSupport == 1 ? chipFeatures_MEM32 : 0) |
				(gChipInfo_csky[i].REG_PipeVG == 1 ? chipFeatures_PIPE_VG : 0) |
				(gChipInfo_csky[i].REG_VGTS == 1 ? chipFeatures_VGTS : 0) |
				(gChipInfo_csky[i].REG_FE20 == 1 ? chipFeatures_FE20 : 0) |
				(gChipInfo_csky[i].REG_ByteWrite3D == 1 ? chipFeatures_BYTE_WRITE_3D : 0) |
				(gChipInfo_csky[i].REG_RsYuvTarget == 1 ? chipFeatures_RS_YUV_TARGET : 0) |
				(gChipInfo_csky[i].REG_FE20BitIndex == 1 ? chipFeatures_32_BIT_INDICES : 0);

			uint32_t features0 =
				(gChipInfo_csky[i].REG_FlipY == 1 ? chipMinorFeatures0_FLIP_Y : 0) |
				(gChipInfo_csky[i].REG_DualReturnBus == 1 ? chipMinorFeatures0_DUAL_RETURN_BUS : 0) |
				(gChipInfo_csky[i].REG_EndiannessConfig == 1 ? chipMinorFeatures0_ENDIANNESS_CONFIG : 0) |
				(gChipInfo_csky[i].REG_Texture8K == 1 ? chipMinorFeatures0_TEXTURE_8K : 0) |
				(gChipInfo_csky[i].REG_CorrectTextureConverter == 1 ? chipMinorFeatures0_CORRECT_TEXTURE_CONVERTER : 0) |
				(gChipInfo_csky[i].REG_SpecialMsaaLod == 1 ? chipMinorFeatures0_SPECIAL_MSAA_LOD : 0) |
				(gChipInfo_csky[i].REG_FastClearFlush == 1 ? chipMinorFeatures0_FAST_CLEAR_FLUSH : 0) |
				(gChipInfo_csky[i].REG_2DPE20 == 1 ? chipMinorFeatures0_2DPE20 : 0) |
				(gChipInfo_csky[i].REG_CorrectAutoDisable == 1 ? chipMinorFeatures0_CORRECT_AUTO_DISABLE : 0) |
				(gChipInfo_csky[i].REG_Render8K == 1 ? chipMinorFeatures0_RENDERTARGET_8K : 0) |
				(gChipInfo_csky[i].REG_TileStatus2Bits == 1 ? chipMinorFeatures0_2BITPERTILE : 0) |
				(gChipInfo_csky[i].REG_SeparateTileStatusWhenInterleaved == 1 ? chipMinorFeatures0_SEPARATE_TILE_STATUS_WHEN_INTERLEAVED : 0) |
				(gChipInfo_csky[i].REG_SuperTiled32x32 == 1 ? chipMinorFeatures0_SUPER_TILED : 0) |
				(gChipInfo_csky[i].REG_VG20 == 1 ? chipMinorFeatures0_VG_20 : 0) |
				(gChipInfo_csky[i].REG_TSExtendedCommands == 1 ? chipMinorFeatures0_TS_EXTENDED_COMMANDS : 0) |
				(gChipInfo_csky[i].REG_CompressionFifoFixed == 1 ? chipMinorFeatures0_COMPRESSION_FIFO_FIXED : 0) |
#ifdef ESTIMATED_VALUE
				(gChipInfo_csky[i].REG_ExtraShaderInstructions0 == 1 ? chipMinorFeatures0_HAS_SIGN_FLOOR_CEIL : 0) | //?
				(gChipInfo_csky[i].REG_ExtraShaderInstructions1 == 1 ? chipMinorFeatures0_HAS_SQRT_TRIG : 0) | //?
				(gChipInfo_csky[i].REG_DefaultReg0 == 1 ? chipMinorFeatures0_MORE_MINOR_FEATURES : 0) | //?
#endif
				(gChipInfo_csky[i].REG_VGFilter == 1 ? chipMinorFeatures0_VG_FILTER : 0) |
				(gChipInfo_csky[i].REG_VG21 == 1 ? chipMinorFeatures0_VG_21 : 0) |
				(gChipInfo_csky[i].REG_ShaderGetsW == 1 ? chipMinorFeatures0_SHADER_HAS_W : 0) |
				(gChipInfo_csky[i].REG_MC20 == 1 ? chipMinorFeatures0_MC20 : 0) |
				(gChipInfo_csky[i].REG_ShaderMSAASideband == 1 ? chipMinorFeatures0_MSAA_SIDEBAND : 0) |
				(gChipInfo_csky[i].REG_BugFixes0 == 1 ? chipMinorFeatures0_BUG_FIXES0 : 0) |
				(gChipInfo_csky[i].REG_VAA == 1 ? chipMinorFeatures0_VAA : 0) |
				(gChipInfo_csky[i].REG_BypassInMSAA == 1 ? chipMinorFeatures0_BYPASS_IN_MSAA : 0) |
				(gChipInfo_csky[i].REG_HierarchicalZ == 1 ? chipMinorFeatures0_HZ : 0) |
				(gChipInfo_csky[i].REG_NewTexture == 1 ? chipMinorFeatures0_NEW_TEXTURE : 0) |
				(gChipInfo_csky[i].REG_A8TargetSupport == 1 ? chipMinorFeatures0_2D_A8_TARGET : 0) |
				(gChipInfo_csky[i].REG_CorrectStencil == 1 ? chipMinorFeatures0_CORRECT_STENCIL : 0) |
				(gChipInfo_csky[i].REG_EnhanceVR == 1 ? chipMinorFeatures0_ENHANCE_VR : 0);

			uint32_t features1 =
				(gChipInfo_csky[i].REG_RSUVSwizzle == 1 ? chipMinorFeatures1_RSUV_SWIZZLE : 0) |
				(gChipInfo_csky[i].REG_V2Compression == 1 ? chipMinorFeatures1_V2_COMPRESSION : 0) |
				(gChipInfo_csky[i].REG_VGDoubleBuffer == 1 ? chipMinorFeatures1_VG_DOUBLE_BUFFER : 0) |
				(gChipInfo_csky[i].REG_BugFixes1 == 1 ? chipMinorFeatures1_EXTRA_EVENT_STATES : 0) |
				(gChipInfo_csky[i].REG_BugFixes2 == 1 ? chipMinorFeatures1_NO_STRIPING_NEEDED : 0) |
				(gChipInfo_csky[i].REG_TextureStride == 1 ? chipMinorFeatures1_TEXTURE_STRIDE : 0) |
				(gChipInfo_csky[i].REG_BugFixes3 == 1 ? chipMinorFeatures1_BUG_FIXES3 : 0) |
				(gChipInfo_csky[i].REG_CorrectAutoDisable1 == 1 ? chipMinorFeatures1_AUTO_DISABLE : 0) |
				(gChipInfo_csky[i].REG_AutoRestartTS == 1 ? chipMinorFeatures1_AUTO_RESTART_TS : 0) |
				(gChipInfo_csky[i].REG_BugFixes4 == 1 ? chipMinorFeatures1_DISABLE_PE_GATING : 0) |
				(gChipInfo_csky[i].REG_L2Windowing == 1 ? chipMinorFeatures1_L2_WINDOWING : 0) |
				(gChipInfo_csky[i].REG_HalfFloatPipe == 1 ? chipMinorFeatures1_HALF_FLOAT : 0) |
				(gChipInfo_csky[i].REG_PixelDither == 1 ? chipMinorFeatures1_PIXEL_DITHER : 0) |
				(gChipInfo_csky[i].REG_TwoStencilReference == 1 ? chipMinorFeatures1_TWO_STENCIL_REFERENCE : 0) |
				(gChipInfo_csky[i].REG_ExtendedPixelFormat == 1 ? chipMinorFeatures1_EXTENDED_PIXEL_FORMAT : 0) |
				(gChipInfo_csky[i].REG_CorrectMinMaxDepth == 1 ? chipMinorFeatures1_CORRECT_MIN_MAX_DEPTH : 0) |
				(gChipInfo_csky[i].REG_DitherAndFilterPlusAlpha2D == 1 ? chipMinorFeatures1_2D_DITHER : 0) |
				(gChipInfo_csky[i].REG_BugFixes5 == 1 ? chipMinorFeatures1_BUG_FIXES5 : 0) |
				(gChipInfo_csky[i].REG_New2D == 1 ? chipMinorFeatures1_NEW_2D : 0) |
				(gChipInfo_csky[i].REG_NewFloatingPointArithmetic == 1 ? chipMinorFeatures1_NEW_FP : 0) |
				(gChipInfo_csky[i].REG_TextureHorizontalAlignmentSelect == 1 ? chipMinorFeatures1_TEXTURE_HALIGN : 0) |
				(gChipInfo_csky[i].REG_NonPowerOfTwo == 1 ? chipMinorFeatures1_NON_POWER_OF_TWO : 0) |
				(gChipInfo_csky[i].REG_LinearTextureSupport == 1 ? chipMinorFeatures1_LINEAR_TEXTURE_SUPPORT : 0) |
				(gChipInfo_csky[i].REG_Halti0 == 1 ? chipMinorFeatures1_HALTI0 : 0) |
				(gChipInfo_csky[i].REG_CorrectOverflowVG == 1 ? chipMinorFeatures1_CORRECT_OVERFLOW_VG : 0) |
				(gChipInfo_csky[i].REG_NegativeLogFix == 1 ? chipMinorFeatures1_NEGATIVE_LOG_FIX : 0) |
				(gChipInfo_csky[i].REG_ResolveOffset == 1 ? chipMinorFeatures1_RESOLVE_OFFSET : 0) |
				(gChipInfo_csky[i].REG_OkToGateAxiClock == 1 ? chipMinorFeatures1_OK_TO_GATE_AXI_CLOCK : 0) |
				(gChipInfo_csky[i].REG_MMU == 1 ? chipMinorFeatures1_MMU_VERSION : 0) |
				(gChipInfo_csky[i].REG_WideLine == 1 ? chipMinorFeatures1_WIDE_LINE : 0) |
				(gChipInfo_csky[i].REG_BugFixes6 == 1 ? chipMinorFeatures1_BUG_FIXES6 : 0) |
				(gChipInfo_csky[i].REG_FcFlushStall == 1 ? chipMinorFeatures1_FC_FLUSH_STALL : 0);

			uint32_t features2 = (gChipInfo_csky[i].REG_LineLoop == 1 ? chipMinorFeatures2_LINE_LOOP : 0) |
				(gChipInfo_csky[i].REG_LogicOp == 1 ? chipMinorFeatures2_LOGIC_OP : 0) |
				(gChipInfo_csky[i].REG_SeamlessCubeMap == 1 ? chipMinorFeatures2_SEAMLESS_CUBE_MAP : 0) |
				(gChipInfo_csky[i].REG_SuperTiledTexture == 1 ? chipMinorFeatures2_SUPERTILED_TEXTURE : 0) |
				(gChipInfo_csky[i].REG_LinearPE == 1 ? chipMinorFeatures2_LINEAR_PE : 0) |
				(gChipInfo_csky[i].REG_RectPrimitive == 1 ? chipMinorFeatures2_RECT_PRIMITIVE : 0) |
				(gChipInfo_csky[i].REG_Composition == 1 ? chipMinorFeatures2_COMPOSITION : 0) |
				(gChipInfo_csky[i].REG_CorrectAutoDisableCountWidth == 1 ? chipMinorFeatures2_CORRECT_AUTO_DISABLE_COUNT : 0) |
				(gChipInfo_csky[i].REG_PESwizzle == 1 ? chipMinorFeatures2_PE_SWIZZLE : 0) |
				(gChipInfo_csky[i].REG_EndEvent == 1 ? chipMinorFeatures2_END_EVENT : 0) |
				(gChipInfo_csky[i].REG_S1S8 == 1 ? chipMinorFeatures2_S1S8 : 0) |
				(gChipInfo_csky[i].REG_Halti1 == 1 ? chipMinorFeatures2_HALTI1 : 0) |
				(gChipInfo_csky[i].REG_RGB888 == 1 ? chipMinorFeatures2_RGB888 : 0) |
				(gChipInfo_csky[i].REG_TX_YUVAssembler == 1 ? chipMinorFeatures2_TX__YUV_ASSEMBLER : 0) |
				(gChipInfo_csky[i].REG_DynamicFrequencyScaling == 1 ? chipMinorFeatures2_DYNAMIC_FREQUENCY_SCALING : 0) |
				(gChipInfo_csky[i].REG_TXFilter == 1 ? chipMinorFeatures2_TX_FILTER : 0) |
				(gChipInfo_csky[i].REG_FullDirectFB == 1 ? chipMinorFeatures2_FULL_DIRECTFB : 0) |
				(gChipInfo_csky[i].REG_OnePass2DFilter == 1 ? chipMinorFeatures2_2D_TILING : 0) |
				(gChipInfo_csky[i].REG_ThreadWalkerInPS == 1 ? chipMinorFeatures2_THREAD_WALKER_IN_PS : 0) |
				(gChipInfo_csky[i].REG_TileFiller == 1 ? chipMinorFeatures2_TILE_FILLER : 0) |
				(gChipInfo_csky[i].REG_YUVStandard == 1 ? chipMinorFeatures2_YUV_STANDARD : 0) |
				(gChipInfo_csky[i].REG_MultiSourceBlt == 1 ? chipMinorFeatures2_2D_MULTI_SOURCE_BLIT : 0) |
				(gChipInfo_csky[i].REG_YUVConversion == 1 ? chipMinorFeatures2_YUV_CONVERSION : 0) |
				(gChipInfo_csky[i].REG_FlushFixed2D == 1 ? chipMinorFeatures2_FLUSH_FIXED_2D : 0) |
				(gChipInfo_csky[i].REG_Interleaver == 1 ? chipMinorFeatures2_INTERLEAVER : 0) |
				(gChipInfo_csky[i].REG_MixedStreams == 1 ? chipMinorFeatures2_MIXED_STREAMS : 0) |
				(gChipInfo_csky[i].REG_L2CacheFor2D420 == 1 ? chipMinorFeatures2_2D_420_L2CACHE : 0) |
				(gChipInfo_csky[i].REG_BugFixes7 == 1 ? chipMinorFeatures2_BUG_FIXES7 : 0) |
				(gChipInfo_csky[i].REG_NoIndexPattern == 1 ? chipMinorFeatures2_2D_NO_INDEX8_BRUSH : 0) |
				(gChipInfo_csky[i].REG_TextureTileStatus == 1 ? chipMinorFeatures2_TEXTURE_TILED_READ : 0) |
				(gChipInfo_csky[i].REG_DecompressZ16 == 1 ? chipMinorFeatures2_DECOMPRESS_Z16 : 0) |
				(gChipInfo_csky[i].REG_BugFixes8 == 1 ? chipMinorFeatures2_BUG_FIXES8 : 0);

			uint32_t features3 = (gChipInfo_csky[i].REG_DERotationStallFix == 1 ? chipMinorFeatures3_ROTATION_STALL_FIX : 0) |
				(gChipInfo_csky[i].REG_OclOnly == 1 ? chipMinorFeatures3_OCL_ONLY : 0) |
#ifdef ESTIMATED_VALUE
				(gChipInfo_csky[i].REG_NewFeatures0 == 1 ? chipMinorFeatures3_2D_MULTI_SOURCE_BLT_EX : 0) |
#endif
				(gChipInfo_csky[i].REG_InstructionCache == 1 ? chipMinorFeatures3_INSTRUCTION_CACHE : 0) |
				(gChipInfo_csky[i].REG_GeometryShader == 1 ? chipMinorFeatures3_GEOMETRY_SHADER : 0) |
				(gChipInfo_csky[i].REG_TexCompressionSupertiled == 1 ? chipMinorFeatures3_TEX_COMPRESSION_SUPERTILED : 0) |
				(gChipInfo_csky[i].REG_Generics == 1 ? chipMinorFeatures3_GENERICS : 0) |
				(gChipInfo_csky[i].REG_BugFixes9 == 1 ? chipMinorFeatures3_BUG_FIXES9 : 0) |
				(gChipInfo_csky[i].REG_FastMSAA == 1 ? chipMinorFeatures3_FAST_MSAA : 0) |
				(gChipInfo_csky[i].REG_WClip == 1 ? chipMinorFeatures3_WCLIP : 0) |
				(gChipInfo_csky[i].REG_BugFixes10 == 1 ? chipMinorFeatures3_BUG_FIXES10 : 0) |
				(gChipInfo_csky[i].REG_UnifiedSamplers == 1 ? chipMinorFeatures3_UNIFIED_SAMPLERS : 0) |
				(gChipInfo_csky[i].REG_BugFixes11 == 1 ? chipMinorFeatures3_BUG_FIXES11 : 0) |
				(gChipInfo_csky[i].REG_PerformanceCounters == 1 ? chipMinorFeatures3_PERFORMANCE_COUNTERS : 0) |
				(gChipInfo_csky[i].REG_ExtraShaderInstructions2 == 1 ? chipMinorFeatures3_HAS_FAST_TRANSCENDENTALS : 0) |
				(gChipInfo_csky[i].REG_BugFixes12 == 1 ? chipMinorFeatures3_BUG_FIXES12 : 0) |
				(gChipInfo_csky[i].REG_BugFixes13 == 1 ? chipMinorFeatures3_BUG_FIXES13 : 0) |
				(gChipInfo_csky[i].REG_DEEnhancements1 == 1 ? chipMinorFeatures3_DE_ENHANCEMENTS1 : 0) |
				(gChipInfo_csky[i].REG_ACE == 1 ? chipMinorFeatures3_ACE : 0) |
				(gChipInfo_csky[i].REG_TXEnhancements1 == 1 ? chipMinorFeatures3_TX_ENHANCEMENTS1 : 0) |
				(gChipInfo_csky[i].REG_SHEnhancements1 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS1 : 0) |
				(gChipInfo_csky[i].REG_SHEnhancements2 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS2 : 0) |
				(gChipInfo_csky[i].REG_PEEnhancements1 == 1 ? chipMinorFeatures3_PE_ENHANCEMENTS1 : 0) |
				(gChipInfo_csky[i].REG_DEEnhancements2 == 1 ? chipMinorFeatures3_2D_FC_SOURCE : 0) |
				(gChipInfo_csky[i].REG_BugFixes14 == 1 ? chipMinorFeatures3_BUG_FIXES_14 : 0) |
				(gChipInfo_csky[i].REG_PowerOptimizations0 == 1 ? chipMinorFeatures3_POWER_OPTIMIZATIONS_0 : 0) |
				(gChipInfo_csky[i].REG_NewHZ == 1 ? chipMinorFeatures3_NEW_HZ : 0) |
				(gChipInfo_csky[i].REG_BugFixes15 == 1 ? chipMinorFeatures3_PE_DITHER_FIX : 0) |
				(gChipInfo_csky[i].REG_DEEnhancements3 == 1 ? chipMinorFeatures3_DE_ENHANCEMENTS3 : 0) |
				(gChipInfo_csky[i].REG_SHEnhancements3 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS3 : 0) |
				(gChipInfo_csky[i].REG_SHEnhancements4 == 1 ? chipMinorFeatures3_SH_ENHANCEMENTS4 : 0) |
				(gChipInfo_csky[i].REG_TXEnhancements2 == 1 ? chipMinorFeatures3_TX_ENHANCEMENTS2 : 0);

			uint32_t features4 = (gChipInfo_csky[i].REG_FEEnhancements1 == 1 ? chipMinorFeatures4_FE_ENHANCEMENTS1 : 0) |
				(gChipInfo_csky[i].REG_PEEnhancements2 == 1 ? chipMinorFeatures4_PE_ENHANCEMENTS2 : 0) |
				(gChipInfo_csky[i].REG_PAEnhancements1 == 1 ? chipMinorFeatures4_FRUSTUM_CLIP_FIX : 0) |
				(gChipInfo_csky[i].REG_DENoGamma == 1 ? chipMinorFeatures4_DE_NO_GAMMA : 0) |
				(gChipInfo_csky[i].REG_PAEnhancements2 == 1 ? chipMinorFeatures4_PA_ENHANCEMENTS_2 : 0) |
				(gChipInfo_csky[i].REG_DEEnhancements4 == 1 ? chipMinorFeatures4_2D_GAMMA : 0) |
				(gChipInfo_csky[i].REG_PEEnhancements3 == 1 ? chipMinorFeatures4_SINGLE_BUFFER : 0) |
				(gChipInfo_csky[i].REG_HIEnhancements1 == 1 ? chipMinorFeatures4_HI_ENHANCEMENTS_1 : 0) |
				(gChipInfo_csky[i].REG_TXEnhancements3 == 1 ? chipMinorFeatures4_TX_ENHANCEMENTS_3 : 0) |
				(gChipInfo_csky[i].REG_SHEnhancements5 == 1 ? chipMinorFeatures4_SH_ENHANCEMENTS_5 : 0) |
				(gChipInfo_csky[i].REG_FEEnhancements2 == 1 ? chipMinorFeatures4_FE_ENHANCEMENTS_2 : 0) |
				(gChipInfo_csky[i].REG_BugFixes16 == 1 ? chipMinorFeatures4_TX_LERP_PRECISION_FIX : 0) |
				(gChipInfo_csky[i].REG_DEEnhancements5 == 1 ? chipMinorFeatures4_2D_COLOR_SPACE_CONVERSION : 0) |
				(gChipInfo_csky[i].REG_TXEnhancements4 == 1 ? chipMinorFeatures4_TEXTURE_ASTC : 0) |
				(gChipInfo_csky[i].REG_PEEnhancements4 == 1 ? chipMinorFeatures4_PE_ENHANCEMENTS_4 : 0) |
				(gChipInfo_csky[i].REG_MCEnhancements1 == 1 ? chipMinorFeatures4_MC_ENHANCEMENTS_1 : 0) |
				(gChipInfo_csky[i].REG_Halti2 == 1 ? chipMinorFeatures4_HALTI2 : 0) |
				(gChipInfo_csky[i].REG_DEMirrorRotate == 1 ? chipMinorFeatures4_2D_MIRROR_EXTENSION : 0) |
				(gChipInfo_csky[i].REG_SmallMSAA == 1 ? chipMinorFeatures4_SMALL_MSAA : 0) |
				(gChipInfo_csky[i].REG_BugFixes17 == 1 ? chipMinorFeatures4_BUG_FIXES_17 : 0) |
				(gChipInfo_csky[i].REG_Rasterizer2 == 1 ? chipMinorFeatures4_NEW_RA : 0) |
				(gChipInfo_csky[i].REG_DualPipeOPF == 1 ? chipMinorFeatures4_2D_OPF_YUV_OUTPUT : 0) |
				(gChipInfo_csky[i].REG_MultiSrcV2 == 1 ? chipMinorFeatures4_2D_MULTI_SOURCE_BLT_EX2 : 0) |
				(gChipInfo_csky[i].REG_CSCV2 == 1 ? chipMinorFeatures4_NO_USER_CSC : 0) |
				(gChipInfo_csky[i].REG_PAEnhancements3 == 1 ? chipMinorFeatures4_ZFIXES : 0) |
				(gChipInfo_csky[i].REG_BugFixes18 == 1 ? chipMinorFeatures4_BUG_FIXES18 : 0) |
				(gChipInfo_csky[i].REG_Compression2D == 1 ? chipMinorFeatures4_2D_COMPRESSION : 0) |
				(gChipInfo_csky[i].REG_Probe == 1 ? chipMinorFeatures4_PROBE : 0) |
				(gChipInfo_csky[i].REG_MediumPrecision == 1 ? chipMinorFeatures4_MEDIUM_PRECISION : 0) |
				(gChipInfo_csky[i].REG_DESupertile == 1 ? chipMinorFeatures4_2D_SUPER_TILE_VERSION : 0) |
				(gChipInfo_csky[i].REG_BugFixes19 == 1 ? chipMinorFeatures4_BUG_FIXES19 : 0) |
				(gChipInfo_csky[i].REG_SHEnhancements6 == 1 ? chipMinorFeatures4_SH_ENHANCEMENTS6 : 0);

			uint32_t features5 = (gChipInfo_csky[i].REG_SHEnhancements7 == 1 ? chipMinorFeatures5_SH_ENHANCEMENTS7 : 0) |
				(gChipInfo_csky[i].REG_BugFixes20 == 1 ? chipMinorFeatures5_BUG_FIXES20 : 0) |
				(gChipInfo_csky[i].REG_DEAddress40 == 1 ? chipMinorFeatures5_DE_ADDRESS_40 : 0) |
				(gChipInfo_csky[i].REG_MiniMMUFix == 1 ? chipMinorFeatures5_MINI_MMU_FIX : 0) |
				(gChipInfo_csky[i].REG_EEZ == 1 ? chipMinorFeatures5_EEZ : 0) |
				(gChipInfo_csky[i].REG_BugFixes21 == 1 ? chipMinorFeatures5_BUG_FIXES21 : 0) |
				(gChipInfo_csky[i].REG_ExtraVgCaps == 1 ? chipMinorFeatures5_EXTRA_VG_CAPS : 0) |
				(gChipInfo_csky[i].REG_MultiSrcV15 == 1 ? chipMinorFeatures5_MULTI_SRC_V15 : 0) |
				(gChipInfo_csky[i].REG_BugFixes22 == 1 ? chipMinorFeatures5_BUG_FIXES22 : 0) |
				(gChipInfo_csky[i].REG_Halti3 == 1 ? chipMinorFeatures5_HALTI3 : 0) |
				(gChipInfo_csky[i].REG_TessellationShaders == 1 ? chipMinorFeatures5_TESSELATION_SHADERS : 0) |
				(gChipInfo_csky[i].REG_OnePass2DFilter == 1 ? chipMinorFeatures5_2D_ONE_PASS_FILTER_TAP : 0) |
				(gChipInfo_csky[i].REG_MultiSrcV2StrQuad == 1 ? chipMinorFeatures5_MULTI_SRC_V2_STR_QUAD : 0) |
				(gChipInfo_csky[i].REG_SeperateSRCAndDstCache == 1 ? chipMinorFeatures5_SEPARATE_SRC_DST : 0) |
				(gChipInfo_csky[i].REG_Halti4 == 1 ? chipMinorFeatures5_HALTI4 : 0) |
				(gChipInfo_csky[i].REG_RAWriteDepth == 1 ? chipMinorFeatures5_RA_WRITE_DEPTH : 0) |
				(gChipInfo_csky[i].REG_AndroidOnly == 1 ? chipMinorFeatures5_ANDROID_ONLY : 0) |
				(gChipInfo_csky[i].REG_HasChipProductReg == 1 ? chipMinorFeatures5_HAS_PRODUCTID : 0) |
				(gChipInfo_csky[i].REG_TXSupportDEC == 1 ? chipMinorFeatures5_TX_SUPPORT_DEC : 0) |
				(gChipInfo_csky[i].REG_S8MSAACompression == 1 ? chipMinorFeatures5_S8_MSAA_COMPRESSION : 0) |
				(gChipInfo_csky[i].REG_BugFixesIn544 == 1 ? chipMinorFeatures5_PE_DITHER_FIX2 : 0) |
				(gChipInfo_csky[i].REG_L2CacheRemove == 1 ? chipMinorFeatures5_L2_CACHE_REMOVE : 0) |
				(gChipInfo_csky[i].REG_FEAllowRndVtxCnt == 1 ? chipMinorFeatures5_FE_ALLOW_RND_VTX_CNT : 0) |
				(gChipInfo_csky[i].REG_CubeMapFL28 == 1 ? chipMinorFeatures5_CUBE_MAP_FL28 : 0) |
				(gChipInfo_csky[i].REG_TX6bitFrac == 1 ? chipMinorFeatures5_TX_6BIT_FRAC : 0) |
				(gChipInfo_csky[i].REG_FEAllowStallPrefetchEng == 1 ? chipMinorFeatures5_FE_ALLOW_STALL_PREFETCH_ENG : 0) |
				(gChipInfo_csky[i].REG_ThirdPartyCompression == 1 ? chipMinorFeatures5_THIRD_PARTY_COMPRESSION : 0) |
				(gChipInfo_csky[i].REG_RSS8 == 1 ? chipMinorFeatures5_RS_DEPTHSTENCIL_NATIVE_SUPPORT : 0) |
				(gChipInfo_csky[i].REG_MSAACoherencyCheck == 1 ? chipMinorFeatures5_V2_MSAA_COMP_FIX : 0) |
				(gChipInfo_csky[i].REG_Halti5 == 1 ? chipMinorFeatures5_HALTI5 : 0) |
				(gChipInfo_csky[i].REG_Evis == 1 ? chipMinorFeatures5_EVIS : 0) |
				(gChipInfo_csky[i].REG_BltEngine == 1 ? chipMinorFeatures5_BLT_ENGINE : 0);

			uint32_t features6 = (gChipInfo_csky[i].REG_BugFixes23 == 1 ? chipMinorFeatures6_BUG_FIXES_23 : 0) |
				(gChipInfo_csky[i].REG_BugFixes24 == 1 ? chipMinorFeatures6_BUG_FIXES_24 : 0) |
				(gChipInfo_csky[i].REG_DEC == 1 ? chipMinorFeatures6_DEC : 0) |
				(gChipInfo_csky[i].REG_VSTileNV12 == 1 ? chipMinorFeatures6_VS_TILE_NV12 : 0) |
				(gChipInfo_csky[i].REG_VSTileNV12_10BIT == 1 ? chipMinorFeatures6_VS_TILE_NV12_10BIT : 0) |
				//(gChipInfo_csky[i].REG_DisableVIP == 1 ?  : 0) |
				(gChipInfo_csky[i].RenderTarget8 == 1 ? chipMinorFeatures6_RENDER_TARGET_8 : 0) |
				(gChipInfo_csky[i].TxLodFlowCorrection == 1 ? chipMinorFeatures6_TEX_LOD_FLOW_CORR : 0) |
				(gChipInfo_csky[i].FaceLod == 1 ? chipMinorFeatures6_FACE_LOD : 0) |
				(gChipInfo_csky[i].MultiCoreSemaphoreStallV2 == 1 ? chipMinorFeatures6_MULTI_CORE_SEMAPHORE_STALL_V2 : 0) |
				(gChipInfo_csky[i].VMSAA == 1 ? chipMinorFeatures6_VMSAA : 0) |
				(gChipInfo_csky[i].ChipEnableLink == 1 ? chipMinorFeatures6_CHIP_ENABLE_LINK : 0) |
				(gChipInfo_csky[i].MULTI_SRC_BLT_1_5_ENHANCEMENT == 1 ? chipMinorFeatures6_MULTI_SRC_BLT_1_5_ENHANCEMENT : 0) |
				(gChipInfo_csky[i].MULTI_SRC_BLT_BILINEAR_FILTER == 1 ? chipMinorFeatures6_MULTI_SRC_BLT_BILINEAR_FILTER : 0) |
				(gChipInfo_csky[i].RA_HZEZ_CLOCK_CONTROL == 1 ? chipMinorFeatures6_RA_HZEZ_CLOCK_CONTROL : 0) |
				(gChipInfo_csky[i].CACHE128B256BPERLINE == 1 ? chipMinorFeatures6_CACHE128B256BPERLINE : 0) |
				(gChipInfo_csky[i].V4Compression == 1 ? chipMinorFeatures6_V4_COMPRESSION : 0) |
				//(gChipInfo_csky[i].DE2D_MAJOR_SUPER_TILE == 1 ?  : 0) | //?
				(gChipInfo_csky[i].PE2D_MAJOR_SUPER_TILE == 1 ? chipMinorFeatures6_PE2D_MAJOR_SUPER_TILE : 0) |
				(gChipInfo_csky[i].PE_32BPC_COLORMASK_FIX == 1 ? chipMinorFeatures6_PE_32BPC_COLORMASK_FIX : 0) |
				(gChipInfo_csky[i].ALPHA_BLENDING_OPT == 1 ? chipMinorFeatures6_ALPHA_BLENDING_OPT : 0) |
				(gChipInfo_csky[i].NEW_GPIPE == 1 ? chipMinorFeatures6_NEW_GPIPE : 0) |
				(gChipInfo_csky[i].PIPELINE_32_ATTRIBUTES == 1 ? chipMinorFeatures6_PIPELINE_32_ATTRIBUTES : 0) |
				(gChipInfo_csky[i].MSAA_SHADING == 1 ? chipMinorFeatures6_MSAA_SHADING : 0) |
				(gChipInfo_csky[i].NO_ANISTRO_FILTER == 1 ? chipMinorFeatures6_NO_ANISTRO_FILTER : 0) |
				(gChipInfo_csky[i].NO_ASTC == 1 ? chipMinorFeatures6_NO_ASTC : 0) |
				(gChipInfo_csky[i].NO_DXT == 1 ? chipMinorFeatures6_NO_DXT : 0) |
				(gChipInfo_csky[i].HWTFB == 1 ? chipMinorFeatures6_HWTFB : 0) |
				(gChipInfo_csky[i].RA_DEPTH_WRITE_MSAA1X_FIX == 1 ? chipMinorFeatures6_RA_DEPTH_WRITE_MSAA1X_FIX : 0) |
				(gChipInfo_csky[i].EZHZ_CLOCKGATE_FIX == 1 ? chipMinorFeatures6_EZHZ_CLOCKGATE_FIX : 0) |
				(gChipInfo_csky[i].SH_SNAP2PAGE_FIX == 1 ? chipMinorFeatures6_SH_SNAP2PAGE_FIX : 0) |
				(gChipInfo_csky[i].SH_HALFDEPENDENCY_FIX == 1 ? chipMinorFeatures6_SH_HALFDEPENDENCY_FIX : 0) |
				(gChipInfo_csky[i].USC_MCFILL_FIX == 1 ? chipMinorFeatures6_USC_MCFILL_FIX : 0) |
				(gChipInfo_csky[i].TPG_TCPERF_FIX == 1 ? chipMinorFeatures6_TPG_TCPERF_FIX : 0);

			uint32_t features7 = (gChipInfo_csky[i].USC_MDFIFO_OVERFLOW_FIX == 1 ? chipMinorFeatures7_USC_MDFIFO_OVERFLOW_FIX : 0) |
				(gChipInfo_csky[i].SH_TEXLD_BARRIER_IN_CS_FIX == 1 ? chipMinorFeatures7_SH_TEXLD_BARRIER_IN_CS_FIX : 0) |
				(gChipInfo_csky[i].RS_NEW_BASEADDR == 1 ? chipMinorFeatures7_RS_NEW_BASEADDR : 0) |
				(gChipInfo_csky[i].PE_8bpp_DUALPIPE_FIX == 1 ? chipMinorFeatures7_PE_8BPP_DUALPIPE_FIX : 0) |
				(gChipInfo_csky[i].SH_ADVANCED_INSTR == 1 ? chipMinorFeatures7_SH_ADVANCED_INSTR : 0) |
				(gChipInfo_csky[i].SH_FLAT_INTERPOLATION_DUAL16_FIX == 1 ? chipMinorFeatures7_SH_FLAT_INTERPOLATION_DUAL16_FIX : 0) |
				(gChipInfo_csky[i].USC_CONTINUOUS_FLUS_FIX == 1 ? chipMinorFeatures7_USC_CONTINUOUS_FLUS_FIX : 0) |
				(gChipInfo_csky[i].SH_SUPPORT_V4 == 1 ? chipMinorFeatures7_SH_SUPPORT_V4 : 0) |
				(gChipInfo_csky[i].SH_SUPPORT_ALPHA_KILL == 1 ? chipMinorFeatures7_SH_SUPPORT_ALPHA_KILL : 0) |
				(gChipInfo_csky[i].PE_NO_ALPHA_TEST == 1 ? chipMinorFeatures7_PE_NO_ALPHA_TEST : 0) |
				(gChipInfo_csky[i].TX_LOD_NEAREST_SELECT == 1 ? chipMinorFeatures7_TX_LOD_NEAREST_SELECT : 0) |
				(gChipInfo_csky[i].SH_FIX_LDEXP == 1 ? chipMinorFeatures7_SH_FIX_LDEXP : 0) |
				(gChipInfo_csky[i].SUPPORT_MOVAI == 1 ? chipMinorFeatures7_SUPPORT_MOVAI : 0) |
				(gChipInfo_csky[i].SH_SNAP2PAGE_MAXPAGES_FIX == 1 ? chipMinorFeatures7_SH_SNAP2PAGE_MAXPAGES_FIX : 0) |
				(gChipInfo_csky[i].PE_RGBA16I_FIX == 1 ? chipMinorFeatures7_PE_RGBA16I_FIX : 0) |
				(gChipInfo_csky[i].BLT_8bpp_256TILE_FC_FIX == 1 ? chipMinorFeatures7_BLT_8bpp_256TILE_FC_FIX : 0) |
				(gChipInfo_csky[i].PE_64bit_FENCE_FIX == 1 ? chipMinorFeatures7_PE_64BIT_FENCE_FIX : 0) |
				(gChipInfo_csky[i].USC_FULL_CACHE_FIX == 1 ? chipMinorFeatures7_USC_FULL_CACHE_FIX : 0) |
				(gChipInfo_csky[i].TX_YUV_ASSEMBLER_10BIT == 1 ? chipMinorFeatures7_TX_YUV_ASSEMBLER_10BIT : 0) |
				(gChipInfo_csky[i].FE_32bit_INDEX_FIX == 1 ? chipMinorFeatures7_FE_32BIT_INDEX_FIX : 0) |
				(gChipInfo_csky[i].BLT_64bpp_MASKED_CLEAR_FIX == 1 ? chipMinorFeatures7_BLT_64BPP_MASKED_CLEAR_FIX : 0) |
				(gChipInfo_csky[i].SECURITY == 1 ? chipMinorFeatures7_BIT_SECURITY : 0) |
				(gChipInfo_csky[i].ROBUSTNESS == 1 ? chipMinorFeatures7_BIT_ROBUSTNESS : 0) |
				(gChipInfo_csky[i].USC_ATOMIC_FIX == 1 ? chipMinorFeatures7_USC_ATOMIC_FIX : 0) |
				(gChipInfo_csky[i].SH_PSO_MSAA1x_FIX == 1 ? chipMinorFeatures7_SH_PSO_MSAA1x_FIX : 0) |
				(gChipInfo_csky[i].USC_VX_PERF_FIX == 1 ? chipMinorFeatures7_BIT_USC_VX_PERF_FIX : 0) |
				(gChipInfo_csky[i].EVIS_NO_ABSDIFF == 1 ? chipMinorFeatures7_EVIS_NO_ABSDIFF : 0) |
				(gChipInfo_csky[i].EVIS_NO_BITREPLACE == 1 ? chipMinorFeatures7_EVIS_NO_BITREPLACE : 0) |
				(gChipInfo_csky[i].EVIS_NO_BOXFILTER == 1 ? chipMinorFeatures7_EVIS_NO_BOXFILTER : 0) |
				(gChipInfo_csky[i].EVIS_NO_CORDIAC == 1 ? chipMinorFeatures7_EVIS_NO_CORDIAC : 0) |
				(gChipInfo_csky[i].EVIS_NO_DP32 == 1 ? chipMinorFeatures7_EVIS_NO_DP32 : 0) |
				(gChipInfo_csky[i].EVIS_NO_FILTER == 1 ? chipMinorFeatures7_EVIS_NO_FILTER : 0);

			uint32_t features8 =
				(gChipInfo_csky[i].EVIS_NO_IADD == 1 ? chipMinorFeatures8_EVIS_NO_IADD : 0) |
				(gChipInfo_csky[i].EVIS_NO_SELECTADD == 1 ? chipMinorFeatures8_EVIS_NO_SELECTADD : 0) |
				(gChipInfo_csky[i].EVIS_LERP_7OUTPUT == 1 ? chipMinorFeatures8_EVIS_LERP_7OUTPUT : 0) |
				(gChipInfo_csky[i].EVIS_ACCSQ_8OUTPUT == 1 ? chipMinorFeatures8_EVIS_ACCSQ_8OUTPUT : 0) |
				(gChipInfo_csky[i].USC_GOS_ADDR_FIX == 1 ? chipMinorFeatures8_USC_GOS_ADDR_FIX : 0) |
				(gChipInfo_csky[i].TX_8bit_UVFrac == 1 ? chipMinorFeatures8_TX_8BIT_UVFRAC : 0) |
				(gChipInfo_csky[i].TX_DESC_CACHE_CLOCKGATE_FIX == 1 ? chipMinorFeatures8_TX_DESC_CACHE_CLOCKGATE_FIX : 0) |
				(gChipInfo_csky[i].RSBLT_MSAA_DECOMPRESSION == 1 ? chipMinorFeatures8_RSBLT_MSAA_DECOMPRESSION : 0) |
				(gChipInfo_csky[i].TX_INTEGER_COORDINATE == 1 ? chipMinorFeatures8_TX_INTEGER_COORDINATE : 0) |
				(gChipInfo_csky[i].DRAWID == 1 ? chipMinorFeatures8_DRAWID : 0) |
				(gChipInfo_csky[i].PSIO_SAMPLEMASK_IN_R0ZW_FIX == 1 ? chipMinorFeatures8_PSIO_SAMPLEMASK_IN_R0ZW_FIX : 0) |
				(gChipInfo_csky[i].TX_INTEGER_COORDINATE_V2 == 1 ? chipMinorFeatures8_TX_INTEGER_COORDINATE_V2 : 0) |
				(gChipInfo_csky[i].MULTI_CORE_BLOCK_SET_CONFIG == 1 ? chipMinorFeatures8_MULTI_CORE_BLOCK_SET_CONFIG : 0) |
				(gChipInfo_csky[i].VG_RESOLVE_ENGINE == 1 ? chipMinorFeatures8_VG_RESOLVE_ENGINE : 0) |
				(gChipInfo_csky[i].VG_PE_COLOR_KEY == 1 ? chipMinorFeatures8_VG_PE_COLOR_KEY : 0) |
				(gChipInfo_csky[i].VG_IM_INDEX_FORMAT == 1 ? chipMinorFeatures8_VG_IM_INDEX_FORMAT : 0) |
				(gChipInfo_csky[i].SNAPPAGE_CMD == 1 ? chipMinorFeatures8_SNAPPAGE_CMD : 0) |
				(gChipInfo_csky[i].SH_NO_INDEX_CONST_ON_A0 == 1 ? chipMinorFeatures8_SH_NO_INDEX_CONST_ON_A0 : 0) |
				(gChipInfo_csky[i].SH_NO_ONECONST_LIMIT == 1 ? chipMinorFeatures8_SH_NO_ONECONST_LIMIT : 0) |
				(gChipInfo_csky[i].SH_IMG_LDST_ON_TEMP == 1 ? chipMinorFeatures8_SH_IMG_LDST_ON_TEMP : 0) |
				(gChipInfo_csky[i].COMPUTE_ONLY == 1 ? chipMinorFeatures8_COMPUTE_ONLY : 0) |
				(gChipInfo_csky[i].SH_IMG_LDST_CLAMP == 1 ? chipMinorFeatures8_SH_IMG_LDST_CLAMP : 0) |
				(gChipInfo_csky[i].SH_ICACHE_ALLOC_COUNT_FIX == 1 ? chipMinorFeatures8_SH_ICACHE_ALLOC_COUNT_FIX : 0) |
				(gChipInfo_csky[i].SH_ICACHE_PREFETCH == 1 ? chipMinorFeatures8_SH_ICACHE_PREFETCH : 0) |
				(gChipInfo_csky[i].PE2D_SEPARATE_CACHE == 1 ? chipMinorFeatures8_PE2D_SEPARATE_CACHE : 0) |
				(gChipInfo_csky[i].VG_AYUV_INPUT_OUTPUT == 1 ? chipMinorFeatures8_VG_AYUV_INPUT_OUTPUT : 0) |
				(gChipInfo_csky[i].VG_DOUBLE_IMAGE == 1 ? chipMinorFeatures8_VG_DOUBLE_IMAGE : 0) |
				(gChipInfo_csky[i].VG_RECTANGLE_STRIPE_MODE == 1 ? chipMinorFeatures8_VG_RECTANGLE_STRIPE_MODE : 0) |
				(gChipInfo_csky[i].VG_MMU == 1 ? chipMinorFeatures8_VG_MMU : 0) |
				(gChipInfo_csky[i].VG_IM_FILTER == 1 ? chipMinorFeatures8_VG_IM_FILTER : 0) |
				(gChipInfo_csky[i].VG_IM_YUV_PACKET == 1 ? chipMinorFeatures8_VG_IM_YUV_PACKET : 0) |
				(gChipInfo_csky[i].VG_IM_YUV_PLANAR == 1 ? chipMinorFeatures8_VG_IM_YUV_PLANAR : 0);

			uint32_t features9 =
				(gChipInfo_csky[i].VG_PE_YUV_PACKET == 1 ? chipMinorFeatures9_VG_PE_YUV_PACKET : 0) |
				(gChipInfo_csky[i].VG_COLOR_PRECISION_8_BIT == 1 ? chipMinorFeatures9_VG_COLOR_PRECISION_8_BIT : 0) |
				(gChipInfo_csky[i].PE_MSAA_OQ_FIX == 1 ? chipMinorFeatures9_PE_MSAA_OQ_FIX : 0) |
				(gChipInfo_csky[i].PSIO_MSAA_CL_FIX == 1 ? chipMinorFeatures9_PSIO_MSAA_CL_FIX : 0) |
				(gChipInfo_csky[i].USC_DEFER_FILL_FIX == 1 ? chipMinorFeatures9_USC_DEFER_FILL_FIX : 0) |
				(gChipInfo_csky[i].SH_CLOCK_GATE_FIX == 1 ? chipMinorFeatures9_SH_CLOCK_GATE_FIX : 0) |
				(gChipInfo_csky[i].FE_NEED_DUMMYDRAW == 1 ? chipMinorFeatures9_FE_NEED_DUMMYDRAW : 0) |
				(gChipInfo_csky[i].PE2D_LINEAR_YUV420_OUTPUT == 1 ? chipMinorFeatures9_PE2D_LINEAR_YUV420_OUTPUT : 0) |
				(gChipInfo_csky[i].PE2D_LINEAR_YUV420_10BIT == 1 ? chipMinorFeatures9_PE2D_LINEAR_YUV420_10BIT : 0) |
				(gChipInfo_csky[i].MULTI_CLUSTER == 1 ? chipMinorFeatures9_MULTI_CLUSTER : 0) |
				(gChipInfo_csky[i].VG_TS_CULLING == 1 ? chipMinorFeatures9_VG_TS_CULLING : 0) |
				(gChipInfo_csky[i].VG_FP25 == 1 ? chipMinorFeatures9_VG_FP25 : 0) |
				(gChipInfo_csky[i].SH_MULTI_WG_PACK == 1 ? chipMinorFeatures9_SH_MULTI_WG_PACK : 0) |
				(gChipInfo_csky[i].SH_DUAL16_SAMPLEMASK_ZW == 1 ? chipMinorFeatures9_SH_DUAL16_SAMPLEMASK_ZW : 0) |
				(gChipInfo_csky[i].TPG_TRIVIAL_MODE_FIX == 1 ? chipMinorFeatures9_TPG_TRIVIAL_MODE_FIX : 0) |
				(gChipInfo_csky[i].TX_ASTC_MULTISLICE_FIX == 1 ? chipMinorFeatures9_TX_ASTC_MULTISLICE_FIX : 0) |
				(gChipInfo_csky[i].FE_ROBUST_FIX == 1 ? chipMinorFeatures9_FE_ROBUST_FIX : 0) |
				(gChipInfo_csky[i].SH_GPIPE_ACCESS_FULLTEMPS == 1 ? chipMinorFeatures9_SH_GPIPE_ACCESS_FULLTEMPS : 0) |
				(gChipInfo_csky[i].PSIO_INTERLOCK == 1 ? chipMinorFeatures9_PSIO_INTERLOCK : 0) |
				(gChipInfo_csky[i].PA_WIDELINE_FIX == 1 ? chipMinorFeatures9_PA_WIDELINE_FIX : 0) |
				(gChipInfo_csky[i].WIDELINE_HELPER_FIX == 1 ? chipMinorFeatures9_WIDELINE_HELPER_FIX : 0) |
				(gChipInfo_csky[i].G2D_3rd_PARTY_COMPRESSION_1_1 == 1 ? chipMinorFeatures9_G2D_3RD_PARTY_COMPRESSION_1_1 : 0) |
				(gChipInfo_csky[i].TX_FLUSH_L1CACHE == 1 ? chipMinorFeatures9_TX_FLUSH_L1CACHE : 0) |
				(gChipInfo_csky[i].PE_DITHER_FIX2 == 1 ? chipMinorFeatures9_PE_DITHER_FIX2 : 0) |
				(gChipInfo_csky[i].G2D_DEC400 == 1 ? chipMinorFeatures9_G2D_DEC400 : 0) |
				(gChipInfo_csky[i].SH_TEXLD_U_FIX == 1 ? chipMinorFeatures9_SH_TEXLD_U_FIX : 0) |
				(gChipInfo_csky[i].MC_FCCACHE_BYTEMASK == 1 ? chipMinorFeatures9_MC_FCCACHE_BYTEMASK : 0) |
				(gChipInfo_csky[i].SH_MULTI_WG_PACK_FIX == 1 ? chipMinorFeatures9_SH_MULTI_WG_PACK_FIX : 0) |
				(gChipInfo_csky[i].DC_OVERLAY_SCALING == 1 ? chipMinorFeatures9_DC_OVERLAY_SCALING : 0) |
				(gChipInfo_csky[i].DC_SOURCE_ROTATION == 1 ? chipMinorFeatures9_DC_SOURCE_ROTATION : 0) |
				(gChipInfo_csky[i].DC_TILED == 1 ? chipMinorFeatures9_DC_TILED : 0) |
				(gChipInfo_csky[i].DC_YUV_L1 == 1 ? chipMinorFeatures9_DC_YUV_L1 : 0);

			uint32_t features10 =
				(gChipInfo_csky[i].DC_D30_OUTPUT == 1 ? chipMinorFeatures10_DC_D30_OUTPUT : 0) |
				(gChipInfo_csky[i].DC_MMU == 1 ? chipMinorFeatures10_DC_MMU : 0) |
				(gChipInfo_csky[i].DC_COMPRESSION == 1 ? chipMinorFeatures10_DC_COMPRESSION : 0) |
				(gChipInfo_csky[i].DC_QOS == 1 ? chipMinorFeatures10_DC_QOS : 0) |
				(gChipInfo_csky[i].PE_ADVANCE_BLEND_PART0 == 1 ? chipMinorFeatures10_PE_ADVANCE_BLEND_PART0 : 0) |
				(gChipInfo_csky[i].FE_PATCHLIST_FETCH_FIX == 1 ? chipMinorFeatures10_FE_PATCHLIST_FETCH_FIX : 0) |
				(gChipInfo_csky[i].RA_CG_FIX == 1 ? chipMinorFeatures10_RA_CG_FIX : 0) |
				(gChipInfo_csky[i].EVIS_VX2 == 1 ? chipMinorFeatures10_EVIS_VX2 : 0) |
				(gChipInfo_csky[i].NN_FLOAT == 1 ? chipMinorFeatures10_NN_FLOAT : 0) |
				(gChipInfo_csky[i].DEC400 == 1 ? chipMinorFeatures10_DEC400 : 0) |
				(gChipInfo_csky[i].LS_SUPPORT_PERCOMP_DEPENDENCY == 1 ? chipMinorFeatures10_LS_SUPPORT_PERCOMP_DEPENDENCY : 0) |
				(gChipInfo_csky[i].TP_ENGINE == 1 ? chipMinorFeatures10_TP_ENGINE : 0) |
				(gChipInfo_csky[i].MULTI_CORE_BLOCK_SET_CONFIG2 == 1 ? chipMinorFeatures10_MULTI_CORE_BLOCK_SET_CONFIG2 : 0) |
				(gChipInfo_csky[i].PE_VMSAA_COVERAGE_CACHE_FIX == 1 ? chipMinorFeatures10_PE_VMSAA_COVERAGE_CACHE_FIX : 0) |
				(gChipInfo_csky[i].SECURITY_AHB == 1 ? chipMinorFeatures10_SECURITY_AHB : 0) |
				(gChipInfo_csky[i].MULTICORE_SEMAPHORESTALL_V3 == 1 ? chipMinorFeatures10_MULTICORE_SEMAPHORESTALL_V3 : 0) |
				(gChipInfo_csky[i].SMALLBATCH == 1 ? chipMinorFeatures10_SMALLBATCH : 0) |
				(gChipInfo_csky[i].SH_CMPLX == 1 ? chipMinorFeatures10_SH_CMPLX : 0) |
				(gChipInfo_csky[i].SH_IDIV0_SWZL_EHS == 1 ? chipMinorFeatures10_SH_IDIV0_SWZL_EHS : 0) |
				(gChipInfo_csky[i].TX_LERP_LESS_BIT == 1 ? chipMinorFeatures10_TX_LERP_LESS_BIT : 0) |
				(gChipInfo_csky[i].SH_GM_ENDIAN == 1 ? chipMinorFeatures10_SH_GM_ENDIAN : 0) |
				(gChipInfo_csky[i].SH_GM_USC_UNALLOC == 1 ? chipMinorFeatures10_SH_GM_USC_UNALLOC : 0) |
				(gChipInfo_csky[i].SH_END_OF_BB == 1 ? chipMinorFeatures10_SH_END_OF_BB : 0) |
				(gChipInfo_csky[i].VIP_V7 == 1 ? chipMinorFeatures10_VIP_V7 : 0) |
				(gChipInfo_csky[i].TX_BORDER_CLAMP_FIX == 1 ? chipMinorFeatures10_TX_BORDER_CLAMP_FIX : 0) |
				(gChipInfo_csky[i].SH_IMG_LD_LASTPIXEL_FIX == 1 ? chipMinorFeatures10_SH_IMG_LD_LASTPIXEL_FIX : 0) |
				(gChipInfo_csky[i].ASYNC_BLT == 1 ? chipMinorFeatures10_ASYNC_BLT : 0) |
				(gChipInfo_csky[i].ASYNC_FE_FENCE_FIX == 1 ? chipMinorFeatures10_ASYNC_FE_FENCE_FIX : 0) |
				(gChipInfo_csky[i].PSCS_THROTTLE == 1 ? chipMinorFeatures10_PSCS_THROTTLE : 0) |
				(gChipInfo_csky[i].SEPARATE_LS == 1 ? chipMinorFeatures10_SEPARATE_LS : 0) |
				(gChipInfo_csky[i].MCFE == 1 ? chipMinorFeatures10_MCFE : 0) |
				(gChipInfo_csky[i].WIDELINE_TRIANGLE_EMU == 1 ? chipMinorFeatures10_WIDELINE_TRIANGLE_EMU : 0);

			uint32_t features11 =
				(gChipInfo_csky[i].VG_RESOLUTION_8K == 1 ? chipMinorFeatures11_VG_RESOLUTION_8K : 0) |
				(gChipInfo_csky[i].FENCE_32BIT == 1 ? chipMinorFeatures11_FENCE_32BIT : 0) |
				(gChipInfo_csky[i].FENCE_64BIT == 1 ? chipMinorFeatures11_FENCE_64BIT : 0) |
				(gChipInfo_csky[i].NN_INTERLEVE8 == 1 ? chipMinorFeatures11_NN_INTERLEVE8 : 0) |
				(gChipInfo_csky[i].PE_DEPTH_ONLY_OQFIX == 1 ? chipMinorFeatures11_PE_DEPTH_ONLY_OQFIX : 0) |
#if defined EXTENDED_FEATURE_LIST
				(gChipInfo_csky[i].TX_SEAMLESS_CUBE == 1 ? chipMinorFeatures11_TX_SEAMLESS_CUBE : 0) |
				(gChipInfo_csky[i].TX_SNORM_SUPPORT == 1 ? chipMinorFeatures11_TX_SNORM_SUPPORT : 0) |
				(gChipInfo_csky[i].SH_SCATTER_GATHER == 1 ? chipMinorFeatures11_SH_SCATTER_GATHER : 0) |
				(gChipInfo_csky[i].HWMANAGED_LS == 1 ? chipMinorFeatures11_HWMANAGED_LS : 0) |
				(gChipInfo_csky[i].SH_IMAGE_ENABLE_FIX == 1 ? chipMinorFeatures11_SH_IMAGE_ENABLE_FIX : 0) |
				(gChipInfo_csky[i].MSAA_FRAGMENT_OPERATION == 1 ? chipMinorFeatures11_MSAA_FRAGMENT_OPERATION : 0) |
#endif
				(gChipInfo_csky[i].TP_REORDER == 1 ? chipMinorFeatures11_TP_REORDER : 0);

#if defined EXTENDED_FEATURE_LIST
			uint32_t features12 = 0;

			uint32_t features13 =
				(gChipInfo_csky[i].SWTILING_PHASE1 == 1 ? chipMinorFeatures13_SWTILING_PHASE1 : 0) |
				(gChipInfo_csky[i].TP_SIMPLE_INT16 == 1 ? chipMinorFeatures13_TP_SIMPLE_INT16 : 0) |
				(gChipInfo_csky[i].TP_REAL_INT16 == 1 ? chipMinorFeatures13_TP_REAL_INT16 : 0) |
				(gChipInfo_csky[i].TP_ROI_POOLING == 1 ? chipMinorFeatures13_TP_ROI_POOLING : 0) |
				(gChipInfo_csky[i].TP_MAX_POOLING_STRIDE1 == 1 ? chipMinorFeatures13_TP_MAX_POOLING_STRIDE1 : 0) |
				(gChipInfo_csky[i].TP_LRN == 1 ? chipMinorFeatures13_TP_LRN : 0) |
				(gChipInfo_csky[i].TF_QUANTIZATION == 1 ? chipMinorFeatures13_TF_QUANTIZATION : 0);

			uint32_t features14 =
				(gChipInfo_csky[i].NN_ZDP6 == 1 ? chipMinorFeatures14_NN_ZDP6 : 0) |
				(gChipInfo_csky[i].NN_XYDP9 == 1 ? chipMinorFeatures14_NN_XYDP9 : 0) |
				(gChipInfo_csky[i].NN_ZDP3 == 1 ? chipMinorFeatures14_NN_ZDP3 : 0);

			uint32_t features15 = 0;

			uint32_t features16 = 0;

			uint32_t features17 = 0;

			uint32_t features18 = 0;

			uint32_t features19 = 0;

			uint32_t features20 =
				(gChipInfo_csky[i].NN_FP16_ALU == 1 ? chipMinorFeatures20_NN_FP16_ALU : 0) |
				(gChipInfo_csky[i].NN_INT16_ALU == 1 ? chipMinorFeatures20_NN_INT16_ALU : 0) |
				(gChipInfo_csky[i].NN_INT8_SCALE == 1 ? chipMinorFeatures20_NN_INT8_SCALE : 0) |
				(gChipInfo_csky[i].NN_POWER_ISOLATION == 1 ? chipMinorFeatures20_NN_POWER_ISOLATION : 0);
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
			linux_devices[count].minor_features14 = features14;
			linux_devices[count].minor_features15 = features15;
			linux_devices[count].minor_features16 = features16;
			linux_devices[count].minor_features17 = features17;
			linux_devices[count].minor_features18 = features18;
			linux_devices[count].minor_features19 = features19;
			linux_devices[count].minor_features20 = features20;
#endif

#if defined NN_CORE_COUNT
			linux_devices[count].nn_core_count = gChipInfo_csky[i].NNCoreCount;
#endif
			count++;
		}
	}

	ofstream myfile;
	myfile.open("features.txt");

	// Sort entries and write it to console and features.txt
	for (int model = 0; model < 0xA000; model++) {
		for (int revision = 0; revision < 0xA000; revision++) {
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
#if defined NN_CORE_COUNT
					std::cout << "		.nn_core_count = " << linux_devices[i].nn_core_count << ",\n";
#endif
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
					std::cout << "		.minor_features14 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features14 << ",\n";
					std::cout << "		.minor_features15 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features15 << ",\n";
					std::cout << "		.minor_features16 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features16 << ",\n";
					std::cout << "		.minor_features17 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features17 << ",\n";
					std::cout << "		.minor_features18 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features18 << ",\n";
					std::cout << "		.minor_features19 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features19 << ",\n";
					std::cout << "		.minor_features20 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features20 << ",\n";
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
#if defined NN_CORE_COUNT
					myfile << "\t\t.nn_core_count = " << linux_devices[i].nn_core_count << ",\n";
#endif
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
					myfile << "\t\t.minor_features14 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features14 << ",\n";
					myfile << "\t\t.minor_features15 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features15 << ",\n";
					myfile << "\t\t.minor_features16 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features16 << ",\n";
					myfile << "\t\t.minor_features17 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features17 << ",\n";
					myfile << "\t\t.minor_features18 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features18 << ",\n";
					myfile << "\t\t.minor_features19 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features19 << ",\n";
					myfile << "\t\t.minor_features20 = 0x" << std::hex << std::setw(8) << linux_devices[i].minor_features20 << ",\n";
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
