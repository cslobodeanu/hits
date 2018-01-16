#ifndef __MORGANA_BASE_DEVICES_DISPLAY_PIPELINE_STATES_H__
#define __MORGANA_BASE_DEVICES_DISPLAY_PIPELINE_STATES_H__

#include "reflection.h"
#include "../io/debug.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Devices
		{
			namespace Display
			{
				class PipelineState : public MEObject
				{
					__declare_class_abstract(PipelineState, MEObject);
				protected:
					unsigned long hashValue;
					virtual void ComputeHash() { hashValue = 0; }
				public:
					make_enum(CompareFunc,
						Never,
						Less,
						Equal,
						LessEqual,
						Greater,
						NotEqual,
						GreaterEqual,
						Always
					);

					make_enum(StencilAction,
						Keep,
						Zero,
						Replace,
						Increment,
						IncrementWrap,
						Decrement,
						DecrementWrap,
						Invert
					);

					make_enum(BlendFactor,
						Zero,
						One,
						SrcColor,
						OneMinusSrcColor,
						SrcAlpha,
						OneMinusSrcAlpha,
						DstAlpha,
						OneMinusDstAlpha,
						DstColor,
						OneMinusDstColor,
						SrcAlphaSaturate        
					);

					make_enum(RasterCullOp,
						Back,
						Front,
						None
					);

					virtual const bool Equals(const PipelineState* other) const
					{
						if (other == null) return false;
						if (GetClassDesc() != other->GetClassDesc()) return false;
						if (hashValue != other->hashValue) return false;

						return memcmp(this, other, sizeof(*other)) == 0;
					}
				};

				class BlendState : public PipelineState
				{
					__declare_class(BlendState, PipelineState);
					BlendState() : srcFactor(0), dstFactor(0) { NOT_PERMITTED }
				protected:
					virtual void ComputeHash()
					{
						hashValue = (ulong)srcFactor + (ulong)dstFactor;
					}
				public:
					const unsigned short srcFactor, dstFactor;
					BlendState(const unsigned short _srcFactor, const unsigned short _dstFactor) : srcFactor(_srcFactor), dstFactor(_dstFactor) { ComputeHash(); }
					static const BlendState *Additive;
					static const BlendState *Multiply;
					static const BlendState *AlphaBlend;
					static const BlendState *Opaque;
				};

				class DepthState : public PipelineState
				{
					__declare_class(DepthState, PipelineState);
					DepthState() : enabled(false), writeEnabled(false), compareFunc(0) { NOT_PERMITTED }
				protected:
					virtual void ComputeHash()
					{
						hashValue = (ulong)enabled + (ulong)writeEnabled + (ulong)compareFunc;
					}
				public:
					const bool enabled, writeEnabled;
					const unsigned short compareFunc;

					DepthState(const bool _enabled, const bool depthWrite, const unsigned short _compareFunc) : 
						enabled(_enabled), writeEnabled(depthWrite), compareFunc(_compareFunc) { ComputeHash();	}

					static const DepthState *LessEqual;
					static const DepthState *LessEqualNoWrite;
					static const DepthState *Equal;
					static const DepthState *Disabled;
				};

				class StencilState : public PipelineState
				{
					__declare_class(StencilState, PipelineState);
					StencilState() : enabled(false), mask(0xff), func(CompareFunc::Always), ref(0), funcMask(0xff), stencilFail(StencilAction::Keep), depthFail(StencilAction::Keep), depthPass(StencilAction::Keep) { NOT_PERMITTED }

				protected:
					virtual void ComputeHash()
					{
						hashValue = (ulong)enabled + (ulong)mask + (ulong)func + (ulong)ref + (ulong)funcMask + (ulong)stencilFail + (ulong)depthFail + (ulong)depthPass;
					}
				public:
					const bool enabled;
					const uint mask;

					const CompareFunc::Values func;
					const uint ref;
					const uint funcMask;

					const StencilAction::Values stencilFail;
					const StencilAction::Values depthFail;
					const StencilAction::Values depthPass;

					StencilState(const bool _enabled, const uint _mask = 0xff, const CompareFunc::Values _func = CompareFunc::Greater, const uint _ref = 0, const uint _funcMask = 0xff, const StencilAction::Values _stencilFail = StencilAction::Keep, const StencilAction::Values _depthFail = StencilAction::Keep, const StencilAction::Values _depthPass = StencilAction::Keep) :
								enabled(_enabled), mask(_mask), func(_func), ref(_ref), funcMask(_funcMask), stencilFail(_stencilFail), depthFail(_depthFail), depthPass(_depthPass) { ComputeHash(); }

					static const StencilState* Disabled;
				};

				class RasterizerState : public PipelineState
				{
					__declare_class(RasterizerState, PipelineState);
					RasterizerState() : cullFace(0) { NOT_PERMITTED }
				public:
					const unsigned short cullFace;
					RasterizerState(const unsigned short cull) : cullFace(cull) {}

					static const RasterizerState	*CullNone;
					static const RasterizerState	*CullFront;
					static const RasterizerState	*CullBack;
				};
			}
		}
	}
}

#endif